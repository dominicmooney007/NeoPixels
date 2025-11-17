/*
 * Example 5: Fade In and Out
 *
 * All LEDs fade smoothly from off to bright and back.
 * Beautiful breathing effect!
 *
 * What you'll learn:
 * - How to create smooth animations
 * - How brightness values work
 * - Creating fade effects
 */

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.setBrightness(50);

  Serial.println("Fade In/Out Example");
  Serial.println("Watch the LEDs breathe!");
}

void loop() {
  Serial.println("Fading IN...");

  // Fade in: brightness goes from 0 to 255
  for(int brightness = 0; brightness <= 255; brightness += 5) {

    // Set all LEDs to blue with this brightness
    for(int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, brightness));
    }

    pixels.show();
    delay(20);  // Small delay makes the fade smooth
  }

  Serial.println("Fading OUT...");

  // Fade out: brightness goes from 255 to 0
  for(int brightness = 255; brightness >= 0; brightness -= 5) {

    // Set all LEDs to blue with this brightness
    for(int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, brightness));
    }

    pixels.show();
    delay(20);
  }

  Serial.println("--- Breathing cycle complete ---\n");

  // Small pause before next cycle
  delay(100);
}

/*
 * EXPERIMENTS TO TRY:
 *
 * 1. Change the color:
 *    pixels.Color(brightness, 0, 0) for red fade
 *    pixels.Color(brightness, brightness, 0) for yellow fade
 *
 * 2. Change fade speed:
 *    brightness += 1  (slower, smoother)
 *    brightness += 10 (faster, choppier)
 *
 * 3. Fade different colors:
 *    Start with red, fade to blue
 *
 * 4. Change timing:
 *    Adjust the delay(20) value
 */
