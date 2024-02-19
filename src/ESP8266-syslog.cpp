#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <blink_modl.h>
#include <wifi_modl.h>
#include <secretdata.h>
#include <syslog_secrets.h>
#include <gitstatus.h>
#include <buildstatus.h>
#include <Wire.h>
#include <Tlog.h>
#include <map>

IPAddress wifiIP;

#include <TelnetSerialStream.h>
TelnetSerialStream telnetSerialStream = TelnetSerialStream();

#include <WebSerialStream.h>
WebSerialStream webSerialStream = WebSerialStream();

#ifdef SYSLOG_HOST
#include <SyslogStream.h>
SyslogStream syslogStream = SyslogStream();
#endif

// forward declaratoin
void TlogConnectAction();

// Identification

const char vfname[] = __FILE__;
const char vtimestamp[] = __DATE__ " " __TIME__;
const char projdir[] = PROJECT_DIR;
const char branch[] = BRANCH;
const char ltimestamp[] = TIMESTAMP;
const char commit_hash[] = COMMIT_HASH;
const char commit_comment[] = LAST_COMMIT_COMMENT;
const char project_url[] = PROJECT_URL;
String myHostname = "amnesiac";
unsigned long needident = 1;
unsigned long identInterval = (5 * 60 * 1000); // 5 minutes
unsigned long nextIdent = 0;

unsigned long currentMillis;

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("Booting");

#ifdef SYSLOG_HOST
  syslogStream.setDestination(SYSLOG_HOST);
  syslogStream.setRaw(false); // whether or not the syslog server is a modern(ish) unix.
#ifdef SYSLOG_PORT
  syslogStream.setPort(SYSLOG_PORT);
#endif
#endif

  wifi_setup();

  // Identification
  Log.println();
  Log.print("file: ");
  Log.println(vfname);
  Log.print("Project dir: ");
  Log.println(projdir);
  Log.print("current branch: ");
  Log.print(branch);
  Log.print("  commit: ");
  Log.println(commit_hash);
  Log.println(commit_comment);
  Log.print("compile timestamp: ");
  Log.println(ltimestamp);
  Log.print("Project URL: ");
  Log.println(project_url);
  Log.println();

  blink_setup();
}

void loop()
{
  currentMillis = millis();
  if (currentMillis > nextIdent)
  {
    needident = 1;
    nextIdent = currentMillis + identInterval;
  }
  if (needident)
  {
    needident = 0;
    // repeat identification occasionally
    // Identification
    Log.println();
    Log.print("file: ");
    Log.println(vfname);
    Log.print("Project dir: ");
    Log.println(projdir);
    Log.print("current branch: ");
    Log.print(branch);
    Log.print("  commit: ");
    Log.println(commit_hash);
    Log.println(commit_comment);
    Log.print("compile timestamp: ");
    Log.println(ltimestamp);
    Log.print("Project URL: ");
    Log.println(project_url);
    Log.print(wifiIP.toString());
    Log.println();
  }

  wifi_handler();
  blink_handler();
  Log.loop();
}

void TlogConnectAction()
{
  Log.println("TlogConnectAction start");
  Log.addPrintStream(std::make_shared<TelnetSerialStream>(telnetSerialStream));
  Log.addPrintStream(std::make_shared<WebSerialStream>(webSerialStream));

#ifdef SYSLOG_HOST
  // syslogStream.begin();
  const std::shared_ptr<LOGBase> syslogStreamPtr = std::make_shared<SyslogStream>(syslogStream);
  Log.addPrintStream(syslogStreamPtr);
  Log.println("added syslogStream");
#endif
  Log.begin();
}