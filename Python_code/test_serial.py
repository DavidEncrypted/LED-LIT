# -*- coding: utf-8 -*-
"""
Created on Mon Jun 19 00:03:44 2017

@author: jelle
"""

#!/usr/bin/python
import serial
import syslog
import time

#The following line is for serial over GPIO
port = '/dev/ttyUSB0'


ard = serial.Serial(port,9600,timeout=5)

i = 0

while (i < 4):
    # Serial write section

    setTempCar1 = 63
    setTempCar2 = 37
    ard.flush()
    setTemp1 = str(setTempCar1)
    setTemp2 = str(setTempCar2)
    print ("Python value sent: ")
    print (setTemp1)
    ard.write(setTemp1)
    time.sleep(4)

    # Serial read section
    msg = ard.readline()
    print ("Message from arduino: ")
    print (msg)
    i = i + 1
else:
    print "Exiting"
exit()