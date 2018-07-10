const byte sensorPin = A0;
const byte bluePin = 5;

void setup(){
  Serial.begin(115200);
}

// byte ledValue = 0;
void loop(){
  int lightLvl = analogRead(sensorPin);
  byte ledValue = (byte)(lightLvl / 4);

  Serial.print("Sensor value: ");
  Serial.print(lightLvl);
  Serial.print("    output: ");

  Serial.print(ledValue);
  Serial.print("\n");
  analogWrite(bluePin, ledValue++);


  delay(2000);
}
