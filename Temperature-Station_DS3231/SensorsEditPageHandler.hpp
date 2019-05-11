#ifndef SENSORS_EDIT_PAGE_H
#define SENSORS_EDIT_PAGE_H

#include "CommonUtils.hpp"

class SensorsEditPageHandler : public RequestHandler {
  protected:
    String _uri;
    const char* _filename;

  public:
    SensorsEditPageHandler(const char* uri, const char* f)
      : _uri(uri), _filename(f) {
    }

    bool canHandle(HTTPMethod requestMethod, String requestUri) override  {
      if (requestMethod != HTTP_POST || requestUri != _uri)
        return false;
      return true;
    }

    bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) override {
      if (!LoginPageHandler::verifyLogin(&server, CommonUtils::setptr)) {
        LoginPageHandler::redirectToLogin(&server);
        return true;
      }

      DynamicJsonBuffer sensBuff(1000);
      File tmpSens = SD.open(_filename , FILE_READ);
      JsonArray& sensSet = sensBuff.parseArray(tmpSens);
      tmpSens.close();

      if (!sensSet.success())
        return false;

      for (int x = server.args() - 2; x >= 0; x--) {
        if (server.argName(x) == "r")
          sensSet.remove((atoi((server.arg(x)).c_str())));
        else {
          sensSet[atoi((server.argName(x)).c_str())][0] = server.arg(x);
        }

      }
      CommonUtils::saveJson(sensSet, _filename);

      server.sendContent(F("HTTP/1.1 303 See Other\r\nLocation:/sensors\r\n"));
      return true;
    }
};

#endif
