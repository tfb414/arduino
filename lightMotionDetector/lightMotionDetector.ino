#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "secrets/motion_sensor_s3cr3ts.h"

const int sensePin = 36;
const int motionDetectedLedPin = 26;
const int activeLedPin = 33;
const int pResistorPin = 32;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char homeAssistantId[] = SECRET_HOME_ASSISTANT_ID;

int pirState = LOW;
int pirValue = 0;   
int pResistorValue;
int pResistorThreshold = 2000;

void connectToWifi()
{
  Serial.println("trying to connect");
  WiFi.begin(ssid, pass);
}

void toggleOnLight()
{
  Serial.println("toggleOnLight");
  HTTPClient http;
  String url = "http://192.168.1.69:3000/status";

  http.begin(url);
  http.addHeader("x-ha-access", homeAssistantId);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.GET();
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("REsponse: ");
    Serial.println(response);
  }
  http.end();
}

void setup()
{
  Serial.begin(9600);
  pinMode(sensePin, INPUT);
  pinMode(motionDetectedLedPin, OUTPUT);
  pinMode(activeLedPin, OUTPUT);
  pinMode(pResistorPin, INPUT);

  while (WiFi.status() != WL_CONNECTED)
      {
        connectToWifi();
        delay(3000);
      }
     Serial.println("connected");
   toggleOnLight();
}
 
void loop(){
  pirValue = LOW;
  pResistorValue = analogRead(pResistorPin);
  
  if (pResistorValue > pResistorThreshold) {
    pirValue = digitalRead(sensePin);
    digitalWrite(activeLedPin, HIGH);
  } else {
    digitalWrite(activeLedPin, LOW);
    }
  
  if (pirValue == HIGH) {            
    digitalWrite(motionDetectedLedPin, HIGH);  
    if (pirState == LOW) {
      toggleOnLight();
      Serial.println("Motion detected!");
      pirState = HIGH;
    }
  } else {
    digitalWrite(motionDetectedLedPin, LOW); 
    if (pirState == HIGH){
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
}
