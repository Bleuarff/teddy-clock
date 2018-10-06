/*********************************************************

  server methods

**********************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

void handleTime(){
  Serial.print("GET /time: ");
  Time t = getTime();
  String res = "20";
  res += t.year;
  res += "-";

  if (t.month < 10)
    res += "0";
  res += t.month;

  res += "-";

  if (t.date < 10)
    res += "0";
  res += t.date;

  res += "T";

  if (t.hour < 10)
    res += "0";
  res += t.hour;

  res += ":";

  if (t.min < 10)
    res += "0";
  res += t.min;

  res += ":00";
  res += winterTimeChangeDone ? "+0100" : "+0200";
  res += " ";
  res += t.dow;
  Serial.println(res);
  server.send(200, "text/plain", res);
}

void getAlarms(){
  Serial.print("GET /alarms: ");
  String res = "";
  res += dodo;
  res += " ";
  res += wakeup1;
  res += " ";
  res += wakeup2;
  Serial.println(res);
  server.send(200, "text/plain", res);
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
  server.on("/alarms", HTTP_GET, getAlarms);
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
