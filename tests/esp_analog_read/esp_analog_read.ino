
void setup(){
  Serial.begin(115200);
}

void loop(){
  delay(2000);
  int val = analogRead(A0);
  Serial.printf("value: %i\n", val);
}
