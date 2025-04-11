#include "fullRuntimeMode.h"
#include <esp_now.h>
#include <WiFi.h>
#include "telemetry.h"
#include "gps.h"
#include "motorControl.h"
#include "pinConfig.h"
#include "wifiManager.h"

typedef struct __attribute__((packed)) {
  int8_t throttleLeft;
  bool directionLeft;
  int8_t throttleRight;
  bool directionRight;
  uint8_t button1;
  uint8_t button2;
  uint8_t controlMode;
} ControlPacket;


ControlPacket receivedControls;
TelemetryPacket telemetryData;

static unsigned long lastTelemetryTime = 0;
//==========
// mock data

uint8_t throttleLeft = 128;
uint8_t throttleRight = 128;
bool directionLeft = true;
bool directionRight = true;

bool button1 = false;
bool button2 = false;
uint8_t controlMode = 0;


void sendTelemetry() {
  String telemetry = "GPS:";

  if (gps.location.isValid()) {
    telemetry += String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  } else {
    telemetry += "INVALID";
  }

  telemetry += "; RPM1:" + String(rpm_motor1);
  telemetry += "; RPM2:" + String(rpm_motor2);

  sendTelemetryPacket(telemetry);
}
void onDataReceived(const uint8_t* mac, const uint8_t* data, int len) {
  if (len < 6) {
    Serial.println("Invalid packet received");
    return;
  }

  throttleLeft = data[0];
  directionLeft = data[1];
  throttleRight = data[2];  
  directionRight = data[3];
  button1 = data[4];
  button2 = data[5];
  controlMode = data[6];

  controlMotor(1, throttleLeft, directionLeft);
  controlMotor(2, throttleRight, directionRight);

  Serial.printf("Received: ThrottleLeft=%d, ThrottleRight=%d, B1=%d, B2=%d, Mode=%d\n",
    throttleLeft,throttleRight, button1, button2, controlMode);
}

void fullRuntimeSetup() {
  Serial.println("Full Runtime Mode Setup");
  connectToEspNow();
  esp_now_register_recv_cb(onDataReceived);
}

void fullRuntimeLoop() {
  // Example action
  if (button1) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  // More control code here...

unsigned long currentTime = millis();

if (currentTime - lastTelemetryTime >= sendTelemetryInterval) {
  sendTelemetry();
  lastTelemetryTime = currentTime;
}

}