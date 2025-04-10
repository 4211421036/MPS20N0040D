#include "MPS20N0040D.h"

MPS20N0040D pressureSensor(3, 4); // PD_SCK, DOUT

void setup() {
  Serial.begin(9600);
  pressureSensor.begin();
}

void loop() {
  float pressure = pressureSensor.readPressure();
  float flowRate = pressureSensor.calculateFlowRate(pressure);
  
  Serial.print("Pressure: "); Serial.print(pressure); Serial.println(" Pa");
  Serial.print("Flow Rate: "); Serial.print(flowRate, 6); Serial.println(" m³/s");
  delay(1000);
}
