#include <Arduino.h>
#include <DHT.h>
#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 oled(OLED_RESET);


#define _SSID "Galaxy10"          // Your WiFi SSID
#define _PASSWORD "12345679"      // Your WiFi Password
#define REFERENCE_URL "https://thuctapiot-53e65-default-rtdb.firebaseio.com"  // Your Firebase project reference url

Firebase firebase(REFERENCE_URL);

#define LED_BUILTIN  2
#define DHTPIN  D4    
#define DHTTYPE DHT11   

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

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(10);
  oled.clearDisplay();
  delay(10);
  oled.display();
  delay(10);

  WiFi.begin(_SSID, _PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0,0);
  oled.println("Connecting");
  oled.println("to Wi-Fi");
  oled.display();
  delay(10);

  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
  }
  oled.clearDisplay();
  delay(10);

  dht.begin();
  Serial.println();
  Serial.print("Connected with IP: ");
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0,0);
  oled.println("Connected with IP: ");
  oled.println(WiFi.localIP());
  oled.display();
  delay(10);
  Serial.println(WiFi.localIP());
  Serial.println();

  delay(1000);
  oled.clearDisplay();
  delay(10);

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
    oled.clearDisplay();
    
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(0,0);
    oled.println("Nhiet do:");
    oled.print(t);oled.println("*C");
    oled.println("Do am:");
    oled.print(h);oled.println("%");
    oled.display();
    

    Serial.printf("Nhiet do: %f°C \n", t);
    firebase.setFloat("clock1/Nhietdo", t);
    Serial.printf("Do am: %f \n", h);
    firebase.setFloat("clock1/Doam", h);
}
}