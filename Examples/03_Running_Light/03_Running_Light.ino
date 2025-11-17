/*
 * Example 3: Running Light (LED Chaser)
 *
 * A single LED moves along your strip/ring.
 * Classic animation effect!
 *
 * What you'll learn:
 * - How to use loops with NeoPixels
 * - How to create animations
 * - How to clear all pixels
 */

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.setBrightness(50);

  Serial.println("Running Light Example");
  Serial.println("Watch a red LED chase along the strip!");
}

void loop() {
  // Move through each LED position
  for(int i = 0; i < NUMPIXELS; i++) {

    Serial.print("LED ");
    Serial.print(i);
    Serial.println(" is ON");

    // Turn off all LEDs
    pixels.clear();

    // Turn on just this one LED
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));  // Red

    // Update the display
    pixels.show();

    // Wait a bit before moving to next LED
    delay(100);

    // Try changing the delay value:
    // - Smaller number = faster movement
    // - Larger number = slower movement
  }

  Serial.println("--- Restarting from beginning ---\n");

  // The loop repeats, so the light keeps chasing!
}

/*
 * EXPERIMENTS TO TRY:
 *
 * 1. Change the color:
 *    pixels.setPixelColor(i, pixels.Color(0, 255, 0));  // Try green!
 *
 * 2. Make it go backwards:
 *    for(int i = NUMPIXELS - 1; i >= 0; i--)
 *
 * 3. Add a trail effect:
 *    Don't use pixels.clear() - just dim the previous LED
 */
