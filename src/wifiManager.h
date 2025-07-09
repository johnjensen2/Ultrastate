#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "secrets.h"
#include <WiFi.h>

extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;

void connectToWiFi(const char* ssid, const char* password);
void connectToEspNow(); 
bool isWiFiConnected();
void setupOTA();
void setupHotspot();
#endif // WIFI_MANAGER_H
