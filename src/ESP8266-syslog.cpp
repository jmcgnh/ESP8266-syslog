#include <Arduino.h>
#include <blink_modl.h>
#include <wifi_modl.h>
#include <secretdata.h>
#include <Wire.h>
#include <map>

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

  // Identification
  Serial.println();
  Serial.print("file: ");
  Serial.println(vfname);
  Serial.print("Project dir: ");
  Serial.println(projdir);
  Serial.print("current branch: ");
  Serial.print(branch);
  Serial.print("  commit: ");
  Serial.println(commit_hash);
  Serial.println(commit_comment);
  Serial.print("compile timestamp: ");
  Serial.println(ltimestamp);
  Serial.print("Project URL: ");
  Serial.println(project_url);
  Serial.println();

  blink_setup();
}

void loop()
{

  if (needident)
  {
    needident = 0;
    // repeat identification occasionally
    // Identification
    Serial.println();
    Serial.print("file: ");
    Serial.println(vfname);
    Serial.print("Project dir: ");
    Serial.println(projdir);
    Serial.print("current branch: ");
    Serial.print(branch);
    Serial.print("  commit: ");
    Serial.println(commit_hash);
    Serial.println(commit_comment);
    Serial.print("compile timestamp: ");
    Serial.println(ltimestamp);
    Serial.print("Project URL: ");
    Serial.println(project_url);
    Serial.println();
  }

  currentMillis = millis();
  wifi_handler();
  blink_handler();
}
