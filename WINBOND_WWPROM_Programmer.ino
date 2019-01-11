#define DATAOUT 11//MOSI
#define DATAIN  12//MISO
#define SPICLOCK  13//sck
#define SLAVESELECT 10//ss

//opcodes
#define WREN      0x06
#define WRDI      0x04
#define RDSR      0x05
#define WRSR      0x01
#define READ      0x03
#define FASTREAD  0x0B
#define WRITE     0x02
#define WRITE     0x90

byte eeprom_output_data;
byte eeprom_input_data = 0;
byte clr;
int address = 0;
//data buffer
char buffer [128];

void fill_buffer() {
  for (int I = 0; I < 128; I++) {
    buffer[I]=I;
  }
}

char spi_transfer(volatile char data) {
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1<<SPIF))) {};    // Wait the end of the transmission
  return SPDR;                    // return the received byte
}

// functions for WINBOND EEPROM chips
void get_device_info(byte &manufacturer_id, byte &device_id) {
  // read manufacturer id and device id
  digitalWrite(SLAVESELECT, LOW);
  spi_transfer(0x90);
  spi_transfer(0x00);
  spi_transfer(0x00);
  spi_transfer(0x00);
  manufacturer_id = spi_transfer(0xFF); //get data byte
  device_id = spi_transfer(0xFF); //get data byte
  digitalWrite(SLAVESELECT, HIGH);
  delay(10);  
}

void setup() {
  Serial.begin(115200);

  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK, OUTPUT);
  pinMode(SLAVESELECT, OUTPUT);
  digitalWrite(SLAVESELECT, HIGH); //disable device
  // SPCR = 01010000
  //interrupt disabled,spi enabled,msb 1st,master,clk low when idle,
  //sample on leading edge of clk,system clock/4 rate (fastest)
  SPCR = (1 << SPE) | (1 << MSTR);
  clr = SPSR;
  clr = SPDR;
  delay(10);
  
//  //fill buffer with data
//  fill_buffer();
//  
//  //fill eeprom w/ buffer
//  digitalWrite(SLAVESELECT,LOW);
//  spi_transfer(WREN); //write enable
//  digitalWrite(SLAVESELECT,HIGH);
//  delay(10);
//  digitalWrite(SLAVESELECT,LOW);
//  spi_transfer(WRITE); //write instruction
//  address=0;
//  spi_transfer((char)(address>>8));   //send MSByte address first
//  spi_transfer((char)(address));      //send LSByte address
//  //write 128 bytes
//  for (int I = 0; I < 128; I++) {
//    spi_transfer(buffer[I]); //write data byte
//  }
//  digitalWrite(SLAVESELECT,HIGH); //release chip
//  //wait for eeprom to finish writing
//  delay(3000);
//  Serial.write('h');
//  Serial.write('i');
//  Serial.write('\n');//debug
//  delay(1000);

  /*// read manufacturer id and device id
  byte manufacturer_id;
  byte device_id;
  get_device_info(manufacturer_id, device_id);
  Serial.println(manufacturer_id, HEX);
  Serial.println(device_id, HEX);

  // read data
  read_block(0x000000, 16, 20, true);*/

  dump_rom(true);
}

byte read_eeprom(int EEPROM_address) {
  //READ EEPROM
  int data;
  digitalWrite(SLAVESELECT, LOW);
  spi_transfer(READ); //transmit read opcode
  spi_transfer((char)(EEPROM_address >> 8));   //send MSByte address first
  spi_transfer((char)(EEPROM_address));      //send LSByte address
  data = spi_transfer(0xFF); //get data byte
  digitalWrite(SLAVESELECT, HIGH); //release chip, signal end transfer
  return data;
}

void read_block(long starting_address, int block_size, int block_count, boolean fastread) {
  // read data
  for (long i = starting_address; i < block_size * block_count; i += block_size) { // 16 bytes in a row
    byte addrByte2 = (i >> 16) & 0x0000FF;
    byte addrByte1 = (i >> 8) & 0x0000FF;
    byte addrByte0 = (i) & 0x0000FF;
    
    digitalWrite(SLAVESELECT, LOW);
    if (fastread)
      spi_transfer(FASTREAD);
    else
      spi_transfer(READ);
    spi_transfer(addrByte2);
    spi_transfer(addrByte1);
    spi_transfer(addrByte0);
    if (fastread)
      spi_transfer(0xFF); // adding dummy clock for fast read

    byte data[block_size];
    char buf[block_size * 2 + block_size + 8]; 

    int length = 0;
    length += sprintf(buf + length, "%06X\t", (int)i);
  
    for (int j = 0; j < block_size; j++) {
      data[j] = spi_transfer(0xFF);
      length += sprintf(buf + length, " %02X", data[j]);
    }
    digitalWrite(SLAVESELECT, HIGH);

    Serial.println(buf);
  }
}

void dump_rom(boolean fastread) {
  while (!Serial.available()) {}
  char ch = (char)Serial.read();

  if (ch == 'd') {
    // read data    
    digitalWrite(SLAVESELECT, LOW);
    if (fastread)
      spi_transfer(FASTREAD);
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
    digitalWrite(SLAVESELECT, HIGH);
    //Serial.println("EOF");
  }
}

void loop() {
//  eeprom_output_data = read_eeprom(address);
//  Serial.print(eeprom_output_data,HEX);
//  Serial.write('\n');
//  address++;
//  if (address == 128)
//    address = 0;
//  delay(1000); //pause for readability
}
