#include <Wire.h>
#include <EEPROM.h>


void setup(){
  Serial.begin(115200);

  EEPROM.begin(13);
  EEPROM.write(0, 0); // reset winter time flag
  // set hours in memory
  EEPROM.put(1, (int)1170);
  EEPROM.put(5, (int)420);
  EEPROM.put(9, (int)510);
  EEPROM.commit();

}

void loop(){
  int dodo, wakeup1, wakeup2;
  EEPROM.read(1, dodo);
  EEPROM.read(5, wakeup1);
  EEPROM.read(9, wakeup2);

  Serial.printf("dodo time: %i\n", dodo);
  Serial.printf("wakeup1 time: %i\n", wakeup1);
  Serial.printf("wakeup2 time: %i\n", wakeup2);

  delay(1000 * 60);
}
