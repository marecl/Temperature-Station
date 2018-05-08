#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>//
//#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include "set_htm.h"
#include <OneWire.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

static const uint8_t x509[] PROGMEM = {
#include "x509.h"
};
static const uint8_t rsakey[] PROGMEM = {
#include "key.h"
};

/*
   Create config
   Webserver with config
   Secure only webserver
   Consider WifiManager

   sens.txt -> sensors
   temp.txt -> readings
   set.txt  -> settings
*/

char* wifi_ssid = "xxx";
char* wifi_pass = "xxx";

ESP8266WebServer server(80);
//ESP8266WebServerSecure server(443);
ESP8266HTTPUpdateServer updServer;
OneWire oneWire(2);
DallasTemperature sensors(&oneWire);
WiFiServer telnet(23);
WiFiClient telnetClient;
#include "FS.h"

void setup() {
  Serial.begin(115200);
  Serial.print("\r\nWololo");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.persistent(false);
  IPAddress ip(192, 168, 2, 120);
  IPAddress gateway(192, 168, 2, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(wifi_ssid, wifi_pass);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) { //wait 10 seconds
    delay(500);
    Serial.print(F("."));
  }
  if (WiFi.status() != WL_CONNECTED) Serial.println("FAIL");
  else Serial.println(WiFi.localIP());
  //server.setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));
  server.on("/", sensorsPreview);
  server.on("/settings", devconfig);
  //server.addHandler(&httpHandler);
  updServer.setup(&server, "/update", "admin", "admin");
  telnet.setNoDelay(true);
  sensors.begin();
  server.begin();
  telnet.begin();
  refreshSensors();

  SPIFFS.begin();
  DynamicJsonBuffer jsonBuffer(375);
  File root = SPIFFS.open("/sens.txt" , "r");
  JsonArray& nSet = jsonBuffer.parseArray(root);
  root.close();
  SPIFFS.end();

  Serial.print(F("\r\nLocal:\r\n"));
  int nSensor = nSet.size();
  for (int a = 0; a < nSensor; a++) {
    Serial.print('\t' + (String)(a + 1) + "/" + (String)(nSensor) + ": ");
    Serial.print(nSet[a]["n"].as<const char*>());
    if (getTemp(nSet[a]["a"]) != -127)
      Serial.println(F(" Connected"));
    else Serial.println(F(" N/C"));
  }
}

void loop() {
  server.handleClient();
  yield();
  sensors.requestTemperatures();
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
        DynamicJsonBuffer buf(400);
        JsonArray & lel = buf.parseArray(_in);
        if (lel.success()) {
          SPIFFS.begin();
          File saveout = SPIFFS.open("/sens.txt", "w");
          lel.printTo(saveout);
          saveout.flush();
          saveout.close();
          SPIFFS.end();
          succ = 1;
        }
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
          for (uint8_t a = 0; a < _devs.size(); a++) {
            JsonArray& _oneAddr = _devs[a]["a"];
            double readTemp = getTemp(_oneAddr);
            if (first) {
              _tempfile.print("[");
              first = false;
            }
            else _tempfile.print(",");
            _tempfile.print("[\"" + _devs[a]["n"].as<String>() + "\",");
            _tempfile.print(readTemp);
            _tempfile.print(F("]"));
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
    case 29: { //kod 0x57
        Serial.println(F("Refreshing devices"));
        if (refreshSensors()) succ = 1;
        else succ = -1;
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

  /*else if (input.substring(0, 2) == "SE") {
    SPIFFS.begin();
    File saveout = SPIFFS.open("/temp.txt", "r");
    while (saveout.peek() != -1)
    telnetClient.write((uint8_t)saveout.read());
    saveout.close();
    SPIFFS.end();
    }*/
}

bool isMember(byte _1[], JsonArray & compArr) {
  for (uint8_t a = 0; a < compArr.size(); a++) {
    for (uint8_t b = 0; b < 8; b++) {
      uint8_t _2 = compArr[a]["a"][b];
      if (_1[b] != _2)
        break;
      else if (b == 7) return true;
    }
  }
  return false;
}

void sensorsPreview() {
  sensors.requestTemperatures();
  DynamicJsonBuffer sensBuff(1024);
  SPIFFS.begin();
  File tmpSens = SPIFFS.open("/sens.txt" , "r");
  JsonArray& sensSet = sensBuff.parse(tmpSens);
  tmpSens.close();
  SPIFFS.end();

  if (server.method() == HTTP_GET && server.args() == 0) {
    server.sendContent(F("<!DOCTYPE html><html><head><title>Sensors</title>"));
    server.sendContent(F("<style>table, th, td{border: 1px solid black; "));
    server.sendContent(F("text-align: center}</style></head>"));
    server.sendContent(F("<body><table><caption><b>Available sensors</b></caption>"));
    server.sendContent(F("<th>Name</th><th>Address (DEC)</th><th>Temperature</th>"));

    for (uint8_t a = 0; a < sensSet.size(); a++) {

      server.sendContent(F("<tr><td>"));
      String tempName = sensSet[a]["n"];
      server.sendContent(tempName);
      server.sendContent(F("</td><td>"));

      uint8_t addr[8];
      for (int b = 0; b < 8; b++)
        addr[b] = sensSet[a]["a"][b];

      for (int i = 0; i < 8; i++) {
        if (addr[i] < 100) server.sendContent(F("0"));
        if (addr[i] < 10) server.sendContent(F("0"));
        server.sendContent((String)addr[i]);
        if (i < 7)
          server.sendContent(F(","));
      }
      server.sendContent(F("</td><td>"));

      double testpom = sensors.getTempC(addr);
      if (testpom != -127)
        server.sendContent((String)testpom + (char)176 + "C");
      else
        server.sendContent(F("N/C"));

      server.sendContent(F("</td></tr>"));
    }
  }
  server.sendContent(F("</form></table></body></html>"));
  return;
}

String addrToString(uint8_t _addr[8]) {
  String out = "";
  for (int a = 0; a < 8; a++)
    out += String(_addr[a]);
  return out;
}

double getTemp(JsonArray&_addr) {
  uint8_t _dev[8];
  for (uint8_t a = 0; a < 8; a++)
    _dev[a] = _addr[a].as<uint8_t>();
  return sensors.getTempC(_dev);
}

bool refreshSensors() {
  bool retcode = false;
  DynamicJsonBuffer devrefr(750);
  SPIFFS.begin();
  if (SPIFFS.exists("/sens.txt")) {
    File devf = SPIFFS.open("/sens.txt", "r");
    //while (devf.peek() != -1) Serial.write(devf.read());
    JsonArray & devcon = devrefr.parse(devf);
    devf.close();
    if (devcon.success()) {
      Serial.println(F("sparslem"));
      uint8_t addr[8];
      while (oneWire.search(addr)) {
        if (isMember(addr, devcon)) continue;
        else {
          JsonObject& _dev = devcon.createNestedObject();
          _dev["n"] = addrToString(addr);
          JsonArray& _add = _dev.createNestedArray("a");
          for (int w = 0; w < 8; w++) _add.add(addr[w]);
        }
      }
      devcon.printTo(Serial);
      saveJson((char*)"/sens.txt", devcon);
      retcode = true;
    }
  } else {
    Serial.println(F("ni ma pliku"));
    JsonArray & devcon = devrefr.createArray();
    if (devcon.success()) {
      uint8_t addr[8];
      while (oneWire.search(addr)) {
        if (isMember(addr, devcon)) continue;
        else {
          JsonObject& _dev = devcon.createNestedObject();
          _dev["n"] = addrToString(addr);
          JsonArray& _add = _dev.createNestedArray("a");
          for (int w = 0; w < 8; w++) _add.add(addr[w]);
        }
      }
      devcon.printTo(Serial);
      saveJson((char*)"/sens.txt", devcon);
      retcode = true;
    }
  }
  SPIFFS.end();
  return retcode;
}

void saveJson(char* _where, JsonArray& _what) {
  File _save = SPIFFS.open(_where, "w");
  _what.printTo(_save);
  _save.flush();
  _save.close();
  return void();
}

void devconfig() {
  Serial.println();
  if (server.method() == HTTP_POST) {
    if (server.arg("action") == "load" && server.args() == 1) {
      server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      StaticJsonBuffer<500> output;
      JsonObject& data = output.createObject();
      /*data["selfName"] = settings.selfName;
      data["selfSSID"] = settings.selfSSID;
      data["selfLOGIN"] = settings.selfLOGIN;
      data["outSSID"] = settings.outSSID;
      data["outIP"] = settings.outIP;
      data["outGW"] = settings.outGW;
      data["outMASK"] = settings.outMASK;
      data["outDHCP"] = settings.outDHCP;
      String resp;
      data.printTo(resp);
      server.sendContent(resp);
      server.sendContent("\x00");
      data.prettyPrintTo(Serial);*/
    }
    if (server.arg("action") == "save") {
      if (server.arg("selfName") != "")
        //strcpy(settings.selfName, server.arg("selfName").c_str());
        if (server.arg("selfLOGIN") != "")
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
                        SPIFFS.begin();
      //saveSettings((char*)"/set.dat", settings);
      SPIFFS.end();
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
      //SPIFFS.remove("/set.dat");
      Serial.println("Remove settings");
      SPIFFS.end();
      ESP.restart();
    }
  } else server.send(200, "text/html", devSetPage);
}

