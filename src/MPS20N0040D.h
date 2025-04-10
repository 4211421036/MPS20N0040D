/**
 * @file MPS20N0040D.h
 * @author-team Nama Author 1 Ana Maulida (anamaulida@students.unnes.ac.id), 2. GALIH RIDHO UTOMO (g4lihru@students.unnes.ac.id)
 * @url GitHub Repositori: https://github.com/4211421036/MPS20N0040D
 * @versi 1.0
 * @tanggal 2025-10-4
 * 
 * Fitur:
 * - Kisaran tekanan 0-40 kPa (lembar datasheet)
 * - Perhitungan laju aliran Venturi
 * - Perhitungan Tekanan aliran Venturi
 * 
 * Referensi:
 * 1. Lembar Data MPS20N0040D-S (e-radionica.com)
 * 2. Lembar Data HX710B (AVIA Semiconductor)
 * 
 */

#ifndef MPS20N0040D_H
#define MPS20N0040D_H
// Ini adalah bagian "guard" yang mencegah file di-include lebih dari sekali dalam program

#include <Arduino.h>
// Mengimpor library Arduino untuk menggunakan fungsi-fungsi dasar Arduino

class MPS20N0040D {
  // Definisi kelas untuk sensor MPS20N0040D
  
  public:
    // Bagian yang bisa diakses oleh pengguna library
    
    // Konstruktor untuk membuat objek sensor baru dengan menentukan pin yang digunakan
    MPS20N0040D(uint8_t pdSckPin, uint8_t doutPin);
    
    // Fungsi untuk memulai komunikasi dengan sensor
    void begin();
    
    // Fungsi untuk membaca nilai tekanan dalam satuan Pascal
    float readPressure();
    
    // Fungsi untuk menghitung laju aliran air berdasarkan perbedaan tekanan
    // Parameter density (kerapatan) memiliki nilai default 1000.0 kg/m³ (air)
    float calculateFlowRate(float pressureDiff, float density = 1000.0);
    
  private:
    // Bagian yang hanya bisa diakses di dalam kelas ini
    
    uint8_t *pdSckPin, *doutPin; 
    // Pointer untuk menyimpan nomor pin yang digunakan (ada kesalahan di sini, seharusnya bukan pointer)
    
    // Fungsi internal untuk membaca data mentah dari chip ADC HX710
    long readADC();
    
    // Fungsi untuk mengkonversi nilai ADC menjadi tegangan listrik
    float adcToVoltage(long adcValue);
    
    // Fungsi untuk mengkonversi tegangan listrik menjadi nilai tekanan
    float voltageToPressure(float voltage);
    
    // Konstanta untuk kalibrasi sensor berdasarkan datasheet
    const float V_REF = 5.0;       // Tegangan referensi untuk ADC HX710 (5 volt)
    const float FS_OUTPUT = 0.1;   // Output maksimum sensor adalah 100mV (0.1V)
    const float PRESSURE_RANGE = 40000.0; // Rentang tekanan sensor 0-40 kPa (40000 Pascal)
};

#endif
// Akhir dari guard kondisi
