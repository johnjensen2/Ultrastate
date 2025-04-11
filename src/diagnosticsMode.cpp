#include <Arduino.h>
#include "diagnosticsMode.h"
#include "openPortal.h"
#include "debug.h"
#include "calibrationMode.h"
#include "diagnosticsMode.h"
#include "webServer.h"
#include "motorControl.h"
#include "pinConfig.h"
#include "wifiManager.h"

#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "openPortal.h"




void diagnosticsSetup() {
  Serial.println("Diagnostics mode setup initialized.");
  startCaptivePortal(server, dnsServer);
}

void diagnosticsLoop() {
  handleCaptivePortal(dnsServer);
  Serial.println("Running diagnostics...");
  delay(1000);
}
