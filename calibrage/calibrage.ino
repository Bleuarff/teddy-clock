#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

ESP8266WebServer server(80);

const byte redPin = 14;
const byte greenPin = 13;
const byte bluePin = 12;


void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);

  // local IP and & gateway 192.168.14.1, subnet 255.255.255.0
  if (!WiFi.softAPConfig(0x010EA8C0, 0x010EA8C0, 0x00FFFFFF)){
    Serial.println("Wifi config failed");
    return;
  }

  // ssid, no password, channel 14, ssid visible, max connection
  bool res = WiFi.softAP("Hérisson", NULL, 14, false, 2);

  if (!res){
    Serial.println("Wifi failed");
    return;
  }

  // Returns current light level
  server.on("/sense", HTTP_GET, getSense);

  // Set led to given values
  server.on("/led", HTTP_POST, setLed);
  server.begin();
  Serial.println("HTTP server started");
}

// Read current light level and returns it
void getSense(){
  int lvl = analogRead(A0);
  String s = String(lvl);
  server.send(200, "text/plain", s);
}

// Sets led to given rgb value
void setLed(){
  int red = server.arg("r").toInt();
  int green = server.arg("g").toInt();
  int blue = server.arg("b").toInt();

  Serial.printf("r:%s   g:%s  b:%s", red, green, blue);
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
  server.send(204, "text/plain", "");
}

void loop(){

}
