#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H


extern int rpm_motor1;
extern int rpm_motor2;
extern volatile long pulse_count_motor1;
extern volatile long pulse_count_motor2;
extern volatile int direction_motor1;
extern volatile int direction_motor2;

extern unsigned long last_rpm_update;
extern unsigned long rpm_interval;

void stopAllMotors();
void controlMotor(int motor, int speed, bool forward);
void setMotorSpeeds(int left, int right);
void initMotors();

#endif
