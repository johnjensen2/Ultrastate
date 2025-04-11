#ifndef CALIBRATION_MODE_H
#define CALIBRATION_MODE_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

void runCalibrationSetup(AsyncWebServer& server);
void runCalibrationLoop();

#endif