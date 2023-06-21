#define mq2Pin 32   // Pin analog untuk sensor MQ-2
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  pinMode(mq2Pin , INPUT);
}

void loop() {
  int sensorValue = analogRead(mq2Pin); // Membaca nilai sensor analog

  float voltage = sensorValue * (5.0 / 1023.0); // Mengonversi nilai sensor menjadi tegangan (5V adalah tegangan referensi Arduino)
  
  // Menghitung konsentrasi gas menggunakan rumus yang sesuai dengan sensor MQ-2
  float gasResistance = ((5.0 - voltage) / voltage) * 10.0; // Menggunakan faktor 10.0 untuk mengkoreksi nilai
  
  // Menampilkan hasil ke Serial Monitor
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  delay(2000);
  Serial.print("Gas Resistance: ");
  Serial.print(gasResistance);
  Serial.println(" KΩ");
  delay(2000);

}