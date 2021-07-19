#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial nodeSerial(6, 5); //Rx , Tx

#define MQ135_THRESHOLD_1 1000
#define MQ135pin (A0)
#define MQ2pin (A2)
#include "MQ7.h"
MQ7 mq7(A1, 5.0);

int MQ135_data;
float Smoke;  //variable to store sensor value
float co ;

void setup() {
  Serial.begin(115200);                            // sets the serial port to 9600
  nodeSerial.begin(9600);
  Serial.println("Gas sensor warming up!");
  delay(20000); // allow the MQ-6 to warm up
}

void loop() {
  //MQ-135//
  Serial.println("MQ-135");
  MQ135_data = analogRead(MQ135pin);       // read analog input pin 0
  Serial.print("Air Quality=");
  Serial.print(MQ135_data, DEC);               // prints the value read
  Serial.println(" PPM");
  if (MQ135_data < MQ135_THRESHOLD_1) {
    Serial.println("Fresh Air");
  } else {
    Serial.println("Poor Air");
  }
  Serial.println("");
  delay(1000);                                   // wait 100ms for next reading
  //END MQ-135//

  //MQ-2//
  Smoke = analogRead(MQ2pin); // read analog input pin 0
  Serial.println("MQ-2");
  Serial.print("Smoke= ");
  Serial.print(Smoke);
  Serial.println(" PPM");
  Serial.println("");
  delay(2000); // wait 2s for next reading
  //END MQ-2//

  //MQ-7//
  co = mq7.getPPM();
  Serial.println("MQ-7");
  Serial.print("CO= ");
  Serial.print(co);
  Serial.println(" PPM");
  Serial.println("");
  delay(1000);
  //END MQ-7//

  //Sending Data to NodeMCU//
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["airq"] = MQ135_data;
  root["smoke"] = Smoke;
  root["co"] = co;
  root.printTo(nodeSerial);
  //END Sending Data to NodeMCU//
}
