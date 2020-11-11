#include "DHT.h"
 
#define DHTPIN 5
//our sensor is DHT22 type
#define DHTTYPE DHT22
//create an instance of DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void getTemp() 
{
    //use the functions which are supplied by library.
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
  Serial.println("Failed to read from DHT sensor!");
  return;
  }
  // print the result to Terminal
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  //we delay a little bit for next read
  delay(2000);
    
}

void setup() {
  Serial.begin(9600);
  
  //initialize the dht / temperature sensor
  dht.begin();

}

void loop() {
  getTemp();

}
