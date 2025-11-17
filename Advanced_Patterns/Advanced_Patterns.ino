/*
 * Advanced NeoPixel Patterns for 8x8 LED Matrix
 * Entertaining visual effects and algorithms
 *
 * Hardware:
 * - Arduino Uno R4 WiFi (or compatible)
 * - 8x8 NeoPixel LED Matrix (WS2812B)
 * - 5V Power Supply
 *
 * Wiring:
 * - Matrix DIN  -> Arduino Pin 6
 * - Matrix +5V  -> 5V Power Supply +
 * - Matrix GND  -> Common Ground (Arduino GND + Power Supply -)
 *
 * Featured Patterns:
 * 1. Conway's Game of Life
 * 2. Fire Effect
 * 3. Bouncing Balls with Physics
 * 4. Matrix Digital Rain
 * 5. Plasma Wave
 * 6. Ripple Effect
 * 7. Starfield
 * 8. DNA Helix
 */

#include <Adafruit_NeoPixel.h>

// Pin and matrix configuration
#define PIN 6
#define ROWS 8
#define COLS 8
#define NUMPIXELS (ROWS * COLS)

// Create NeoPixel object
Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Global variables
int currentPattern = 0;
const int NUM_PATTERNS = 8;
unsigned long patternStartTime = 0;
const unsigned long PATTERN_DURATION = 20000;  // 20 seconds per pattern

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setBrightness(40);
  matrix.show();

  randomSeed(analogRead(0));

  Serial.println("========================================");
  Serial.println("  Advanced NeoPixel Pattern Display");
  Serial.println("========================================");
  Serial.println("Each pattern runs for 20 seconds");
  Serial.println("========================================\n");
}

void loop() {
  // Switch patterns every 20 seconds
  if (millis() - patternStartTime >= PATTERN_DURATION) {
    currentPattern = (currentPattern + 1) % NUM_PATTERNS;
    patternStartTime = millis();
    clearMatrix();

    Serial.print("\n--- Switching to Pattern ");
    Serial.print(currentPattern + 1);
    Serial.print(" of ");
    Serial.print(NUM_PATTERNS);
    Serial.println(" ---");
  }

  switch(currentPattern) {
    case 0:
      pattern_GameOfLife();
      break;
    case 1:
      pattern_Fire();
      break;
    case 2:
      pattern_BouncingBalls();
      break;
    case 3:
      pattern_MatrixRain();
      break;
    case 4:
      pattern_Plasma();
      break;
    case 5:
      pattern_Ripple();
      break;
    case 6:
      pattern_Starfield();
      break;
    case 7:
      pattern_DNAHelix();
      break;
  }
}

// ========== PATTERN 1: CONWAY'S GAME OF LIFE ==========

byte grid[ROWS][COLS];
byte newGrid[ROWS][COLS];
unsigned long lastLifeUpdate = 0;
int lifeGeneration = 0;

void pattern_GameOfLife() {
  if (lifeGeneration == 0) {
    Serial.println("Pattern: Conway's Game of Life");
    // Initialize with random pattern
    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < COLS; c++) {
        grid[r][c] = random(2);
      }
    }
    lifeGeneration = 1;
  }

  if (millis() - lastLifeUpdate >= 200) {
    lastLifeUpdate = millis();

    // Calculate next generation
    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < COLS; c++) {
        int neighbors = countNeighbors(r, c);

        // Conway's rules
        if (grid[r][c] == 1) {
          newGrid[r][c] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
        } else {
          newGrid[r][c] = (neighbors == 3) ? 1 : 0;
        }
      }
    }

    // Copy new grid to current and draw
    int aliveCount = 0;
    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < COLS; c++) {
        grid[r][c] = newGrid[r][c];
        int pixel = getPixelIndex(r, c);
        if (grid[r][c] == 1) {
          aliveCount++;
          // Color based on neighbors for visual interest
          int neighbors = countNeighbors(r, c);
          matrix.setPixelColor(pixel, getLifeColor(neighbors));
        } else {
          matrix.setPixelColor(pixel, matrix.Color(0, 0, 0));
        }
      }
    }

    matrix.show();
    lifeGeneration++;

    // Reset if population dies or stabilizes
    if (aliveCount < 3 || lifeGeneration > 100) {
      lifeGeneration = 0;
      delay(1000);
    }
  }
}

int countNeighbors(int row, int col) {
  int count = 0;
  for (int dr = -1; dr <= 1; dr++) {
    for (int dc = -1; dc <= 1; dc++) {
      if (dr == 0 && dc == 0) continue;
      int r = (row + dr + ROWS) % ROWS;  // Wrap around
      int c = (col + dc + COLS) % COLS;
      count += grid[r][c];
    }
  }
  return count;
}

uint32_t getLifeColor(int neighbors) {
  switch(neighbors) {
    case 2: return matrix.Color(0, 100, 255);   // Blue
    case 3: return matrix.Color(0, 255, 100);   // Green
    case 4: return matrix.Color(255, 200, 0);   // Yellow
    default: return matrix.Color(100, 0, 255);  // Purple
  }
}

// ========== PATTERN 2: FIRE EFFECT ==========

byte heat[ROWS][COLS];
unsigned long lastFireUpdate = 0;

void pattern_Fire() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern: Fire Effect");
    initialized = true;
  }

  if (millis() - lastFireUpdate >= 50) {
    lastFireUpdate = millis();

    // Cool down every cell a little
    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < COLS; c++) {
        heat[r][c] = max(0, heat[r][c] - random(0, 10));
      }
    }

    // Heat from the bottom
    for (int c = 0; c < COLS; c++) {
      if (random(100) < 80) {
        heat[ROWS - 1][c] = random(200, 255);
      }
    }

    // Rise and diffuse heat
    for (int r = 0; r < ROWS - 1; r++) {
      for (int c = 0; c < COLS; c++) {
        int heatSum = heat[r + 1][c];
        int count = 1;

        if (c > 0) { heatSum += heat[r + 1][c - 1]; count++; }
        if (c < COLS - 1) { heatSum += heat[r + 1][c + 1]; count++; }

        heat[r][c] = heatSum / count;
      }
    }

    // Draw fire
    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < COLS; c++) {
        int pixel = getPixelIndex(r, c);
        matrix.setPixelColor(pixel, getFireColor(heat[r][c]));
      }
    }

    matrix.show();
  }

  // Reset initialization flag when pattern changes
  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
  }
}

uint32_t getFireColor(byte temperature) {
  // Black -> Red -> Orange -> Yellow -> White
  if (temperature < 64) {
    return matrix.Color(temperature * 4, 0, 0);
  } else if (temperature < 128) {
    return matrix.Color(255, (temperature - 64) * 4, 0);
  } else if (temperature < 192) {
    return matrix.Color(255, 255, (temperature - 128) * 4);
  } else {
    return matrix.Color(255, 255, 255);
  }
}

// ========== PATTERN 3: BOUNCING BALLS ==========

struct Ball {
  float x, y;
  float vx, vy;
  uint32_t color;
};

Ball balls[4];
unsigned long lastBallUpdate = 0;
const float GRAVITY = 0.15;
const float DAMPING = 0.85;

void pattern_BouncingBalls() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern: Bouncing Balls with Physics");
    for (int i = 0; i < 4; i++) {
      balls[i].x = random(10, 70) / 10.0;
      balls[i].y = random(10, 70) / 10.0;
      balls[i].vx = random(-20, 20) / 10.0;
      balls[i].vy = random(-20, 20) / 10.0;
      balls[i].color = matrix.Color(random(100, 255), random(100, 255), random(100, 255));
    }
    initialized = true;
  }

  if (millis() - lastBallUpdate >= 30) {
    lastBallUpdate = millis();
    clearMatrix();

    for (int i = 0; i < 4; i++) {
      // Apply gravity
      balls[i].vy += GRAVITY;

      // Update position
      balls[i].x += balls[i].vx * 0.1;
      balls[i].y += balls[i].vy * 0.1;

      // Bounce off walls
      if (balls[i].x <= 0 || balls[i].x >= COLS - 1) {
        balls[i].vx = -balls[i].vx * DAMPING;
        balls[i].x = constrain(balls[i].x, 0, COLS - 1);
      }

      if (balls[i].y <= 0 || balls[i].y >= ROWS - 1) {
        balls[i].vy = -balls[i].vy * DAMPING;
        balls[i].y = constrain(balls[i].y, 0, ROWS - 1);
      }

      // Draw ball with trail
      int x = (int)balls[i].x;
      int y = (int)balls[i].y;
      int pixel = getPixelIndex(y, x);
      matrix.setPixelColor(pixel, balls[i].color);
    }

    matrix.show();
  }

  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
  }
}

// ========== PATTERN 4: MATRIX DIGITAL RAIN ==========

byte drops[COLS];
byte dropColors[COLS];
unsigned long lastRainUpdate = 0;

void pattern_MatrixRain() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern: Matrix Digital Rain");
    for (int c = 0; c < COLS; c++) {
      drops[c] = random(ROWS);
      dropColors[c] = random(150, 255);
    }
    initialized = true;
  }

  if (millis() - lastRainUpdate >= 80) {
    lastRainUpdate = millis();

    // Fade all pixels
    for (int i = 0; i < NUMPIXELS; i++) {
      uint32_t color = matrix.getPixelColor(i);
      uint8_t r = (color >> 16) & 0xFF;
      uint8_t g = (color >> 8) & 0xFF;
      uint8_t b = color & 0xFF;

      r = r > 10 ? r - 10 : 0;
      g = g > 10 ? g - 10 : 0;
      b = b > 10 ? b - 10 : 0;

      matrix.setPixelColor(i, matrix.Color(r, g, b));
    }

    // Update drops
    for (int c = 0; c < COLS; c++) {
      int pixel = getPixelIndex(drops[c], c);
      matrix.setPixelColor(pixel, matrix.Color(0, dropColors[c], 0));

      drops[c]++;
      if (drops[c] >= ROWS || random(100) < 10) {
        drops[c] = 0;
        dropColors[c] = random(150, 255);
      }
    }

    matrix.show();
  }

  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
  }
}

// ========== PATTERN 5: PLASMA WAVE ==========

unsigned long plasmaTime = 0;

void pattern_Plasma() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern: Plasma Wave");
    initialized = true;
  }

  plasmaTime++;

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      float value = sin(r * 0.5 + plasmaTime * 0.05) +
                    sin(c * 0.5 + plasmaTime * 0.05) +
                    sin((r + c) * 0.5 + plasmaTime * 0.03) +
                    sin(sqrt(r * r + c * c) * 0.5 + plasmaTime * 0.04);

      int colorIndex = (int)((value + 4) * 32) % 256;
      int pixel = getPixelIndex(r, c);
      matrix.setPixelColor(pixel, wheel(colorIndex));
    }
  }

  matrix.show();
  delay(20);

  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
    plasmaTime = 0;
  }
}

// ========== PATTERN 6: RIPPLE EFFECT ==========

struct Ripple {
  int centerX, centerY;
  int radius;
  bool active;
};

Ripple ripples[3];
unsigned long lastRippleUpdate = 0;
unsigned long lastRippleSpawn = 0;

void pattern_Ripple() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern: Ripple Effect");
    for (int i = 0; i < 3; i++) {
      ripples[i].active = false;
    }
    initialized = true;
  }

  // Spawn new ripple
  if (millis() - lastRippleSpawn >= 1500) {
    lastRippleSpawn = millis();
    for (int i = 0; i < 3; i++) {
      if (!ripples[i].active) {
        ripples[i].centerX = random(COLS);
        ripples[i].centerY = random(ROWS);
        ripples[i].radius = 0;
        ripples[i].active = true;
        break;
      }
    }
  }

  if (millis() - lastRippleUpdate >= 100) {
    lastRippleUpdate = millis();
    clearMatrix();

    // Update and draw ripples
    for (int i = 0; i < 3; i++) {
      if (ripples[i].active) {
        for (int r = 0; r < ROWS; r++) {
          for (int c = 0; c < COLS; c++) {
            int dx = c - ripples[i].centerX;
            int dy = r - ripples[i].centerY;
            float dist = sqrt(dx * dx + dy * dy);

            if (abs(dist - ripples[i].radius) < 1.5) {
              int pixel = getPixelIndex(r, c);
              int brightness = 255 - (ripples[i].radius * 20);
              if (brightness > 0) {
                uint32_t color = matrix.Color(0, brightness / 2, brightness);
                matrix.setPixelColor(pixel, color);
              }
            }
          }
        }

        ripples[i].radius++;
        if (ripples[i].radius > 12) {
          ripples[i].active = false;
        }
      }
    }

    matrix.show();
  }

  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
  }
}

// ========== PATTERN 7: STARFIELD ==========

struct Star {
  float z;
  int x, y;
  int speed;
};

Star stars[32];
unsigned long lastStarUpdate = 0;

void pattern_Starfield() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern: Starfield");
    for (int i = 0; i < 32; i++) {
      stars[i].x = random(COLS);
      stars[i].y = random(ROWS);
      stars[i].z = random(1, 10);
      stars[i].speed = random(1, 4);
    }
    initialized = true;
  }

  if (millis() - lastStarUpdate >= 50) {
    lastStarUpdate = millis();
    clearMatrix();

    for (int i = 0; i < 32; i++) {
      stars[i].z -= stars[i].speed * 0.1;

      if (stars[i].z <= 0) {
        stars[i].x = random(COLS);
        stars[i].y = random(ROWS);
        stars[i].z = 10;
      }

      int brightness = map(stars[i].z * 10, 0, 100, 255, 50);
      int pixel = getPixelIndex(stars[i].y, stars[i].x);
      matrix.setPixelColor(pixel, matrix.Color(brightness, brightness, brightness));
    }

    matrix.show();
  }

  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
  }
}

// ========== PATTERN 8: DNA HELIX ==========

unsigned long dnaTime = 0;

void pattern_DNAHelix() {
  static bool initialized = false;
  if (!initialized) {
    Serial.println("Pattern: DNA Helix");
    initialized = true;
  }

  clearMatrix();

  for (int c = 0; c < COLS; c++) {
    float angle1 = (c + dnaTime * 0.1) * 0.785;  // 45 degrees per column
    float angle2 = angle1 + PI;

    int r1 = 3.5 + sin(angle1) * 3;
    int r2 = 3.5 + sin(angle2) * 3;

    // First strand
    int pixel1 = getPixelIndex(r1, c);
    matrix.setPixelColor(pixel1, matrix.Color(0, 150, 255));

    // Second strand
    int pixel2 = getPixelIndex(r2, c);
    matrix.setPixelColor(pixel2, matrix.Color(255, 0, 150));

    // Connecting rungs (every other column)
    if (c % 2 == 0) {
      int minR = min(r1, r2);
      int maxR = max(r1, r2);
      for (int r = minR + 1; r < maxR; r++) {
        int pixel = getPixelIndex(r, c);
        matrix.setPixelColor(pixel, matrix.Color(100, 100, 100));
      }
    }
  }

  matrix.show();
  dnaTime++;
  delay(50);

  if (millis() - patternStartTime >= PATTERN_DURATION - 100) {
    initialized = false;
    dnaTime = 0;
  }
}

// ========== HELPER FUNCTIONS ==========

int getPixelIndex(int row, int col) {
  if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
    return 0;
  }

  if (row % 2 == 0) {
    return row * COLS + col;
  } else {
    return row * COLS + (COLS - 1 - col);
  }
}

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

void clearMatrix() {
  for (int i = 0; i < NUMPIXELS; i++) {
    matrix.setPixelColor(i, 0);
  }
  matrix.show();
}
