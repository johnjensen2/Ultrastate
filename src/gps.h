#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>
#include "telemetry.h"  // Include telemetry.h to use TelemetryPacket

extern TinyGPSPlus gps;
extern HardwareSerial GPS_Serial;

void initGPS();
void updateGPS();
String getGPSData();
void updateTelemetryWithGPS(TelemetryPacket &packet);

#endif