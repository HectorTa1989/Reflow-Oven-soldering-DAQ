const byte sensor = A0;  // where the multiplexer in/out port is connected

// the multiplexer address select lines (A/B/C)
const byte addressA = 6; // low-order bit
const byte addressB = 5;
const byte addressC = 4; // high-order bit

void setup ()
  {
  Serial.begin (115200);
  Serial.println ("Starting multiplexer test ...");
  pinMode (addressA, OUTPUT); 
  pinMode (addressB, OUTPUT); 
  pinMode (addressC, OUTPUT); 
  }  // end of setup

int readSensor (const byte which)
  {
  // select correct MUX channel
  digitalWrite (addressA, (which & 1) ? HIGH : LOW);  // low-order bit
  digitalWrite (addressB, (which & 2) ? HIGH : LOW);
  digitalWrite (addressC, (which & 4) ? HIGH : LOW);  // high-order bit
  // now read the sensor
  return analogRead (sensor);
  }  // end of readSensor
  
void loop ()
  {
  // show all 8 sensor readings
  for (byte i = 0; i < 15; i++)
    {
    Serial.print ("Sensor ");
    Serial.print (i);
    Serial.print (" reads: ");
    Serial.println (readSensor (i));
    }
  delay (1000);
  }  // end of loop
