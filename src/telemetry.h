#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>

struct TelemetryPacket {
  float latitude;
  float longitude;
  float speed;
  int rpm_motor1;
  int rpm_motor2;
  float batteryVoltage;
  String systemStatus;

  // IMU Data
  float accelX;
  float accelY;
  float accelZ;
  float gyroX;
  float gyroY;
  float gyroZ;

   bool isShaking;             // New field to report if shaking was detected
};

// Initializes telemetry system
void initTelemetry();

// Sends telemetry packet via ESP-NOW
void sendTelemetry(const TelemetryPacket &packet);
void sendTelemetryPacket(const String& message);



extern unsigned long lastTelemetryTime; //used only in full runtime mode
#define sendTelemetryInterval 1000 // milliseconds = 1 second
#endif
