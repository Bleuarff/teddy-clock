const int ledRed = 6;
const int ledGreen = 3;
const int ledBlue = 5;
const int sensorPin = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledRed, OUTPUT);
  //pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  Serial.begin(9600);
}


void writeColor(int red, int green, int blue){
  analogWrite(ledRed, red);
  analogWrite(ledGreen, green);
  analogWrite(ledBlue, blue);
}

void loop() {
  //writeColor(0, 0, 255);
  //delay(2000);

  //writeColor(64, 128, 0);
  //delay(2000);


  int lightLvl = analogRead(sensorPin);
  Serial.print("\nlight: ");
  Serial.print(lightLvl);
  
  int blueLvl = lightLvl / 4;
  blueLvl = max(blueLvl, 10);
  Serial.print(" - ");
  Serial.print(blueLvl);

  writeColor(0, 0, blueLvl);
  delay(2000);

  int greenLvl = lightLvl / 4;
  greenLvl = max(greenLvl, 20);
  Serial.print(" - ");
  Serial.print(greenLvl);
  
  writeColor(greenLvl / 2, greenLvl, 0);
  
  delay(2000);

}
