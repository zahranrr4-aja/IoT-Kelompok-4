#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

const char* ssid = "Realme 7";
const char* password = "liverpoolfc150502";

AsyncWebServer server(80);

void setup(void) {

  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  //wait fot connection
  while (WiFi.status()!=WL_CONNECTED){
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to");
  Serial.println(ssid);
  Serial.print("IP addres ");
  Serial.println(WiFi.localIP());

  server.on("/" , HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "ESP32 OTA (Over The Air).");
  });
  AsyncElegantOTA.begin(&server); 
  server.begin();
  Serial.println(" HTTP Server Started");
}
void loop(void){
  AsyncElegantOTA.loop();
  digitalWrite(2, HIGH); 
  delay(1000);
  digitalWrite(2, LOW); 
  delay(1000);
}

  