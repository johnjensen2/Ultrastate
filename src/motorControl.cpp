#include <Arduino.h>
#include "motorControl.h"
#include "pinConfig.h"
// Example motor pin values
const int motorLeftPin = 25;
const int motorRightPin = 26;

volatile long pulse_count_motor1 = 0;
volatile long pulse_count_motor2 = 0;
volatile int direction_motor1 = 0;  // 1 for forward, -1 for reverse
volatile int direction_motor2 = 0;

unsigned long last_rpm_update = 0;
unsigned long rpm_interval = 1000;  // Update RPM every second
int rpm_motor1 = 0;
int rpm_motor2 = 0;


void initMotors() {
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
  pinMode(MOTOR1_EN, OUTPUT);
  pinMode(MOTOR2_EN, OUTPUT);

}

void setMotorSpeeds(int left, int right) {
  analogWrite(motorLeftPin, left);
  analogWrite(motorRightPin, right);
}

void controlMotor(int motor, int speed, bool forward) {
  if (motor == 1) {
    analogWrite(MOTOR1_EN, speed);
    digitalWrite(MOTOR1_IN1, forward ? HIGH : LOW);
    digitalWrite(MOTOR1_IN2, forward ? LOW : HIGH);
  } else if (motor == 2) {
    analogWrite(MOTOR2_EN, speed);
    digitalWrite(MOTOR2_IN1, forward ? HIGH : LOW);
    digitalWrite(MOTOR2_IN2, forward ? LOW : HIGH);
  }
}

void stopAllMotors() {
    // Set motor speeds to zero
    analogWrite(MOTOR1_EN, 0);
    analogWrite(MOTOR2_EN, 0);
}