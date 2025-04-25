#include "tempControl.h"
#include <OneWire.h>
#include <DallasTemperature.h>

namespace tempControl {
  static OneWire* oneWire = nullptr;
  static DallasTemperature* sensors = nullptr;
  static TaskHandle_t tempTaskHandle = nullptr;
  static float currentTemp = 0.0;
  static uint8_t relayPin;
  static bool relayIsOn = false;

  const float TEMP_HIGH = 50.0;  // Turn ON relay above this
  const float TEMP_LOW  = 30.0;  // Turn OFF relay below this
  const TickType_t DELAY = pdMS_TO_TICKS(2000); // 2 seconds

  void setRelayOn() {
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW); // Active LOW
    relayIsOn = true;
  }

  void setRelayOff() {
    pinMode(relayPin, INPUT); // Float pin to disable
    relayIsOn = false;
  }

  void tempTask(void* parameter) {
    for (;;) {
      sensors->requestTemperatures();
      float temp = sensors->getTempCByIndex(0);
      if (temp != DEVICE_DISCONNECTED_C) {
        currentTemp = temp;
        if (temp >= TEMP_HIGH) {
          setRelayOn();
        } else if (temp <= TEMP_LOW) {
          setRelayOff();
        }
      } else {
        Serial.println("Error: Temperature sensor disconnected");
      }
      vTaskDelay(DELAY);
    }
  }

  void begin(uint8_t sensorPin, uint8_t rPin) {
    oneWire = new OneWire(sensorPin);
    sensors = new DallasTemperature(oneWire);
    sensors->begin();

    relayPin = rPin;
    setRelayOff();  // Safe default

    xTaskCreatePinnedToCore(
      tempTask,         // Task function
      "TempControl",    // Task name
      2048,             // Stack size
      NULL,             // Params
      1,                // Priority
      &tempTaskHandle,  // Task handle
      1                 // Run on core 1
    );
  }

  float getTemperature() {
    return currentTemp;
  }

  bool isRelayOn() {
    return relayIsOn;
  }
}
