#include <ESP32Firebase.h>
#include "DHT.h"
#include <WiFi.h> 

#define DATABASE_URL "https://thuctapiot-53e65-default-rtdb.firebaseio.com/" // Url của Firebase Realtime Database
#define WIFI_SSID "Galaxy10"          // Your WiFi SSID
#define WIFI_PASSWORD "12345679"      // Your WiFi Password
#define DHTTYPE DHT22 // Định nghĩa loại cảm biến DHT22
#define DHTPIN 14 // Chân GPIO kết nối cảm biến DHT
DHT dht(DHTPIN, DHTTYPE); // Khởi tạo cảm biến DHT
//Firebase fbdo;
Firebase firebase(DATABASE_URL);
void setup() {
  Serial.begin(9600);
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Nhiet do: ");
  Serial.print(t);
  Serial.print("*C ");
  Serial.print("Do am: ");
  Serial.print(h);
  Serial.println("% ");
  firebase.setFloat("clock1/Nhietdo", t);
  firebase.setFloat("clock1/Doam", h);
}