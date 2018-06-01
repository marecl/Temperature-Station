#include <Wire.h>
#include <Muxtemp.h>

/*
   Example of master program for Muxtemp
   Connect by I2C to slave device
   Due to DHT "speed" it needs some time after sensor-related operations
*/

Muxtemp ext(Wire);

void setup() {
  Wire.begin(0, 2);
  Serial.begin(115200);
  Serial.println(F("Master begin"));
  Serial.println((ext.begin(0x10) == 0) ? F("Working!") : F("Error"));
  Serial.println(ext.getCount());

  ext.getPorts();

  for (uint8_t a = 0; a < ext.getCount(); a++)
    Serial.print(String(ext.typeOf(a)) + " ");
}

void loop() {
  ext.refreshPorts(); //Probe ports and get temperatures at the same time
  delay(2000); //Any DHT-related action needs some time

  ext.getPorts(); //Which ports are connected & what type they are
  
  for (int a = 0; a < ext.getCount(); a++) {
    //Print type of every port
    Serial.print(String(a) + "(" + String(ext.typeOf(a)) + "): ");
    //Get the reading from each port independently
    Serial.println(String(ext.getTemp(a)));
  }
  Serial.println();
}
