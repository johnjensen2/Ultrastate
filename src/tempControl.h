#ifndef TEMP_CONTROL_H
#define TEMP_CONTROL_H

#include <Arduino.h>

void initTempSensor(uint8_t pin, uint8_t relayPin);
void updateTempControl();
void setRelayOff();
void setRelayOn();


namespace tempControl {
  void begin(uint8_t sensorPin, uint8_t relayPin);
  float getTemperature();
  bool isRelayOn();
  extern bool relayIsOn;
  }

#endif


