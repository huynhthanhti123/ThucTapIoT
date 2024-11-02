#include <Arduino.h>
#include <DHT.h>
//#include <ESP32Firebase.h>
#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>

#define _SSID "Galaxy10"          // Your WiFi SSID
#define _PASSWORD "12345679"      // Your WiFi Password
#define REFERENCE_URL "https://thuctapiot-53e65-default-rtdb.firebaseio.com"  // Your Firebase project reference url

Firebase firebase(REFERENCE_URL);

#define LED_BUILTIN  2
#define DHTPIN  D5    // Chân dữ liệu của DHT 11 , với NodeMCU chân D5 GPIO là 14
#define DHTTYPE DHT11   // DHT 11
 
DHT dht(DHTPIN, DHTTYPE);

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

void setup()
{
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  WiFi.begin(_SSID, _PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print(".");
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
  }
  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  dht.begin();
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    return;
  }

  if (millis() - sendDataPrevMillis > 2000 )
  {
    sendDataPrevMillis = millis();
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.printf("Nhiet do: %f°C \n", t);
    firebase.setFloat("clock3/Nhietdo", t);
    Serial.printf("Do am: %f \n", h);
    firebase.setFloat("clock3/Doam", h);
    digitalWrite(LED_BUILTIN, LOW); 
  }
}