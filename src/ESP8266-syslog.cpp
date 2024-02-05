#include <Arduino.h>
#include <blink_modl.h>
#include <wifi_modl.h>
#include <secretdata.h>
#include <Wire.h>
#include <Tlog.h>
#include <map>

#include <WebSerialStream.h>
WebSerialStream webSerialStream = WebSerialStream();

#ifdef SYSLOG_HOST
#include <SyslogStream.h>
SyslogStream syslogStream = SyslogStream();
#endif

// Identification

const char vfname[] = __FILE__;
const char vtimestamp[] = __DATE__ " " __TIME__;
String versionstring = "20240205.0345.1";
String myHostname = "testbed9D";
unsigned long needident = 1;

unsigned long currentMillis;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("Booting");

  wifi_setup();

#ifdef SYSLOG_HOST
  syslogStream.setDestination(SYSLOG_HOST);
  syslogStream.setRaw(false); // whether or not the syslog server is a modern(ish) unix.
#ifdef SYSLOG_PORT
  syslogStream.setPort(SYSLOG_PORT);
#endif

  const std::shared_ptr<LOGBase> syslogStreamPtr = std::make_shared<SyslogStream>(syslogStream);
  Log.addPrintStream(syslogStreamPtr);
#endif
  Log.addPrintStream(std::make_shared<WebSerialStream>(webSerialStream));
 
  // Identification
  Log.println();
  Log.print("file: ");
  Log.println(vfname);
  Log.print("timestamp (local time): ");
  Log.println(vtimestamp);
  Log.println(versionstring);
  Log.println("https://gitea.jmcg.net/jmcg/WeatherStation06");
  Log.println();

  blink_setup();
 }

void loop() {

  if( needident){
    needident=0;
    // repeat identification occasionally
    Log.println();
    Log.print("file: ");
    Log.println(vfname);
    Log.print("timestamp (local time): ");
    Log.println(vtimestamp);
    Log.println(versionstring);
    Log.println("https://github.com/jmcgnh/ESP8266-syslog");
    Log.println();
  }
  
  currentMillis = millis();
  wifi_handler();
  blink_handler();
}
