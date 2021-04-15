/** MAX6675 Thermocouple Reader Example Code
 *  
 *  This is so simple a library would be silly!
 *  
 *  6675 Module   ==>   Arduino
 *    CS          ==>     D10
 *    SO          ==>     D12
 *    SCK         ==>     D13
 *    Vcc         ==>     Vcc (5v OK)
 *    Gnd         ==>     Gnd
 *    
 *  You can change the pin assignments below, any pins you want are fine.  
 *  
 *  Upload coade and open your Serial terminal at 9600 to see the temperature 
 *    printed every 1.5 seconds.  That's all!
 *  
 */

#include <SPI.h>

#define MAX6675_CS1   22
#define MAX6675_CS2   23
#define MAX6675_SO   12
#define MAX6675_SCK  13

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(readThermocouple(MAX6675_CS1, MAX6675_SO, MAX6675_SCK));
  delay(1); 
  Serial.println(readThermocouple(MAX6675_CS2, MAX6675_SO, MAX6675_SCK));
//  Serial.println('c');
  
  delay(200);
}

double readThermocouple(int8_t CS, int8_t SO, int8_t SCK) {

  uint16_t v;
  pinMode(CS, OUTPUT);
  pinMode(SO, INPUT);
  pinMode(SCK, OUTPUT);
  
  digitalWrite(CS, LOW);
  delay(1);

  // Read in 16 bits,
  //  15    = 0 always
  //  14..2 = 0.25 degree counts MSB First
  //  2     = 1 if thermocouple is open circuit  
  //  1..0  = uninteresting status
  
  v = shiftIn(SO, SCK, MSBFIRST);
  v <<= 8;
  v |= shiftIn(SO, SCK, MSBFIRST);
  
  digitalWrite(CS, HIGH);
  if (v & 0x4) 
  {    
    // Bit 2 indicates if the thermocouple is disconnected
    return NAN;     
  }

  // The lower three bits (0,1,2) are discarded status bits
  v >>= 3;

  // The remaining bits are the number of 0.25 degree (C) counts
  return v*0.25;
}
