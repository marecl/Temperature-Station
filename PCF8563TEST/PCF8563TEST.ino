#include <Wire.h>
#include <PCF8563.h>

byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
String days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
const uint8_t daysInMonth [] PROGMEM = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

byte bcdToDec(byte value) {
  return ((value / 16) * 10 + value % 16);
}

byte decToBcd(byte value) {
  return (value / 10 * 16 + value % 10);
}

void setPCF8563(uint32_t t) {
  t -= 946684800UL;
  uint8_t ss = t % 60;
  t /= 60;
  uint8_t mm = t % 60;
  t /= 60;
  uint8_t hh = t % 24;
  uint16_t d = t / 24;
  int dt = (d % 7) - 1;
  uint8_t przest = 0;
  int r;
  int m;
  for (r = 0; ; ++r) {
    przest = r % 4 == 0;
    if (d < 365 + przest) break;
    d -= 365 + przest;
  }
  for (m = 1; ; m++) {
    uint8_t dniwmsc = pgm_read_byte(daysInMonth + m - 1);
    if (przest && m == 2)
      dniwmsc++;
    if (d < dniwmsc)
      break;
    d -= dniwmsc;
  }
  d++;
  Serial.println(ss);
  Serial.println(mm);
  Serial.println(hh);
  Serial.println(d);
  Serial.println(dt);
  Serial.println(m);
  Serial.println(r);

  Wire.beginTransmission(0x51);
  Wire.write(0x02);
  Wire.write(decToBcd(ss)); //sek
  Wire.write(decToBcd(mm)); //min
  Wire.write(decToBcd(hh)); //godz
  Wire.write(decToBcd(d)); //dzien
  Wire.write(decToBcd(dt)); //dzien tyg (1-7)
  Wire.write(decToBcd(m)); //msc
  Wire.write(decToBcd(r)); //rok-2000
  Wire.endTransmission();
}
int czas[7];
void readPCF8563() {
  Wire.beginTransmission(0x51);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(0x51, 7);
  czas[0] = bcdToDec(Wire.read() & B01111111); // sek
  czas[1] = bcdToDec(Wire.read() & B01111111); // min
  czas[2] = bcdToDec(Wire.read() & B00111111); //godz
  czas[3] = bcdToDec(Wire.read() & B00111111); //dz msc
  czas[4] = bcdToDec(Wire.read() & B00000111); //dz tyg
  czas[5] = bcdToDec(Wire.read() & B00011111); // msc
  czas[6] = bcdToDec(Wire.read()); //rok
}

uint32_t readPCF8563epoch(){
  
}

void setup() {
  Wire.begin(D1, D2);
  Serial.begin(115200);
  setPCF8563(1496435510UL);
}

void loop() {
  readPCF8563();
  Serial.print(days[czas[4]]);
  Serial.print(" ");
  Serial.print(czas[3], DEC);
  Serial.print("/");
  Serial.print(czas[5], DEC);
  Serial.print("/20");
  Serial.print(czas[6], DEC);
  Serial.print(" - ");
  Serial.print(czas[2], DEC);
  Serial.print(":");
  if (czas[1] < 10)
    Serial.print("0");
  Serial.print(czas[1], DEC);
  Serial.print(":");
  if (czas[0] < 10)Serial.print("0");
  Serial.println(czas[0], DEC);
  delay(1000);
}
