#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>

void sendServoOpen(uint8_t dropNum);
void sendServoClose(uint8_t dropNum);

#endif
