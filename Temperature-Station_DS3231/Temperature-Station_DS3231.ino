#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <pgmspace.h>
#include "defs.h"
/*
  Zrobic podglad ostatnich 24h w pliku domyslnie otwartym
  Zrobic auto ustawianie czasu letniego

  Zrobic druga wersje plytki i port programu

  Dodac LEDa jakby byl blad (np. zly czas)
*/
RtcDS3231<TwoWire> Rtc(Wire);

IPAddress timeServerIP;
WiFiUDP udp;

ESP8266WebServer server(80);


RtcDateTime teraz;
File uploadFile;
File root;
OneWire oneWire(OW_PORT);
DallasTemperature sensors(&oneWire);

void setup(void) {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.print("\n");
  sensors.begin();
  Rtc.Begin();
  Wire.begin(SDA, SCL);

  if (SD.begin(SD_CS)) {
    Serial.println("SD Card initialized.");
    hasSD = true;
  } else {
    Serial.print("SDcard not inserted!\n");
    Serial.print("I'm supposed to be server, not paperweight!\n");
    Serial.print("Need SDcard.\n");
    Serial.print("Rebooting...\n");
    delay(1000);
    ESP.restart();
  }

  if (SD.exists(CONFIGFILE)) {
    if (wifiConn())
      httpserver = true;
    else {
      Serial.print("Could not connect to WiFi!\n");
      Serial.print("Check SSID and PASSWORD and try again\n");
      Serial.print("Log mode only\n");
      httpserver = false;
    }
  } else {
    Serial.print("File pass.txt dosen't exist!\n");
    Serial.print("Log mode only\n");
    httpserver = false;
  }

  if (httpserver) {
    server.on("/list", HTTP_GET, printDirectory);
    server.on("/edit", HTTP_DELETE, handleDelete);
    server.on("/edit", HTTP_PUT, handleCreate);
    server.on("/edit", HTTP_POST, []() {
      returnOK();
    }, handleFileUpload);
    server.on("/pass.txt", returnForbidden);
    server.onNotFound(handleNotFound);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");

    delay(2500);
    udp.begin(localPort);
    WiFi.hostByName(ntpServerName, timeServerIP);
    sendNTPpacket(timeServerIP);
    delay(1000);
    int cb = udp.parsePacket();
    if (!cb)
      Serial.println("no packet yet");
    else {
      Serial.print("packet received, length=");
      Serial.println(cb);
      udp.read(packetBuffer, NTP_PACKET_SIZE);
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      const unsigned long seventyYears = 2208988800UL;
      unsigned long epoch = secsSince1900 - seventyYears - 946684800;
      epoch += 3600; //UTC +1
      if (letni) epoch += 3600; //Czas letni
      Serial.println("Czas NTP od 1/1/2000: " + (String)epoch);
      Serial.print("Czas zegara: ");
      teraz = Rtc.GetDateTime();
      uint32_t tempseconds = teraz.TotalSeconds();
      Serial.println(tempseconds);
      if (abs(epoch - tempseconds) > 59 || !Rtc.IsDateTimeValid()) {
        Serial.println("Zegar zle chodzi");
        Rtc.SetDateTime(0); //Reset do 1/1/2000
        Rtc.SetDateTime(epoch); //Prawidlowy czas
        if (!Rtc.IsDateTimeValid()) Rtc.SetDateTime(epoch); //Jak zle zapisal to jeszcze raz
      } else Serial.println("Zegar chodzi dobrze");
    }
  }
  teraz = Rtc.GetDateTime();
  createfile();
  Serial.println(printDateTime(teraz));

}

void loop() {
  if (teraz.Hour() == 23 && teraz.Minute() <= 59 && teraz.Minute() >= 55) {
    teraz = Rtc.GetDateTime();
    //ESP.restart();
  }
  while (teraz.Minute() % 5 != 0) {
    teraz = Rtc.GetDateTime();
    if (httpserver)
      server.handleClient();
  }
  sensors.requestTemperatures();
  _t1_ = round(10 * sensors.getTempC(_temp1_));
  _t1_ /= 10;
  _t2_ = round(10 * sensors.getTempC(_temp2_));
  _t2_ /= 10;
  _t3_ = round(10 * sensors.getTempC(_temp3_));
  _t3_ /= 10;
  _t4_ = round(10 * sensors.getTempC(_temp4_));
  _t4_ /= 10;

  if (hasSD) {
    updatetime();
    root = SD.open(workfile, FILE_WRITE);
    delay(150);
    if (root) {
      teraz = Rtc.GetDateTime();
      root.print(printDateTime(teraz) + ";");
      root.flush();
      root.print(_t1_, 1);
      root.print(";");
      root.print(_t2_, 1);
      root.print(";");
      root.print(_t3_, 1);
      root.print(";");
      root.print(_t4_, 1);
      root.print("\r\n");
      root.flush();
      root.close();
      Serial.print("Temperatura (");
      Serial.print(printDateTime(teraz));
      Serial.print("):\n\tWewnatrz: ");
      Serial.println(_t1_);
      Serial.print("\tNa zewnatrz: ");
      Serial.println(_t2_);
      Serial.print("\tWyjscie pieca: ");
      Serial.println(_t3_);
      Serial.print("\tPowrot pieca: ");
      Serial.println(_t4_);
    } else {
      hasSD = false; Serial.println("!!! BRAK KARTY !!!");
    }
  }
  while (teraz.Minute() % 5 == 0) {
    teraz = Rtc.GetDateTime();
    //if (teraz.Minute() == 165 && teraz.Day() == 165) ESP.restart();
    if (httpserver)
      server.handleClient();
  }
}

void createfile() {
  String path = "archiwum/" + (String)teraz.Year();
  if (teraz.Month() < 10) path += "0" + (String)teraz.Month();
  else path += (String)teraz.Month();
  char* path2 = new char[path.length() + 1];
  strcpy(path2, path.c_str());
  SD.mkdir(path2);

  path += "/";
  if (teraz.Day() < 10) path += "0" + (String)teraz.Day() + ".csv";
  else path += (String)teraz.Day() + ".csv";
  path2 = new char[path.length() + 1];
  strcpy(path2, path.c_str());
  File dest = SD.open(path2, FILE_WRITE);
  if (dest.size() < 10) {
    dest.print("Data;Godzina;Wewnatrz;");
    dest.flush();
    dest.print("Zewnatrz;PiecWyjscie;PiecPowrot\n");
    dest.flush();
    dest.close();
  }
  workfile = path;
}

void updatetime() {
  sendNTPpacket(timeServerIP);
  delay(1000);
  int cb = udp.parsePacket();
  if (cb) {
    Serial.println(cb);
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears - 946684800;
    epoch += 3600; //UTC +1
    if (letni) epoch += 3600;
    teraz = Rtc.GetDateTime();
    uint32_t tempseconds = teraz.TotalSeconds();
    if (abs(epoch - tempseconds) > 59 || !Rtc.IsDateTimeValid()) {
      Rtc.SetDateTime(0); //Reset do 1/1/2000
      Rtc.SetDateTime(epoch); //Prawidlowy czas
      if (!Rtc.IsDateTimeValid()) Rtc.SetDateTime(epoch); //Jak zle zapisal to jeszcze raz
    }
  }
}

bool wifiConn () {
  File root = SD.open(IPSETFILE, FILE_READ);
  char tmp = root.read();
  String tmpS = "";
  while (!dhcp && (!gotip || !gotgate || !gotsub)) {
    while (READ_COND) {
      tmpS += tmp;
      tmp = root.read();
    }

    if (tmpS == "mode") {
      tmpS = "";
      tmp = root.read();
      while (READ_COND) {
        tmpS += tmp;
        tmp = root.read();
      }
      if (tmpS == "dhcp")dhcp = true;
      if (tmpS == "static")dhcp = false;
    }

    if (tmpS == "ip") {
      tmpS = "";
      tmp = root.read();
      while (READ_COND) {
        tmpS += tmp;
        tmp = root.read();
      }
      sip = tmpS;
      gotip = true;
    }
    if (tmpS == "gateway") {
      tmpS = "";
      tmp = root.read();
      while (READ_COND) {
        tmpS += tmp;
        tmp = root.read();
      }
      sgate = tmpS;
      gotgate = true;
    }
    if (tmpS == "subnet") {
      tmpS = "";
      tmp = root.read();
      while (READ_COND) {
        tmpS += tmp;
        tmp = root.read();
      }
      ssub = tmpS;
      gotsub = true;
    }
    tmp = root.read();
    while (READ_COND2)tmp = root.read();
    tmpS = "";
  }
  root.close();
  sip.trim();
  sgate.trim();
  ssub.trim();

  tmp = 1;
  tmpS = "";

  root = SD.open("PASS.PWD", FILE_READ);
  tmp = root.read();
  tmpS = "";
  while (READ_COND) {
    tmpS += tmp;
    tmp = root.read();
  }
  if (tmpS == "ssid") {
    tmpS = "";
    tmp = root.read();
    while (READ_COND) {
      tmpS += tmp;
      tmp = root.read();
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

  tmp = root.read();
  while (tmp == '\r' || tmp == '\n' || tmp == '\t' || tmp == '=' )
    tmp = root.read();
  while (READ_COND) {
    tmpS += tmp;
    tmp = root.read();
  }
  if (tmpS == "pass") {
    tmpS = "";
    tmp = root.read();
    while (READ_COND) {
      tmpS += tmp;
      tmp = root.read();
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
  root.close();

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
  Serial.print("Connected! \nIP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Subnet mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("IP obtain mode: ");
  if (dhcp) Serial.println("DHCP");
  else Serial.println("Static");
  if (dhcp) {
    if (SD.exists(DHCPFILE)) SD.remove(DHCPFILE); //Zrzut adresu IP
    root = SD.open(DHCPFILE, FILE_WRITE);
    root.println("mode=dhcp");
    root.print("ip="); root.println(WiFi.localIP());
    root.flush();
    root.print("gateway="); root.println(WiFi.gatewayIP());
    root.print("subnet="); root.println(WiFi.subnetMask());
    root.flush();
    root.close();
  }
  return true;
}

unsigned long sendNTPpacket(IPAddress & address) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void printDirectory() {
  if (!server.hasArg("dir")) return returnFail("BAD ARGS");
  String path = server.arg("dir");
  if (path != "/" && !SD.exists((char *)path.c_str())) return returnFail("BAD PATH");
  File dir = SD.open((char *)path.c_str());
  path = String();
  if (!dir.isDirectory()) {
    dir.close();
    return returnFail("NOT DIR");
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/json", "");
  WiFiClient client = server.client();

  server.sendContent("[");
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    if (!entry)
      break;

    String output;
    if (cnt > 0)
      output = ',';

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += entry.name();
    output += "\"";
    output += "}";
    server.sendContent(output);
    entry.close();
  }
  server.sendContent("]");
  dir.close();
}

void handleNotFound() {
  if (hasSD && loadFromSdCard(server.uri())) return;
  String message = "PLIKU NIE ZNALEZIONO\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.print(message);
}

void handleDelete() {
  if (server.args() == 0) return returnFail("BAD ARGS");
  String path = server.arg(0);
  if (path == "/" || !SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }
  deleteRecursive(path);
  returnOK();
}

void handleCreate() {
  if (server.args() == 0) return returnFail("BAD ARGS");
  String path = server.arg(0);
  if (path == "/" || SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }

  if (path.indexOf('.') > 0) {
    File file = SD.open((char *)path.c_str(), FILE_WRITE);
    if (file) {
      file.write((const char *)0);
      file.close();
    }
  } else {
    SD.mkdir((char *)path.c_str());
  }
  returnOK();
}

void returnOK() {
  server.send(200, "text/plain", "");
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

void returnForbidden() {
  String output = "<html>\r\n";
  output += "<h1>403 FORBIDDEN</h1>\r\n";
  output += "</html>";
  server.send(403, "text/html", output);
}

bool loadFromSdCard(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";

  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  else if (path.endsWith(".png")) dataType = "image/png";
  else if (path.endsWith(".gif")) dataType = "image/gif";
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
  else if (path.endsWith(".ico")) dataType = "image/x-icon";
  else if (path.endsWith(".xml")) dataType = "text/xml";
  else if (path.endsWith(".pdf")) dataType = "application/pdf";
  else if (path.endsWith(".zip")) dataType = "application/zip";
  else if (path.endsWith(".csv")) dataType = "text/txt";

  File dataFile = SD.open(path.c_str());
  if (dataFile.isDirectory()) {
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open(path.c_str());
  }

  if (!dataFile)
    return false;

  if (server.hasArg("download")) dataType = "application/octet-stream";

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    Serial.println("Sent less data than expected!");
  }

  dataFile.close();
  return true;
}

void handleFileUpload() {
  if (server.uri() != "/edit") return;
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    if (SD.exists((char *)upload.filename.c_str())) SD.remove((char *)upload.filename.c_str());
    uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    Serial.print("Upload: START, filename: "); Serial.println(upload.filename);
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
    Serial.print("Upload: WRITE, Bytes: "); Serial.println(upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) uploadFile.close();
    Serial.print("Upload: END, Size: "); Serial.println(upload.totalSize);
  }
}
