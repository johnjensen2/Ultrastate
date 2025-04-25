#include <ESPAsyncWebServer.h>
#include "webServer.h"
#include "motorControl.h"
#include "pinConfig.h"
#include "defaultMode.h"
#include "tempControl.h"

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
)rawliteral";


const char defaultMode2_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
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
    .drop-buttons {
      display: flex;
      justify-content: center;
      align-items: center;
      margin-top: -45px;
      gap: 150px;
      width: 100%;
    }
    .drop {
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
    .drop.red {
      background-color: red;
    }
    .drop:disabled {
      background-color: #777;
      cursor: not-allowed;
    }
    .drop:hover:not(:disabled) {
      background-color: #555;
    }
    .switchCircle {
      width: 40px;
      height: 40px;
      border-radius: 50%;
      background-color: #bbb;
      color: white;
      display: flex;
      align-items: center;
      justify-content: center;
      font-weight: bold;
      font-size: 1.2em;
      transition: background-color 0.3s ease;
    }
  </style>
  <script src="https://cdn.jsdelivr.net/npm/nipplejs@0.9.0/dist/nipplejs.min.js"></script>
</head>
<body>
  <div class="top-bar">
    <div id="gpsLocation" style="font-size: 14px; color: #aaa;">GPS: Waiting...</div>

    <div id="switchDisplay" style="display: flex; gap: 10px;">
      <div class="switchCircle" id="sw1">1</div>
      <div class="switchCircle" id="sw2">2</div>
    </div>

    <div id="tempStatus" style="font-size: 14px; color: #aaa;">Temp: Waiting...</div>

    <div style="display: flex; align-items: center; gap: 12px;">
      <div class="battery-container">
        <div class="battery-icon">
          <div class="battery-level" id="batteryLevel"></div>
        </div>
        <span id="batteryStatus" style="margin-left: 8px;">Battery: N/A</span>
      </div>
      <div id="fanStatusCircle" style="
        width: 16px;
        height: 16px;
        border-radius: 50%;
        background-color: green;
        box-shadow: 0 0 4px #0005;
        transition: background-color 0.3s;
      " title="Fan status"></div>
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
    <button class="drop red" id="drop1" onclick="sendServoCommand(1)">DROP 1</button>
    <button class="drop red" id="drop2" onclick="sendServoCommand(2)">DROP 2</button>
  </div>

  <script>
    const deadZone = 0.2;
    let estopActive = false;
    const servoStates = { drop1: false, drop2: false };

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

    function sendServoCommand(dropNum) {
      const buttonId = `drop${dropNum}`;
      const button = document.getElementById(buttonId);
      const isOpen = servoStates[buttonId];

      servoStates[buttonId] = !isOpen;

      if (servoStates[buttonId]) {
        button.style.backgroundColor = 'green';
        button.textContent = `OPEN ${dropNum}`;
        fetch(`/openServo${dropNum}`);
      } else {
        button.style.backgroundColor = 'red';
        button.textContent = `DROP ${dropNum}`;
        fetch(`/closeServo${dropNum}`);
      }
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

    function updateBatteryDisplay(percent) {
      const level = document.getElementById("batteryLevel");
      document.getElementById("batteryStatus").textContent = `Battery: ${percent}%`;
      level.style.width = `${percent}%`;
      level.style.backgroundColor = percent > 50 ? '#4caf50' : percent > 20 ? '#ff9800' : '#f44336';
    }

    function updateGPSLocation(lat, lon) {
      document.getElementById("gpsLocation").textContent = `GPS: ${lat.toFixed(6)}, ${lon.toFixed(6)}`;
    }

    function updateTempDisplay(temp) {
      document.getElementById("tempStatus").textContent = `Temp: ${temp.toFixed(1)}°C`;
    }

function updateFanStatus(relayState) {
  try {
    const data = typeof relayState === 'string' ? JSON.parse(relayState) : relayState;
    const status = data.relay;

   // console.log("Relay status:", status);
    const fanCircle = document.getElementById('fanStatusCircle');
    fanCircle.style.backgroundColor = (status === 'on') ? 'blue' : 'green';
  } catch (e) {
    console.error("Failed to update fan status:", e);
  }
}




    function updateSwitchDisplay(data) {
      const display = document.getElementById("switchStateDisplay");
      if (!display) return;
      const s1 = data.switch1 ? 1 : 0;
      const s2 = data.switch2 ? 1 : 0;
   
    }

    window.onload = () => {
      createJoystick("left_joystick", 1);
      createJoystick("right_joystick", 2);
    };

  setInterval(() => {
    // GPS request
    fetch('/gps')
        .then(res => {
            if (!res.ok) {
                throw new Error('Failed to fetch GPS data');
            }
            return res.json();
        })
        .then(data => updateGPSLocation(data.lat, data.lon))
        .catch(console.warn);

    // Temperature request
    fetch('/temp')
        .then(res => {
            if (!res.ok) {
                throw new Error('Failed to fetch temperature data');
            }
            return res.json();
        })
        .then(data => updateTempDisplay(data.temp))
        .catch(console.warn);

    // Battery request
    fetch('/battery')
        .then(res => {
            if (!res.ok) {
                throw new Error('Failed to fetch battery data');
            }
            return res.json();
        })
        .then(data => updateBatteryDisplay(data.percent))
        .catch(console.warn);

    // Relay status request
    fetch('/relayStatus')
        .then(res => {
            if (!res.ok) {
                throw new Error('Failed to fetch relay status');
            }
            return res.json();
        })
        .then(data => updateFanStatus(data))
        .catch(console.warn);

    // Switches request
//fetch('/switches').then(res => res.json()).then(data => updateSwitchDisplay(data)).catch(console.warn);
    fetchSwitchStates();

}, 3000);
    function fetchSwitchStates() {
  fetch('/getSwitchStates')
    .then(response => response.json())
    .then(data => {
      updateSwitchDisplay(data);

      // Update switch circles visually
      document.getElementById('sw1').style.backgroundColor = data.switch1 ? '#4caf50' : '#bbb';
      document.getElementById('sw2').style.backgroundColor = data.switch2 ? '#4caf50' : '#bbb';
    })
    .catch(error => {
      console.error('Error fetching switch states:', error);
    });
}
  </script>
</body>
</html>
)rawliteral";

const char gps_test_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>RC Tank Control</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background: #111;
      color: #0f0;
      text-align: center;
      padding: 20px;
    }
    #gps {
      font-size: 1.5em;
      margin-top: 40px;
    }
    button {
      padding: 10px 20px;
      font-size: 1em;
      margin-top: 20px;
      background-color: #222;
      color: #0f0;
      border: 1px solid #0f0;
      cursor: pointer;
    }
    button:hover {
      background-color: #0f0;
      color: #111;
    }
  </style>
</head>
<body>
  <h1>RC Tank Control</h1>
  <div id="gps">Fetching GPS...</div>
  <button onclick="fetchGPS()">Refresh GPS</button>

  <script>
    async function fetchGPS() {
      try {
        const res = await fetch('/gps');
        const text = await res.text();
        document.getElementById('gps').innerText = text;
      } catch (err) {
        document.getElementById('gps').innerText = 'Error fetching GPS';
      }
    }

    // Auto-refresh every 5 seconds
    setInterval(fetchGPS, 5000);
    window.onload = fetchGPS;
  </script>
</body>
</html>
)rawliteral";
