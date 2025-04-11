#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <TinyGPSPlus.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h>
#include <WebSerial.h>
//#include <DallasTemperature.h>
#include <set>

// =================== Mine ===================
#include "debug.h"
#include "calibrationMode.h"
#include "diagnosticsMode.h"
#include "webServer.h"
#include "motorControl.h"
#include "pinConfig.h"
#include "wifiManager.h"




const char* ssid = "homesweethome";
const char* password = "johnandamy";

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

void IRAM_ATTR handleEncoderMotor1() {
  int stateA = digitalRead(ENCODER1_A);
  int stateB = digitalRead(ENCODER1_B);
  
  if (stateA == stateB) {
    pulse_count_motor1++;  // Forward
    direction_motor1 = 1;
  } else {
    pulse_count_motor1--;  // Reverse
    direction_motor1 = -1;
  }
}

// Function to handle Motor 2 encoder (quadrature)
void IRAM_ATTR handleEncoderMotor2() {
  int stateA = digitalRead(ENCODER2_A);
  int stateB = digitalRead(ENCODER2_B);
  
  if (stateA == stateB) {
    pulse_count_motor2++;  // Forward
    direction_motor2 = 1;
  } else {
    pulse_count_motor2--;  // Reverse
    direction_motor2 = -1;
  }
}


void runDefaultSetup(AsyncWebServer& server) {
  Serial.begin(115200);

 connectToWiFi(ssid, password);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Ensure relay is off at startup

  Serial.println("Boot Started");
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
  pinMode(MOTOR1_EN, OUTPUT);
  pinMode(MOTOR2_EN, OUTPUT);

  pinMode(ENCODER1_A, INPUT_PULLUP);
  pinMode(ENCODER1_B, INPUT_PULLUP);
  pinMode(ENCODER2_A, INPUT_PULLUP);
  pinMode(ENCODER2_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER1_A), handleEncoderMotor1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER2_A), handleEncoderMotor2, CHANGE);

    // Set up the ESP32 as an Access Point
 Serial.println("Starting Wifi");
  //WiFi.softAP(ssid, password);
 // WiFi.softAPConfig(local_IP, gateway, subnet);

  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);  // No processor used here
  });

  server.on("/updateMotor1", HTTP_GET, [](AsyncWebServerRequest *request) {
    String speed = request->getParam("speed")->value();
    String direction = request->getParam("direction")->value();
    controlMotor(1, speed.toInt(), direction == "forward");
    request->send(200, "text/plain", "OK");
  });

  server.on("/updateMotor2", HTTP_GET, [](AsyncWebServerRequest *request) {
    String speed = request->getParam("speed")->value();
    String direction = request->getParam("direction")->value();
    controlMotor(2, speed.toInt(), direction == "forward");
    request->send(200, "text/plain", "OK");
  });
  
  server.on("/rpm", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{\"motor1_rpm\":" + String(rpm_motor1) + ",\"motor2_rpm\":" + String(rpm_motor2) + "}";
    request->send(200, "application/json", json);
});

  server.on("/emergencyStop", HTTP_GET, [](AsyncWebServerRequest *request){
    // Code to stop all motors
    stopAllMotors();
    request->send(200, "text/plain", "Emergency Stop Activated");
});

  server.onNotFound([](AsyncWebServerRequest *request) {
        request->redirect("/");
    });


}



void runDefaultLoop(){
  // Check for OTA updates
  ArduinoOTA.handle();
  // Check ?
  delay(updateDelay);
}
