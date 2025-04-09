#include "MPS20N0040D.h"

MPS20N0040D::MPS20N0040D(uint8_t pdSckPin, uint8_t doutPin) {
  _pdSckPin = pdSckPin;
  _doutPin = doutPin;
}

void MPS20N0040D::begin() {
  pinMode(_pdSckPin, OUTPUT);
  pinMode(_doutPin, INPUT);
  digitalWrite(_pdSckPin, LOW);
}

long MPS20N0040D::readADC() {
  // Implementasi pembacaan HX710 (adaptasi dari datasheet)
  while (digitalRead(_doutPin));
  
  long count = 0;
  for (uint8_t i = 0; i < 24; i++) {
    digitalWrite(_pdSckPin, HIGH);
    delayMicroseconds(1);
    count = count << 1;
    digitalWrite(_pdSckPin, LOW);
    delayMicroseconds(1);
    if (digitalRead(_doutPin)) count++;
  }
  
  // 25th pulse untuk set mode
  digitalWrite(_pdSckPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(_pdSckPin, LOW);
  
  return count ^ 0x800000; // Konversi 2's complement
}

float MPS20N0040D::adcToVoltage(long adcValue) {
  // Konversi ADC ke voltase (HX710 gain 128, VREF 5V)
  return (adcValue / 8388608.0) * V_REF / 128.0; // 24-bit resolution
}

float MPS20N0040D::voltageToPressure(float voltage) {
  // Konversi voltase ke tekanan (linearitas datasheet ±0.25% FS)
  // Referensi jurnal: doi:10.3390/s19132985 (kalibrasi sensor MEMS)
  return (voltage / FS_OUTPUT) * PRESSURE_RANGE;
}

float MPS20N0040D::readPressure() {
  long raw = readADC();
  float voltage = adcToVoltage(raw);
  return voltageToPressure(voltage);
}

float MPS20N0040D::calculateFlowRate(float pressureDiff, float density) {
  // Persamaan Venturi: Q = A1 * sqrt(2ΔP/(ρ(1 - β⁴)))
  // Asumsi diameter throat (d2) = 0.5*d1, β = 0.5
  const float beta = 0.5;
  const float A1 = 0.000314; // Luas pipa (contoh: diameter 20mm)
  return A1 * sqrt(2 * abs(pressureDiff) / (density * (1 - pow(beta, 4))));
}
