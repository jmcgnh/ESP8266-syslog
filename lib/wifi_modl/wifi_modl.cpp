#include <Arduino.h>
#define DEBUG_ESP_WIFI 1
#define DEBUG_ESP_HTTP_UPDATE 1
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <secretdata.h>
#include <map>

// OTA variables
const unsigned long OtaUpdateInterval = 10 * 60 * 1000; // 10 minutes
unsigned long nextOtaUpdate = 0;

int wifi_disconnected = 0;

#define PLUGGEDIN 1

ESP8266WiFiMulti wifiMulti;

void wifi_setup()
{
  WiFi.mode(WIFI_AP_STA);
  // WiFi.enableInsecureWEP(true); // needed since we still run WEP at home

  // Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);
  
  for (auto station : wifisecret)
  {
    if (station.first)
    {
      wifiMulti.addAP(station.first, station.second);
      Serial.print( station.first);
      Serial.print( " ");
      Serial.print( station.second);
      Serial.println( " added");
    }
  }
  WiFi.printDiag(Serial); //DEBUG

  int n = WiFi.scanNetworks();
  for( int i = 0; i < n; i++)
    {
      Serial.println(WiFi.SSID(i));
    }
  wifiMulti.run();

  // Omit OTA stuff for now...
}

void wifi_handler()
{
  int wifistatus;
#ifndef PLUGGEDIN
  if( sensing && wifi_disconnected) return; // avoid WiFi actions while using A0
  if( sensing ) {
      Serial.println("Entering modem sleep mode");
      WiFi.disconnect();
      wifi_disconnected = 1;
      WiFi.forceSleepBegin();
      delay( 1); // supposedly required, but perhaps apocraphal
      return;
  }
  if( wifi_disconnected && ! sensing) 

#else  // PLUGGEDIN
if( wifi_disconnected)
#endif // PLUGGEDIN
  {
      WiFi.forceSleepWake();
      // delay(1); // supposedly required, but perhaps apocraphal
      wifi_disconnected = 0;
      Serial.println("Exit modem sleep mode");

      wifistatus = WiFi.status();

      if ( wifistatus != WL_CONNECTED)
      {
        wifiMulti.run();
      }
  }
  // Omit OTA stuff for now...
}
