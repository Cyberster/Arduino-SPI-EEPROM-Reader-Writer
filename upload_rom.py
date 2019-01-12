##############
## Script listens to serial port and send data to serial port
##############
## requires pySerial to be installed 
import sys 
import serial
import time

MEMORY_SIZE = 1048576 #in bytes

serial_port = '/dev/ttyACM0'
baud_rate = 115200; #In arduino, Serial.begin(baud_rate)
ser = serial.Serial(serial_port, baud_rate)

fileName = raw_input("Please enter file name: ")

with open(fileName, mode='rb') as file: # b is important -> binary
	start = time.time()
	i = 1

	# whole file at a time
	#fileContent = file.read()

	# 1 byte at a time
	while 1:
		byte_s = file.read(1)
		if not byte_s:
			break
		byte = byte_s[0]
		ser.write(byte)
		print str(i), 'bytes of', MEMORY_SIZE, 'bytes has been uploaded.'
		i += 1

	status = ser.readline()
	print(status)
'''
if ch == 'd':
	ser.write('d')
	for i in range(MEMORY_SIZE):
		print(str(MEMORY_SIZE - i) + " bytes remaining.")
		byte = ser.read();
		#line = line.decode("utf-8") #ser.readline returns a binary, convert to string
		#print(line);
		output_file.write(byte);'''
		
print '\nIt took', time.time()-start, 'seconds.'
