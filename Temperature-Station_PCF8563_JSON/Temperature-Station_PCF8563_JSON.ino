#include <ArduinoJson.h>
#include <Czas.h> //PCF8563 library
#include <DallasTemperature.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <SD.h>
#include <SPI.h>
#include <WiFiUdp.h>
#define FS_NO_GLOBALS
#include "FS.h"
/*
  Fix selfAP on website

  Swap SD_D and SD_CS pins! Maybe then I'll can use interrupts

  Encrypt WiFi passwords in file

  Add option to change/update date&time via http

  Add links to main site
*/
#define MAX_SENSORS 16

bool httpserver = false;
bool letni = false;

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

OneWire oneWire(OW_PORT);
DallasTemperature sensors(&oneWire);

DynamicJsonBuffer jsonBuffer(1000);

WiFiServer server(23);
WiFiClient serverClients[1];

Czas zegar(SDA, SCL); //SDA, SCL

/*struct _setup {
  char* selfName = "Temperature Station";
  char* selfSSID = "Temperature Station";
  char* selfWPA2 = "0123456789";
  char* selfLOGIN = "admin";
  char* selfPASS = "admin";
  char* outSSID = "";
  char* outPASS = "";
  char* outIP = "";
  char* outGW = "";
  char* outMASK = "";
  bool outDHCP = true;
  bool useNTP = false;
  bool selfAP = false;
  char* ntpServer = "tempus1.gum.gov.pl";
  uint32_t lastUpdate = 0; //epoch of last time update from NTP
  uint8_t timezone = 0;
  };
*/
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
    SPIFFS.remove("/set.dat");
    Serial.println("Creating settings file...");
    fs::File lel = SPIFFS.open("/set.dat", "w");
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
    lel.write((const uint8_t*)&settings, sizeof(settings) / sizeof(char));
    lel.flush();
    lel.close();
  }
  fs::File lol = SPIFFS.open("/set.dat", "r");
  lol.readBytes((char*)&settings, lol.size());
  lol.close();
  SPIFFS.end();

  settings.selfAP = true;

  if (wifiConn()) httpserver = true;
  if (httpserver) {
    server.on("/settime", HTTP_POST, timeset);
    server.on("/settings", devconfig);

    server.on("/sensors", sensorSettings);
    server.on("/time", HTTP_GET, updatetime);
    server.on("/list", HTTP_GET, printDirectory);
    server.on("/", HTTP_DELETE, handleDelete);
    server.on("/", HTTP_PUT, handleCreate);
    server.on("/", HTTP_POST, []() {
      returnOK();
    }, handleFileUpload);
    server.on("/settings.txt", returnForbidden);
    server.onNotFound(handleNotFound);
    httpUpdater.setup(&server);
    server.begin();
    Serial.println(F("HTTP server started"));

    if (settings.useNTP) {
      uint32_t newTime = updateNTP(settings.ntpServer);
      zegar.readRTC();
      Serial.println(printDateTime(zegar));
      if (!zegar.compareTimeEpoch(newTime, 5)) {
        Serial.println(F("Adjusting time..."));
        zegar.setRTC(newTime);
        zegar.readRTC();
        Serial.println(printDateTime(zegar));
      }
      if (newTime > settings.lastUpdate)
        settings.lastUpdate = newTime;
    }
  }

  File root = SD.open(SETTINGS_FILE , FILE_READ);
  JsonObject& nSet = jsonBuffer.parseObject(root);
  root.close();
  Serial.println(F("\nSensors:"));
  int nSensor = nSet["sensor"].size();
  for (int a = 0; a < nSensor; a++) {
    Serial.print((String)(a + 1) + "/" + (String)nSensor + ": ");
    String xdtmp = nSet["sensor"][a][0];
    Serial.print(xdtmp);
    if (getTemp(nSet, a) != -127)
      Serial.println(F(" Connected"));
    else
      Serial.println(F(" N/C"));
  }
  saveSettings("/set.dat", settings);
  Serial.println();
}

void loop() {
  server.handleClient();

  while (1) {
    while (((zegar.minute() % 5) != 0) && !digitalRead(SD_D)) {
      zegar.readRTC();
      httpserver ? server.handleClient() : delay(10);
    }

    if (!digitalRead(SD_D)) {
      sensors.requestTemperatures();
      Serial.println("Zapis");
      if (!(zegar.day() > 31 || zegar.month() > 12 || zegar.minute() > 59 || zegar.hour() > 23)) {
        String path = "archiwum/" + (String)zegar.year();
        if (zegar.month() < 10) path += "0";
        path += (String)zegar.month();
        char* path2 = new char[path.length() + 1];
        strcpy(path2, path.c_str());
        SD.mkdir(path2);
        path += "/";
        if (zegar.day() < 10) path += "0";
        path += (String)zegar.day() + ".csv";
        path2 = new char[path.length() + 1];
        strcpy(path2, path.c_str());

        File root = SD.open(SETTINGS_FILE , FILE_READ);
        JsonObject& nSet = jsonBuffer.parseObject(root);
        root.close();

        File dest = SD.open(path, FILE_WRITE);
        dest.print(printDateTime(zegar) + ";");
        dest.flush();

        for (int c = 0; c < nSet["sensor"].size(); c++) {
          String tempname = nSet["sensor"][c][0];
          double tempRead = getTemp(nSet, c);
          if (tempRead != -127) {
            dest.print(tempname);
            dest.print(F("="));
            dest.print(tempRead, 1);
            dest.print(F(";"));
            dest.flush();
          }
        }
        dest.print(F("\r\n"));
        dest.flush();
        dest.close();
      }
    } else {
      Serial.println(F("NO SDCARD!"));
      while (digitalRead(SD_D)) delay(1000);
      Serial.println(F("SD Card found. Reboot..."));
      ESP.restart();
    }

    while (((zegar.minute() % 5) == 0) && !digitalRead(SD_D)) {
      zegar.readRTC();
      httpserver ? server.handleClient() : delay(10);
    }
  }
}

unsigned long updateNTP(char* ntpServerName) {
  WiFiUDP udp;
  IPAddress timeServerIP;
  udp.begin(2390);
  delay(1000);
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

  udp.beginPacket(timeServerIP, 123);
  udp.write(packetBuffer, 48);
  udp.endPacket();

  delay(1000);
  if (udp.parsePacket()) {
    udp.read(packetBuffer, 48);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    return (secsSince1900 - 2208988800UL + (settings.timezone * 3600));
  }
  else return false;
}

void devconfig() {
  Serial.println();
  if (server.method() == HTTP_POST) {
    if (server.arg("action") == "load" && server.args() == 1) {
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      StaticJsonBuffer<500> output;
      JsonObject& data = jsonBuffer.createObject();
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
      //data["currTime"] = zegar.dateToEpoch();
      String resp;
      data.printTo(resp);
      server.sendContent(resp);
      data.prettyPrintTo(Serial);
    }
    if (server.arg("action") == "save") {
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
      if (server.arg("ntpServer") != "")
        strcpy(settings.ntpServer, server.arg("ntpServer").c_str());
      if (server.arg("outDHCP") != "")
        settings.outDHCP = toBool(server.arg("outDHCP"));
      if (server.arg("useNTP") != "")
        settings.useNTP = toBool(server.arg("useNTP"));
      if (server.arg("selfAP") != "")
        settings.selfAP = toBool(server.arg("selfAP"));
      if (server.arg("lastUpdate") != "") {
        uint32_t newLast = strtoul(server.arg("lastUpdate").c_str(), NULL, 10);
        if (newLast > settings.lastUpdate)
          settings.lastUpdate = newLast;
      }
      if (server.arg("timezone") != "")
        settings.timezone = atoi(server.arg("timezone").c_str());
      saveSettings("/set.dat", settings);
      server.sendContent(F("\nHTTP/1.1 303 See Other\r\n"));
      server.sendContent(F("Location: /settings\r\n"));
    }
    if (server.arg("action") == "test") {
      Serial.println(server.uri());
      for (int x = server.args() - 1; x >= 0; x--) {
        Serial.print(server.argName(x));
        Serial.print("=");
        Serial.println(server.arg(x));
      }
      server.sendContent(F("\nHTTP/1.1 303 See Other\r\n"));
      server.sendContent(F("Location: /settings\r\n"));
    }
    if (server.arg("action") == "reset") {
      SPIFFS.begin();
      SPIFFS.remove("/set.dat");
      SPIFFS.end();
      ESP.restart();
    }
  } else loadFromSdCard("/set.htm");
}

void timeset() {
  /*server.sendContent(F("HTTP/1.1 307 Temporary Redirect\r\n"));
    server.sendContent(F("Location: /\r\n"));
    server.sendContent(F("Connection: Close\r\n\r\n"));*/
  Serial.println("Timeset");
  for (int x = server.args() - 1; x >= 0; x--) {
    Serial.print(server.argName(x));
    Serial.print("=");
    Serial.println(server.arg(x));
  }
}

void sensorSettings() {
  sensors.requestTemperatures();
  DynamicJsonBuffer sensBuff(1000);
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
        JsonArray& setMain = sensSet["sensor"];
        setMain.remove((atoi((server.arg(x)).c_str())));
        if ((server.arg(x) == server.argName(x - 1)) && x > 0)
          x -= 1;
      } else {
        changePos = atoi((server.argName(x)).c_str());
        changeName = server.arg(x);
        String currentName = sensSet["sensor"][changePos][0];
        if (currentName != changeName)
          sensSet["sensor"][changePos][0] = changeName;
      }
    }
    server.sendContent(F("HTTP/1.1 303 See Other\r\n"));
    server.sendContent(F("Location: /sensors\r\n"));
    saveJson(sensSet);
  } else if (server.method() == HTTP_GET && server.args() == 0) {

    uint8_t i;
    uint8_t addr[8];
    uint8_t sensorRow;

    server.sendContent(F("<!DOCTYPE html><html><head><title>Sensors</title>"));
    server.sendContent(F("<style>table, th, td{border: 1px solid black; "));
    server.sendContent(F("text-align: center}</style></head>"));
    server.sendContent(F("<body><table><caption><b>Available sensors </b > </caption>"));
    server.sendContent(F("<tr><th>Address (DEC) </th> <th>Temperature </th><th>Name</th>"));
    server.sendContent(F("<th>Name Settings </th>"));
    server.sendContent(F("<form id=\"sensors\" method=\"POST\" action=\"/sensors\">"));

    for (int a = 0; a < sensSet["sensor"].size(); a++) {
      server.sendContent(F("<tr><td>"));
      for (int b = 0; b < 8; b++)
        addr[b] = sensSet["sensor"][a][b + 1];

      for (i = 0; i < 8; i++) {
        if (addr[i] < 100) server.sendContent(F("0"));
        if (addr[i] < 10) server.sendContent(F("0"));
        server.sendContent((String)addr[i]);
        if (i < 7)
          server.sendContent(F(","));
      }
      server.sendContent(F("</td><td>"));
      if (getTemp(sensSet, a) != -127)
        server.sendContent((String)sensors.getTempC(addr) + (char)176 + "C");
      else
        server.sendContent(F("N/C"));

      server.sendContent(F("</td><td>"));
      String tempName = sensSet["sensor"][a][0];

      server.sendContent(tempName);
      server.sendContent(F("</td><td><input type = \"text\" name = "));
      server.sendContent(String(a)); //Position in file
      server.sendContent(F(" value = "));
      server.sendContent(tempName);
      server.sendContent(F("><input type=\"checkbox\" name=\"Remove\" value= "));
      server.sendContent(String(a));
      server.sendContent(F(">Remove</td></tr>"));
    }
    bool anyNew = false;
    while (oneWire.search(addr)) {
      if (isMember(addr, sensSet))
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
      JsonArray& setMain = sensSet["sensor"];
      JsonArray& newSet = setMain.createNestedArray();
      newSet.add(tempName);
      for (int x = 0; x < 8; x++)
        newSet.add(addr[x]);
      sensorRow = sensSet["sensor"].size() - 1;

      server.sendContent((String)sensors.getTempC(addr) + (char)176 + "C");
      server.sendContent(F("</td><td><input type = \"text\" name = "));
      server.sendContent(String(sensorRow)); //Position in file
      server.sendContent(F(" value = New"));
      server.sendContent(F("><input type=\"checkbox\" name=\"Remove\" value= "));
      server.sendContent(String(sensorRow)); //Position in file
      server.sendContent(F("></td></tr>"));
    }
    server.sendContent(F("<button type=\"submit\" value=\"Save\">Save Changes</button>"));
    server.sendContent(F("<button type=\"reset\" value=\"Reset\">Reset</button>"));
    server.sendContent(F("</form></table></body></html>"));
    if (anyNew) saveJson(sensSet);
  }
  return;
}

void updatetime() {
  /*Serial.print(F("Aktualizacja czasu!\nCzas przed: "));
    Serial.println(printDateTime(zegar));

    Serial.print(F("Czas po: "));
    Serial.println(printDateTime(zegar));*/

  server.sendContent(F("HTTP/1.1 307 Temporary Redirect\r\n"));
  server.sendContent(F("Location: /\r\n"));
  server.sendContent(F("Connection: Close\r\n\r\n"));
}

bool wifiConn () {
  if (settings.selfAP) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(settings.selfSSID, settings.selfWPA2);
  }
  else WiFi.mode(WIFI_STA); //WIFI_AP, WIFI_STA, WIFI_AP_STA
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
  server.send(200, F("text/plain"), F(""));
}

void returnFail(String msg) {
  server.send(500, F("text/plain"), msg + "\r\n");
}

void returnForbidden() {
  server.send(403, F("text/plain"), F("Access Denied\r\n"));
}

bool loadFromSdCard(String path) {
  if (!server.authenticate(settings.selfLOGIN, settings.selfPASS)) {
    server.sendHeader(F("WWW-Authenticate"), F("Basic realm=\"Login Required\""));
    server.send(401);
    return false;
  }

  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";

  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".htm")) dataType = F("text/html");
  else if (path.endsWith(".css")) dataType = F("text/css");
  else if (path.endsWith(".js")) dataType = F("application/javascript");
  else if (path.endsWith(".png")) dataType = F("image/png");
  else if (path.endsWith(".gif")) dataType = F("image/gif");
  else if (path.endsWith(".jpg")) dataType = F("image/jpeg");
  else if (path.endsWith(".ico")) dataType = F("image/x-icon");
  else if (path.endsWith(".xml")) dataType = F("text/xml");
  else if (path.endsWith(".pdf")) dataType = F("application/pdf");
  else if (path.endsWith(".zip")) dataType = F("application/zip");
  else if (path.endsWith(".csv")) dataType = F("text/txt");

  File dataFile = SD.open(path.c_str());
  if (dataFile.isDirectory()) {
    path += "/index.htm";
    dataType = F("text/html");
    dataFile = SD.open(path.c_str());
  }

  if (!dataFile)
    return false;

  if (server.hasArg("download")) dataType = F("application/octet-stream");

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    Serial.println(F("Sent less data than expected!"));
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

void deleteRecursive(String path) {
  File file = SD.open((char *)path.c_str());
  if (!file.isDirectory()) {
    file.close();
    SD.remove((char *)path.c_str());
    return;
  }

  file.rewindDirectory();
  while (true) {
    File entry = file.openNextFile();
    if (!entry) break;
    String entryPath = path + "/" + entry.name();
    if (entry.isDirectory()) {
      entry.close();
      deleteRecursive(entryPath);
    } else {
      entry.close();
      SD.remove((char *)entryPath.c_str());
    }
    yield();
  }

  SD.rmdir((char *)path.c_str());
  file.close();
}

void handleDelete() {
  if (server.args() == 0) return returnFail(F("BAD ARGS"));
  String path = server.arg(0);
  if (path == "/" || !SD.exists((char *)path.c_str())) {
    returnFail(F("BAD PATH"));
    return;
  }
  deleteRecursive(path);
  returnOK();
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
  if (!server.hasArg("dir")) return returnFail(F("BAD ARGS"));
  String path = server.arg("dir");
  if (path != "/" && !SD.exists((char *)path.c_str()) || path == "/settings.txt")
    return returnFail(F("BAD PATH"));
  File dir = SD.open((char *)path.c_str());
  path = String();
  if (!dir.isDirectory()) {
    dir.close();
    return returnFail(F("NOT DIR"));
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/json"), "");
  WiFiClient client = server.client();

  server.sendContent(F("["));
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    String forb = String(entry.name());
    forb.toLowerCase();
    if (forb == (String)"settings.txt") {
      cnt--;
      continue;
    }
    if (!entry)
      break;

    String output;
    if (cnt > 0)
      output = ',';

    output += F("{\"type\":\"");
    output += (entry.isDirectory()) ? "dir" : "file";
    output += F("\",\"name\":\"");
    output += entry.name();
    output += F("\"");
    output += F("}");
    server.sendContent(output);
    entry.close();
  }
  server.sendContent(F("]"));
  dir.close();
}

void handleNotFound() {
  bool loadResult = loadFromSdCard(server.uri());
  if (!digitalRead(SD_D) && loadResult) return;

  String message = F("Handle not found\n\n");
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
  server.send(404, F("text/plain"), message);
  Serial.print(message);
}
