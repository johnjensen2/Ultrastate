#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

extern const char* ssid;
extern const char* password;

void connectToWiFi(const char* ssid, const char* password);
void connectToEspNow(); 
bool isWiFiConnected();
void setupOTA();

#endif // WIFI_MANAGER_H
