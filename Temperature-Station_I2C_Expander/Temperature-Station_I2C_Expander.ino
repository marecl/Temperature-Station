#include <DallasTemperature.h>
#include <DHT.h>
#include <OneWire.h>
#include <Wire.h>

/*
   Use Arduino Pro Mini 3V3 bootloader and internal 8MHz clock
   Change BOD detection to 1.8V (2.8V if you're not experiencing resets)

   This sketch will fit into ATmega328/168

   0x75 - probe sensors
   0x78 - send connected sensors
   0x81[N] - sensor address from port N (only when 1wire bypass is not used!)
   0x84[N] - send measurement from port N
   0x87 - get port count
   0x90 - is 1wire bypassed

   Types:
      0 - not connected
      1 - connected, not recognized
      11/21/22 - DHTxx
      40 - DS18B20 (or from the same device family)
*/

/* Address */
#define I2C_ADDR 0x10

/* Misc config */
#define F_CPU 8000000L
#define N_PORTS 8

/* Pins 8 */
#define LOCK_P A3
#define BYPASS_P A2

/* Uncomment when debugging */
//#define DEBUG

/* Pins, temperatures, saved ports */
const uint8_t _pins[N_PORTS] = {5, 6, 7, 8, 9, 10, 11, 12};
static double _temps[N_PORTS];  //Readings
static uint8_t _ports[N_PORTS]; //Socket status
static uint8_t _addr[N_PORTS][8]; //1wire address
static uint8_t* strtemp = new uint8_t[(N_PORTS < 8) ? 8 : N_PORTS];
volatile uint8_t command, reqport; //Command and requested port
bool bypass1Wire = false; //Should it avoid using 1wire
uint8_t toSend = 0; //Part of NULL solution

void setup() {
  pinMode(LOCK_P, OUTPUT);

  //Preamble to distinguish reboot from normal work
  for (uint8_t _s = 0; _s < 255; _s++) {
    digitalWrite(LOCK_P, HIGH);
    digitalWrite(LOCK_P, LOW);
  }

  /*
    For this option to work:
      No bypass: cut all 1wire jumpers out from connectors, solder A2 jumper
      Bypass: Connect all jumpers, desolder A3 jumper
      Or just move everything to DEFINE, whatever
  */
  pinMode(BYPASS_P, INPUT_PULLUP);
  if (digitalRead(BYPASS_P))
    bypass1Wire = true;

  //Quick look if it's reading sensors correctly
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println(F("\r\nI2C Expander Debug"));
  Serial.print(F("Address: 0x"));
  Serial.println(I2C_ADDR, HEX);
  Serial.print(F("Ports: "));
  Serial.println(N_PORTS);
  Serial.print(F("1wire bypass: "));
  Serial.println(bypass1Wire ? F("Yes") : F("No"));
  refreshPorts();
  for (int z = 0; z < N_PORTS; z++) {
    Serial.print(z);
    Serial.print(F(" ("));
    Serial.print(_ports[z]);
    Serial.print(F(") "));
    Serial.print(_temps[z]);
    Serial.print(F(" ("));
    for (int q = 0; q < 8; q++) {
      Serial.print(_addr[z][q]);
      if (q == 7) Serial.print(F(")\r\n"));
      else Serial.print(F(","));
    }
  }
#endif

  Wire.begin(I2C_ADDR);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  digitalWrite(LOCK_P, HIGH); //Ready to work
}

void loop() {
  if (command != 0x00 && command != 0xFF) {
    switch (command) {
      case 0x75: refreshPorts(); break;
      case 0x78: for (int b = 0; b < N_PORTS; b++)
          strtemp[b] = _ports[b];
        toSend = N_PORTS;
        break;
      case 0x81: if (!bypass1Wire) {
          for (uint8_t _a = 0; _a < 8; _a++) {
            strtemp[_a] = _addr[reqport][_a];
#ifdef DEBUG
            Serial.print(strtemp[_a], DEC);
            Serial.print('|');
            Serial.print(_addr[reqport][_a]);
            if (_a != 7) Serial.print(" ");
            else Serial.println();
#endif
          }
          toSend = 8;
        } break;
      case 0x84: strcpy((char*)strtemp, String(_temps[reqport]).c_str());
        toSend = strlen((char*)strtemp); break;
      case 0x87: strtemp[0] = N_PORTS;
        toSend = 1;
        break;
      case 0x90: strtemp[0] = bypass1Wire ? 1 : 0;
        toSend = 1; break;
    }
    command = 0;
    reqport = 0;
    digitalWrite(LOCK_P, HIGH); //Command done, unlock
  }
}

void requestEvent() {
  for (int _a = 0; _a < toSend; _a++)
    Wire.write(strtemp[_a]); //Master needs response immediately
  return void();
}

void receiveEvent(int numBytes) {
  digitalWrite(LOCK_P, LOW); //Receiving command, lock
  if (numBytes == 1)
    command = Wire.read();
  else if (numBytes == 2) {
    command = Wire.read();
    reqport = Wire.read();
  } else digitalWrite(LOCK_P, HIGH);
  return void();
}

void refreshPorts() { //Detect if ports are used
  for (uint8_t a = 0; a < N_PORTS; a++) {
    //Setup for detecting plugged sensors
    pinMode(_pins[a], INPUT);
    digitalWrite(_pins[a], LOW);

    if (!digitalRead(_pins[a])) {
      _ports[a] = 0; //NOT connected
      _temps[a] = -127.00;
      for (uint8_t _a = 0; _a < 8; _a++)
        _addr[a][_a] = 0;
      continue;
    } else _ports[a] = 1; //Connected, not yet recognized

    if (!bypass1Wire) {
      byte _s[8];
      OneWire oneWire(_pins[a]);
      DallasTemperature sensors(&oneWire);
      if (oneWire.search(_s) && OneWire::crc8( _s, 7) == _s[7]) {
        sensors.requestTemperatures();
        _temps[a] = sensors.getTempC(_s);
        for (uint8_t _a = 0; _a < 8; _a++)
          _addr[a][_a] = _s[_a];
        _ports[a] = 40; //Always first byte of DS18B20 address
        continue;
      }
    }

    for (int z = 0; z < 3; z++) { //Just try all possibilities
      const uint8_t _t[3] = {11, 21, 22}; //DHTxx
      double b;
      DHT dht(_pins[a], _t[z]);
      dht.begin();
      b = dht.readTemperature();
      if (!isnan(b) && b < 85) {
        _ports[a] = _t[z];
        _temps[a] = b; //We already have correct temperature
        break;
      }
    }
  }
  return void();
}
