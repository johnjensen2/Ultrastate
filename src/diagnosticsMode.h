#ifndef DIAGNOSTICS_MODE_H
#define DIAGNOSTICS_MODE_H

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

extern AsyncWebServer server;  // Declare it so other files can use it
extern DNSServer dnsServer;

void diagnosticsSetup();
void diagnosticsLoop();

#endif
