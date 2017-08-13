#include <SocketIOClient.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const int dht_pin = 2;
const int dhttype = DHT11;
DHT dht(dht_pin, dhttype);

SocketIOClient client;
const char* ssid = "Family";
const char* password = "bat3glendi";
char host[] = "192.168.1.3";
int port = 8080;
// char namespace_esp8266[] = "esp8266";

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

  if (!client.connect(hostname, port))
   {Serial.println(F("Not connected."));}
  if (client.connected())
  {
    client.send("connection", "message");
  }
  else
  {
    Serial.println("Connection Error");
    while(1);
  }
  delay(1000);
}

void loop() {
  delay(2000);
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
  sensor();
} 

void sensor(){
  StaticJsonBuffer<200> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  root["humidity"] = h;
  root["temperature"] = t;
  root.printTo(Serial);
  String json;
  const char * data = "DATA CHECKED";
  root.printTo(json);
  
  client.sendJSON("received_data",json);
}
