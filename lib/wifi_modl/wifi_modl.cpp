#include <Arduino.h>
#define DEBUG_ESP_WIFI 1
#define DEBUG_ESP_HTTP_UPDATE 1
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <secretdata.h>
#include <map>

int wifi_disconnected = 0;
extern IPAddress wifiIP;

#define PLUGGEDIN 1

ESP8266WiFiMulti wifiMulti;

// plan for event-driven wifi handling
WiFiEventHandler stationConnectedHandler;
WiFiEventHandler stationGotIPHandler;
WiFiEventHandler stationDisconnectedHandler;

// forward declarations
void onStationConnected(const WiFiEventStationModeConnected &evt);
void onStationGotIP(const WiFiEventStationModeGotIP &evt);
void onStationDisconnected(const WiFiEventStationModeDisconnected &evt);
extern void TlogConnectAction();

void wifi_setup()
{
  int wifistatus;

  // set up event handlers early, before checking to see if WiFi already connected
  stationConnectedHandler = WiFi.onStationModeConnected(&onStationConnected);
  stationGotIPHandler = WiFi.onStationModeGotIP(&onStationGotIP);
  stationDisconnectedHandler = WiFi.onStationModeDisconnected(&onStationDisconnected);

  wifistatus = WiFi.status();

  if (wifistatus == WL_CONNECTED)
  {
    TlogConnectAction();
    return; // if already connected, skip the rest of the steps
  }

  WiFi.mode(WIFI_STA);
  // WiFi.enableInsecureWEP(true); // no longer exlusively running WEP at home

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
  if (wifi_disconnected)
  {
    wifiMulti.run();
  }
}

void onStationConnected(const WiFiEventStationModeConnected &evt)
{
  Serial.print("Station connected to SSID: ");
  Serial.println(evt.ssid);
  wifi_disconnected = 0;
}

void onStationGotIP(const WiFiEventStationModeGotIP &evt)
{
  Serial.print("Station IP assigned: ");
  Serial.println(evt.ip.toString());
  wifiIP = evt.ip;

  TlogConnectAction();
}

void onStationDisconnected(const WiFiEventStationModeDisconnected &evt)
{
  Serial.print("Station disconnected: ");
  Serial.println(evt.reason);
  wifi_disconnected = 1;
}
