#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "settingsHandler.h"
#include "FS.h"

settingsHandler::settingsHandler() {
  this->_name = new char[32];
  this->_ssid = new char[32];
  this->_pass = new char[32];
  this->_ssidap = new char[32];
  this->_passap = new char[32];
  this->_user = new char[16];
  this->_pwd = new char[16];
  this->_ntp = new char[32];
  this->_dhcp = true;
  this->useNTP = false;
  this->lastUpdate = 0;
  this->timezone = 0;
}

settingsHandler::~settingsHandler() {

}

const char* settingsHandler::ntpServer() {
  return this->_ntp;
}

void settingsHandler::ntpServer(const char* _ntp) {
  setField(this->_ntp, _ntp, 32);
  return void();
}

void settingsHandler::save(const char* _filename) {
  SPIFFS.begin();
  File _toSave = SPIFFS.open(_filename, "w");
  _toSave.print(F("{\"DHCP\":"));
  _toSave.print(this->_dhcp ? "true" : "false");
  _toSave.print(F(",\"NAME\":\""));
  _toSave.print(this->_name);
  _toSave.print(F("\",\"SSID\":\""));
  _toSave.print(this->_ssid);
  _toSave.print(F("\",\"PASS\":\""));
  _toSave.print(this->_pass);
  _toSave.print(F("\",\"AP_SSID\":\""));
  _toSave.print(this->_ssidap);
  _toSave.print(F("\",\"AP_PASS\":\""));
  _toSave.print(this->_passap);
  _toSave.print(F("\",\"USR\":\""));
  _toSave.print(this->_user);
  _toSave.print(F("\",\"USR_PASS\":\""));
  _toSave.print(this->_pwd);
  _toSave.print(F("\",\"IP\":\""));
  _toSave.print(this->_ip);
  _toSave.print(F("\",\"GW\":\""));
  _toSave.print(this->_gw);
  _toSave.print(F("\",\"MASK\":\""));
  _toSave.print(this->_mask);
  _toSave.print(F("\",\"useNTP\":"));
  _toSave.print(this->useNTP ? "true" : "false");
  _toSave.print(F(",\"ntpServer\":\""));
  _toSave.print(this->_ntp);
  _toSave.print(F("\",\"lastUpdate\":"));
  _toSave.print(this->lastUpdate);
  _toSave.print(F(",\"tz\":"));
  _toSave.print(this->timezone);
  _toSave.print(F("}"));
  _toSave.flush();
  _toSave.close();
  SPIFFS.end();
  return void();
}

bool settingsHandler::load(const char* _f) {
  SPIFFS.begin();
  if (!SPIFFS.exists(_f)) {
    SPIFFS.end();
    return false;
  }
  File _toSave = SPIFFS.open(_f, "r");
  DynamicJsonBuffer _ld(JSON_OBJECT_SIZE(11) + 230);
  JsonObject& _data = _ld.parseObject(_toSave);
  _toSave.close();
  SPIFFS.end();
  if (!_data.success()) return false;
  _data.printTo(*(this->_debug));

  this->setField(this->_name, _data["NAME"], 32);
  this->setField(this->_ssid, _data["SSID"], 32);
  this->setField(this->_pass, _data["PASS"], 32);
  this->setField(this->_ssidap, _data["AP_SSID"], 32);
  this->setField(this->_passap, _data["AP_PASS"], 32);
  this->setField(this->_user, _data["USR"], 16);
  this->setField(this->_pwd, _data["USR_PASS"], 16);
  this->setField(this->_ntp, _data["ntpServer"], 32);
  this->_ip = stringToIP(_data["IP"]);
  this->_gw = stringToIP(_data["GW"]);
  this->_mask = stringToIP(_data["MASK"]);
  this->_dhcp = _data["DHCP"].as<bool>();
  this->useNTP = _data["useNTP"].as<bool>();
  this->lastUpdate = _data["lastUpdate"].as<uint32_t>();
  this->timezone = _data["tz"].as<short int>();
  return true;
}

void settingsHandler::serialDebug(HardwareSerial *_debug) {
  this->_debug = _debug;
  return void();
}

bool settingsHandler::reset(const char* _l, const char* _p, const char* _f) {
  if (this->authenticate(_l, _p) == false) return false;
  _debug->println("Removing config");
  SPIFFS.begin();
  SPIFFS.remove(_f);
  SPIFFS.end();
  return true;
}

void settingsHandler::configUpdateServer(ESP8266WebServer *_conf, ESP8266HTTPUpdateServer *_upd, const char* _url) {
  _upd->setup(_conf, _url, this->_user, this->_pwd);
  return void();
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

  if (WiFi.status() != WL_CONNECTED) return false;
  if (this->_dhcp == true) {
    this->_ip = WiFi.localIP();
    this->_gw = WiFi.gatewayIP();
    this->_mask = WiFi.subnetMask();
  }
  return true;
}

bool settingsHandler::beginAP() {
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_AP);
  WiFi.setOutputPower(0);
  return WiFi.softAP(this->_ssidap, this->_passap, false);
}

void settingsHandler::configUser(const char* _u, const char* _p) {
  this->setField(this->_user, _u, 16);
  this->setField(this->_pwd, _p, 16);
  return void();
}

bool settingsHandler::authenticate(const char* _username, const char* _password) {
  if (strcmp(this->_user, _username) == 0)
    if (strcmp(this->_pwd, _password) == 0)
      return true;
  return false;
}

void settingsHandler::config(const char* _ssid, const char* _pass) {
  this->setField(this->_ssid, _ssid, 32);
  this->setField(this->_pass, _pass, 32);
  return void();
}

void settingsHandler::configAP(const char* _ssidap, const char* _passap) {
  this->setField(this->_ssidap, _ssidap, 32);
  this->setField(this->_passap, _passap, 32);
  return void();
}

void settingsHandler::configIP(IPAddress _ip, IPAddress _gw, IPAddress _mask) {
  this->_ip = _ip;
  this->_gw = _gw;
  this->_mask = _mask;
  return void();
}

void settingsHandler::configIP(const char* _ip, const char* _gw, const char* _mask) {
  this->_ip = this->stringToIP(_ip);
  this->_gw = this->stringToIP(_gw);
  this->_mask = this->stringToIP(_mask);
  return void();
}

void settingsHandler::useDHCP(bool _dhcp) {
  this->_dhcp = _dhcp;
  return void();
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

void settingsHandler::ssid(const char* _ssid) {
  setField(this->_ssid, _ssid, 32);
  return void();
}

void settingsHandler::ssidAP(const char* _ssidap) {
  setField(this->_ssidap, _ssidap, 32);
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

void settingsHandler::name(const char* _name) {
  this->setField(this->_name, _name, 32);
  return void();
}

void settingsHandler::setField(char* _dest, const char* _src, uint8_t _size) {
  uint8_t _len = strlen(_src);
  if (_len > (_size - 1)) _len = (_size - 1);
  memcpy(_dest, _src, _len);
  _dest[_len] = '\x00';
  return void();
}
