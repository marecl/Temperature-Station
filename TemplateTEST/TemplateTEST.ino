#include <SPI.h>
#include <SD.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#define READ_COND (tmp != '\r' && tmp != '\n' && tmp != 255 && tmp != '=')
#define READ_COND2 (tmp == '\r' && tmp == '\n' && tmp == 255 && tmp == '\t' && tmp =='\0')
#define TEMPLATE "TEMPLATE.TXT"

OneWire oneWire(D3);
DallasTemperature sensors(&oneWire);

bool usetemplate = false;
String fileheader = "Date;Time;";
byte _templ1_[8] = { -1, -1, -1, -1, -1, -1, -1, -1};
byte _templ2_[8] = { -1, -1, -1, -1, -1, -1, -1, -1};
byte _templ3_[8] = { -1, -1, -1, -1, -1, -1, -1, -1};
byte _templ4_[8] = { -1, -1, -1, -1, -1, -1, -1, -1};

void setup() {
  Serial.begin(115200);
  SD.begin(D4);
  if (SD.exists("/TEMPLATE.TXT")) usetemplate = true;
  if (usetemplate) {
    File root = SD.open(TEMPLATE, FILE_READ);
    char tmp = root.read();
    String tmpN = "";
    while (READ_COND) {
      tmpN += tmp;
      tmp = root.read();
    }
    fileheader += tmpN;
    Serial.println(fileheader);
    for (int b = 0; b < 8; b++) {
      _templ1_[b] += 1;
      tmpN = "";
      for (int a = 0; a < 2; a++) {
        tmp = root.read();
        tmpN += tmp;
      }
      //Serial.println(tmpN + ",");
      int zxc = 0;
      for (int a = 0; a <= 1; a++) {
        switch (tmpN[a]) {
          case '0': zxc = 0; break;
          case '1': zxc = 1; break;
          case '2': zxc = 2; break;
          case '3': zxc = 3; break;
          case '4': zxc = 4; break;
          case '5': zxc = 5; break;
          case '6': zxc = 6; break;
          case '7': zxc = 7; break;
          case '8': zxc = 8; break;
          case '9': zxc = 9; break;
          case 'A': zxc = 10; break;
          case 'B': zxc = 11; break;
          case 'C': zxc = 12; break;
          case 'D': zxc = 13; break;
          case 'E': zxc = 14; break;
          case 'F': zxc = 15; break;
        }
        _templ1_[b] += (zxc * pow(16, map(a, 0, 1, 1, 0)));
      }
      Serial.print("0x");
      Serial.println(_templ1_[b], HEX);
    }
    while (READ_COND2) tmp = root.read();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
