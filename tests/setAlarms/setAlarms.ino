/*****************************

 Simple code to write data to arduino EEPROM

*******************************/

#include <EEPROM.h>

void setup() {
  Serial.begin(9600);

  int dodo = 20 * 60;
  int time1 = 7 * 60;
  int time2 = 7 * 60 + 30;

  EEPROM.put(1, dodo);
  EEPROM.put(3, time1);
  EEPROM.put(5, time2);

}

void loop() {
  int dodo, a1, a2;
  EEPROM.get(1, dodo);
  EEPROM.get(3, a1);
  EEPROM.get(5, a2);

  print(dodo);
  print(a1);
  print(a2);

  delay(5 * 60 * (long)1000);
}

void print(int a){
  byte hour = a / 60;
  byte min = a % 60;
  Serial.print(hour);
  Serial.print(":");
  Serial.print(min);
  Serial.print("\n");
}
