#include <Wire.h>
#include <Muxtemp.h>

/*
   Example of master program for Muxtemp
   Connect by I2C to slave device
   Due to DHT "speed" it needs some time after sensor-related operations
*/
Muxtemp ext(Wire);

void setup() {
  Wire.begin(D1, D2);
  Serial.begin(115200);
  Serial.println(F("Master begin"));
  Serial.println(ext.begin(0x10) ? "Yeee" : "Naaa");
  Serial.println(ext.getCount());

  ext.sendPorts();

  for (int a = 0; a < ext.getCount(); a++)
    Serial.print(String(ext.typeOf(a)) + " ");
}

void loop() {
  ext.probePorts(); //Any DHT-related action needs some time
  delay(2000);
  ext.readPorts();
  delay(2000);

  ext.sendPorts();
  for (int a = 0; a < ext.getCount(); a++) {
    Serial.print(String(a) + "(" + String(ext.typeOf(a)) + "): ");
    Serial.println(String(ext.getTemp(a)));
  }
  Serial.println();

  delay(1000);
}
