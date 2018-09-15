// Send file data, write each byte to eeprom, starting at offset 2.
// when file complete, get length and write to eeprom at offset 0. Then proceeds to read the content and send it back

// uses an RGB led for debugging. Blue: arduino started. Red: message incoming. Green: message finished

#include <Wire.h>
#define EEPROM_ADDRESS 0x50 // i2C address of the 24LC256 eeprom
#define BUFFER_SIZE 30 // Wire internal buffer is 32 bytes, of which 2 are for the address
#define PAGE_SIZE 64 // EEPROM page size

unsigned int numReceived = 0; // counter of bytes received
boolean newData = false;

void setup(){
  Serial.begin(9600); // slower baud rate to ease pressure on code execution time
  Wire.begin();

  // set led pins
  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);

  setLed(0,0,255);

  delay(100);
  i2cReady(0x50);
}

void loop(){
  receive();
  show();
}

void receive(){
  static boolean recvInProgress = false;
  static int ndx = 0;

  // start and end marker around message
  byte startMarker = 0x02;
  byte endMarker = 0x03;
  byte rb;

  while (Serial.available() > 0 && newData == false){
    rb = Serial.read();

    if (recvInProgress == true){
      if (rb != endMarker){
        ndx++;
        // store byte in buffer
        Wire.write(rb);

        // Serial.print(rb);
        // Serial.print(" ");

        // stop and send data when either Wire buffer limit is reached or to not cross page boundaries on the 24LC256
        if ((ndx % BUFFER_SIZE == 0) || ((ndx+2) % PAGE_SIZE == 0)){
          Wire.endTransmission();
          byte hob = ((ndx + 2) & 0xff00) >> 8;
          byte lob = (ndx + 2) & 0x00ff;

          // Serial.print("\nwrite page - new page address: ");
          // Serial.print(hob);
          // Serial.print("/");
          // Serial.println(lob);

          i2cReady(EEPROM_ADDRESS);

          // open connection for next page
          Wire.beginTransmission(EEPROM_ADDRESS);
          Wire.write(hob);
          Wire.write(lob);
        }
      }
      else{
        Serial.println("\nMessage end");
        recvInProgress = false;
        numReceived = ndx;
        ndx = 0;
        newData = true;
        setLed(0, 255,0);
        Wire.endTransmission();
        writeContentLength(numReceived);
      }
    }
    else if (rb == startMarker){
      Serial.println("Message start");
      recvInProgress = true;
      setLed(255,0,0);

      // open connection to memory chip and set address
      Wire.beginTransmission(EEPROM_ADDRESS);
      Wire.write(0);
      Wire.write(2);
    }
  }
}

void writeContentLength(int length){
  // split length int into 2 bytes
  byte lob = length & 0x00ff;
  byte hob = (length & 0xff00) >> 8;

  Serial.print("Write content length: ");
  Serial.print(length);
  Serial.print("  ");
  Serial.print(hob);
  Serial.print("/");
  Serial.println(lob);

  i2cReady(EEPROM_ADDRESS);
  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write(0);
  Wire.write(0);

  // write length bytes
  Wire.write(hob);
  Wire.write(lob);
  Wire.endTransmission();
}

void show(){
  if (newData == true){
    Serial.print("data in...");
    Serial.print(numReceived);
    Serial.println(" bytes received");
    newData = false;

    readContent(numReceived + 3);
  }
}

void readContent(int length){
  Serial.println("\nMemory dump:");
  int pages = length / 32 + 1;

  bool ready = i2cReady(EEPROM_ADDRESS);

  // Set pointer at 0 and let it roll, no need to set it after that.
  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write(0);
  Wire.write(0);
  Wire.endTransmission();
  i2cReady(EEPROM_ADDRESS);

  for(int i = 0; i < pages; i++){
    int address = i * 32;

    int readLength = (i < pages - 1) ? 32 : length % 32;
    // Serial.print("read length ");
    // Serial.println(readLength);


    Wire.requestFrom(EEPROM_ADDRESS, readLength);
    while(Wire.available()){
      byte b = Wire.read();
      Serial.print(b);
      Serial.print(" ");
    }

    Serial.println();
  }
  Serial.print("\n\n\n\n\n");

}

void setLed(int red, int green, int blue){
  analogWrite(6, red);
  analogWrite(3, green);
  analogWrite(5, blue);
}

bool i2cReady(uint8_t adr){
  uint32_t timeout = millis();
  bool ready = false;
  while((millis() - timeout < 100) && (!ready)){
    Wire.beginTransmission(adr);
    int err = Wire.endTransmission();
    ready = (err == 0);
  }
  if (ready == false)
    Serial.println("Not ready !");
  // Serial.print("ready: ");
  // Serial.println(ready);
  return ready;
}
