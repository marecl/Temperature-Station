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
#define OW_PORT 2
#define SD_CS 16

#define countof(a) (sizeof(a) / sizeof(a[0]))

int isMember(byte, JsonArray&);
int isMember(JsonArray&, JsonObject&);
int isMember(int, JsonArray&);
String addrToString(uint8_t[8]);
String addrToString(JsonArray&);

static const uint8_t x509[] PROGMEM = {
#include "x509.h"
};
static const uint8_t rsakey[] PROGMEM = {
#include "key.h"
};

int isMember(byte _1[], JsonArray& compArr) {
  for (uint8_t a = 0; a < compArr.size(); a++) {
    for (uint8_t b = 0; b < 8; b++) {
      if (_1[b] != compArr[a]["a"][b].as<byte>())
        break;
      else if (b == 7) return a;
    }
  }
  return -1;
}

int isMember(JsonArray& locArr, JsonObject& remObj) {
  for (uint8_t locsiz = 0; locsiz < locArr.size(); locsiz++) {
    for (uint8_t a = 0; a < 8; a++) {
      if (locArr[locsiz]["a"][a].as<byte>() != remObj["a"][a].as<byte>())
        break;
      else if (a == 7) return locsiz;
    }
  }
  return -1;
}

int isMember(int locAddr, JsonArray& remArr) {
  for (uint8_t remSiz = 0; remSiz < remArr.size(); remSiz++) {
    if (remArr[remSiz]["a"].is<int>() == false) continue;
    if (locAddr != remArr[remSiz]["a"].as<int>())
      continue;
    else return remSiz;
  }
  return -1;
}

String addrToString(uint8_t _addr[8]) {
  String out = "";
  for (uint8_t a = 0; a < 8; a++)
    out += String(_addr[a]);
  return out;
}

String addrToString(JsonArray& _addr) {
  String out = "";
  for (uint8_t a = 0; (a < 8 && a < _addr.size()); a++)
    out += _addr[a].as<String>();
  return out;
}

void saveJson(JsonObject & toSave, const char* _f) {
  File root = SD.open(_f, FILE_WRITE);
  toSave.prettyPrintTo(root);
  root.flush();
  root.close();
  return;
}

String printDateTime(Czas & timeobj) {
  char datestring[20];
  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u;%02u:%02u"),
             timeobj.day(),
             timeobj.month(),
             timeobj.year(),
             timeobj.hour(),
             timeobj.minute());
  return datestring;
}

IPAddress stringToIP(char* input) {
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

String IPtoString(IPAddress address) {
  String out;
  for (int z = 0; z < 4; z++) {
    out += String(address[z]);
    if (z < 3)out += ".";
  }
  return out;
}

bool toBool(String input) {
  if (input == "") return false;
  else if (input[0] == '0') return false;
  else return true;
}

void refreshSensors(Muxtemp* _s, const char* _n) {
  DynamicJsonBuffer jsonBuffer(2250);
  File root = SD.open(_n, FILE_READ);
  JsonObject& nSet = jsonBuffer.parseObject(root);
  root.close();

  JsonArray& _saved = nSet["saved"];
  _s->getPorts();

  for (int a = 0; a < _s->getCount(); a++) {
    int pos;
    switch (_s->typeOf(a)) {
      case 0:
        nSet["local"][a] = -1;
        continue; break;
      case 11:
      case 21:
      case 22:
        pos = isMember(a, _saved);
        if (pos != -1) nSet["local"][a] = pos;
        else {
          JsonObject& _new = _saved.createNestedObject();
          switch (_s->typeOf(a)) {
            case 11: _new["n"] = "Unknown_DHT11"; break;
            case 21: _new["n"] = "Unknown_DHT21"; break;
            case 22: _new["n"] = "Unknown_DHT22"; break;
          }
          _new["a"] = a;
          a--;
        }
        continue; break;
      case 5:
        uint8_t q[8];
        uint8_t *_q = _s->getAddress(a);
        for (int z = 0; z < 8; z++) q[z] = *(_q + z);
        pos = isMember(q, _saved);
        if (pos == -1) {
          JsonObject& _new = _saved.createNestedObject();
          _new["n"] = addrToString(q);
          JsonArray& _ad = _new.createNestedArray("a");
          for (uint8_t y = 0; y < 8; y++) _ad.add(q[y]);
          a--;
          continue;
        } else nSet["local"][a] = pos;
    }
  }
  saveJson(nSet, _n);
}
