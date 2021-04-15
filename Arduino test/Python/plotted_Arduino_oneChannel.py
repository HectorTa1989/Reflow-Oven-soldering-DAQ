#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      Hector Ta
#
# Created:     24/07/2019
# Copyright:   (c) Hector Ta 2019
# Licence:     <your licence>
#-------------------------------------------------------------------------------
import serial
import csv
import time
import numpy as np
import matplotlib.pyplot as plt
import os,sys
from scipy import stats
##ser = serial.Serial('COM4', 9600, timeout=.1)
##while True:
##	data = ser.readline()[:-2] #the last bit gets rid of the new-line chars
##	if data:
##		print data

##Approach 1
##In this case, we rely on seeing the letter "E" to know when to stop reading from the file.
# Open com port
ser = serial.Serial('COM4', 9600)

# Read and record the data
data =[]                       # empty list to store the data
for i in range(50):
    b = ser.readline()         # read a byte string
    string_n = b.decode()  # decode byte string into Unicode
    string = string_n.rstrip() # remove \n and \r
##    flt = float(string)        # convert string to float
##    print(flt)
    data.append(string)           # add to the end of data list
    time.sleep(0.1)            # wait (sleep) 0.1 seconds

ser.close()

# show the data

for line in data:
    print(line)

# if using a Jupyter notebook include
#matplotlib inline

plt.plot(data)
plt.xlabel('Time (seconds)')
plt.ylabel('Thermocouples Reading')
plt.title('Thermocouples Reading vs. Time')
plt.show()
