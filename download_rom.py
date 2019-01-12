## It listens to serial port and writes contents into a file
## requires pySerial to be installed 
import sys 
import serial
import time
start = time.time()

MEMORY_SIZE = 1048576 # In bytes
serial_port = '/dev/ttyACM0'
baud_rate = 115200; # In arduino, Serial.begin(baud_rate)
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
		output_file.write(byte);
		
print '\nIt took', time.time()-start, ' seconds.'
