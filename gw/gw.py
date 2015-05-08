#!/usr/bin/env python2

from influxdb.influxdb08 import InfluxDBClient
import json
import serial
import signal
import sys
import time

# SIGINT function.
def sig_SIGINT( signal, frame ):
	arduino.close( )
	print 'Exiting..'
	sys.exit( 0 )
# Trap interrupt signal.
signal.signal( signal.SIGINT, sig_SIGINT )

beername = 'hope&king'

influx_host = 'influx.theclonchs.com'
influx_user = 'brew'
influx_pass = 'brew'
influx_db = 'brewery'

seriesbase = beername + '.fermentation'

influxdb = InfluxDBClient( influx_host, 8086, influx_user, influx_pass, influx_db )
arduino = serial.Serial( '/dev/ttyACM0', 115200 )

print "Reseting Arduino.."

# Toggle DTR to reset Arduino
arduino.setDTR(False)
time.sleep(1)
# toss any data already received, see
# http://pyserial.sourceforge.net/pyserial_api.html#serial.Serial.flushInput
arduino.flushInput()
arduino.setDTR(True)

print "Reading data.."

# Loop forever.
while True:
	keys = []
	values = []
	# Read a complete line from the serial input.
	line = arduino.readline( )
	# Chomp
	line = line.rstrip( '\n' )
	# Split line on ',', then split on ':', then loop on resultant key/value pair.
	for key,value in (item.split(':') for item in line.split( ',' )):
		#print "%s == %s" % (key, value)
		keys.append( key )
		values.append( float( value ) )
	print "Sending data.."
	datas = []
	data = {}
	data['name'] = 'fermtemp'
	data['columns'] = keys
	data['points'] = [values]
	datas.append( data )
	jdata = json.dumps(datas)
	print jdata
	influxdb.write_points( jdata )
