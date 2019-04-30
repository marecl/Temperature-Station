#define SD_D D0 //GPIO16
#define SDA D1 //GPIO5
#define SCL D2 //GPIO4
#define OW_PORT D3 //GPIO0
#define SD_CS D4 //GPIO2

void refreshSensors(OneWire*, const char*);
bool isMember(byte[], JsonArray&);
void saveJson(JsonArray&, const char*);
String printDateTime(const RtcDateTime&);
void I2C_clearBus();

void createFile(char* _fn, uint8_t _buf, RtcDateTime& dt) {
  memset(_fn, 0, _buf);
  snprintf_P(_fn, _buf, PSTR("/data/%04u%02u"), dt.Year(), dt.Month());
  SD.mkdir(_fn);
  snprintf_P(_fn, _buf, PSTR("%s/%02u.csv"), _fn, dt.Day());
}

void refreshSensors(OneWire *_ow, const char* _n) {
  DynamicJsonBuffer jsonBuffer(1000);
  File root;
  if (!SD.exists(_n)) {
    Serial.println("no file yo");
    root = SD.open(_n, FILE_WRITE);
    root.print(F("[]"));
    root.flush();
    root.close();
  }

  root = SD.open(_n, FILE_READ);
  JsonArray& nSet = jsonBuffer.parseArray(root);
  root.close();

  bool _ch = false;
  byte _a[8];

  while (_ow->search(_a)) {
    if (isMember(_a, nSet) == false) {
      if (OneWire::crc8(_a, 7) == _a[7]) {
        _ch = true;
        JsonArray& _e = nSet.createNestedArray();
        _e.add("New_" + String(nSet.size()));
        JsonArray& _r = _e.createNestedArray();
        for (uint8_t x = 0; x < 8; x++)
          _r.add(_a[x]);
      }
    }
  }
  _ow->reset_search();

  if (_ch) saveJson(nSet, _n);
  return;
}

bool isMember(byte _1[], JsonArray& compArr) {
  for (uint8_t a = 0; a < compArr.size(); a++) {
    for (uint8_t b = 0; b < 8; b++) {
      uint8_t _2 = compArr[a][1][b];
      if (_1[b] != _2)
        break;
      else if (b == 7) return true;
    }
  }
  return false;
}

void saveJson(JsonArray& toSave, const char* _f) {
  if (!toSave.success()) return;
  if (SD.exists(_f)) SD.remove(_f);
  File root = SD.open(_f, FILE_WRITE);
  toSave.printTo(root);
  root.flush();
  root.close();
  return void();
}

String printDateTime(const RtcDateTime & dt) {
  char datestring[20];
  snprintf_P(datestring, 20,
             PSTR("%02u/%02u/%04u;%02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute());
  return datestring;
}

void I2C_clearBus() {
  //http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html
  Serial.println(F("Resetting RTC I2C interface"));
  pinMode(SDA, INPUT_PULLUP);
  pinMode(SCL, INPUT_PULLUP);

  delay(2500);

  boolean SCL_LOW = (digitalRead(SCL) == LOW); // Check is SCL is Low.
  if (SCL_LOW)
    Serial.println("Cannot become master");

  boolean SDA_LOW = (digitalRead(SDA) == LOW);  // vi. Check SDA input.
  int clockCount = 20; // > 2x9 clock

  while (SDA_LOW && (clockCount > 0)) { //  vii. If SDA is Low,
    clockCount--;

    pinMode(SCL, INPUT);
    pinMode(SCL, OUTPUT);
    delayMicroseconds(10);
    pinMode(SCL, INPUT);
    pinMode(SCL, INPUT_PULLUP);
    delayMicroseconds(10); //  for >5uS
    // The >5uS is so that even the slowest I2C devices are handled.
    SCL_LOW = (digitalRead(SCL) == LOW); // Check if SCL is Low.
    int counter = 20;
    while (SCL_LOW && (counter > 0)) {  //  loop waiting for SCL to become High only wait 2sec.
      counter--;
      delay(100);
      SCL_LOW = (digitalRead(SCL) == LOW);
    }
    if (SCL_LOW) break;
    SDA_LOW = (digitalRead(SDA) == LOW); //   and check SDA input again and loop
  }
  if (SDA_LOW) { // still low
    Serial.println("SDA still low");
  }

  // else pull SDA line low for Start or Repeated Start
  pinMode(SDA, INPUT); // remove pullup.
  pinMode(SDA, OUTPUT);  // and then make it LOW i.e. send an I2C Start or Repeated start control.
  // When there is only one I2C master a Start or Repeat Start has the same function as a Stop and clears the bus.
  /// A Repeat Start is a Start occurring after a Start with no intervening Stop.
  delayMicroseconds(10); // wait >5uS
  pinMode(SDA, INPUT); // remove output low
  pinMode(SDA, INPUT_PULLUP); // and make SDA high i.e. send I2C STOP control.
  delayMicroseconds(10); // x. wait >5uS
  pinMode(SDA, INPUT); // and reset pins as tri-state inputs which is the default state on reset
  pinMode(SCL, INPUT);
  return void();
}
