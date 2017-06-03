#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>
#include <pgmspace.h>
#define CONFIGFILE "PASS.PWD"
#define IPSETFILE "IP_SET.TXT"
#define DHCPFILE "DHCP.TXT"
#define SD_CS D4
#define READ_COND (tmp != '\r' && tmp != '\n' && tmp != 255 && tmp != '=')
#define READ_COND2 (tmp == '\r' && tmp == '\n' && tmp == 255 && tmp == '\t' && tmp =='\0')
File uploadFile;
bool dhcp = false;
bool gotip = false;
bool gotgate = false;
bool gotsub = false;
String sip = "";
String sgate = "";
String ssub = "";

void setup() {
  Serial.begin(115200);
  if (SD.begin(SD_CS))
    Serial.println("SD Card initialized.");
  else Serial.println("Brak Karty");

  if (wifiConn()) {
    Serial.println("TAK");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.subnetMask());
    if (dhcp) {
      if (SD.exists(DHCPFILE))
        SD.remove(DHCPFILE);
      uploadFile = SD.open(DHCPFILE, FILE_WRITE);
      uploadFile.println("mode=dhcp");
      uploadFile.print("ip="); uploadFile.println(WiFi.localIP());
      uploadFile.flush();
      uploadFile.print("gateway="); uploadFile.println(WiFi.gatewayIP());
      uploadFile.print("subnet="); uploadFile.println(WiFi.subnetMask());
      uploadFile.flush();
      uploadFile.close();
    }
  }
  else Serial.println("NIE");
}

IPAddress stringToIP(String input) {
  int parts[4] = {0, 0, 0, 0};
  int part = 0;
  for (int a = 0; a < input.length(); a++) {
    char b = input[a];
    if (b == '.') {
      part++;
      continue;
    }
    parts[part] *= 10;
    parts[part] += b - '0';
  }
  return IPAddress(parts[0], parts[1], parts[2], parts[3]);
}

void loop() {

}

bool wifiConn () {
  uploadFile = SD.open(IPSETFILE, FILE_READ);
  char tmp = uploadFile.read();
  String tmpS = "";
  while (!dhcp && (!gotip || !gotgate || !gotsub)) {
    while (READ_COND) {
      tmpS += tmp;
      tmp = uploadFile.read();
    }

    if (tmpS == "mode") {
      Serial.print("tryb: ");
      tmpS = "";
      tmp = uploadFile.read();
      while (READ_COND) {
        tmpS += tmp;
        tmp = uploadFile.read();
      }
      if (tmpS == "dhcp") {
        Serial.println("DHCP");
        dhcp = true;
      }
      if (tmpS == "static") {
        Serial.println("Statyczny");
        dhcp = false;
      }
    }

    if (tmpS == "ip") {
      Serial.print("ip: ");
      tmpS = "";
      tmp = uploadFile.read();
      while (READ_COND) {
        tmpS += tmp;
        tmp = uploadFile.read();
      }
      Serial.println(tmpS);
      sip = tmpS;
      gotip = true;
    }
    if (tmpS == "gateway") {
      Serial.print("gateway: ");
      tmpS = "";
      tmp = uploadFile.read();
      while (READ_COND) {
        tmpS += tmp;
        tmp = uploadFile.read();
      }
      Serial.println(tmpS);
      sgate = tmpS;
      gotgate = true;
    }
    if (tmpS == "subnet") {
      Serial.print("subnet: ");
      tmpS = "";
      tmp = uploadFile.read();
      while (READ_COND) {
        tmpS += tmp;
        tmp = uploadFile.read();
      }
      Serial.println(tmpS);
      ssub = tmpS;
      gotsub = true;
    }
    tmp = uploadFile.read();
    while (READ_COND2)tmp = uploadFile.read();
    tmpS = "";
  }
  uploadFile.close();
  sip.trim();
  sgate.trim();
  ssub.trim();

  tmp = 1;
  tmpS = "";

  uploadFile = SD.open("PASS.PWD", FILE_READ);
  tmp = uploadFile.read();
  tmpS = "";
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
    Serial.print("FORMAT:\n");
    Serial.print("ssid=[SSID]\n");
    Serial.print("pass=[PASSWORD]\n");
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
    Serial.print("FORMAT:\n");
    Serial.print("ssid=[SSID]\n");
    Serial.print("pass=[PASSWORD]\n");
    return false;
  }
  char *password = new char[tmpS.length() + 1];
  strcpy(password, tmpS.c_str());
  tmpS = "";
  uploadFile.close();

  //Serial.println(ssid);
  //Serial.println(password);

  if (!dhcp) WiFi.config(stringToIP(sip), stringToIP(sgate), stringToIP(ssub));
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  // Wait for connection
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {//wait 10 seconds
    delay(500);
  }
  if (i >= 21 && WiFi.status() != WL_CONNECTED)
    return false;
  return true;
}
