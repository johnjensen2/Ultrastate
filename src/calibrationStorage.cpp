// calibrationStorage.cpp
#include "calibrationStorage.h"

#define EEPROM_SIZE 64
#define OFFSET_ADDR 0

void saveIMUOffsets(const IMUOffsets& offsets) {
  EEPROM.begin(EEPROM_SIZE);

  EEPROM.put(OFFSET_ADDR, offsets);
  EEPROM.commit();
  EEPROM.end();
}

void loadIMUOffsets(IMUOffsets& offsets) {
  EEPROM.begin(EEPROM_SIZE);

  EEPROM.get(OFFSET_ADDR, offsets);
  EEPROM.end();
} 
