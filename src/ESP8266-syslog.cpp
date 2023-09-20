#include <Arduino.h>
#include <blink_modl.h>
#include <wifi_modl.h>
#include <secretdata.h>
#include <Wire.h>
#include <Syslog.h>
#include <WiFiUDP.h>
#include <map>

// Identification

const char vfname[] = __FILE__;
const char vtimestamp[] = __DATE__ " " __TIME__;
const char versionstring[] = "20230919.2150.2";
const char myHostname[] = "weather-16";
unsigned long nextident = 0;
unsigned long indentinterval = 20000;

unsigned long currentMillis;

// Syslog naive setup begins here
#define SYSLOG_SERVER "syslog5514.jmcg.net"
#define SYSLOG_PORT 5514

// This device info
#define DEVICE_HOSTNAME "weather-16"
#define APP_NAME "app123"

WiFiUDP udpClient;
Syslog syslog(udpClient, SYSLOG_PROTO_BSD);


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

  // setting up syslog
  // prepare syslog configuration here (can be anywhere before first call of 
	// log/logf method)
  syslog.server(SYSLOG_SERVER, SYSLOG_PORT);
  syslog.deviceHostname(DEVICE_HOSTNAME);
  syslog.appName(APP_NAME);
  syslog.defaultPriority(LOG_LOCAL1);

  syslog.log(LOG_INFO, F("setup complete"));
  Serial.println(F("syslog setup complete"));
  // blink_change( blinkSeqNormal);
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
    Serial.println(versionstring);
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
