#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// =================== Pin Configuration ===================

// Motor 1 (L298N)
#define MOTOR1_IN1 1
#define MOTOR1_IN2 2
#define MOTOR1_EN  42  // PWM

// Motor 2 (L298N)
#define MOTOR2_IN1 6
#define MOTOR2_IN2 5
#define MOTOR2_EN  4  // PWM

// Encoders
#define ENCODER1_A 8
#define ENCODER1_B 15

#define ENCODER2_A 10
#define ENCODER2_B 9

// Sensors and Actuators
#define TEMP_SENSOR_PIN 21
#define RELAY_PIN       47

// Battery
#define BATTERY_PIN 35  // Analog input

// GPS UART Pins
#define GPS_RX_PIN 18  // GPS TX -> ESP RX
#define GPS_TX_PIN 17  // GPS RX -> ESP TX (optional)

//Switch Pins for mode
#define SWITCH1_PIN 36
#define SWITCH2_PIN 37

//Steering Servo
#define STEERING_PIN 3

// Drop Servos
#define DROP_SERVO_1_PIN 12
#define DROP_SERVO_2_PIN 13

#define wsLedPin 7
#define wsLedCount 10
#define wsLedType WS2812B
#define wsLedColorOrder GRB
#define wsLedBrightness 50
#define wsLedMode 0
#define wsLedSpeed 100
// Add any additional pins here...

#endif // PIN_CONFIG_H
