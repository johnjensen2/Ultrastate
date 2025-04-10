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
#include "pinConfig.h"

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


const int toggleSwitchPins[] = {SWITCH1_PIN, SWITCH2_PIN}; // Example pins
const int numSwitches = 2;


// Define target states
const bool targetStates[1][numSwitches] = {
  {1, 0}, //default diagnositcs 
  {0, 1} // calibraion mode 
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
  return analogRead(batteryPin) * (3.3 / 4095.0) * 2; // Adjust if using voltage divider
}
// =================== WebSerial Setup ===================

unsigned long last_print_time = millis();

 int matchedIndex =0;



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

void setup() {
  Serial.begin(115200);

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
    runDefaultSetup();
  }

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
}