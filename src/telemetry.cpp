#include "telemetry.h"
#include <esp_now.h>
#include <WiFi.h>
#include <MPU6050.h>
#include <TinyGPSPlus.h>
#include "pinConfig.h"
#include "gps.h"
#include "BatteryControls.h"
#include <LoRa.h>
#include "motorControl.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

extern MPU6050 mpu;
extern TinyGPSPlus gps;
extern HardwareSerial gpsSerial;

unsigned long lastTelemetryTime= 1000;



void sendTelemetry(const TelemetryPacket &packet) {
  LoRa.beginPacket();
  LoRa.write((uint8_t *)&packet, sizeof(packet));
  LoRa.endPacket();
}

void sendTelemetryPacket(const String& message) {
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
}

void setupIMU() {
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
  } else {
    Serial.println("MPU6050 ready");
  }
}


void getIMUData(TelemetryPacket &packet) {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Optionally scale for real-world units
  packet.accelX = ax / 16384.0; // ±2g scale
  packet.accelY = ay / 16384.0;
  packet.accelZ = az / 16384.0;

  packet.gyroX = gx / 131.0; // degrees/sec for ±250°/s scale
  packet.gyroY = gy / 131.0;
  packet.gyroZ = gz / 131.0;
}

// IMU-based violent shaking detection
bool imuIsShakingViolently() {
  // Fetch raw accelerometer data
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Calculate magnitude of acceleration (just a simple example)
  float accelerationMagnitude = sqrt(ax * ax + ay * ay + az * az);

  // Define a threshold value to determine what "shaking" is
  float threshold = 5000;  // Tweak this value based on experimentation
  return accelerationMagnitude > threshold;
}

// Check if the boat is stuck by comparing RPM to speed
bool isStuck(float rpm_left, float rpm_right, float gpsSpeed) {
  // If RPMs are high but GPS speed is 0, it's likely stuck
  float speedThreshold = 0.1; // Small GPS speed threshold to account for small movements
  return (rpm_left > 50 && rpm_right > 50 && gpsSpeed < speedThreshold);
}

// Send stuck alert if detected
void triggerStuckAlert() {
  String alertMessage = "Stuck detected!";
  sendTelemetryPacket(alertMessage);  // Send alert via LoRa or ESP-NOW
}


void shakeDetectionTask(void *pvParameters) {
  while (true) {
    if (imuIsShakingViolently()) {
      Serial.println("Shaking detected!");
      // Optionally trigger some action (like stopping or reversing)
      // For example:
      // sendTelemetryPacket("Shaking detected!");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Check every 1 second
  }
}


void stuckDetectionTask(void *pvParameters) {
  while (true) {
     float gpsSpeed = gps.speed.kmph();  // Use km/h or m/s as required
    if (isStuck(rpm_motor1, rpm_motor2, gpsSpeed)) {
      Serial.println("Stuck detected!");
      triggerStuckAlert(); // Send alert via LoRa or ESP-NOW
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Check every 1 second
  }
}

void telemetryTask(void *pvParameters) {
  TelemetryPacket packet;

  while (true) {
    // Update telemetry packet with GPS data
    updateTelemetryWithGPS(packet);

    // Send the telemetry packet over LoRa
    sendTelemetry(packet);

    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Send every 1 second
  }
}


void updateTelemetryWithGPS(TelemetryPacket &packet) {
  while (GPS_Serial.available()) {
    gps.encode(GPS_Serial.read());
  }

  if (gps.location.isValid()) {
    packet.latitude = gps.location.lat();
    packet.longitude = gps.location.lng();
    packet.speed = gps.speed.kmph(); // Or gps.speed.mps() for meters per second
  } else {
    packet.latitude = 0.0;
    packet.longitude = 0.0;
    packet.speed = 0.0;
  }
}


 void initTelemetry() {
  // Initialize IMU, GPS, etc.
  setupIMU();

  // Create tasks for shaking detection, stuck detection, and telemetry
  xTaskCreate(shakeDetectionTask, "Shake Detection Task", 2048, NULL, 1, NULL);
  xTaskCreate(stuckDetectionTask, "Stuck Detection Task", 2048, NULL, 1, NULL);
  xTaskCreate(telemetryTask, "Telemetry Task", 2048, NULL, 1, NULL);
}


/*/TelemetryPacket packet;
packet.latitude = gps.location.lat();
packet.longitude = gps.location.lng();
packet.speed = gps.speed.kmph();
// Fill in rpm_motor1, rpm_motor2, batteryVoltage, systemStatus

getIMUData(packet); // Add this line
sendTelemetry(packet);
*/
