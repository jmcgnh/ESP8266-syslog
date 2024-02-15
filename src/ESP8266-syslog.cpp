#include <Arduino.h>
#include <blink_modl.h>
#include <wifi_modl.h>
#include <secretdata.h>
#include <syslog_secrets.h>
#include <gitstatus.h>
#include <buildstatus.h>
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
const char projdir[] = PROJECT_DIR;
const char branch[] = BRANCH;
const char ltimestamp[] = TIMESTAMP;
const char commit_hash[] = COMMIT_HASH;
const char commit_comment[] = LAST_COMMIT_COMMENT;
const char project_url[] = PROJECT_URL;
String myHostname = "weather26";
unsigned long needident = 1;

unsigned long currentMillis;

void setup()
{
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
#endif

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
    Log.println();
  }

  currentMillis = millis();
  wifi_handler();
  blink_handler();
}

void TlogConnectAction()
{
#ifdef SYSLOG_HOST
  const std::shared_ptr<LOGBase> syslogStreamPtr = std::make_shared<SyslogStream>(syslogStream);
  Log.addPrintStream(syslogStreamPtr);
#endif
  Log.addPrintStream(std::make_shared<WebSerialStream>(webSerialStream));
}