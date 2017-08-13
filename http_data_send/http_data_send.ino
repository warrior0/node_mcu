#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const int dht_pin = 2;
const int dhttype = DHT11;
DHT dht(dht_pin, dhttype);

const char* ssid = "Family";
const char* password = "bat3glendi";
float h;
float t;

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(20);

  Serial.print("conneting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("connected to ");
  Serial.println(ssid);
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(3000);
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)){
    Serial.println("failed to read from DHT sensor");
    return;
  }
  Serial.print("nhiet do: ");
  Serial.println(t);
  Serial.print("do am: ");
  Serial.println(h);
  HTTPClient http;
  char *hu;
  char *te;
  dtostrf(h, 4,1, hu);
  dtostrf(t, 4,1, te);
  Serial.println(hu);
  Serial.println(te);
  char * url = ("http://192.168.1.3:80/%f/%f/",hu,te);
  Serial.println(url);

  http.begin(url);
  Serial.println("HTTP begin");
  int httpCode = http.GET();
  if (httpCode > 0){
    String payload = http.getString(); 
      Serial.println(payload);  
    }
    http.end();
  }


