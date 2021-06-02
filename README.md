# Reflow-soldering-oven-DAQ
Reflow soldering oven is used to solder PCB with a low rate of damaging circuits' components.

Herein, I write Python code to acquire data from 32 thermocouples measuring temperature in a soldering oven. The sensor data is transmitted/received via SPI protocol and is read on a microcontroller (MCU).

Along with the length of the oven, we need to control the temperature increasing subsequently. 

In the inlet of the oven, the temperature should be around 27 degree celcius. 

In the middle of the oven, the temperature should be around 100 degree celcius.

In the end of the oven, the temperature should be around 270 degree celcius.

Based on the sensor data being read by MCU, we can monitor and build an AI model to train the system automatically adjust the temperature.
