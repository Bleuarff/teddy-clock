#include <Wire.h>
#include <EEPROM.h>

#define RTC_ADDRESS 0x68 // i2C address of the DS3231

// Object to store timestamp
// seconds are not needed.
struct Time {
  byte hour;
  byte min;
  byte date;
  byte month;
  byte dow;
};

// object to store an alarm, point in time at which an action must be performed
struct Alarm {
  byte hour;
  byte min;
};

// 3 alarms needed
Alarm dodo, wakeup1, wakeup2;

void setup(){
  Serial.begin(115200); // test 115200
  Wire.begin();

  // for dev/tests: set RTC
  setTime((Time){15, 10, 24, 6, 6});

  // get all alarms stored in EEPROM
  EEPROM.get(1, dodo);
  EEPROM.get(3, wakeup1);
  EEPROM.get(5, wakeup2);

  printAlarm(dodo);
  printAlarm(wakeup1);
  printAlarm(wakeup2);
}

void loop(){
  // get time & check action to do (time change, light on/off)
  Time t = getTime();
  printTime(t);
  // get light sensor value and set output brightness
  
  delay(5000);
}

// displays alarm
void printAlarm(Alarm a){
  Serial.print(a.hour);
  Serial.print(":");
  Serial.print(a.min);
  Serial.print("\n");
}

void printTime(Time t){
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(" ");
  Serial.print(t.date);
  Serial.print("/");
  Serial.print(t.month);
  Serial.print(" ");
  Serial.print(t.dow);
  Serial.print("\n");
}


