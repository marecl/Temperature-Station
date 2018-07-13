/*
  #define SD_D D0 //GPIO16
  #define SDA D1 //GPIO5
  #define SCL D2 //GPIO4
  #define OW_PORT D3 //GPIO0
  #define SD_CS D4 //GPIO2
*/

#define SD_D A0
#define SDA 5
#define SCL 4
#define MUX_LOCK 2
#define SD_CS 16

static const uint8_t x509[] PROGMEM = {
#include "x509.h"
};
static const uint8_t rsakey[] PROGMEM = {
#include "key.h"
};

void saveJson(JsonObject& toSave, const char* _f) {
  if (!toSave.success()) return;
  if (SD.exists(_f)) SD.remove(_f);
  File root = SD.open(_f, FILE_WRITE);
  toSave.printTo(root);
  root.flush();
  root.close();
  return void();
}

String printDateTime(Czas& timeobj) {
  char datestring[20];
  snprintf_P(datestring, 20,
             PSTR("%02u/%02u/%04u;%02u:%02u"),
             timeobj.day(),
             timeobj.month(),
             timeobj.year(),
             timeobj.hour(),
             timeobj.minute());
  return datestring;
}

void refreshSensors(Muxtemp* _s, const char* _n) {
  DynamicJsonBuffer jsonBuffer(350);
  File root;
  if (!SD.exists(_n)) {
    Serial.println("no file yo");
    root = SD.open(_n, FILE_WRITE);
    root.print(F("{\"remote\":[],\"local\":[]}"));
    root.flush();
    root.close();
  }
  root = SD.open(_n);

  JsonObject& nSet = jsonBuffer.parseObject(root);
  root.close();
  JsonArray& _l = nSet["local"];

  int8_t dif = _s->getCount() - _l.size();
  if (dif > 0) {
    for (uint8_t z = 0; z < dif; z++)
      _l.add("Port_" + String(_l.size() + 1));
    saveJson(nSet, _n);
  }
  _s->getPorts();
  return void();
}
