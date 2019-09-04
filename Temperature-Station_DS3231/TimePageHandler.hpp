/*#ifndef SENSORS_EDITPAGE_H
  #define SENSORS_EDITPAGE_H

  #include "CommonUtils.hpp"

  class TimePageHandler : public RequestHandler {
  protected:
    String _uri;
    uint32_t _epoch;
    settingsManager *_set;

  public:
    TimePageHandler(const char* uri, settingsManager *s)
      : _uri(uri), _set(s) {
    }

    bool canHandle(HTTPMethod requestMethod, String requestUri) override  {
      if (requestMethod != HTTP_POST || requestUri != _uri)
        return false;
      return false;
    }

    bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) override {
      if (!LoginPageHandler::verifyLogin(&server, CommonUtils::setptr)) {
        LoginPageHandler::redirectToLogin(&server);
        return true;
      }
  RtcDateTime _now = CommonUtils::globalTimer;
      if (server.arg("a") == "l") {
        char _d[20];
        snprintf_P(_d, 20,
                   PSTR("%04u-%02u-%02uT%02u:%02u"),
                   _now.Year(),
                   _now.Month(),
                   _now.Day(),
                   _now.Hour(),
                   _now.Minute());
        server.send(200, F("text/plain"), _d);
      } else if (server.arg("a") == "s" && _set->authenticate(server.arg("SL").c_str(), server.arg("SPL").c_str())) {
        if (server.hasArg("TZ"))
          _set->timezone = atoi(server.arg("TZ").c_str());
        if (server.hasArg("UN"))
          _set->useNTP = (server.arg("UN")[0] == '1');
        if (server.hasArg("NS"))
          _set->ntpServer(server.arg("NS").c_str());
        if (server.hasArg("EP")) {
          uint32_t newTime = strtoul(server.arg("EP").c_str(), NULL, 10);
          newTime += 3600 * _set->timezone;
          newTime -= 946684800;
          _epoch = newTime;
          //zegar.SetDateTime(RtcDateTime(newTime));
          _set->lastUpdate = newTime;
        }
        _set->save();
        CommonUtils::returnOK(F("Time settings saved"));
      } else CommonUtils::returnLoginFail(&server);
      return true;
    }

  };

  #endif*/
