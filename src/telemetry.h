#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>

// Struct to package telemetry data
struct TelemetryPacket {
  float latitude;
  float longitude;
  float speed;
  int rpm_motor1;
  int rpm_motor2;
  float batteryVoltage;
  String systemStatus;
};

// Initializes telemetry system
void initTelemetry();

// Sends telemetry packet via ESP-NOW
void sendTelemetry(const TelemetryPacket &packet);
void sendTelemetryPacket(const String& message);



extern unsigned long lastTelemetryTime; //used only in full runtime mode
#define sendTelemetryInterval 1000 // milliseconds = 1 second
#endif
