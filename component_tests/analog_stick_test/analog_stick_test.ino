//int VRx = 32;
//int VRy = 33;
//int SW = 35;
//
//int xPosition = 0;
//int yPosition = 0;
//int SW_state = 0;
//int mapX = 0;
//int mapY = 0;
//
//void setup() {
//  Serial.begin(9600); 
//  
//  pinMode(VRx, INPUT);
//  pinMode(VRy, INPUT);
//  pinMode(SW, INPUT); 
//  
//}
//
//void loop() {
//  xPosition = analogRead(VRx);
//  yPosition = analogRead(VRy);
//  SW_state = digitalRead(SW);
//  mapX = map(xPosition, 0, 1023, -512, 512);
//  mapY = map(yPosition, 0, 1023, -512, 512);
//  
//  Serial.print("X: ");
//  Serial.println(mapX);
////  Serial.print(" | Y: ");
////  Serial.println(mapY);
////  Serial.print(" | Button: ");
////  Serial.println(SW_state);
//
//  delay(100);
//  
//}

#define joyX 32
#define joyY 33
 
void setup() {
  Serial.begin(9600);
}
 
void loop() {

  int xValue = analogRead(joyX);
  int yValue = analogRead(joyY);
 

  Serial.print(xValue);
  Serial.print("\t");
  Serial.println(yValue);
}
