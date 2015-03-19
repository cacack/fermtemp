#!/usr/bin/env python2

import serial
import sys
import time

arduino = serial.Serial( '/dev/ttyACM0', 115200 )

# Toggle DTR to reset Arduino
arduino.setDTR(False)
time.sleep(1)
# toss any data already received, see
# http://pyserial.sourceforge.net/pyserial_api.html#serial.Serial.flushInput
arduino.flushInput()
arduino.setDTR(True)

while True:
	sys.stdout.write( arduino.readline( ) )
