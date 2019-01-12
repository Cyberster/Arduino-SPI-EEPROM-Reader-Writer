// pin configurations
#define CS_BAR  10 // SS     EEPROM pin 1
#define DO      11 // MOSI   EEPROM pin 2
#define DIO     12 // MISO   EEPROM pin 5
#define CLK     13 // SCK    EEPROM pin 6

// opcodes for winbond W25X10A, W25X20A, W25X40A, W25X80A EEPROMs
#define WREN      0x06  // write enable
#define WRDI      0x04  // write disable
#define RDSR      0x05  // read status register
#define WRSR      0x01  // write status register
#define READ      0x03  // read from memory
#define FREAD     0x0B  // fast read from memory 
#define FREAD2    0x3B  // fast read dual output from memory 
#define WRITE     0x02  // write to memory
#define BERASE    0xD8  // block erase (64KB)
#define SERASE    0x20  // sector erase (4KB)
#define CERASE    0xC7  // chip erase (0xC7/0x60)
#define PWRDWN    0xB9  // power down
#define RELPWR    0xAB  // block erase (64KB)
#define INFO      0x90  // get manufacturer & device ID info
#define JEDEC     0x9F  // get JEDDEC ID

byte clr;

char spi_transfer(volatile char data) {
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1<<SPIF))) {}; // Wait the end of the transmission
  return SPDR;                    // return the received byte
}

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  // wait for serial port to connect. Needed for native USB port only
  while (!Serial);

  pinMode(DO, OUTPUT);
  pinMode(DIO, INPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS_BAR, OUTPUT);
  digitalWrite(CS_BAR, HIGH); // disable device
  // SPCR = 01010000
  //interrupt disabled,spi enabled,msb 1st,master,clk low when idle,
  //sample on leading edge of clk,system clock/4 rate (fastest)
  SPCR = (1 << SPE) | (1 << MSTR);
  clr = SPSR;
  clr = SPDR;
  delay(10);

//  // read manufacturer id and device id
//  byte manufacturer_id;
//  byte device_id;
//  get_device_info(manufacturer_id, device_id);
//  Serial.println(manufacturer_id, HEX);
//  Serial.println(device_id, HEX);

//  // read data
//  read_block(0x000000, 16, 20, true);

//  // download entier rom into a file
  download_rom(true);

//  sector_erase(0x000000); // erase 4kB from given address
//  block_erase(0x000000); // erase 64kB from given address
//  chip_erase(); // erase the entier chip

//  // write data to eeprom
//  // fill buffer with data
//  byte buffer[256];
//  for (int I = 0; I < 256; I++) {
//    buffer[I] = I;
//  }
//  write_eeprom(0x00010E, buffer, 256);
  //chip_erase();
  //upload_rom();
  //read_block(0x000000, 16, 20, true);
}

// read data from EEPROM
byte read_eeprom(int EEPROM_address) {
  int data;
  digitalWrite(CS_BAR, LOW); // Write function
  spi_transfer(READ); //transmit read opcode
  spi_transfer((char)(EEPROM_address >> 16)); //send b2 address first
  spi_transfer((char)(EEPROM_address >> 8));  //send b1 address
  spi_transfer((char)(EEPROM_address));       //send b0 address last
  data = spi_transfer(0xFF); //get data byte
  digitalWrite(CS_BAR, HIGH); //release chip, signal end transfer
  return data;
}

// functions for WINBOND EEPROM chips
void get_device_info(byte &manufacturer_id, byte &device_id) {
  // read manufacturer id and device id
  digitalWrite(CS_BAR, LOW);
  spi_transfer(INFO);
  spi_transfer(0x00);
  spi_transfer(0x00);
  spi_transfer(0x00);
  manufacturer_id = spi_transfer(0xFF); //get data byte
  device_id = spi_transfer(0xFF); //get data byte
  digitalWrite(CS_BAR, HIGH);
  delay(10);  
}

void read_block(long starting_address, int block_size, int block_count, boolean fastread) {
  // read data, block_size columns (bytes) in a row, block_count rows
  for (long i = starting_address; i < starting_address + (block_size * block_count); i += block_size) {
    digitalWrite(CS_BAR, LOW);
    if (fastread)
      spi_transfer(FREAD);
    else
      spi_transfer(READ);
    spi_transfer((byte)((i >> 16) & 0x0000FF));
    spi_transfer((byte)((i >> 8) & 0x0000FF));
    spi_transfer((byte)((i) & 0x0000FF));
    if (fastread)
      spi_transfer(0xFF); // adding dummy clock for fast read

    byte data[block_size];
    char buf[block_size * 4 + block_size + 8]; 

    int length = 0;
    length += sprintf(buf + length, "%06lX\t ", i);
  
    for (int j = 0; j < block_size; j++) {
      data[j] = spi_transfer(0xFF);
      length += sprintf(buf + length, " %02X", data[j]);
    }
    length += sprintf(buf + length, "\t ");
    for (int j = 0; j < block_size; j++) {
      if (data[j] >= 32 && data[j] <= 127)
        length += sprintf(buf + length, "%c", (char)data[j]);
      else
        length += sprintf(buf + length, ".");
    }
    digitalWrite(CS_BAR, HIGH);

    Serial.println(buf);
  }
}

void download_rom(boolean fastread) {
  while (!Serial.available()) {}
  char ch = (char)Serial.read();

  if (ch == 'd') {
    // read data    
    digitalWrite(CS_BAR, LOW);
    if (fastread)
      spi_transfer(FREAD);
    else
      spi_transfer(READ);
    spi_transfer(0x00);
    spi_transfer(0x00);
    spi_transfer(0x00);
    if (fastread)
      spi_transfer(0xFF); // adding dummy clock for fast read
  
    for (int i = 0x000000; i <= 0xFF0000; i++) {
      byte data = spi_transfer(0xFF);
      Serial.write(data);
    }
    digitalWrite(CS_BAR, HIGH);
    //Serial.println("EOF");
    Serial.println("ROM downloaded successfully.");
  }
}

// erase 4kB from given address
void sector_erase(long address) {
  digitalWrite(CS_BAR, LOW);
  spi_transfer(WREN); //write enable
  digitalWrite(CS_BAR, HIGH);
  digitalWrite(CS_BAR, LOW);
  spi_transfer(SERASE);
  spi_transfer((byte) (address >> 16));   //send b2 address first
  spi_transfer((byte) (address >> 8));   //send b1 address
  spi_transfer((byte) address);   //send b0 address last
  digitalWrite(CS_BAR, HIGH);
  delay(250); // tBE = 120-250ms
  digitalWrite(CS_BAR, LOW);
  spi_transfer(WRDI); //write disable
  digitalWrite(CS_BAR, HIGH);
  Serial.println("Operation SECTOR ERASE has been executed successfully.");
}

// erase 64kB from given address
void block_erase(long address) {
  digitalWrite(CS_BAR, LOW);
  spi_transfer(WREN); //write enable
  digitalWrite(CS_BAR, HIGH);
  digitalWrite(CS_BAR, LOW);
  spi_transfer(BERASE);
  spi_transfer((byte) (address >> 16));   //send b2 address first
  spi_transfer((byte) (address >> 8));   //send b1 address
  spi_transfer((byte) address);   //send b0 address last
  digitalWrite(CS_BAR, HIGH);
  delay(1000); // tBE = 0.4-1s
  digitalWrite(CS_BAR, LOW);
  spi_transfer(WRDI); //write disable
  digitalWrite(CS_BAR, HIGH);
  Serial.println("Operation BLOCK ERASE has been executed successfully.");
}

// erase the entier chip
void chip_erase() {
  digitalWrite(CS_BAR, LOW);
  spi_transfer(WREN); //write enable
  digitalWrite(CS_BAR, HIGH);
  digitalWrite(CS_BAR, LOW);
  spi_transfer(CERASE);
  digitalWrite(CS_BAR, HIGH);
  delay(10000); // tCE = 6-10s for W25X80A in our case.
  digitalWrite(CS_BAR, LOW);
  spi_transfer(WRDI); //write disable
  digitalWrite(CS_BAR, HIGH);
  Serial.println("Operation CHIP ERASE has been executed successfully.");
}

// write data to the EEPROM
// max buffer size is 256
void write_eeprom(long address, byte data[], int data_size) {
  digitalWrite(CS_BAR, LOW);
  spi_transfer(WREN); //write enable
  digitalWrite(CS_BAR, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(CS_BAR, LOW);
  spi_transfer(WRITE); //write instruction
  spi_transfer((byte)((address >> 16) & 0x0000FF));  // send b2 address first
  spi_transfer((byte)((address >> 8) & 0x0000FF));   // send b1 address

  // send b0 address:
  // If an entire 256 byte page is to be programmed, the last address 
  // byte (the 8 least significant address bits) should be set to 0.
  if (data_size == 256)
    spi_transfer(0x00);                              // as 0xFF
  else
    spi_transfer((byte)((address) & 0x0000FF));      // as it is

  for (int i = 0; i < data_size; i++) {
    spi_transfer(data[i]);   //write data byte
  }
  
  digitalWrite(CS_BAR, HIGH); //release chip
  //wait for eeprom to finish writing
  delayMicroseconds(1500); // tpp = 1.5-3ms
  
  digitalWrite(CS_BAR, LOW);
  spi_transfer(WRDI); //write disable
  digitalWrite(CS_BAR, HIGH);
}

// upload data from file to the EEPROM
void upload_rom() {
  while (!Serial.available()) {}
  while (Serial.read() != 'H'); // wait for handshake request from cmoputer
  
  byte buff[256];

  // 1 page at a time where page size = 256 byte
  for (long address = 0; address <= 0x0FFFFF; address += 256) { //0x0FFFFF
    Serial.write('R'); // requests 256 bytes of data
    while (!Serial.available()) {
      // wait for computer  
    }

    Serial.readBytes(buff, 256);
    write_eeprom(address, buff, 256);
  }
  
//  while (Serial.available()) {
//    byte data = (byte)Serial.read();
//    Serial.readBytes(buff, 256);
//    
//    buff[i++] = data;
//    if (i == 256) {
//      write_eeprom(address, buff, 256);
//      address += 256;
//      i = 0;
//    }
//  }
  
  Serial.println("ROM uploaded successfully.");
}

void loop() {}
