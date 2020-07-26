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

char garageDoor;
char garageLight;

void connectToWifi()
{
  Serial.println("trying to connect");
  WiFi.begin(ssid, pass);
}

void updateGarageStatus(String garageDoorParam, String garageLightParam)
{
  Serial.println("updateGarageStatus");
  HTTPClient http;
  String url = "http://10.0.0.14:3000/garage";

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST("garageDoor=" + garageDoorParam + "&garageStatus=" + garageLightParam);
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
    garageDoor = homeStatus["garageDoor"];
    garageLight = homeStatus["garageLight"];

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
    char garageDoor = homeStatus["garageDoor"];
    char garageLight = homeStatus["garageLight"];
//    const char* test = homeStatus["garageDoor"]

//   const char* garageDoor = homeStatus["garageDoor"];
//   const char* garageLight = homeStatus["garageLight"];
//   Serial.print(garageDoor);
//   Serial.print(garageLight);
}
 
void loop(){
  int garageDoorSense = digitalRead(garageDoorPin);
  int lightSense = analogRead(lightSensorPin);

  bool garageDoorNew = garageDoorSense < 0; // true === closed
  bool garageLightNew = lightSense < 500; // true === off 

  bool testGarage = garageDoorNew != homeStatus["garageDoor"];
  bool testLight = garageLightNew != homeStatus["garageLight"];

  Serial.print("boolean");
  Serial.println(testLight);

  Serial.print("current light value");
  Serial.println(garageLightNew);
  
  
  if (garageDoorNew != homeStatus["garageDoor"] || garageLightNew != homeStatus["garageLight"]) {
    getGarageStatus();

    updateGarageStatus("true", "false");
  }
}
