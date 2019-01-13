## It listens to serial port and send data to serial port
## requires pySerial to be installed 
import sys 
import serial
import time

MEMORY_SIZE = 1048576 # In bytes
serial_port = '/dev/ttyACM0'
baud_rate = 115200 # In arduino, Serial.begin(baud_rate)
ser = serial.Serial(serial_port, baud_rate)

fileName = raw_input("Please enter file name: ")

with open(fileName, mode='rb') as file: # b is important -> binary
	start = time.time()
	#i = 0
	ser.write('H') # for handshake request

	# whole file at a time
	#fileContent = file.read()

	# 1 byte at a time
	for i in range(4096): # 4096 x 256 bytes = 1MB
		# wait until arduino requests 'R' i.e. 256 bytes of data
		while (ser.read() != 'R'): continue

		for j in range(256):
			byte_s = file.read(1)
			if not byte_s: break
			byte = byte_s[0]
			ser.write(byte)
		'''byte_s = file.read(256)
		if not byte_s: break
		ser.write(byte_s)'''
		print str((i + 1) * 256), 'bytes of', MEMORY_SIZE, 'bytes has been uploaded.'
		
	'''while 1:
		byte_s = file.read(1)
		if not byte_s:
			break
		byte = byte_s[0]
		ser.write(byte)
		print str(i + 1), 'bytes of', MEMORY_SIZE, 'bytes has been uploaded.'
		i += 1'''

	status = ser.readline()
	print(status)
		
print '\nIt took', time.time()-start, 'seconds.'
