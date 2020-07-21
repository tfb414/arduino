/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <Servo.h>
Servo myservo;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(2, 3); // CE, CSN
const byte address[6] = "00001";



void setup() {
  Serial.begin(9600);

  myservo.attach(6);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    int command;
    radio.read(&command, sizeof(command));
    if (command == 0) {
      Serial.println("serial == 0");
    }

    if (command == 1) {
      Serial.println("Command == 1");
    }
//    Serial.println(command == 2);
//    Serial.print("2");
//    if (command == 1) {
//      myservo.write(150);
//    }
//    if (command == 0) {
//      myservo.write(30);
//    }
  }
}
