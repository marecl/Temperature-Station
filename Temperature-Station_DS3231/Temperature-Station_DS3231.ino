#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <pgmspace.h>
#include <RtcDS3231.h>
#include <settingsManager.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFiUdp.h>

#define FS_NO_GLOBALS
#include "FS.h"
#include "defs.h"

/* Good when trying different configurations */
#define SETTINGS_FILE "/SET.TXT" //SPIFFS
#define SENSORS_FILE "/DATA/SENSORS.TXT" //SD

/* Webpage */
extern char index_htm[];
extern char index_htm2[];
extern char settings_htm[];
extern char sensors_htm[];
extern char sensors_htm2[];

ESP8266WebServer server(80);
//ESP8266WebServerSecure server(443);
ESP8266HTTPUpdateServer updServer;

OneWire oneWire(OW_PORT);
DallasTemperature sensors(&oneWire);

RtcDS3231<TwoWire> zegar(Wire);
RtcDateTime teraz;

settingsManager settings(SETTINGS_FILE);

void setup() {
  SPIFFS.begin();
  WiFi.disconnect();
  WiFi.persistent(false);
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println(F("\r\n\r\nBooting..."));
  pinMode(SD_D, INPUT);

  if (!digitalRead(SD_D)) {
    if (SD.begin(SD_CS)) Serial.println(F("SD Card initialized"));
    else Serial.println(F("SD Card detected but cannot be initialized!"));
  } else Serial.println(F("No card inserted"));

#ifdef DEBUG_INSECURE
  settings.serialDebug(&Serial);
  //Use this section to overwrite settings while debugging
  settings.configSTA("", "");  // ssid and password
  settings.configAP("TemperatureStation");            // AP ssid
  settings.useDHCP(true);
  settings.name("TemperatureStation");                // Device name
  settings.ntpServer("");           // NTP server (not pool)
  settings.configUser("admin", "admin");              // Login and password
  settings.timezone = 1;
  settings.useNTP = true;
  settings.readInterval = 10;                         // Read interval [min]
  settings.save();
  settings.printConfig();
#endif

  Serial.print(F("Loading... "));
  if (!settings.load()) { //Fill with default values
    Serial.println(F("Fail"));
    settings.configUser("admin", "admin");
    settings.name(PSTR("TemperatureStation"));
    settings.configAP(("TemperatureStation" + (String)ESP.getChipId()).c_str(), "TemperatureStation");
  } else Serial.println(F("OK"));

  bool gotwifi = false;
  if (strcmp(settings.ssid(), "")) {
    Serial.print(F("Connecting to:\r\n\t"));
    Serial.print(settings.ssid());
    if (settings.beginWiFi()) {
      Serial.print(F("\tSuccess \r\nIP Address:\t"));
      Serial.print(WiFi.localIP());
      gotwifi = true;
    } else {
      Serial.print(F("\tConnection failed"));
      gotwifi = false;
    }
    Serial.println();
  }

  if (!gotwifi) {
    Serial.print(F("Starting Access Point... "));
    Serial.println(settings.beginAP() ? F("Success") : F("Fail"));
    Serial.print(F("Connect to:\t"));
    Serial.println(settings.ssidAP());
  }

  sensors.begin();
  sensors.requestTemperatures();

  settings.configUpdateServer(&server, &updServer, "/update");
  //server.setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));
  server.on("/settings.cgi", devconfig);
  server.on("/sensors.cgi", HTTP_GET, sensorSettings);
  server.on("/editsaved.cgi", HTTP_POST, editsaved);
  server.on("/time.cgi", HTTP_POST, timeset);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println(F("HTTP server:\tStarted"));

  if (!digitalRead(SDA))
    I2C_clearBus();

  Wire.begin(SDA, SCL);
  zegar.Begin();
  zegar.SetIsRunning(true);

  if (settings.useNTP) {
    teraz = zegar.GetDateTime();
    uint32_t newTime = updateNTP(settings.ntpServer());
    if ((newTime > 0 && (newTime - (uint32_t)teraz) > 59) || settings.lastUpdate == 0) {
      Serial.println(F("Adjusting time..."));
      Serial.println(PSTR("Before: ") + String(teraz));
      Serial.println(PSTR("Current: ") + String(newTime));
      zegar.SetIsRunning(false);
      zegar.SetDateTime(RtcDateTime(newTime));
      settings.lastUpdate = teraz;
      zegar.SetIsRunning(true);
    }
  }
  teraz = zegar.GetDateTime();
  Serial.println(printDateTime(teraz));
  settings.save();

  refreshSensors(&oneWire, SENSORS_FILE);

  DynamicJsonBuffer jsonBuffer(1000);
  File root = SD.open(SENSORS_FILE, FILE_READ);
  JsonArray& nSet = jsonBuffer.parseArray(root);
  root.close();

  Serial.println(F("\nLocal:"));
  uint8_t nSensor = nSet.size();
  for (uint8_t a = 0; a < nSensor; a++) {
    JsonArray& _e = nSet[a];
    Serial.print('\t' + String(a + 1) + "/" + String(nSensor) + ": ");
    Serial.print(_e[0].as<const char*>());
    if (getTemp(_e[1]) != -127)
      Serial.println(F(" Connected"));
    else Serial.println(F(" N/C"));
  }

  Serial.println();
}

void loop() {
  while (zegar.GetDateTime() % (settings.readInterval * 60) != 0) {
    server.handleClient();
    yield();
  }

  if (!digitalRead(SD_D)) {
    sensors.requestTemperatures();
    teraz = zegar.GetDateTime();

    DynamicJsonBuffer jsonBuffer(1000);
    File root = SD.open(SENSORS_FILE , FILE_READ);
    JsonArray& nSet = jsonBuffer.parseArray(root);
    root.close();

    char* filename = new char[24];
    createFile(filename, 24, teraz);

    File dest = SD.open(filename, FILE_WRITE);

    Serial.print(printDateTime(teraz));
    Serial.println(F("\tZapis"));

    dest.print(printDateTime(teraz) + ";");
    dest.print(settings.name());
    dest.print(F(":("));
    dest.flush();
    bool first = true;
    for (uint8_t c = 0; c < nSet.size(); c++) {
      JsonArray& _e = nSet[c];
      double tempRead = getTemp(_e[1]);
      if (tempRead == -127) continue;

      if (!first) dest.print(F(","));
      else first = false;
      dest.print(_e[0].as<const char*>());
      dest.print(F("="));
      dest.print(tempRead, 1);
      dest.flush();

    }
    dest.println(F(");"));
    dest.flush();
    dest.close();
    delete[] filename;
  } else {
    Serial.println(F("!!! NO SDCARD !!!"));
    while (digitalRead(SD_D)) {
      yield();
    }
    Serial.println(F("SD Card inserted. Rebooting..."));
    ESP.restart();
  }

  while (zegar.GetDateTime() % (settings.readInterval * 60) == 0) {
    server.handleClient();
    yield();
  }
}

uint32_t updateNTP(const char* ntpServerName) {
  WiFiUDP udp;
  IPAddress timeServerIP;
  udp.begin(2390);
  WiFi.hostByName(ntpServerName, timeServerIP);
  Serial.println(F("Time sync..."));

  byte packetBuffer[48];
  memset(packetBuffer, 0, 48);
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  int cb = udp.parsePacket();

  uint8_t i = 0;
  while (!cb && i++ < 10) {
    udp.beginPacket(timeServerIP, 123);
    udp.write(packetBuffer, 48);
    udp.endPacket();
    udp.flush();
    delay(1000);
    cb = udp.parsePacket();
  }

  if (i < 10 && cb) {
    udp.read(packetBuffer, 48);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    secsSince1900 -= 2208988800UL;
    secsSince1900 -= 946684800;
    secsSince1900 += settings.timezone * 3600;
    return secsSince1900;
  } else return false;
}

void devconfig() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  if (server.method() == HTTP_POST) {
    DynamicJsonBuffer output(JSON_OBJECT_SIZE(11));
    JsonObject& data = output.createObject();
    String resp;
    if (server.arg("a") == "l") {
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      data["SN"] = settings.name();
      data["SS"] = settings.ssidAP();
      data["OS"] = settings.ssid();
      data["OI"] = settings.IPtoString(WiFi.localIP());
      data["OG"] = settings.IPtoString(WiFi.gatewayIP());
      data["OM"] = settings.IPtoString(WiFi.subnetMask());
      data["OD"] = settings.useDHCP();
      data["SL"] = settings.username();
      data["UN"] = settings.useNTP;
      data["NS"] = settings.ntpServer();
      data["TZ"] = settings.timezone;
      data.printTo(resp);
      server.send(200, F("application/json"), resp);
    } else if (server.arg("a") == "s") {
      if (server.hasArg("SS") && server.hasArg("SPA"))
        settings.configAP(server.arg("SS").c_str(),
                          server.arg("SPA").c_str());
      if (server.hasArg("OS") && server.hasArg("OP"))
        settings.configSTA(server.arg("OS").c_str(),
                           server.arg("OP").c_str());
      if (server.hasArg("OD"))
        settings.useDHCP(server.arg("OD")[0] == '1');
      if (server.hasArg("SN"))
        settings.name(server.arg("SN").c_str());
      if (server.hasArg("NL") && server.hasArg("NP"))
        settings.configUser(server.arg("NL").c_str(),
                            server.arg("NP").c_str());
      if (server.hasArg("OI"))
        settings.configIP(settings.stringToIP(server.arg("OI").c_str()),
                          settings.gatewayIP(),
                          settings.subnetMask());
      if (server.hasArg("OG"))
        settings.configIP(settings.localIP(),
                          settings.stringToIP(server.arg("OG").c_str()),
                          settings.subnetMask());
      if (server.hasArg("OM"))
        settings.configIP(settings.localIP(),
                          settings.gatewayIP(),
                          settings.stringToIP(server.arg("OM").c_str()));
      settings.save();
      returnOK(F("Settings saved"));
    } else if (server.arg("a") == "r") {
      returnOK(F("Settings reset. Rebooting..."));
      settings.remove(server.arg("SL").c_str(),
                      server.arg("SPL").c_str(),
                      SETTINGS_FILE);
      ESP.restart();
    } else returnFail(F("Method not allowed"));
  } else server.send(200, F("text/html"), settings_htm);
  return void();
}

void timeset() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  if (server.method() == HTTP_POST) {
    if (server.arg("a") == "l") {
      char _d[20];
      snprintf_P(_d, 20,
                 PSTR("%04u-%02u-%02uT%02u:%02u"),
                 teraz.Year(),
                 teraz.Month(),
                 teraz.Day(),
                 teraz.Hour(),
                 teraz.Minute());
      server.send(200, F("text/plain"), _d);
    } else if (settings.authenticate(
                 server.arg("SL").c_str(),
                 server.arg("SPL").c_str())) {
      if (server.arg("a") == "s") {
        if (server.hasArg("TZ"))
          settings.timezone = atoi(server.arg("TZ").c_str());
        if (server.hasArg("UN"))
          settings.useNTP = (server.arg("UN")[0] == '1');
        if (server.hasArg("NS"))
          settings.ntpServer(server.arg("NS").c_str());
        if (server.hasArg("EP")) {
          uint32_t newTime = strtoul(server.arg("EP").c_str(), NULL, 10);
          newTime += 3600 * settings.timezone;
          newTime -= 946684800;
          zegar.SetDateTime(RtcDateTime(newTime));
          settings.lastUpdate = newTime;
        }
        settings.save();
        returnOK(F("Time settings saved"));
      }
    } else returnLoginFail();
  }
  return void();
}

void sensorSettings() {
  sensors.requestTemperatures();
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  DynamicJsonBuffer sensBuff(1000);
  File tmpSens = SD.open(SENSORS_FILE , FILE_READ);
  JsonArray& sensSet = sensBuff.parseArray(tmpSens);
  tmpSens.close();

  if (!sensSet.success())
    return;

  server.sendContent(sensors_htm);

  for (uint8_t a = 0; a < sensSet.size(); a++) {
    JsonArray& _e = sensSet[a];
    double _t = getTemp(_e[1]);
    server.sendContent(F("<tr><td><input type='text' name='"));
    server.sendContent(String(a)); //Position in file
    server.sendContent(F("' placeholder='"));
    server.sendContent(_e[0].as<const char*>());
    server.sendContent(F("'><td>"));
    server.sendContent(_e[1]);
    server.sendContent(F("</td><td>"));
    server.sendContent(_t == -127.00 ? "N/C" : String(_t));
    server.sendContent(F("</td><td><input type='checkbox' name='r' value='"));
    server.sendContent(String(a));
    server.sendContent(F("'>Remove</td></tr>"));
  }
  server.sendContent(sensors_htm2);
  return void();
}

void editsaved() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  if (server.args() != 0) {
    DynamicJsonBuffer sensBuff(1000);
    File tmpSens = SD.open(SENSORS_FILE , FILE_READ);
    JsonArray& sensSet = sensBuff.parseArray(tmpSens);
    tmpSens.close();

    if (!sensSet.success())
      return;

    for (int x = server.args() - 1; x >= 0; x--) {
      if (server.argName(x) == "r")
        sensSet.remove((atoi((server.arg(x)).c_str())));
      else
        sensSet[atoi((server.argName(x)).c_str())][0] = server.arg(x);

    }
    saveJson(sensSet, SENSORS_FILE);
  }
  server.sendContent(F("HTTP/1.1 303 See Other\r\nLocation:/sensors.cgi\r\n"));
}

void returnOK(const __FlashStringHelper* _m) {
  server.send(200, F("text/plain"), _m);
  return void();
}

void returnFail(String _m) {
  server.send(500, F("text/plain"), _m + PSTR("\r\n"));
  return void();
}

void returnLoginFail() {
  server.send(401, F("text/plain"), F("Incorrect login or password"));
  return void();
}

bool loadFromSdCard(String path) {
  if (digitalRead(SD_D))
    return false;
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";

  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".csv")) dataType = "text/txt";

  File dataFile = SD.open(path.c_str(), FILE_READ);
  if (!dataFile)
    return false;

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    Serial.println(F("Sent less data than expected!"));
  }

  dataFile.close();
  return true;
}

void handleNotFound() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  String p = server.uri();
  if (SD.exists("/data" + p) && !digitalRead(SD_D)) {
    handleIndex(p);
    return;
  }

  String message = F("PLIKU NIE ZNALEZIONO\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

double getTemp(JsonArray & addrset) {
  byte tempaddr[8];
  for (int b = 0; b < 8; b++)
    tempaddr[b] = addrset[b];
  return sensors.getTempC(tempaddr);
}

void handleIndex(String path) {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  File dir = SD.open("/data" + path, FILE_READ);
  if (!dir.isDirectory()) {
    dir.close();
    if (!digitalRead(SD_D) && loadFromSdCard("/data" + path)) return;
  }
  dir.rewindDirectory();

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.sendContent(index_htm);
  server.sendContent(path);
  server.sendContent(index_htm2);

  server.sendContent(F("<tr><td>Dir</td><td><a href=\""));
  if (path.lastIndexOf('/') > 4)
    server.sendContent(path.substring(0, path.lastIndexOf('/')));
  else
    server.sendContent(F("/"));
  server.sendContent(F("\">..</a></td></tr>"));
  File entry;
  for (int cnt = 0; true; cnt++) {
    entry = dir.openNextFile();
    if (!entry) break;

    server.sendContent(F("<tr><td>"));
    server.sendContent(entry.isDirectory() ? F("Dir") : F("File"));
    server.sendContent(F("</td><td><a href=\""));
    server.sendContent(path);
    if (path != "/") server.sendContent("/");
    server.sendContent(String(entry.name()) + "\">");
    server.sendContent(entry.name());
    server.sendContent(F("</a></td><td>"));
    server.sendContent(String(entry.size()));
    server.sendContent(F("</td></tr>"));
  }
  entry.close();
  server.sendContent(F("</table></body></html>"));
  return void();
}
