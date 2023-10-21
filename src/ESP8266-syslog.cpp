#include <Arduino.h>
#include <blink_modl.h>
#include <ident_modl.h>
#include <syslog_modl.h>
#include <wifi_modl.h>
#include <secretdata.h>
#include <map>

// Identification

const char vfname[] = __FILE__;
const char vtimestamp[] = __DATE__ " " __TIME__;
const char versionstring[] = "20231020.2128.1";
const char myHostname[] = "weather-16";
unsigned long nextident = 0;
unsigned long indentinterval = 30000;

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
  Serial.print("versionstring: ");
  Serial.println(versionstring);
  Serial.print("link url: ");
  Serial.println("https://gitea.jmcg.net/jmcg/WeatherStation06");
  Serial.println();

  blink_setup();
  syslog_setup();
}

void loop() {
  currentMillis = millis();
 
  if( currentMillis > nextident){
    nextident = currentMillis + indentinterval;
    // repeat identification occasionally
    Serial.println();
    Serial.print("file: ");
    Serial.println(vfname);
    Serial.print("timestamp (local time): ");
    Serial.println(vtimestamp);
    Serial.print("versionstring: ");
    Serial.println(versionstring);
    Serial.print("link url: ");
    Serial.println("https://github.com/jmcgnh/ESP8266-syslog");
    Serial.println();
    // now do the same for syslog
    syslog.logf(LOG_INFO, "file: %s", vfname );
    syslog.logf(LOG_INFO, "timestamp: %s (local time)", vtimestamp);
    syslog.logf(LOG_INFO, "versionstring: %s", versionstring);
    syslog.logf(LOG_INFO, "link url: https://github.com/jmcgnh/ESP8266-syslog");

  }
  
  wifi_handler();
  blink_handler();
}
