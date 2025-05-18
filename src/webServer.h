#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

extern AsyncWebServer server;  // Declare it so other files can use it
extern DNSServer dnsServer;
void initWebServer();
extern const char index_html[]; 
extern const char defaultMode_html[]; // Declare it so other files know it exists
extern const char defaultMode2_html[];
extern const char gps_test_html[]; // Declare it so other files know it exists
extern const char ota_html[] ;


// Call this in setup()
void setupServos();

// Call this to control the steering servo (-90 to 90 degrees)
void setSteeringAngle(int angle);

// Optional: If you're also using drop servos
void openServo1();
void closeServo1();
void openServo2();
void closeServo2();
#endif
