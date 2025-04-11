#include <ESPAsyncWebServer.h>
#include "webServer.h"
#include "motorControl.h"
#include "pinConfig.h"
#include "defaultMode.h"


void initWebServer() {
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

  server.begin();
}


// Declare the index_html string here (without PROGMEM for now)
const char index_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>Motor Control</title>
    <style>
      body { font-family: Arial; text-align: center; margin: 0; padding: 0; overflow: hidden; }
      .controls {
        display: flex;
        justify-content: center;
        align-items: center;
      }
      .slider-container {
        display: flex;
        flex-direction: column;
        align-items: center;
        margin: 20px;
      }
      .slider {
        writing-mode: bt-lr;
        -webkit-appearance: slider-vertical;
        width: 15px;
        height: 150px;
      }
      .button { width: 150px; height: 50px; margin: 10px; font-size: 18px; }
      .rpm { font-size: 24px; margin: 20px; }
      .estop { background-color: red; color: white; font-size: 22px; padding: 15px; border: none; cursor: pointer; }
    </style>
    <script>
      document.addEventListener("DOMContentLoaded", function() {
        document.documentElement.requestFullscreen();
      });
    </script>
  </head>
  <body>
    <button class="estop" onclick="emergencyStop()">EMERGENCY STOP</button>
    <div class="controls">
      <div class="slider-container">
        <input type="range" min="0" max="2000" value="0" class="slider" id="motor1_speed" orient="vertical" oninput="updateMotor1Speed()">
        <button class="button" id="motor1_button" onclick="toggleMotor1()">Forward</button>
      </div>
      <div class="slider-container">
        <input type="range" min="0" max="2000" value="0" class="slider" id="motor2_speed" orient="vertical" oninput="updateMotor2Speed()">
        <button class="button" id="motor2_button" onclick="toggleMotor2()">Forward</button>
      </div>
    </div>

    <script>
      let motor1Direction = 'forward';
      let motor2Direction = 'forward';
      
      // Function to update motor 1 speed based on the slider input
      function updateMotor1Speed() {
        const speed = document.getElementById('motor1_speed').value;
        fetch(`/updateMotor1?speed=${speed}&direction=${motor1Direction}`);
      }
      
      // Function to update motor 2 speed based on the slider input
      function updateMotor2Speed() {
        const speed = document.getElementById('motor2_speed').value;
        fetch(`/updateMotor2?speed=${speed}&direction=${motor2Direction}`);
      }
      
      // Function to toggle motor 1 direction
      function toggleMotor1() {
        const speed = document.getElementById('motor1_speed').value;
        motor1Direction = motor1Direction === 'forward' ? 'reverse' : 'forward';
        document.getElementById('motor1_button').textContent = motor1Direction === 'forward' ? 'Forward' : 'Reverse';
        fetch(`/updateMotor1?speed=${speed}&direction=${motor1Direction}`);
      }
      
      // Function to toggle motor 2 direction
      function toggleMotor2() {
        const speed = document.getElementById('motor2_speed').value;
        motor2Direction = motor2Direction === 'forward' ? 'reverse' : 'forward';
        document.getElementById('motor2_button').textContent = motor2Direction === 'forward' ? 'Forward' : 'Reverse';
        fetch(`/updateMotor2?speed=${speed}&direction=${motor2Direction}`);
      }
      
      // Emergency stop function
      function emergencyStop() {
        fetch('/emergencyStop');
      }

      // Function to update RPM values (if needed)
      function updateRPM() {
        fetch('/rpm').then(response => response.json()).then(data => {
          document.getElementById('motor1_rpm').textContent = data.motor1_rpm;
          document.getElementById('motor2_rpm').textContent = data.motor2_rpm;
        });
      }
      
      setInterval(updateRPM, 1000);
    </script>
  </body>
</html>




)rawliteral";