#include <ArduinoJson.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
//#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include <Muxtemp.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <Wire.h>
#include "FS.h"
#include "settingsHandler.h"
#include "set_htm.h"

static const uint8_t x509[] PROGMEM = {
#include "x509.h"
};
static const uint8_t rsakey[] PROGMEM = {
#include "key.h"
};


/*
   Strona zmiany danych logowania
   Zrobic strone z ustawieniami ktora dziala
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

Muxtemp ext(Wire);
settingsHandler settings;

void setup() {
  Wire.begin(0, 2); //SDA, SCL
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println(F("\r\nTemperature Slave"));
  Serial.print(F("\r\nMuxtemp: "));
  delay(500); //Wait for boot
  Serial.println((ext.begin(0x10) == 0) ? F("Present") : F("Error"));
  Serial.print(F("Ports: "));
  Serial.println(ext.getCount());
  Serial.print(F("1wire bypass: "));
  Serial.println(ext.bypass1Wire() ? F("yes") : F("no"));
  ext.refreshPorts();

  IPAddress ip(192, 168, 2, 120);
  IPAddress gateway(192, 168, 2, 1);
  IPAddress subnet(255, 255, 255, 0);

  Serial.print(F("Loading... "));
  settings.serialDebug(&Serial);
  if (!settings.load("/set.txt")) { //Defaults
    Serial.println(F("Fail"));
    settings.configUser("admin", "admin");
    settings.name("TemperatureStation");
    settings.configAP(("TemperatureStation" + (String)ESP.getChipId()).c_str(), "TemperatureStation");
  } else Serial.println(F("OK"));

  settings.config("bosszantogazember", "P!ontk0wyW13cz#r");
  settings.configAP("TemperatureSlave", "TemperatureSlave");
  settings.useDHCP(false);
  settings.name("Pokoj");
  settings.configIP(ip, gateway, subnet);
  settings.configUpdateServer(&server, &updServer, "/update");
  settings.serialDebug(&Serial);
  settings.save("/set.txt");

  bool gotwifi = false;
  if (strcmp(settings.ssid(), "") != 0) gotwifi = true;
  if (gotwifi) {
    Serial.print(F("\nConnecting to "));
    Serial.print(settings.ssid());
    Serial.print(F("... "));
    if (settings.beginWiFi()) {
      Serial.println(F("Success"));
      Serial.println(WiFi.localIP());
    } else Serial.println(F("Connection failed"));
  } else {
    Serial.println(F("Starting Access Point..."));
    settings.beginAP();
  }

  //server.setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));
  server.on("/", sensorsPreview);
  server.on("/sensors", sensorsConfig);
  server.on("/settings", devconfig);

  //server.addHandler(&httpHandler);
  telnet.setNoDelay(true);
  telnet.begin();
  server.begin();
  delay(2000);
  SPIFFS.begin();
  refreshSensors(&ext, "/SENS.TXT");
  SPIFFS.end();
  nextAvailable = millis() + minDelay;

  DynamicJsonBuffer jsonBuffer(375);
  SPIFFS.begin();
  File root = SPIFFS.open("/SENS.TXT" , "r");
  JsonObject& nSet = jsonBuffer.parseObject(root);
  root.close();
  SPIFFS.end();

  if (nSet.success()) {
    JsonArray& _saved = nSet["saved"];
    Serial.println(F("\r\nLocal:"));
    for (int a = 0; a < ext.getCount(); a++) {
      uint8_t _type = ext.typeOf(a);
      JsonObject& _curr = nSet["local"][a];
      Serial.print('\t' + (String)(a + 1) + "/" + (String)(ext.getCount()) + ": ");
      Serial.print('\t' + String(_type) + '\t');
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
  } else Serial.println(F("Error reading list"));
}

void loop() {
  if (Serial.available() > 0) Serial.write(Serial.read());
  if (millis() >= nextAvailable) { //refresh every time possible
    refreshSensors(&ext, "/SENS.TXT");
    ext.refreshPorts();
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
    while (telnetClient.available())
      input += (char)telnetClient.read();

    int succCode = handleRequest(input);
    if (succCode >= 0) {
      Serial.println(F("OK"));
      if (succCode == 1)
        telnetClient.print('\x06'); //ACK OK
      yield();
    }
    else {
      Serial.println(F("FAIL"));
      if (succCode == -1)
        telnetClient.print('\x15'); //NAK FAIL
      yield();
    }
    telnetClient.flush();

  }
  yield();
}

void refreshSensors(Muxtemp* _s, const char* _n) {
  DynamicJsonBuffer jsonBuffer(1500);
  String _rt = "";
  if (SPIFFS.exists(_n)) {
    File root = SPIFFS.open(_n, "r");
    while (root.available())
      if (root.peek() != ' ')
        _rt += (char)root.read();
    root.close();
  } else _rt = "{\"local\":[],\"saved\":[]}";

  JsonObject& nSet = jsonBuffer.parseObject(_rt);
  JsonArray& _l = nSet["local"];
  JsonArray& _saved = nSet["saved"];

  int8_t dif = _s->getCount() - _l.size();
  if (dif > 0) {
    for (uint8_t z = 0; z < dif; z++) {
      JsonObject& _e = _l.createNestedObject();
      _e["n"] = "";
      _e["c"] = false;
    }
  } else if (dif < 0) {
    JsonArray& _e = nSet["local"];
    for (uint8_t z = 0; z < dif; z++)
      _e.remove(_e.size() - 1);
  }

  _s->getPorts();

  for (int a = 0; a < _s->getCount(); a++) {
    JsonObject& _curr = nSet["local"][a];
    int pos;
    uint8_t _type = _s->typeOf(a);
    switch (_type) {
      case 0: _curr["c"] = false; break;
      case 11:
      case 21:
      case 22:
        if (_curr["n"] == "")
          _curr["n"] = "Unknown_DHT";
        _curr["c"] = true;
        break;
      case 5:
        uint8_t *_q = _s->getAddress(a);
        pos = isMember(_q, _saved, 8);
        if (pos == -1) {
          JsonObject& _new = _saved.createNestedObject();
          _new["n"] = addrToString(_q, 8);
          JsonArray& _ad = _new.createNestedArray("a");
          for (uint8_t y = 0; y < 8; y++) _ad.add(*(_q + y));
          a--;
          break;
        } else _curr["c"] = pos;
        break;
    }
  }
  saveJson(nSet, _n);
}

int handleRequest(String _in) {
  int succ = -1; //return code

  //operation = code/3
  _in.trim();
  uint8_t command = _in.charAt(0);
  if (command % 3 == 0 && command != 0) {
    _in.remove(0, 1);
    command /= 3;
    Serial.println(String(command) + ": " + _in);
  } else command = 0;

  switch (command) {
    default: break;
    case 25: { //kod 0x4B
        Serial.println(F("Overwrite sensor data"));
        DynamicJsonBuffer buf(1500);
        SPIFFS.begin();
        File cur = SPIFFS.open("/SENS.TXT", "r");
        JsonObject& all = buf.parseObject(cur);
        cur.close();
        JsonArray& lel = buf.parseArray(_in);
        if (lel.success() && all.success()) {
          JsonArray& _loc = all["local"];
          File saveout = SPIFFS.open("/SENS.TXT", "w");
          saveout.print("{\"local\":");
          _loc.printTo(saveout);
          saveout.print(",\"saved\":");
          lel.printTo(saveout);
          saveout.print("}");
          saveout.flush();
          saveout.close();
          succ = 1;
        }
        SPIFFS.end();
        break;
      }
    case 26: { //kod 0x4E
        Serial.println(F("Sending my sensor list"));
        SPIFFS.begin();
        if (SPIFFS.exists("/sens.txt")) {
          File senslist = SPIFFS.open("/sens.txt", "r");
          telnetClient.write(senslist, senslist.size());
          telnetClient.flush();
          senslist.close();
          succ = 0;
        } else telnetClient.print("[]");
        SPIFFS.end();
        break;
      }
    case 27: { //kod 0x51
        Serial.println(F("Preparing readings"));
        SPIFFS.begin();
        if (SPIFFS.exists("/sens.txt")) {
          File _devfile = SPIFFS.open("/sens.txt", "r");
          DynamicJsonBuffer readBuff(1200);
          JsonArray& _devs = readBuff.parseArray(_devfile);
          _devfile.close();

          File _tempfile = SPIFFS.open("/temp.txt", "w");
          bool first = true;
          _tempfile.print("[");
          for (uint8_t a = 0; a < ext.getCount(); a++) {
            if (ext.typeOf(a) != 0) {
              if (!first)
                _tempfile.print(", ");
              else  first = false;
              _tempfile.print("[\"" + _devs[a]["n"].as<String>() + "\",");
              double readTemp = ext.getTemp(a);
              _tempfile.print(readTemp);
              _tempfile.print(F("]"));
            }
          }
          _tempfile.print(F("]"));
          _tempfile.close();
          succ = 1;
        }
        SPIFFS.end();
        break;
      }
    case 28: { //kod 0x54
        Serial.println(F("Sending readings"));
        SPIFFS.begin();
        if (SPIFFS.exists("/temp.txt")) {
          File _tempfile = SPIFFS.open("/temp.txt", "r");
          telnetClient.write(_tempfile);
          telnetClient.flush();
          _tempfile.close();
          succ = 0;
        }
        SPIFFS.end();
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

int isMember(const uint8_t *_a, JsonArray& compArr, uint8_t _s) {
  for (uint8_t a = 0; a < compArr.size(); a++) {
    for (uint8_t b = 0; b < _s; b++) {
      if (*(_a + b) != compArr[a]["a"][b].as<uint8_t>())
        break;
      else if (b == 7) return a;
    }
  }
  return -1;
}

void sensorsConfig() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();
  SPIFFS.begin();

  if (server.args() != 0 && server.method() == HTTP_POST) {
    DynamicJsonBuffer sensBuff(1000);
    File tmpSens = SPIFFS.open("/SENS.TXT" , "r");
    JsonObject& sensSet = sensBuff.parseObject(tmpSens);
    tmpSens.close();
    if (!sensSet.success())
      return;

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
    server.sendContent(F("Location: /\r\n"));
    saveJson(sensSet, "/SENS.TXT");
  }
  SPIFFS.end();
}

void sensorsPreview() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  DynamicJsonBuffer sensBuff(1000);
  SPIFFS.begin();
  File tmpSens = SPIFFS.open("/SENS.TXT" , "r");
  JsonObject& sensSet = sensBuff.parseObject(tmpSens);
  tmpSens.close();
  SPIFFS.end();

  if (!sensSet.success())
    return;

  if (server.method() == HTTP_GET && server.args() == 0) {
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
    server.sendContent(F("</form></body></html>"));
  }
  return void();
}

String addrToString(const uint8_t *_addr, uint8_t _s) {
  String out = "";
  for (uint8_t _a = 0; _a < _s; _a++)
    out += String(*(_addr + _a));
  return out;
}

void saveJson(JsonObject& _what, const char* _where) {
  File _save = SPIFFS.open(_where, "w");
  _what.printTo(_save);
  _save.flush();
  _save.close();
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
  Serial.println();
  if (server.arg("action") == "test") {
    Serial.println(server.uri());
    for (int x = server.args() - 1; x >= 0; x--) {
      Serial.print(server.argName(x));
      Serial.print("=");
      Serial.println(server.arg(x));
    }
    server.sendContent(F("\nHTTP/1.1 303 See Other\r\n"));
    server.sendContent(F("Location: /settings\r\n"));

    /* if (server.arg("selfLOGIN") != "")
       //strcpy(settings.selfLOGIN, server.arg("selfLOGIN").c_str());
       if (server.arg("selfPASS") != "")
         //strcpy(settings.selfPASS, server.arg("selfPASS").c_str());
         if (server.arg("outSSID") != "")
           //strcpy(settings.outSSID, server.arg("outSSID").c_str());
           if (server.arg("outPASS") != "")
             //strcpy(settings.outPASS, server.arg("outPASS").c_str());
             if (server.arg("outIP") != "")
               //strcpy(settings.outIP, server.arg("outIP").c_str());
               if (server.arg("outGW") != "")
                 //strcpy(settings.outGW, server.arg("outGW").c_str());
                 if (server.arg("outMASK") != "")
                   //strcpy(settings.outMASK, server.arg("outMASK").c_str());
                   if (server.arg("outDHCP") != "")
                     //settings.outDHCP = toBool(server.arg("outDHCP"));
    */
  } else if (server.arg("action") == "reset") {
    SPIFFS.begin();
    //SPIFFS.remove("/set.dat");
    Serial.println("Remove settings");
    SPIFFS.end();
    ESP.restart();
  }

  if (server.method() == HTTP_POST) {
    if (server.arg("action") == "load") {
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
      String resp = "";
      data.printTo(resp);
      server.sendContent(resp);
      server.sendContent("\x00");
      data.prettyPrintTo(Serial);
    } else if (server.arg("action") == "save") {
      if (server.arg("selfName") != "")
        settings.name(server.arg("selfName").c_str());
      if (server.arg("outPASS") != "")
        yield();
      if (server.arg("outIP") != "" && server.arg("outGW") != "")
        if (server.arg("outMASK") != "")
          settings.configIP(server.arg("outIP").c_str(), \
                            server.arg("outGW").c_str(), \
                            server.arg("outGW").c_str());
      if (server.arg("outDHCP") != "")
        settings.useDHCP((server.arg("outDHCP")[0] == '1') ? true : false);
      if (server.hasArg("outSSID"))
        settings.ssid(server.arg("outSSID").c_str());
      if (server.arg("selfSSID") != "")
        settings.ssid(server.arg("selfSSID").c_str());

      //strcpy(settings.outPASS, server.arg("outPASS").c_str());

      //strcpy(settings.outSSID, server.arg("outSSID").c_str());
      //settings.save("/set.txt");
      server.sendContent(F("\nHTTP/1.1 303 See Other\r\n"));
      server.sendContent(F("Location: /settings\r\n"));
    }
  } else server.send(200, "text/html", devSetPage);

  //strcpy(settings.selfName, server.arg("selfName").c_str());
  //if (server.arg("selfLOGIN") != "")
  //strcpy(settings.selfLOGIN, server.arg("selfLOGIN").c_str());
  //if (server.arg("selfPASS") != "")
  //strcpy(settings.selfPASS, server.arg("selfPASS").c_str());

}

bool toBool(String input) {
  if (input[0] == '1') return true;
  else return false;
}
