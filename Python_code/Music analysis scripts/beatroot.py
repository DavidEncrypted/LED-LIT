# -*- coding: utf-8 -*-
"""
Created on Wed Jan  4 15:04:15 2017

@author: jelle

How to re-route sound:
https://askubuntu.com/questions/388570/how-do-i-route-the-output-of-a-single-program-to-a-mic-input#388675

This script is an application of the beatroot algorithm for detecting beats. 
It works by detecting the magnitude of the increase of sound intensity.

More info:
https://stackoverflow.com/questions/657073/how-to-detect-the-bpm-of-a-song-in-php
"""
import pyaudio
#import wave
import numpy as np
import matplotlib.pyplot as plt
from scipy.io.wavfile import read#,write
#for determining the runtime
from timeit import default_timer
import time
#for rounding
import math

#for sending serial data
#import serial

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
tStep = 0.02 #seconds
#start time when we take the first FFt
startTime = 90.
#maximum time till which we want to plot the FFT
recLength = 10. #seconds
maxTime = startTime + recLength

y = data.T[0]

#the time step array that determines when the FFT is taken
FFTtime = np.arange(startTime, maxTime, tStep)
#array containing certain values of the FFT (like mean, median etc)
soundData = []
beat = []

previousbeat = 1. #s

#initial values for the prevSound array, necessary for comparing previous intensities
FFT = np.abs(np.fft.fft(y[int(0. * RATE): int((0. + tStep) * RATE)]))
prevFFT = FFT[:int(len(FFT)/2)]

#loop over the file until maxTime seconds, with step size tStep
for t in FFTtime:
	startime = default_timer()
	
	tempFFT = np.abs(np.fft.fft(y[int(t * RATE): int((t + tStep) * RATE)]))
	FFT = tempFFT[:int(len(tempFFT)/2)]
	
#	soundData = np.append(soundData, np.std(plotdata)) #[:int(freq * len(FFT) / RATE)]
#	soundData = np.append(soundData, np.sum(FFT))
	
	#takes difference between previous and current FFT
	if len(FFT) > len(prevFFT):
		FFTdiff = FFT[:len(prevFFT)] - prevFFT
	elif len(FFT) < len(prevFFT):
		FFTdiff = FFT - prevFFT[:len(FFT)]
	else:
		FFTdiff = FFT - prevFFT
	#ignores all decreases in FFT
	FFTdiff[FFTdiff < 0] = 0
	#saves the sum to the soundData array
	soundData = np.append(soundData, np.sum(FFTdiff))
	
	if np.sum(FFTdiff) > 600:
		beat = np.append(beat, t)
	
	previousbeat += tStep
	prevFFT = FFT
	
	duration = default_timer() - startime
	prtDur = math.ceil(duration * 1e6)/1000
	print prtDur, "ms,", t, "s"
	#if it took less than the time step to do these calculations, we wait until
	#we have matched the time step	
#	if duration < tStep:
#		time.sleep(tStep - duration)
	
print "FFT length:", len(FFT), "Hz"



plt.plot(FFTtime, soundData)
#plt.plot(FFTtime, beat)
plt.scatter(beat, np.zeros(len(beat)) + 1000., edgecolor = 'none', c = 'r', s = 40)
plt.xlabel("Time [s]")
plt.ylabel("Magnitude difference")
#plt.savefig("./Beat plots/Beatroot t0 = " + str(startTime) + ".png")
plt.show()


