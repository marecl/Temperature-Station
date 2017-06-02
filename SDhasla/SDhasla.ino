#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <SD.h>
#define DBG_OUTPUT_PORT Serial
#define READ_COND (tmp != '\r' && tmp != '\n' && tmp != 255 && tmp != '=')

const char* host = "SerwerTemperatury";
static bool hasSD = false;
File uploadFile;

void setup() {
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.setDebugOutput(false);
  DBG_OUTPUT_PORT.print("\n");

  if (SD.begin(2)) {
    DBG_OUTPUT_PORT.println("SD Card initialized.");
    hasSD = true;
    if (SD.exists("pass.txt")) {
      if (wifiConn()) {
        DBG_OUTPUT_PORT.print("Connected! IP address: ");
        DBG_OUTPUT_PORT.println(WiFi.localIP());
      } else {
        DBG_OUTPUT_PORT.print("Could not connect to WiFi!\n");
        DBG_OUTPUT_PORT.print("Check SSID and PASSWORD and try again\n");
        DBG_OUTPUT_PORT.print("Log mode only\n");
      }
    } else {
      DBG_OUTPUT_PORT.print("File pass.txt dosen't exist!\n");
      DBG_OUTPUT_PORT.print("Log mode only\n");
    }
  } else {
    DBG_OUTPUT_PORT.print("SDcard not inserted!\n");
    DBG_OUTPUT_PORT.print("I'm supposed to be server, not paperweight!\n");
    DBG_OUTPUT_PORT.print("Need SDcard.\n");
    DBG_OUTPUT_PORT.print("Rebooting...\n");
    delay(1000);
    while (1) {}
  }
}

void loop() {
  DBG_OUTPUT_PORT.println(millis());
  delay(10000);
}

bool wifiConn () {
  uploadFile = SD.open("pass.txt", FILE_READ);
  char tmp = uploadFile.read();
  String tmpS = "";
  while (READ_COND) {
    tmpS += tmp;
    tmp = uploadFile.read();
  }
  if (tmpS == "ssid") {
    tmpS = "";
    tmp = uploadFile.read();
    while (READ_COND) {
      tmpS += tmp;
      tmp = uploadFile.read();
    }
  } else {
    DBG_OUTPUT_PORT.print("FORMAT:\n");
    DBG_OUTPUT_PORT.print("ssid=[SSID]\n");
    DBG_OUTPUT_PORT.print("pass=[PASSWORD]\n");
    return false;
  }
  char *ssid = new char[tmpS.length() + 1];
  strcpy(ssid, tmpS.c_str());
  tmpS = "";

  tmp = uploadFile.read();
  while (tmp == '\r' || tmp == '\n' || tmp == '\t' || tmp == '=' )
    tmp = uploadFile.read();
  while (READ_COND) {
    tmpS += tmp;
    tmp = uploadFile.read();
  }
  if (tmpS == "pass") {
    tmpS = "";
    tmp = uploadFile.read();
    while (READ_COND) {
      tmpS += tmp;
      tmp = uploadFile.read();
    }
  } else {
    DBG_OUTPUT_PORT.print("FORMAT:\n");
    DBG_OUTPUT_PORT.print("ssid=[SSID]\n");
    DBG_OUTPUT_PORT.print("pass=[PASSWORD]\n");
    return false;
  }
  char *password = new char[tmpS.length() + 1];
  strcpy(password, tmpS.c_str());
  tmpS = "";
  uploadFile.close();

  //DBG_OUTPUT_PORT.println(ssid);
  //DBG_OUTPUT_PORT.println(password);

  WiFi.begin(ssid, password);
  DBG_OUTPUT_PORT.print("Connecting to ");
  DBG_OUTPUT_PORT.println(ssid);
  // Wait for connection
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {//wait 10 seconds
    delay(500);
  }
  if (i >= 21 && WiFi.status() != WL_CONNECTED)
    return false;
  return true;
}
