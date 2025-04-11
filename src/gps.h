#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>

extern TinyGPSPlus gps;

void initGPS();
void updateGPS();
 String getGPSData();
#endif