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
  Wire.requestFrom(RTC_ADDRESS, (uint8_t)6);
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
      case 6:
        t.year = n;
        break;
    }
  }

  if (c == 6)
    return checkTimeChange(t);
  else
    return {0, 0, 0, 0, 0, 0};
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
  Wire.write(dec2Bcd(t.year));
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

// detects when a time change is needed and updates the DS3231.
Time checkTimeChange(Time t){
  // summer time change at 2AM on the last sunday of March
  if (t.hour == 2 && t.month == 3 && t.dow == 7 && t.date >= 25 && t.date <= 31){
    Serial.print("forward 1h...");
    t.hour = 3;
    Wire.beginTransmission(RTC_ADDRESS);
    Wire.write(0x02); // set pointer to hour register
    Wire.write(t.hour); // no need for bcd encoding since value is single-digit and higher bits are already 0.
    Wire.endTransmission();

    // reset winter time change flag, without changing other (future) bits
    byte flags = EEPROM.read(0) & 0b11111110;
    EEPROM.write(0, flags);
    EEPROM.commit();
    winterTimeChangeDone = false;
  }
  // winter time change at 3AM on the last sunday of October
  else if (t.hour == 3 && t.month == 10 && t.dow == 7 && t.date >= 25 && t.date <= 31){
    // request first byte from EEPROM where the LSB is the flag that indicate whether it's been done already
    if (winterTimeChangeDone){
      Serial.print("already done");
    }
    else{
      t.hour = 2;
      Serial.print("back 1h...");
      Wire.beginTransmission(RTC_ADDRESS);
      Wire.write(0x02); // set pointer to hour register
      Wire.write(t.hour);
      Wire.endTransmission();

      // sets winter time change flag to 1 while keeping other bits' value
      byte flags = EEPROM.read(0) | 0b00000001;
      EEPROM.write(0, flags);
      EEPROM.commit();
      winterTimeChangeDone = true;
    }
  }

  return t;
}
