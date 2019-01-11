#define DATAOUT 11//MOSI
#define DATAIN  12//MISO
#define SPICLOCK  13//sck
#define SLAVESELECT 10//ss

//opcodes
#define WREN  0x06
#define WRDI  0x04
#define RDSR  0x05
#define WRSR  0x01
#define READ  0x03
#define WRITE 0x02

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

void setup() {
  Serial.begin(9600);

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

  // read manufacturer id and device id
  digitalWrite(SLAVESELECT, LOW);
  spi_transfer(0x90);
  spi_transfer(0x00);
  spi_transfer(0x00);
  spi_transfer(0x00);
  byte man = spi_transfer(0xFF); //get data byte
  byte dev = spi_transfer(0xFF); //get data byte
  digitalWrite(SLAVESELECT, HIGH);
  delay(10);
  Serial.println(man, HEX);
  Serial.println(dev, HEX); 

  // read data
  digitalWrite(SLAVESELECT, LOW);
  spi_transfer(READ);
  spi_transfer(0x00);
  spi_transfer(0x00);
  spi_transfer(0x00);
  byte data = spi_transfer(0xFF);
  digitalWrite(SLAVESELECT, HIGH);
  Serial.println(data, HEX);
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

void loop() {
//  eeprom_output_data = read_eeprom(address);
//  Serial.print(eeprom_output_data,HEX);
//  Serial.write('\n');
//  address++;
//  if (address == 128)
//    address = 0;
//  delay(1000); //pause for readability
}
