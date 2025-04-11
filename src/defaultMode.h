#ifndef DEFAULTMODE_H
#define DEFAULTMODE_H

#include <ESPAsyncWebServer.h>


void runDefaultSetup();
void runDefaultLoop();
void IRAM_ATTR handleEncoderMotor1();
void IRAM_ATTR handleEncoderMotor2();

// Function declarations
void runDefaultSetup(AsyncWebServer &server);
void runDefaultLoop();

extern int rpm_motor1;
extern int rpm_motor2;

extern volatile long pulse_count_motor1;
extern volatile long pulse_count_motor2;
extern volatile int direction_motor1;
extern volatile int direction_motor2;
extern unsigned long last_rpm_update;
extern unsigned long rpm_interval;

#endif