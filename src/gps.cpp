// gps.cpp
#include "gps.h"
#include <HardwareSerial.h>

TinyGPSPlus gps;
HardwareSerial GPS_Serial(1);

void initGPS() {
  GPS_Serial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
}

void updateGPS() {
  while (GPS_Serial.available() > 0) {
    gps.encode(GPS_Serial.read());
  }
}
