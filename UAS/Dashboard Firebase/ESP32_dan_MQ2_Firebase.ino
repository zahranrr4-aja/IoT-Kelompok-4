#include <FirebaseESP32.h>
#include <WiFi.h>

#define FIREBASE_HOST "https://node-mq-2-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "4uBddN3Kxe8umvdg4qfPsF15WcAcuYK9YJD7BeZV"

#define WIFI_SSID "Zahran"
#define WIFI_PASSWORD "Zahran007"

FirebaseData fbdo; //fbdo adalah variabel. 

#define PinDigital 4 // mendefinisikan pin yang digunakan adalah pin Digital
int NilaiDigital;

void setup() {
  Serial.begin(9600);
  pinMode(PinDigital, INPUT); //mode pada pin D4 dijadikan sebagai input
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("terhubung dengan WiFi IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  NilaiDigital = digitalRead(PinDigital); // membaca nilai digital

  Serial.print("Nilai Output Digital = ");
  Serial.println(NilaiDigital);
  //Aktif LOW = Jika Ada Asap Maka Nilai nya 0 jika tidak ada Asap maka nilai Nya 1

  //Proses Kirim Data
  Firebase.setFloat(fbdo, "/Nilai_Asap", NilaiDigital);
  
  if(NilaiDigital==0){
    Firebase.setString(fbdo, "/Kondisi", "Ada Asap");
  } else {
    Firebase.setString(fbdo, "/Kondisi", "Aman");
  }
  
  delay(1000);
}
