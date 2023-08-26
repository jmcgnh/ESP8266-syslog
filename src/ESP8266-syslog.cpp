#include <Arduino.h>
#include <blink_modl.h>
#include <wifi_modl.h>
#include <secretdata.h>
#include <Wire.h>
#include <map>

// Identification

const char vfname[] = __FILE__;
const char vtimestamp[] = __DATE__ " " __TIME__;
String versionstring = "20230826.0230.1";
String myHostname = "weather26";
unsigned long needident = 1;

unsigned long currentMillis;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("Booting");

  wifi_setup();
 
  // Identification
  Serial.println();
  Serial.print("file: ");
  Serial.println(vfname);
  Serial.print("timestamp (local time): ");
  Serial.println(vtimestamp);
  Serial.println(versionstring);
  Serial.println("https://gitea.jmcg.net/jmcg/WeatherStation06");
  Serial.println();

  blink_setup();
 }

void loop() {

  if( needident){
    needident=0;
    // repeat identification occasionally
    Serial.println();
    Serial.print("file: ");
    Serial.println(vfname);
    Serial.print("timestamp (local time): ");
    Serial.println(vtimestamp);
    Serial.println(versionstring);
    Serial.println("https://github.com/jmcgnh/ESP8266-syslog");
    Serial.println();
  }
  
  currentMillis = millis();
  wifi_handler();
  blink_handler();
}
