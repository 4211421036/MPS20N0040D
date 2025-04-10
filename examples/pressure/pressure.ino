#include "MPS20N0040D.h"

MPS20N0040D pressureSensor(3, 4); // PD_SCK, DOUT

void setup() {
  Serial.begin(9600);
  pressureSensor.begin();
}

void loop() {
  float pressure = pressureSensor.readPressure();
  
  Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" Pa");
  delay(1000);
}
