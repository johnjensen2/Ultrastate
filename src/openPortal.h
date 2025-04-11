#ifndef OPEN_PORTAL_H
#define OPEN_PORTAL_H

#include <DNSServer.h>
#include <ESPAsyncWebServer.h>

extern const char openportal_html[];

// These now accept the server/dns objects passed from your app
void startCaptivePortal(AsyncWebServer &server, DNSServer &dnsServer);
void handleCaptivePortal(DNSServer &dnsServer);

#endif