const int buttonPin = 2;    
const int ledPin =  4;      

int buttonState = 0;
int ledState = LOW;
byte readInterval = 200;
unsigned long lastReadTime;

void toggleLedState() {
    if (ledState == HIGH) {
      ledState = LOW;
    } else {
        ledState = HIGH;
    }
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

//this really needs a debouncer
void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH && (millis() - lastReadTime) > readInterval) {
    lastReadTime = millis();
    toggleLedState();
  }

  if (ledState == HIGH) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
