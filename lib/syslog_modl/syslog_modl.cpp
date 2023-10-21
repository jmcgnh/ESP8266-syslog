#include <Arduino.h>
#include <syslog_modl.h>

// This device info
#define DEVICE_HOSTNAME "weather-16"
#define APP_NAME "app123"

WiFiUDP udpClient;
Syslog syslog(udpClient, SYSLOG_PROTO_BSD);

void syslog_setup()
{
  // setting up syslog
  // prepare syslog configuration here (can be anywhere before first call of
  // log/logf method)
  
  syslog.server(SYSLOG_SERVER, SYSLOG_PORT);
  syslog.deviceHostname(DEVICE_HOSTNAME);
  syslog.appName(APP_NAME);
  syslog.defaultPriority(LOG_LOCAL1);

  syslog.log(LOG_INFO, F("setup complete"));
  Serial.println(F("syslog setup complete"));
}