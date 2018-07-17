// TODO: Send file data (utf-8 ?), write each byte to eeprom, starting at offset 2.
// when file complete, get length and write to eeprom at offset 0

unsigned int numReceived = 0; // counter of bytes received
boolean newData = false;

void setup(){
  Serial.begin(9600); // slower baud rate to ease pressure on code execution time
}

void loop(){
  receive();
  show();
}

void receive(){
  static boolean recvInProgress = false;
  static byte ndx = 0;
  byte startMarker = 0x02;
  byte endMarker = 0x03;
  byte rb;

  while (Serial.available() > 0 && newData == false){
    rb = Serial.read();

    if (recvInProgress == true){
      if (rb != endMarker){
        ndx++;
        // TODO: write byte to eeprom
      }
      else{
        recvInProgress = false;
        numReceived = ndx;
        ndx = 0;
        newData = true;
      }
    }
    else if (rb == startMarker){
      recvInProgress = true;
    }
  }
}

void show(){
  if (newData == true){
    Serial.print("data in...");
    Serial.print(numReceived);
    Serial.println(" bytes received");
    newData = false;
  }
}
