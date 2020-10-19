

const int led = 19;
const int jumperPin = 21;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led, OUTPUT);
  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(led, HIGH);
  delay(10000);
  pinMode(jumperPin, OUTPUT);
  digitalWrite(jumperPin, HIGH);
  

}
