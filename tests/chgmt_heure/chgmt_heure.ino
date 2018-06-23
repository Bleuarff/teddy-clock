#include <Wire.h>
#include <EEPROM.h>
#define RTC_ADDRESS 0x68

struct Time {
  byte s;
  byte m;
  byte h;
  byte dow;
  byte date;
  byte month;
};

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0);
  Wire.write(dec2Bcd(50));
  Wire.write(dec2Bcd(59));
  Wire.write(dec2Bcd(23));
  Wire.write(dec2Bcd(0));
  Wire.write(dec2Bcd(28));
  Wire.write(dec2Bcd(2));
  Wire.write(dec2Bcd(18));
  Wire.endTransmission();

  //EEPROM.update(0, 0); // set

}

void loop() {
  // set register pointer
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  // get data from rtc, set Time struct
  Wire.requestFrom(RTC_ADDRESS, 6, true);

  byte c = 0, h,m, s, d, dow, month;
  while(Wire.available()){
    byte n = bcd2Dec(Wire.read());
    switch(c++){
      case 0:
        s = n;
        break;
      case 1:
        m = n;
        break;
      case 2:
        h = n;
        break;
      case 3:
        dow = n;
        break;
      case 4:
        d = n;
        break;
      case 5:
        month = n;
        break;
    }
  }

  Time t = {
    s, m, h, dow, d, month
  };

  printDate(t);
  checkTimeChange(t);

  delay(5000);
}

void checkTimeChange(Time t){
  // summer time change at 2AM on the last sunday of March
  if (t.h == 2 && t.month == 3 && t.dow == 0 && t.date >= 25 && t.date <= 31){
    Serial.print("forward 1h...");
    Wire.beginTransmission(RTC_ADDRESS);
    Wire.write(0x02); // set pointer to hour register
    Wire.write(3);
    Wire.endTransmission();
    // CAUTION when other flags are used
    EEPROM.update(0, 0); // reset winter time change flag
    Serial.print(" Done\n");
  }
  // winter time change at 3AM on the last sunday of October
  else if (t.h == 3 && t.month == 10 && t.dow == 0 && t.date >= 25 && t.date <= 31){
    // request first byte from EEPROM where the LSB is the flag that indicate whether it's been done already
    byte flag = EEPROM.read(0);
    if ((flag & 0b00000001) == 1){
      Serial.print("already done");
      return;
    }
    Serial.print("back 1h...");
    Wire.beginTransmission(RTC_ADDRESS);
    Wire.write(0x02); // set pointer to hour register
    Wire.write(2);
    Wire.endTransmission();
    EEPROM.update(0, 1); // set flag that winter time change is done
    Serial.print("Done\n");
  }
}

void printDate(Time t){

  pad(t.h);
  Serial.print(":");
  pad(t.m);
  Serial.print(":");
  pad(t.s);
  Serial.print(" ");
  pad(t.date);
  Serial.print("/");
  pad(t.month);
  Serial.print(" ");

  switch(t.dow){
    case 0:
      Serial.print("Dim"); break;
    case 1:
      Serial.print("Lun"); break;
    case 2:
      Serial.print("Mar"); break;
    case 3:
      Serial.print("Mer"); break;
    case 4:
      Serial.print("Jeu"); break;
    case 5:
      Serial.print("Ven"); break;
    case 6:
      Serial.print("Sam"); break;
  }
  Serial.print("\n");
}

void pad(int v){
  if (v < 10)
    Serial.print("0");
  Serial.print(v);
}

byte bcd2Dec(byte val)  {
  // Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

byte dec2Bcd(byte val)  {
  // Convert decimal to BCD format
  return ( (val/10*16) + (val%10) );
}
