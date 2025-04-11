#include <FastLED.h>
#include "ledControl.h"
#include "pinConfig.h"

#define NUM_LEDS wsLedCount

CRGB leds[NUM_LEDS];

void setupLEDs() {
  FastLED.addLeds<wsLedType, wsLedPin, wsLedColorOrder>(leds, NUM_LEDS);
  FastLED.setBrightness(wsLedBrightness);

  // Initial test pattern (wipe red across strip)
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(50);
  }
}

void updateLEDs() {
  // You can expand this with different effects or modes later
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue + (i * 10), 255, wsLedBrightness);
  }
  FastLED.show();
  hue++;
  delay(1000 / wsLedSpeed);
}
