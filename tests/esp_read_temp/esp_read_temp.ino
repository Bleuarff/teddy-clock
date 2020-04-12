#include <Wire.h>

const uint8_t RTC_ADDRESS = 0x68;

void setup(){
  Serial.begin(115200);
  Wire.begin();
}

void loop(){
  Serial.print("\n---Temp---\n");
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x11);
  Wire.endTransmission();

  byte c = 0;
  // don't care about decimal part, just retrieve the integer part of the temp
  Wire.requestFrom(RTC_ADDRESS, (uint8_t)1);
  // while(Wire.available()){
  if (Wire.available()){
    byte n = Wire.read();

    // Serial.printf("%i: ", c++);
    // Serial.print(n, HEX);
    // Serial.print("\n");
    if (n >= 128)
      Serial.print("Temp < 0\n");
    else
      Serial.printf("Temp: %i\n", n);
  }


  delay(1000*60);
}
