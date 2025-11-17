/*
 * Example 4: Rainbow
 *
 * Creates a beautiful rainbow pattern across all LEDs.
 *
 * What you'll learn:
 * - How to create a rainbow
 * - Working with arrays
 * - Creating static displays
 */

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.setBrightness(50);

  Serial.println("Rainbow Example");
  Serial.println("Creating a rainbow across all LEDs");
}

void loop() {
  // Create rainbow colors
  // Each LED gets a different color of the rainbow

  if(NUMPIXELS >= 1) pixels.setPixelColor(0, pixels.Color(255, 0, 0));    // Red
  if(NUMPIXELS >= 2) pixels.setPixelColor(1, pixels.Color(255, 128, 0));  // Orange
  if(NUMPIXELS >= 3) pixels.setPixelColor(2, pixels.Color(255, 255, 0));  // Yellow
  if(NUMPIXELS >= 4) pixels.setPixelColor(3, pixels.Color(0, 255, 0));    // Green
  if(NUMPIXELS >= 5) pixels.setPixelColor(4, pixels.Color(0, 255, 255));  // Cyan
  if(NUMPIXELS >= 6) pixels.setPixelColor(5, pixels.Color(0, 0, 255));    // Blue
  if(NUMPIXELS >= 7) pixels.setPixelColor(6, pixels.Color(128, 0, 255));  // Purple
  if(NUMPIXELS >= 8) pixels.setPixelColor(7, pixels.Color(255, 0, 255));  // Magenta

  // Show the rainbow
  pixels.show();

  Serial.println("Rainbow displayed!");

  // Keep the rainbow on
  delay(5000);
}

/*
 * EXPERIMENTS TO TRY:
 *
 * 1. Rotate the rainbow:
 *    Store colors in an array and shift them each frame
 *
 * 2. Change colors over time:
 *    Use loops to gradually change from one color to another
 *
 * 3. Add more colors:
 *    Create smoother transitions between rainbow colors
 */
