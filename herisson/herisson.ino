#include <Wire.h>
#include <EEPROM.h>

const uint8_t RTC_ADDRESS = 0x68; // i2C address of the DS3231

// All possible states for the led
enum LedStates {Dodo, Debout, Off};

const byte redPin = 12;
const byte greenPin = 14;
const byte bluePin = 13;
const byte sensorPin = A0;

// Object to store timestamp
// seconds are not needed.
struct Time {
  byte hour;
  byte min;
  byte date;
  byte month;
  byte dow;
};

// Store light modulation factor for each rgb channel
struct ModFactor {
  float rf; // red channel
  float gf; // green channel
  float bf; // blue channel
};

// 3 alarms needed
int dodo, wakeup1, wakeup2;
bool winterTimeChangeDone;

void setup(){
  Serial.begin(115200);
  Wire.begin();

  // for dev/tests: set RTC
  // setTime((Time){7, 28, 24, 6, 6});

  EEPROM.begin(13); // 13 bytes needed. cf readme for usage/Layout

  winterTimeChangeDone = (EEPROM.read(0) & 0b00000001) == 1;

  // get all alarms stored in EEPROM
  EEPROM.get(1, dodo);
  EEPROM.get(5, wakeup1);
  EEPROM.get(9, wakeup2);

  printAlarm(dodo);
  printAlarm(wakeup1);
  printAlarm(wakeup2);
}

void loop(){
  Time t = getTime(); // read time. Already adjusted for daylight savings

  if (t.month == 0){ // in case of error, getTime returns a Time with all members set to 0
    Serial.print("invalid time\n");
    delay(5000);
    return;
  }

  printTime(t);

  // gets clock  state to apply
  // const LedStates lightState = getLightState(t);
  // printState(lightState);
  //
  // ModFactor mofact = getModFactor(); // get modulation factor for each rgb channel from light sensor value
  // setLed(lightState, mofact); // update led state

  delay(5000);
}

// displays alarm
void printAlarm(int a){
  byte hr = a / 60;
  byte min = a % 60;
  Serial.print(hr);
  Serial.print(":");
  Serial.print(min);
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
