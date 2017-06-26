#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <WiFiUdp.h>
#include "Czas.h"
#include "defs.h"
/*
  Read JSON directly from SDcard

  Create PC executable to create JSON file
    (pc asks to connect one sensor, reads address and gives name
    then asks for ip settings)
*/

IPAddress timeServerIP;
WiFiUDP udp;

ESP8266WebServer server(80);

File uploadFile;
OneWire oneWire(OW_PORT);
DallasTemperature sensors(&oneWire);

DynamicJsonBuffer jsonBuffer(750);

czas zegar(1, SDA, SCL); //PCF8563 address, timezone

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.print("\n");
  sensors.begin();

  if (!digitalRead(SD_D)) {
    if (SD.begin(SD_CS)) {
      Serial.println("SD Card initialized.");
      hasSD = true;
      if (!SD.exists(SETTINGS_FILE)) {
        Serial.println("No settings file. Unable to connect to WiFi or log temperature!");
        Serial.println("Rebooting and waiting for valid SETTINGS.TXT file...");
        delay(1000);
        ESP.restart();
      }
    }
    else {
      Serial.println("SD Card present but cannot be initialized!");
      Serial.println("No card - no data logging!");
      hasSD = false;
    }
  } else if (!hasSD) {
    Serial.print("I'm supposed to be server, not paperweight!\n");
    Serial.print("Need SDcard.\n");
    Serial.print("Rebooting...\n");
    delay(1000);
    ESP.restart();
  }

  File root = SD.open(SETTINGS_FILE , FILE_READ);
  int tmp = root.read();
  String json = "";
  while (tmp != -1) {
    if (tmp != 10 && tmp != 13 && tmp != 32 && tmp != 9)
      json += (char)tmp;
    tmp = root.read();
  }
  root.close();

  JsonObject& settings = jsonBuffer.parseObject(json);
  if (!settings.success()) {
    Serial.println("Invalid JSON file");
    Serial.println("Come back with valid one. Rebooting...");
    delay(1000);
    ESP.restart();
  }

  if (wifiConn(settings))
    httpserver = true;
  else {
    Serial.print("Could not connect to WiFi!\n");
    Serial.print("Check SSID and PASSWORD and try again\n");
    Serial.print("Maybe static IP settings are incorrect\n");
    Serial.print("Log mode only\n");
    httpserver = false;
  }

  if (httpserver) {
    server.on("/list", HTTP_GET, printDirectory);
    server.on("/", HTTP_DELETE, handleDelete);
    server.on("/", HTTP_PUT, handleCreate);
    server.on("/", HTTP_POST, []() {
      returnOK();
    }, handleFileUpload);
    server.on("/pass.pwd", HTTP_GET, returnForbidden);
    server.on("/pass.pwd", HTTP_POST, returnForbidden);
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
      Serial.println("Received invalid NTP data");
    else {
      Serial.println("Received valid NTP data");
      udp.read(packetBuffer, NTP_PACKET_SIZE);
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      unsigned long epoch = secsSince1900 - 2208988800UL;
      zegar.setRTC(epoch);
    }
  }
  zegar.readRTC();
  Serial.println(printDateTime());
  //createtemplate();

  valid_sensors = settings["valid_sensors"];
  Serial.println("\r\nSensor list:");
  for (int a = 0; a < valid_sensors; a++) {
    Serial.print((String)(a + 1) + "/" + (String)MAX_SENSORS + ": ");
    String xdtmp = settings["sensor"][a][0];
    Serial.print(xdtmp);
    Serial.print(" (");
    for (int x = 0; x < 8; x++) {
      short tee = settings["sensor"][a][x + 1];
      if (tee < 10) Serial.print("0");
      Serial.print(tee, HEX);
    }
    Serial.print(")\n");
    getTemp(settings, a);
  }
  createfile(settings);
}

void loop() {
  File setfile = SD.open(SETTINGS_FILE , FILE_READ);
  int tmp = setfile.read();
  String json = "";
  while (tmp != -1) {
    if (tmp != 10 && tmp != 13 && tmp != 32 && tmp != 9)
      json += (char)tmp;
    tmp = setfile.read();
  }
  setfile.close();
  JsonObject& settings = jsonBuffer.parseObject(json);
  //Okay, JSON loaded so now we can start loop
  //Uhnfortunately I can't pass object from setup()

  while (1) {
    if (zegar.hour == 23 && zegar.minute <= 59 && zegar.second >= 55) {
      zegar.readRTC();
      //ESP.restart();
    }
    while (zegar.minute % 5 != 0) {
      zegar.readRTC();
      if (httpserver)
        server.handleClient();
      if (digitalRead(SD_D) && hasSD) {
        hasSD = false;
        httpserver = false;
        Serial.println("!!! NO SDCARD !!!");
      }
      if (!hasSD) {
        while (digitalRead(SD_D)) delay(1000);
        Serial.println("SD Card found. Reboot...");
        ESP.restart();
      }
    }
    sensors.requestTemperatures();
    for (int c = 0; c < valid_sensors; c++) {
      _temps_[c] = getTemp(settings, c);
      _temps_[c] = round(10 * _temps_[c]);
      _temps_[c] /= 10;
    }
    if (hasSD) {
      delay(150);
      if (hasSD && !digitalRead(SD_D)) {
        File root = SD.open(workfile, FILE_WRITE);
        zegar.readRTC();
        if (zegar.hour == 0 && zegar.minute == 0) createfile(settings);
        root.print(printDateTime() + ";");
        root.flush();
        for (int c = 0; c < valid_sensors; c++) {
          root.print(_temps_[c], 1);
          if (valid_sensors - c > 1) root.print(";");
          else root.print("\r\n");
          root.flush();
        }
        root.flush();
        root.close();
        Serial.println(printDateTime() + ":");
        for (int c = 0; c < valid_sensors; c++) {
          String tempname = settings["sensor"][c][0];
          Serial.print("\t" + tempname + ": ");
          Serial.println(_temps_[c]);
        }
      } else {
        hasSD = false;
        httpserver = false;
        Serial.println("!!! NO SDCARD !!!");
      }
    }
    while (zegar.minute % 5 == 0) {
      zegar.readRTC();
      if (httpserver)
        server.handleClient();
      if (digitalRead(SD_D) && hasSD) {
        hasSD = false;
        httpserver = false;
        Serial.println("!!! NO SDCARD !!!");
      }
      if (!hasSD) {
        while (digitalRead(SD_D)) delay(1000);
        Serial.println("SD Card found. Reboot...");
        ESP.restart();
      }
    }
  }
}

void createfile(JsonObject &nameobj) {
  String path = "archiwum/" + (String)zegar.year;
  if (zegar.month < 10) path += "0" + (String)zegar.month;
  else path += (String)zegar.month;
  char* path2 = new char[path.length() + 1];
  strcpy(path2, path.c_str());
  SD.mkdir(path2);

  path += "/";
  if (zegar.day < 10) path += "0" + (String)zegar.day + ".csv";
  else path += (String)zegar.day + ".csv";
  path2 = new char[path.length() + 1];
  strcpy(path2, path.c_str());
  if (!SD.exists(path2)) {
    File dest = SD.open(path2, FILE_WRITE);

    dest.print("Date;Time;");
    dest.flush();
    for (int c = 0; c < valid_sensors; c++) {
      String tempname = nameobj["sensor"][c][0];
      dest.print(tempname);
      if (valid_sensors - c > 1) dest.print(";");
      else dest.print("\r\n");
      dest.flush();
    }
    dest.close();
  }
  workfile = path;
  delete [] path2;
}

void updatetime() {
  sendNTPpacket(timeServerIP);
  delay(1000);
  int cb = udp.parsePacket();
  if (cb) {
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    zegar.setRTC(epoch);
  }
}

unsigned long sendNTPpacket(IPAddress & address) {
  Serial.println("Sending NTP packet...");
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

bool wifiConn (JsonObject &wifiset) {
  bool dhcp;
  String tmpdhcp = wifiset["ip"]["mode"]; //Messed up stuff
  if (tmpdhcp == "dhcp") dhcp = true;     //Idk how to fix it into wifiConn
  else if (tmpdhcp == "static") dhcp = false;
  String ip = wifiset["ip"]["ip"];
  String gate = wifiset["ip"]["gateway"];
  String sub = wifiset["ip"]["subnet"];
  String ssid = wifiset["wlan"]["ssid"];
  String pass = wifiset["wlan"]["pass"];

  char *ssidc = new char[ssid.length() + 1];
  strcpy(ssidc, ssid.c_str());
  char *passc = new char[pass.length() + 1];
  strcpy(passc, pass.c_str());
  //Serial.println(ssid);
  //Serial.println(password);
  if (!dhcp) WiFi.config(stringToIP(ip), stringToIP(gate), stringToIP(sub));
  WiFi.begin(ssidc, passc);
  Serial.print("Connecting to ");
  Serial.println(ssidc);

  delete[] passc;
  delete[] ssidc;
  // Wait for connection
  uint8_t i = 0;

  while (WiFi.status() != WL_CONNECTED && i++ < 20) {//wait 10 seconds
    delay(500);
  }

  if (i >= 21 && WiFi.status() != WL_CONNECTED) return false;

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
    File root = SD.open(DHCPFILE, FILE_WRITE);
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
  Serial.println("Somebody tried to access password file!");
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
  if (server.uri() != "/") return;
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

void deleteRecursive(String path) {
  File file = SD.open((char *)path.c_str());
  if (!file.isDirectory()) {
    file.close();
    SD.remove((char *)path.c_str());
    return;
  }

  file.rewindDirectory();
  while (true) {
    File entry = file.openNextFile();
    if (!entry) break;
    String entryPath = path + "/" + entry.name();
    if (entry.isDirectory()) {
      entry.close();
      deleteRecursive(entryPath);
    } else {
      entry.close();
      SD.remove((char *)entryPath.c_str());
    }
    yield();
  }

  SD.rmdir((char *)path.c_str());
  file.close();
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

String printDateTime() {
  char datestring[20];
  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u;%02u:%02u"),
             zegar.day,
             zegar.month,
             zegar.year,
             zegar.hour,
             zegar.minute );
  return datestring;
}

double getTemp(JsonObject &addrset, int row) {
  byte tempaddr[8];
  for (int b = 0; b < 8; b++)
    tempaddr[b] = addrset["sensor"][row][b + 1];

  //if (tempaddr[b] < 10) Serial.print("0");
  //Serial.print(tempaddr[b], HEX);
  //Serial.println();
  return sensors.getTempC(tempaddr);
}
