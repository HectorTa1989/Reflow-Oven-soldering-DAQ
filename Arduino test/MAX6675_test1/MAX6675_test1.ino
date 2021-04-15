#include <max6675.h>
#include <SPI.h>
#include <SoftwareSerial.h>
// Kết nối:
//          MAX6675                   Arduino
//           GND                        GND
//           VCC                        5V
//           SCK                        6
//           CS                         5
//           SO                         4


// Nạp code mở Serial Monitor chọn No line ending, baud 9600.

//determine which breakout board is currently active, 
//by taking that board's CS pin low, whilst the CS for the 
//other board remains high.


int CS1 = 22;
int CS2 = 23;
int SO = 12;
int CLK = 13;
//static const uint8_t SCK = 13;
int vccPin = 3;
int gndPin = 2;

MAX6675 thermocouple1(SCK, CS1, SO);
MAX6675 thermocouple2(SCK, CS2, SO);
 
void setup() {
  Serial.begin(9600);
  // use Arduino pins 
  pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
  pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);
  
//  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
   int time = millis()/1000;
   Serial.print("Channel 1 Degree C = "); 
   Serial.print(time); 
   Serial.print(",");  
   Serial.println(thermocouple1.readCelsius());
   delay(50);            //delay between readings
   time = millis()/1000;
   Serial.print("Channel 2 Degree C = "); 
   Serial.print(time); 
   Serial.print(",");  
   Serial.println(thermocouple2.readCelsius());
   
//   Serial.print("\t Degree F = ");
//   Serial.println(thermocouple1.readFahrenheit());
 
   delay(1000);
}
