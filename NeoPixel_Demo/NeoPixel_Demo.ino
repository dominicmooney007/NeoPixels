/*
 * NeoPixel 8x8 LED Matrix Demo
 * Demonstrates various capabilities of the Adafruit NeoPixel library
 *
 * Hardware:
 * - Arduino Uno R4 WiFi
 * - 8x8 NeoPixel LED Matrix (WS2812B)
 * - 5V 2A Power Supply
 *
 * Wiring:
 * - Matrix DIN  -> Arduino Pin 6
 * - Matrix +5V  -> 5V Power Supply +
 * - Matrix GND  -> Common Ground (Arduino GND + Power Supply -)
 */

#include <Adafruit_NeoPixel.h>

// Pin and matrix configuration
#define PIN 6
#define ROWS 8
#define COLS 8
#define NUMPIXELS (ROWS * COLS)  // 64 LEDs

// Create NeoPixel object
Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Global variables
int currentDemo = 0;
const int NUM_DEMOS = 8;

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setBrightness(30);  // Keep brightness low (0-255) to save power
  matrix.show();  // Initialize all pixels to 'off'

  Serial.println("NeoPixel 8x8 Matrix Demo Starting...");
}

void loop() {
  Serial.print("Running Demo ");
  Serial.print(currentDemo + 1);
  Serial.print(" of ");
  Serial.println(NUM_DEMOS);

  switch(currentDemo) {
    case 0:
      demo_colorFill();
      break;
    case 1:
      demo_rainbow();
      break;
    case 2:
      demo_theater();
      break;
    case 3:
      demo_rowByRow();
      break;
    case 4:
      demo_spiral();
      break;
    case 5:
      demo_randomSparkle();
      break;
    case 6:
      demo_breathe();
      break;
    case 7:
      demo_checkerboard();
      break;
  }

  // Move to next demo
  currentDemo = (currentDemo + 1) % NUM_DEMOS;
  delay(1000);  // Pause between demos
}

// ========== DEMO FUNCTIONS ==========

// Demo 1: Fill entire matrix with different colors
void demo_colorFill() {
  Serial.println("  - Color Fill Demo");
  uint32_t colors[] = {
    matrix.Color(255, 0, 0),    // Red
    matrix.Color(0, 255, 0),    // Green
    matrix.Color(0, 0, 255),    // Blue
    matrix.Color(255, 255, 0),  // Yellow
    matrix.Color(255, 0, 255),  // Magenta
    matrix.Color(0, 255, 255),  // Cyan
    matrix.Color(255, 255, 255) // White
  };

  for(int c = 0; c < 7; c++) {
    for(int i = 0; i < NUMPIXELS; i++) {
      matrix.setPixelColor(i, colors[c]);
    }
    matrix.show();
    delay(500);
  }
  clearMatrix();
}

// Demo 2: Rainbow wave effect
void demo_rainbow() {
  Serial.println("  - Rainbow Wave Demo");
  for(int j = 0; j < 256; j++) {
    for(int i = 0; i < NUMPIXELS; i++) {
      matrix.setPixelColor(i, wheel((i * 256 / NUMPIXELS + j) & 255));
    }
    matrix.show();
    delay(20);
  }
  clearMatrix();
}

// Demo 3: Theater chase effect
void demo_theater() {
  Serial.println("  - Theater Chase Demo");
  for(int j = 0; j < 3; j++) {  // 3 cycles
    for(int q = 0; q < 3; q++) {
      for(int i = 0; i < NUMPIXELS; i += 3) {
        matrix.setPixelColor(i + q, matrix.Color(127, 0, 0));  // Red
      }
      matrix.show();
      delay(100);

      for(int i = 0; i < NUMPIXELS; i += 3) {
        matrix.setPixelColor(i + q, 0);  // Turn off
      }
    }
  }
  clearMatrix();
}

// Demo 4: Light up row by row
void demo_rowByRow() {
  Serial.println("  - Row by Row Demo");
  for(int row = 0; row < ROWS; row++) {
    for(int col = 0; col < COLS; col++) {
      int pixel = getPixelIndex(row, col);
      matrix.setPixelColor(pixel, matrix.Color(0, 100, 255));
    }
    matrix.show();
    delay(200);
  }
  delay(500);

  // Turn off row by row
  for(int row = 0; row < ROWS; row++) {
    for(int col = 0; col < COLS; col++) {
      int pixel = getPixelIndex(row, col);
      matrix.setPixelColor(pixel, 0);
    }
    matrix.show();
    delay(200);
  }
}

// Demo 5: Spiral pattern from center
void demo_spiral() {
  Serial.println("  - Spiral Demo");
  clearMatrix();

  // Simple spiral sequence (clockwise from outside)
  int sequence[] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    15, 23, 31, 39, 47, 55, 63, 62,
    61, 60, 59, 58, 57, 56, 48, 40,
    32, 24, 16, 8, 9, 10, 11, 12,
    13, 14, 22, 30, 38, 46, 54, 53,
    52, 51, 50, 49, 41, 33, 25, 17,
    18, 19, 20, 21, 29, 37, 45, 44,
    43, 42, 34, 26, 27, 28, 36, 35
  };

  for(int i = 0; i < NUMPIXELS; i++) {
    matrix.setPixelColor(sequence[i], wheel(i * 4));
    matrix.show();
    delay(30);
  }
  delay(1000);
  clearMatrix();
}

// Demo 6: Random sparkle effect
void demo_randomSparkle() {
  Serial.println("  - Random Sparkle Demo");
  for(int i = 0; i < 200; i++) {  // 200 sparkles
    int pixel = random(NUMPIXELS);
    int r = random(256);
    int g = random(256);
    int b = random(256);

    matrix.setPixelColor(pixel, matrix.Color(r, g, b));
    matrix.show();
    delay(30);
    matrix.setPixelColor(pixel, 0);  // Turn off
  }
  clearMatrix();
}

// Demo 7: Breathing effect
void demo_breathe() {
  Serial.println("  - Breathing Demo");

  for(int cycle = 0; cycle < 3; cycle++) {
    // Fade in
    for(int brightness = 0; brightness <= 255; brightness += 5) {
      for(int i = 0; i < NUMPIXELS; i++) {
        matrix.setPixelColor(i, matrix.Color(0, brightness / 4, brightness / 2));
      }
      matrix.show();
      delay(20);
    }

    // Fade out
    for(int brightness = 255; brightness >= 0; brightness -= 5) {
      for(int i = 0; i < NUMPIXELS; i++) {
        matrix.setPixelColor(i, matrix.Color(0, brightness / 4, brightness / 2));
      }
      matrix.show();
      delay(20);
    }
  }
  clearMatrix();
}

// Demo 8: Animated checkerboard pattern
void demo_checkerboard() {
  Serial.println("  - Checkerboard Demo");

  for(int frame = 0; frame < 10; frame++) {
    for(int row = 0; row < ROWS; row++) {
      for(int col = 0; col < COLS; col++) {
        int pixel = getPixelIndex(row, col);

        // Alternate pattern based on row, col, and frame
        if((row + col + frame) % 2 == 0) {
          matrix.setPixelColor(pixel, matrix.Color(255, 0, 128));
        } else {
          matrix.setPixelColor(pixel, matrix.Color(0, 255, 128));
        }
      }
    }
    matrix.show();
    delay(300);
  }
  clearMatrix();
}

// ========== HELPER FUNCTIONS ==========

// Get pixel index from row and column (assumes zigzag wiring pattern)
// Adjust this based on your specific matrix wiring
int getPixelIndex(int row, int col) {
  // Standard zigzag pattern: even rows go left-to-right, odd rows go right-to-left
  if(row % 2 == 0) {
    return row * COLS + col;
  } else {
    return row * COLS + (COLS - 1 - col);
  }
}

// Rainbow color wheel function (0-255)
uint32_t wheel(byte wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return matrix.Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return matrix.Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return matrix.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// Clear all LEDs
void clearMatrix() {
  for(int i = 0; i < NUMPIXELS; i++) {
    matrix.setPixelColor(i, 0);
  }
  matrix.show();
}
