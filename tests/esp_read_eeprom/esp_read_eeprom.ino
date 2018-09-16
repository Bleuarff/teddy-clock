
#include <Wire.h>

const int EEPROM_ADDRESS = 0x50;
const int PAGE_SIZE = 32;

void setup(){
  Serial.begin(9600);
  delay(1000);
  Wire.begin(4, 5);

  delay(1000);

  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write(0);
  Wire.write(0);
  int wr = Wire.endTransmission();
  Serial.print("Write result: ");
  Serial.println(wr);

  // i2cReady(EEPROM_ADDRESS);
  delay(500);

  byte lenA[2] = {0, 0};

  // read content length
  read(2, lenA);
  byte hob = lenA[0];
  byte lob = lenA[1];
  int length = (hob << 8) | lob;

  Serial.print("Length: ");
  Serial.print(hob);
  Serial.print("/");
  Serial.print(lob);
  Serial.print(": ");
  Serial.println(length);

  // read content
  byte data[length];
  read(length, data);

  for(int i = 0; i < length; i++){
    byte b = data[i];
    Serial.printf("%i ", b);
    if (i % PAGE_SIZE == 0);
      Serial.println();
  }
}

void loop(){
  delay(30000);
}

void read(int length, byte* data){
  int i = 0;
  int pageCount = (length / PAGE_SIZE) + 1;

  Serial.printf("Pages #: %i", pageCount);
  // Serial.print(pageCount);

  for (int pc = 0; pc < pageCount; pc++){
    if (pageCount > 1)
      Serial.printf("\np %i", pc);

    int readLength = (pc < pageCount - 1) ? PAGE_SIZE : length % PAGE_SIZE;
    // Serial.print(" - Read : ");
    // Serial.println(readLength);

    Wire.requestFrom(EEPROM_ADDRESS, readLength);
    while(Wire.available()) {
      byte b = Wire.read();
      // data[i++] = b;
      // Serial.print(i);
      // Serial.print("/");
      Serial.print(b);
      Serial.print(" ");
    }
  }
  Serial.println("\n");
}

// bool i2cReady(uint8_t adr){
//   uint32_t timeout=millis();
//   bool ready=false;
//   while((millis()-timeout<100)&&(!ready)){
//     Wire.beginTransmission(adr);
//     int err=Wire.endTransmission();
//     ready=(err==0);
//   }
//   return ready;
// }
