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
    Czas(TwoWire&);
    ~Czas();
    int second();
    int minute();
    int hour();
    int day();
    int month();
    int year();
    int dow(); //Day of week
    void readRTC();
    void setRTC(uint32_t);
    void setRTC(uint8_t, uint8_t, uint8_t, uint16_t, uint8_t, uint8_t, uint16_t);
    uint32_t dateToEpoch(uint8_t, uint8_t, uint8_t, uint16_t, uint8_t, uint8_t);
    uint32_t dateAsEpoch();
    uint32_t epochToDate(uint8_t, uint8_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);
    bool compareTimeEpoch(uint32_t, int); //I'm too lazy to convert date to epoch
  private:
    TwoWire *_Wire;
    byte bcdToDec(byte);
    byte decToBcd(byte);
    int _second;
    int _minute;
    int _hour;
    int _day;
    int _month;
    int _year;
    int _dow; //Day of week
};
#endif