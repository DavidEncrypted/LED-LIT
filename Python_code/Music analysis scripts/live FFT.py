# -*- coding: utf-8 -*-
"""
Created on Wed Jan  4 15:04:15 2017

@author: jelle

How to re-route sound:
https://askubuntu.com/questions/388570/how-do-i-route-the-output-of-a-single-program-to-a-mic-input#388675

This script produces a live plot of audio in the 'output.wav' file
"""
import pyaudio
import wave
import numpy as np
import matplotlib.pyplot as plt
from scipy.io.wavfile import read#,write
#for determining the runtime
from timeit import default_timer
import time

#for sending serial data
import serial

#initiale the serial port
#ser = serial.Serial('/dev/ttyUSB0', 9600)

plt.close()

FORMAT = pyaudio.paInt16
CHANNELS = 2
RATE = 44100
WAVE_OUTPUT="output.wav"
CHUNK_SIZE = 512
RECORD_SECONDS = 0.1
#index of device to use
deviceIndex = 4

	#this is for recording audio
'''
#initialize pyaudio
p = pyaudio.PyAudio()

print p.get_device_count()
#list all the devices
for i in range(p.get_device_count()):
	info = p.get_device_info_by_index(i)
	print info['name']
	print info['defaultSampleRate']
	print "Max # output channels:",info['maxOutputChannels']
	print "Max # input channels:",info['maxInputChannels']
	print i, '\n'
				

stream = p.open(format=FORMAT, channels=CHANNELS, rate=RATE, input=True, frames_per_buffer=CHUNK_SIZE, input_device_index=deviceIndex)

print "recording..."
frames = []
 
for i in range(0, int(RATE / CHUNK_SIZE * RECORD_SECONDS)):
	data = stream.read(CHUNK_SIZE)
	frames.append(data)
	#frames = np.append(frames, np.fromstring(data, 'Float32'))

# stop Recording
stream.stop_stream()
stream.close()

startime = default_timer()

#save recorded stream
waveFile = wave.open(WAVE_OUTPUT, 'wb')
waveFile.setnchannels(CHANNELS)
waveFile.setsampwidth(p.get_sample_size(FORMAT))
waveFile.setframerate(RATE)
waveFile.writeframes(b''.join(frames))
waveFile.close()
'''

#read file
rate,data = read('output.wav')

#the size of the time step for the plot
tStep = 0.05 #seconds
#start time when we take the first FFt
startTime = 0.
#maximum time till which we want to plot the FFT
recLength = 10. #seconds
maxTime = startTime + recLength

y = data.T[0]

# You probably won't need this if you're embedding things in a tkinter plot...
plt.ion()

#start a new plot
fig = plt.figure()
ax = fig.add_subplot(111)

#set first plotting values
FFT = np.abs(np.fft.fft(y[:int(tStep * RATE)]))
plotdata = FFT[int(len(FFT)/2)]
line1, = ax.plot(plotdata, 'r-') # Returns a tuple of line objects, thus the comma

#set the limits
plt.xlim((0,RATE * tStep))
plt.ylim((0,6))

#the time step array that determines when the FFT is taken
FFTtime = np.arange(startTime, maxTime, tStep)
#array containing certain values of the FFT (like mean, median etc)
soundData = []
beat = []

previousbeat = 1. #s

#loop over the file until maxTime seconds, with step size tStep
for t in FFTtime:
	startime = default_timer()
	
	FFT = np.abs(np.fft.fft(y[int(t * RATE): int((t + tStep) * RATE)]))
	plotdata = FFT[:int(len(FFT)/2)]
	
	#updates the x and y data in the plot
	line1.set_xdata(np.arange(len(plotdata)))
	line1.set_ydata(plotdata)
	fig.canvas.draw()
#	freq = 100.
	soundData = np.append(soundData, np.std(plotdata)) #[:int(freq * len(FFT) / RATE)]
	
	duration = default_timer() - startime
#	print duration
	#if it took less than the time step to do these calculations, we wait until
	#we have matched the time step
	if duration < tStep:
		time.sleep(tStep - duration)	
	'''
	#if standard deviation is high enough, a beat is detected. This is then 
	#sent to the arduino
	if np.std(plotdata) > 19: #19 for step 0.05 , 12 for step 0.02
		ser.write("1")
		previousbeat = 0.
		beat = np.append(beat, 10)
#	elif previousbeat < 0.10:
#		ser.write("1")
	else:
		ser.write("0")
		beat = np.append(beat, 0)
	'''
	previousbeat += tStep

#plt.plot(FFTtime, soundData)
#plt.plot(FFTtime, beat)
#plt.savefig("./Beat plots/std.png")
#plt.show()

