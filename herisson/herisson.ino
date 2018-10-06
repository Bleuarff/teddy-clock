#include <Wire.h>
#include <EEPROM.h>
#include "FS.h"
#include <ESP8266WebServer.h>

const uint8_t RTC_ADDRESS = 0x68; // i2C address of the DS3231

// All possible states for the led
enum LedStates {Dodo, Debout, Off};

const byte redPin = 14;
const byte greenPin = 13;
const byte bluePin = 12;
const byte switchPin = 16;
const int CLOCK_CHECK_INTERVAL = 5000; // time between clock checks

// Object to store timestamp
// seconds are not needed.
struct Time {
  byte hour;
  byte min;
  byte date;
  byte month;
  byte dow;
  byte year;
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

// timestamp of last time we checked time & set led according to it
unsigned long lastClockCheck;

// server object
ESP8266WebServer server(80);

void setup(){
  Serial.begin(115200);
  Wire.begin();

  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(switchPin, INPUT);

  setAirplaneMode();

  // for dev/tests: set RTC
  // setTime((Time){19, 59, 25, 9, 3, 18});

  EEPROM.begin(13); // 13 bytes needed. cf readme for usage/Layout

  winterTimeChangeDone = (EEPROM.read(0) & 0b00000001) == 1;
  Serial.print("Winter time change done: ");
  Serial.println(winterTimeChangeDone);

  // get all alarms stored in EEPROM
  EEPROM.get(1, dodo);
  EEPROM.get(5, wakeup1);
  EEPROM.get(9, wakeup2);

  printAlarm(dodo);
  printAlarm(wakeup1);
  printAlarm(wakeup2);

  if (!SPIFFS.begin()){
    Serial.println("SPIFFS mount fail.");
  }

  lastClockCheck = millis();
}

void loop(){
  static unsigned long lastWifiSwitch = 0;
  static bool isUpdateMode = false; // true when switch is pressed, until next press
  static bool serverOK = false;

  int switchState = digitalRead(switchPin);

  // act upon switch pressed if not too close from last press. Avoids 1 press to be detected multiple times (a kind of debounce)
  if (switchState == HIGH && millis() - lastWifiSwitch >= 3000){
    isUpdateMode = !isUpdateMode;

    if (isUpdateMode){
      // start wifi & server
      serverOK = startServer();
    }
    else{
      // switch off wifi
      bool res = stopServer();
      // led red if error
      if (!res){
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);
        digitalWrite(bluePin, LOW);
        delay(5000);
      }
    }

    lastWifiSwitch = millis(); // debounce: keep track of last time we registered a switch between clock check and update mode
  }

  // in update mode, blink green if wifi & server are set up ok. Otherwise show red
  if (isUpdateMode){
    server.handleClient();
    if (serverOK)
      blink(1);
    else{
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
    }
  }

  // check time & set led if update mode, (i.e. server) is inactive.
  // also use millis & time compare instead of delay, for non-blocking code
  if (!isUpdateMode && millis() - lastClockCheck >= CLOCK_CHECK_INTERVAL){
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

    lastClockCheck = millis();
  }
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
  Serial.println(min);
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
  Serial.printf(" 20%i\n", t.year);
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

// color: specify which color to set when blinking:
// 0: red
// 1: green
// 2: blue
void blink(byte color){
  static unsigned long lastBlink = millis();
  static bool blinkOn = false;

  if (millis() - lastBlink >= 800){
    blinkOn = !blinkOn;

    if (blinkOn){

      digitalWrite(redPin, color == 0 ? HIGH : LOW);
      digitalWrite(greenPin, color == 1 ? HIGH : LOW);
      digitalWrite(bluePin, color == 2 ? HIGH : LOW);
    }
    else{
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
    }

    lastBlink = millis();
  }
}
