  #include <WiFi.h>
  #include <HTTPClient.h>
  
  int one_minute = 800;

  int minute_threshold_to_alert = 1;
  int threshold_to_alert = one_minute * 8;
  
  int photocellPin = 34;
  int photocellReading;
  bool lightStatus;
  int contactSensorPin = 21;
  int contactSensorReading;
  bool garageDoorStatus;
  bool lastGarageDoorStatus;
  bool lastLightStatus;

  char ssid[] = "Spruce Wayne";
  char pass[] = "shady lane";

  char twilio_bearer_token[] = "Basic QUNlMzgxMmIzMWJkOWI2ZTRmMDExMGI4YzhjZDJkZTUxZDo2NzE5Mzk4NTYzZGYzYjFiZWE4MjVjOGFjYmRjY2MxNQ==";
  char twilio_account_sid[] = "ACe3812b31bd9b6e4f0110b8c8cd2de51d";

  char twilio_number[] = "+12562425358";
  char my_number[] = "+12566981624";

 
void connectToWifi()
{
  Serial.println("trying to connect to network");
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
  return b == 1 ? "true" : "false";
}

void updateGarageStatus(bool garageDoor, bool garageLight)
{
  Serial.println("updateGarageStatus");
  HTTPClient http;
  String url = "http://10.0.0.14:3000/garage";

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  Serial.print(BoolToString(garageDoor));
  Serial.println("CHECK ABOVE");
  String requestBody = "garageLight=" + BoolToString(garageLight) + "&garageDoor=" + BoolToString(garageDoor);

  Serial.print(requestBody);
  int httpResponseCode = http.POST(requestBody);
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

String get_message(bool lightStatus, bool garageDoorStatus) {
    if(lightStatus == true && garageDoorStatus == true) 
    {
        return "Garage door is open and garage light is on";
    }
    if(lightStatus == true) 
    {
      return "Garage light has been left on";
    }
    return "Garage door has been left open";
  
  }

void setup() {
  Serial.begin(9600);
  pinMode(photocellPin, INPUT);
  pinMode(contactSensorPin, INPUT);

  while (WiFi.status() != WL_CONNECTED)
      {
        connectToWifi();
        delay(3000);
      }
     Serial.println("connected");
}

void loop() {
  photocellReading = analogRead(photocellPin);
  contactSensorReading = digitalRead(contactSensorPin);
  lightStatus = photocellReading < 30;
  garageDoorStatus = contactSensorReading < 1;
 
  Serial.print("light is:  ");
  Serial.println(photocellReading);
 
  Serial.print("Garage Door is:    ");
  Serial.println(contactSensorReading);

  if (lastGarageDoorStatus != garageDoorStatus || lastLightStatus != lightStatus)
  {
    lastGarageDoorStatus = garageDoorStatus;
    lastLightStatus = lightStatus;

    Serial.print("light status");
    Serial.println(lightStatus);

    Serial.print("garageDoorStatus");
    Serial.println(garageDoorStatus);
    updateGarageStatus(garageDoorStatus, lightStatus);
  }

  if (lightStatus == false || garageDoorStatus == false) 
  {
    threshold_to_alert = threshold_to_alert - 1;
    Serial.print("threshold countdown:     ");
    Serial.println(threshold_to_alert);
  }
  else 
  {
    threshold_to_alert = one_minute * 10;
    Serial.print("Threshold has been reset:      ");
    Serial.println(threshold_to_alert);  
  }

  if(threshold_to_alert <= 0) {
    threshold_to_alert = one_minute * 30;
    String message = get_message(lightStatus, garageDoorStatus);
    updateGarageStatus(garageDoorStatus, lightStatus);
    sendTextMessage(message, my_number, twilio_number);
    }
  
}
