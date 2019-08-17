#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "secrets/motion_sensor_s3cr3ts.h"

#define timeSeconds 10

const int sensePin = 36;
const int ledPin = 21;

unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char homeAssistantId[] = SECRET_HOME_ASSISTANT_ID;

void connectToWifi()
{
  Serial.println("trying to connect");
  WiFi.begin(ssid, pass);
}

void toggleOnLight()
{
  HTTPClient http;
  String url = "http://192.168.1.69:3000/motionSensorActivated";

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

void IRAM_ATTR detectsMovement()
{
  Serial.println("MOTION DETECTED!!!");
  digitalWrite(ledPin, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

void setup()
{
  Serial.begin(9600);
  pinMode(sensePin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(sensePin), detectsMovement, RISING);
  //  pinMode(sensePin, INPUT);

  //  while (WiFi.status() != WL_CONNECTED)
  //    {
  //      connectToWifi();
  //      delay(2000);
  //    }
  //  Serial.print("Connected at: ");
  //  Serial.println(WiFi.localIP());
}

void loop()
{
  now = millis();
  if (startTimer && (now - lastTrigger > (timeSeconds * 1000)))
  {
    Serial.println("Motion stopped...");
    digitalWrite(ledPin, LOW);
    startTimer = false;
  }
  //  if(WiFi.status() == WL_CONNECTED){

  //  int motionSensorValue = analogRead(sensePin);
  //  Serial.println(motionSensorValue);
  //  if (motionSensorValue > 0 )
  //  {
  //    digitalWrite(ledPin, HIGH);
  //        delay(5000);
  //        toggleOnLight();
  //        digitalWrite(ledPin, LOW);
  //    Serial.println("just sent request");
  //        delay(5000);
  //  }
  //  else
  //  {
  //    digitalWrite(ledPin, LOW);
  //  }
  //    }
}
