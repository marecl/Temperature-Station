#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>
#define SETTINGS_FILE "SETTINGS.TXT"
#define SD_D D0
#define SD_CS D4

File root;
bool hasSD = false;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  if (SD.begin(SD_CS) && !digitalRead(SD_D)) hasSD = true;
  else ESP.restart();
  root = SD.open(SETTINGS_FILE , FILE_READ);
  int tmp = root.read();
  String json = "";
  while (tmp != -1) {
    if (tmp != 10 && tmp != 13 && tmp != 32 && tmp != 9)
      json += (char)tmp;
    tmp = root.read();
  }
  root.close();
  json.trim();
  Serial.println("\n\n" + json + '\n');
  DynamicJsonBuffer jsonBuffer(750);
  JsonObject& settings = jsonBuffer.parseObject(json);

  if (!settings.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  String ext = settings["sensor_name"][1][0];
  Serial.println(ext);
}

void loop() {
  while (true) {
    Serial.print(".");
    delay(1000);
  }
}
