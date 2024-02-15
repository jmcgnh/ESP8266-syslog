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

// plan for event-driven wifi handling
WiFiEventHandler stationConnectedHandler;
WiFiEventHandler stationGotIPHandler;
WiFiEventHandler stationDisconnectedHandler;

// forward declarations
void onStationConnected(const WiFiEventStationModeConnected& evt);
void onStationGotIP(const WiFiEventStationModeGotIP& evt);
void onStationDisconnected(const WiFiEventStationModeDisconnected& evt);

void wifi_setup()
{
  int wifistatus;

  // set up event handlers early, before checking to see if WiFi already autoconnected
  stationConnectedHandler = WiFi.onStationModeConnected(&onStationConnected);
  stationGotIPHandler = WiFi.onStationModeGotIP(&onStationGotIP);
  stationDisconnectedHandler = WiFi.onStationModeDisconnected(&onStationDisconnected);

  wifistatus = WiFi.status();

  if (wifistatus == WL_CONNECTED)
  {
   // perform the main action of the GotIPHandler in this case
   //// ....
    return; // if already connected, skip the rest of the steps
  }

  WiFi.mode(WIFI_STA);
  // WiFi.enableInsecureWEP(true); // needed since we still run WEP at home

  // choose faster reconnect over physical device security
  WiFi.persistent(true);

  for (auto station : wifisecret)
  {
    if (station.first)
    {
      wifiMulti.addAP(station.first, station.second);
      Serial.print(station.first);
      Serial.print(" ");
      Serial.print(station.second);
      Serial.println(" added");
    }
  }
  WiFi.printDiag(Serial); // DEBUG

  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++)
  {
    Serial.println(WiFi.SSID(i));
  }
  wifiMulti.run();

  // Omit OTA stuff for now...
}

void wifi_handler()
{
  // nothing to do here, since event handlers should be handlin it
  // No longer interested in putting WiFi to sleep while we do other actions
  // Omit OTA stuff for now...
}

void onStationConnected(const WiFiEventStationModeConnected& evt) {
  Serial.print("Station connected to SSID: ");
  Serial.println(evt.ssid);
}

void onStationGotIP(const WiFiEventStationModeGotIP& evt) {
  Serial.print("Station IP assigned: ");
  Serial.println(evt.ip.toString());
}

void onStationDisconnected(const WiFiEventStationModeDisconnected& evt) {
  Serial.print("Station disconnected: ");
  Serial.println(evt.reason);
}
