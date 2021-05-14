#include "secrets/motion_sensor_s3cr3ts.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>


const size_t capacity = JSON_OBJECT_SIZE(2) + 65;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

byte sensorInterrupt = 4; 
byte sensorPin       = 4;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;  

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTotalMilliLitres;
String tml;

unsigned long oldTime;
int countdown;

void connectToWifi()
{
  Serial.println(F("trying to connect"));
  WiFi.begin(ssid, pass);
}

void updateKegVolume(String kegVolume, int kegNumber)
{  
  DynamicJsonDocument responseJson(capacity);
//  Serial.println(F("updateKegVolume"));
  HTTPClient http;
  String url = "http://10.0.0.100:3000/kegVolume";

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST("kegVolume=" + kegVolume);
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
//    Serial.print(F("Response code: "));
//    Serial.println(httpResponseCode);
//    Serial.print(F("Response: "));
//    Serial.println(response);
    
    DeserializationError error = deserializeJson(responseJson, response);
    if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
    }

  }
  http.end();

}

void getFlowRate()
{
    if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorInterrupt);
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
      
    unsigned int frac;
    
    // Print the flow rate for this second in litres / minute
//    Serial.print("Flow rate: ");
//    Serial.print(int(flowRate));  // Print the integer part of the variable
//    Serial.print("L/min");
//    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
//    Serial.print("Output Liquid Quantity: ");        
//    Serial.print(totalMilliLitres);
//    Serial.println("mL"); 
//    Serial.print("\t");       // Print tab space
//    Serial.print(totalMilliLitres/1000);
//    Serial.print("L");
    

    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

  }
}


void setup()
{
  Serial.begin(9600);

 while (WiFi.status() != WL_CONNECTED)
  {
      connectToWifi();
      Serial.print(F("."));
      delay(3000);
  }
//  Serial.println(F("connected"));

  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
  countdown         = -1;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)

  // YOU MIGHT NEED TO CHANGE THIS BACK TO FALLING
  attachInterrupt(sensorPin, pulseCounter, FALLING);
}

/**
 * Main program loop
 */
void loop()
{

  oldTotalMilliLitres = totalMilliLitres;
  getFlowRate();
  Serial.println(countdown);
  if(oldTotalMilliLitres != totalMilliLitres) {
    countdown = 1000;
    tml = String(totalMilliLitres);
//    Serial.println(tml);
  }

  if(countdown == 0) {
        updateKegVolume(tml, 1);
//        Serial.println("Countdown done");
//        Serial.println("TML" + tml);
    }

  if(countdown > -1) 
  {
    
    countdown --;
  }
  
  
  
}

/*
Interrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
