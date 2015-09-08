import serial
import time
ser = serial.Serial('/dev/ttyUSB0', 9600)
tx = 'somerandomstringhereqwert'
ser.open()
maxtime = int(rawinput(“How much time zigbee interference ?”))
currenttime = time.time()

while (time.time() - currenttime < max_time):
	ser.write('%s\n' %tx)

ser.close()	
