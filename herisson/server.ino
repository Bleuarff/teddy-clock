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

void updateTime(){
  Serial.print("POST /time: ");
  String ts = server.arg("t");

  if (ts.length() != 21){
    server.send(400, "text/plain", "invalid parameter (length)");
    return;
  }

  // Serial.println(ts);
  Time t = parseDate(ts);
  printTime(t);

  if (t.year == 0 || t.month == 0 || t.date == 0 || t.dow == 0){
      server.send(500, "text/plain", "parse error");
      return;
  }

  setTime(t);
  server.send(204, "text/plain", "");
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

void setAlarms(){
  Serial.println("/POST alarms: ");
  int _dodo = server.arg("dodo").toInt();
  int _w1 = server.arg("wakeup1").toInt();
  int _w2 = server.arg("wakeup2").toInt();

  dodo = _dodo;
  wakeup1 = _w1;
  wakeup2 = _w2;
  EEPROM.put(1, dodo);
  EEPROM.put(5, wakeup1);
  EEPROM.put(9, wakeup2);
  EEPROM.commit();

  Serial.printf("dodo: %i / ", dodo);
  printAlarm(_dodo);
  Serial.printf("w1: %i / ", _w1);
  printAlarm(_w1);
  Serial.printf("w2: %i / ", _w2);
  printAlarm(_w2);
  server.send(204, "text/plain", "");
}

bool startServer(){
  wifi_set_sleep_type(NONE_SLEEP_T);
  WiFi.mode(WIFI_AP);

  // local IP and & gateway 192.168.14.1, subnet 255.255.255.0
  if (!WiFi.softAPConfig(0x010EA8C0, 0x010EA8C0, 0x00FFFFFF)){
    Serial.println("Wifi config failed");
    return false;
  }

  // ssid, no password, channel 14, ssid visible, max connection
  bool res = WiFi.softAP("Hérisson", NULL, 14, false, 2);

  if (!res){
    Serial.println("Wifi failed");
    return false;
  }

  server.serveStatic("/", SPIFFS, "/webui.html");
  server.on("/time", HTTP_GET, handleTime);
  server.on("/time", HTTP_POST, updateTime);
  server.on("/alarms", HTTP_GET, getAlarms);
  server.on("/alarms", HTTP_POST, setAlarms);
  server.serveStatic("/favicon.png", SPIFFS, "/favicon.png");
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

Time parseDate(String ts){
  Time t;
  t.year = ts.substring(2, 4).toInt();
  t.month = ts.substring(5, 7).toInt();
  t.date = ts.substring(8, 10).toInt();
  t.hour = ts.substring(11, 13).toInt();
  t.min = ts.substring(14, 16).toInt();
  t.dow = ts.substring(20, 21).toInt();
  return t;
}
