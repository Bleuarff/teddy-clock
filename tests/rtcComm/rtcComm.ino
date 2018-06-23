#include <Wire.h>
#define DS3231_ADDR 0x68

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(DS3231_ADDR);
  Wire.write(0);
  Wire.write(128);
  Wire.write(dec2Bcd(50));
  Wire.write(dec2Bcd(23));
  Wire.write(dec2Bcd(2));
  Wire.write(dec2Bcd(19));
  Wire.write(dec2Bcd(6));
  Wire.write(dec2Bcd(18));
  Wire.endTransmission();

}

byte bcdToDec(byte val)  {
  // Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

byte dec2Bcd(byte val)  {
  // Convert binary coded decimal to normal decimal numbers
  return ( (val/10*16) + (val%10) );
}

void loop() {

  // Reset the register pointer
  Wire.beginTransmission(DS3231_ADDR);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_ADDR, 7, true);

  while(Wire.available() /*&& c < 7*/)    // slave may send less than requested
  {
    int n = bcdToDec(Wire.read());    // receive a byte as character
    if (n < 10)
      Serial.print('0');
    Serial.print(n);         // print the character
    Serial.print(" ");
  }

  Serial.print("\n");



  delay(5000);
}
