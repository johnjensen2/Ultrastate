/* Ultrstate base code system 


*/

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
#define DEBUG 1

/* debugging time

#define DEBUG 1  //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG    //Macros are usually in all capital letters.
   #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
   #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
   #define DPRINT(...)     //now defines a blank line
   #define DPRINTLN(...)   //now defines a blank line
#endif
*/

// Pin Definitions
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

AsyncWebServer server(80);


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



void runDefaultSetup() {
  Serial.println("Switches do not match any target state");
  
  
  // Initialize the hardware components
  pinMode(linearPot1Pin, INPUT);
  pinMode(linearPot2Pin, INPUT);
  pinMode(rotaryPot1Pin, INPUT);
  pinMode(rotaryPot2Pin, INPUT);

  for (int i = 0; i < numSwitches; i++) {
    pinMode(toggleSwitchPins[i], INPUT_PULLUP);
  }

  // Initialize the LED strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Set up Wi-Fi
  WiFi.begin("homesweethome", "johnandamy");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  // Set up the web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  // updateWebPage();
    String html = "<html><head><title>ESP32 Control Panel</title>";
    html += "<script>";
    html += "function fetchData() {";
    html += "  fetch('/data')"; 
    html += "    .then(response => response.json())"; 
    html += "    .then(data => {";

    html += "      document.getElementById('Switch1Value').textContent = 'Switch 1: ' + data.Switch1Value;";
    html += "      document.getElementById('Switch2Value').textContent = 'Switch 2: ' + data.Switch2Value;";
    html += "      document.getElementById('Switch3Value').textContent = 'Switch 3: ' + data.Switch3Value;";
    html += "      document.getElementById('Switch4Value').textContent = 'Switch 4: ' + data.Switch4Value;";
    html += "      document.getElementById('Switch5Value').textContent = 'Switch 5: ' + data.Switch5Value;";
                
    html += "      document.getElementById('motorLeftSpeed').textContent = 'Motor Left Speed: ' + data.motorLeftSpeed;";
    html += "      document.getElementById('motorRightSpeed').textContent = 'Motor Right Speed: ' + data.motorRightSpeed;";
    html += "      document.getElementById('trimValueLeft').textContent = 'Motor Left Trim: ' + data.trimValueLeft;";
    html += "      document.getElementById('trimValueRight').textContent = 'Motor Right Trim: ' + data.trimValueRight;";
    html += "      document.getElementById('linearPot1Value').textContent = 'Linear Pot 1: ' + data.linearPot1Value;";
    html += "      document.getElementById('linearPot2Value').textContent = 'Linear Pot 2: ' + data.linearPot2Value;";
    html += "      document.getElementById('rotaryPot1Value').textContent = 'Rotary Pot 1: ' + data.rotaryPot1Value;";
    html += "      document.getElementById('rotaryPot2Value').textContent = 'Rotary Pot 2: ' + data.rotaryPot2Value;";
    html += "    });";
    html += "}";
    html += "setInterval(fetchData, 1000);"; // Update every second
    html += "</script>";
    html += "</head><body>";
    html += "<h1>ESP32 Control Panel</h1>";
    html += "<p>Toggle Switches:</p>";

    html += "<p id='Switch1Value'>Switch 1 Value: </p>";
    html += "<p id='Switch2Value'>Switch 1 Value: </p>";
    html += "<p id='Switch3Value'>Switch 1 Value: </p>";
    html += "<p id='Switch4Value'>Switch 1 Value: </p>";
    html += "<p id='Switch5Value'>Switch 1 Value: </p>";
    
    html += "<p id='motorLeftSpeed'>Motor Left Speed: </p>";
    html += "<p id='motorRightSpeed'>Motor Right Speed: </p>";
    html += "<p id='trimValueLeft'>Motor Left Trim: </p>";
    html += "<p id='trimValueRight'>Motor Right Trim: </p>";
    html += "<p id='linearPot1Value'>Linear Pot 1: </p>";
    html += "<p id='linearPot2Value'>Linear Pot 2: </p>";
    html += "<p id='rotaryPot1Value'>Rotary Pot 1: </p>";
    html += "<p id='rotaryPot2Value'>Rotary Pot 2: </p>";

    // LED Color Display
    html += "<p>LED Colors:</p>";
    html += "<div style='display: flex; justify-content: center; align-items: center;'>";
    for (int i = 0; i < numLEDs; i++) {
      uint32_t color = strip.getPixelColor(i);
      int r = (color >> 16) & 0xFF;
      int g = (color >> 8) & 0xFF;
      int b = color & 0xFF;
      html += "<div style='width: 30px; height: 30px; background-color: rgb(" + String(r) + "," + String(g) + "," + String(b) + "); margin: 5px;'></div>";
    }
    html += "</div>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

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

  // Start the server
  server.begin();
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

void runDefaultLoop() {
  // Your loop code for the default state here
  static unsigned long lastUpdate = 0;
  unsigned long now = millis();

  // Check and update pots and switches
  checkPots();
  checkToggleSwitches();

  // Update LEDs
  updateLEDs();

  // Check if it's time to update
  if (now - lastUpdate >= updateDelay) {
    lastUpdate = now;
    updateWebPage();
  }
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
