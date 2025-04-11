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
#include "imu.h"
#include <EEPROM.h>


void diagnosticsSetup() {
  Serial.println("Diagnostics mode setup initialized.");
  startCaptivePortal(server, dnsServer);
}

void diagnosticsLoop() {
  handleCaptivePortal(dnsServer);
  Serial.println("Running diagnostics...");
  delay(1000);
}


void diagnosticsIMU() {
  // Example: press a button or send a command to calibrate
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'c') {
      calibrateIMU();
    }
  }

  delay(100);
}