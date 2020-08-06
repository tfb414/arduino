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

char garageDoorClosed;
char garageLightOff;

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
//    garageDoorClosed = homeStatus["garageDoorClosed"];
//    garageLightOff = homeStatus["garageLightOff"];
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

    DeserializationError error = deserializeJson(responseJson, response);
    if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
    homeStatus = responseJson;
    const char garageDoorClosed = homeStatus["garageDoorClosed"];
    const char garageLightOff = homeStatus["garageLightOff"];

    Serial.print(garageDoorClosed);
    Serial.println("HEY OVER HERE");

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
//    const char* test = homeStatus["garageDoor"]

//   const char* garageDoor = homeStatus["garageDoor"];
//   const char* garageLight = homeStatus["garageLight"];
//   Serial.print(garageDoor);
//   Serial.print(garageLight);

//  updateGarageStatus("true", "false");
  garageDoorClosed = homeStatus["garageDoorClosed"];
  garageLightOff = homeStatus["garageLightOff"];
  Serial.print(garageDoorClosed);
  Serial.print(garageLightOff);
}
 
void loop(){
  int garageDoorSense = digitalRead(garageDoorPin);
  int lightSense = analogRead(lightSensorPin);

//  bool garageDoorClosed = garageDoorSense < 0; // true === closed
//  bool garageLightOff = lightSense < 500; // true === off 


  garageDoorClosed = homeStatus["garageDoorClosed"];
  garageLightOff = homeStatus["garageLightOff"];  

   bool garageDoorClosedLocal = true;
   bool garageLightOffLocal = true;

   Serial.print("garageDoorClosed");
  Serial.println(garageDoorClosed);

  Serial.print("garageLightOff");
  Serial.println(garageLightOff);


//  bool testGarage = garageDoorClosed != homeStatus["garageDoorClosed"];
//  bool testLight = garageLightOff != homeStatus["garageLightOff"];
//
//  Serial.print("boolean");
//  Serial.println(testGarage);
//
//  Serial.print("current light value");
//  Serial.println(testLight);
//  
//  
//  if (garageDoorClosed != homeStatus["garageDoorClosed"] || garageLightOff != homeStatus["garageLightOff"]) {
//    getGarageStatus();
//
//    updateGarageStatus("true", "true");
//  }
}
