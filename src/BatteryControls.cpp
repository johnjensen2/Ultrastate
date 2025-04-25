#include "BatteryControls.h"
#include "defaultMode.h"
#include "debug.h"
#include "calibrationMode.h"
#include "diagnosticsMode.h"
#include "webServer.h"
#include "motorControl.h"
//#include "ledControl.h"
#include "pinConfig.h"
#include "wifiManager.h"
#include "telemetry.h"
#include "gps.h"
#include "fullRuntimeMode.h"
#include "aiTrainingMode.h"
#include "imu.h"
#include "tempControl.h"
#include <Arduino.h>



// Voltage divider constants: R1 = 10k(bat+), R2 = 3.3k(bat-)
// The voltage divider scales the battery voltage down to a safe level for the ADC
const float VOLTAGE_DIVIDER_SCALE = (10.0 + 3.3) / 3.3;

float readBatteryVoltage() {
  int raw = analogRead(BATTERY_PIN );
  float voltage = (raw / 4095.0) * 3.3;  // ADC to voltage (ESP32 = 3.3V)
  return voltage * VOLTAGE_DIVIDER_SCALE;
}

// Maps 6.6V - 8.4V to 0% - 100%
int getBatteryPercentage() {
  float batt = readBatteryVoltage();
  int percent = map(batt * 100, 660, 840, 0, 100);  // in hundredths of volts
  percent = constrain(percent, 0, 100);  // Clamp safely
  return percent;
}