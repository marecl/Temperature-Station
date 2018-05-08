#include <Wire.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <DHT.h>
#define F_CPU 8000000L
#define _1WIRE 7
#define _port0 8
#define _port1 9
#define _port2 10
#define _port3 11
#define _port4 12
#define _port5 13
/*
   Use Arduino Pro Mini 3V3 bootloader and internal 8MHz clock
*/

/*
   ToDo:
   Detect 1wire
   Unstable?
   add I2C
*/

uint8_t _ports[6];
/*
    0 - no
    1 - present
    2 - 1wire
    11/21/22 - DHT
*/
double _temps[6];
//"40,255,9,127,112,23,3,6" 1wire test sensor

OneWire oneWire(_1WIRE);
DallasTemperature sensors(&oneWire);

void setup() {
  //Wire.begin(0x10);
  //Wire.onRequest(requestEvent);
  //Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  Serial.println("Slave begin");
  sensors.begin();
  
  //Setup for detecting plugged sensors
  for (int a = _port0; a <= _port5; a++) {
    pinMode(a, INPUT);
    digitalWrite(a, LOW);
  }
  
  probePorts();
  getPorts();
  printPorts();
  delay(2000);
}

void loop() {
  //Just for debugging purposes
  if (millis() % 2000 == 0) {
    getPorts();
    printPorts();
  }

  if (millis() % 7000 == 0)
    probePorts();
}

//Unused yet, I2C communication events
/*void requestEvent() {
  for (int b = 0; b < a.length(); b++)
    Wire.write((byte)a[b]); // respond with message of 6 bytes
  Wire.write("\n");
  // as expected by master
  }

  void receiveEvent(int numBytes) {
  addr = "";
  for (int z = 0; z < numBytes; z++)
    addr += (char)Wire.read();
  }
*/

void printPorts() { //Print port data
  for (int a = 0; a < 6; a++) {
    Serial.print("Port ");
    Serial.print(a);
    Serial.print(" (type ");
    Serial.print(_ports[a]);
    Serial.print("): ");
    Serial.println(_ports[a] ? String(_temps[a]) : (String)"N/C");
  }
  Serial.print("1wire devices: ");
  Serial.println(sensors.getDeviceCount());
}

void probePorts() { //Detect if ports are used
  Serial.println("Probing devices");
  uint8_t _t[3] = {11, 21, 22};

  for (int a = _port0; a <= _port5; a++) {
    digitalWrite(a, LOW);
    if (digitalRead(a))
      _ports[a - _port0] = 1;
    else
      _ports[a - _port0] = 0;
  }

  for (int a = _port0; a <= _port5; a++) {
    if (_ports[a - _port0] == 0) continue;
    double b;
    for (int z = 0; z < 3; z++) {
      DHT dht(a, _t[z]);
      dht.begin();
      b = dht.readTemperature();
      if (!isnan(b)) {
        _ports[a - _port0] = _t[z];
        _temps[a - _port0] = b;
        break;
      }
    }
  }
}

void getPorts() { //Get temperatures from sensors
  for (int a = _port0; a <= _port5; a++) {
    if (_ports[a - _port0] == 0 || !digitalRead(a)) continue;
    double b;
    DHT dht(a, _ports[a - _port0]);
    dht.begin();
    b = dht.readTemperature();
    if (!isnan(b)) _temps[a - _port0] = b;
  }
}

double getTemp_wire(String ad) {
  uint8_t tempaddr[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t part = 0;
  for (uint8_t z = 0; z < ad.length(); z++) {
    uint8_t b = ad[z];
    if (b == ',') {
      part++;
      continue;
    }
    tempaddr[part] *= 10;
    tempaddr[part] += b - '0';
  }
  return sensors.getTempC(tempaddr);
}
