#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>

// Servo control functions
void setupSteeringServo();
void setSteeringAngle(int angle);
void setSteeringFromJoystick(float x);

// Drop mechanism functions
void sendServoOpen(uint8_t dropNum);
void sendServoClose(uint8_t dropNum);

#endif
