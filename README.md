# Read carefully before using this project


## Steps to download the content of EEPROM into a file:

1. First, make **download_rom()** function only active and flash the sketch into your arduino.
2. Configure **serial port** and **baud rate** in **download_rom.py** according to your arduino sketch.
3. Press **reset** button in your arduino.
4. Run **download_rom.py** by entering '**python2.7 download_rom.py**' in your terminal/cmd window.
5. Now press '**d**' and hit enter in the prompt in your terminal/cmd window and wait for finish.


## Steps to upload a file from computer to the EEPROM:

1. First, make **upload_rom()** function only active and flash the sketch into your arduino.
2. Configure **serial port** and **baud rate** in **upload_rom.py** according to your arduino sketch.
3. Run **upload_rom.py** by entering '**python2.7 upload_rom.py**' in your terminal/cmd window.
4. Press **reset** button in your arduino.
5. Enter the file path i.e. '**/path/to/the/file.rom**' and hit enter and wait for finish.

## Schematic
Schematic can be found here: https://easyeda.com/arp14/spi-based-eeprom-reader-writer
![SPI-Based-EEPROM-Reader-Writer Schematic](https://raw.githubusercontent.com/Cyberster/SPI-Based-EEPROM-Reader-Writer/master/Schematic_SPI-Based-EEPROM-Reader-Writer_Sheet-1_20190113165852.png)

## Details about various functions with examples can be found here:
https://w3epic.com/how-to-read-write-spi-based-serial-eeprom-using-arduino-uno-e-g-winbond-w25x80a/