#ifndef CALIBRATION_STORAGE_H
#define CALIBRATION_STORAGE_H

#include <EEPROM.h>
#include <Arduino.h>

struct IMUOffsets {
  int16_t ax_offset;
  int16_t ay_offset;
  int16_t az_offset;
  int16_t gx_offset;
  int16_t gy_offset;
  int16_t gz_offset;
};

void saveIMUOffsets(const IMUOffsets& offsets);
void loadIMUOffsets(IMUOffsets& offsets);

#endif