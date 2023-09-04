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
String versionstring = "20230826.0230.1";
String myHostname = "weather26";
unsigned long needident = 1;

unsigned long currentMillis;

// Syslog naive setup begins here
#define SYSLOG_SERVER "10.200.200.215"
#define SYSLOG_PORT 5514

// This device info
#define DEVICE_HOSTNAME "weather-16"
#define APP_NAME "app123"

WiFiUDP udpClient;
Syslog syslog(udpClient, SYSLOG_PROTO_IETF);


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
  syslog.defaultPriority(LOG_KERN);

  syslog.log(LOG_INFO, "setup complete");
  Serial.println("syslog setup complete");
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
