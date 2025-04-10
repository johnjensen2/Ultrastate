#include <Arduino.h>
#include "defaultMode.h"


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