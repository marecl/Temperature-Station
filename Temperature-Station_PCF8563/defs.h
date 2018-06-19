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
int isMember(const uint8_t*, JsonArray&, uint8_t);
String addrToString(uint8_t[8]);
String addrToString(const uint8_t*, uint8_t);
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

String addrToString(const uint8_t *_addr, uint8_t _s) {
  String out = "";
  for (uint8_t _a = 0; _a < _s; _a++)
    out += String(*(_addr + _a));
  return out;
}

void saveJson(JsonObject & toSave, const char* _f) {
  if (SD.exists(_f)) SD.remove(_f);
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

IPAddress stringToIP(const char* input) {
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
  DynamicJsonBuffer jsonBuffer(1500);
  String _rt = "";
  if (SD.exists(_n)) {
    File root = SD.open(_n, FILE_READ);
    while (root.available()) {
      char _c = root.read();
      if (_c > 32 && _c < 127)
        _rt += _c;
    }
    root.close();
  } else _rt = F("{\"local\":[],\"saved\":[]}");
  
  JsonObject& nSet = jsonBuffer.parseObject(_rt);
  JsonArray& _l = nSet["local"];
  JsonArray & _saved = nSet["saved"];

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
