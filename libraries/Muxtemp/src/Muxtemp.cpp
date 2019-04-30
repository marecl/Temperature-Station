#include "Muxtemp.h"

Muxtemp::Muxtemp(TwoWire &_W, uint8_t _a, uint8_t _l) {
  this->_lockPin = _l;
  pinMode(this->_lockPin, INPUT_PULLUP);
  this->_begin(_W, _a);
}

void Muxtemp::_begin(TwoWire &_W, uint8_t _a) {
  this->_Wire = &_W;
  this->_addr = _a;
  this->_available = 0;
  this->_count = 0;
}

Muxtemp::Muxtemp(TwoWire &_W, uint8_t _a) {
  this->_begin(_W, _a);
}

Muxtemp::~Muxtemp() {
  delete[] _sensors;
}

uint8_t Muxtemp::begin() {
  uint8_t _ret = this->send(255, 255);
  if (_ret) return _ret;
  this->_count = this->_readCount();
  this->_sensors = new uint8_t[this->_count];
  return 0;
}

uint8_t Muxtemp::_readCount() {
  this->send(MUXTEMP_COUNT, 255);
  _Wire->requestFrom(this->_addr, 1);
  return _Wire->read();
}

uint8_t Muxtemp::getCount() {
  return this->_count;
}

void Muxtemp::refresh() {
  this->send(MUXTEMP_REFRESH, 255);
  this->_available = millis() + this->_delay;
  return void();
}

float Muxtemp::getTemp(uint8_t _p) {
  this->send(MUXTEMP_READ, _p);
  delayMicroseconds(200);
  _Wire->requestFrom(this->_addr, 8);
  String r = "";
  for (uint8_t z = 0; z <= 8; z++) {
    char t = _Wire->read();
    if (t == 255) continue;
    else r += t;
  }
  return atof(r.c_str());
}

void Muxtemp::getPorts() {
  uint8_t _a = 0;
  this->send(MUXTEMP_SEND, 255);
  _Wire->requestFrom(this->_addr, this->_count);
  for (uint8_t _z = 0; _z < this->_count; _z++) {
    _a = Wire.read();
    if (_a != 255)
      this->_sensors[_z] = _a;
  }
  return void();
}

uint8_t Muxtemp::typeOf(uint8_t _p) {
  return this->_sensors[_p];
}

uint8_t *Muxtemp::getAddress(uint8_t _p) {
  uint8_t *_ta = new uint8_t[8];
  this->send(MUXTEMP_ADDR, _p);
  _Wire->requestFrom(this->_addr, 8);
  for (uint8_t s = 0; s < 8; s++)
    _ta[s] = _Wire->read();
  return &_ta[0];
}

bool Muxtemp::bypass1Wire() {
  this->send(MUXTEMP_BYPASS, 255);
  _Wire->requestFrom(this->_addr, 1);
  return Wire.read() == 1 ? true : false;
}

bool Muxtemp::lock() {
  return !digitalRead(this->_lockPin);
}

uint8_t Muxtemp::send(uint8_t _d, uint8_t _dd) {
  _Wire->beginTransmission(this->_addr);
  if (_d != 255)
    _Wire->write(_d);
  if (_dd != 255)
    _Wire->write(_dd);
  return _Wire->endTransmission();
}