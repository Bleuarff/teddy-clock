
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// network id & pwd
const char *ssid = "Herisson";
const char *password = "gabuzomeu";

ESP8266WebServer server(80);



void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");

  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // set FQDN for clients that support it
  if (!MDNS.begin("herisson")) {
    Serial.println("Error setting up MDNS responder!");
  }
  else{
    Serial.print("mDNS responder started as herisson.local");
  }

  server.on("/", handleWebui);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleWebui(){
  server.send(200, "text/html", "<h1>Webui</h1>");
}
