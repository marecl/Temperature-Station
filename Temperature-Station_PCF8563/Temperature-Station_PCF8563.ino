#include <ArduinoJson.h> //github: bblanchon
#include <ArduinoOTA.h>
#include <Czas.h> //PCF8563 library
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
//#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include <Muxtemp.h>
#include <pgmspace.h>
#include <Pinger.h> //Alessio Leoncini
#include <SD.h>
#include <SPI.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#define FS_NO_GLOBALS
#include "FS.h"
#include "defs.h"
#include "settingsHandler.h"

/* Software Version */
#define SW_VERSION "1.0.0"
/* Hardware Version */
#define HW_VERSION "1.0.0"

/* Timeout for slave devices */
#define DEFAULT_TIMEOUT 5000

/* How often we want data logged (minutes, 2-59) */
#define M_INTERVAL 5

extern char index_htm[];
extern char settings_htm[];
extern uint8_t x509[];
extern uint8_t rsakey[];

/*
  Pinger 1.1.0
  LwIP v1.4 Higher Bandwidth (due to hostByName error -6 on other LwIP)
  You need to generate x509 key using script somewhere in ESP examples
  When setting time - use GMT+0 time without DST (DST will be automatic)

  [settings] add buttons for fetching and sending sensors and check if works
  [misc] why os it resetting from time to time
  [html settings] show remote devices and their names (+edit)
  [html settings] show local date&time (not UTC)

  ToDO:
  Download all configs, edit and send back (buttons)
  Add backup options
  Automatic DST
  Secure connection with slave but dosen't need to be
  Move index to be hard-coded
  Encrypt passwords
*/

bool letni = true;

WiFiUDP udp;
ESP8266WebServer server(80);
//ESP8266WebServerSecure server(443);
ESP8266HTTPUpdateServer updServer;

//Interface for connecting with slaves
WiFiClient telnet;

//Remote view console
WiFiClient telnetDebug;
WiFiServer telnetDebugServer(23);

Czas zegar(Wire);
Muxtemp ext(Wire, 0x10, MUX_LOCK);
settingsHandler settings(SETTINGS_FILE);
Pinger pinger;

void setup() {
  delay(100);
  SPIFFS.begin();
  pinMode(SD_D, INPUT);
  pinMode(0, OUTPUT);

  //For testing purposes
  /*
    ESP.eraseConfig();
    SPIFFS.format();
  */

  Wire.begin(SDA, SCL);
  Serial.begin(115200);
  Serial.println(F("Temperature Station"));
  Serial.setDebugOutput(false);
  WiFi.persistent(false);

#ifdef S_DEBUG
  settings.serialDebug(&Serial);
  //Use this section to overwrite settings while debugging
  IPAddress ip(192, 168, 2, 98);
  IPAddress gateway(192, 168, 2, 1);
  IPAddress subnet(255, 255, 255, 0);
  settings.configSTA("bosszantogazember", "P!ontk0wyW13cz#r");
  settings.configAP("TemperatureStation", "TemperatureStation");
  settings.useDHCP(false);
  settings.name("TemperatureStation");
  settings.configIP(ip, gateway, subnet);
  settings.ntpServer("tempus1.gum.gov.pl");
  settings.configUser("admin", "admin");
  settings.timezone = 1;
  settings.useNTP = true;
  telnetDebug.println("Saving");
  settings.save();
#endif

  Serial.print(F("Loading... "));
  if (!settings.load()) { //Fill with default values
    Serial.println(F("Fail"));
    settings.configUser("admin", "admin");
    settings.name("TemperatureStation");
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

  Serial.print(F("\r\nMuxtemp:\t"));
  Serial.println((ext.begin() == 0) ? F("Present") : F("Error"));
  Serial.print(F("Ports:\t\t"));
  Serial.println(ext.getCount());
  Serial.print(F("1wire bypass:\t"));
  Serial.println(ext.bypass1Wire() ? F("yes") : F("no"));
  Serial.println();
  ext.refresh();

  if (analogRead(SD_D) < 128) {
    if (SD.begin(SD_CS)) Serial.println(F("SD Card initialized"));
    else Serial.println(F("SD Card detected but cannot be initialized!"));
  } else Serial.print(F("No card inserted\n"));

  settings.beginOTA();
  settings.configUpdateServer(&server, &updServer, "/update");
  //server.setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));
  server.on("/", HTTP_GET, loadindex);
  server.on("/settings", devconfig);
  server.on("/sensors", HTTP_GET, sensorSettings);
  server.on("/editports", HTTP_POST, editports);
  server.on("/editsaved", HTTP_POST, editsaved);
  server.on("/fetchsensors", HTTP_GET, []() {
    mergeSensors();
    server.sendContent(F("HTTP/1.1 303 See Other\r\nLocation:/sensors\r\n"));
  });
  server.on("/sendsensors", HTTP_GET, []() {
    sendSensors();
    server.sendContent(F("HTTP/1.1 303 See Other\r\nLocation:/sensors\r\n"));
  });

  server.on("/time", HTTP_POST, timeset);
  server.on("/list", HTTP_GET, printDirectory);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println(F("HTTP server:\tStarted")); //No sense but looks nice :)
  telnetDebugServer.begin(); //Remote debug interface
  telnet.setNoDelay(true);

  zegar.readRTC();
  udp.begin(2390);
  if (settings.useNTP && gotwifi) {
    Serial.print(F("NTP server:\t"));
    Serial.println(settings.ntpServer());
    uint32_t newTime = updateNTP(settings.ntpServer());
    Serial.print(F("NTP Time:\t"));
    Serial.println(newTime);
    Serial.print(F("Current Time:\t"));
    Serial.println(zegar.dateAsEpoch());
    if ((newTime > 0 && (newTime - zegar.dateAsEpoch()) > 59) || settings.lastUpdate == 0) {
      Serial.println(F("Adjusting time..."));
      zegar.setRTC(newTime);
      delay(5);
      zegar.readRTC();
      settings.lastUpdate = newTime;
    }
    settings.save();
  }
  if (zegar.dateAsEpoch() < 59)
    zegar.setRTC(settings.lastUpdate);
  Serial.println(printDateTime(zegar));

  //Emergency connection
  //In case when it dosen't detect SDcard, wrong config etc
  Serial.println(F("Emergency loading"));
  while (millis() % 10000 > 500 || ext.lock()) {
    ArduinoOTA.handle();
    server.handleClient();
    yield();
  }
  refreshSensors(&ext, SENSORS_FILE);

  printSensors(&ext, Serial);

  mergeSensors();
  sendSensors();

  /*for (uint8_t rtd = 0; rtd < nSet["remote"].size(); rtd++) {
    JsonArray& locdata = nSet["remote"][rtd];
    const char* _n = locdata[0];
    const char* _a = locdata[1];
    IPAddress _target = settings.stringToIP(_a);
    if (pinger.Ping(_target) == -1) {
    Serial.print(F("Device \""));
    Serial.print(locdata[0].as<String>());
    Serial.print(F("\" at "));
    Serial.print(_target);
    Serial.println(F(" is offline"));
    continue;
    }

    Serial.println(F("\r\nSending config"));
    bool succ = sendToSlave('\x4B', nSet["saved"], _target);
    Serial.println(succ ? F("suc") : F("nah"));
    yield();
    }*/

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
    newEntry[0] = entry[0];
    newEntry[1] = entry[1];
    }
    }
    }
    saveJson(nSet, SENSORS_FILE);

    //Send configs to slaves


    rtd++;
    }

    nSensor = nSet["remote"].size();
    Serial.println("\nRemote:");
    for (uint8_t a = 0; a < nSensor; a++) {
    Serial.print("\t" + nSet["remote"][a]["loc"][0].as<String>());
    Serial.println(" : " + nSet["remote"][a]["loc"][1].as<String>());
    for (uint8_t x = 0; x < nSet["remote"][a]["dev"].size(); x++)
    Serial.println("\t\t*" + nSet["remote"][a]["dev"][x][0].as<String>());
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
  if (telnetDebug.available()) telnetDebug.write(telnetDebug.read());
  if (telnetDebugServer.hasClient() && (!telnetDebug || !telnetDebug.connected())) {
    telnetDebug = telnetDebugServer.available();
    telnetDebug.println(F("TemperatureStation Debug View"));
    telnetDebug.print(F("Software Version: "));
    telnetDebug.println(F(SW_VERSION));
    telnetDebug.print(F("Hardware Version: "));
    telnetDebug.println(F(HW_VERSION));
    telnetDebug.println(printDateTime(zegar));
    telnetDebug.print(F("Epoch: "));
    telnetDebug.println(zegar.dateAsEpoch());
    telnetDebug.print(F("Available stack: "));
    telnetDebug.println(ESP.getFreeHeap());

    printSensors(&ext, telnetDebug);
  }

  zegar.readRTC();
  server.handleClient();
  ArduinoOTA.handle();
  yield();

  //refresh when possible but not too often
  if (!ext.lock() && millis() % 10000 < 500) {
    //We can refresh remote device without refreshing them in file
    if (analogRead(SD_D) < 128)
      refreshSensors(&ext, SENSORS_FILE);
    ext.refresh();
  }
  yield();

  if ((zegar.minute() % M_INTERVAL) == 0 && !outLoop) {
    doIdle = false;
    outLoop = false;
  } else if ((zegar.minute() % M_INTERVAL) != 0) outLoop = false;

  yield();
  if (doIdle) goto idle;
  yield();

  while (ext.lock()) { //If muxtemp is not ready, wait
    server.handleClient();
    ArduinoOTA.handle();
    yield();
  }

  if (analogRead(SD_D) < 128) {
    DynamicJsonBuffer jsonBuffer(2250);
    File root = SD.open(SENSORS_FILE, FILE_READ);
    JsonObject& setts = jsonBuffer.parseObject(root);
    JsonArray& nSet = setts["local"];

    if (zegar.isValid()) {
      char *_pa = new char[24];
      memset(_pa, 0, 24);
      snprintf_P(_pa, 24,
                 PSTR("/archiwum/%04u%02u"), zegar.year(), zegar.month());
      SD.mkdir(_pa);
      snprintf_P(_pa, 24,
                 PSTR("%s/%02u.csv"), _pa, zegar.day());
      File dest = SD.open(_pa, FILE_WRITE);

      dest.print(printDateTime(zegar) + ";");
      dest.print(settings.name());
      dest.print(F(":("));
      dest.flush();

      telnetDebug.println();
      telnetDebug.println(printDateTime(zegar));
      bool _f = true;
      for (uint8_t c = 0; c < ext.getCount(); c++) {
        telnetDebug.print(String(c + 1) + '/' + String(ext.getCount()));
        telnetDebug.print('\t');
        telnetDebug.print(ext.typeOf(c));
        int pos;
        String _name;
        switch (ext.typeOf(c)) {
          default: pos = c;
            _name = PSTR("UNKNOWN_TYPE:") + String(c);
            break;
          case 0:
            telnetDebug.println();
            continue;
          case 1:
          case 11:
          case 21:
          case 22: pos = c;
            _name = nSet[c][0].as<String>();
            break;
          case 40: pos = nSet[c][1].as<int>();
            _name = setts["saved"][pos][0].as<String>();
            break;
        }

        double tempRead = ext.getTemp(c);
        telnetDebug.print('\t' + String(tempRead));
        telnetDebug.println('\t' + _name);

        if (tempRead == -127.00 || tempRead == 85.00)
          continue;

        if (!_f) dest.print(F(","));
        else _f = false;
        dest.print(c);
        dest.print(F("="));
        dest.print(_name);
        dest.print(F("="));
        dest.print(tempRead, 1);
        dest.flush();
      }
      dest.println(");");
      Serial.println(F("Remote devices are disabled"));
      /*
        for (uint8_t rtd = 0; rtd < setts["remote"].size(); rtd++) {
        JsonArray& locdata = setts["remote"][rtd];
        const char* _n = locdata[0];
        const char* _a = locdata[1];
        IPAddress _target = settings.stringToIP(_a);
        if (pinger.Ping(_target) == -1)
        continue;

        bool suc = sendToSlave('\x51', "", _target);
        telnetDebug.println(suc ? F("suc") : F("nah"));

        char* xd = new char[250];
        getFromSlave(0x54, _target, xd, 250);

        DynamicJsonBuffer readBuf;
        JsonArray& readVal = readBuf.parseArray(xd);
        dest.print(_n);
        dest.print(F(":("));
        for (uint8_t a = 0; a < readVal.size(); a++) {
        JsonArray& _e = readVal[a];
        if (a != 0)
        dest.print(F(","));
        dest.print(_e[0].as<int>());
        dest.print(F("="));
        dest.print(_e[1].as<const char*>());
        dest.print(F("="));
        dest.print(_e[2].as<double>());
        dest.flush();
        }
        dest.println(F(");"));
        delete[] xd;
        }*/
      dest.flush();
      dest.close();
      delete[] _pa;
    }
    root.close();
    yield();
  } else {
    telnetDebug.println(F("NO SDCARD!"));
    while (digitalRead(SD_D)) delay(1000);
    telnetDebug.println(F("SD Card found. Reboot..."));
    ESP.restart();
  }
  doIdle = true;
  outLoop = true;
}

bool sendToSlave(char _c, const char* _d, IPAddress _loc) {
  if (telnet.connect(_loc, 23)) {
    telnet.setNoDelay(false);
    //telnet.write(_c);
    //if (strcmp(_d, ""))
    telnet.print((char)_c + String(_d));

    char com = '\x15';
    uint32_t tt = millis();
    while (millis() - tt < DEFAULT_TIMEOUT && !telnet.available())
      yield();
    while (telnet.available() > 0)
      com = (char)telnet.read();
    telnet.stop();
    if (com == '\x06')
      return true;
  } telnetDebug.println(PSTR("[Error] Connecting to ") + String(_loc));
  return false;
}

void getFromSlave(int8_t _code, IPAddress _loc, char* _out, uint32_t _s) {
  if (telnet.connect(_loc, 23)) {
    memset(_out, 0, _s);
    telnetDebug.println(PSTR("Sending request: ") + settings.IPtoString(_loc));
    telnet.write(_code);
    telnet.flush();

    uint32_t tt = millis();
    uint32_t a = 0;
    while (millis() - tt < DEFAULT_TIMEOUT && !telnet.available())
      yield();
    if (telnet.available()) {
      while (telnet.available() && a < _s) {
        _out[a] = (char)telnet.read();
        a++;
        tt = millis();
        while (!telnet.available() && millis() - tt < 25) yield();
      }
      while (telnet.available() && telnet.read());
    } else telnetDebug.println(F("Communication error"));
    telnet.stop();
  } else {
    telnetDebug.println(PSTR("Unable to connect to ") + String(_loc));
    strcpy(_out, PSTR("NO"));
  }
}

uint32_t updateNTP(const char* ntpServerName) {
  IPAddress timeServerIP;
  WiFi.hostByName(ntpServerName, timeServerIP);
  if (IPAddress(0, 0, 0, 0) == timeServerIP) return false;
  Serial.print(F("Timeserver IP:\t"));
  Serial.println(settings.IPtoString(timeServerIP));

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
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  Serial.println(F("\r\n[HTML] Settings:"));
  for (int x = server.args() - 1; x >= 0; x--) {
    Serial.print(server.argName(x));
    Serial.print(F("="));
    Serial.println(server.arg(x));
  }

  if (server.method() == HTTP_POST) {
    if (server.arg("a") == "l" && server.args() == 1) {
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      DynamicJsonBuffer output(JSON_OBJECT_SIZE(11));
      JsonObject& data = output.createObject();
      data["SN"] = settings.name();
      data["SS"] = settings.ssidAP();
      data["OS"] = settings.ssid();
      data["OI"] = settings.IPtoString(settings.localIP());
      data["OG"] = settings.IPtoString(settings.gatewayIP());
      data["OM"] = settings.IPtoString(settings.subnetMask());
      data["OD"] = settings.useDHCP();
      data["SL"] = settings.username();
      data["UN"] = settings.useNTP;
      data["NS"] = settings.ntpServer();
      data["TZ"] = settings.timezone;
      String resp;
      data.printTo(resp);
      server.send(200, F("application/json"), resp);
    } else if (settings.authenticate(
                 server.arg("SL").c_str(),
                 server.arg("SPL").c_str())) {
      if (server.arg("a") == "s") {
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
        settings.remove(server.arg("SL").c_str(),
                        server.arg("SPL").c_str(),
                        SENSORS_FILE);
        ESP.restart();
      }
    } else returnLoginFail();
  } else server.send(200, "text/html", settings_htm);
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
                 zegar.year(),
                 zegar.month(),
                 zegar.day(),
                 zegar.hour(),
                 zegar.minute());
      server.send(200, F("text/plain"), _d);
    } else if (settings.authenticate(
                 server.arg("SL").c_str(),
                 server.arg("SPL").c_str())) {
      if (server.arg("a") == "s") {
        if (server.hasArg("TZ"))
          settings.timezone = atoi(server.arg("TZ").c_str());
        if (server.hasArg("UN"))
          settings.useNTP = (server.arg("UN")[0] == 1);
        if (server.hasArg("NS"))
          settings.ntpServer(server.arg("NS").c_str());
        if (server.hasArg("EP")) {
          uint32_t newTime = strtoul(server.arg("EP").c_str(), NULL, 10);
          newTime += 3600 * settings.timezone;
          if (letni) newTime += 3600;
          zegar.setRTC(newTime);
          settings.lastUpdate = newTime;
        }
        settings.save();
        returnOK(F("Time settings saved"));
      }
    } else returnLoginFail();
  }
  return void();
}

void mergeSensors() {
  char* _r = new char[1200];
  memset(_r, 0, 1200);
  DynamicJsonBuffer sensBuff(2250);
  File tmpSens = SD.open(SENSORS_FILE, FILE_READ);
  JsonObject& sensSet = sensBuff.parseObject(tmpSens);
  tmpSens.close();
  JsonArray& _remdev = sensSet["remote"];
  JsonArray& _saved = sensSet["saved"];

  Serial.println(F("Synchronizing with slave devices"));
  for (uint8_t a = 0; a < _remdev.size(); a++) {
    const char* _n = _remdev[a][0];
    const char* _a = _remdev[a][1];
    Serial.print(F("\tDevice \""));
    Serial.print(_n);
    Serial.print(F("\" ("));
    Serial.print(_a);
    Serial.print(F("): "));
    if (pinger.Ping(_a) == -1) {
      Serial.println(F("----"));
      continue;
    } else Serial.println(F("Online"));

    getFromSlave(0x4E, settings.stringToIP(_a), _r, 1200);
    Serial.println(_r);
    Serial.println();
    if (addDiff(_saved, _r))
      saveJson(sensSet, SENSORS_FILE);
  }
  //Finished working with received JSON
  delete[] _r;
  return void();
}

void sendSensors() {
  DynamicJsonBuffer sensBuff(2250);
  File tmpSens = SD.open(SENSORS_FILE, FILE_READ);
  JsonObject& sensSet = sensBuff.parseObject(tmpSens);
  JsonArray& _remdev = sensSet["remote"];
  tmpSens.close();

  for (uint8_t a = 0; a < _remdev.size(); a++) {
    IPAddress _a = settings.stringToIP(_remdev[a][1]);
    const char* _n = _remdev[a][0];

    Serial.print("Sending config to device " + String(_n) + ":\t");
    if (pinger.Ping(_a) != -1)
      Serial.println(sendToSlave(0x4B, (sensSet["saved"].as<String>()).c_str(), _a) ? "Success" : "Fail");
    else {
      Serial.println(F("Offline"));
      continue;
    }
  }
}

void sensorSettings() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  DynamicJsonBuffer sensBuff(2250);
  File tmpSens = SD.open(SENSORS_FILE , FILE_READ);
  JsonObject& sensSet = sensBuff.parseObject(tmpSens);
  tmpSens.close();
  JsonArray& _saved = sensSet["saved"];
  JsonArray& _local = sensSet["local"];

  if (!sensSet.success())
    return;

  server.sendContent(F("<!DOCTYPE html><html><head><title>Sensors</title>\
<style>table, th, td{border: 1px solid black;text-align: center}</style>\
</head><body><form id=\"ports\" method=\"POST\" action=\"/editports\" \
onsubmit=\"var a=document.forms.ports;for(var i=0;i<a.length-1;i++){var b=a[i]\
;b.disabled=(b.value==b.placeholder||b.value=='');}\"><table><caption><b>\
Available sensors</b></caption><tr><th>Port</th><th>Type</th><th>Port Name\
</th><th>Device Name</th><th>Temperature</th></tr>"));

  for (uint8_t a = 0; a < ext.getCount(); a++) {

    while (ext.lock())
      yield();

    uint8_t _t = ext.typeOf(a);
    const char* _n = _local[a][0];

    server.sendContent(F("<tr><td>"));
    server.sendContent(String(a + 1));
    server.sendContent(F("</td><td>"));
    server.sendContent(String(_t));
    server.sendContent(F("</td><td><input type='text' name='"));
    server.sendContent(String(a));
    server.sendContent(F("' placeholder='"));
    server.sendContent(_n);
    server.sendContent(F("'></td><td>"));

    if (_t == 40) {
      int pos = _local[a][1].as<int>();
      const char* _nn = _saved[pos][0];
      server.sendContent(_nn);
    } else server.sendContent(F("N/A"));

    server.sendContent(F("</td><td>"));

    if (_t != 0)
      server.sendContent(String(ext.getTemp(a)) + (char)176 + 'C');
    else server.sendContent(F("N/A"));
    server.sendContent(F("</td></tr>"));
  }

  server.sendContent(F("</table><button type=\"submit\">Save Changes</button>\
</form><form id=\"saved\" method=\"POST\" action=\"/editsaved\" onsubmit=\"\
var a=document.forms.saved.elements;for(var t=0;t<a.length-1;t+=2)a[t].disabled\
=(a[t+1].checked||a[t].value==''||a[t].placeholder==a[t].value);\"><table>\
<caption><b>Name Settings</b></caption><th>Name</th><th>Address/Port</th><th>\
Remove</th>"));

  for (uint8_t a = 0; a < sensSet["saved"].size(); a++) {
    JsonArray& _e = _saved[a];
    server.sendContent(F("<tr><td><input type='text' name='"));
    server.sendContent(String(a)); //Position in file
    server.sendContent(F("' placeholder='"));
    server.sendContent(_e[0].as<const char*>());
    server.sendContent(F("'><td>"));
    server.sendContent(_e[1]);
    server.sendContent(F("</td><td><input type='checkbox' name='r' value='"));
    server.sendContent(String(a));
    server.sendContent(F("'>Remove</td></tr>"));
  }

  server.sendContent(F("</table><button type=\"submit\">Save Changes\
</button></form><button onclick=\"location.href=' / sensorsbdc'\">\
Sync all devices</button></body></html>"));

  return void();
}

void editports() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  Serial.println(F("\r\n[HTML] Port edit:"));
  for (int x = 0; x < server.args(); x++) {
    Serial.print(server.argName(x));
    Serial.print(F("="));
    Serial.println(server.arg(x));
  }

  if (server.args() != 0) {
    DynamicJsonBuffer sensBuff(2250);
    File tmpSens = SD.open(SENSORS_FILE , FILE_READ);
    JsonObject& sensSet = sensBuff.parseObject(tmpSens);
    tmpSens.close();
    JsonArray& _local = sensSet["local"];

    if (!sensSet.success())
      return;

    for (int x = 0; x < server.args(); x++)
      _local[atoi((server.argName(x)).c_str())] = server.arg(x);
    saveJson(sensSet, SENSORS_FILE);
  }

  server.sendContent(F("HTTP/1.1 303 See Other\r\nLocation:/sensors\r\n"));
}

void editsaved() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  Serial.println(F("\r\n[HTML] Sensor edit:"));
  for (int x = (server.args() - 1); x >= 0 ; x--) {
    Serial.print(server.argName(x));
    Serial.print(F("="));
    Serial.println(server.arg(x));
  }

  if (server.args() != 0) {
    DynamicJsonBuffer sensBuff(2250);
    File tmpSens = SD.open(SENSORS_FILE , FILE_READ);
    JsonObject& sensSet = sensBuff.parseObject(tmpSens);
    tmpSens.close();
    JsonArray& _saved = sensSet["saved"];

    if (!sensSet.success())
      return;

    for (int x = server.args() - 1; x >= 0; x--) {
      if (server.argName(x) == "r")
        _saved.remove((atoi((server.arg(x)).c_str())));
      else
        _saved[atoi((server.argName(x)).c_str())][0] = server.arg(x);

    }
    saveJson(sensSet, SENSORS_FILE);
  }

  server.sendContent(F("HTTP/1.1 303 See Other\r\nLocation:/sensors\r\n"));
}

void returnOK(const __FlashStringHelper * _m) {
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
  if (!settings.webAuthenticate(&server)) {
    server.requestAuthentication();
    return false;
  }

  if (analogRead(A0) > 128)
    return false;

  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";

  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".csv")) dataType = F("text/txt");
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

  File dataFile = SD.open(path.c_str());
  if (dataFile.isDirectory()) {
    path += "/index.htm";
    dataType = F("text/html");
    dataFile = SD.open(path.c_str());
  }

  if (!dataFile)
    return false;

  if (server.hasArg("download"))
    dataType = F("application/octet-stream");

  if (server.streamFile(dataFile, dataType) != dataFile.size())
    telnetDebug.println(F("Sent less data than expected!"));

  dataFile.close();
  return true;
}

void printDirectory() {
  if (!server.hasArg("dir")) return returnFail(F("BAD ARGS"));
  String path = server.arg("dir");
  if (path != "/" && !SD.exists((char *)path.c_str()))
    return returnFail(F("BAD PATH"));
  File dir = SD.open((char *)path.c_str());
  path = String();
  if (!dir.isDirectory()) {
    dir.close();
    return returnFail(F("NOT DIR"));
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  WiFiClient client = server.client();

  server.sendContent(F("["));
  for (int cnt = 0; true; cnt++) {
    File entry = dir.openNextFile();
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
  return void();
}

void handleNotFound() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  if (!loadFromSdCard(server.uri()))
    return;

  String message = F("Handle not found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += '\n';
  for (uint8_t i = 0; i < server.args(); i++) {
    message += F(" NAME:");
    message += server.argName(i);//
    message += F("\n VALUE:");
    message += server.arg(i) + "\n";
  }
  server.send(404, F("text/plain"), message);
  telnetDebug.print(message);
  return void();
}

void loadindex() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();
  server.send(200, F("text/html"), index_htm);
  return void();
}
