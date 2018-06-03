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

   0x75 - probe sensors
   0x78 - send connected sensors
   0x81[N] - sensor address from port N (only when 1wire bypass is not used!)
   0x84[N] - send measurement from port N
   0x87 - get port count
   0x90 - is 1wire bypassed
*/

/* Pins, temperatures, saved ports */
const uint8_t _pins[N_PORTS] = {5, 6, 7, 8, 9, 10, 11, 12};
double _temps[N_PORTS];  //Readings
uint8_t _ports[N_PORTS]; //Socket status
uint8_t _addr[N_PORTS][8]; //1wire address
uint8_t* strtemp = new uint8_t[(N_PORTS < 8) ? 8 : N_PORTS];
uint8_t command, reqport; //Command and requested port
bool bypass1Wire = false; //Should it avoid using 1wire
uint8_t toSend = 0; //Part of NULL solution

/*
    0 - no
    1 - connected to port (probably 1wire)
    5 - 1wire (if not bypassed)
    11/21/22 - DHT
    255 - unknown

    DS18B20 can have NULLs in address so printing directly dosent work
*/

void setup() {
  //If master does support 1wire interface the device will not use it
  //But if this pin is shorted to ground - slave will treat it as its own device
  pinMode(A3, INPUT_PULLUP);
  if (digitalRead(A3))
    bypass1Wire = true;

  Wire.begin(0x10);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

//Quick look if it's reading sensors correctly
#ifdef DEBUG
  Serial.begin(115200);
  refreshPorts();
  for (int z = 0; z < 8; z++) {
    Serial.print(z);
    Serial.print(" (");
    Serial.print(_ports[z]);
    Serial.print(") ");
    Serial.print(_temps[z]);
    Serial.print(" (");
    for (int q = 0; q < 8; q++) {
      Serial.print(_addr[z][q]);
      if (q == 7) Serial.print(")\r\n");
      else Serial.print(",");
    }
  }
#endif
}

void loop() {
  if (command != 0x00 && command != 0xFF) {
    switch (command) {
      case 0x75: refreshPorts(); break;
      case 0x78: for (int b = 0; b < N_PORTS; b++)
          strtemp[b] = _ports[b];
        strtemp[N_PORTS] = 0x00;
        toSend = N_PORTS;
        break;
      case 0x81: if (!bypass1Wire) {
          for (uint8_t _a = 0; _a < 8; _a++)
            strtemp[_a] = _addr[reqport][_a];
          strtemp[8] = 0x00;
          toSend = 8;
        } break;
      case 0x84: strcpy(strtemp, String(_temps[reqport]).c_str());
        toSend = strlen(strtemp); break;
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

void receiveEvent(int numBytes) { //Receive type of data
  command = Wire.read();
  if (command == 0x84 || command == 0x81) //If there is command - read argument
    reqport = Wire.read();
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
      continue;
    } else _ports[a] = 1; //Connected, not yet recognized

    if (bypass1Wire == false) {
      byte _t[8];
      OneWire oneWire(_pins[a]);
      DallasTemperature sensors(&oneWire);
      if (oneWire.search(_t) && OneWire::crc8( _t, 7) == _t[7]) {
        sensors.requestTemperatures();
        _temps[a] = sensors.getTempC(_t);
        for (uint8_t _a = 0; _a < 8; _a++)
          _addr[a][_a] = _t[_a];
        _ports[a] = 5;
      } else {
        for (uint8_t _a = 0; _a < 8; _a++)
          _addr[a][_a] = 0;
        _ports[a] = 255;
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
