#include <Arduino.h>
#include "motorControl.h"

// Example motor pin values
const int motorLeftPin = 25;
const int motorRightPin = 26;

void initMotors() {
  pinMode(motorLeftPin, OUTPUT);
  pinMode(motorRightPin, OUTPUT);
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