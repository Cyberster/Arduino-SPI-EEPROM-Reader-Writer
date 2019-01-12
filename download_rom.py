'''from serial import Serial

try:
	# send output_str to the robot using pyserial
	ser = Serial("/dev/ttyACM0", 9600) # parameters: comport, baudrate
	ser.write(output_str) #send file
	
	while 1:
		serial_line = ser.readline()
		print(serial_line) # If using Python 2.x use: print serial_line
		# Do some other work on the data
		#time.sleep(300) # sleep 5 minutes
		
	ser.close()
except:
	print("\nError: \tCould not open /dev/ttyACM0 @ 9600")'''
	
##############
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import sys 
import serial
import time
start = time.time()

MEMORY_SIZE = 1048576 #in bytes

serial_port = '/dev/ttyACM0'
baud_rate = 115200; #In arduino, Serial.begin(baud_rate)
write_to_file_path = "dump.rom"

output_file = open(write_to_file_path, "wb")
ser = serial.Serial(serial_port, baud_rate)

print("Press d for dump ROM else CTRL+C to exit.")
ch = sys.stdin.read(1)

if ch == 'd':
	ser.write('d')
	for i in range(MEMORY_SIZE):
		print(str(MEMORY_SIZE - i) + " bytes remaining.")
		byte = ser.read();
		#line = line.decode("utf-8") #ser.readline returns a binary, convert to string
		#print(line);
		output_file.write(byte);
		
print '\nIt took', time.time()-start, ' seconds.'
