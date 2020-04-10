#include <Wire.h>
#include <EEPROM.h>

const uint8_t RTC_ADDRESS = 0x68; // i2C address of the DS3231

void setup(){
  Serial.begin(115200);

  EEPROM.begin(13);
  EEPROM.write(0, 0); // reset winter time flag
  // set hours in memory
  EEPROM.put(1, (int)1170);
  EEPROM.put(5, (int)420);
  EEPROM.put(9, (int)510);
  EEPROM.commit();

  Wire.begin();
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0); // set register
  Wire.write(0); // seconds
  Wire.write(0); // minutes
  Wire.write(0); // hours
  Wire.write(1); // day of week (1-7)
  Wire.write(9); // date (1-31)
  Wire.write(4); // month (1-12)
  Wire.write(0b00100000); // year (20 in BCD format)
  Wire.endTransmission();
}

void loop(){
  int dodo, wakeup1, wakeup2;
  EEPROM.get(1, dodo);
  EEPROM.get(5, wakeup1);
  EEPROM.get(9, wakeup2);

  Serial.printf("dodo time: %i\n", dodo);
  Serial.printf("wakeup1 time: %i\n", wakeup1);
  Serial.printf("wakeup2 time: %i\n", wakeup2);

  getTime();

  delay(1000 * 60);
}

void getTime(){
  // set register pointer (at 1, ignore the seconds)
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x01);
  Wire.endTransmission();

  Wire.requestFrom(RTC_ADDRESS, (uint8_t)6);
  while(Wire.available()){
    byte n = bcd2Dec(Wire.read());
    Serial.print(n);
    Serial.print(" ");
  }
  Serial.print("\n");
}

// Convert binary coded decimal to normal decimal numbers
byte bcd2Dec(byte val)  {
  return ( (val / 16 * 10) + (val % 16) );
}
