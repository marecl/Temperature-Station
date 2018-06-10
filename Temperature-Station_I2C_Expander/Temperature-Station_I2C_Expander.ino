#include <Wire.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define F_CPU 8000000L
#define N_PORTS 8
//#define DEBUG

/*
   Use Arduino Pro Mini 3V3 bootloader and internal 8MHz clock
   Change BOD detection to 1.8V (2.8V if you're not experiencing resets)

   ToDo: 1wire jumpers (global rail interferes with searching)
   When bypassing - add jumpers
   No bypass - every 1wire device with separate circuit!

   0x75 - probe sensors
   0x78 - send connected sensors
   0x81[N] - sensor address from port N (only when 1wire bypass is not used!)
   0x84[N] - send measurement from port N
   0x87 - get port count
   0x90 - is 1wire bypassed
*/

/* Pins, temperatures, saved ports */
const uint8_t address = 0x10;
const uint8_t _pins[N_PORTS] = {5, 6, 7, 8, 9, 10, 11, 12};
static double _temps[N_PORTS];  //Readings
static uint8_t _ports[N_PORTS]; //Socket status
static uint8_t _addr[N_PORTS][8]; //1wire address
static uint8_t* strtemp = new uint8_t[(N_PORTS < 8) ? 8 : N_PORTS];
volatile uint8_t command, reqport; //Command and requested port
bool bypass1Wire = false; //Should it avoid using 1wire
uint8_t toSend = 0; //Part of NULL solution

/*
    0 - no
    1 - not recognized
    5 - 1wire
    11/21/22 - DHT
*/

void setup() {
  /*
    For this option to work:
      No bypass: cut all 1wire jumpers out from connectors, solder A3 jumper
      Bypass: Connect all jumpers, desolder A3 jumper
  */
  pinMode(A3, INPUT_PULLUP);
  if (digitalRead(A3))
    bypass1Wire = true;

  //Quick look if it's reading sensors correctly
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println(F("\r\nI2C Expander Debug"));
  Serial.print(F("Address: 0x"));
  Serial.println(address, HEX);
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

  Wire.begin(address);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
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
  }
}

void requestEvent() {
  for (int _a = 0; _a < toSend; _a++)
    Wire.write(strtemp[_a]); //Master needs response immediately
  return void();
}

void receiveEvent(int numBytes) {
  if (numBytes == 1)
    command = Wire.read();
  else if (numBytes == 2) {
    command = Wire.read();
    reqport = Wire.read();
  }
  return void();
}

void refreshPorts() { //Detect if ports are used
  const uint8_t _t[3] = {11, 21, 22};
  for (uint8_t a = 0; a < N_PORTS; a++) {
    //Setup for detecting plugged sensors
    digitalWrite(_pins[a], LOW);
    pinMode(_pins[a], INPUT);

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
        _ports[a] = 5;
        continue;
      }
    }

    for (int z = 0; z < 3; z++) { //Just try all possibilities
      DHT dht(_pins[a], _t[z]);
      dht.begin();
      double b = dht.readTemperature();
      if (!isnan(b) && b < 200) {
        _ports[a] = _t[z];
        _temps[a] = b; //We already have correct temperature
        break;
      }
    }
  }
  return void();
}
