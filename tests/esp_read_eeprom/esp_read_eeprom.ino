
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

  Serial.printf("Length: %i/%i: %i\n", hob, lob, length);

  length = 4000;
  // read content
  byte data[length];
  read(length, data);

  Serial.println("read done");
  for(int i = 0; i < length; i++){
    byte b = data[i];
    // Serial.printf("%i ", b);
    printHex(b);
    if (i % PAGE_SIZE == PAGE_SIZE - 1){
      Serial.println();
      delay(100);
    }
  }
  Serial.println("\n--end--");
}

void loop(){
  delay(30000);
}

void read(int length, byte* data){
  int i = 0;
  int pageCount = (length / PAGE_SIZE) + 1;

  Serial.printf("Pages #: %i\n", pageCount);
  // Serial.print(pageCount);

  for (int pc = 0; pc < pageCount; pc++){
    if (pageCount > 1)
      Serial.printf("%i ", pc);

    int readLength = (pc < pageCount - 1) ? PAGE_SIZE : length % PAGE_SIZE;
    // Serial.print(" - Read : ");
    // Serial.println(readLength);

    Wire.requestFrom(EEPROM_ADDRESS, readLength);
    while(Wire.available()) {
      byte b = Wire.read();
      data[i++] = b;
      // Serial.print(i);
      // Serial.print("/");
      // Serial.print(b);
      // Serial.print(" ");
    }
  }
  Serial.println("\n");
}

void printHex(byte val){
  byte d = val / 16;
  byte u = val % 16;
  Serial.print(conv(d));
  Serial.print(conv(u));
  Serial.print(" ");
}

char conv(byte b){
  char c;
  switch(b){
    case 0: c = '0'; break;
    case 1: c = '1'; break;
    case 2: c = '2'; break;
    case 3: c = '3'; break;
    case 4: c = '4'; break;
    case 5: c = '5'; break;
    case 6: c = '6'; break;
    case 7: c = '7'; break;
    case 8: c = '8'; break;
    case 9: c = '9'; break;
    case 10: c = 'A'; break;
    case 11: c = 'B'; break;
    case 12: c = 'C'; break;
    case 13: c = 'D'; break;
    case 14: c = 'E'; break;
    case 15: c = 'F'; break;
  }
  return c;
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
