#include <SPI.h>
#include <SD.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#define READ_COND (tmp != '\r' && tmp != '\n' && tmp != 255 && tmp != '=')
#define READ_COND2 (tmp == '\r' || tmp == '\n' || tmp == '\t' || tmp == 0)
#define TEMPLATE "TEMPLATE.TXT"
#define MAX_SENSORS 16

OneWire oneWire(D3);
DallasTemperature sensors(&oneWire);
/*
  !!! Request temperatures from sensors using row number from address table


  valid_sensors -> variable used to check how many sensors
      have assigned addresses

  MAX_SENSORS - declaration how many sensors can be connected
*/
bool usetemplate = false;
int valid_sensors = 0;
byte _templa_[MAX_SENSORS][8];
String fileheader = "Date;Time;";

void setup() {
  Serial.begin(115200);

  Serial.println();

  if (SD.begin(D4) && SD.exists("/TEMPLATE.TXT")) usetemplate = true;
  if (usetemplate) {
    File root = SD.open(TEMPLATE, FILE_READ);
    char tmp = root.read();
    String tmpN = "";
    int v = 0;
    while (root.peek() != -1 && valid_sensors < MAX_SENSORS) {
      while (READ_COND) {
        tmpN += tmp;
        tmp = root.read();
      }
      fileheader += tmpN + ";";

      Serial.print((String)(v + 1) + ". ");
      Serial.println(tmpN);

      tmpN = "";
      for (int b = 0; b < 8; b++) {
        for (int a = 0; a < 2; a++) {
          tmp = root.read();
          tmpN += tmp;
        }
        int zxc = 0;
        for (int a = 0; a <= 1; a++) {
          switch (tmpN[a]) {
            case '0': zxc += 0 * pow(16, map(a, 0, 1, 1, 0)); break;
            case '1': zxc += 1 * pow(16, map(a, 0, 1, 1, 0)); break;
            case '2': zxc += 2 * pow(16, map(a, 0, 1, 1, 0)); break;
            case '3': zxc += 3 * pow(16, map(a, 0, 1, 1, 0)); break;
            case '4': zxc += 4 * pow(16, map(a, 0, 1, 1, 0)); break;
            case '5': zxc += 5 * pow(16, map(a, 0, 1, 1, 0)); break;
            case '6': zxc += 6 * pow(16, map(a, 0, 1, 1, 0)); break;
            case '7': zxc += 7 * pow(16, map(a, 0, 1, 1, 0)); break;
            case '8': zxc += 8 * pow(16, map(a, 0, 1, 1, 0)); break;
            case '9': zxc += 9 * pow(16, map(a, 0, 1, 1, 0)); break;
            case 'A': zxc += 10 * pow(16, map(a, 0, 1, 1, 0)); break;
            case 'B': zxc += 11 * pow(16, map(a, 0, 1, 1, 0)); break;
            case 'C': zxc += 12 * pow(16, map(a, 0, 1, 1, 0)); break;
            case 'D': zxc += 13 * pow(16, map(a, 0, 1, 1, 0)); break;
            case 'E': zxc += 14 * pow(16, map(a, 0, 1, 1, 0)); break;
            case 'F': zxc += 15 * pow(16, map(a, 0, 1, 1, 0)); break;
          }
          _templa_[b][v] = zxc;
        }
        tmpN = "";

        if (_templa_[b][v] < 10) Serial.print('0');
        Serial.print(_templa_[b][v], HEX);

      }
      Serial.println();
      tmp = root.read();
      while (READ_COND2) tmp = root.read();
      tmpN = "";
      v++;
      valid_sensors = v;
    }
    root.close();
  }
  fileheader.remove(fileheader.length() - 1);

  Serial.println(fileheader);
  Serial.print("Valid sensors: ");
  Serial.print(valid_sensors);
  Serial.println("/" + (String)MAX_SENSORS);
  if (SD.exists("/TEMPHEAD.TXT")) SD.remove("/TEMPHEAD.TXT");
  File root = SD.open("/TEMPHEAD.TXT", FILE_WRITE);
  root.println(fileheader);
  root.flush();
  for (int c = 0; c < valid_sensors; c++) {
    root.print(getTemp(c));
    if (valid_sensors - c > 1) root.print(";");
    root.flush();
  }
  root.close();

}

double getTemp(int row) {
  byte tmp[8];
  for (int a = 0; a < 8; a++)
    tmp[a] = _templa_[a][row];
  sensors.requestTemperatures();
  return sensors.getTempC(tmp);
}

void loop() {
  while (1) delay(2500);

}
