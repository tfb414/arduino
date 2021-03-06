#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "secrets/motion_sensor_s3cr3ts.h"


const int garageDoorPin = 21;
const int lightSensorPin = 34;

int one_minute = 2000;

int minute_threshold_to_alert = 1;
int threshold_to_alert = one_minute * 5;
int threshold_health_check = one_minute * 30;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char twilio_bearer_token[] = TWILIO_BEARER_TOKEN;
char twilio_account_sid[] = TWILIO_ACCOUNT_SID;

char twilio_number[] = "+12563339288";
char my_number[] = MY_NUMBER;

const size_t capacity = JSON_OBJECT_SIZE(2) + 65;
DynamicJsonDocument homeStatus(capacity);
String hostIp;


bool garageDoorClosed;
bool garageLightOff;

void connectToWifi()
{
  Serial.println(F("trying to connect"));
  WiFi.begin(ssid, pass);
}

void sendTextMessage(String message, String toNumber, String fromNumber)
{
    HTTPClient http;
    String accountSid = twilio_account_sid;

    String url = "https://api.twilio.com/2010-04-01/Accounts/" + accountSid + "/Messages";

    http.begin(url);

    http.addHeader("Authorization", twilio_bearer_token);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.POST("From=" + fromNumber + "&To=" + toNumber + "&Body=" + message);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
    }
    http.end();
}

String BoolToString(bool b)
{
  return b ? "true" : "false";
}

void updateGarageStatus(bool garageDoorClosedParam, bool garageLightOffParam)
{
  String garageDoorParam = BoolToString(garageDoorClosedParam);
  String garageLightParam = BoolToString(garageLightOffParam);
  
  DynamicJsonDocument responseJson(capacity);
  Serial.println(F("updateGarageStatus"));
  HTTPClient http;
  String url = "http://" + hostIp + ":3000/garage";

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST("garageDoorClosed=" + garageDoorParam + "&garageLightOff=" + garageLightParam);
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.print(F("Response code: "));
    Serial.println(httpResponseCode);
    Serial.print(F("Response: "));
    Serial.println(response);
    
    DeserializationError error = deserializeJson(responseJson, response);
    if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
    }
    homeStatus = responseJson;

  }
  http.end();

}

void getGarageStatus()
{

  DynamicJsonDocument responseJson(capacity);
  Serial.println(F("getGarageStatus"));
  HTTPClient http;
  String url = "http://" + hostIp + ":3000/garageStatus";

  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.GET();
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.print(F("Response code: "));
    Serial.println(httpResponseCode);
    Serial.print(F("Response: "));
    Serial.println(response);

    DeserializationError error = deserializeJson(responseJson, response);
    if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
    
    homeStatus = responseJson;

  }
  http.end();

}

void sendAlert()
{
  Serial.println("sendAlert Called");
  HTTPClient http;
  String url = "http://" + hostIp + ":3000/garageAlert";

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST("");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.print(F("Response code: "));
    Serial.println(httpResponseCode);
    Serial.print(F("Response: "));
    Serial.println(response);

  }
  http.end();

}

void updateHealthCheck()
{
  Serial.println("updateHealthCheck Called");
  HTTPClient http;
  String url = "http://" + hostIp + ":3000/garageHealthCheck";

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST("");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.print(F("Response code: "));
    Serial.println(httpResponseCode);
    Serial.print(F("Response: "));
    Serial.println(response);

  }
  http.end();

}

void getIpAddress()
{

  DynamicJsonDocument responseJson(capacity);
  Serial.println(F("getIpAddress"));
  HTTPClient http;
  String url = "https://raw.githubusercontent.com/tfb414/arduino/master/ip-local.json";

  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.GET();
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    DynamicJsonDocument ipAddresses(capacity);
    Serial.print(F("Response code: "));
    Serial.println(httpResponseCode);
    Serial.print(F("Response: "));
    deserializeJson(ipAddresses, response);
    Serial.println(ipAddresses["homeAssistantStatus"].as<String>());

//I'm commenting this out because it's not able to deserialize probably because of the above code
//    DeserializationError error = deserializeJson(responseJson, response);
//    if (error) {
//    Serial.print(F("deserializeJson() failed: "));
//    Serial.println(error.c_str());
//    return;
//  }
    
    hostIp = ipAddresses["homeAssistantStatus"].as<String>();

  }
  http.end();

}

void setup()
{
  Serial.begin(9600);
  pinMode(garageDoorPin, INPUT_PULLUP);

  while (WiFi.status() != WL_CONNECTED)
    {
        connectToWifi();
        Serial.print(F("."));
        delay(3000);
    }
  Serial.println(F("connected"));
  
  getIpAddress();
  getGarageStatus();
  
}
 
void loop(){
  threshold_health_check -= 1;
  //For some reason if I don't print this the value goes to 1 instead of subracting 1. I know it's crazy
//  Serial.println(threshold_health_check);
//  Serial.println(threshold_to_alert);
  
  int garageDoorSense = digitalRead(garageDoorPin);
  int lightSense = analogRead(lightSensorPin);

  bool newGarageDoorClosed = garageDoorSense == 0; 
  bool newGarageLightOff = lightSense < 500;
 
  if (newGarageDoorClosed != homeStatus["garageDoorClosed"] || newGarageLightOff != homeStatus["garageLightOff"]) {
    updateGarageStatus(newGarageDoorClosed, newGarageLightOff);
    getGarageStatus();
  }
//  Serial.println("garageDoorClosed");
//  Serial.println(newGarageDoorClosed);
  if(newGarageDoorClosed == false) {
    threshold_to_alert = threshold_to_alert - 1;
    Serial.print(F("threshold countdown: "));
    Serial.println(threshold_to_alert);
  }
  if(newGarageDoorClosed == true) {
    threshold_to_alert = one_minute * 5;
    Serial.println(threshold_to_alert);
  }
//  else {
//    threshold_to_alert = one_minute * 10;
//    Serial.print(F("Threshold has been reset: "));
//    Serial.println(threshold_to_alert);  
//  }

  if(threshold_to_alert <= 0) {
    threshold_to_alert = one_minute * 30;
    String message = "Garage door is open";
    sendTextMessage(message, my_number, twilio_number);
    sendAlert();
  }

  if(threshold_health_check <= 0) {
    Serial.print(F("HealthCheck sent: "));
      threshold_health_check = one_minute * 30;
      updateHealthCheck();
  }
}
