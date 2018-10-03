
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FS.h"

ESP8266WebServer server(80);
// File uiFile;

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  // WiFi.begin("SOBL-240", "onchoisisunmotdepassepournepassefairepirateroulahnononveutpas!");
  // Serial.print("Connecting");
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println();

  WiFi.mode(WIFI_AP);
  bool res = WiFi.softAP("Herisson", NULL, 14, false, 2);

  if (!res){
    Serial.println("Wifi failed");
    return;
  }

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  if (!SPIFFS.begin()){
    Serial.println("SPIFFS mount fail :(");
  }

  Serial.print("Configuring access point...");

  server.serveStatic("/", SPIFFS, "/webui.html");
  server.on("/time", HTTP_GET, handleWebui);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleWebui(){
  Serial.println("GET /time");
  server.send(200, "text/plain", "2018-10-02T22:27:00+0200");
}
