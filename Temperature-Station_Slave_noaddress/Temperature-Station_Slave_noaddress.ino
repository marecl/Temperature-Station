#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
//#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include <Muxtemp.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include "FS.h"
#include "settingsHandler.h"
#include "set_htm.h"

#define SEN_FILE "/SENS.TXT"
#define SET_FILE "/SET.TXT"
#define R_FILE "/TEMP.TXT"

static const uint8_t x509[] PROGMEM = {
#include "x509.h"
};
static const uint8_t rsakey[] PROGMEM = {
#include "key.h"
};

/*
   Zapis / odczyt ustawien

   SPIFFS 1M+64K
*/

ESP8266WebServer server(80);
//ESP8266WebServerSecure server(443);
ESP8266HTTPUpdateServer updServer;
WiFiServer telnet(23);
WiFiClient telnetClient;

Muxtemp ext(Wire, 0x10);
settingsHandler settings;

void setup() {
  delay(100); //Wait for evertything to stabilize after connecting power
  SPIFFS.begin();
  Wire.begin(0, 2); //SDA, SCL
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println(F("\r\nTemperature Slave"));
  Serial.print(F("\r\nMuxtemp: "));
  Serial.println(!ext.begin() ? F("Present") : F("Error"));
  Serial.print(F("Ports: "));
  Serial.println(ext.getCount());
  Serial.print(F("1wire bypass: "));
  Serial.println(ext.bypass1Wire() ? F("yes") : F("no"));

  ext.setDelay(10000);
  ext.refreshPorts();

#ifdef S_DEBUG
  settings.serialDebug(&Serial);

  //Use this section to overwrite settings
  IPAddress ip(192, 168, 2, 120);
  IPAddress gateway(192, 168, 2, 1);
  IPAddress subnet(255, 255, 255, 0);
  settings.config("bosszantogazember", "P!ontk0wyW13cz#r");
  settings.configAP("TemperatureSlave", "TemperatureSlave");
  settings.useDHCP(false);
  settings.name("Pokoj");
  settings.configIP(ip, gateway, subnet);
  settings.configUser("admin", "admin");
  Serial.println("Saving");
  settings.save(SET_FILE);
#endif

  Serial.print(F("Loading... "));
  if (!settings.load(SET_FILE)) { //Defaults
    Serial.println(F("Fail"));
    settings.configUser("admin", "admin");
    settings.name("TemperatureStation");
    settings.configAP(("TemperatureStation" + String(ESP.getChipId())).c_str(), "TemperatureStation");
  } else Serial.println(F("OK"));

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

  settings.beginOTA();
  //server.setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));
  server.on("/", sensorsPreview);
  server.on("/sensors", sensorsConfig);
  server.on("/settings", devconfig);

  //server.addHandler(&httpHandler);
  telnet.setNoDelay(true);
  telnet.begin();
  server.begin();

  Serial.print(F("Emergency loading:\t"));
  Serial.println(ext.lock());
  while (ext.lock()) {
    ArduinoOTA.handle();
    server.handleClient();
    yield();
  }
  refreshSensors(&ext, SEN_FILE);

  DynamicJsonBuffer jsonBuffer(300);
  File root = SPIFFS.open(SEN_FILE , "r");
  JsonArray& nSet = jsonBuffer.parseArray(root);
  root.close();

  if (nSet.success()) {
    Serial.println(F("\r\nPort\tPort Name\tType\tAddress (if supported)"));
    for (int a = 0; a < ext.getCount(); a++) {
      Serial.print(String(a + 1) + '/' + String(ext.getCount()) + ":\t");
      const char* _n = nSet[a];
      Serial.print(_n);
      if (strlen(_n) < 16) Serial.print('\t');
      if (strlen(_n) < 8) Serial.print('\t');

      Serial.print(ext.typeOf(a), DEC);
      if (ext.typeOf(a) == 5) {
        uint8_t *_q = ext.getAddress(a);
        Serial.print(F("\t("));
        for (uint8_t y = 0; y < 8; y++) {
          Serial.print(*(_q + y));
          Serial.print(y < 7 ? ',' : ')');
        }
      }
      Serial.println();
    }
  } else Serial.println(F("Error reading list"));
}

void loop() {
  if (Serial.available() > 0) Serial.write(Serial.read());
  if (!ext.lock()) { //refresh every time possible
    refreshSensors(&ext, SEN_FILE);
    ext.refreshPorts();
  }
  ArduinoOTA.handle();
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
  DynamicJsonBuffer jsonBuffer(600);
  File root;
  if (!SPIFFS.exists(_n)) {
    Serial.println("no file yo");
    root = SPIFFS.open(_n, "w");
    root.print(F("[]"));
    root.flush();
    root.close();
  }
  root = SPIFFS.open(_n, "r");

  JsonArray& nSet = jsonBuffer.parseArray(root);

  int8_t dif = _s->getCount() - nSet.size();
  if (dif > 0)
    for (uint8_t z = 0; z < dif; z++)
      nSet.add("Port_" + String(z + 1));
  _s->getPorts();

  root.close();
  saveJson(nSet, _n);
  return void();
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
        DynamicJsonBuffer buf(800);
        JsonArray& lel = buf.parseArray(_in);
        if (lel.success()) {
          File cur = SPIFFS.open(SEN_FILE, "w");
          lel.printTo(cur);
          cur.flush();
          cur.close();
          succ = 1;
        }
        break;
      }
    case 26: { //kod 0x4E
        Serial.println(F("Sending my sensor list"));
        if (SPIFFS.exists(SEN_FILE)) {
          DynamicJsonBuffer readBuff(1200);
          File senslist = SPIFFS.open(SEN_FILE, "r");
          JsonArray& _devs = readBuff.parseArray(senslist);

          if (_devs.success())
            _devs.printTo(telnetClient);
          else telnetClient.print(F("[]"));
          telnetClient.flush();
          senslist.close();
          succ = 0;
        } else telnetClient.print(F("[]"));
        break;
      }
    case 27: { //kod 0x51
        Serial.println(F("Preparing readings"));
        if (SPIFFS.exists(SEN_FILE)) {
          File _devfile = SPIFFS.open(SEN_FILE, "r");
          DynamicJsonBuffer readBuff(750);
          JsonArray& _devs = readBuff.parseArray(_devfile);

          File dest = SPIFFS.open(R_FILE, "w");
          bool first = true;
          dest.print(F("["));
          dest.flush();
          for (uint8_t c = 0; c < ext.getCount(); c++) {
            double tempRead = ext.getTemp(c);
            if (tempRead == -127.00 || tempRead == 85.00) continue;
            if (ext.typeOf(c) == 0) continue;

            if (!first) dest.print(F(","));
            const char* _name = _devs[c];
            dest.print(F("["));
            dest.print(c);
            dest.print(F(",\""));
            dest.print(_name);
            dest.print(F("\","));
            dest.print(tempRead);
            dest.print(F("]"));
            first = false;
          }
          dest.print(F("]"));
          dest.close();
          _devfile.close();
          succ = 1;
        }
        break;
      }
    case 28: { //kod 0x54
        Serial.println(F("Sending readings"));
        //SPIFFS.begin();
        if (SPIFFS.exists(R_FILE)) {
          File _tempfile = SPIFFS.open(R_FILE, "r");
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

void sensorsConfig() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();
  //SPIFFS.begin();

  if (server.args() != 0 && server.method() == HTTP_POST) {
    DynamicJsonBuffer sensBuff(1000);
    File tmpSens = SPIFFS.open(SEN_FILE , "r");
    JsonArray& sensSet = sensBuff.parseArray(tmpSens);
    tmpSens.close();
    if (!sensSet.success()) {
      server.send(500, F("text/html"), F("Error\x20parsing\x20JSON\x20file"));
      return;
    }

    if (server.args() != 0 && server.method() == HTTP_POST) {
      for (int x = 0; x < server.args(); x++) {
        int changePos = atoi((server.argName(x)).c_str());
        if (sensSet[changePos] != server.arg(x).c_str())
          sensSet[changePos] = server.arg(x);
      }
    }
    saveJson(sensSet, SEN_FILE);
  }
  server.sendContent(F("HTTP/1.1 303 See Other\r\nLocation:/\r\n"));
  return void();
}

void sensorsPreview() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  DynamicJsonBuffer sensBuff(1000);
  File tmpSens = SPIFFS.open(SEN_FILE , "r");
  JsonArray& sensSet = sensBuff.parseArray(tmpSens);
  tmpSens.close();

  if (!sensSet.success())
    return;

  if (server.method() == HTTP_GET && server.args() == 0) {
    server.sendContent(F("<!DOCTYPE html><html><head><title>Sensors</title>\
      <style>table,th,td{border:1px solid black;text-align:center}</style>\
      </head><body><table><caption><b>Available sensors</b></caption>\
      <form id='sensors' method=\"POST\" action=\"/sensors\" onsubmit=\"\
      var a=document.forms.sensors;for(var i=0;i<a.length;i++){if(a[i].value==\
      a[i].placeholder||a[i].value=='')a[i].disabled=true;}a.submit();\"><tr>\
      <th>Port</th><th>Type</th><th>Name</th><th>Temperature</th></tr>"));

    for (uint8_t a = 0; a < ext.getCount(); a++) {
      uint8_t _t = ext.typeOf(a);
      const char* _n = sensSet[a];
      server.sendContent(F("<tr><td>"));
      server.sendContent(String(a + 1));
      server.sendContent(F("</td><td>"));
      server.sendContent(String(_t));
      server.sendContent(F("</td><td><input type='text' name='"));
      server.sendContent(String(a));
      server.sendContent(F("' placeholder='"));
      server.sendContent(_n);
      server.sendContent(F("'></td><td>"));

      if (_t == 0) server.sendContent(F("N/C"));
      else server.sendContent(String(ext.getTemp(a)) + (char)176 + 'C');
      server.sendContent(F("</td></tr>"));
    }

    server.sendContent(F("</table><button type=\"submit\" value=\"Save\">Save Changes</button>"));
    server.sendContent(F("<button type=\"reset\" value=\"Reset\">Reset</button>"));
    server.sendContent(F("</form></body></html>"));
  }
  return void();
}

void saveJson(JsonArray& _what, const char* _where) {
  File _save = SPIFFS.open(_where, "w");
  _what.printTo(_save);
  _save.flush();
  _save.close();
  return void();
}

void devconfig() {
  if (!settings.webAuthenticate(&server))
    return server.requestAuthentication();

  if (server.method() == HTTP_POST) {
    if (server.arg("a") == "l" && server.args() == 1) {
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      StaticJsonBuffer<400> output;
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
      data.printTo(Serial);
    } else if (server.arg("a") == "s") {
      if (settings.authenticate(
            server.arg("SL").c_str(),
            server.arg("SPL").c_str())) {

        /*
           Tu beda zapisywane ustawienia yo
        */

        for (int x = 0; x < server.args(); x++) {
          Serial.print(server.argName(x));
          Serial.print(F("="));
          Serial.println(server.arg(x));
        }

        //settings.save(SETTINGS_FILE);
        server.send(200, F("text/plain"), F("Settings saved"));
      } else server.send(401, F("text/plain"), F("Incorrect login or password"));
    } else if (server.arg("a") == "r") {
      if (settings.authenticate(
            server.arg("SL").c_str(),
            server.arg("SPL").c_str())) {
        server.send(200, F("text/plain"), F("Settings reset. Rebooting..."));
        Serial.println(F("REMOVE CONFIG YO"));
        ESP.restart();
      } else server.send(401, F("text/plain"), F("Incorrect login or password"));
    }
  } else server.send(200, "text/html", devSetPage);
  return void();
}
