#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <RtcDS3231.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFiUdp.h>

#define FS_NO_GLOBALS
#include "FS.h"
#include "defs.h"

#define HTTP_LOGIN String(settings.selfLOGIN).c_str()
#define HTTP_PASS String(settings.selfPASS).c_str()
/*
  This sketch needs hardware hack if you're using Chinese SD adapter

  Solder 10kom resistor between 3V3 and SD detect pin (on shield)
  and SD Detect pin to D0

  SD Detect is connected to GND when SDcard is loaded.

  Automatic DST

  Fixt timezones on /settings
*/

bool letni = true;

//ESP8266WebServer server(80);
ESP8266WebServerSecure server(443);

OneWire oneWire(OW_PORT);
DallasTemperature sensors(&oneWire);

RtcDS3231<TwoWire> zegar(Wire);
RtcDateTime teraz;

struct _setup {
  char selfName[32];
  char selfSSID[32];
  char selfWPA2[32];
  char selfLOGIN[32];
  char selfPASS[32];
  char outSSID[32];
  char outPASS[32];
  char outIP[16];
  char outGW[16];
  char outMASK[16];
  bool outDHCP;
  bool useNTP;
  bool selfAP;
  char ntpServer[32];
  uint32_t lastUpdate;
  uint8_t timezone;
} settings;

void setup() {
  WiFi.disconnect();
  WiFi.persistent(false);
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  pinMode(SD_D, INPUT);
  //attachInterrupt(SD_D, noCard, HIGH);
  //interrupts();
  Serial.print(F("\n"));
  sensors.begin();
  sensors.requestTemperatures();

  if (!digitalRead(SD_D)) {
    if (SD.begin(SD_CS)) {
      Serial.println(F("SD Card initialized"));
      if (!SD.exists(SETTINGS_FILE))
        bootFailHandler(1);
    } else bootFailHandler(2);
  } else bootFailHandler(3);

  SPIFFS.begin();

  if (!SPIFFS.exists("/set.dat")) {
    Serial.println("Creating settings file...");
    strcpy(settings.selfName, "Temperature Station");
    strcpy(settings.selfSSID, "Temperature_Station");
    strcpy(settings.selfWPA2, "0123456789");
    strcpy(settings.selfLOGIN, "admin");
    strcpy(settings.selfPASS, "admin");

    settings.outDHCP = false;
    settings.useNTP = false;
    settings.selfAP = true;
    settings.lastUpdate = 0;
    settings.timezone = 0;
    fs::File lel = SPIFFS.open("/set.dat", "w");
    lel.write((const uint8_t*)&settings, sizeof(settings) / sizeof(char));
    lel.flush();
    lel.close();
  } else {
    fs::File lol = SPIFFS.open("/set.dat", "r");
    lol.readBytes((char*)&settings, lol.size());
    lol.close();
  }

  SPIFFS.end();

  wifiConn();

  server.setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));
  server.on("/time", HTTP_POST, timeset);
  server.on("/sysinfo", HTTP_GET, httpinfo);
  server.on("/list", HTTP_GET, printDirectory);
  server.on("/", HTTP_PUT, handleCreate);
  server.on("/", HTTP_POST, []() {
    returnOK();
  }, handleFileUpload);
  server.on("/settings", devconfig);
  server.on("/sensors", sensorSettings);
  server.on("/settings.txt", returnForbidden);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println(F("HTTPS server started"));
  if (!digitalRead(SDA))
    I2C_clearBus();

  Wire.begin(SDA, SCL);
  zegar.Begin();
  zegar.SetIsRunning(true);

  if (settings.useNTP) {
    teraz = zegar.GetDateTime();
    String wo = String(settings.ntpServer);
    char* ntpName = new char[wo.length() + 1];
    strcpy(ntpName, wo.c_str());
    uint32_t newTime = updateNTP(ntpName);
    if (RtcDateTime(newTime) >= teraz) {
      Serial.println(F("Adjusting time..."));
      Serial.println("Before: " + printDateTime(teraz));
      Serial.println("Current: " + printDateTime(RtcDateTime(newTime)));
      zegar.SetIsRunning(false);

      if (newTime != 0) zegar.SetDateTime(RtcDateTime(newTime));
      else Serial.println("NTP error");
      teraz = zegar.GetDateTime();
      Serial.print("After: ");
    }
    if (newTime > settings.lastUpdate)
      settings.lastUpdate = newTime;
  }
  Serial.println(printDateTime(teraz));

  SPIFFS.begin();
  saveSettings("/set.dat", settings);
  SPIFFS.end();

  DynamicJsonBuffer jsonBuffer(1000);
  File root = SD.open(SETTINGS_FILE, FILE_READ);
  JsonObject& nSet = jsonBuffer.parseObject(root);
  root.close();

  Serial.println(F("\nLocal:"));
  int nSensor = nSet["local"].size();
  for (int a = 0; a < nSensor; a++) {
    Serial.print('\t' + (String)(a + 1) + "/" + (String)(nSensor) + ": ");
    Serial.print(nSet["local"][a]["n"].as<const char*>());
    if (getTemp(nSet["local"][a]["a"]) != -127)
      Serial.println(F(" Connected"));
    else Serial.println(F(" N/C"));
  }
  Serial.println();
}

void loop() {
  while ((teraz.Minute() % 5 != 0) && !digitalRead(SD_D)) {
    teraz = zegar.GetDateTime();
    server.handleClient();
    yield();
  }

  if (!digitalRead(SD_D)) {
    sensors.requestTemperatures();
    teraz = zegar.GetDateTime();

    Serial.println("Zapis");
    if (!(teraz.Day() > 31 || teraz.Month() > 12 \
          || teraz.Minute() > 59 || teraz.Hour() > 23)) {
      String path = "archiwum/" + (String)teraz.Year();
      if (teraz.Month() < 10) path += "0";
      path += (String)teraz.Month();
      char* path2 = new char[path.length() + 1];
      strcpy(path2, path.c_str());
      SD.mkdir(path2);
      path += "/";
      if (teraz.Day() < 10) path += "0";
      path += (String)teraz.Day() + ".csv";
      path2 = new char[path.length() + 1];
      strcpy(path2, path.c_str());

      DynamicJsonBuffer jsonBuffer(1000);
      File root = SD.open(SETTINGS_FILE , FILE_READ);
      JsonObject& nSet = jsonBuffer.parseObject(root);
      root.close();
      File dest = SD.open(path, FILE_WRITE);
      dest.print(printDateTime(teraz) + ";");
      dest.print(String(settings.selfName));
      dest.print(":(");
      dest.flush();
      bool first = true;
      for (int c = 1; c < nSet["local"].size(); c++) {
        double tempRead = getTemp(nSet["local"][c]["a"]);
        if (tempRead != -127) {
          if (!first) dest.print(F(","));
          else first = false;
          dest.print(nSet["local"][c]["n"].as<String>());
          dest.print(F("="));
          dest.print(tempRead, 1);
          dest.flush();
        }
      }
      dest.println(");");
      dest.flush();
      dest.close();
    }
  } else {
    Serial.println(F("!!! NO SDCARD !!!"));
    while (digitalRead(SD_D)) {
      yield();
    }
    Serial.println(F("SD Card inserted. Reboot..."));
    ESP.restart();
  }

  while ((teraz.Minute() % 5 == 0) && !digitalRead(SD_D)) {
    teraz = zegar.GetDateTime();
    server.handleClient();
    yield();
  }
}

uint32_t updateNTP(char* ntpServerName) {
  WiFiUDP udp;
  IPAddress timeServerIP;
  udp.begin(2390);
  WiFi.hostByName(ntpServerName, timeServerIP);
  Serial.println(F("Sending NTP packet..."));

  byte packetBuffer[48];
  memset(packetBuffer, 0, 48);
  packetBuffer[0] = 0b11100011; // LI, Version, Mode
  packetBuffer[1] = 0; // Stratum, or type of clock
  packetBuffer[2] = 6; // Polling Interval
  packetBuffer[3] = 0xEC; // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
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
    if (letni) secsSince1900 += 3600;
    secsSince1900 -= 2208988800UL;
    secsSince1900 -= 946684800;
    secsSince1900 += settings.timezone * 3600;
    if (letni) secsSince1900 += 3600;
    return secsSince1900;
  } else return false;
}

void devconfig() {
  Serial.println();
  if (server.method() == HTTP_POST) {
    if (server.arg("action") == "load" && server.args() == 1) {
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      StaticJsonBuffer<500> output;
      JsonObject& data = output.createObject();
      data["selfName"] = settings.selfName;
      data["selfSSID"] = settings.selfSSID;
      data["selfLOGIN"] = settings.selfLOGIN;
      data["outSSID"] = settings.outSSID;
      data["outIP"] = settings.outIP;
      data["outGW"] = settings.outGW;
      data["outMASK"] = settings.outMASK;
      data["outDHCP"] = settings.outDHCP;
      data["useNTP"] = settings.useNTP;
      data["selfAP"] = settings.selfAP;
      data["ntpServer"] = settings.ntpServer;
      data["timezone"] = settings.timezone;
      data["datetime"] = nowToString(teraz);
      String resp;
      data.printTo(resp);
      //server.send(200, "text/json", "");
      server.sendContent(resp);
      server.sendContent("\x00");
      data.prettyPrintTo(Serial);
    } else if (server.arg("action") == "save") {
      //if (!server.authenticate((char*)HTTP_LOGIN, (char*)HTTP_PASS))
      //  return server.requestAuthentication();

      if (server.arg("selfName") != "")
        strcpy(settings.selfName, server.arg("selfName").c_str());
      if (server.arg("selfSSID") != "")
        strcpy(settings.selfSSID, server.arg("selfSSID").c_str());
      if (server.arg("selfWPA2") != "")
        strcpy(settings.selfWPA2, server.arg("selfWPA2").c_str());
      if (server.arg("selfLOGIN") != "")
        strcpy(settings.selfLOGIN, server.arg("selfLOGIN").c_str());
      if (server.arg("selfPASS") != "")
        strcpy(settings.selfPASS, server.arg("selfPASS").c_str());
      if (server.arg("outSSID") != "")
        strcpy(settings.outSSID, server.arg("outSSID").c_str());
      if (server.arg("outPASS") != "")
        strcpy(settings.outPASS, server.arg("outPASS").c_str());
      if (server.arg("outIP") != "")
        strcpy(settings.outIP, server.arg("outIP").c_str());
      if (server.arg("outGW") != "")
        strcpy(settings.outGW, server.arg("outGW").c_str());
      if (server.arg("outMASK") != "")
        strcpy(settings.outMASK, server.arg("outMASK").c_str());
      if (server.arg("outDHCP") != "")
        settings.outDHCP = toBool(server.arg("outDHCP"));
      if (server.arg("selfAP") != "")
        settings.selfAP = toBool(server.arg("selfAP"));
      SPIFFS.begin();
      saveSettings("/set.dat", settings);
      SPIFFS.end();
      Serial.println("Zapis ustawien");
      server.sendContent(F("\nHTTP/1.1 303 See Other\r\n"));
      server.sendContent(F("Location: /settings\r\n"));
    } else if (server.arg("action") == "test") {
      Serial.println(server.uri());
      for (int x = server.args() - 1; x >= 0; x--) {
        Serial.print(server.argName(x));
        Serial.print("=");
        Serial.println(server.arg(x));
      }
      server.sendContent(F("\nHTTP/1.1 303 See Other\r\n"));
      server.sendContent(F("Location: /settings\r\n"));
    } else if (server.arg("action") == "reset") {
      SPIFFS.begin();
      SPIFFS.remove("/set.dat");
      SPIFFS.end();
      ESP.restart();
    }
  } else loadFromSdCard("/set.htm");
}

String nowToString(const RtcDateTime & _now) {
  char _toString[20];
  snprintf_P(_toString,
             countof(_toString),
             PSTR("%04u/%02u/%02uT%02u:%02u"),
             _now.Year(),
             _now.Month(),
             _now.Day(),
             _now.Hour(),
             _now.Minute());
  return _toString;
}

void timeset() {
  if (server.arg("action") == "test") {
    Serial.println(server.uri());
    for (int x = server.args() - 1; x >= 0; x--) {
      Serial.print(server.argName(x));
      Serial.print("=");
      Serial.println(server.arg(x));
    }
  } else if (server.arg("action") == "reset") {
    SPIFFS.begin();
    SPIFFS.remove("/set.dat");
    SPIFFS.end();
    ESP.restart();
  } else if (server.arg("action") == "save") {
    if (server.arg("timezone") != "")
      settings.timezone = atoi(server.arg("timezone").c_str());
    if (server.arg("useNTP") != "")
      settings.useNTP = toBool(server.arg("useNTP"));
    if (server.arg("ntpServer") != "")
      strcpy(settings.ntpServer, server.arg("ntpServer").c_str());
    if (server.arg("epoch") != "NaN") {
      uint32_t newLast = strtoul(server.arg("epoch").c_str(), NULL, 10);
      newLast -= 946684800;
      newLast += 3600 * settings.timezone + 3600 * (int)letni;
      Serial.println(newLast);
      zegar.SetDateTime(newLast);
      settings.lastUpdate = newLast;

    }
    SPIFFS.begin();
    saveSettings("/set.dat", settings);
    SPIFFS.end();
    Serial.println("Zapis czasu");
    teraz = zegar.GetDateTime();
    Serial.println(printDateTime(teraz));
  }
  server.sendContent(F("\nHTTP/1.1 303 See Other\r\n"));
  server.sendContent(F("Location: /settings\r\n"));
}

void sensorSettings() {
  if (!server.authenticate((char*)HTTP_LOGIN, (char*)HTTP_PASS))
    return server.requestAuthentication();

  DynamicJsonBuffer sensBuff(1500);
  File tmpSens = SD.open(SETTINGS_FILE , FILE_READ);
  JsonObject& sensSet = sensBuff.parseObject(tmpSens);
  tmpSens.close();

  if (!sensSet.success())
    return;

  String changeName;
  int changePos;

  if (server.args() != 0 && server.method() == HTTP_POST) {
    for (int x = server.args() - 1; x >= 0; x--) {
      if (server.argName(x) == "Remove") {
        JsonArray& setMain = sensSet["local"];
        setMain.remove((atoi((server.arg(x)).c_str())));
        if ((server.arg(x) == server.argName(x - 1)) && x > 0)
          x -= 1;
      } else {
        changePos = atoi((server.argName(x)).c_str());
        changeName = server.arg(x);
        if (sensSet["local"][changePos]["n"].as<String>() != changeName)
          sensSet["local"][changePos]["n"] = changeName;
      }
    }
    server.sendContent(F("HTTP/1.1 303 See Other\r\n"));
    server.sendContent(F("Location: /sensors\r\n"));
    saveJson(sensSet);
  } else if (server.method() == HTTP_GET && server.args() == 0) {

    sensors.requestTemperatures();
    uint8_t i;
    uint8_t addr[8];
    uint8_t sensorRow;

    server.sendContent(F("<!DOCTYPE html><html><head><title>Sensors</title>"));
    server.sendContent(F("<style>table, th, td{border: 1px solid black; "));
    server.sendContent(F("text-align: center}</style></head>"));
    server.sendContent(F("<body><a href=\"/\"><button>Strona glowna</button>"));
    server.sendContent(F("</a><table><caption><b>Available sensors</b></caption>"));
    server.sendContent(F("<tr><th>Address (DEC) </th> <th>Temperature </th><th>Name</th>"));
    server.sendContent(F("<th>Name Settings </th>"));
    server.sendContent(F("<form id=\"sensors\" method=\"POST\" action=\"/sensors\">"));

    for (int a = 0; a < sensSet["local"].size(); a++) {
      server.sendContent(F("<tr><td>"));
      for (int b = 0; b < 8; b++)
        addr[b] = sensSet["local"][a]["a"][b];

      for (i = 0; i < 8; i++) {
        if (addr[i] < 100) server.sendContent(F("0"));
        if (addr[i] < 10) server.sendContent(F("0"));
        server.sendContent((String)addr[i]);
        if (i < 7)
          server.sendContent(F(","));
      }
      server.sendContent(F("</td><td>"));

      double _te = sensors.getTempC(addr);
      if (_te != -127)
        server.sendContent((String)_te + (char)176 + "C");
      else
        server.sendContent(F("N/C"));

      server.sendContent(F("</td><td>"));

      server.sendContent(sensSet["local"][a]["n"].as<String>());
      server.sendContent(F("</td><td><input type = \"text\" name = "));
      server.sendContent(String(a)); //Position in file
      server.sendContent(F(" value = "));
      server.sendContent(sensSet["local"][a]["n"].as<String>());
      server.sendContent(F("><input type=\"checkbox\" name=\"Remove\" value= "));
      server.sendContent(String(a));
      server.sendContent(F(">Remove</td></tr>"));
    }
    bool anyNew = false;
    while (oneWire.search(addr)) {
      if (isMember(addr, sensSet["local"]))
        continue;
      else anyNew = true;

      server.sendContent(F("<tr><td>"));
      for (i = 0; i < 8; i++) {
        if (addr[i] < 100) server.sendContent(F("0"));
        if (addr[i] < 10) server.sendContent(F("0"));
        server.sendContent((String)addr[i]);
        if (i < 7)
          server.sendContent(F(","));
      }
      if (OneWire::crc8(addr, 7) != addr[7]) {
        server.sendContent(F("Invalid CRC!"));
        return;
      }
      server.sendContent(F("</td><td>"));

      String tempName = addrToString(addr);
      JsonArray& _local = sensSet["local"];
      JsonObject& newSet = _local.createNestedObject();
      newSet["n"] = tempName;
      JsonArray& _addr = newSet.createNestedArray("a");
      for (int x = 0; x < 8; x++)
        _addr.add(addr[x]);
      sensorRow = sensSet["local"].size() - 1;

      server.sendContent((String)sensors.getTempC(addr) + (char)176 + "C");
      server.sendContent(F("</td><td>"));
      server.sendContent(tempName);
      server.sendContent(F("</td><td><input type=\"text\" name="));
      server.sendContent(String(sensorRow)); //Position in file
      server.sendContent(F(" value="));
      server.sendContent(newSet["n"].as<String>());
      server.sendContent(F("><input type=\"checkbox\" name=\"Remove\" value="));
      server.sendContent(String(sensorRow)); //Position in file
      server.sendContent(F(">Remove</td></tr>"));
    }
    server.sendContent(F("<button type=\"submit\" value=\"Save\">Save Changes</button>"));
    server.sendContent(F("<button type=\"reset\" value=\"Reset\">Reset</button>"));
    server.sendContent(F("</form></table></body></html>"));
    //server.sendContent(F(""));
    if (anyNew) saveJson(sensSet);
  }
  return;
}

void httpinfo() {
  if (!server.authenticate((char*)HTTP_LOGIN, (char*)HTTP_PASS))
    return server.requestAuthentication();

  server.sendContent(F("<!DOCTYPE html><html>\
  <head><title>System info</title></head>"));

  server.sendContent(F("<body><b>Not implemented yet</b>"));
  server.sendContent(F("<a href=\"/\"><button>Strona glowna</button></a>"));
  server.sendContent(F("</body></html>"));
}

bool wifiConn () {
  if (settings.selfAP) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(settings.selfSSID, settings.selfWPA2);
  }
  else WiFi.mode(WIFI_STA);
  if (!settings.outDHCP)
    WiFi.config(stringToIP(settings.outIP), stringToIP(settings.outGW), stringToIP(settings.outMASK));

  WiFi.begin(settings.outSSID, settings.outPASS);
  Serial.print(F("Connecting to "));
  Serial.print(settings.outSSID);

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) { //wait 10 seconds
    delay(500);
    Serial.print(F("."));
  }

  if (i >= 21 && WiFi.status() != WL_CONNECTED) {
    Serial.println(F(" Fail"));
    if (!settings.selfAP) {
      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP(settings.selfSSID, settings.selfWPA2);
    }
    return false;
  }

  Serial.print(F(" Success\n"));
  Serial.print(F("IP obtain mode: "));
  if (settings.outDHCP)
    Serial.println(F("DHCP"));
  else Serial.println(F("Static"));
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("Gateway: "));
  Serial.println(WiFi.gatewayIP());
  Serial.print(F("Subnet mask: "));
  Serial.println(WiFi.subnetMask());

  return true;
}

void returnOK() {
  server.send(200, "text/plain", "");
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

void returnForbidden() {
  server.send(403, "text/plain", "Access Denied\r\n");
}

bool loadFromSdCard(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";

  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  else if (path.endsWith(".png")) dataType = "image/png";
  else if (path.endsWith(".gif")) dataType = "image/gif";
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
  else if (path.endsWith(".ico")) dataType = "image/x-icon";
  else if (path.endsWith(".xml")) dataType = "text/xml";
  else if (path.endsWith(".pdf")) dataType = "application/pdf";
  else if (path.endsWith(".zip")) dataType = "application/zip";
  else if (path.endsWith(".csv")) dataType = "text/txt";

  File dataFile = SD.open(path.c_str());
  if (dataFile.isDirectory()) {
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open(path.c_str());
  }

  if (!dataFile)
    return false;

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    Serial.println("Sent less data than expected!");
  }

  dataFile.close();
  return true;
}

void handleFileUpload() {
  File uploadFile;
  if (server.uri() != "/") return;
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    if (SD.exists((char *)upload.filename.c_str()))
      SD.remove((char *)upload.filename.c_str());
    uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    Serial.print(F("Upload: START, filename: "));
    Serial.println(upload.filename);
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
    Serial.print(F("Upload: WRITE, Bytes: "));
    Serial.println(upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) uploadFile.close();
    Serial.print(F("Upload: END, Size: "));
    Serial.println(upload.totalSize);
  }
}

void handleCreate() {
  if (server.args() == 0) return returnFail(F("BAD ARGS"));
  String path = server.arg(0);
  if (path == "/" || SD.exists((char *)path.c_str())) {
    returnFail(F("BAD PATH"));
    return;
  }

  if (path.indexOf('.') > 0) {
    File file = SD.open((char *)path.c_str(), FILE_WRITE);
    if (file) {
      file.write((const char *)0);
      file.close();
    }
  } else {
    SD.mkdir((char *)path.c_str());
  }
  returnOK();
}

void printDirectory() {
  /*for (int a = 0; a < server.args(); a++) {
    Serial.print(server.argName(a));
    Serial.print(" = ");
    Serial.println(server.arg(a));
    }*/
  if (!server.hasArg("dir"))
    return returnFail("BAD ARGS");
  String path = server.arg("dir");
  if (path != "/" && !SD.exists((char *)path.c_str()))
    return returnFail("BAD PATH");
  File dir = SD.open((char *)path.c_str());
  path = String();
  if (!dir.isDirectory()) {
    dir.close();
    return returnFail("NOT DIR");
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  //server.send(200, "text/json", "");
  WiFiClient client = server.client();
  server.sendContent("[");
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    if (!entry)
      break;

    String output;
    if (cnt > 0)
      output = ',';

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += entry.name();
    output += "\"}";
    server.sendContent(output);
    entry.close();
  }
  server.sendContent("]\r\n\x00");
  dir.close();
}

void handleNotFound() {
  if (!server.authenticate((char*)HTTP_LOGIN, (char*)HTTP_PASS))
    return server.requestAuthentication();

  if (!digitalRead(SD_D) && loadFromSdCard(server.uri())) return;
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
  Serial.print(message);
}

double getTemp(JsonArray & addrset) {
  byte tempaddr[8];
  for (int b = 0; b < 8; b++)
    tempaddr[b] = addrset[b];
  return sensors.getTempC(tempaddr);
}

void saveSettings(char* filename, struct _setup & tmp) {
  if (SPIFFS.exists(filename)) SPIFFS.remove(filename);
  fs::File toSave = SPIFFS.open(filename, "w");
  toSave.write((const uint8_t*)&tmp, sizeof(tmp));
  toSave.flush();
  toSave.close();
}

void I2C_clearBus() {
  //http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html
  Serial.println("Resetting RTC I2C interface");
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
    Serial.println("SDA nadal nisko");
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
