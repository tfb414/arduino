  int photocellPin = 27;
  int photocellReading;
  bool lightStatus;
  int contactSensorPin = 26;
  int contactSensorReading;
  bool garageDoorStatus;

void setup() {
  Serial.begin(9600);
}

void loop() {
  photocellReading = analogRead(photocellPin);
  contactSensorReading = analogRead(contactSensorPin);
  lightStatus = photocellReading > 1000;
  garageDoorStatus = contactSensorReading < 1000;

  Serial.print("light is:  ");
  Serial.println(lightStatus);
  Serial.print("Garage Door is:    ");
  Serial.println(contactSensorReading);
}
