// gps.cpp
#include "gps.h"
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include <Arduino.h>

extern TinyGPSPlus gps;
extern HardwareSerial GPS_Serial;

void initGPS() {
  GPS_Serial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
}

void updateGPS() {
  while (GPS_Serial.available() > 0) {
    gps.encode(GPS_Serial.read());
  }
}

String getGPSData() {
  while (GPS_Serial.available()) {
    gps.encode(GPS_Serial.read());
  }

  if (gps.location.isValid()) {
    return "GPS:LAT=" + String(gps.location.lat(), 6) +
           " LNG=" + String(gps.location.lng(), 6);
  }
  return "GPS:No Fix";
}