
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FS.h"

// network id & pwd
const char *ssid = "Herisson";
const char *password = "gabuzomeu";

ESP8266WebServer server(80);
// File uiFile;

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  WiFi.begin("SOBL-240", "onchoisisunmotdepassepournepassefairepirateroulahnononveutpas!");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  if (!SPIFFS.begin()){
    Serial.println("SPIFFS mount fail :(");
  }

  Serial.print("Configuring access point...");

  // server.on("/", handleWebui);
  server.serveStatic("/", SPIFFS, "/webui.html");
  server.begin();
  Serial.println("HTTP server started");


}

void loop() {
  server.handleClient();
}

void handleWebui(){
  server.send(200, "text/html", "<h1>Bouahaha</h1>");
}
