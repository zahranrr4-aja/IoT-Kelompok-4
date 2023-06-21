int SensorPin = 32;// deklarasi pin analog yg dipakai
int soilMoistureValue; // menyimpan nilai analog dari sensor ke esp32
int soilmoisturepercent; // nilai yg diperoleh dalam bentuk persen setelah dimaping

void setup() {
  Serial.begin(115200); // Baudrate komunikasi dengan serial monitor
}

void loop() {
      soilMoistureValue = analogRead(SensorPin);
        Serial.print("Nilai analog = ");
          Serial.print(soilMoistureValue);
           soilmoisturepercent = map(soilMoistureValue, 4095, 0, 0, 100);

        Serial.print(" Presentase kelembaban tanah= ");
       Serial.print(soilmoisturepercent);
      Serial.println("% ");


delay(500);
}