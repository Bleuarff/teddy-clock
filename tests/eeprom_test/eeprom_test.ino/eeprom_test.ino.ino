#include <EEPROM.h>

struct Time {
  int hour;
  int minutes;
};

void setup() {
  Serial.begin(9600);
  
  while (!Serial) {
    ;  //wait for serial port to connect. Needed for Leonardo only
  }
  
  int memsize = EEPROM.length();
  /*Serial.print("eeprom size: ");
  Serial.print(memsize);*/

  Time h1 = {
    07,
    15
  };

  Time h2 = {
    20,
    00
  };

  int addr = 0;
  EEPROM.put(addr, h1);

  addr += sizeof(Time);
  EEPROM.put(addr, h2);

  delay(1000);

  Time r1;
  Time r2;
  
  EEPROM.get(0, r1);
  EEPROM.get(sizeof(Time), r2);

  Serial.print(r1.hour);
  Serial.print(r1.minutes);

  Serial.print(r2.hour);
  Serial.print(r2.minutes);
}

void loop() {
  
}
