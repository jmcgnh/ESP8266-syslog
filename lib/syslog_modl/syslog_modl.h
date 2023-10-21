#ifndef _SYSLOG_MODL_H_
#define _SYSLOG_MODL_H_

#include <Syslog.h>
#include <WiFiUDP.h>

#define SYSLOG_SERVER "syslog5514.jmcg.net"
#define SYSLOG_PORT 5514

extern void syslog_setup();
extern WiFiUDP udpClient;
extern Syslog syslog;

#endif /* _SYSLOG_MODL_H_ */