# -*- coding: utf-8 -*-
"""
Created on Wed Jan  4 15:04:15 2017

@author: jelle

How to re-route sound:
https://askubuntu.com/questions/388570/how-do-i-route-the-output-of-a-single-program-to-a-mic-input#388675

This script makes a plot of the standard deviation or sound intensity of a sound file,
in time steps of 50 ms.
"""
import pyaudio
import wave
import numpy as np
import matplotlib.pyplot as plt
from scipy.io.wavfile import read#,write
#for determining the runtime
from timeit import default_timer
import time
#for rounding
import math
#for detecting peaks
from scipy import signal
#for fitting 
from scipy.optimize import curve_fit

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
RECORD_SECONDS = 2.
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
#model of a gaussian
#def gaus(x,a,x0,sigma):
#	return a*exp(-(x-x0)**2/(2*sigma**2))

#read file
rate,data = read('output.wav')

#the size of the time step for the plot
tStep = 0.02 #seconds
#start time when we take the first FFt
startTime = 20.
#maximum time till which we want to plot the FFT
recLength = 10. #seconds
maxTime = startTime + recLength

#std above which a beat should be detected
stdlevel = 15.

y = data.T[0]

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
	
#	soundData = np.append(soundData, np.std(plotdata)) #[:int(freq * len(FFT) / RATE)]
	soundData = np.append(soundData, np.std(plotdata))
	
#	n = len(FFT)                          #the number of data
#	mean = sum(x*y)/n                   #note this correction
#	sigma = sum(y*(x-mean)**2)/n        #note this correction
	
	
	#if standard deviation is high enough, a beat is detected. This is then 
	#sent to the arduino
	if np.std(plotdata) > stdlevel: #19 for step 0.05 , 12 for step 0.02
#		serial.write("1")
		previousbeat = 0.
		beat = np.append(beat, 1)
#	elif previousbeat < 0.10:
#		ser.write("1")
	else:
#		serial.write("0")
		beat = np.append(beat, 0)
	
#	previousbeat += tStep
	
	duration = default_timer() - startime
	prtDur = math.ceil(duration * 1e6)/1000
	print prtDur, "ms,", t, "s"
	
	#if it took less than the time step to do these calculations, we wait until
	#we have matched the time step
#	if duration < tStep:
#		time.sleep(tStep - duration)
'''
'''
	#plots the beat with a peak finding algorithm
peak_widths = np.arange(1, 70)
#finds locations of peaks
peak_indices = signal.find_peaks_cwt(soundData, peak_widths)
#transforms indices to time
peakLoc = np.array(peak_indices) * tStep + startTime
peak_count = len(peak_indices) # the number of peaks in the array

peakdiff = np.roll(peakLoc, -1) - peakLoc

print 60. / np.median(peakdiff)

print "FFT length:", len(FFT), "Hz"

plt.plot(FFTtime, soundData)
plt.scatter(peakLoc, np.zeros(len(peak_indices)) + stdlevel, c = 'r', edgecolor = 'none')
#plt.plot(FFTtime, beat)
plt.xlabel("Time [s]")
plt.ylabel("Summed intensity")
#plt.savefig("./Beat plots/Std t0 = " + str(startTime) + ".png")
plt.show()
'''

	#plots the beat with the std threshold
'''
peak_widths = np.arange(1, 70)
#finds locations of peaks
peak_indices = signal.find_peaks_cwt(soundData, peak_widths)
#transforms indices to time
peakLoc = np.array(peak_indices) * tStep + startTime
peak_count = len(peak_indices) # the number of peaks in the array

peakdiff = np.roll(peakLoc, -1) - peakLoc

print 60. / np.median(peakdiff)

print "FFT length:", len(FFT), "Hz"
'''
'''
#finds the time locations of the beat
beatTime = (np.arange(len(beat)) * tStep + startTime)[beat > 0]
#makes the y data for the plot
beatlevel = (beat * stdlevel)[beat > 0]

plt.plot(FFTtime, soundData)
plt.scatter(beatTime, beatlevel, c = 'r', edgecolor = 'none')
#plt.plot(FFTtime, beat)
plt.xlabel("Time [s]")
plt.ylabel("Summed intensity")
#plt.savefig("./Beat plots/Std t0 = " + str(startTime) + ".png")
plt.show()
'''