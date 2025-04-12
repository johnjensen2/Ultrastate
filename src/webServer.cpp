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

const char index_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
</html>
)rawliteral";





// Declare the index_html string here (without PROGMEM for now)
const char defaultMode_html[] = R"rawliteral(
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
.estop {
  background-color: grey;
  color: white;
  font-size: 22px;
  padding: 15px;
  border: none;
  cursor: pointer;
  transition: background-color 0.3s;
}
.estop.active {
  background-color: red;
}

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
<button onclick="resetEmergencyStop()" style="margin-top: 10px; padding: 10px;">Reset Emergency</button>

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
  fetch('/emergencyStop')
    .then(() => {
      const button = document.querySelector('.estop');
      button.classList.add('active');
      button.disabled = true;
    });
}
function resetEmergencyStop() {
  fetch('/resetEmergencyStop')
    .then(() => {
      const button = document.querySelector('.estop');
      button.classList.remove('active');
      button.disabled = false;
    });
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

const char defaultMode2_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Tank Control - Dark Mode</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      background-color: #121212;
      color: #e0e0e0;
    }
    h3 {
      margin: 0;
    }
    .top-bar {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 10px 20px;
      background-color: #1f1f1f;
      border-bottom: 2px solid #333;
    }
    .battery-container {
      display: flex;
      align-items: center;
    }
    .battery-icon {
      width: 40px;
      height: 20px;
      border: 2px solid #888;
      border-radius: 3px;
      position: relative;
      margin-right: 10px;
    }
    .battery-icon::after {
      content: "";
      position: absolute;
      right: -6px;
      top: 4px;
      width: 4px;
      height: 12px;
      background-color: #888;
      border-radius: 1px;
    }
    .battery-level {
      height: 100%;
      width: 60%;
      background-color: #4caf50;
    }
    .container {
      display: flex;
      justify-content: center;
      align-items: center;
      height: 60vh;
    }
    .joystick-zone {
      width: 150px;
      height: 150px;
      margin: 20px;
      background-color: #2a2a2a;
      border: 2px solid #444;
      border-radius: 10px;
      position: relative;
      touch-action: none;
    }
    .center-controls {
      display: flex;
      flex-direction: column;
      align-items: center;
      margin: 20px;
    }
    .estop, .stopAll {
      background-color: #333;
      color: white;
      font-size: 18px;
      padding: 12px 24px;
      border: none;
      cursor: pointer;
      margin: 10px;
      border-radius: 8px;
      transition: background-color 0.3s;
    }
    .estop.active {
      background-color: red;
    }
    .estop:hover, .stopAll:hover {
      background-color: #555;
    }
    .drop-row {
      display: flex;
      justify-content: center;
      margin-top: -20px;
      margin-bottom: 10px;
    }

.drop-buttons {
  display: flex;
  justify-content: center;
  align-items: center;
  margin-top: -45px;
  gap: 150px;
  width: 100%;
}
  background-color: #555;
color: white;
      font-size: 18px;
      padding: 12px 24px;
      margin: 0 20px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      transition: background-color 0.3s;
      }
.dropBtn.red {
  background-color: red;
}
    .dropBtn:disabled {
      background-color: #777;
      cursor: not-allowed;
    }
    .dropBtn:hover:not(:disabled) {
      background-color: #555;
    }
  </style>
  <script src="https://cdn.jsdelivr.net/npm/nipplejs@0.9.0/dist/nipplejs.min.js"></script>
</head>
<body>
  <div class="top-bar">
    <h3>RC Tank Control</h3>
    <div class="battery-container">
      <div class="battery-icon">
        <div class="battery-level" id="batteryLevel"></div>
      </div>
      <span id="batteryStatus">Battery: N/A</span>
    </div>
  </div>

  <div class="container">
    <div id="left_joystick" class="joystick-zone"></div>
    <div class="center-controls">
      <button class="stopAll" onclick="stopAllMotors()">STOP ALL</button>
      <button class="estop" id="estopBtn" onclick="emergencyStop()">EMERGENCY STOP</button>
    </div>
    <div id="right_joystick" class="joystick-zone"></div>
  </div>

<div class="drop-buttons">
  <button class="drop estop" id="drop1" onclick="sendServoCommand(1)">DROP 1</button>
  <button class="drop estop" id="drop2" onclick="sendServoCommand(2)">DROP 2</button>
</div>

  <script>
    const deadZone = 0.2;
    let estopActive = false;

    function sendMotorCommand(motor, speed, direction) {
      fetch(`/updateMotor${motor}?speed=${speed}&direction=${direction}`);
    }

    function stopAllMotors() {
      sendMotorCommand(1, 0, "stop");
      sendMotorCommand(2, 0, "stop");
    }

    function emergencyStop() {
      fetch('/emergencyStop');
      estopActive = true;
      const btn = document.getElementById("estopBtn");
      btn.classList.add("active");
      btn.textContent = "STOPPED";
    }
// servoControl.js

const servoStates = {
  drop1: false,
  drop2: false
};

function sendServoCommand(dropNum) {
  const buttonId = `drop${dropNum}`;
  const button = document.getElementById(buttonId);
  const isOpen = servoStates[buttonId];

  // Toggle state
  servoStates[buttonId] = !isOpen;

  if (servoStates[buttonId]) {
    // If it's open now, allow retraction
    button.style.backgroundColor = 'green';
    button.textContent = `OPEN ${dropNum}`;
    sendServoOpen(dropNum);
  } else {
    // If it's closed now, show as DROP again
    button.style.backgroundColor = 'red';
    button.textContent = `CLOSE ${dropNum}`;
    sendServoClose(dropNum);
  }
}

function sendServoOpen(dropNum) {
  fetch(`/openServo${dropNum}`);
}

function sendServoClose(dropNum) {
  fetch(`/closeServo${dropNum}`);
}



    function createJoystick(id, motorNumber) {
      const zone = document.getElementById(id);
      const joystick = nipplejs.create({
        zone: zone,
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'white',
        size: 120,
        restOpacity: 0.6,
      });

      joystick.on('move', (evt, data) => {
        if (!data || !data.vector) return;
        const dy = data.vector.y;
        const distance = Math.min(data.distance / (data.instance.options.size / 2), 1);
        const abs = Math.abs(dy);

        if (abs < deadZone) {
          sendMotorCommand(motorNumber, 0, "stop");
          return;
        }

        const speed = Math.floor(2000 * abs);
        const direction = dy < 0 ? "forward" : "reverse";
        sendMotorCommand(motorNumber, speed, direction);
      });

      joystick.on('end', () => {
        sendMotorCommand(motorNumber, 0, "stop");
      });
    }

    window.onload = () => {
      createJoystick("left_joystick", 1);
      createJoystick("right_joystick", 2);
    };

    function updateBatteryDisplay(percent) {
      document.getElementById("batteryStatus").textContent = `Battery: ${percent}%`;
      document.getElementById("batteryLevel").style.width = `${percent}%`;
      const level = document.getElementById("batteryLevel");
      level.style.backgroundColor = percent > 50 ? '#4caf50' : percent > 20 ? '#ff9800' : '#f44336';
    }
  </script>
</body>
</html>
)rawliteral";
