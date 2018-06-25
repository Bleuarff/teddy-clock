/*********************************************************

  Methods used to interact with DS3231 RTC

**********************************************************/

// reads time
Time getTime(){
  // set register pointer (at 1, ignore the seconds)
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x01);
  Wire.endTransmission();

  byte c = 0; // counter to know which Time member to set & check reading is complete
  Time t;

  // get data from rtc, set Time struct
  Wire.requestFrom(RTC_ADDRESS, 5, true);
  while(Wire.available()){
    byte n = bcd2Dec(Wire.read());
    switch(++c){
      case 1:
        t.min = n;
        break;
      case 2:
        t.hour = n;
        break;
      case 3:
        t.dow = n;
        break;
      case 4:
        t.date = n;
        break;
      case 5:
        t.month = n;
        break;
    }
  }

  if (c == 5)
    return t;
  else
    return {0, 0, 0, 0, 0};
}

// set time on DS3231
void setTime(Time t){
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0); // set register
  Wire.write(0); // seconds part
  Wire.write(dec2Bcd(t.min));
  Wire.write(dec2Bcd(t.hour));
  Wire.write(dec2Bcd(t.dow));
  Wire.write(dec2Bcd(t.date));
  Wire.write(dec2Bcd(t.month));
  Wire.write(dec2Bcd(18));
  Wire.endTransmission();
}

// Convert binary coded decimal to normal decimal numbers
byte bcd2Dec(byte val)  {
  return ( (val / 16 * 10) + (val % 16) );
}

// Convert binary coded decimal to normal decimal numbers
byte dec2Bcd(byte val)  {
  return ( (val / 10 * 16) + (val % 10) );
}

