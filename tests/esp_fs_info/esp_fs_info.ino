#include "FS.h"

void setup(){
  Serial.begin(9600);
  SPIFFS.begin();
  delay(5000);
}

void loop(){
  FSInfo fs_info;
  SPIFFS.info(fs_info);

  Serial.println("FS info:");
  Serial.printf("total: %i\n",fs_info.totalBytes);
  Serial.printf("used: %i\n", fs_info.usedBytes);
  Serial.printf("block size: %i\n", fs_info.blockSize);
  Serial.printf("page size: %i\n", fs_info.pageSize);
  // usedBytes

  File f = SPIFFS.open("/webui.html", "r");

  if (!f) {
    Serial.println("file open failed");
  }

  int size = f.size();
  Serial.printf("webui size: %i\n", size);
  f.close();


  delay(60000);
}
