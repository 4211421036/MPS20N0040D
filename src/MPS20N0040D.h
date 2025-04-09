#ifndef MPS20N0040D_H
#define MPS20N0040D_H

#include <Arduino.h>

class MPS20N0040D {
  public:
    // Konstruktor dengan pin HX710
    MPS20N0040D(uint8_t pdSckPin, uint8_t doutPin);
    
    // Inisialisasi sensor
    void begin();
    
    // Membaca tekanan dalam Pascal (Pa)
    float readPressure();
    
    // Menghitung laju aliran (m³/s) berdasarkan persamaan Venturi
    float calculateFlowRate(float pressureDiff, float density = 1000.0);

  private:
    uint8_t _pdSckPin, _doutPin;
    long readADC();
    float adcToVoltage(long adcValue);
    float voltageToPressure(float voltage);
    
    // Kalibrasi berdasarkan datasheet dan jurnal
    const float V_REF = 5.0;       // Referensi HX710 (sesuaikan)
    const float FS_OUTPUT = 0.1;   // 100mV full-scale (datasheet)
    const float PRESSURE_RANGE = 40000.0; // 40 kPa = 40000 Pa
};

#endif
