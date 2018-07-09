#include "Muxtemp.h"

Muxtemp::Muxtemp(TwoWire &_W, uint8_t _a) {
  this->_Wire = &_W;
  this->_addr = _a;
  this->_available = 0;
  this->_count = 0;
  this->_delay = 5000;
}

Muxtemp::~Muxtemp() {
  delete[] _sensors;
}

uint8_t Muxtemp::begin() {
  _Wire->beginTransmission(this->_addr);
  byte _ret = _Wire->endTransmission(true);
  if (_ret) return _ret;
  this->_count = this->_readCount();
  this->_sensors = new uint8_t[this->_count];
  return 0;
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

void Muxtemp::refresh() {
  _Wire->beginTransmission(this->_addr);
  _Wire->write(MUXTEMP_REFRESH);
  _Wire->endTransmission();
  this->_available = millis() + this->_delay;
  return void();
}

void Muxtemp::setDelay(uint32_t _d) {
  this->_delay = _d;
  return void();
}

float Muxtemp::getTemp(uint8_t _p) {
  _Wire->beginTransmission(this->_addr);
  _Wire->write(MUXTEMP_READ);
  _Wire->write(_p);
  _Wire->endTransmission();
  delayMicroseconds(175);
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
  _Wire->beginTransmission(this->_addr);
  _Wire->write(MUXTEMP_SEND);
  _Wire->endTransmission();
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
  uint8_t *_a = new uint8_t[8];
  _Wire->beginTransmission(this->_addr);
  _Wire->write(MUXTEMP_ADDR);
  _Wire->write(_p);
  _Wire->endTransmission();
  _Wire->requestFrom(this->_addr, 8);
  for (uint8_t s = 0; s < 8; s++)
    _a[s] = _Wire->read();
  return &_a[0];
}

bool Muxtemp::bypass1Wire() {
  _Wire->beginTransmission(this->_addr);
  _Wire->write(MUXTEMP_BYPASS);
  _Wire->endTransmission();
  _Wire->requestFrom(this->_addr, 1);
  return Wire.read() == 1 ? true : false;
}

uint32_t Muxtemp::lock() {
  if (millis() > this->_available) return 0;
  else return this->_available - millis();
}