#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include <SD.h>

const char* dayList[7] = {
  "Mon", "Tue", "Wed", "Thu",
  "Fri", "Sat", "Sun"
};
const char* monthList[12] = {
  "Jan", "Feb", "Mar", "Apr",
  "May", "Jun", "Jul", "Aug",
  "Sep", "Oct", "Nov", "Dec"
};

class CommonUtils {
  public:
    /* Cookie expiration date */
    static String printCookieDate(uint32_t time, settingsManager* set) {
      RtcDateTime _n = (time + set->tokenLifespan);
      String ret = dayList[_n.DayOfWeek() - 1];
      ret += ", ";
      if (_n.Day() < 10) ret += '0';
      ret += String(_n.Day()) + ' ';
      ret += String(monthList[_n.Month() - 1]) + ' ';
      ret += String(_n.Year()) + ' ';
      if (_n.Hour() < 10) ret += '0';
      ret += String(_n.Hour()) + ':';
      if (_n.Minute() < 10) ret += '0';
      ret += String(_n.Minute()) + ':';
      if (_n.Second() < 10) ret += '0';
      ret += String(_n.Second()) + " GMT";
      return ret;
    }

    static uint32_t globalTimer;
    static settingsManager* setptr;

    static void returnFail(ESP8266WebServer *_s, String _m) {
      _s->send(500, F("text/plain"), _m + PSTR("\r\n"));
      return void();
    }

    static void returnLoginFail(ESP8266WebServer *_s) {
      _s->send(401, F("text/plain"), F("Incorrect login or password"));
      return void();
    }

    static void returnOK(ESP8266WebServer *_s, const __FlashStringHelper* _m) {
      _s->send(200, F("text/plain"), _m);
      return void();
    }

    static void saveJson(JsonArray& toSave, const char* _f) {
      if (!toSave.success()) return;
      if (SD.exists(_f)) SD.remove(_f);
      File root = SD.open(_f, FILE_WRITE);
      toSave.printTo(root);
      root.flush();
      root.close();
      return void();
    }

    static double getTemp(DallasTemperature *sen, JsonArray &addrset) {
      byte tempaddr[8];
      for (int b = 0; b < 8; b++)
        tempaddr[b] = addrset[b];
      return sen->getTempC(tempaddr);
    }

    static bool isMember(byte _1[], JsonArray& compArr) {
      for (uint8_t a = 0; a < compArr.size(); a++) {
        for (uint8_t b = 0; b < 8; b++) {
          uint8_t _2 = compArr[a][1][b];
          if (_1[b] != _2)
            break;
          else if (b == 7) return true;
        }
      }
      return false;
    }

    static void refreshSensors(OneWire *_ow, const char* _n) {
      if (true) {
        DynamicJsonBuffer jsonBuffer(1000);
        File root;
        if (!SD.exists(_n)) {
          Serial.println("no file yo");
          root = SD.open(_n, FILE_WRITE);
          root.print(F("[]"));
          root.flush();
          root.close();
        }

        root = SD.open(_n, FILE_READ);
        JsonArray& nSet = jsonBuffer.parseArray(root);
        root.close();

        bool _ch = false;
        byte _a[8];

        while (_ow->search(_a)) {
          if (CommonUtils::isMember(_a, nSet) == false) {
            if (OneWire::crc8(_a, 7) == _a[7]) {
              _ch = true;
              JsonArray& _e = nSet.createNestedArray();
              _e.add("New_" + String(nSet.size()));
              JsonArray& _r = _e.createNestedArray();
              for (uint8_t x = 0; x < 8; x++)
                _r.add(_a[x]);
            }
          }
        }
        _ow->reset_search();

        if (_ch) CommonUtils::saveJson(nSet, _n);
      }
      return;
    }

    static char* printDateTime(char* input, const RtcDateTime *dt) {
      snprintf_P(input, 20,
                 PSTR("%02u/%02u/%04u;%02u:%02u"),
                 dt->Day(),
                 dt->Month(),
                 dt->Year(),
                 dt->Hour(),
                 dt->Minute());
      return input;
    }
};
#endif
