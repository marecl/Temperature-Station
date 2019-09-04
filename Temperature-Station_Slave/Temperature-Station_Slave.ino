#include <ArduinoJson.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
//#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include <Muxtemp.h>
#include <pgmspace.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <Wire.h>
#include "FS.h"
#include "settingsHandler.h"
#define SDA D1 //0
#define SCL D2 //2

extern char settings_htm[];
static const uint8_t x509[] PROGMEM = {
#include "x509.h"
};
static const uint8_t rsakey[] PROGMEM = {
#include "key.h"
};

#define SETTINGS_FILE "/SET.TXT"
#define SENSORS_FILE "/SENSORS.TXT"

/*
  Different codes for sending saved sensors and port names

  Strona zmiany danych logowania
   Zapis / odczyt ustawien

  sens.txt -> sensors
  temp.txt -> readings
  set.txt  -> settings

  SPIFFS 1M+64K
*/

ESP8266WebServer server(80);
//ESP8266WebServerSecure server(443);
ESP8266HTTPUpdateServer updServer;
WiFiServer telnet(23);
WiFiClient telnetClient;
const uint16_t minDelay = 10000;
uint32_t nextAvailable = 0;

Muxtemp ext(Wire, 0x10);
settingsHandler settings(SETTINGS_FILE);

void setup() {
  delay(100); //Wait for boot
  SPIFFS.begin();
  Wire.begin(SDA, SCL);
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println(F("\r\nTemperature Slave"));
  Serial.print(F("\r\nMuxtemp: "));
  Serial.println((ext.begin() == 0) ? F("Present") : F("Error"));
  Serial.print(F("Ports: "));
  Serial.println(ext.getCount());
  Serial.print(F("1wire bypass: "));
  Serial.println(ext.bypass1Wire() ? F("yes") : F("no"));
  ext.refresh();

#ifdef S_DEBUG
  settings.serialDebug(&Serial);
  //Use this section to overwrite settings while debugging
  IPAddress ip(192, 168, 2, 122);
  IPAddress gateway(192, 168, 2, 1);
  IPAddress subnet(255, 255, 255, 0);
  settings.configSTA("bosszantogazember", "P!ontk0wyW13cz#r");
  settings.configAP("TemperatureSlave", "TemperatureSlave");
  settings.useDHCP(false);
  settings.name("TemperatureSlave");
  settings.configIP(ip, gateway, subnet);
  settings.configUser("admin", "admin");
  Serial.println("Saving");
  settings.save();
#endif

  Serial.print(F("Loading... "));
  if (!settings.load()) { //Defaults
    Serial.println(F("Fail"));
    settings.configUser("admin", "admin");
    settings.name("TemperatureSlave");
    settings.configAP(("TemperatureSlave" + (String)ESP.getChipId()).c_str(), "TemperatureSlave");
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

  //server.setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));
  server.on("/", HTTP_GET, sensorsPreview);
  server.on("/editports", HTTP_POST, editports);
  server.on("/settings", devconfig);

  telnet.setNoDelay(true);
  telnet.begin();
  server.begin();
  delay(2000);
  refreshSensors(&ext, SENSORS_FILE);
  nextAvailable = millis() + minDelay;

  DynamicJsonBuffer jsonBuffer(2250);
  File root = SPIFFS.open(SENSORS_FILE, "r");
  JsonObject& nSet = jsonBuffer.parseObject(root);
  JsonArray& _local = nSet["local"];
  JsonArray& _saved = nSet["saved"];
  root.close();

  if (nSet.success()) {
    Serial.println(F("\r\nPort\tType\tPort Name\tDevice Name\t\tAddress (if supported)"));
    for (int a = 0; a < ext.getCount(); a++) {
      uint8_t _type = ext.typeOf(a);
      JsonArray& _curr = _local[a];

      Serial.print(String(a + 1) + "/" + String(ext.getCount()));
      Serial.print('\t' + String(_type) + '\t');
      const char* _n = _curr[0];
      Serial.print(_n);

      if (_type == 40) {
        for (uint8_t a = 0; a < 2 - (strlen(_n) / 8); a++)
          Serial.print('\t');
        uint8_t *_q = ext.getAddress(a);
        int pos = isMember(_q, _saved, 8);

        _n = _saved[pos][0];
        Serial.print(_n);
        for (uint8_t a = 0; a < 3 - (strlen(_n) / 8); a++)
          Serial.print('\t');
        Serial.print(F("("));

        for (uint8_t y = 0; y < 8; y++) {
          Serial.print(*(_q + y));
          Serial.print(y < 7 ? ',' : ')');
        }
      }
      Serial.println();
    }

    Serial.println(F("\r\nAll sensors:"));
    for (uint8_t a = 0; a < _saved.size(); a++) {
      JsonArray& _e =  _saved[a];
      Serial.print('\t' + String(a + 1) + '.');
      Serial.print(_e[0].as<const char*>());
      Serial.print('\t');
      Serial.println(_e[1].as<const char*>());
    }
    Serial.println();
  } else Serial.println(F("Error reading list"));
}

void loop() {
  if (Serial.available() > 0) Serial.write(Serial.read());
  if (millis() >= nextAvailable) { //refresh every time possible
    refreshSensors(&ext, SENSORS_FILE);
    ext.refresh();
    nextAvailable = millis() + minDelay;
  }
  server.handleClient();
  yield();
  if (telnet.hasClient() && (!telnetClient || !telnetClient.connected())) {
    telnetClient = telnet.available();
    Serial.print(F("New client\r\n"));
  }
  if (telnetClient && telnetClient.connected() && telnetClient.available()) {
    String input = "";
    input.reserve(250);
    yield();
    uint32_t tt = millis();
    while (telnetClient.available()) {
      input += (char)telnetClient.read();
      tt = millis();
      while (!telnet.available() && millis() - tt < 25) yield();
    }
    int succCode = handleRequest(input);
    if (succCode >= 0) {
      Serial.println(F("OK"));
      if (succCode == 1)
        telnetClient.print('\x06'); //ACK
      yield();
    } else {
      Serial.println(F("FAIL"));
      if (succCode == -1)
        telnetClient.print('\x15'); //NAK
      yield();
    }
  }
  yield();
}

void refreshSensors(Muxtemp * _s, const char* _n) {
  DynamicJsonBuffer jsonBuffer(2250);
  File root;
  if (!SPIFFS.exists(_n)) {
    Serial.println("no file yo");
    root = SPIFFS.open(_n, "w");
    root.print(F("{\"local\":[],\"saved\":[]}"));
    root.flush();
    root.close();
  }

  root = SPIFFS.open(_n, "r");
  JsonObject& nSet = jsonBuffer.parseObject(root);
  root.close();

  JsonArray& _l = nSet["local"];
  JsonArray& _saved = nSet["saved"];

  bool _ch = false;
  int8_t dif = _s->getCount() - _l.size();
  if (dif > 0) {
    for (uint8_t z = 0; z < dif; z++) {
      JsonArray& _e = _l.createNestedArray();
      _e.add("Port_" + String(_l.size()));
      _e.add(false);
    }
    _ch = true;
  }

  _s->getPorts();
  for (int a = 0; a < _s->getCount(); a++) {
    JsonArray& _curr =  _l[a];
    uint8_t _type = _s->typeOf(a);
    switch (_type) {
      case 0: _curr[1] = false;
        _ch = true;
        break;
      case 11:
      case 21:
      case 22:
        _curr[1] = true;
        _ch = true;
        break;
      case 40:
        uint8_t *_q = _s->getAddress(a);
        int pos = isMember(_q, _saved, 8);
        if (pos == -1 && *_q == 40) {
          JsonArray& _new = _saved.createNestedArray();
          String _a = addrToString(_q, 8);
          _new.add(_a);
          JsonArray& _ad = _new.createNestedArray();
          for (uint8_t y = 0; y < 8; y++) _ad.add(*(_q + y));
          a--;
          continue;
        } else _curr[1] = pos;
        _ch = true;
        break;
    }
  }
  if (_ch) saveJson(nSet, _n);
  return;
}

int handleRequest(String _in) {
  int succ = -1; //return code

  //operation = code/3
  _in.trim();
  uint8_t command = _in.charAt(0);
  if (command % 3 == 0 && command != 0) {
    _in.remove(0, 1);
    command /= 3;
    Serial.print(String(command) + ": ");
  } else command = 0;

  switch (command) {
    default: break;
    case 25: { //kod 0x4B
        Serial.println(F("Overwrite sensor data"));
        Serial.println(_in);
        DynamicJsonBuffer buf(1500);
        File cur = SPIFFS.open(SENSORS_FILE, "r");
        JsonObject& all = buf.parseObject(cur);
        cur.close();
        JsonArray& lel = buf.parseArray(_in);
        if (lel.success() && all.success()) {
          JsonArray& _loc = all["local"];
          File saveout = SPIFFS.open(SENSORS_FILE, "w");
          saveout.print("{\"local\":");
          _loc.printTo(saveout);
          saveout.print(",\"saved\":");
          lel.printTo(saveout);
          saveout.print("}");
          saveout.flush();
          saveout.close();
          succ = 1;
        }
        break;
      }
    case 26: { //kod 0x4E
        Serial.println(F("Sending my sensor list"));
        if (SPIFFS.exists(SENSORS_FILE)) {
          DynamicJsonBuffer readBuff(1200);
          File senslist = SPIFFS.open(SENSORS_FILE, "r");
          JsonObject& _devs = readBuff.parseObject(senslist);
          senslist.close();

          _devs["saved"].printTo(telnetClient);

          //telnetClient.flush();
          succ = 0;
        } else telnetClient.print("[]");
        break;
      }
    case 27: { //kod 0x51
        Serial.println(F("Preparing readings"));
        if (SPIFFS.exists(SENSORS_FILE)) {
          File _devfile = SPIFFS.open(SENSORS_FILE, "r");
          DynamicJsonBuffer readBuff(1200);
          JsonObject& _devs = readBuff.parseObject(_devfile);
          _devfile.close();

          Serial.println();
          Serial.println();
          _devs.printTo(Serial);
          Serial.println();
          Serial.println();
          Serial.println("Opened output file");

          File _tempfile = SPIFFS.open("/temp.txt", "w");
          bool first = true;
          _tempfile.print('[');
          Serial.print('[');
          for (uint8_t a = 0; a < ext.getCount(); a++) {
            if (ext.typeOf(a) != 0) {
              if (!first) {
                _tempfile.print(", ");
                Serial.print(", ");
              }
              else  first = false;
              int pos = a;
              String _n = _devs["local"][a][0].as<String>();
              if (ext.typeOf(a) == 40) {
                pos = _devs["local"][a]["c"].as<int>();
                _n = _devs["saved"][pos][0].as<String>();
              }
              _tempfile.print('[' + String(a) + ",\"" + _n + "\",");
              Serial.print("[\"" + _n + "\",");
              double readTemp = ext.getTemp(a);
              _tempfile.print(readTemp);
              Serial.print(readTemp);
              _tempfile.print(F("]"));
              Serial.print(F("]"));
              _tempfile.flush();
            }
          }
          _tempfile.print(']');
          Serial.print(']');
          _tempfile.close();
          succ = 1;
        }
        break;
      }
    case 28: { //kod 0x54
        Serial.println(F("Sending readings"));
        if (SPIFFS.exists("/temp.txt")) {
          File _tempfile = SPIFFS.open("/temp.txt", "r");
          telnetClient.write(_tempfile);
          telnetClient.flush();
          _tempfile.close();
          succ = 0;
        }
        break;
      }
    case 29: {
        //Nothing in particular lol
        //Maybe will be used later
        break;
      }
    case 40: { //kod 0x78
        Serial.println(F("Sending config"));
        succ = 1;
        break;
      }
    case 41: { //kod 0x7B
        Serial.println(F("Receiving and overwriting config"));
        //telnetClient.print("OK");
        //telnetClient.print("NO");
        succ = 1;
        break;
      }
  }
  return succ;
}

int isMember(const uint8_t *_a, JsonArray & compArr, uint8_t _s) {
  for (uint8_t a = 0; a < compArr.size(); a++) {
    for (uint8_t b = 0; b < _s; b++) {
      if (*(_a + b) != compArr[a][1][b].as<uint8_t>())
        break;
      else if (b == 7) return a;
    }
  }
  return -1;
}

void sensorsPreview() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  DynamicJsonBuffer sensBuff(2250);
  File tmpSens = SPIFFS.open(SENSORS_FILE , "r");
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
</form><table><caption><b>Name Settings</b></caption><th>Name</th><th>Address/\
Port</th>"));

  for (uint8_t a = 0; a < sensSet["saved"].size(); a++) {
    JsonArray& _e = _saved[a];
    server.sendContent(F("<tr><td>"));
    server.sendContent(_e[0].as<const char*>());
    server.sendContent(F("</td><td>"));
    server.sendContent(_e[1]);
    server.sendContent(F("</td></tr>"));
  }

  server.sendContent(F("</table></body></html>"));

  return void();
}

void editports() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();
  if (server.args() != 0) {
    DynamicJsonBuffer sensBuff(2250);
    File tmpSens = SPIFFS.open(SENSORS_FILE , "r");
    JsonObject& sensSet = sensBuff.parseObject(tmpSens);
    tmpSens.close();
    JsonArray& _local = sensSet["local"];
    if (!sensSet.success())
      return;
    for (int x = 0; x < server.args(); x++)
      _local[atoi((server.argName(x)).c_str())] = server.arg(x);
    saveJson(sensSet, SENSORS_FILE);
  }
  server.sendContent(F("HTTP/1.1 303 See Other\r\nLocation:/\r\n"));
}

String addrToString(const uint8_t *_addr, uint8_t _s) {
  String out = "";
  for (uint8_t _a = 0; _a < _s; _a++)
    out += String(*(_addr + _a));
  return out;
}

void saveJson(JsonObject & toSave, const char* _f) {
  if (!toSave.success()) return;
  if (SPIFFS.exists(_f)) SPIFFS.remove(_f);
  File root = SPIFFS.open(_f, "w");
  toSave.printTo(root);
  root.flush();
  root.close();
  return void();
}

String IPtoString(IPAddress address) {
  String out;
  for (int z = 0; z < 4; z++) {
    out += String(address[z]);
    if (z < 3)out += ".";
  }
  return out;
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
        ESP.restart();
      }
    } else returnLoginFail();
  } else server.send(200, "text/html", settings_htm);
  return void();
}

void returnFail(String _m) {
  server.send(500, F("text/plain"), _m + PSTR("\r\n"));
  return void();
}

void returnOK(const __FlashStringHelper * _m) {
  server.send(200, F("text/plain"), _m);
  return void();
}

void returnLoginFail() {
  server.send(401, F("text/plain"), F("Incorrect login or password"));
  return void();
}
