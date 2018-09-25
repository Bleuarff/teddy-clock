
int pin = 12;

void setup(){
  pinMode(pin, OUTPUT);
}

void loop(){
  analogWrite(pin, 1023);
  // digitalWrite(pin, HIGH);
  delay(1000);
  analogWrite(pin, 512);
  // digitalWrite(pin, LOW);
  delay(1000);
}
