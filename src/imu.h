#pragma once
#include <Wire.h>
#include <MPU6050.h>
#include "calibrationStorage.h"  // This ensures IMUOffsets is known
extern MPU6050 mpu;

String getIMUData();
void setupIMU();
void calibrateIMU();
void applyIMUOffsets();


void saveIMUOffsets(const IMUOffsets& offsets);
void loadIMUOffsets(IMUOffsets& offsets);
