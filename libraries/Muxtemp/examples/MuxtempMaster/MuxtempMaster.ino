#include <Wire.h>
#include <Muxtemp.h>

/*
   Example of master program for Muxtemp
   Connect by I2C to slave device
   Due to DHT "speed" it needs some time after sensor-related operations

   ToDo: Add humidity reading (DHT)
*/

Muxtemp ext(Wire);

void setup() {
  Wire.begin(5, 4); //SDA, SCL
  Serial.begin(115200);
  Serial.print(F("\r\nMaster begin \r\nMuxtemp: "));
  delay(500); //Wait for boot
  Serial.println((ext.begin(0x10) == 0) ? F("Present") : F("Error"));
  Serial.print(F("Ports: "));
  Serial.println(ext.getCount());

  Serial.print(F("1wire bypass: "));
  Serial.println(ext.bypass1Wire() ? F("yes") : F("no"));
}

void loop() {
  ext.refreshPorts(); //Probe ports and get temperatures at the same time
  delay(5000); //Any DHT-related action needs some time

  ext.getPorts(); //Which ports are connected & what type they are

  Serial.println();
  for (uint8_t a = 0; a < ext.getCount(); a++) {
    //Print type of every port
    Serial.print(String(a) + "(" + String(ext.typeOf(a)) + "): ");
    //Get the reading from each port independently
    Serial.print(String(ext.getTemp(a)));

    if (ext.typeOf(a) == 5) {
      Serial.print(F(" ("));
      uint8_t *_a = ext.getAddress(a); //Pointer to 8-element array!
      for (int c = 0; c < 8; c++) {
        Serial.print(_a[c]); //Print address
        if (c != 7) Serial.print(',');
      }
      Serial.print(')');
    }
    Serial.println();
  }
}