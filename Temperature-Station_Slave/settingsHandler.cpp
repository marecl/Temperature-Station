#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServerSecure.h>
#include "FS.h"
#include "settingsHandler.h"

settingsHandler::settingsHandler() {
  this->_name = new char[32];
  this->_ssid = new char[32];
  this->_pass = new char[32];
  this->_ssidap = new char[32];
  this->_passap = new char[32];
  this->_user = new char[16];
  this->_pwd = new char[16];
  this->_dhcp = true;
}

settingsHandler::~settingsHandler() {
  delete this->_name;
  delete this->_ssid;
  delete this->_pass;
  delete this->_ssidap;
  delete this->_passap;
  delete this->_user;
  delete this->_pwd;
#ifdef S_DEBUG
  this->_debug->println(F("Settings:\tEnd"));
#endif
}

void settingsHandler::save(const char* _f) {
  SPIFFS.remove(_f);
  File _toSave = SPIFFS.open(_f, "w");
  _toSave.print(F("{\"SN\":\""));
  _toSave.print(this->_name);
  _toSave.print(F("\",\"SL\":\""));
  _toSave.print(this->_user);
  _toSave.print(F("\",\"SPL\":\""));
  _toSave.print(this->_pwd);
  _toSave.print(F("\",\"OS\":\""));
  _toSave.print(this->_ssid);
  _toSave.print(F("\",\"OP\":\""));
  _toSave.print(this->_pass);
  _toSave.print(F("\",\"SS\":\""));
  _toSave.print(this->_ssidap);
  _toSave.print(F("\",\"SPA\":\""));
  _toSave.print(this->_passap);
  _toSave.print(F("\",\"OI\":\""));
  _toSave.print(this->_ip);
  _toSave.print(F("\",\"OG\":\""));
  _toSave.print(this->_gw);
  _toSave.print(F("\",\"OM\":\""));
  _toSave.print(this->_mask);
  _toSave.print(F("\",\"OD\":"));
  _toSave.print(this->_dhcp ? F("true") : F("false"));
  _toSave.print(F("}"));
  _toSave.flush();
  _toSave.close();

#ifdef S_DEBUG
  this->_debug->print(F("{\"SN\":\""));
  this->_debug->print(this->_name);
  this->_debug->print(F("\",\"SL\":\""));
  this->_debug->print(this->_user);
  this->_debug->print(F("\",\"SPL\":\""));
  this->_debug->print(this->_pwd);
  this->_debug->print(F("\",\"OS\":\""));
  this->_debug->print(this->_ssid);
  this->_debug->print(F("\",\"OP\":\""));
  this->_debug->print(this->_pass);
  this->_debug->print(F("\",\"SS\":\""));
  this->_debug->print(this->_ssidap);
  this->_debug->print(F("\",\"SPA\":\""));
  this->_debug->print(this->_passap);
  this->_debug->print(F("\",\"OI\":\""));
  this->_debug->print(this->_ip);
  this->_debug->print(F("\",\"OG\":\""));
  this->_debug->print(this->_gw);
  this->_debug->print(F("\",\"OM\":\""));
  this->_debug->print(this->_mask);
  this->_debug->print(F("\",\"OD\":"));
  this->_debug->print(this->_dhcp ? F("true") : F("false"));
  this->_debug->print(F("}\r\n"));
  this->_debug->println(F("Settings:\tSaved settings"));
#endif
  return void();
}

void settingsHandler::beginOTA(uint16_t _p) {
  ArduinoOTA.setHostname(this->_name);
  ArduinoOTA.setPassword(this->_pwd);
  ArduinoOTA.setPort(_p);
#ifdef S_DEBUG
  this->_debug->println(F("Settings:\tArduinoOTA Started"));
  ArduinoOTA.onStart([this]() {
    this->_debug->println(F("Settings:\tBegin flash update"));
  });
  ArduinoOTA.onEnd([this]() {
    SPIFFS.end();
    this->_debug->println(F("Settings:\tUpdate End"));
  });
  ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
    this->_debug->printf("Settings:\tUpdate Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([this](ota_error_t error) {
    this->_debug->printf("Settings:\tError[%u]: ", error);
    if (error == OTA_AUTH_ERROR) this->_debug->println(F("Auth Failed"));
    else if (error == OTA_BEGIN_ERROR) this->_debug->println(F("Begin Failed"));
    else if (error == OTA_CONNECT_ERROR) this->_debug->println(F("Connect Failed"));
    else if (error == OTA_RECEIVE_ERROR) this->_debug->println(F("Receive Failed"));
    else if (error == OTA_END_ERROR) this->_debug->println(F("End Failed"));
    SPIFFS.begin();
  });
#endif
  ArduinoOTA.begin();
  return void();
}

bool settingsHandler::load(const char* _f) {
  if (!SPIFFS.exists(_f)) {
#ifdef S_DEBUG
    this->_debug->println(F("Settings:\tNo file to load"));
#endif
    return false;
  }
#ifdef S_DEBUG
  this->_debug->println(F("Settings: Found file"));
#endif
  File _toSave = SPIFFS.open(_f, "r");
  DynamicJsonBuffer _ld(JSON_OBJECT_SIZE(15) + 250);
  JsonObject& _data = _ld.parseObject(_toSave);
  _toSave.close();
  if (!_data.success()) {
#ifdef S_DEBUG
    this->_debug->println(F("Settings:\tLoad error"));
#endif
    return false;
  }
  this->setField(this->_name, _data["SN"], 32);
  this->setField(this->_user, _data["SL"], 16);
  this->setField(this->_pwd, _data["SPL"], 16);
  this->setField(this->_ssid, _data["OS"], 32);
  this->setField(this->_pass, _data["OP"], 32);
  this->setField(this->_ssidap, _data["SS"], 32);
  this->setField(this->_passap, _data["SPA"], 32);
  this->_ip = stringToIP(_data["OI"]);
  this->_gw = stringToIP(_data["OG"]);
  this->_mask = stringToIP(_data["OM"]);
  this->_dhcp = _data["OD"].as<bool>();
#ifdef S_DEBUG
  this->_debug->println(F("Settings:\tLoaded successfully"));
#endif
  return true;
}

#ifdef S_DEBUG
void settingsHandler::serialDebug(HardwareSerial *_d) {
  this->_debug = _d;
  this->_debug->println(F("Settings:\tDebug begin"));
  return void();
}
#endif

bool settingsHandler::reset(const char* _l, const char* _p, const char* _f) {
  if (!this->authenticate(_l, _p))
    return false;

#ifdef S_DEBUG
  this->_debug->println(F("Settings:\tRemoving config"));
#endif
  SPIFFS.remove(_f);
  return true;
}

bool settingsHandler::webAuthenticate(ESP8266WebServer* _s) {
  return _s->authenticate(this->_user, this->_pwd);
}

bool settingsHandler::beginWiFi() {
  WiFi.mode(WIFI_STA);
  if (this->_dhcp == false)
    WiFi.config(this->_ip, this->_gw, this->_mask);
  WiFi.setOutputPower(0);
  WiFi.begin(this->_ssid, this->_pass);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20)  //wait 10 seconds
    delay(500);

  if (WiFi.status() != WL_CONNECTED) {
#ifdef S_DEBUG
    this->_debug->println(F("Settings:\tUnable to connect to WiFi"));
#endif
    return false;
  }
  if (this->_dhcp == true)
    this->configIP(this->_ip, this->_gw, this->_mask);
#ifdef S_DEBUG
  this->_debug->print(F("Settings:\tConnected to "));
  this->_debug->println(this->_ssid);
#endif
  return true;
}

bool settingsHandler::beginAP() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_AP);
  WiFi.setOutputPower(0);
  delay(10);
  bool _r = WiFi.softAP(this->_ssidap, this->_passap, false);
#ifdef S_DEBUG
  this->_debug->print(F("Settings:\tAccessPoint Started: "));
  this->_debug->println(_r ? F("Yes") : F("No"));
#endif
  return _r;
}

void settingsHandler::configUser(const char* _u, const char* _p) {
  this->setField(this->_user, _u, 16);
  this->setField(this->_pwd, _p, 16);
#ifdef S_DEBUG
  this->_debug->print(F("Settings:\tConfigured user "));
  this->_debug->print(this->_user);
  this->_debug->print(F("@"));
  this->_debug->println(this->_pwd);
#endif
  return void();
}

bool settingsHandler::authenticate(const char* _username, const char* _password) {
  if (strcmp(this->_user, _username) == 0)
    if (strcmp(this->_pwd, _password) == 0) {
#ifdef S_DEBUG
      this->_debug->printf("Settings:\tUser %s authenticated\r\n", this->_user);
#endif
      return true;
    }
#ifdef S_DEBUG
  this->_debug->print(F("Settings:\tInvalid user or password "));
  this->_debug->print(this->_user);
  this->_debug->print(F("@"));
  this->_debug->println(this->_pwd);
#endif
  return false;
}

void settingsHandler::config(const char* _s, const char* _p) {
  this->setField(this->_ssid, _s, 32);
  this->setField(this->_pass, _p, 32);
#ifdef S_DEBUG
  this->_debug->print(F("Settings:\tConfigured WiFi "));
  this->_debug->print(this->_ssid);
  this->_debug->print(F("@"));
  this->_debug->println(this->_pass);
#endif
  return void();
}

void settingsHandler::configAP(const char* _ssidap, const char* _passap) {
  this->setField(this->_ssidap, _ssidap, 32);
  this->setField(this->_passap, _passap, 32);
#ifdef S_DEBUG
  this->_debug->print(F("Settings:\tConfigured AccessPoint "));
  this->_debug->print(this->_ssidap);
  this->_debug->print(F("@"));
  this->_debug->println(this->_passap);
#endif
  return void();
}

void settingsHandler::configIP(IPAddress _ip, IPAddress _gw, IPAddress _mask) {
  this->_ip = _ip;
  this->_gw = _gw;
  this->_mask = _mask;
#ifdef S_DEBUG
  this->_debug->print(F("Settings:\tIP Config:\r\n\t\tIP Address:\t"));
  this->_debug->print(this->_ip);
  this->_debug->print(F("\r\n\t\tGateway:\t"));
  this->_debug->print(this->_gw);
  this->_debug->print(F("\r\n\t\tSubnet:\t"));
  this->_debug->println(this->_mask);
#endif
  return void();
}

void settingsHandler::configIP(const char* _ip, const char* _gw, const char* _mask) {
  this->configIP(stringToIP(_ip), stringToIP(_gw), stringToIP(_mask));
  return void();
}

void settingsHandler::useDHCP(bool _d) {
  this->_dhcp = _d;
#ifdef S_DEBUG
  this->_debug->print(F("Settings:\tDHCP has been "));
  this->_debug->println(this->_dhcp ? F("enabled") : F("disabled"));
#endif
  return void();
}

void settingsHandler::ssid(const char* _ssid) {
  setField(this->_ssid, _ssid, 32);
#ifdef S_DEBUG
  this->_debug->print(F("Settings:\tSSID changed to "));
  this->_debug->println(_ssid);
#endif
  return void();
}

void settingsHandler::ssidAP(const char* _ssidap) {
  setField(this->_ssidap, _ssidap, 32);
#ifdef S_DEBUG
  this->_debug->print(F("Settings:\tAP SSID changed to "));
  this->_debug->println(_ssid);
#endif
  return void();
}

const char* settingsHandler::ssid() {
  return this->_ssid;
}

const char* settingsHandler::ssidAP() {
  return this->_ssidap;
}

const char* settingsHandler::username() {
  return this->_user;
}

const char* settingsHandler::name() {
  return this->_name;
}

bool settingsHandler::useDHCP() {
  return this->_dhcp;
}

IPAddress settingsHandler::localIP() {
  return this->_ip;
}

IPAddress settingsHandler::gatewayIP() {
  return this->_gw;
}

IPAddress settingsHandler::subnetMask() {
  return this->_mask;
}

void settingsHandler::name(const char* _name) {
  this->setField(this->_name, _name, 32);
#ifdef S_DEBUG
  this->_debug->print(F("Settings:\tChanged name to "));
  this->_debug->println(this->_name);
#endif
  return void();
}

void settingsHandler::setField(char* _dest, const char* _src, uint8_t _size) {
  uint8_t _len = strlen(_src);
  if (_len > (_size - 1)) _len = (_size - 1);
  memcpy(_dest, _src, _len);
  _dest[_len] = '\x00';
  return void();
}

IPAddress settingsHandler::stringToIP(const char* input) {
  uint8_t parts[4] = {0, 0, 0, 0};
  uint8_t part = 0;
  for (uint8_t a = 0; a < strlen(input); a++) {
    uint8_t b = input[a];
    if (b == '.') {
      part++;
      continue;
    }
    parts[part] *= 10;
    parts[part] += b - '0';
  }
  return IPAddress(parts[0], parts[1], parts[2], parts[3]);
}

String settingsHandler::IPtoString(IPAddress address) {
  String out;
  for (int z = 0; z < 4; z++) {
    out += String(address[z]);
    if (z < 3)out += ".";
  }
  return out;
}
