/**
 * @file MPS20N0040D.cpp
 * @author-team Nama Author 1 Ana Maulida (anamaulida@students.unnes.ac.id), 2. GALIH RIDHO UTOMO (g4lihru@students.unnes.ac.id)
 * @url GitHub Repository: https://github.com/yourrepo/venturi_meter
 * 
 * @brief Implementasi driver sensor tekanan MPS20N0040D-S.
 * 
 * @details
 * 1. Konversi ADC:
 * - Menggunakan protokol HX710B (24-bit)
 * - Penguatan tetap 128 (datasheet HX710 hal.1)
 * 
 * 2. Perhitungan Tekanan:
 * - Model linier: P = (V_out / V_fs) * P_range
 * 
 * 3. Aliran Venturi:
 * - Implementasi persamaan Bernoulli (fluida inkompresibel)
 * - Asumsi rasio beta 0.5 (d2/d1)
 * 
 * @Hasil pengujian:
 * - Akurasi: ± 0,5% FS (dengan kalibrasi)
 * - Sampling Rate: 10 Hz (default HX710)
 */

#include "MPS20N0040D.h" // Mengimpor file header untuk mendapatkan definisi class

// Konstruktor - Dipanggil saat objek sensor dibuat
MPS20N0040D::MPS20N0040D(uint8_t pdSckPin, uint8_t doutPin) {
  _pdSckPin = pdSckPin;  // Menyimpan pin clock untuk komunikasi dengan HX710
  _doutPin = doutPin;    // Menyimpan pin data untuk komunikasi dengan HX710
}

// Fungsi untuk memulai komunikasi dengan sensor
void MPS20N0040D::begin() {
  pinMode(_pdSckPin, OUTPUT);  // Set pin clock sebagai output
  pinMode(_doutPin, INPUT);    // Set pin data sebagai input
  digitalWrite(_pdSckPin, LOW);  // Mulai dengan clock dalam kondisi LOW
}

// Fungsi untuk membaca nilai ADC dari chip HX710
long MPS20N0040D::readADC() {
  // Menunggu hingga chip siap mengirim data (ditandai dengan DOUT menjadi LOW)
  while (digitalRead(_doutPin));
  
  // Membaca 24-bit data (bit per bit)
  long count = 0;
  for (uint8_t i = 0; i < 24; i++) {
    digitalWrite(_pdSckPin, HIGH);  // Kirim pulsa clock HIGH
    delayMicroseconds(1);           // Delay mikro detik
    count = count << 1;             // Geser nilai hasil satu bit ke kiri
    digitalWrite(_pdSckPin, LOW);   // Turunkan clock ke LOW
    delayMicroseconds(1);           // Delay mikro detik
    if (digitalRead(_doutPin)) count++;  // Jika pin data HIGH, tambahkan 1 ke nilai hasil
  }
  
  // Pulsa ke-25 untuk mengatur mode chip HX710
  digitalWrite(_pdSckPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(_pdSckPin, LOW);
  
  // Mengembalikan nilai dengan operasi XOR untuk mengkonversi format 2's complement
  return count ^ 0x800000;
}

// Fungsi untuk mengkonversi nilai ADC ke tegangan listrik
float MPS20N0040D::adcToVoltage(long adcValue) {
  // HX710 memiliki resolusi 24-bit (2^24 = 16777216) dengan gain 128
  // Rumus: Voltage = (ADC / 2^23) * Vref / gain
  return (adcValue / 8388608.0) * V_REF / 128.0;
}

// Fungsi untuk mengkonversi tegangan ke nilai tekanan
float MPS20N0040D::voltageToPressure(float voltage) {
  // Sensor memiliki output maksimum 100mV pada tekanan maksimum 40 kPa
  // Tekanan = (Tegangan / Tegangan Maksimum) * Rentang Tekanan Maksimum
  return (voltage / FS_OUTPUT) * PRESSURE_RANGE;
}

// Fungsi untuk membaca tekanan langsung dalam satuan Pascal
float MPS20N0040D::readPressure() {
  long raw = readADC();             // Baca nilai ADC mentah
  float voltage = adcToVoltage(raw);  // Konversi ke tegangan
  return voltageToPressure(voltage);  // Konversi ke tekanan
}

// Fungsi untuk menghitung laju aliran berdasarkan perbedaan tekanan
float MPS20N0040D::calculateFlowRate(float pressureDiff, float density) {
  // Menggunakan prinsip Venturi untuk menghitung aliran berdasarkan perbedaan tekanan
  // Persamaan Venturi: Q = A1 * sqrt(2ΔP/(ρ(1 - β⁴)))
  // di mana:
  // - Q = laju aliran
  // - A1 = luas penampang pipa utama
  // - ΔP = perbedaan tekanan
  // - ρ = kerapatan cairan
  // - β = rasio diameter (throat / pipa utama)
  
  const float beta = 0.5;  // Asumsi diameter throat adalah setengah dari diameter pipa utama
  const float A1 = 0.000314;  // Luas pipa (contoh untuk diameter 20mm)
  return A1 * sqrt(2 * abs(pressureDiff) / (density * (1 - pow(beta, 4))));
}
