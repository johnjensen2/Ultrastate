#include "imu.h"
#include <Wire.h>
#include <MPU6050.h>
#include "calibrationStorage.h"
#include "pinConfig.h"
#include "BatteryControls.h"
#include "gps.h"
#include "telemetry.h"


extern MPU6050 mpu;


void calibrateIMU() {
  Serial.println("Starting IMU calibration...");
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    return;
  }

  delay(3000);  // Wait for user to hold still

  long ax = 0, ay = 0, az = 0;
  long gx = 0, gy = 0, gz = 0;

  const int samples = 1000;
  for (int i = 0; i < samples; i++) {
    int16_t x, y, z;

    mpu.getAcceleration(&x, &y, &z);
    ax += x;
    ay += y;
    az += z;

    mpu.getRotation(&x, &y, &z);
    gx += x;
    gy += y;
    gz += z;

    delay(2);
  }

  IMUOffsets offsets;
  offsets.ax_offset = -(ax / samples);
  offsets.ay_offset = -(ay / samples);
  offsets.az_offset = (16384 - (az / samples)); // gravity compensation

  offsets.gx_offset = -(gx / samples);
  offsets.gy_offset = -(gy / samples);
  offsets.gz_offset = -(gz / samples);

  saveIMUOffsets(offsets);
  Serial.println("IMU calibration complete and offsets saved!");
}


void applyIMUOffsets() {
  IMUOffsets offsets;
  loadIMUOffsets(offsets);

  mpu.setXAccelOffset(offsets.ax_offset);
  mpu.setYAccelOffset(offsets.ay_offset);
  mpu.setZAccelOffset(offsets.az_offset);
  mpu.setXGyroOffset(offsets.gx_offset);
  mpu.setYGyroOffset(offsets.gy_offset);
  mpu.setZGyroOffset(offsets.gz_offset);

  Serial.println("IMU offsets applied.");
}