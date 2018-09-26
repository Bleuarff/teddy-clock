#include <EEPROM.h>

unsigned int dodo = 2, wakeup1 = 2, wakeup2 = 2;
bool winterTimeChangeDone;

void setup(){
  delay(1000);
  Serial.begin(115200);

  // int i = 123;
  // char c = 'e';
  // bool b = false;
  // byte by = 5;
  //
  // Serial.printf("i: %i\n", sizeof(i));
  // Serial.printf("c: %i\n", sizeof(c));
  // Serial.printf("b: %i\n", sizeof(b));
  // Serial.printf("by: %i\n", sizeof(by));
  EEPROM.begin(13);
  //
  // EEPROM.write(0, 1);
  EEPROM.put(1, (int)(20*60));
  EEPROM.put(5, (int)(7*60 + 15));
  EEPROM.put(9, (int)(9*60+30));

  bool res = EEPROM.commit();
  if (res == true)
    Serial.println("write OK");
  else
    Serial.println("Write failure");
}

void loop(){
  delay(5000);
  // winterTimeChangeDone = (EEPROM.read(0) & 0b00000001) == 1;
  //
  // Serial.printf("winter: %i\n", winterTimeChangeDone);
  //
  EEPROM.get(1, dodo);
  EEPROM.get(5, wakeup1);
  EEPROM.get(9, wakeup2);
  //
  Serial.printf("dodo: %i\nwake1: %i\nwake2: %i\n\n", dodo, wakeup1, wakeup2);
  // Serial.print("dodo: ");
  // Serial.println(dodo);
  // // Serial.print("w1: ");
  // // Serial.println(wakeup1);
  // // Serial.print("w2: ");
  // // Serial.println(wakeup2);
  // // Serial.println();
}
