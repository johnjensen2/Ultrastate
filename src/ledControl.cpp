//#include <Adafruit_NeoPixel.h>
#include "ledControl.h"

const int ledPin = 4;
const int numLEDs = 5;
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDs, ledPin, NEO_GRB + NEO_KHZ800);

void initLEDs() {
  // strip.begin();
  // strip.show(); // Initialize all pixels to 'off'
}

void setLEDColor(int index, int r, int g, int b) {
  // if (index < numLEDs) {
  //   strip.setPixelColor(index, strip.Color(r, g, b));
  //   strip.show();
  //}
}
