# -*- coding: utf-8 -*-
"""
Created on Wed Jan  4 14:47:26 2017

@author: jelle

This script sends data to an arduino using PySerial;
http://playground.arduino.cc/Interfacing/Python
"""

import serial

ser = serial.Serial('/dev/ttyUSB0', 9600)

answer = '1'

while answer != "0":
	answer = raw_input("1=ON, exit=EXIT PROGRAM")
	ser.write(answer)


	
	


