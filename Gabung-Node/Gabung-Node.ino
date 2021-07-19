#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
SoftwareSerial nodeSerial(D6, D5);
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;

char ssid[]            = ""//ISI SSID WIFI;
char pass[]            = "";//ISI PASSWOR WIFI
char auth[]            = "";//ISI Blynk AUTH Token
char server[]          = "blynk-cloud.com";
unsigned int port      = 8442;

void sendSensor()
{
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(nodeSerial);
  if (root == JsonObject::invalid()) {
    return;
  }
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.println("");
  Serial.print("Air Quality = ");
  float data1 = root["airq"];
  Serial.print(data1);
  Serial.println(" PPM");
  Serial.print(" Smoke = ");
  float data2 = root["smoke"];
  Serial.print(data2);
  Serial.println(" PPM");
  Serial.print(" CO = ");
  float data3 = root["co"];
  Serial.print(data3);
  Serial.println(" PPM");
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
  Blynk.virtualWrite(V0, data1);
  Blynk.virtualWrite(V1, data2);
  Blynk.virtualWrite(V2, data3);
}

void setup() {
  // Initialize Serial port
  Serial.begin(115200);
  nodeSerial.begin(9600);
  Blynk.connectWiFi(ssid, pass);
  Blynk.config(auth, server, port);
  Blynk.connect();
  timer.setInterval(2000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
