#include <DallasTemperature.h>
#include <OneWire.h>

OneWire oneWire(D3);
DallasTemperature sensors(&oneWire);
byte pokoj[8] = {0x28, 0xFF, 0x3B, 0xBD, 0x72, 0x16, 0x05, 0x69};
byte nadworze[8] = {0x28, 0xA5, 0xE2, 0x27, 0x00, 0x00, 0x80, 0x8A};
byte piec_wyjscie[8] = {0x28, 0xFF, 0xE2, 0x33, 0x34, 0x16, 0x04, 0xB6};
byte piec_powrot[8] = {0x28, 0xFF, 0xB0, 0xDC, 0x33, 0x16, 0x03, 0x8A};
void setup() {
  Serial.begin(9600);
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();
  Serial.print("Temperatura: \n");
  Serial.print("\tWewnatrz: ");
  Serial.println(sensors.getTempC(pokoj));
  Serial.print("\tNa zewnatrz: ");
  Serial.println(sensors.getTempC(nadworze));
  Serial.print("\tWyjscie pieca: ");
  Serial.println(sensors.getTempC(piec_wyjscie));
  Serial.print("\tPowrot pieca: ");
  Serial.println(sensors.getTempC(piec_powrot));
  delay(2500);
}
