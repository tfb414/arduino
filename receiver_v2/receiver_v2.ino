
/*
* Getting Started example sketch for nRF24L01+ radios
* This is an example of how to send data from one node to another using data structures
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

byte addresses[][6] = {"1Node","2Node"};


/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(2,3);
/**********************************************************/
Servo myservo;

/**
* Create a data structure for transmitting and receiving data
* This allows many variables to be easily sent and received in a single transmission
* See http://www.cplusplus.com/doc/tutorial/structures/
*/
struct dataStruct{
  unsigned long _micros;
  float value;
  int servo_value;
}myData;

void setup() {

  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted_HandlingData"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  myservo.attach(6);
  myData.value = 1.22;
  myData.servo_value = 0;
  // Start the radio listening for data
  radio.startListening();
}




void loop() {
 

if( radio.available()){
                                                       // Variable for the received timestamp
  while (radio.available()) {                          // While there is data ready
    radio.read( &myData, sizeof(myData) );             // Get the payload
  }
 
  radio.stopListening();                               // First, stop listening so we can talk  
  myData.value += 0.01;                               // Increment the float value
  radio.write( &myData, sizeof(myData) );              // Send the final one back.      
  radio.startListening();                              // Now, resume listening so we catch the next packets.     
  Serial.print(F("Sent response "));
  Serial.print(myData._micros);  
  Serial.print(F(" : "));
  Serial.println(myData.value);
  Serial.print(F("Servo_value"));
  Serial.println(myData.servo_value);

  if (myData.servo_value > 0) {
    Serial.print("right");
    myservo.write(180);  
  }
  if(myData.servo_value < 0) {
    Serial.print("left");
    myservo.write(0);  
  }


}


} 
