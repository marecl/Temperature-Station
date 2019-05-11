#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include "CommonUtils.hpp"

class LoginPageHandler : public RequestHandler {
  protected:
    String _uri;

  public:
    LoginPageHandler(const char* uri)
      : _uri(uri) {
    }

    bool canHandle(HTTPMethod requestMethod, String requestUri) override {
      if (requestUri == _uri)
        return true;
      return false;
    }

    bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) override {
      if (requestMethod == HTTP_GET) {
        String content = F("<html><body><form action='/login' method='POST'>");
        content += PSTR("User:<input type='text' name='USERNAME' placeholder='username'><br>");
        content += PSTR("Password:<input type='password' name='PASSWORD' placeholder='password'><br>");
        content += PSTR("<input type='submit' name='SUBMIT' value='Submit'></form><br>");
        content += PSTR("</body></html>");
        server.send(200, "text/html", content);
      } else if (requestMethod == HTTP_POST) {
        if (!verifyLogin(&server, CommonUtils::setptr)) {
          redirectToLogin(&server);
          return true;
        }
        if (requestUri == _uri)
          server.sendHeader("Location", "/");
        else server.sendHeader("Location", requestUri);
        server.sendHeader("Cache-Control", "no-cache");
        server.send(301);
      }
      return true;
    }

    static bool verifyLogin(ESP8266WebServer* ser, settingsManager* set) {
      String toVerify = ser->header("Cookie");
      uint8_t status = set->verifyEncryptedKey(toVerify, CommonUtils::globalTimer);
      Serial.print("Cookie verification status: ");
      Serial.println(status);
      /* No cookie or expired */
      if (status >= 5) {
        if (!ser->hasArg("USERNAME") || !ser->hasArg("PASSWORD"))
          return false;
        if (!set->authenticate(ser->arg("USERNAME").c_str(), ser->arg("PASSWORD").c_str()))
          return false;
      } else if (status > 0)
        return false;
      /* Codes 1-4 are invalid */
      ser->sendHeader("Set-Cookie", set->encryptKey(CommonUtils::globalTimer) +
                      "; Expires=" + CommonUtils::printCookieDate(CommonUtils::globalTimer, CommonUtils::setptr));
      return true;
    }

    static void redirectToLogin(ESP8266WebServer* ser) {
      ser->sendHeader("Location", "/login");
      ser->sendHeader("Cache-Control", "no-cache");
      ser->send(301);
      return void();
    }
};

#endif
