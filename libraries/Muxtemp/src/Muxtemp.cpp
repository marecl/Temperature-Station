#include <Arduino.h>
#include <cstdlib>
#include <Wire.h>
#include "Muxtemp.h"

#define MUXTEMP_PROBE 0x75
#define MUXTEMP_SEND 0x78
#define MUXTEMP_READ 0x84
#define MUXTEMP_COUNT 0x87

/*
   0x75 - probe sensors
   0x78 - send connected sensors
   0x81 - refresh measurements
   0x84[n] - send measurement from port n
   0x87 - get port count
*/

Muxtemp::Muxtemp(TwoWire &_Wire) {
  this->_Wire = &_Wire;
}

Muxtemp::~Muxtemp() {
}

byte Muxtemp::begin(uint8_t _addr) {
  this->_addr = _addr;
  _Wire->beginTransmission(this->_addr);
  byte _ret = _Wire->endTransmission(true);
  this->_count = _readCount();
  this->_sensors = new uint8_t[this->_count];
  return _ret;
}

uint8_t Muxtemp::_readCount() {
  _Wire->beginTransmission(this->_addr);
  _Wire->write(MUXTEMP_COUNT);
  _Wire->endTransmission();

  _Wire->requestFrom(this->_addr, 1);

  return _Wire->read();
}

uint8_t Muxtemp::getCount() {
  return this->_count;
}

void Muxtemp::refreshPorts() {
  _Wire->beginTransmission(this->_addr);
  _Wire->write(MUXTEMP_PROBE);
  _Wire->endTransmission();
  return void();
}

float Muxtemp::getTemp(uint8_t _port) {
  _Wire->beginTransmission(this->_addr);
  _Wire->write(MUXTEMP_READ);
  _Wire->write(_port);
  _Wire->endTransmission();
  delay(5);
  _Wire->requestFrom(this->_addr, 7);
  delay(5);
  String r = "";
  while (_Wire->available()) {
    char t = _Wire->read();
    if (t != 0 && t != 255)r += t;
  }
  return atof(r.c_str());
}

void Muxtemp::getPorts() {
  _Wire->beginTransmission(this->_addr);
  _Wire->write(MUXTEMP_SEND);
  _Wire->endTransmission();
  delay(5);
  _Wire->requestFrom(this->_addr, this->_count);
  delay(5);
  for (int _z = 0; _z < this->_count; _z++) {
    uint8_t _a = Wire.read();
	if (_a != 0 && _a != 255)
    this->_sensors[_z] = _a;
  }
  return void();
}

uint8_t Muxtemp::typeOf(uint8_t _p) {
  return this->_sensors[_p];
}

