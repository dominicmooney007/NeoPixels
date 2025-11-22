/*
 * Pulsing Rainbow - 8x8 NeoPixel Matrix
 *
 * A collection of mesmerizing pulsing rainbow effects that combine
 * color cycling with breathing/wave animations.
 *
 * Hardware:
 * - Arduino Uno R4 WiFi (or compatible)
 * - 8x8 NeoPixel LED Matrix (WS2812B)
 * - Data pin: 6
 *
 * What you'll learn:
 * - Combining sine waves with color algorithms
 * - Creating smooth pulsing effects
 * - Working with spatial patterns (ripples, waves)
 * - Time-based animations using millis()
 */

#include <Adafruit_NeoPixel.h>

// Pin and matrix configuration
#define PIN 6
#define ROWS 8
#define COLS 8
#define NUMPIXELS (ROWS * COLS)  // 64 LEDs

// Create NeoPixel object
Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Pattern control
int currentPattern = 0;
const int NUM_PATTERNS = 5;
unsigned long patternStartTime = 0;
const unsigned long PATTERN_DURATION = 15000;  // 15 seconds per pattern

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setBrightness(30);  // Keep low for power saving
  matrix.show();

  Serial.println("Pulsing Rainbow Patterns - Starting!");
  Serial.println("Each pattern runs for 15 seconds");
  Serial.println("========================================");

  patternStartTime = millis();
}

void loop() {
  // Check if it's time to switch patterns
  if (millis() - patternStartTime >= PATTERN_DURATION) {
    currentPattern = (currentPattern + 1) % NUM_PATTERNS;
    patternStartTime = millis();

    Serial.print("\nSwitching to Pattern ");
    Serial.println(currentPattern + 1);
  }

  // Run current pattern
  switch(currentPattern) {
    case 0:
      pattern_radialPulse();
      break;
    case 1:
      pattern_wavePulse();
      break;
    case 2:
      pattern_diagonalRainbow();
      break;
    case 3:
      pattern_breathingRainbow();
      break;
    case 4:
      pattern_spiralPulse();
      break;
  }

  matrix.show();
  delay(20);  // Smooth animation at ~50 FPS
}

// ========== PATTERN 1: Radial Pulse ==========
// Rainbow ripples emanate from the center
void pattern_radialPulse() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern 1: Radial Pulse - Rainbow ripples from center");
    initialized = true;
  }

  unsigned long t = millis();
  float centerRow = 3.5;
  float centerCol = 3.5;

  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      // Calculate distance from center
      float dx = col - centerCol;
      float dy = row - centerRow;
      float distance = sqrt(dx * dx + dy * dy);

      // Create pulsing effect based on distance and time
      float wave = sin((distance * 0.8) - (t / 200.0)) * 0.5 + 0.5;

      // Map distance to rainbow color
      byte colorPos = (byte)((int)(distance * 30 + t / 20) % 256);
      uint32_t color = wheel(colorPos);

      // Apply pulsing brightness
      color = dimColor(color, wave);

      int pixel = getPixelIndex(row, col);
      matrix.setPixelColor(pixel, color);
    }
  }

  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
  }
}

// ========== PATTERN 2: Wave Pulse ==========
// Horizontal rainbow waves with vertical pulsing
void pattern_wavePulse() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern 2: Wave Pulse - Horizontal rainbow waves");
    initialized = true;
  }

  unsigned long t = millis();

  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      // Horizontal wave with time progression
      float horizontalWave = sin((col * 0.5) + (t / 150.0)) * 0.5 + 0.5;

      // Vertical wave for color variation
      float verticalWave = sin((row * 0.7) - (t / 200.0)) * 0.5 + 0.5;

      // Combine waves for color selection
      byte colorPos = (byte)((int)(horizontalWave * 128 + verticalWave * 128 + t / 30) % 256);
      uint32_t color = wheel(colorPos);

      // Pulse based on vertical position
      float pulse = sin((row * 0.5) - (t / 180.0)) * 0.4 + 0.6;
      color = dimColor(color, pulse);

      int pixel = getPixelIndex(row, col);
      matrix.setPixelColor(pixel, color);
    }
  }

  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
  }
}

// ========== PATTERN 3: Diagonal Rainbow ==========
// Rainbow stripes move diagonally with pulsing intensity
void pattern_diagonalRainbow() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern 3: Diagonal Rainbow - Animated diagonal stripes");
    initialized = true;
  }

  unsigned long t = millis();

  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      // Diagonal gradient
      int diagonal = row + col;

      // Create moving rainbow
      byte colorPos = (byte)((diagonal * 25 + t / 15) % 256);
      uint32_t color = wheel(colorPos);

      // Pulsing effect across entire matrix
      float pulse = sin(t / 300.0) * 0.3 + 0.7;

      // Add secondary pulse based on position
      float positionPulse = sin((diagonal * 0.4) - (t / 250.0)) * 0.3 + 0.7;

      color = dimColor(color, pulse * positionPulse);

      int pixel = getPixelIndex(row, col);
      matrix.setPixelColor(pixel, color);
    }
  }

  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
  }
}

// ========== PATTERN 4: Breathing Rainbow ==========
// Entire matrix breathes through rainbow spectrum
void pattern_breathingRainbow() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern 4: Breathing Rainbow - Full matrix color breathing");
    initialized = true;
  }

  unsigned long t = millis();

  // Global rainbow color that cycles slowly
  byte globalColor = (byte)((t / 25) % 256);

  // Breathing intensity
  float breathe = sin(t / 800.0) * 0.5 + 0.5;

  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      // Each pixel gets a slightly offset color based on position
      byte localColor = (byte)((globalColor + row * 8 + col * 4) % 256);
      uint32_t color = wheel(localColor);

      // Apply breathing effect
      color = dimColor(color, breathe);

      int pixel = getPixelIndex(row, col);
      matrix.setPixelColor(pixel, color);
    }
  }

  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
  }
}

// ========== PATTERN 5: Spiral Pulse ==========
// Rainbow spiral with pulsing waves
void pattern_spiralPulse() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern 5: Spiral Pulse - Rotating rainbow spiral");
    initialized = true;
  }

  unsigned long t = millis();
  float centerRow = 3.5;
  float centerCol = 3.5;

  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      // Calculate polar coordinates
      float dx = col - centerCol;
      float dy = row - centerRow;
      float distance = sqrt(dx * dx + dy * dy);
      float angle = atan2(dy, dx);

      // Create spiral pattern
      float spiral = angle * 40 + distance * 20 - (t / 30.0);

      // Map to rainbow color
      byte colorPos = (byte)((int)(spiral) % 256);
      uint32_t color = wheel(colorPos);

      // Pulsing based on distance
      float pulse = sin((distance * 0.6) - (t / 200.0)) * 0.4 + 0.6;
      color = dimColor(color, pulse);

      int pixel = getPixelIndex(row, col);
      matrix.setPixelColor(pixel, color);
    }
  }

  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
  }
}

// ========== HELPER FUNCTIONS ==========

// Get pixel index from row and column (zigzag wiring pattern)
int getPixelIndex(int row, int col) {
  // Even rows: left-to-right, Odd rows: right-to-left
  if (row % 2 == 0) {
    return row * COLS + col;
  } else {
    return row * COLS + (COLS - 1 - col);
  }
}

// Generate rainbow colors (input 0-255)
uint32_t wheel(byte wheelPos) {
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85) {
    return matrix.Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if (wheelPos < 170) {
    wheelPos -= 85;
    return matrix.Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return matrix.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// Dim a color by a factor (0.0 = off, 1.0 = full brightness)
uint32_t dimColor(uint32_t color, float factor) {
  // Clamp factor between 0 and 1
  if (factor < 0.0) factor = 0.0;
  if (factor > 1.0) factor = 1.0;

  // Extract RGB components
  byte r = (color >> 16) & 0xFF;
  byte g = (color >> 8) & 0xFF;
  byte b = color & 0xFF;

  // Apply dimming
  r = (byte)(r * factor);
  g = (byte)(g * factor);
  b = (byte)(b * factor);

  return matrix.Color(r, g, b);
}

/*
 * EXPERIMENTS TO TRY:
 *
 * 1. Speed Control:
 *    Add potentiometer to adjust animation speed in real-time
 *
 * 2. Pattern Selection:
 *    Use Serial input to manually select patterns (send 0-4)
 *
 * 3. Custom Patterns:
 *    Create your own pattern combining different wave functions
 *
 * 4. Music Sync:
 *    Add microphone to pulse with music beats
 *
 * 5. Brightness Adjustment:
 *    Increase setBrightness() value (requires external power)
 *
 * 6. Combine Patterns:
 *    Mix two patterns together for complex effects
 */
