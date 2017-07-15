#ifndef CZAS_H
#define CZAS_H
#include <pgmspace.h>
#include <Wire.h>
#define RTC_ADDR 0x51

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

/*
  I made this library to make interfacing with PCF8563 easier

  TODO:
  - Access to variables ONLY via functions
  - Make proper destructor to free memory when work is done
  - Make daylight saving automatic and move dls to private
  - Return actual date and time as epoch (for time comparsion)

*/

const uint8_t daysInMonth [] PROGMEM = { 31, 28, 31, 30, 31, 30,
                                         31, 31, 30, 31, 30, 31
                                       };
                                       
class Czas { //PCF8563
  public:
    Czas(byte sda_pin, byte scl_pin);
    ~Czas();
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

#endif
