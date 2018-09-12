/*
Writes some content to the 24LC256 and reads it back
*/

#include <Wire.h>

#define PAGE_SIZE 30

#define EEPROM_ADDRESS 0x50 // i2C address of the 24LC256 eeprom


void setup(){
  Serial.begin(9600);
  Wire.begin();

  // int len = 70;
  // byte content[len] = {49,50,51,52,53,54,55,56,57,48,49,50,51,52,53,54,55,56,57,48,49,50,51,52,53,54,55,56,57,48, 65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,97,98,99,100,101,102,103,104,105,106};
  int len = 66;
  byte content[len] = {49,50,51,52,53,54,55,56,57,48,49,50,51,52,53,54,55,56,57,48,49,50,51,52,53,54,55,56,57,48, 65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,97,98,99,100,101,102};


  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write(0x00); // adress high order byte
  Wire.write(0x02); // address low order byte

  // byte buffer[PAGE_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  // write content
  for (int i = 0; i < len; i++){
    Wire.write(content[i]);
    // buffer[i % PAGE_SIZE] = content[i];
    Serial.print(content[i]);
    Serial.print(" ");

    // buffer full, stop, write data and open new connection
    if (((i + 1) % PAGE_SIZE == 0) || ((i+3) % 64 == 0) ){
      // Wire.write(buffer, PAGE_SIZE);
      Wire.endTransmission();
      // showBuffer(buffer);
      // compute new address
      byte hob = ((i+3) & 0xff00) >> 8;
      byte lob = (i+3) & 0x00ff;
      Serial.print("\nwrite page. Next page address: ");
      Serial.print(hob);
      Serial.print("/");
      Serial.println(lob);
      // for(int k = 0; k < PAGE_SIZE; k++)
      //   buffer[k] = 0;

      i2cReady(EEPROM_ADDRESS);
      Wire.beginTransmission(EEPROM_ADDRESS);
      Wire.write(hob);
      Wire.write(lob);
    }
  }
  // Wire.write(buffer, PAGE_SIZE);
  Wire.endTransmission();
  // showBuffer(buffer);

  // split length int into 2 bytes
  byte lob = len & 0x00ff;
  byte hob = (len & 0xff00) >> 8;

  Serial.print("\nLength: ");
  Serial.print(hob);
  Serial.print("/");
  Serial.println(lob);

  i2cReady(EEPROM_ADDRESS);

  // write content length
  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write(0x00); // address hob
  Wire.write(0x00); //address lob

  // and write them
  Wire.write(hob);
  Wire.write(lob);
  Wire.endTransmission();

  // i2cReady(EEPROM_ADDRESS);
  Serial.println("--Write ok--");
}

void read(){
  Serial.println("\nMemory dump:");

  i2cReady(EEPROM_ADDRESS);
  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write(0);
  Wire.write(0);
  Wire.endTransmission();
  i2cReady(EEPROM_ADDRESS);
  Wire.requestFrom(EEPROM_ADDRESS, 2);
  while(Wire.available()){
    byte b = Wire.read();
    Serial.print(b);
    Serial.print(" ");
  }
  Serial.println();


  for(int i = 0; i < 3; i++){
    int address = i * 30 + 2;
    // Wire.beginTransmission(EEPROM_ADDRESS);
    // Wire.write((address & 0xff00) >> 8);
    // Wire.write(address & 0x00ff);
    // Wire.endTransmission();

    // Serial.print("read length ");
    // Serial.println(readLength);
    // Serial.print(address);
    // Serial.print(": ");
    //
    // i2cReady(EEPROM_ADDRESS);

    Wire.requestFrom(EEPROM_ADDRESS, 30);
    while(Wire.available()){
      byte b = Wire.read();
      Serial.print(b);
      Serial.print(" ");
    }

    Serial.println();
  }

}


// int read(){
//   // set pointer at address 0x0000
//   Wire.beginTransmission(EEPROM_ADDRESS);
//   Wire.write(0); // address hob
//   Wire.write(0); //address lob
//   Wire.endTransmission();
//
//   Wire.requestFrom(EEPROM_ADDRESS, 2);
//   int len = 0;
//   int c = 0;
//   byte lenarray[2];
//
//   while(Wire.available()){
//     byte b = Wire.read();
//     lenarray[c++] = b;
//   }
//
//   byte hob = lenarray[0];
//   byte lob = lenarray[1];
//   len = (hob << 8) | lob;
//
//   Serial.print("hob: ");
//   Serial.print(hob);
//   Serial.print("  lob: ");
//   Serial.print(lob);
//
//   Serial.print(" length found: ");
//   Serial.println(len);
//
//   if (len > 0){
//     Wire.requestFrom(EEPROM_ADDRESS, len);
//     byte found[len];
//     c = 0;
//     while (Wire.available()){
//       byte b = Wire.read();
//       found[c++] = b;
//       Serial.print(b);
//     }
//   }
//   else{
//     Serial.println("Length 0, nothing to read");
//   }
//
//   return len;
// }

void loop(){
  // delay(1000);
  read();
  delay(300000);
}

bool i2cReady(uint8_t adr){
  uint32_t timeout=millis();
  bool ready=false;
  while((millis()-timeout<100)&&(!ready)){
    Wire.beginTransmission(adr);
    int err=Wire.endTransmission();
    ready=(err==0);
  }
  // Serial.print("ready:");
  // Serial.print(ready);
  return ready;
}

void showBuffer(byte buf[]){
  for(int j = 0; j < PAGE_SIZE; j++){
    Serial.print(buf[j]);
    Serial.print(" ");
  }
  Serial.println();
}
