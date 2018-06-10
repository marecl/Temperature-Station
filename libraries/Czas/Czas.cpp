#include "Czas.h"
#define LEAP_YEAR(Y) ( ((1970+(Y))>0) && !((1970+(Y))%4) && ( ((1970+(Y))%100) || !((1970+(Y))%400) ) )
/* PCF8563 inferfacing library */

Czas::Czas(TwoWire &_Wire) {
  this->_Wire = &_Wire;
  this->_Wire->beginTransmission(RTC_ADDR);
  this->_Wire->write(0x0D);
  this->_Wire->write(0x00); //disable clock output
  this->_Wire->endTransmission();
}

Czas::~Czas() {
}

byte Czas::bcdToDec(byte value) {
  return ((value / 16) * 10 + value % 16);
}

byte Czas::decToBcd(byte value) {
  return (value / 10 * 16 + value % 10);
}

void Czas::readRTC() {
  this->_Wire->beginTransmission(RTC_ADDR);
  this->_Wire->write(0x02);
  this->_Wire->endTransmission();
  this->_Wire->requestFrom(RTC_ADDR, 7);
  this->_second = this->bcdToDec(_Wire->read() & B01111111); // sek
  this->_minute = this->bcdToDec(_Wire->read() & B01111111); // min
  this->_hour = this->bcdToDec(_Wire->read() & B00111111); //godz
  this->_day = this->bcdToDec(_Wire->read() & B00111111); //dz msc
  this->_dow = this->bcdToDec(_Wire->read() & B00000111); //dz tyg
  this->_month = this->bcdToDec(_Wire->read() & B00011111); // msc
  this->_year = this->bcdToDec(_Wire->read()); //rok
  this->_year += 2000;
}

void Czas::setRTC(uint32_t t) {
  t -= 946684800UL;
  uint8_t ss = t % 60;
  t /= 60;
  uint8_t mm = t % 60;
  t /= 60;
  uint8_t hh = t % 24;
  uint16_t d = t / 24;
  int dt = (d % 7) - 1;
  uint8_t przest = 0;
  uint16_t r;
  uint8_t m;
  for (r = 0; ; ++r) {
    przest = r % 4 == 0;
    if (d < 365 + przest) break;
    d -= 365 + przest;
  }
  for (m = 1; ; m++) {
    uint8_t dniwmsc = pgm_read_byte(daysInMonth + m - 1);
    if (przest && m == 2)
      dniwmsc++;
    if (d < dniwmsc)
      break;
    d -= dniwmsc;
  }
  d++;
  if (r > 2000) r -= 2000;
  this->_Wire->beginTransmission(RTC_ADDR);
  this->_Wire->write(0x02);
  this->_Wire->write(this->decToBcd(ss)); //sek
  this->_Wire->write(this->decToBcd(mm)); //min
  this->_Wire->write(this->decToBcd(hh)); //godz
  this->_Wire->write(this->decToBcd(d)); //dzien
  this->_Wire->write(this->decToBcd(dt)); //dzien tyg (1-7)
  this->_Wire->write(this->decToBcd(m)); //msc
  this->_Wire->write(this->decToBcd(r)); //rok-2000
  this->_Wire->endTransmission();
}

void Czas::setRTC(uint8_t ss, uint8_t mm, uint8_t hh, uint16_t d, uint8_t dt, uint8_t m, uint16_t r) {
  this->_Wire->beginTransmission(RTC_ADDR);
  this->_Wire->write(0x02);
  this->_Wire->write(this->decToBcd(ss)); //sek
  this->_Wire->write(this->decToBcd(mm)); //min
  this->_Wire->write(this->decToBcd(hh)); //godz
  this->_Wire->write(this->decToBcd(d)); //dzien
  this->_Wire->write(this->decToBcd(dt)); //dzien tyg (1-7)
  this->_Wire->write(this->decToBcd(m)); //msc
  if (r > 2000) r -= 2000;
  this->_Wire->write(this->decToBcd(r)); //rok-2000
  this->_Wire->endTransmission();
}

uint32_t Czas::dateToEpoch(uint8_t ss, uint8_t mm, uint8_t hh, uint16_t d, uint8_t m, uint8_t r) {
  uint32_t dte = 946684800UL;
  dte += ss;
  dte += mm * 60;
  dte += hh * 3600;
  dte += d * 86400L;

  for (int a = 0; a < m; a++)
    dte += (uint8_t)pgm_read_byte(daysInMonth + a - 1) * 86400L;

  r -= 2000;
  dte += (r % 4) * 31536000;
  dte += ((r - (r % 4)) / 4) * 126230400;
  return (dte);
}

uint32_t Czas::dateAsEpoch() {
  return (this->dateToEpoch(this->_second, this->_minute, this->_hour, this->_day, this->_month, this->_year));
}

bool Czas::compareTimeEpoch(uint32_t t, int tolerance) { //tolerance in seconds, less than 1 minute
  t -= 946684800UL;
  uint8_t ss = t % 60;
  t /= 60;
  uint8_t mm = t % 60;
  t /= 60;
  uint8_t hh = t % 24;
  uint16_t d = t / 24;
  uint8_t przest = 0;
  uint16_t r;
  uint8_t m;
  for (r = 0; ; ++r) {
    przest = r % 4 == 0;
    if (d < 365 + przest) break;
    d -= 365 + przest;
  }
  for (m = 1; ; m++) {
    uint8_t dniwmsc = pgm_read_byte(daysInMonth + m - 1);
    if (przest && m == 2)
      dniwmsc++;
    if (d < dniwmsc)
      break;
    d -= dniwmsc;
  }
  d++;

  if (r != (this->_year - 2000)) return false;
  if (m != this->_month) return false;
  if (d != this->_day) return false;
  if (hh != this->_hour) return false;
  if (mm != this->_minute) return false;
  if (ss < (this->_second - tolerance)) return false;
  if (ss > (this->_second + tolerance)) return false;
  return true;
}

int Czas::second() {
  return this->_second;
}
int Czas::minute() {
  return this->_minute;
}
int Czas::hour() {
  return this->_hour;
}
int Czas::day() {
  return this->_day;
}
int Czas::month() {
  return this->_month;
}
int Czas::year() {
  return this->_year;
}