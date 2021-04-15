#include <Arduino.h>

#define SIZE 200
#define INTERVAL 5000 // 5 milliseconds per sample for 200 Hz, change this for different sampling rates.
//#define inPin0 A0

const unsigned long SampleIntervalMicros = 1000000UL / 100;  // 100 Hz
unsigned long LastSampleTimeMicros;

unsigned int lastMicros = 0;
unsigned int storedValue[SIZE];


void setup() 
{  
//  pinMode(0,INPUT);
//  pinMode(1,INPUT);
//  pinMode(2,INPUT);
//  pinMode(3,INPUT);
//  pinMode(4,INPUT);
//  pinMode(5,INPUT);
  
  Serial.begin(115200);  
  timeBefore = micros();
}

void loop()
{ 
  if ( elapsedTime >= INTERVAL){ // 400 Hz sample rate
  {
   timeNow = micros(); // do this first or your interval is too long!
    for (int n=0; n<SIZE; n++)
    {
      storedValue[n] = analogRead(A0);
    }
    timeBefore = timeNow + 2500;  // set next sample time
// for (int m = 0; m<SIZE; m++)
// {
//  Serial.println(storedValue[m]*5.0/1024);
// }
  }
}
