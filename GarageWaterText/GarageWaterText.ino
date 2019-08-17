
#include "secrets/garage_water_text_s3cr3ts.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "SSD1306.h"

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define uS_TO_M_FACTOR 60      /* Convert from seconds to minutes */
#define TIME_TO_SLEEP 5

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char from_number[] = SECRET_TWILIO_NUMBER;
char my_number[] = SECRET_MY_NUMBER;

char twilio_bearer_token[] = SECRET_TWILIO_TOKEN;
char twilio_account_sid[] = TWILIO_ACCOUNT_SID;

int threshold = 1000;

SSD1306 display(0x3c, 5, 4);

bool checkForWater(int threshold)
{
    Serial.print("water level: ");
    int sensePin = 39;
    int waterSensorValue = 0;
    waterSensorValue = analogRead(sensePin);
    Serial.println(waterSensorValue);
    if (waterSensorValue > threshold && waterSensorValue != 4095)
    {
        return true;
    }
    return false;
}

void connectToWifi()
{
    Serial.println("trying to connect");
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

void goToSleep()
{
    Serial.println("Going to sleep");
    delay(500);
    esp_sleep_enable_timer_wakeup(uS_TO_S_FACTOR * uS_TO_M_FACTOR * 30);
    esp_deep_sleep_start();
    delay(1000);
}

void setup()
{
    Serial.begin(9600);
    display.init();
    display.setContrast(255);
    display.clear();
}

void loop()
{
    String toNumber = my_number;
    String fromNumber = from_number;
    bool isWaterPresent = checkForWater(threshold);
    Serial.println("ARE WE CONNECTED:");
    Serial.println(WiFi.status());
    if (isWaterPresent)
    {
        while (WiFi.status() != WL_CONNECTED)
        {
            connectToWifi();
            delay(3000);
        }
        Serial.println("sending text message");
        sendTextMessage("There is water in your garage", toNumber, fromNumber);
        delay(5000);
    }
    goToSleep();
    delay(3000);
}
