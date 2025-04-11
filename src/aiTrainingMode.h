#ifndef AITRAININGMODE_H
#define AITRAININGMODE_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

void runAITrainingSetup(AsyncWebServer& server); // Setup function for AI Training Mode
void runAITrainingLoop(); // Loop function for AI Training Mode

#endif // AITRAININGMODE_H
