/*
 * Example 1: First Light
 *
 * This is your first NeoPixel program!
 * It makes the first LED blink red.
 *
 * What you'll learn:
 * - How to initialize NeoPixels
 * - How to set a pixel color
 * - How to turn pixels on and off
 */

#include <Adafruit_NeoPixel.h>

// Configuration - CHANGE THESE to match your setup
#define PIN 6           // Which Arduino pin is connected to NeoPixels
#define NUMPIXELS 8     // How many NeoPixels you have

// Create the NeoPixel object
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Start Serial Monitor so we can see what's happening
  Serial.begin(9600);
  Serial.println("First Light Example Starting...");

  // Initialize the NeoPixel library
  pixels.begin();

  // Set brightness (0-255). Start low to save power!
  pixels.setBrightness(50);

  // Turn all pixels off to start
  pixels.show();

  Serial.println("Setup complete! Watch the first LED blink red.");
}

void loop() {
  Serial.println("LED ON - Red");

  // Turn on the first LED (pixel 0) to red
  // Color format: (red, green, blue) each 0-255
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));

  // IMPORTANT: Nothing happens until you call show()!
  pixels.show();

  // Wait 1 second (1000 milliseconds)
  delay(1000);

  Serial.println("LED OFF");

  // Turn off the LED (set all colors to 0)
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();

  // Wait 1 second
  delay(1000);

  // The loop() repeats forever, so the LED will keep blinking
}
