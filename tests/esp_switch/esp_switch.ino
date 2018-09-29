
void setup(){
  Serial.begin(115200);
  pinMode(16, INPUT);
}

void loop(){
  int state = digitalRead(16);
  Serial.println(state);
  if (state == HIGH)
    Serial.println("state HIGH");
  else
    Serial.println("state LOW");

  delay(3000);
}
