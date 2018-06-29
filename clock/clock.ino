#include <Wire.h>
#include <EEPROM.h>

#define RTC_ADDRESS 0x68 // i2C address of the DS3231

// All possible states for the led
enum LedStates {Dodo, Debout, Off};

const byte redPin = 6;
const byte greenPin = 3;
const byte bluePin = 5;
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
  byte rf; // red channel
  byte gf; // g channel
  byte bf; // b channel
};

// 3 alarms needed
int dodo, wakeup1, wakeup2;
bool winterTimeChangeDone;

void setup(){
  Serial.begin(115200);
  Wire.begin();

  // for dev/tests: set RTC
  setTime((Time){7, 29, 24, 6, 6});

  winterTimeChangeDone = (EEPROM.read(0) & 0b00000001) == 1;
  // get all alarms stored in EEPROM
  EEPROM.get(1, dodo);
  EEPROM.get(3, wakeup1);
  EEPROM.get(5, wakeup2);

  printAlarm(dodo);
  printAlarm(wakeup1);
  printAlarm(wakeup2);
}

void loop(){
  Time t = getTime(); // read time. Already adjusted for daylight savings

  if (t.month == 0){ // in case of error, getTime returns a Time with all members set to 0
    Serial.print("invalid time\n");
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

// set modulation factor from light sensor
ModFactor getModFactor(){
  int lightLvl = analogRead(sensorPin);
  //float lightRatio = (float)lightLvl / 1024;

}

// Write RGB values from clockstate
// state: clock state
//
void setLed(LedStates state, ModFactor mofact){
  int red, green, blue; // pwm value for each pin

  switch(state){
    case Off:
      red = green = blue = 0;
      break;
    case Dodo:
      red = green = 0;
      blue = 255 ;
      break;
    case Debout:
      blue = 0;
      green = 255;
      red = 128;
      break;
  }
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);

  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.print(blue);
  Serial.print("\n");
}

// void writeColor(int red, int green, int blue){
//
// }

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
