/*********************************************************

  server methods

**********************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

void handleTime(){
  Serial.println("GET /time");
  // Time t = getTime();
  // String message = t.month;
  // message += ":";
  // message += t.day;
  // message += "\nMethod: ";
  // message += (server.method() == HTTP_GET) ? "GET" : "POST";
  // message += "\nArguments: ";
  // message += server.args();
  // message += "\n";
  server.send(200, "text/plain", "coincoin");
}

bool startServer(){
  wifi_set_sleep_type(NONE_SLEEP_T);
  WiFi.mode(WIFI_AP);

  // ssid, no password, channel 14, ssid visible, max connection
  bool res = WiFi.softAP("Herisson", NULL, 14, false, 2);

  if (!res){
    Serial.println("Wifi failed");
    return false;
  }

  server.serveStatic("/", SPIFFS, "/webui.html");
  server.on("/time", HTTP_GET, handleTime);
  server.begin();
  Serial.println("HTTP server started");

  return res;
}

bool stopServer(){
  bool res = WiFi.softAPdisconnect(true);
  server.close();

  setAirplaneMode();
  Serial.println(res ? "Shutdown OK" : "shutdown failed");
  return res;
}

const char* returnTime(){

}

// goes to modem-sleep mode. Consumes ~20mA instead of 80+ mA when wifi is up
void setAirplaneMode(){
  WiFi.mode(WIFI_OFF);
  wifi_set_sleep_type(MODEM_SLEEP_T);
  WiFi.forceSleepBegin();
  delay(1); // a delay is necessary for forcesleepbegin to have effect
}
