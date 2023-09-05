#include <Arduino.h>
#include <blink_modl.h>
#include <wifi_modl.h>
#include <secretdata.h>
#include <Wire.h>
#include <ESP8266Ping.h>
#include <map>

// Identification

const char vfname[] = __FILE__;
const char vtimestamp[] = __DATE__ " " __TIME__;
String versionstring = "20230905.0325.1";
String myHostname = "weather26";
unsigned long indentinterval = 10000;
unsigned long nextident = 0;

unsigned long currentMillis;

const IPAddress remote_ip(192,168,1,1);

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

  currentMillis = millis();

  wifi_handler();

  if( currentMillis > nextident ){
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
    
    WiFi.printDiag(Serial);
    Serial.println(WiFi.localIP().toString());
    Serial.print("ping remote at IP ");
    Serial.println(remote_ip.toString());
    if( Ping.ping(remote_ip, 4)) {
      Serial.println("Success");
    } else {
      Serial.println("Error");
    }
  }
  
  blink_handler();
}
