#include "servoControl.h"
#include <Arduino.h>
#include <ESP32Servo.h>
#include "pinConfig.h"

Servo steeringServo;
Servo dropServo1;
Servo dropServo2;


// Servo angle configuration
const int SERVO_LEFT = 40;    // Full left physical angle
const int SERVO_CENTER = 90;  // Center angle
const int SERVO_RIGHT = 135;  // Full right physical angle

// Logical input range
const int INPUT_MIN = -45;
const int INPUT_MAX = 45;

int currentSteeringAngle = 90; // Neutral center position

void setupSteeringServo() {
    steeringServo.setPeriodHertz(50); // Standard 50Hz servo
    steeringServo.attach(STEERING_PIN);
    steeringServo.write(currentSteeringAngle);
}

// Converts joystick X input (-1.0 to 1.0) to servo range (45 to 135 degrees)
void setSteeringFromJoystick(float xVal) {
    int angle = map(xVal * 100, -100, 100, 45, 135);  
    angle = constrain(angle, 45, 135);  
    currentSteeringAngle = angle;
    steeringServo.write(currentSteeringAngle);
}

// Directly sets servo angle (used by API)
// Expect input: -45 = full left, 0 = center, +45 = full right
void setSteeringAngle(int angle) {
    angle = constrain(angle, INPUT_MIN, INPUT_MAX);
    int servoValue = map(angle, INPUT_MIN, INPUT_MAX, SERVO_LEFT, SERVO_RIGHT);
    steeringServo.write(servoValue);
}

// Drop servo control
void setupDropServos() {
    dropServo1.setPeriodHertz(50);
    dropServo2.setPeriodHertz(50);
    dropServo1.attach(DROP_SERVO_1_PIN);
    dropServo2.attach(DROP_SERVO_2_PIN);
    sendServoClose(1);
    sendServoClose(2);
}

void sendServoOpen(uint8_t dropNum) {
    if (dropNum == 1) dropServo1.write(120);  // Adjust as needed
    else if (dropNum == 2) dropServo2.write(120);
}

void sendServoClose(uint8_t dropNum) {
    if (dropNum == 1) dropServo1.write(0);
    else if (dropNum == 2) dropServo2.write(0);
}
