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
##import time
from datetime import datetime
##import numpy as np
##import matplotlib.pyplot as plt
import os,sys
##from scipy import stats
##import fileinput

##ser = serial.Serial('COM4', 9600, timeout=.1)
##while True:
##	data = ser.readline()[:-2] #the last bit gets rid of the new-line chars
##	if data:
##		print data

# Open com port
ser = serial.Serial('COM4', 9600)

# Read and record the data
data =[]                       # empty list to store the data
headers = ['DateTime', 'Channel 1(°C)', 'Channel 2(°C)']
now1 = datetime.now()
date_time_name1 = now1.strftime("%Y%m%d %H")
with open(str(date_time_name1) + ".csv","a") as f:
            writer = csv.writer(f)
            writer.writerow(["DateTime","Channel 1(°C)","Channel 2(°C)"])
while True:

        ser_bytes1 = ser.readline()
        ser_bytes2 = ser.readline()
        string_n1 = ser_bytes1.decode()  # decode byte string into Unicode
        string_n2 = ser_bytes2.decode()
        print(string_n1)
        print(string_n2)
        string1 = string_n1.rstrip() # remove \n and \r
        string2 = string_n2.rstrip()
        data1 = [string1, string2]
        data.append(data1)           # add to the end of data list
        now = datetime.now() # current date and time
        date_time = now.strftime("%Y-%m-%d \t%H:%M:%S.%f")
        date_time_name2 = now.strftime("%Y%m%d %H")
        file_name = date_time_name2 + ".csv"
        if os.path.isfile(file_name):
            with open(str(date_time_name2) + ".csv","a") as f:
                writer = csv.writer(f,delimiter=",")
##                for datum in data:
##                    writer.writerow([date_time,datum])
                writer.writerow([date_time,string1,string2])
        else:
            with open(str(date_time_name2) + ".csv","w") as f:
                 writer = csv.writer(f)
                 writer.writerow(headers)
##                 for datum in data:
                 writer.writerow([date_time,string1,string2])

##----------------------------------##
    ## file manipulation by panda
##from pandas import read_csv
##    df = read_csv('test.csv')
##    df.columns = ['a', 'b']
##    df.to_csv('test_2.csv')

    #### file manipulation by fileinput library
##import fileinput
##
##for line in fileinput.input(files=['mycsvfile.csv'], inplace=True):
##    if fileinput.isfirstline():
##        print 'ColA,ColB'
##    print line,
##----------------------------------##

##for i in range(50):
##    b = ser.readline()         # read a byte string
##    string_n = b.decode()  # decode byte string into Unicode
##    string = string_n.rstrip() # remove \n and \r
####    flt = float(string)        # convert string to float
####    print(flt)
##    data.append(string)           # add to the end of data list
##    time.sleep(0.1)            # wait (sleep) 0.1 seconds

ser.close()

# show the data



# if using a Jupyter notebook include
#matplotlib inline

##plt.plot(data)
##plt.xlabel('Time (seconds)')
##plt.ylabel('Thermocouples Reading')
##plt.title('Thermocouples Reading vs. Time')
##plt.show()

input('Press ENTER to exit')