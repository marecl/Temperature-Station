#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <RtcDS3231.h>
#include <settingsManager.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include "CommonUtils.hpp"
#include "SensorsPageHandler.hpp"
#include "SensorsEditPageHandler.hpp"
#include "TimePageHandler.hpp"
#include "LoginPageHandler.hpp"

/*
   przy niewaznym ciasteczku przekierowac na /login
   naprawic /login
   Dostep do plikow przez strona/file=/[sciezka]
   wywalic strings.cpp
*/

#define FS_NO_GLOBALS
#include "FS.h"

/* Pin definitions */
#define SD_D D0     //GPIO16
#define SDA D1      //GPIO5
#define SCL D2      //GPIO4
#define OW_PORT D3  //GPIO0
#define SD_CS D4    //GPIO2

/* Good when trying different configurations */
#define SETTINGS_FILE "/SET.TXT" //SPIFFS
#define SENSORS_FILE "/DATA/SENSORS.TXT" //SD

/* Webpage */
extern char settings_htm[];

ESP8266WebServer server(80);
//ESP8266WebServerSecure server(443);
ESP8266HTTPUpdateServer updServer;

OneWire oneWire(OW_PORT);
DallasTemperature sensors(&oneWire);

RtcDS3231<TwoWire> zegar(Wire);
RtcDateTime teraz;

settingsManager settings(SETTINGS_FILE);

settingsManager* CommonUtils::setptr = &settings;
uint32_t CommonUtils::globalTimer = 0;

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
  settings.ntpServer("tempus1.gum.gov.pl");           // NTP server (not pool)
  settings.configUser("admin", "admin");              // Login and password
  settings.timezone = 1;
  settings.useNTP = true;
  settings.readInterval = 10;                         // Read interval [min]
  settings.tokenLifespan = 600;                      // Cookie lifespan [s]
  settings.save();
  settings.printConfig();
#endif

  Serial.print(F("Loading..."));
  if (!settings.load()) { //Fill with default values
    Serial.println(F(" Fail"));
    settings.configUser("admin", "admin");
    settings.name(PSTR("TemperatureStation"));
    settings.configAP(("TemperatureStation" + (String)ESP.getChipId()).c_str(), "TemperatureStation");
  } else Serial.println(F("OK"));

  bool gotwifi = false;
  if (strcmp(settings.ssid(), "")) {
    Serial.print(F("Connecting to:\r\n\t"));
    Serial.print(settings.ssid());
    if (settings.beginSTA()) {
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

  const char* headerkeys[] = {"Cookie"};
  uint16_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  server.collectHeaders(headerkeys, headerkeyssize);
  //settings.configUpdateServer(&server, &updServer, "/update");
  //add SSL/BearSSL key
  server.addHandler(new LoginPageHandler("/login"));
  server.addHandler(new SensorsPageHandler("/sensors", &sensors, SENSORS_FILE));
  server.addHandler(new SensorsEditPageHandler("/editsaved", SENSORS_FILE));
  server.on("/settings", devconfig);
  //server.addHandler(new TimePageHandler("/time", &settings));
  server.on("/time", HTTP_POST, timeset);
  server.on("/logout", []() {
    server.sendHeader("Set-Cookie", settings.encryptKey(1));
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
  });
  server.onNotFound(handleNotFound);
  server.begin();
  settings.beginOTA();

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
  char *date = new char[20];
  Serial.println(CommonUtils::printDateTime(date, &teraz));
  delete[] date;
  settings.save();

  CommonUtils::refreshSensors(&oneWire, SENSORS_FILE);

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
    if (CommonUtils::getTemp(&sensors, _e[1]) != -127)
      Serial.println(F(" Connected"));
    else Serial.println(F(" N/C"));
  }
}

void loop() {
  while (CommonUtils::globalTimer = zegar.GetDateTime(), \
         CommonUtils::globalTimer % (settings.readInterval * 60) != 0) {
    server.handleClient();
    ArduinoOTA.handle();
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

    char *date = new char[20];
    Serial.print(CommonUtils::printDateTime(date, &teraz));
    Serial.println(F("\tZapis"));

    dest.print(date);
    dest.print(F(";"));
    dest.print(settings.name());
    dest.print(F(":("));
    dest.flush();
    delete[] date;
    bool first = true;
    for (uint8_t c = 0; c < nSet.size(); c++) {
      JsonArray& _e = nSet[c];
      double tempRead = CommonUtils::getTemp(&sensors, _e[1]);
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

  while (CommonUtils::globalTimer = zegar.GetDateTime(),
         CommonUtils::globalTimer % (settings.readInterval * 60) == 0) {
    server.handleClient();
    ArduinoOTA.handle();
    yield();
  }
}

uint32_t updateNTP(const char* ntpServerName) {
  WiFiUDP udp;
  IPAddress timeServerIP;
  udp.begin(2390);
  WiFi.hostByName(ntpServerName, timeServerIP);
  Serial.print(F("Time sync..."));

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
    Serial.println(F(" OK"));
    return secsSince1900;
  }
  Serial.println(F(" Fail"));
  return false;
}

void devconfig() {
  if (!LoginPageHandler::verifyLogin(&server, &settings)) {
    LoginPageHandler::redirectToLogin(&server);
    return;
  }

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
      CommonUtils::returnOK(&server, F("Settings saved"));
    } else if (server.arg("a") == "r") {
      CommonUtils::returnOK(&server, F("Settings reset. Rebooting..."));
      settings.remove(server.arg("SL").c_str(),
                      server.arg("SPL").c_str(),
                      SETTINGS_FILE);
      ESP.restart();
    } else CommonUtils::returnFail(&server, F("Method not allowed"));
  } else server.send(200, F("text/html"), settings_htm);
  return void();
}

void timeset() {
  if (!LoginPageHandler::verifyLogin(&server, &settings)) {
    LoginPageHandler::redirectToLogin(&server);
    return;
  }

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
    } else CommonUtils::returnLoginFail(&server);
  }
  return void();
}

void returnOK(const __FlashStringHelper* _m) {
  server.send(200, F("text/plain"), _m);
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
  if (!LoginPageHandler::verifyLogin(&server, &settings)) {
    LoginPageHandler::redirectToLogin(&server);
    return;
  }

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

void handleIndex(String path) {
  if (!LoginPageHandler::verifyLogin(&server, &settings)) {
    LoginPageHandler::redirectToLogin(&server);
    return;
  }

  File dir = SD.open("/data" + path, FILE_READ);
  if (!dir.isDirectory()) {
    dir.close();
    if (!digitalRead(SD_D) && loadFromSdCard("/data" + path)) return;
  }
  dir.rewindDirectory();

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.sendContent(F("<!DOCTYPE html><html lang=\"en\">"));
  server.sendContent(F("<head><meta http-equiv=\"cache-control\" content"));
  server.sendContent(F("=\"no-cache\" /><title>Temperature Station"));
  server.sendContent(F("</title></head><body><a href=\"/logout\"><input type"));
  server.sendContent(F("=\"button\" value=\"Logout\" /></a><a href=\"/settings\">"));
  server.sendContent(F("<input type=\"button\" value=\"Settings\" /></a>"));
  server.sendContent(F("<a href=\"/sensors\"><input type=\"button\""));
  server.sendContent(F(" value=\"Sensors\" /></a><h1>Index of "));

  server.sendContent(path);
  server.sendContent(F("</h1><table><tr><th>Type</th><th>Name</th>"));
  server.sendContent(F("<th>Size</th></tr>"));

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

void createFile(char* _fn, uint8_t _buf, RtcDateTime& dt) {
  memset(_fn, 0, _buf);
  snprintf_P(_fn, _buf, PSTR("/data/%04u%02u"), dt.Year(), dt.Month());
  SD.mkdir(_fn);
  snprintf_P(_fn, _buf, PSTR("%s/%02u.csv"), _fn, dt.Day());
}

void I2C_clearBus() {
  //http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html
  Serial.println(F("Resetting RTC I2C interface"));
  pinMode(SDA, INPUT_PULLUP);
  pinMode(SCL, INPUT_PULLUP);

  delay(2500);

  boolean SCL_LOW = (digitalRead(SCL) == LOW); // Check is SCL is Low.
  if (SCL_LOW)
    Serial.println("Cannot become master");

  boolean SDA_LOW = (digitalRead(SDA) == LOW);  // vi. Check SDA input.
  int clockCount = 20; // > 2x9 clock

  while (SDA_LOW && (clockCount > 0)) { //  vii. If SDA is Low,
    clockCount--;

    pinMode(SCL, INPUT);
    pinMode(SCL, OUTPUT);
    delayMicroseconds(10);
    pinMode(SCL, INPUT);
    pinMode(SCL, INPUT_PULLUP);
    delayMicroseconds(10); //  for >5uS
    // The >5uS is so that even the slowest I2C devices are handled.
    SCL_LOW = (digitalRead(SCL) == LOW); // Check if SCL is Low.
    int counter = 20;
    while (SCL_LOW && (counter > 0)) {  //  loop waiting for SCL to become High only wait 2sec.
      counter--;
      delay(100);
      SCL_LOW = (digitalRead(SCL) == LOW);
    }
    if (SCL_LOW) break;
    SDA_LOW = (digitalRead(SDA) == LOW); //   and check SDA input again and loop
  }
  if (SDA_LOW) { // still low
    Serial.println("SDA still low");
  }

  // else pull SDA line low for Start or Repeated Start
  pinMode(SDA, INPUT); // remove pullup.
  pinMode(SDA, OUTPUT);  // and then make it LOW i.e. send an I2C Start or Repeated start control.
  // When there is only one I2C master a Start or Repeat Start has the same function as a Stop and clears the bus.
  /// A Repeat Start is a Start occurring after a Start with no intervening Stop.
  delayMicroseconds(10); // wait >5uS
  pinMode(SDA, INPUT); // remove output low
  pinMode(SDA, INPUT_PULLUP); // and make SDA high i.e. send I2C STOP control.
  delayMicroseconds(10); // x. wait >5uS
  pinMode(SDA, INPUT); // and reset pins as tri-state inputs which is the default state on reset
  pinMode(SCL, INPUT);
  return void();
}
