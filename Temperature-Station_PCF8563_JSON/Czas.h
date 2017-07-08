#ifndef CZAS_H
#include <pgmspace.h>
#include <Wire.h>
#define RTC_ADDR 0x51

/*
  I made this library to make interfacing with PCF8563 easier

  TODO:
  - Access to variables ONLY via functions
  - Make proper destructor to free memory when work is done
  - Make daylight saving automatic and move dls to private
  - Return actual date and time as epoch

*/

const uint8_t daysInMonth [] PROGMEM = { 31, 28, 31, 30, 31, 30,
                                         31, 31, 30, 31, 30, 31
                                       };
                                       
class czas { //PCF8563
  public:
    czas(byte sda_pin, byte scl_pin);
    ~czas();
    int second;
    int minute;
    int hour;
    int day;
    int month;
    int year;
    int dow; //Day of week
    bool dls; //Daylight saving
    void readRTC();
    void setRTC(uint32_t t);
    signed short int timezone;
  private:
    byte bcdToDec(byte value);
    byte decToBcd(byte value);
};

czas::czas(byte sda_pin, byte scl_pin) {
  dls = true;
  Wire.begin(sda_pin, scl_pin);
  Wire.beginTransmission(RTC_ADDR);
  Wire.write(0x0D);
  Wire.write(0x00); //disable clock output
  Wire.endTransmission();
}

czas::~czas() {
}

byte czas::bcdToDec(byte value) {
  return ((value / 16) * 10 + value % 16);
}

byte czas::decToBcd(byte value) {
  return (value / 10 * 16 + value % 10);
}

void czas::readRTC() {
  Wire.beginTransmission(RTC_ADDR);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(RTC_ADDR, 7);
  this->second = czas::bcdToDec(Wire.read() & B01111111); // sek
  this->minute = czas::bcdToDec(Wire.read() & B01111111); // min
  this->hour = czas::bcdToDec(Wire.read() & B00111111); //godz
  this->day = czas::bcdToDec(Wire.read() & B00111111); //dz msc
  this->dow = czas::bcdToDec(Wire.read() & B00000111); //dz tyg
  this->month = czas::bcdToDec(Wire.read() & B00011111); // msc
  this->year = czas::bcdToDec(Wire.read()); //rok
  this->year += 2000;
}

void czas::setRTC(uint32_t t) {
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
  Wire.write(czas::decToBcd(ss)); //sek
  Wire.write(czas::decToBcd(mm)); //min
  Wire.write(czas::decToBcd(hh)); //godz
  Wire.write(czas::decToBcd(d)); //dzien
  Wire.write(czas::decToBcd(dt)); //dzien tyg (1-7)
  Wire.write(czas::decToBcd(m)); //msc
  Wire.write(czas::decToBcd(r)); //rok-2000
  Wire.endTransmission();
}

#endif
