/* Ultrstate base code system 


*/
#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <TinyGPSPlus.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h>
#include <WebSerial.h>
#include <Adafruit_NeoPixel.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// =================== Mine ===================
#include "defaultMode.h"
#include "debug.h"
#include "calibrationMode.h"
#include "diagnosticsMode.h"
#include "webServer.h"
#include "motorControl.h"
#include "ledControl.h"

#define DEBUG 1

//debugging time

#define DEBUG 1  //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG    //Macros are usually in all capital letters.
   #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
   #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
   #define DPRINT(...)     //now defines a blank line
   #define DPRINTLN(...)   //now defines a blank line
#endif


// =================== Pin Config ===================
//need to sort this out still
#define MOTOR1_IN1 4  // L298N Input Pins for Motor 1
#define MOTOR1_IN2 5
#define MOTOR1_EN 6  // PWM Pin for Motor 1 Speed

#define MOTOR2_IN1 1  // L298N Input Pins for Motor 2
#define MOTOR2_IN2 2
#define MOTOR2_EN 42  // PWM Pin for Motor 2 Speed

#define ENCODER1_A 7  // Motor 1 Encoder output A
#define ENCODER1_B 15  // Motor 1 Encoder output B
#define ENCODER2_A 16  // Motor 2 Encoder output A
#define ENCODER2_B 17  // Motor 2 Encoder output B

#define TEMP_SENSOR_PIN 18  // Change if needed, avoiding your reserved pins
#define RELAY_PIN 19         // Change if needed
const int motorLeftPWM = 5;
const int motorLeftDir = 4;
const int motorRightPWM = 6;
const int motorRightDir = 7;

const int batteryPin = 34; // Analog input
const int gpsRxPin = 16;   // GPS TX -> ESP RX
const int gpsTxPin = 17;   // GPS RX -> ESP TX (optional)

const int linearPot1Pin = 6;
const int linearPot2Pin = 7;
const int rotaryPot1Pin = 5;
const int rotaryPot2Pin = 4;
const int toggleSwitchPins[] = {15, 16, 17, 18, 8}; // Example pins
const int numSwitches = 5;


// Define 10 target states
const bool targetStates[10][numSwitches] = {
  {1, 0, 1, 0, 1}, //default diagnositcs 
  {0, 1, 0, 1, 0}, // calibraion mode
  {1, 1, 0, 0, 1},
  {0, 0, 1, 1, 0},
  {1, 0, 0, 1, 1},
  {0, 1, 1, 0, 1},
  {1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1},
  {1, 0, 1, 1, 0},
  {0, 1, 0, 1, 1}
};

bool switchStates[numSwitches];

// LED Strip Setup
const int numLEDs = 5;
const int ledPin = 4;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDs, ledPin, NEO_GRB + NEO_KHZ800);

// Variables
int linearPot1Value = 0;
int linearPot2Value = 0;
int rotaryPot1Value = 0;
int rotaryPot2Value = 0;
//int switchStates[numSwitches] = {HIGH, HIGH, HIGH, HIGH, HIGH};
int motorLeftSpeed = 0;
int motorRightSpeed = 0;
int trimValueLeft = 0;
int trimValueRight = 0;

// Constants
const int analogResolutionValue = 24;
const int MOTOR_MIN_SPEED = 0;
const int MOTOR_MAX_SPEED = 255;
const int TRIM_MIN = -50;
const int TRIM_MAX = 50;
const int debounceDelay = 50;
const int updateDelay = 1000; // 1 second


// User-set temperature limits (adjustable in code)
float TEMP_MIN = 20.0; // Minimum temperature to turn off cooling
float TEMP_MAX = 30.0; // Maximum temperature to turn on cooling

volatile long pulse_count_motor1 = 0;
volatile long pulse_count_motor2 = 0;
volatile int direction_motor1 = 0;  // 1 for forward, -1 for reverse
volatile int direction_motor2 = 0;

unsigned long last_rpm_update = 0;
unsigned long rpm_interval = 1000;  // Update RPM every second
int rpm_motor1 = 0;
int rpm_motor2 = 0;

// =================== Wi-Fi Setup ===================
const char* ssid = "homesweethome";
const char* password = "johnandamy";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// =================== Objects ===================
MPU6050 mpu;
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature sensors(&oneWire);

// =================== Globals ===================
unsigned long lastTelemetryTime = 0;
int telemetryInterval = 1000;
// Variables for GPS data
double currentLat = 29.676096;
double currentLon = -98.057546;
// Serial data variable
String serialData = "";
bool autoScroll = true;  // Autoscroll toggle


void setup() {
  Serial.begin(115200);
  
  // Set analog read resolution
  analogReadResolution(analogResolutionValue);
  
  // Initialize switch pins
  for (int i = 0; i < numSwitches; i++) {
    pinMode(toggleSwitchPins[i], INPUT_PULLUP); // Use INPUT_PULLUP if your switches are wired to ground
    switchStates[i] = digitalRead(toggleSwitchPins[i]) == LOW; // Assuming active low configuration
  }

  // Determine which setup and loop function to call based on switch states
  int matchedIndex = findMatchingTargetState();
  if (matchedIndex != -1) {
    runTargetStateSetup(matchedIndex);
  } else {
    runDefaultSetup();
  }
}

void loop() {
  // Determine which loop function to call based on switch states
  int matchedIndex = findMatchingTargetState();
  if (matchedIndex != -1) {
    runTargetStateLoop(matchedIndex);
  } else {
    runDefaultLoop();
  }
}

// Function to compare current states with all target states
int findMatchingTargetState() {
  for (int i = 0; i < 10; i++) {
    if (compareStates(switchStates, targetStates[i])) {
      return i; // Return the index of the matching target state
    }
  }
  return -1; // Return -1 if no match is found
}

bool compareStates(const bool currentState[], const bool targetState[]) {
  for (int i = 0; i < numSwitches; i++) {
    if (currentState[i] != targetState[i]) {
      return false;
    }
  }
  return true;
}

// Define your setup functions for different target states
void runTargetStateSetup(int index) {
  Serial.print("Switches match target state ");
  Serial.println(index);
  switch (index){
    case 0:
    diagno_setup();
    case 1:
    calbration_setup();
    default:
    runDefaultSetup();
    
  }
  // Your code for the target state setup here
}



// Define your loop functions for different target states
void runTargetStateLoop(int index) {
    switch (index){
    case 0:
    diagno_loop();
    break;
    case 1:
    calbration_loop();
    break;
    default:
    runDefaultSetup();
    break;
    }
  // Your loop code for the target state here
  
}


// Functions for checking pots, switches, and updating LEDs
void checkPots() {
  int newLinearPot1Value = analogRead(linearPot1Pin);
  int newLinearPot2Value = analogRead(linearPot2Pin);
  int newRotaryPot1Value = analogRead(rotaryPot1Pin);
  int newRotaryPot2Value = analogRead(rotaryPot2Pin);

  // Calculate throttle speeds based on linear pots
  int throttleLeft = map(newLinearPot1Value, 0, pow(2, analogResolutionValue) - 1, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);
  int throttleRight = map(newLinearPot2Value, 0, pow(2, analogResolutionValue) - 1, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);

  // Adjust trim values based on rotary pots
  int newTrimValueLeft = map(newRotaryPot1Value, 0, pow(2, analogResolutionValue) - 1, TRIM_MIN, TRIM_MAX);
  int newTrimValueRight = map(newRotaryPot2Value, 0, pow(2, analogResolutionValue) - 1, TRIM_MIN, TRIM_MAX);

  // Update motor speeds
  motorLeftSpeed = throttleLeft + newTrimValueLeft;
  motorRightSpeed = throttleRight + newTrimValueRight;

  // Constrain motor speeds to be within defined limits
  motorLeftSpeed = constrain(motorLeftSpeed, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);
  motorRightSpeed = constrain(motorRightSpeed, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);

  // Update previous values for pots
  linearPot1Value = newLinearPot1Value;
  linearPot2Value = newLinearPot2Value;
  rotaryPot1Value = newRotaryPot1Value;
  rotaryPot2Value = newRotaryPot2Value;
}


void checkToggleSwitches() {
  static int lastSwitchStates[numSwitches];
  for (int i = 0; i < numSwitches; i++) {
    int currentState = digitalRead(toggleSwitchPins[i]);
    if (currentState != lastSwitchStates[i]) {
      lastSwitchStates[i] = currentState;
      if (currentState == LOW) { // Assuming LOW means ON
        handleSwitchOn(i);
      } else {
        handleSwitchOff(i);
      }
    }
  }
}

void handleSwitchOn(int switchIndex) {
  // Action for switch ON
  Serial.println("Switch " + String(switchIndex + 1) + " ON");
  // Update corresponding LED
  strip.setPixelColor(switchIndex, strip.Color(0, 255, 0)); // Green for ON
  strip.show();
}

void handleSwitchOff(int switchIndex) {
  // Action for switch OFF
  Serial.println("Switch " + String(switchIndex + 1) + " OFF");
  // Update corresponding LED
  strip.setPixelColor(switchIndex, strip.Color(255, 0, 0)); // Red for OFF
  strip.show();
}
  
  void updateWebPage() {

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"motorLeftSpeed\":" + String(motorLeftSpeed) + ",";
    json += "\"motorRightSpeed\":" + String(motorRightSpeed) + ",";
    json += "\"trimValueLeft\":" + String(trimValueLeft) + ",";
    json += "\"trimValueRight\":" + String(trimValueRight) + ",";
    json += "\"linearPot1Value\":" + String(linearPot1Value) + ",";
    json += "\"linearPot2Value\":" + String(linearPot2Value) + ",";
    json += "\"rotaryPot1Value\":" + String(rotaryPot1Value) + ",";
    json += "\"rotaryPot2Value\":" + String(rotaryPot2Value) + ",";
    json += "\"Switch1Value\":" + String(switchStates[0] == LOW ? "ON" : "OFF") + ",";
    json += "\"Switch2Value\":" + String(switchStates[0] == LOW ? "ON" : "OFF") + ",";
    json += "\"Switch3Value\":" + String(switchStates[0] == LOW ? "ON" : "OFF") + ",";
    json += "\"Switch4Value\":" + String(switchStates[0] == LOW ? "ON" : "OFF") + ",";
    json += "\"Switch5Value\":" + String(switchStates[0] == LOW ? "ON" : "OFF") + "}";    
    request->send(200, "application/json", json);
  });

  }
  
  void updateLEDs() {
    // Placeholder function to update LED colors or states
  }
