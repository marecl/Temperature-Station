#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>
//#include <DallasTemperature.h>
//#include <OneWire.h>
//#include <Wire.h>
#define SETTINGS "SETTINGS.TXT"
#define SD_D D0
#define OW_PORT D3
#define SD_CS D4

File root;
//OneWire oneWire(OW_PORT);
//DallasTemperature sensors(&oneWire);
bool hasSD = false;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  if (SD.begin(SD_CS) && !digitalRead(SD_D)) hasSD = true;
  else {
    Serial.println("No card!");
    ESP.restart();
  }
  Serial.println("\n");
  root = SD.open(SETTINGS , FILE_READ);
  int tmp = root.read();
  String json = "";
  while (tmp != -1) {
    if (tmp != 10 && tmp != 13 && tmp != 32) {
      if (tmp == '\"') json += "\\\"";
      else json += (char)tmp;
      tmp = root.read();
    } else tmp = root.read();
  }
  root.close();
  Serial.println("\n\n" + json + '\n');
  json.trim();
  char jsonc[json.length()];
  json.toCharArray(jsonc, json.length());
  DynamicJsonBuffer jsonBuffer(250);
  JsonObject& settings = jsonBuffer.parseObject(json);

  if (!settings.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  String ext = settings["sensor_name"][0];
  Serial.println(ext);
}

void loop() {
  while (true) {
    Serial.print(".");
    delay(1000);
  }
}
