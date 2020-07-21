#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int leftButtonPin = 5;  
const int rightButtonPin = 4;  

int leftButtonState = 0;
int rightButtonState = 0;

RF24 radio(2, 3); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);

  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
  
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  leftButtonState = digitalRead(leftButtonPin);
  rightButtonState = digitalRead(rightButtonPin);
  
  int command;


  if (leftButtonState == HIGH) {
    Serial.println("left");
    command = 0;
  }
   if (rightButtonState == HIGH) {
    Serial.println("right");
    command = 1;
  }

  radio.write(command, sizeof(command));
  Serial.print("still running");
  command = 3;
  Serial.println(command);

}
