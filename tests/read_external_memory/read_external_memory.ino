/*
Reads first bytes of 24LC256 and sends them to serial
*/

#include <Wire.h>
#define address 0x50
#define len 20

void setup(){
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(address);
  Wire.write(0);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(address, len);
  while(Wire.available()){
    byte b = Wire.read();
    Serial.print(b);
    Serial.print(' ');
  }
}

void loop(){
  delay(30000);
}
