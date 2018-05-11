#include <Wire.h>
#include <DHT.h>
#define F_CPU 8000000L
#define N_PORTS 8
/*
   Use Arduino Pro Mini 3V3 bootloader and internal 8MHz clock
   Change BOD detection to 1.8V (2.8V if you're not experiencing resets)
*/

/*
   0x75 - probe sensors
   0x78 - send connected sensors
   0x81 - get measurements
   0x84[0xN] - send measurement from port N
   0x87 - get port count
*/

/* Pins, temperatures, saved ports */
uint8_t _pins[N_PORTS] = {5, 6, 7, 8, 9, 10, 11, 12};
double _temps[N_PORTS];
static volatile uint8_t _ports[N_PORTS];
uint8_t command, reqport;
uint8_t* strtemp = new uint8_t[N_PORTS];

/*
    0 - no
    1 - connected to port
    11/21/22 - DHT
*/

void setup() {
  probePorts();
  getPorts();

  Wire.begin(0x10);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop() {
  if (command != 0 && command != 0xFF) {
    if (command == 0x75) probePorts();
    else if (command == 0x78) {
      for (int b = 0; b < N_PORTS; b++)
        strtemp[b] = _ports[b];
    }
    else if (command == 0x81) getPorts();
    else if (command == 0x84)
      strcpy(strtemp, String(_temps[reqport]).c_str());
    else if (command == 0x87)
      strtemp[0] = N_PORTS;
    command = 0;
  }
}

void requestEvent() {
  Wire.write((char*)strtemp); //Master needs response immediately
}

void receiveEvent(int numBytes) { //Receive type of data
  command = Wire.read();
  if (command == 0x84) //If there is command - read argument
    reqport = Wire.read();
}

void probePorts() { //Detect if ports are used
  const uint8_t _t[3] = {11, 21, 22};
  for (int a = 0; a < N_PORTS; a++) {
    //Setup for detecting plugged sensors
    pinMode(_pins[a], INPUT);
    digitalWrite(_pins[a], LOW);

    if (!digitalRead(_pins[a])) {
      _ports[a] = 1; //NOT connected
      _temps[a] = -127.00;
      continue;
    } else _ports[a] = 2; //Connected, not yet recognized

    for (int z = 0; z < 3; z++) { //Just try all possibilities
      DHT dht(_pins[a], _t[z]);
      dht.begin();
      double b = dht.readTemperature();
      if (!isnan(b) && b < 200) {
        _ports[a] = _t[z];
        _temps[a] = b; //We already have correct temperature
      }
    }
  }
}

void getPorts() { //Get temperatures from sensors
  for (int a = 0; a < N_PORTS; a++) {
    if (_ports[a] <= 2 || !digitalRead(_pins[a])) continue;
    //Skip unconnected and unknown ones
    DHT dht(_pins[a], _ports[a]);
    dht.begin();
    double b = dht.readTemperature();
    if (!isnan(b)) _temps[a] = b;
  }
}
