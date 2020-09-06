#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "secrets/motion_sensor_s3cr3ts.h"

const int garageDoorPin = 21;
const int lightSensorPin = 34;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const size_t capacity = JSON_OBJECT_SIZE(2) + 65;
DynamicJsonDocument homeStatus(capacity);

bool garageDoorClosed;
bool garageLightOff;

void connectToWifi()
{
  Serial.println("trying to connect");
  WiFi.begin(ssid, pass);
}

void updateGarageStatus(String garageDoorClosedParam, String garageLightOffParam)
{
  DynamicJsonDocument responseJson(capacity);
  Serial.println("updateGarageStatus");
  HTTPClient http;
  String url = "http://10.0.0.14:3000/garage";

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST("garageDoorClosed=" + garageDoorClosedParam + "&garageLightOff=" + garageLightOffParam);
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("REsponse: ");
    Serial.println(response);
    
    DeserializationError error = deserializeJson(responseJson, response);
    homeStatus = responseJson;
    garageDoorClosed = homeStatus["garageDoorClosed"];
    garageLightOff = homeStatus["garageLightOff"];
  }
  http.end();

}

void getGarageStatus()
{

  DynamicJsonDocument responseJson(capacity);
  Serial.println("getGarageStatus");
  HTTPClient http;
  String url = "http://10.0.0.14:3000/status";

  http.begin(url);
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

    DeserializationError error = deserializeJson(responseJson, response);
    if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
    
    homeStatus = responseJson;
//    const bool garageDoorClosed = homeStatus["garageDoorClosed"];
//    const bool garageLightOff = homeStatus["garageLightOff"];
//
//    Serial.print(garageDoorClosed);
//    Serial.println("HEY OVER HERE");
//    Serial.println(garageLightOff);
//    Serial.println("did that work?");

  }
  http.end();

}


void setup()
{
  Serial.begin(9600);
  pinMode(garageDoorPin, INPUT);

  while (WiFi.status() != WL_CONNECTED)
      {
        connectToWifi();
        delay(3000);
      }
  Serial.println("connected");

  getGarageStatus();
    
  garageDoorClosed = homeStatus["garageDoorClosed"];
  garageLightOff = homeStatus["garageLightOff"];
  Serial.print("garageDoorClosed: ");
  Serial.println(garageDoorClosed);
  Serial.print("garageLightOff: ");
  Serial.println(garageLightOff);
  
}
 
void loop(){
  int garageDoorSense = digitalRead(garageDoorPin);
  int lightSense = analogRead(lightSensorPin);

  bool newGarageDoorClosed = garageDoorSense < 0; 
  bool newGarageLightOff = lightSense < 500;

 
  if (newGarageDoorClosed != homeStatus["garageDoorClosed"] || newGarageLightOff != homeStatus["garageLightOff"]) {
    getGarageStatus();

    updateGarageStatus("true", "true");
    getGarageStatus();
    
  }
}
