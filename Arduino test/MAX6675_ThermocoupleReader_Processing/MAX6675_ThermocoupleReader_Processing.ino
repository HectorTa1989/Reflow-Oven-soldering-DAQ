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

#define MAX6675_CS   22
#define MAX6675_SO   12
#define MAX6675_SCK  13

void setup() {
  Serial.begin(9600);

  Serial.println("CLEARDATA");   // xóa dữ liệu trên sheet đầu tiên của file excel
  Serial.println("LABEL,Time,Temp 1 (oC),Humid 1 (%RH),Temp 2 (oC),Humid 2 (%RH)"); // Nhãn (label) cho 5 cột đầu tiên trên sheet đầu tiên của file excel
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("DATA,TIME,");   // Gán giá trị cho cột đầu tiên = thời gian hiện hành
  Serial.print(readThermocouple());
  Serial.println('c');
  Serial.print(",");       // chuyển sang cột tiếp theo
  // Just for completeness
  if(readThermocouple() > 33)
  {
     Serial.println("Wow it's hot today!");
  }
  else if(readThermocouple() < 10)
  {
    Serial.println("I hope you have your warm clothes on!");
  }
  
  delay(1500);
}

double readThermocouple() {

  uint16_t v;
  pinMode(MAX6675_CS, OUTPUT);
  pinMode(MAX6675_SO, INPUT);
  pinMode(MAX6675_SCK, OUTPUT);
  
  digitalWrite(MAX6675_CS, LOW);
  delay(1);

  // Read in 16 bits,
  //  15    = 0 always
  //  14..2 = 0.25 degree counts MSB First
  //  2     = 1 if thermocouple is open circuit  
  //  1..0  = uninteresting status
  
  v = shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
  v <<= 8;
  v |= shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
  
  digitalWrite(MAX6675_CS, HIGH);
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
