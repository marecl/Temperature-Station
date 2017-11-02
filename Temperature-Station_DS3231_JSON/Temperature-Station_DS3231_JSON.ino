#include <ArduinoJson.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <RtcDS3231.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include "defs.h"
/*
  This sketch needs hardware hack if you're using Chinese SD adapter

  Solder 10kom resistor between 3V3 and SD detect pin (on shield)
  and SD Detect pin to D0

  SD Detect is connected to GND when SDcard is loaded.
*/

/*
  Web updater!

  Encrypt password in file

  Add option to change/update date&time via http
*/
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

OneWire oneWire(OW_PORT);
DallasTemperature sensors(&oneWire);

DynamicJsonBuffer jsonBuffer(1000);

RtcDS3231<TwoWire> zegar(Wire);
RtcDateTime teraz;

WiFiUDP udp;
IPAddress timeServerIP;
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  pinMode(D0, INPUT);
  Wire.begin(SDA, SCL);
  Serial.print(F("\n"));
  zegar.Begin();
  sensors.begin();
  sensors.requestTemperatures();

  if (!digitalRead(SD_D)) {
    if (SD.begin(SD_CS)) {
      Serial.println(F("TemperatureStation DS3231v1.0"));
      if (!SD.exists(SETTINGS_FILE)) {
        Serial.println(F("No settings file!"));
        delay(1000);
        ESP.restart();
      }
    } else {
      Serial.println(F("SD Card detected but cannot be initialized!"));
      delay(1000);
      ESP.restart();
    }
  } else {
    Serial.print(F("No card inserted\n"));
    delay(1000);
    ESP.restart();
  }

  File root = SD.open(SETTINGS_FILE , FILE_READ);
  int tmp = root.read();
  while (tmp != -1) {
    if (tmp != 32 && tmp != 9)
      json += (char)tmp;
    tmp = root.read();
  }
  root.close();

  JsonObject& settings = jsonBuffer.parseObject(json);
  if (!settings.success()) {
    Serial.println(F("Invalid JSON file"));
    Serial.println(F("Come back with valid one. Rebooting..."));
    delay(1000);
    ESP.restart();
  }

  if (wifiConn(settings))
    httpserver = true;
  else {
    Serial.print(F("Could not connect to WiFi!\n"));
    Serial.print(F("Log mode only\n"));
    httpserver = false;
  }

  if (httpserver) {
    server.on("/sensors", HTTP_GET, sensorSettings);
    server.on("/time", HTTP_GET, updatetime);
    server.on("/list", HTTP_GET, printDirectory);
    server.on("/", HTTP_DELETE, handleDelete);
    server.on("/", HTTP_PUT, handleCreate);
    server.on("/", HTTP_POST, []() {
      returnOK();
    }, handleFileUpload);
    server.on("/settings.txt", returnForbidden);
    server.onNotFound(handleNotFound);
    httpUpdater.setup(&server);
    server.begin();
    Serial.println(F("HTTP server started"));
    zone = settings["timezone"];

    use_ntp = settings["use_ntp"];
    if (use_ntp) {
      unsigned long newTime = updateNTP(settings);

      teraz = zegar.GetDateTime();
      if (RtcDateTime(newTime) != teraz) {
        Serial.println(F("Adjusting time..."));
        Serial.println("Before: " + printDateTime(teraz));
        Serial.println("Current: " + printDateTime(RtcDateTime(newTime)));
        zegar.SetDateTime(newTime);
        teraz = zegar.GetDateTime();
        Serial.print("After: ");
      }
    }
  }
  Serial.println(printDateTime(teraz));

  /*if (use_ntp) {
    delay(2500);
    udp.begin(2390);
    const char* ntpServerName = settings["ntp_server"];
    WiFi.hostByName(ntpServerName, timeServerIP);
    sendNTPpacket(timeServerIP);
    delay(1000);
    int cb = udp.parsePacket();
    if (!cb)
      Serial.println(F("Received invalid NTP data"));
    else {
      udp.read(packetBuffer, 48);
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      unsigned long epoch = secsSince1900 - 2208988800UL - 946684800;
      zone = settings["timezone"];
      epoch += zone * 3600; //strefa
      if (letni) epoch += 3600; //czas letni
      teraz = zegar.GetDateTime();
      if (RtcDateTime(epoch) != teraz) {
        Serial.println(F("Adjusting time..."));
        Serial.println("Before: " + (String)teraz);
        zegar.SetDateTime(epoch);
        teraz = zegar.GetDateTime();
        Serial.println("After:  " + (String)teraz);
      }
    }
    }
    }*/

  valid_sensors = settings["valid_sensors"];
  Serial.println(F("\nSensors:"));
  for (int a = 0; a < valid_sensors; a++) {
    Serial.print((String)(a + 1) + "/" + (String)MAX_SENSORS + ": ");
    String xdtmp = settings["sensor"][a][0];
    Serial.print(xdtmp);

    if (getTemp(settings, a) != -127)
      Serial.println(F(" Connected"));
    else
      Serial.println(F(" N/C"));
  }
  Serial.println();
  createfile(settings);
}

void loop() {
  File setfile = SD.open(SETTINGS_FILE , FILE_READ);
  int tmp = setfile.read();
  String json = "";
  while (tmp != -1) {
    if (tmp != 32 && tmp != 9)
      json += (char)tmp;
    tmp = setfile.read();
  }
  setfile.close();
  JsonObject& settings = jsonBuffer.parseObject(json);
  //Okay, JSON loaded so now we can start loop
  //Uhnfortunately I can't pass object from setup()

  while (1) {
    while ((teraz.Minute() % 5 != 0) && !digitalRead(SD_D)) {
      teraz = zegar.GetDateTime();
      httpserver ? server.handleClient() : delay(10);
    }

    if (!digitalRead(SD_D)) {
      sensors.requestTemperatures();
      teraz = zegar.GetDateTime();
      if (teraz.Hour() == 0 && teraz.Minute() == 0) createfile(settings);
      File root = SD.open(workfile, FILE_WRITE);
      root.print(printDateTime(teraz) + ";");
      root.flush();
      for (int c = 0; c < valid_sensors; c++) {
        root.print(getTemp(settings, c), 1);
        if (c + 1 < valid_sensors)
          root.print(F(";"));
        else root.print(F("\r\n"));
        root.flush();
      }
      root.close();
    } else {
      Serial.println(F("!!! NO SDCARD !!!"));
      while (digitalRead(SD_D)) delay(1000);
      Serial.println(F("SD Card inserted. Reboot..."));
      ESP.restart();
    }

    while ((teraz.Minute() % 5 == 0) && !digitalRead(SD_D)) {
      teraz = zegar.GetDateTime();
      httpserver ? server.handleClient() : delay(10);
    }
  }
}

void sensorSettings() {
  sensors.requestTemperatures();
  DynamicJsonBuffer sensBuff(1000);

  File root = SD.open(SETTINGS_FILE , FILE_READ);
  int tmp = root.read();
  json = "";
  while (tmp != -1) {
    if (tmp != 32 && tmp != 9)
      json += (char)tmp;
    tmp = root.read();
  }
  root.close();
  JsonObject& sensSet = sensBuff.parseObject(json);

  if (!sensSet.success())
    return;

  String changeName;
  int changePos;

  if (server.args() != 0) {
    for (int x = server.args() - 1; x >= 0; x--) {
      if (server.argName(x) == "Remove") {
        JsonArray& setMain = sensSet["sensor"];
        setMain.remove((atoi((server.arg(x)).c_str())));
        valid_sensors--;
        sensSet["valid_sensors"] = valid_sensors;
        saveJson(sensSet);
        if ((server.arg(x) == server.argName(x - 1)) && x > 0)
          x -= 1;
      } else {
        changePos = atoi((server.argName(x)).c_str());
        changeName = server.arg(x);
        sensSet["sensor"][changePos][0] = changeName;
        saveJson(sensSet);
      }
    }
    server.sendContent(F("HTTP/1.1 307 Temporary Redirect\r\n"));
    server.sendContent(F("Location: /sensors\r\n"));
    server.sendContent(F("Connection: Close\r\n\r\n"));
    return;
  }

  byte i;
  byte addr[8];
  String temp;
  int sensorRow;

  server.sendContent(F("<!DOCTYPE html><html><head><title>Sensors</title>"));
  server.sendContent(F("<style>table,th,td{border:1px solid black;"));
  server.sendContent(F("text-align:center}</style></head>"));
  server.sendContent(F("<body><table><caption><b>Available sensors</b></caption>"));
  server.sendContent(F("<tr><th>Address (DEC)</th><th>Temperature</th><th>Name</th>"));
  server.sendContent(F("<form>"));


  for (int a = 0; a < valid_sensors; a++) {
    server.sendContent(F("<tr><td>"));
    for (int b = 0; b < 8; b++)
      addr[b] = sensSet["sensor"][a][b + 1];

    for (i = 0; i < 8; i++) {
      if (addr[i] < 100) server.sendContent(F("0"));
      if (addr[i] < 10) server.sendContent(F("0"));
      server.sendContent((String)addr[i]);
      if (i < 7)
        server.sendContent(F(","));
    }
    server.sendContent(F("</td><td>"));
    if (getTemp(sensSet, a) != -127)
      server.sendContent((String)sensors.getTempC(addr) + (char)176 + "C");
    else
      server.sendContent(F("N/C"));

    server.sendContent(F("</td><td>"));
    String tempName = sensSet["sensor"][a][0];

    server.sendContent(F("<input type = \"text\" name = "));
    server.sendContent(String(a)); //Position in file
    server.sendContent(F(" value = "));
    server.sendContent(tempName);
    server.sendContent(F("><input type=\"checkbox\" name=\"Remove\" value= "));
    server.sendContent(String(a));
    server.sendContent(F("></td></tr>"));
  }

  while (oneWire.search(addr)) {
    if (isMember(addr, sensSet, valid_sensors))
      continue;

    server.sendContent(F("<tr><td>"));
    for (i = 0; i < 8; i++) {
      if (addr[i] < 100) server.sendContent(F("0"));
      if (addr[i] < 10) server.sendContent(F("0"));
      server.sendContent((String)addr[i]);
      if (i < 7)
        server.sendContent(F(","));
    }
    if (OneWire::crc8(addr, 7) != addr[7]) {
      server.sendContent(F("Invalid CRC!"));
      return;
    }

    server.sendContent(F("</td><td>"));

    valid_sensors += 1;
    sensSet["valid_sensors"] = valid_sensors;
    String tempname = addrToString(addr);
    JsonArray& setMain = sensSet["sensor"];
    JsonArray& newSet = setMain.createNestedArray();
    newSet.add(tempname);
    for (int x = 0; x < 8; x++)
      newSet.add(addr[x]);
    sensorRow = valid_sensors - 1;
    saveJson(sensSet);

    server.sendContent((String)sensors.getTempC(addr) + (char)176 + "C");
    server.sendContent(F("</td><td>"));
    server.sendContent(F("<input type = \"text\" name = "));
    server.sendContent(String(sensorRow)); //Position in file
    server.sendContent(F(" value = "));
    server.sendContent(F("UNKNOWN_NEW")); //Sensor name
    server.sendContent(F("><input type=\"checkbox\" name=\"Remove\" value= "));
    server.sendContent(String(sensorRow)); //Position in file
    server.sendContent(F("></td></tr>"));
  }
  server.sendContent(F("<button type=\"submit\" value=\"Save\">Save Changes</button>"));
  server.sendContent(F("<button type=\"reset\" value=\"Reset\">Reset</button>"));
  server.sendContent(F("</form>"));

  server.sendContent(F("</table></body></html>"));
  return;
}

void createfile(JsonObject & nameobj) {
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
  if (!SD.exists(path2)) {
    File dest = SD.open(path2, FILE_WRITE);

    dest.print(F("Date;Time;"));
    dest.flush();
    for (int c = 0; c < valid_sensors; c++) {
      String tempname = nameobj["sensor"][c][0];
      dest.print(tempname);
      if (valid_sensors - c > 1) dest.print(F(";"));
      else dest.print(F("\r\n"));
      dest.flush();
    }
    dest.close();
  }
  workfile = path;
  delete [] path2;
}

void updatetime() {
  /*Serial.print(F("Aktualizacja czasu!\nCzas przed: "));
    Serial.println(printDateTime(zegar));



    Serial.print(F("Czas po: "));
    Serial.println(printDateTime(zegar));*/

  server.sendContent(F("HTTP/1.1 307 Temporary Redirect\r\n"));
  server.sendContent(F("Location: /\r\n"));
  server.sendContent(F("Connection: Close\r\n\r\n"));
}

unsigned long updateNTP(JsonObject& timeSet) {
  unsigned long newTime = 0;
  delay(2500);
  udp.begin(2390);
  const char* ntpServerName = timeSet["ntp_server"];
  Serial.println(ntpServerName);
  WiFi.hostByName(ntpServerName, timeServerIP);

  Serial.println(F("Sending NTP packet..."));

  byte packetBuffer[48];
  memset(packetBuffer, 0, 48);
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  udp.beginPacket(timeServerIP, 123);
  udp.write(packetBuffer, 48);
  udp.endPacket();

  delay(1000);
  int cb = udp.parsePacket();
  if (cb) {
    Serial.println("Valid NTP data");
    udp.read(packetBuffer, 48);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    newTime = secsSince1900 + (int)zone * 3600;
    if (letni) newTime += 3600;
    newTime -= 2208988800UL;
    newTime -= 946684800;
    return newTime;
  } else Serial.println("Invalid NTP data");
  return 0;
}

bool wifiConn(JsonObject & wifiset) {
  String tmpdhcp = wifiset["ip"]["mode"];
  String ip = wifiset["ip"]["ip"];
  String gate = wifiset["ip"]["gateway"];
  String sub = wifiset["ip"]["subnet"];
  saved_ap = wifiset["saved_ap"];

  WiFi.mode(WIFI_STA); //WIFI_AP, WIFI_STA, WIFI_AP_STA
  for (int xxx = 0; xxx < saved_ap; xxx++) {
    String ssid = wifiset["wlan"][xxx][0];
    String pass = wifiset["wlan"][xxx][1];
    if (ssid == "-1" && pass == "-1") return false;
    char *ssidc = new char[ssid.length() + 1];
    strcpy(ssidc, ssid.c_str());
    char *passc = new char[pass.length() + 1];
    strcpy(passc, pass.c_str());
    if (tmpdhcp == "static") WiFi.config(stringToIP(ip), stringToIP(gate), stringToIP(sub));
    WiFi.begin(ssidc, passc);
    Serial.print(F("Connecting to "));
    Serial.print(ssid);

    delete[] passc;
    delete[] ssidc;
    // Wait for connection
    uint8_t i = 0;

    while (WiFi.status() != WL_CONNECTED && i++ < 20) { //wait 10 seconds
      delay(500);
      Serial.print(F("."));
    }

    if (i >= 21 && WiFi.status() != WL_CONNECTED) {
      Serial.println(F(" Fail"));
      if ((xxx + 1) == saved_ap) {
        Serial.println(F("No networks available or incorrect credentials"));
        return false;
      } else continue;
    }

    Serial.print(F(" Success\n"));
    Serial.print(F("IP obtain mode: "));
    if (tmpdhcp == "dhcp") Serial.println(F("DHCP"));
    else Serial.println(F("Static"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
    Serial.print(F("Gateway: "));
    Serial.println(WiFi.gatewayIP());
    Serial.print(F("Subnet mask: "));
    Serial.println(WiFi.subnetMask());
    break;
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
  server.send(403, "text/plain", "Access Denied\r\n");
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
    Serial.println(F("Sent less data than expected!"));
  }

  dataFile.close();
  return true;
}

void handleFileUpload() {
  File uploadFile;
  if (server.uri() != "/") return;
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    if (SD.exists((char *)upload.filename.c_str()))
      SD.remove((char *)upload.filename.c_str());
    uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    Serial.print(F("Upload: START, filename: "));
    Serial.println(upload.filename);
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
    Serial.print(F("Upload: WRITE, Bytes: "));
    Serial.println(upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) uploadFile.close();
    Serial.print(F("Upload: END, Size: "));
    Serial.println(upload.totalSize);
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
  if (server.args() == 0) return returnFail(F("BAD ARGS"));
  String path = server.arg(0);
  if (path == "/" || !SD.exists((char *)path.c_str())) {
    returnFail(F("BAD PATH"));
    return;
  }
  deleteRecursive(path);
  returnOK();
}

void handleCreate() {
  if (server.args() == 0) return returnFail(F("BAD ARGS"));
  String path = server.arg(0);
  if (path == "/" || SD.exists((char *)path.c_str())) {
    returnFail(F("BAD PATH"));
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
  if (!server.hasArg("dir")) return returnFail(F("BAD ARGS"));
  String path = server.arg("dir");
  if (path != "/" && !SD.exists((char *)path.c_str()))
    return returnFail(F("BAD PATH"));
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

  server.sendContent(F("["));
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    if (!entry)
      break;

    String output;
    if (cnt > 0)
      output = ',';

    output += F("{\"type\":\"");
    output += (entry.isDirectory()) ? "dir" : "file";
    output += F("\",\"name\":\"");
    output += entry.name();
    output += F("\"");
    output += F("}");
    server.sendContent(output);
    entry.close();
  }
  server.sendContent(F("]"));
  dir.close();
}

void handleNotFound() {
  if (!digitalRead(SD_D) && loadFromSdCard(server.uri())) return;
  String message = F("PLIKU NIE ZNALEZIONO\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.print(message);
}

double getTemp(JsonObject & addrset, int row) {
  byte tempaddr[8];
  for (int b = 0; b < 8; b++)
    tempaddr[b] = addrset["sensor"][row][b + 1];
  return sensors.getTempC(tempaddr);
}

