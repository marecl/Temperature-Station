#ifndef SENSORS_PAGE_H
#define SENSORS_PAGE_H

#include "CommonUtils.hpp"
#include "LoginPageHandler.hpp"

class SensorsPageHandler : public RequestHandler {
  protected:
    String _uri;
    DallasTemperature *_sen;
    const char* _filename;

  public:
    SensorsPageHandler(const char* uri, DallasTemperature *sen, const char* f)
      : _uri(uri), _sen(sen), _filename(f) {
    }

    bool canHandle(HTTPMethod requestMethod, String requestUri) override  {
      if (requestMethod != HTTP_GET || requestUri != _uri)
        return false;
      return true;
    }

    bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) override {
      if (!LoginPageHandler::verifyLogin(&server, CommonUtils::setptr)) {
        LoginPageHandler::redirectToLogin(&server);
        return true;
      }

      _sen->requestTemperatures();

      DynamicJsonBuffer sensBuff(1000);
      File tmpSens = SD.open(_filename , FILE_READ);
      JsonArray& sensSet = sensBuff.parseArray(tmpSens);
      tmpSens.close();

      if (!sensSet.success())
        return false;

      server.sendContent(F("<!DOCTYPE html><html><head><title>"));
      server.sendContent(F("Sensors</title><style>table, th, td{border: 1px solid "));
      server.sendContent(F("black;text-align: center}</style></head><body>"));
      server.sendContent(F("<a href=\"/\"><input type=\"button\" value=\"Back\""));
      server.sendContent(F(" /></a><form id=\"saved\" method=\"POST\" action=\"/editsaved\""));
      server.sendContent(F(" onsubmit=\"var a=document.forms.saved.elements;"));
      server.sendContent(F("for(var t=0;t<a.length-1;t+=2)a[t].disabled=(a[t+1]"));
      server.sendContent(F(".checked||a[t].value==''||a[t].placeholder==a[t]."));
      server.sendContent(F("value);\"><table><caption><b>Name Settings</b>"));
      server.sendContent(F("</caption><tr><th>Name</th><th>Address</th>"));
      server.sendContent(F("<th>Temperature [C]</th><th>Remove</th></tr>"));

      for (uint8_t a = 0; a < sensSet.size(); a++) {
        JsonArray& _e = sensSet[a];
        double _t = CommonUtils::getTemp(_sen, _e[1]);
        server.sendContent(F("<tr><td><input type='text' name='"));
        server.sendContent(String(a)); //Position in file
        server.sendContent(F("' placeholder='"));
        server.sendContent(_e[0].as<const char*>());
        server.sendContent(F("'><td>"));
        server.sendContent(_e[1]);
        server.sendContent(F("</td><td>"));
        server.sendContent(_t == -127.00 ? "N/C" : String(_t));
        server.sendContent(F("</td><td><input type='checkbox' name='r' value='"));
        server.sendContent(String(a));
        server.sendContent(F("'>Remove</td></tr>"));
      }

      server.sendContent(F("</table><button type=\"submit\">Save Changes"));
      server.sendContent(F("</button><a href=\"sensors.txt\"><input type"));
      server.sendContent(F("=\"button\" value=\"Backup config\" />"));
      server.sendContent(F("</a></form></body></html>"));

      return true;
    }
};

#endif
