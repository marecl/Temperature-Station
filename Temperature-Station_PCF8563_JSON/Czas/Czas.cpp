#include "Czas.h"

Czas::Czas(byte sda_pin, byte scl_pin) {
  dls = true;
  Wire.begin(sda_pin, scl_pin);
  Wire.beginTransmission(RTC_ADDR);
  Wire.write(0x0D);
  Wire.write(0x00); //disable clock output
  Wire.endTransmission();
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
  Wire.beginTransmission(RTC_ADDR);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDR, 7);
  this->second = this->bcdToDec(Wire.read() & B01111111); // sek
  this->minute = this->bcdToDec(Wire.read() & B01111111); // min
  this->hour = this->bcdToDec(Wire.read() & B00111111); //godz
  this->day = this->bcdToDec(Wire.read() & B00111111); //dz msc
  this->dow = this->bcdToDec(Wire.read() & B00000111); //dz tyg
  this->month = this->bcdToDec(Wire.read() & B00011111); // msc
  this->year = this->bcdToDec(Wire.read()); //rok
  this->year += 2000;
}

void Czas::setRTC(uint32_t t) {
  t -= 946684800UL;
  t = t + (3600 * timezone);
  if (this->dls) t += 3600;
  uint8_t ss = t % 60;
  t /= 60;
  uint8_t mm = t % 60;
  t /= 60;
  uint8_t hh = t % 24;
  uint16_t d = t / 24;
  int dt = (d % 7) - 1;
  uint8_t przest = 0;
  uint8_t r;
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

  Wire.beginTransmission(RTC_ADDR);
  Wire.write(0x02);
  Wire.write(this->decToBcd(ss)); //sek
  Wire.write(this->decToBcd(mm)); //min
  Wire.write(this->decToBcd(hh)); //godz
  Wire.write(this->decToBcd(d)); //dzien
  Wire.write(this->decToBcd(dt)); //dzien tyg (1-7)
  Wire.write(this->decToBcd(m)); //msc
  Wire.write(this->decToBcd(r)); //rok-2000
  Wire.endTransmission();
}
