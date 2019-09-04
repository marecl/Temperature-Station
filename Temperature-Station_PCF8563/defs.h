/*
  Mainboard_1.1
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

/* Good when trying different configurations */
#define SETTINGS_FILE "/SET.TXT" //SPIFFS
#define SENSORS_FILE "/SENSORS.TXT" //SD

//int isMember(byte, JsonArray&);
int isMember(JsonArray&, JsonArray&);
//int isMember(int, JsonArray&);
int isMember(const uint8_t*, JsonArray&, uint8_t);
String addrToString(const uint8_t*, uint8_t);
//String addrToString(JsonArray&);

/*
   int isMember(byte _1[], JsonArray& compArr) {
  for (uint8_t a = 0; a < compArr.size(); a++) {
    for (uint8_t b = 0; b < 8; b++) {
      if (_1[b] != compArr[a][1][b].as<byte>())
        break;
      else if (b == 7) return a;
    }
  }
  return -1;
  }
*/

void printSensors(Muxtemp* _s, Stream& _stream) {
  DynamicJsonBuffer jsonBuffer(2250);
  File root = SD.open(SENSORS_FILE, FILE_READ);
  JsonObject& nSet = jsonBuffer.parseObject(root);
  JsonArray& _local = nSet["local"];
  JsonArray& _saved = nSet["saved"];
  root.close();

  _stream.println(F("\r\nPort\tType\tPort Name\tDevice Name\t\tAddress (if supported)"));
  for (int a = 0; a < _s->getCount(); a++) {
    uint8_t _type = _s->typeOf(a);
    JsonArray& _curr = _local[a];

    _stream.print(String(a + 1) + "/" + String(_s->getCount()));
    _stream.print('\t' + String(_type) + '\t');
    const char* _n = _curr[0];
    _stream.print(_n);

    if (_type == 40) {
      for (uint8_t a = 0; a < 2 - (strlen(_n) / 8); a++)
        _stream.print('\t');
      uint8_t *_q = _s->getAddress(a);
      int pos = isMember(_q, _saved, 8);

      _n = _saved[pos][0];
      _stream.print(_n);
      for (uint8_t a = 0; a < 3 - (strlen(_n) / 8); a++)
        _stream.print('\t');
      _stream.print(F("("));

      for (uint8_t y = 0; y < 8; y++) {
        _stream.print(*(_q + y));
        _stream.print(y < 7 ? ',' : ')');
      }
    }
    _stream.println();
  }

  _stream.println(F("\r\nAll sensors:"));
  for (uint8_t a = 0; a < _saved.size(); a++) {
    JsonArray& _e =  _saved[a];
    _stream.print('\t' + String(a + 1) + '.');
    _stream.print(_e[0].as<const char*>());
    _stream.print('\t');
    _stream.println(_e[1].as<const char*>());
  }
  _stream.println();
}

bool addDiff(JsonArray& _s, char* _r) {
  bool _ret = false;
  DynamicJsonBuffer remoteBuffer;
  JsonArray& _rdev = remoteBuffer.parseArray(_r);
  if (!_rdev.success()) Serial.println("JSON error");
  else {
    for (uint8_t z = 0; z < _rdev.size(); z++) {
      if (isMember(_s, _rdev[z][1]) == -1) {
        JsonArray& _e = _s.createNestedArray();
        _e.add(_rdev[z][0]);
        _e.add(_rdev[z][1]);
        _ret = true;
      }
    }
  }
  return _ret;
}

int isMember(JsonArray& _l, JsonArray& _r) { //In context of saved devices
  for (uint8_t locsiz = 0; locsiz < _l.size(); locsiz++) {
    JsonArray& _e = _l[locsiz][1];
    for (uint8_t a = 0; a < 8; a++) {
      if (_e[a].as<byte>() != _r[a].as<byte>())
        break;
      else if (a == 7) return locsiz;
    }
  }
  return -1;
}

/*
  int isMember(int locAddr, JsonArray& remArr) {
  for (uint8_t remSiz = 0; remSiz < remArr.size(); remSiz++) {
    if (remArr[remSiz][1].is<int>() == false) continue;
    if (locAddr != remArr[remSiz][1].as<int>())
      continue;
    else return remSiz;
  }
  return -1;
  }
*/

int isMember(const uint8_t *_a, JsonArray& compArr, uint8_t _s) {
  for (uint8_t a = 0; a < compArr.size(); a++) {
    for (uint8_t b = 0; b < _s; b++) {
      if (*(_a + b) != compArr[a][1][b].as<uint8_t>())
        break;
      else if (b == 7) return a;
    }
  }
  return -1;
}

String addrToString(const uint8_t *_addr, uint8_t _s) {
  String out = "";
  for (uint8_t _a = 0; _a < _s; _a++)
    out += String(*(_addr + _a));
  return out;
}

/*
  String addrToString(JsonArray& _addr) {
  String out = "";
  for (uint8_t a = 0; (a < 8 && a < _addr.size()); a++)
    out += _addr[a].as<String>();
  return out;
  }
*/

void saveJson(JsonObject& toSave, const char* _f) {
  if (!toSave.success()) return;
  if (SD.exists(_f)) SD.remove(_f);
  File root = SD.open(_f, FILE_WRITE);
  toSave.printTo(root);
  root.flush();
  root.close();
  return void();
}

String printDateTime(Czas & timeobj) {
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
  DynamicJsonBuffer jsonBuffer(2250);
  File root;
  if (!SD.exists(_n)) {
    Serial.println("no file yo");
    root = SD.open(_n, FILE_WRITE);
    root.print(F("{\"remote\":[],\"local\":[],\"saved\":[]}"));
    root.flush();
    root.close();
  }

  root = SD.open(_n);
  JsonObject& nSet = jsonBuffer.parseObject(root);
  root.close();

  JsonArray& _l = nSet["local"];
  JsonArray & _saved = nSet["saved"];

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
          break;
        } else _curr[1] = pos;
        delete _q;
        _ch = true;
        break;
    }
  }
  if (_ch) saveJson(nSet, _n);
  return;
}
