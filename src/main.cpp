/* Ultrstate base code system 


*/
#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <TinyGPSPlus.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>
#include <WebSerial.h>
//#include <Adafruit_NeoPixel.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <set>

// =================== Mine ===================
#include "defaultMode.h"
#include "debug.h"
#include "calibrationMode.h"
#include "diagnosticsMode.h"
#include "webServer.h"
#include "motorControl.h"
//#include "ledControl.h"
#include "pinConfig.h"
#include "wifiManager.h"
#include "telemetry.h"
#include "gps.h"
#include "fullRuntimeMode.h"

const int toggleSwitchPins[] = {SWITCH1_PIN, SWITCH2_PIN}; // Example pins
const int numSwitches = 2;


// Define target states
const bool targetStates[2][numSwitches] = {
  {1, 0}, //default diagnositcs 
  {0, 1} // calibraion mode 
};

bool switchStates[numSwitches];

// LED Strip Setup
const int numLEDs = 5;
const int ledPin = 4;
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDs, ledPin, NEO_GRB + NEO_KHZ800);



// =================== Wi-Fi Setup ===================


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// =================== Objects ===================
MPU6050 mpu;
HardwareSerial gpsSerial(1);


// =================== Globals ===================
unsigned long lastTelemetryTime = 0;
int telemetryInterval = 1000;
// Variables for GPS data
double currentLat = 29.676096;
double currentLon = -98.057546;
// Serial data variable
String serialData = "";
bool autoScroll = true;  // Autoscroll toggle
const int analogResolutionValue = 24;

// =================== WebSocket event handler ===================
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("WebSocket client connected");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("WebSocket client disconnected");
  } else if (type == WS_EVT_DATA) {
    // Handle incoming data (if needed)
    String message = String((char*)data);
    Serial.println("Received message: " + message);
  }
}

// =================== Sensor Reading ===================
String getIMUData() {
  // Declare variables for sensor data
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  
  // Get the raw sensor data
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Return the data as a formatted string
  return "IMU:AX=" + String(ax) + " AY=" + String(ay) + " AZ=" + String(az) +
         " GX=" + String(gx) + " GY=" + String(gy) + " GZ=" + String(gz);
}

String getGPSData() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isValid()) {
    return "GPS:LAT=" + String(gps.location.lat(), 6) +
           " LNG=" + String(gps.location.lng(), 6);
  }
  return "GPS:No Fix";
}

float readBatteryVoltage() {
  return analogRead(BATTERY_PIN) * (3.3 / 4095.0) * 2; // Adjust if using voltage divider
}
// =================== WebSerial Setup ===================

unsigned long last_print_time = millis();

 int matchedIndex =0;

bool compareStates(const bool currentState[], const bool targetState[]) {
  for (int i = 0; i < numSwitches; i++) {
    if (currentState[i] != targetState[i]) {
      return false;
    }
  }
  return true;
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



// Define your setup functions for different target states
void runTargetStateSetup(int index) {
  Serial.print("Switches match target state ");
  Serial.println(index);
  switch (index){
    case 0:
    runDefaultSetup(server);
    case 1:
   runDefaultSetup(server);
    default:
    runDefaultSetup(server);
    
  }
  // Your code for the target state setup here
}

// Define your loop functions for different target states
void runTargetStateLoop(int index) {
    switch (index){
    case 0:
    runDefaultLoop();
    break;
    case 1:
    runDefaultLoop();
    break;
    default:
    runDefaultLoop();
    break;
    }
  // Your loop code for the target state here
  
}

void checkToggleSwitches() {
  static int lastSwitchStates[numSwitches];
  for (int i = 0; i < numSwitches; i++) {
    int currentState = digitalRead(toggleSwitchPins[i]);
    if (currentState != lastSwitchStates[i]) {
      lastSwitchStates[i] = currentState;
      if (currentState == LOW) { // Assuming LOW means ON
    
      }
    }
  }
}

void checkForDuplicatePins() {
    std::set<int> usedPins = {
        MOTOR1_IN1, MOTOR1_IN2, MOTOR1_EN,
        MOTOR2_IN1, MOTOR2_IN2, MOTOR2_EN,
        ENCODER1_A, ENCODER1_B,
        ENCODER2_A, ENCODER2_B,
        TEMP_SENSOR_PIN, RELAY_PIN,
        BATTERY_PIN, GPS_RX_PIN, GPS_TX_PIN
    };

    if (usedPins.size() < 15) {
        Serial.println("[⚠️ WARNING] Duplicate pins detected in configuration!");
    } else {
        Serial.println("[✅ Pin Check] All pins are uniquely defined.");
    }
}

void setup() {
  Serial.begin(115200);
initGPS();
  // Initialize OTA
  
    ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  checkForDuplicatePins();

  //need to initilize the motor pins and other pins
  // Set analog read resolution
  analogReadResolution(analogResolutionValue);

  // Initialize switch pins
  for (int i = 0; i < numSwitches; i++) {
    pinMode(toggleSwitchPins[i], INPUT_PULLUP); // Use INPUT_PULLUP if your switches are wired to ground
    switchStates[i] = digitalRead(toggleSwitchPins[i]) == LOW; // Assuming active low configuration
  }

  // Determine which setup and loop function to call based on switch states
  matchedIndex = findMatchingTargetState();
  if (matchedIndex != -1) {
    runTargetStateSetup(matchedIndex);
  } else {
    runDefaultSetup(server);
  }
//sensors.begin();
server.begin();

}

void loop() {
    //This is needed for OTA updates
  ArduinoOTA.handle();
  // Determine which loop function to call based on switch states
  
  if (matchedIndex != -1) {
    runTargetStateLoop(matchedIndex);
  } else {
    runDefaultLoop();
  }
  // admin work for all states
  //sensors.requestTemperatures();
}