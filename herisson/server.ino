/*********************************************************

  server methods

**********************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FS.h"

bool startServer(){
  wifi_set_sleep_type(NONE_SLEEP_T);
  WiFi.mode(WIFI_AP);

  // ssid, no password, channel 14, ssid visible, max connection
  bool res = WiFi.softAP("herisson", NULL, 14, false, 1);
  // bool res = WiFi.softAP("herisson");

  Serial.println(res ? "Wifi OK" : "Wifi failed");
  return res;
}

bool stopServer(){
  bool res = WiFi.softAPdisconnect(true);
  setAirplaneMode();
  Serial.println(res ? "Shutdown OK" : "shutdown failed");
  return res;
}

// goes to modem-sleep mode. Consumes ~20mA instead of 80+ mA when wifi is up
void setAirplaneMode(){
  WiFi.mode(WIFI_OFF);
  wifi_set_sleep_type(MODEM_SLEEP_T);
  WiFi.forceSleepBegin();
  delay(1); // a delay is necessary for forcesleepbegin to have effect
}
