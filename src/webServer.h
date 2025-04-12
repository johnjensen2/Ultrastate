#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

extern AsyncWebServer server;  // Declare it so other files can use it
extern DNSServer dnsServer;
void initWebServer();
extern const char index_html[]; 
extern const char defaultMode_html[]; // Declare it so other files know it exists
extern const char defaultMode2_html[];
#endif
