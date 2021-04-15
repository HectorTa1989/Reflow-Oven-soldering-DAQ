// derived from www.ladyada.net/learn/sensors/thermocouple

#include "max6675.h"
#include <SD.h>
#include<stdlib.h>
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 10;

// Define the thermocouple communication pins
int AthermoDO = 4;
int AthermoCLK = 3;
int AthermoCS = 5;

int BthermoDO = 4;
int BthermoCLK = 3;
int BthermoCS = 6;

// And the thermocouples
MAX6675 thermocoupleA(AthermoCLK, AthermoCS, AthermoDO);
MAX6675 thermocoupleB(BthermoCLK, BthermoCS, BthermoDO);

// the real-time clock module
RTC_DS1307 RTC;

// define a custom degree symbol
uint8_t degree[8]  = {140,146,146,140,128,128,128,128};

  
void setup() 
{
  Serial.begin(57600);
  
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) 
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  Serial.println("Dual Thermocouple Temperature Logger");

 
    // Get a time-stamp
    DateTime now = RTC.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(",");

    Serial.println(" - Start -");
    Serial.close();
  
}

int lastMinute = 0;
int lastSecond = 0;

// monitor loop...
void loop() 
{
    String dataString = "";

    // Get a time-stamp
    DateTime now = RTC.now();
    
    if (now.second() != lastSecond)
    {
      lastSecond = now.second();
    
      // Read both thermocouples
      double tempA = thermocoupleA.readFarenheit();
      double tempB = thermocoupleB.readFarenheit();
    
      // Print to the serial port
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(' ');
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
          

         
     // Assemble the log string:
     char output[16];
     dtostrf(tempA, 10, 2, output);
     dataString += output;
     dataString += ",";
     dtostrf(tempB, 10, 2, output);
     dataString += output;   
    
     // Print to the serial port and the LCD
     Serial.println(dataString);
        
   
   // log once per minute
   if (now.minute() != lastMinute)
   {
     lastMinute = now.minute();
     
    
    
       
       Serial.print(now.year(), DEC);
       Serial.print('/');
       Serial.print(now.month(), DEC);
       Serial.print('/');
       Serial.print(now.day(), DEC);
       Serial.print(' ');
       Serial.print(now.hour(), DEC);
       Serial.print(':');
       Serial.print(now.minute(), DEC);
       Serial.print(':');
       Serial.print(now.second(), DEC);
       Serial.print(",");

       Serial.println(dataString);
       Serial.close();
       delay(100);
       
     // if the file isn't open, pop up an error:
     else 
     {
       Serial.println("error opening datalog.txt");
     } 
   }
}
