#include <ArduinoJson.h> //github: bblanchon
#include <Czas.h> //PCF8563 library
#include <ESP8266HTTPUpdateServer.h>
//#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include <Muxtemp.h>
#include <Pinger.h> //Alessio Leoncini
#include <SD.h>
#include <SPI.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "settingsHandler.h"
#include "html_set.h"

#define FS_NO_GLOBALS
#define DEFAULT_TIMEOUT 1000
#include "FS.h"
#include "defs.h"

/*
  curl -F "image=@Temperature-Station_PCF8563.ino.nodemcu.bin" -u admin:admin 192.168.2.98/update

  Pinger 1.0.0
  LwIP v2 Higher Bandwidth
  You need to generate x509 key using script somewhere in ESP examples
  When setting time - use GMT+0 time without DST  (DST will be automatic)

  ToDO:
    Remake /sensors button to sync 1wire with all devices
    Select if 1wire can override port name
    Select if master can overwrite slave settings
    Add backup options
    Automatic DST
    Secure connection with slave but dosen't need to be
    Move index to be hard-coded
    SET: require authentication, confirm password to change it or username
    Finish backend
    Encrypt passwords
*/
#define MAX_SENSORS 16

bool httpserver = false;
bool letni = true;
uint8_t ports = 0;
uint32_t nextAvailable = 0; //Solve this in Muxtemp only when scanning ports!
const uint32_t minDelay = 10000;

ESP8266WebServer server(80);
WiFiUDP udp;
//ESP8266WebServerSecure server(443);
ESP8266HTTPUpdateServer updServer;

WiFiClient telnet;
WiFiClient debugClient;
WiFiServer debugServer(23);

Czas zegar(Wire);
Muxtemp ext(Wire);
settingsHandler settings;

void setup() {
  pinMode(SD_D, INPUT);
  pinMode(0, OUTPUT);
  digitalWrite(2, LOW);

  //For testing purposes
  //ESP.eraseConfig();
  //SPIFFS.format();

  delay(1000);
  Wire.begin(SDA, SCL);
  Serial.begin(115200);
  Serial.println(F("Temperature Station"));
  Serial.setDebugOutput(false);
  WiFi.persistent(false);

  ///////////////////////////////
  IPAddress ip(192, 168, 2, 98);
  IPAddress gateway(192, 168, 2, 1);
  IPAddress subnet(255, 255, 255, 0);
  settings.configAP("TemperatureStation", "TemperatureStation");
  settings.useDHCP(false);
  settings.name("TemperatureStation");
  settings.configIP(ip, gateway, subnet);
  settings.ntpServer("tempus1.gum.gov.pl");
  settings.configUser("admin", "admin");
  settings.timezone = 1;
  settings.useNTP = true;
  Serial.println("Saving");
  settings.save("/set.txt");
  ///////////////////////////////

  Serial.print(F("Loading... "));
  settings.serialDebug(&Serial);
  if (!settings.load("/set.txt")) { //Defaults
    Serial.println(F("Fail"));
    settings.configUser("admin", "admin");
    settings.name("TemperatureStation");
    settings.configAP(("TemperatureStation" + (String)ESP.getChipId()).c_str(), "TemperatureStation");
  } else Serial.println(F("OK"));

  bool gotwifi = false;
  if (strcmp(settings.ssid(), "") != 0) {
    Serial.print(F("Connecting to:\r\n\t\t"));
    Serial.print(settings.ssid());
    Serial.print(F("... "));
    if (settings.beginWiFi()) {
      Serial.print(F("Success ("));
      Serial.print(WiFi.localIP());
      Serial.println(F(")"));
      gotwifi = true;
    } else {
      Serial.println(F("Connection failed"));
      gotwifi = false;
    }
  }
  if (!gotwifi) {
    Serial.println(F("Starting Access Point..."));
    settings.beginAP();
    Serial.print(F("Connect to:\t"));
    Serial.println(settings.ssidAP());
  }
  Serial.print(F("\r\nMuxtemp:\t"));
  Serial.println((ext.begin(0x10) == 0) ? F("Present") : F("Error"));
  Serial.print(F("Ports:  \t"));
  Serial.println(ext.getCount());
  Serial.print(F("1wire bypass:\t"));
  Serial.println(ext.bypass1Wire() ? F("yes") : F("no"));
  Serial.println();
  ext.refreshPorts();
  nextAvailable = millis() + minDelay;

  if (analogRead(SD_D) < 512) {
    if (SD.begin(SD_CS)) {
      Serial.println(F("SD Card initialized"));
      if (!SD.exists("/SENSORS.TXT"))
        Serial.println(F("No sensors file!"));
    } else Serial.println(F("SD Card detected but cannot be initialized!"));
  } else Serial.print(F("No card inserted\n"));

  settings.configUpdateServer(&server, &updServer, "/update");
  //server.setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));
  server.on("/settings", devconfig);
  server.on("/sensors", sensorSettings);
  server.on("/sensorsbdc", sensorBroadcast);
  server.on("/time", HTTP_POST, timeset);
  server.on("/list", HTTP_GET, printDirectory);
  server.on("/sysinfo", HTTP_GET, httpinfo);
  server.on("/", HTTP_DELETE, handleDelete);
  server.on("/", HTTP_PUT, handleCreate);
  server.on("/", HTTP_POST, []() {
    returnOK();
  }, handleFileUpload);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println(F("HTTP server:\tStarted"));
  debugServer.begin();
  //This has no sense
  //But still it looks good while booting :)

  //Emergency connection - update or something
  //In case when dosen't detect SDcard, wrong config etc

  udp.begin(2390);
  zegar.readRTC();
  if (settings.useNTP && gotwifi) {
    Serial.print(F("NTP server:\t"));
    Serial.println(settings.ntpServer());
    uint32_t newTime = updateNTP(settings.ntpServer());
    Serial.print(F("NTP Time:\t"));
    Serial.println(newTime);
    Serial.print(F("Current Time:\t"));
    Serial.println(zegar.dateAsEpoch());
    if (newTime != 0 && ((newTime - zegar.dateAsEpoch()) > 59)) {
      Serial.println(F("Adjusting time..."));
      zegar.setRTC(newTime);
      delay(10);
      zegar.readRTC();
      settings.lastUpdate = newTime;
    }
    settings.save("/set.txt");
  }
  if (zegar.dateAsEpoch() < 59) zegar.setRTC(settings.lastUpdate);
  Serial.println(printDateTime(zegar));

  while (millis() < (nextAvailable - 5000))
    server.handleClient();

  refreshSensors(&ext, "/SENSORS.TXT");
  nextAvailable = millis() + minDelay;

  DynamicJsonBuffer jsonBuffer(2250);
  File root = SD.open("/SENSORS.TXT", FILE_READ);
  JsonObject& nSet = jsonBuffer.parseObject(root);
  root.close();

  JsonArray& _saved = nSet["saved"];
  Serial.println(F("\r\nLocal:"));
  for (int a = 0; a < ext.getCount(); a++) {
    uint8_t _type = ext.typeOf(a);
    JsonObject& _curr = nSet["local"][a];

    Serial.print('\t' + (String)(a + 1) + "/" + (String)(ext.getCount()) + ": ");
    Serial.print('\t' + String(ext.typeOf(a)) + '\t');
    if (_type == 0) Serial.print(F("N/C"));
    else if (_type != 5) {
      Serial.println(_curr["n"].as<const char*>());
      continue;
    } else if (_type == 5) {
      uint8_t *_q = ext.getAddress(a);
      int pos = isMember(_q, _saved, 8);
      Serial.print(_saved[pos]["n"].as<const char*>());
      Serial.print(F("\t("));

      for (uint8_t y = 0; y < 8; y++) {
        Serial.print(*(_q + y));
        if (y < 7) Serial.print(",");
        else Serial.print(')');
      }
    }
    Serial.println();
  }

  for (uint8_t rtd = 0; rtd < nSet["remote"].size(); rtd++) {
    JsonArray& _target = nSet["remote"][rtd];
    int32_t pingresp = Pinger::Ping(_target[1].as<const char*>());
    if (pingresp <= 0) {
      Serial.print(F("Device \""));
      Serial.print(_target[0].as<String>());
      Serial.print(F("\" at "));
      Serial.print(_target[1].as<String>());
      Serial.println(F(" is offline"));
      rtd++;
      continue;
    }

    String data;
    bool succ;
    Serial.println(F("\r\nSending config"));
    JsonArray& _saved = nSet["saved"];
    _saved.printTo(data);
    _saved.printTo(Serial);
    succ = sendToSlave('\x4B' + data, _target[1].as<const char*>());
    Serial.println(succ ? "suc" : "nah");
    yield();
  }

  /*
    //Refreshing slaves config
    uint8_t rtd = 0;
    while (rtd < nSet["remote"].size() && gotwifi) {
      JsonArray& locdata = nSet["remote"][rtd]["loc"];
      int32_t pingresp = Pinger::Ping(locdata[1].as<const char*>());
      if (pingresp <= 0) {
        Serial.print(F("Device \""));
        Serial.print(locdata[0].as<String>());
        Serial.print(F("\" at "));
        Serial.print(locdata[1].as<String>());
        Serial.println(F(" is offline"));
        rtd++;
        continue;
      }

      bool succ;

      //Refresh master config
      Serial.println(F("\r\nRequest for sensor list"));
      String xd;
      xd = getFromSlave(0x4E, locdata[1].as<const char*>());
      DynamicJsonBuffer remSensBuf(400);
      JsonArray& remSens = remSensBuf.parseArray(xd);
      if (!remSens.success()) Serial.println(F("Unable to fetch data"));
      else {
        for (uint8_t a = 0; a < remSens.size(); a++) {
          JsonObject& entry = remSens[a];
          JsonArray& remdir = nSet["remote"];
          JsonArray& devs = remdir[0]["dev"];

          int entryCode = isMember(devs, entry);
          if (entryCode == -1) { //Create entry for new sensor
            JsonObject& newEntry = devs.createNestedObject();
            newEntry["n"] = entry["n"];
            newEntry["a"] = entry["a"];
          }
        }
      }
      saveJson(nSet, "/SENSORS.TXT");

      //Send configs to slaves


      rtd++;
    }


    nSensor = nSet["remote"].size();
    Serial.println("\nRemote:");
    for (uint8_t a = 0; a < nSensor; a++) {
      Serial.print("\t" + nSet["remote"][a]["loc"][0].as<String>());
      Serial.println(" : " + nSet["remote"][a]["loc"][1].as<String>());
      for (uint8_t x = 0; x < nSet["remote"][a]["dev"].size(); x++)
        Serial.println("\t\t*" + nSet["remote"][a]["dev"][x]["n"].as<String>());
    }


    String data = "\x4B[";
    for (int q = 0; q < nSet["remote"][0]["dev"].size(); q++) {
      if (q > 0) data += ', ';
      data += nSet["remote"][0]["dev"][q].as<String>();
    }
    data += ']';

  */
  yield();
}

bool doIdle = true;
bool outLoop = false;

void loop() {
idle:
  if (debugServer.hasClient() && (!debugClient || !debugClient.connected())) {
    debugClient = debugServer.available();
    Serial.print(F("New debug client\r\n"));
    debugClient.println(F("New debug client"));
    debugClient.println(printDateTime(zegar));
  }
  if (debugClient.available()) debugClient.write(debugClient.read());
  zegar.readRTC();
  server.handleClient();
  yield();

  if (millis() >= nextAvailable) { //refresh every time possible
    //We can refresh remote device without refreshing them in file
    if (analogRead(SD_D) < 512)
      refreshSensors(&ext, "/SENSORS.TXT");
    ext.refreshPorts();
    nextAvailable = millis() + minDelay; //Change if needed to bigger value
  }

  if ((zegar.minute() % 5) == 0 && !outLoop) {
    doIdle = false;
    outLoop = false;
  } else if ((zegar.minute() % 5) != 0) outLoop = false;

  yield();
  if (doIdle) goto idle;
  yield();

  if (analogRead(SD_D) < 512) {
    DynamicJsonBuffer jsonBuffer(1000);
    File root = SD.open("/SENSORS.TXT", FILE_READ);
    JsonObject& setts = jsonBuffer.parseObject(root);
    root.close();

    Serial.println(F("Zapis"));
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

      JsonArray& nSet = setts["local"];

      File dest = SD.open(path, FILE_WRITE);
      dest.print(printDateTime(zegar) + ";");
      dest.print(settings.name());
      dest.print(F(":("));
      dest.flush();
      for (uint8_t c = 0; c < ext.getCount(); c++) {
        Serial.print(String(c + 1) + "/" + String(ext.getCount()));
        Serial.print('\t');
        Serial.print(ext.typeOf(c));
        int pos;
        String _name;
        switch (ext.typeOf(c)) {
          case 0:
            Serial.println();
            continue;
          case 5: pos = nSet[c]["c"].as<int>();
            _name = setts["saved"][pos]["n"].as<String>();
            break;
          case 11:
          case 21:
          case 22:
          default: pos = c;
            _name = nSet[c]["n"].as<String>();
            break;
        }

        double tempRead = ext.getTemp(c);
        Serial.print('\t' + String(tempRead));
        Serial.println('\t' + _name);
        if (c > 0) dest.print(F(","));
        dest.print(_name);
        dest.print(F("="));
        dest.print(tempRead, 1);
        dest.flush();
      }
      dest.println(");");

      for (uint8_t rtd = 0; rtd < setts["remote"].size(); rtd++) {
        JsonArray& locdata = setts["remote"][rtd];
        int32_t pingresp = Pinger::Ping(locdata[1].as<const char*>());
        if (pingresp <= 0) {
          Serial.print(F("Device \""));
          Serial.print(locdata[0].as<String>());
          Serial.print(F("\" at "));
          Serial.print(locdata[1].as<String>());
          Serial.println(F(" is offline"));
          continue;
        }

        bool suc = sendToSlave("\x51", locdata[1].as<const char*>());
        Serial.println(suc ? "suc" : "nah");

        String xdd = "";
        xdd = getFromSlave(0x54, locdata[1].as<const char*>());
        Serial.println(xdd);
        DynamicJsonBuffer readBuf(300);
        JsonArray& readVal = readBuf.parseArray(xdd);
        dest.print(locdata[0].as<String>());
        dest.print(F(":("));
        bool first = true;
        for (uint8_t a = 0; a < readVal.size(); a++) {
          if (!first) dest.print(F(","));
          else first = false;
          dest.print(readVal[a][0].as<String>());
          dest.print('=');
          dest.print(readVal[a][1].as<double>(), 1);
          dest.flush();
        }
        dest.println(F(");"));
      }
      dest.flush();

      dest.close();

    }
    yield();
  } else {
    Serial.println(F("NO SDCARD!"));
    while (digitalRead(SD_D)) delay(1000);
    Serial.println(F("SD Card found. Reboot..."));
    ESP.restart();
  }
  doIdle = true;
  outLoop = true;
}

bool sendToSlave(String _data, const char* _loc) {
  if (telnet.connect(_loc, 23)) {
    Serial.println("Wysylam dane: " + String(_loc));
    Serial.println(_data);
    telnet.write(_data.c_str(), _data.length());
    telnet.flush();

    char com = '\x15';
    uint32_t tt = millis();
    while (millis() - tt < DEFAULT_TIMEOUT) yield();
    while (telnet.available()) {
      Serial.print(telnet.peek(), HEX);
      Serial.print(F("|"));
      Serial.write(telnet.peek());
      Serial.print(F(" "));
      com = (char)telnet.read();
    }
    telnet.stop();
    if (com == '\x06')
      return true;
    else Serial.println(F("Blad komunikacji"));
  } else Serial.println("Nie mozna polaczyc z " + String(_loc));
  return false;
}

String getFromSlave(int _code, const char* _loc) {
  if (telnet.connect(_loc, 23)) {
    Serial.println("Sending request: " + String(_loc));
    telnet.write(_code);
    telnet.flush();

    String com = "";
    com.reserve(250);
    uint32_t tt = millis();
    while (millis() - tt < DEFAULT_TIMEOUT && !telnet.available()) yield();
    if (telnet.available()) {
      while (telnet.available())
        com += (char)telnet.read();
      telnet.stop();
      return com;
    } else Serial.println(F("Communication error"));
    telnet.stop();
  } else Serial.println("Unable to connect to " + String(_loc));
  return "NO";
}

uint32_t updateNTP(const char* ntpServerName) {
  IPAddress timeServerIP;
  WiFi.hostByName(ntpServerName, timeServerIP);
  delay(50);
  if (IPAddress(0, 0, 0, 0) == timeServerIP) return false;
  Serial.print(F("Timeserver IP:\t"));
  Serial.println(IPtoString(timeServerIP));

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

  int cb;
  uint8_t i = 0;

  do {
    udp.beginPacket(timeServerIP, 123);
    udp.write(packetBuffer, 48);
    udp.flush();
    udp.endPacket();
    delay(1000);
    cb = udp.parsePacket();
  } while (!cb && i++ < 10);

  udp.read(packetBuffer, 48);

  if (cb) {
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    secsSince1900 = secsSince1900 - 2208988800UL;
    secsSince1900 += settings.timezone * 3600;
    if (letni) secsSince1900 += 3600;
    return secsSince1900;
  } else return false;
}

void devconfig() {
  Serial.println(F("Entering devconfig"));
  if (server.method() == HTTP_POST) {
    if (server.arg("action") == "load" && server.args() == 1) {
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      StaticJsonBuffer<500> output;
      JsonObject& data = output.createObject();
      data["selfName"] = settings.name();
      data["selfSSID"] = settings.ssidAP();
      data["outSSID"] = settings.ssid();
      data["outIP"] = IPtoString(settings.localIP());
      data["outGW"] = IPtoString(settings.gatewayIP());
      data["outMASK"] = IPtoString(settings.subnetMask());
      data["outDHCP"] = settings.useDHCP();
      data["login"] = settings.username();
      data["useNTP"] = settings.useNTP;
      data["ntpServer"] = settings.ntpServer();
      data["timezone"] = settings.timezone;
      data["currTime"] = zegar.dateAsEpoch();
      String resp;
      data.printTo(resp);
      server.sendContent(resp);
      server.sendContent("\x00");
      data.prettyPrintTo(Serial);
    }
    if (server.arg("action") == "save") {
      /*if (server.arg("selfName") != "")
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
        if (server.arg("epoch") != "NaN") {
        uint32_t newLast = strtoul(server.arg("lastUpdate").c_str(), NULL, 10);
        newLast += 3600 * settings.timezone + 3600 * (int)letni;
        Serial.println(newLast);
        if (newLast > settings.lastUpdate)
        settings.lastUpdate = newLast;
        }
        if (server.arg("timezone") != "")
        settings.timezone = atoi(server.arg("timezone").c_str());
        SPIFFS.begin();
        saveSettings((char*)"/set.dat", settings);
        SPIFFS.end();
        server.sendContent(F("\nHTTP/1.1 303 See Other\r\n"));
        server.sendContent(F("Location: /settings\r\n"));*/
    }
    if (server.arg("action") == "test") {
      Serial.println(server.uri());
      for (int x = server.args() - 1; x >= 0; x--) {
        Serial.print(server.argName(x));
        Serial.print(F("="));
        Serial.println(server.arg(x));
      }
      server.sendContent(F("\nHTTP/1.1 303 See Other\r\n"));
      server.sendContent(F("Location: /settings\r\n"));
    }
    if (server.arg("action") == "reset") {
      SPIFFS.begin();
      SPIFFS.remove("/set.txt");
      SPIFFS.end();
      ESP.restart();
    }
  } else loadFromSdCard("/set.htm");
  //server.send(200, "text/html", html_settings);
}

void timeset() {
  if (server.arg("action") == "test") {
    Serial.println(server.uri());
    for (int x = server.args() - 1; x >= 0; x--) {
      Serial.print(server.argName(x));
      Serial.print(F("="));
      Serial.println(server.arg(x));
    }
  } else if (server.arg("action") == "save") {
    if (server.arg("timezone") != "")
      settings.timezone = atoi(server.arg("timezone").c_str());
    if (server.arg("useNTP") != "")
      settings.useNTP = toBool(server.arg("useNTP"));
    if (server.arg("ntpServer") != "")
      settings.ntpServer(server.arg("ntpServer").c_str());
    if (server.arg("epoch") != "NaN") {
      uint32_t newLast = strtoul(server.arg("epoch").c_str(), NULL, 10);
      newLast += 3600 * settings.timezone;
      if (letni) newLast += 3600;
      Serial.println(newLast);
      zegar.setRTC(newLast);
      settings.lastUpdate = newLast;
    }
    Serial.println(F("Zapis czasu"));
    settings.save("/set.txt");
  }
  server.sendContent(F("\nHTTP/1.1 303 See Other\r\n"));
  server.sendContent(F("Location: /settings\r\n"));
}

void sensorBroadcast() {
  DynamicJsonBuffer sensBuff(1000);
  File tmpSens = SD.open("/SENSORS.TXT" , FILE_READ);
  JsonObject& sensSet = sensBuff.parseObject(tmpSens);
  tmpSens.close();

  for (uint8_t rtd = 0; rtd < sensSet["remote"].size(); rtd++) {
    JsonArray& _target = sensSet["remote"][rtd];
    int32_t pingresp = Pinger::Ping(_target[1].as<const char*>());
    if (pingresp <= 0) {
      Serial.print(F("Device \""));
      Serial.print(_target[0].as<String>());
      Serial.print(F("\" at "));
      Serial.print(_target[1].as<String>());
      Serial.println(F(" is offline"));
      rtd++;
      continue;
    }

    String data;
    bool succ;
    Serial.println(F("\r\nSending config"));
    JsonArray& _saved = sensSet["saved"];
    _saved.printTo(data);
    _saved.printTo(Serial);
    succ = sendToSlave('\x4B' + data, _target[1].as<const char*>());
    Serial.println(succ ? "suc" : "nah");
    yield();
  }
  server.sendContent(F("HTTP/1.1 303 See Other\r\n"));
  server.sendContent(F("Location: /sensors\r\n"));
}

void sensorSettings() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  DynamicJsonBuffer sensBuff(1000);
  File tmpSens = SD.open("/SENSORS.TXT" , FILE_READ);
  JsonObject& sensSet = sensBuff.parseObject(tmpSens);
  tmpSens.close();

  if (!sensSet.success())
    return;

  if (server.args() != 0 && server.method() == HTTP_POST) {
    String changeName;
    int changePos;
    JsonArray& setMain = sensSet["saved"];
    for (int x = server.args() - 1; x >= 0; x--) {
      if (server.argName(x) == "Remove") {
        setMain.remove((atoi((server.arg(x)).c_str())));
        if ((server.arg(x) == server.argName(x - 1)) && x > 0)
          x -= 1;
      } else {
        changePos = atoi((server.argName(x)).c_str());
        changeName = server.arg(x);
        if (setMain[changePos]["n"].as<String>() != changeName)
          setMain[changePos]["n"] = changeName;
      }
    }
    server.sendContent(F("HTTP/1.1 303 See Other\r\n"));
    server.sendContent(F("Location: /sensors\r\n"));
    saveJson(sensSet, "/SENSORS.TXT");
  } else if (server.method() == HTTP_GET && server.args() == 0) {
    server.sendContent(F("<!DOCTYPE html><html><head><title>Sensors</title>"));
    server.sendContent(F("<style>table,th,td{border: 1px solid black;"));
    server.sendContent(F("text-align: center}</style></head>"));
    server.sendContent(F("<body><table><caption><b>Available sensors</b></caption>"));
    server.sendContent(F("<tr><th>Type</th><th>Name</th>"));
    server.sendContent(F("<th>Address/Port</th><th>Temperature</th></tr>"));

    for (uint8_t a = 0; a < ext.getCount(); a++) {
      int pos = a;
      String _name;
      if (ext.typeOf(a) == 5) {
        pos = sensSet["local"][a]["c"];
        _name = sensSet["saved"][pos]["n"].as<String>();
      } else if (ext.typeOf(a) != 0)
        _name = sensSet["local"][pos]["n"].as<String>();
      else _name = "N/C";
      server.sendContent(F("<tr><td>"));
      server.sendContent(String(ext.typeOf(a)));
      server.sendContent(F("</td><td>"));
      server.sendContent(_name);
      server.sendContent(F("</td><td>"));
      if (ext.typeOf(a) == 5)
        server.sendContent(sensSet["saved"][pos]["a"]);
      else if (ext.typeOf(a) > 0)
        server.sendContent(String(a));
      else server.sendContent(F("N/C"));
      server.sendContent(F("</td><td>"));

      if (ext.typeOf(a) != 0)
        server.sendContent((String)(ext.getTemp(a)) + (char)176 + "C");
      else server.sendContent(F("N/C"));
      server.sendContent(F("</td></tr>"));
    }

    server.sendContent(F("</table><table><caption><b>Name Settings</b></caption>"));
    server.sendContent(F("<form id=\"sensors\" method=\"POST\" action=\"/sensors\">"));
    server.sendContent(F("<th>Name</th><th>Address/Port</th><th>Remove</th>"));

    for (uint8_t a = 0; a < sensSet["saved"].size(); a++) {
      String _name = sensSet["saved"][a]["n"].as<String>();
      server.sendContent(F("<tr><td><input type = \"text\" name = "));
      server.sendContent(String(a)); //Position in file
      server.sendContent(F(" value = \""));
      server.sendContent(_name);
      server.sendContent(F("\"><td>"));
      server.sendContent(sensSet["saved"][a]["a"]);
      server.sendContent(F("</td><td><input type=\"checkbox\" name=\"Remove\" value= "));
      server.sendContent(String(a));
      server.sendContent(F(">Remove</td></tr>"));
    }

    server.sendContent(F("</table><button type=\"submit\" value=\"Save\">Save Changes</button>"));
    server.sendContent(F("<button type=\"reset\" value=\"Reset\">Reset</button>"));
    server.sendContent(F("</form><button onclick=\"location.href='/sensorsbdc'\""));
    server.sendContent(F("type=\"button\">Settings</button></body></html>"));
  }
  return void();
}

void httpinfo() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  server.sendContent(F("<!DOCTYPE html><html>\
  <head><title>System info</title></head>"));
  server.sendContent(F("<body><b>Not implemented yet</b>"));
  server.sendContent(F("<a href=\"/\"><button>Strona glowna</button></a>"));
  server.sendContent(F("</body></html>"));
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
  if (!settings.webAuthenticate(&server)) {
    server.requestAuthentication();
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
  if (!settings.webAuthenticate(&server))
    server.requestAuthentication();

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
  if (path != "/" && (!SD.exists((char *)path.c_str()) || path == "/settings.txt"))
    return returnFail(F("BAD PATH"));
  File dir = SD.open((char *)path.c_str());
  path = String();
  if (!dir.isDirectory()) {
    dir.close();
    return returnFail(F("NOT DIR"));
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  //server.send(200, F("text/json"), "");
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
    if (!entry) break;

    String output;
    if (cnt > 0)
      output = ',';

    output += F("{\"type\":\"");
    output += entry.isDirectory() ? F("dir") : F("file");
    output += F("\",\"name\":\"");
    output += entry.name();
    output += F("\"}");
    server.sendContent(output);
    entry.close();
  }
  server.sendContent(F("]"));
  dir.close();
}

void handleNotFound() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  bool loadResult = loadFromSdCard(server.uri());
  if (analogRead(SD_D) < 512 && loadResult) return;

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
