#include <Wire.h>
#include <EEPROM.h>

const uint8_t RTC_ADDRESS = 0x68; // i2C address of the DS3231

// All possible states for the led
enum LedStates {Dodo, Debout, Off};

const byte redPin = 14;
const byte greenPin = 13;
const byte bluePin = 12;

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
  setTime((Time){19, 59, 25, 9, 3});

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
  const LedStates lightState = getLightState(t);
  printState(lightState);

  ModFactor mofact = getModFactor(); // get modulation factor for each rgb channel from light sensor value
  setLed(lightState, mofact); // update led state

  delay(5000);
}

// Write RGB values from clockstate
// state: clock state
void setLed(LedStates state, ModFactor mofact){
  int red, green, blue; // pwm value for each pin

  switch(state){
    case Off:
      red = green = blue = 0;
      break;
    case Dodo:
      red = green = 0;
      blue = 255 * mofact.bf;
      break;
    case Debout:
      blue = 0;
      green = 255 * mofact.gf;
      red = 128 * mofact.rf;
      break;
  }
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);

  // Serial.print(red);
  // Serial.print(" ");
  // Serial.print(green);
  // Serial.print(" ");
  // Serial.print(blue);
  // Serial.print("\n");
  Serial.printf("red: %i\tgreen: %i\tblue: %i\n", red, green, blue);
}

// return the state to apply to the led based on current time & global alarms
// alarms are assumed to be ordered: wakeup1 < wakeup2 < off < dodo
LedStates getLightState(Time t){
  int ts = t.hour * 60 + t.min; // convert time struct into a single number, number of minutes since 00:00
  int wakeup = t.dow <= 5 ? wakeup1 : wakeup2; // sets which wakeup time to use, week or weekend
  int offTime = wakeup + 120; // switch light off 2 hours after wakeup time

  if (ts < wakeup)
    return Dodo;
  else if (ts < offTime)
    return Debout;
  else if (ts < dodo)
    return Off;

  return Dodo;
}

// set modulation factor from light sensor
// Currently: linear relationship between ambiant light value and output led value, for all color channels
// TODO: calibrate: min/max value for min/max ambiant light values. See calibrage_photores sketch
ModFactor getModFactor(){
  int lightLvl = analogRead(A0);
  float lightRatio = (float)lightLvl / 1024;
  return { lightRatio, lightRatio, lightRatio };
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

void printState(LedStates l){
  Serial.print("Apply: ");
  switch (l){
    case Dodo:
      Serial.print("dodo\n");
      break;
    case Debout:
      Serial.print("debout\n");
      break;
    case Off:
      Serial.print("off\n");
      break;
  }
}
