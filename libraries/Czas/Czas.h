#ifndef CZAS_H
#define CZAS_H
#include <pgmspace.h>
#include <Wire.h>
#define RTC_ADDR 0x51

#if (ARDUINO < 100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

/*
  I made this library to make interfacing with PCF8563 easier
*/

const uint8_t daysInMonth [] PROGMEM = { 31, 28, 31, 30, 31, 30,
                                         31, 31, 30, 31, 30, 31
                                       };

class Czas { //PCF8563
  public:
    Czas(byte sda_pin, byte scl_pin);
    ~Czas();
    int second();
    int minute();
    int hour();
    int day();
    int month();
    int year();
    int dow(); //Day of week
    void readRTC();
    void setRTC(uint32_t t);
    void setRTC(uint8_t ss, uint8_t mm, uint8_t hh, uint16_t d, uint8_t dt, uint8_t m, uint8_t r);
    uint32_t dateToEpoch(uint8_t ss, uint8_t mm, uint8_t hh, uint16_t d, uint8_t m, uint8_t r);
	uint32_t dateAsEpoch();
    uint32_t epochToDate(uint8_t ss, uint8_t mm, uint8_t hh, uint16_t d, uint8_t dt, uint8_t m, uint8_t r);
    bool compareTimeEpoch(uint32_t t, int tolerance); //I'm too lazy to convert date to epoch
  private:
    byte bcdToDec(byte value);
    byte decToBcd(byte value);
    int _second;
    int _minute;
    int _hour;
    int _day;
    int _month;
    int _year;
    int _dow; //Day of week
};

#endif
