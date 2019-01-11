#define CLK 3
#define DO 4
#define CS_BAR 5
#define WP_BAR 6
#define DIO 7
#define HOLD_BAR 8

void write_enable() {
  digitalWrite(CS_BAR, LOW);
  shiftOut(DIO, CLK, MSBFIRST, 0x06);
  digitalWrite(CS_BAR, HIGH);
}

void write_disable() {
  digitalWrite(CS_BAR, LOW);
  pinMode(DIO, OUTPUT);
  shiftOut(DIO, CLK, MSBFIRST, 0x04);
  digitalWrite(CS_BAR, HIGH);
}

byte read_status_register() {
  digitalWrite(CS_BAR, LOW);
  pinMode(DIO, OUTPUT);
  shiftOut(DIO, CLK, MSBFIRST, 0x05);
  digitalWrite(CLK, HIGH);
  byte stat = shiftIn(DO, CLK, MSBFIRST);
  digitalWrite(CS_BAR, HIGH);
  return stat;
}

void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(DO, INPUT);
  pinMode(CS_BAR, OUTPUT);
  pinMode(WP_BAR, OUTPUT);
  pinMode(DIO, OUTPUT); // Initially OUTPUT, but used for both I/O operations
  pinMode(HOLD_BAR, OUTPUT);

  // W25X80A is clocked by rising edges i.e. both mode0 and mode3 are supported
  digitalWrite(CLK, LOW); // We'll use SPI mode0
  
  Serial.begin(9600);

  //write_enable();

//  byte stat = read_status_register();
//  char buf[80];
//  sprintf(buf, "status: %02x", stat);
//  Serial.println(buf);

  /* 10.2.16 - Read Manufacturer / Device ID (90h) */
  digitalWrite(CS_BAR, LOW);
  delayMicroseconds(20);
  digitalWrite(HOLD_BAR, HIGH);
  digitalWrite(WP_BAR, LOW);
  
  digitalWrite(CLK, LOW);
  shiftOut(DIO, CLK, MSBFIRST, 0x90);
  shiftOut(DIO, CLK, MSBFIRST, 0x00);
  shiftOut(DIO, CLK, MSBFIRST, 0x00);
  shiftOut(DIO, CLK, MSBFIRST, 0x00);

  digitalWrite(CLK, HIGH);
  for (int i = 0; i < 8; i++) {
      if (i % 2 == 0) digitalWrite(CLK, LOW);
      else digitalWrite(CLK, HIGH);

      Serial.println(digitalRead(DO));
  }
  digitalWrite(CS_BAR, HIGH);

//  digitalWrite(CLK, HIGH);
//  byte ManufacturerID = shiftIn(DO, CLK, MSBFIRST);
//  byte DeviceID = shiftIn(DO, CLK, MSBFIRST);
//  
//  digitalWrite(CS_BAR, HIGH);
//
//  char buf[80];
//  sprintf(buf, "ManufacturerID: %02x, DeviceID: %02x", ManufacturerID, DeviceID);
//  
//  Serial.println(buf);
}


void loop() {
//  pinMode(CS_BAR, OUTPUT);
//  pinMode(HOLD_BAR, OUTPUT);
//  pinMode(WP_BAR, OUTPUT);
//  pinMode(CLK, OUTPUT);
//  pinMode(DO, OUTPUT);
//  pinMode(DIO, OUTPUT);
//  digitalWrite(CS_BAR, HIGH);
//  digitalWrite(HOLD_BAR, HIGH);
//  digitalWrite(WP_BAR, HIGH);  
//  digitalWrite(CLK, HIGH);
//  digitalWrite(DO, HIGH);
//  digitalWrite(DIO, HIGH);  
}
