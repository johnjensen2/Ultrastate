#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H


void stopAllMotors();
void controlMotor(int motor, int speed, bool forward);
void setMotorSpeeds(int left, int right);
void initMotors();

#endif
