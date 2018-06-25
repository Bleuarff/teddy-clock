/*****************************

 Simple code to write data to arduino EEPROM

*******************************/

#include <EEPROM.h>

struct Alarm {
  byte hour;
  byte min;  
};

void setup() {
  Serial.begin(9600);

  Alarm dodo = {20, 0};
  Alarm time1 = {7, 0};
  Alarm time2 = {7, 30};

  EEPROM.put(1, dodo);
  EEPROM.put(3, time1);
  EEPROM.put(5, time2);

}

void loop() {
  Alarm dodo, a1, a2;
  EEPROM.get(1, dodo);
  EEPROM.get(3, a1);
  EEPROM.get(5, a2);

  print(dodo);
  print(a1);
  print(a2);

  delay(5 * 60 * (long)1000);
}

void print(Alarm a){
  Serial.print(a.hour);
  Serial.print(":");
  Serial.print(a.min);
  Serial.print("\n");
}

