/*
 * Example 2: All Colors
 *
 * Lights all your LEDs with different colors.
 * This helps you understand RGB color mixing.
 *
 * What you'll learn:
 * - How to create different colors
 * - How to control multiple LEDs
 * - RGB color values
 */

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.setBrightness(50);

  Serial.println("All Colors Example");
  Serial.println("Showing different colors on each LED");
  Serial.println();
  Serial.println("Color format: Color(Red, Green, Blue)");
  Serial.println("Each value is 0-255");
  Serial.println();
}

void loop() {
  // Light up each LED with a different color
  // Notice how mixing red, green, and blue creates different colors!

  Serial.println("Pixel 0: RED (255, 0, 0)");
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));    // Red

  Serial.println("Pixel 1: GREEN (0, 255, 0)");
  pixels.setPixelColor(1, pixels.Color(0, 255, 0));    // Green

  Serial.println("Pixel 2: BLUE (0, 0, 255)");
  pixels.setPixelColor(2, pixels.Color(0, 0, 255));    // Blue

  Serial.println("Pixel 3: YELLOW (255, 255, 0) - Red + Green");
  pixels.setPixelColor(3, pixels.Color(255, 255, 0));  // Yellow

  Serial.println("Pixel 4: CYAN (0, 255, 255) - Green + Blue");
  pixels.setPixelColor(4, pixels.Color(0, 255, 255));  // Cyan

  Serial.println("Pixel 5: MAGENTA (255, 0, 255) - Red + Blue");
  pixels.setPixelColor(5, pixels.Color(255, 0, 255));  // Magenta

  Serial.println("Pixel 6: WHITE (255, 255, 255) - All colors");
  pixels.setPixelColor(6, pixels.Color(255, 255, 255));// White

  Serial.println("Pixel 7: ORANGE (255, 128, 0) - Mostly red, some green");
  pixels.setPixelColor(7, pixels.Color(255, 128, 0));  // Orange

  // Update all LEDs at once
  pixels.show();

  Serial.println("\nAll colors displayed! Waiting 5 seconds...\n");

  // Keep the colors on for 5 seconds
  delay(5000);

  // Try changing the delay or the colors above!
}
