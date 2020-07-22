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

void connectToWifi()
{
  Serial.println("trying to connect");
  WiFi.begin(ssid, pass);
}

//void updateGarageStatus(String garageDoor, String garageLight)
//{
//  Serial.println("updateGarageStatus");
//  HTTPClient http;
//  String url = "http://localhost:3000/garageStatus";
//
//  http.begin(url);
//  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
//
//  int httpResponseCode = http.POST("garageDoor=" + garageDoor, "&garageStatus=" + garageLight);
//  Serial.println(httpResponseCode);
//
//  if (httpResponseCode > 0)
//  {
//    String response = http.getString();
//    Serial.print("Response code: ");
//    Serial.println(httpResponseCode);
//    Serial.print("REsponse: ");
//    Serial.println(response);
//  }
//  http.end();
//}

void getGarageStatus()
{

  DynamicJsonDocument responseJson(capacity);
  Serial.println("getGarageStatus");
  HTTPClient http;
  String url = "http://10.0.0.14:3000/stringStatus";

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
    homeStatus = responseJson;

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
//    const char* test = homeStatus["garageDoor"]

   const char* garageDoor = homeStatus["garageDoor"];
   const char* garageLight = homeStatus["garageLight"];
   Serial.print(garageDoor);
//   Serial.print(garageLight);
}
 
void loop(){
//  int garageDoor = digitalRead(garageDoorPin);
//  int light = analogRead(lightSensorPin);
//  
//  
//// get the status and check if these status' are any differnet if so then post
//
//  if (light > 500 ) 
//  {
//    Serial.println("light is on");
//    Serial.println(light);
//  }
//
//  if(garageDoor > 0) 
//  {
//    
//    Serial.println("garage is open");
//  }
//
//  if(garageDoor < 1) 
//  {
//    
//    Serial.println("garage is closed");
//  }

}
