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
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include <Update.h>
// =================== Mine ===================
#include "debug.h"
#include "calibrationMode.h"
#include "diagnosticsMode.h"
#include "webServer.h"
#include "motorControl.h"
#include "pinConfig.h"
#include "wifiManager.h"
#include "servoControl.h"
#include "gps.h"
#include "tempControl.h"
#include "BatteryControls.h"
#include "SwitchControls.h"

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

String getSwitchStatesJSON() {
  bool s1 = digitalRead(toggleSwitchPins[0]) == LOW;
  bool s2 = digitalRead(toggleSwitchPins[1]) == LOW;
  return "{\"s1\":" + String(s1 ? 1 : 0) + ",\"s2\":" + String(s2 ? 1 : 0) + "}";
}


void runDefaultSetup(AsyncWebServer& server) {
  Serial.begin(115200);

  //connectToWiFi(ssid, password);
  setupHotspot();
  setupOTA();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Ensure relay is off at startup

  setupSteeringServo() ;

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
 
server.on("/ota", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", ota_html);
});

 server.on("/js/nipplejs.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/js/nipplejs.min.js", "application/javascript");
});

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", defaultMode2_html);  // No processor used here
  });

server.on("/openServo1", HTTP_GET, [](AsyncWebServerRequest *request){
  sendServoOpen(1);
  request->send(200, "text/plain", "Servo 1 Opened");
});
server.on("/closeServo1", HTTP_GET, [](AsyncWebServerRequest *request){
  sendServoClose(1);
  request->send(200, "text/plain", "Servo 1 Closed");
});

server.on("/openServo2", HTTP_GET, [](AsyncWebServerRequest *request){
  sendServoOpen(2);
  request->send(200, "text/plain", "Servo 2 Opened");
});
server.on("/closeServo2", HTTP_GET, [](AsyncWebServerRequest *request){
  sendServoClose(2);
  request->send(200, "text/plain", "Servo 2 Closed");
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

server.on("/steering", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("angle")) {
        int angle = request->getParam("angle")->value().toInt();
        setSteeringAngle(angle);  
        request->send(200, "text/plain", "Steering angle updated");
    } else {
        request->send(400, "text/plain", "Missing angle parameter");
    }
});


  server.on("/emergencyStop", HTTP_GET, [](AsyncWebServerRequest *request){
    // Code to stop all motors
    stopAllMotors();
    request->send(200, "text/plain", "Emergency Stop Activated");
});

server.on("/gps", HTTP_GET, [](AsyncWebServerRequest *request){
  if (gps.location.isValid()) {
    String json = "{";
    json += "\"lat\":" + String(gps.location.lat(), 6) + ",";
    json += "\"lon\":" + String(gps.location.lng(), 6);
    json += "}";
    request->send(200, "application/json", json);
  } else {
    request->send(200, "application/json", "{\"lat\":null,\"lon\":null}");
  }
});
/* server.on("/gps", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", getGPSData());
}); */
  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request) {
    float temp = tempControl::getTemperature();

    String json = "{\"temp\":" + String(temp, 1) + "}";
    request->send(200, "application/json", json);
  });

server.on("/relayStatus", HTTP_GET, [](AsyncWebServerRequest *request){
  String status = tempControl::relayIsOn ? "on" : "off";// Use the instance of tempControl
  request->send(200, "application/json", "{\"relay\":\"" + status + "\"}");
});

server.on("/battery", HTTP_GET, [](AsyncWebServerRequest *request){
  float voltage = readBatteryVoltage();
  int percent = getBatteryPercentage();
  request->send(200, "application/json", 
    String("{\"percent\":") + percent + ",\"voltage\":" + voltage + "}");
});

server.on("/getSwitchStates", HTTP_GET, [](AsyncWebServerRequest *request){
  StaticJsonDocument<100> doc;
  doc["switch1"] = digitalRead(SWITCH1_PIN);
  doc["switch2"] = digitalRead(SWITCH2_PIN);
  String json;
  serializeJson(doc, json);
  request->send(200, "application/json", json);
});
server.on("/switchState", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "application/json", getSwitchStatesJSON());
});

  server.onNotFound([](AsyncWebServerRequest *request) {
    String json = "{\"error\": \"Route not found\"}";
    request->send(404, "application/json", json);
});

server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
    bool shouldReboot = !Update.hasError();
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain",
        shouldReboot ? "Update OK. Rebooting..." : "Update Failed!");
    response->addHeader("Connection", "close");
    request->send(response);
    if (shouldReboot) {
        delay(1000);
        ESP.restart();
    }
}, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data,
      size_t len, bool final){
    if (!index){
        Serial.printf("Update Start: %s\n", filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
            Update.printError(Serial);
        }
    }
    if (!Update.hasError()) {
        if (Update.write(data, len) != len) {
            Update.printError(Serial);
        }
    }
    if (final) {
        if (Update.end(true)) {
            Serial.println("Update complete.");
        } else {
            Update.printError(Serial);
        }
    }
});
}



void runDefaultLoop(){

  // Check for OTA updates
  ArduinoOTA.handle();
  updateGPS(); 
  // Check ?
  delay(updateDelay);
}
