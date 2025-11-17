/*
 * Snake Diagnostic - Simple Auto-Moving Snake
 * This version moves automatically without input to help diagnose issues
 */

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define ROWS 8
#define COLS 8
#define NUMPIXELS (ROWS * COLS)

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

struct Point {
  int row;
  int col;
};

Point snake[64];
int snakeLength = 3;
int direction = 1;  // 0=UP, 1=RIGHT, 2=DOWN, 3=LEFT
unsigned long lastMoveTime = 0;
int moveDelay = 800;  // Slow speed for debugging

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setBrightness(30);
  matrix.show();

  // Initialize snake
  snake[0].row = 3;  // Head
  snake[0].col = 3;
  snake[1].row = 3;
  snake[1].col = 2;
  snake[2].row = 3;
  snake[2].col = 1;

  Serial.println("Snake Debug - Auto Movement Test");
  Serial.println("Snake should move right automatically");
  printSnakeState();
}

void loop() {
  if (millis() - lastMoveTime >= moveDelay) {
    lastMoveTime = millis();

    Serial.println("\n--- Moving ---");

    // Shift body
    for (int i = snakeLength - 1; i > 0; i--) {
      snake[i] = snake[i - 1];
    }

    // Move head right
    snake[0].col++;

    // Wrap around if needed (for testing)
    if (snake[0].col >= COLS) {
      snake[0].col = 0;
      snake[0].row++;
      if (snake[0].row >= ROWS) {
        snake[0].row = 0;
      }
    }

    printSnakeState();
    drawSnake();
  }
}

void printSnakeState() {
  Serial.print("Snake Length: ");
  Serial.println(snakeLength);
  for (int i = 0; i < snakeLength; i++) {
    Serial.print("  Segment ");
    Serial.print(i);
    Serial.print(": row=");
    Serial.print(snake[i].row);
    Serial.print(", col=");
    Serial.print(snake[i].col);
    Serial.print(", pixel=");
    Serial.println(getPixelIndex(snake[i].row, snake[i].col));
  }
}

void drawSnake() {
  // Clear all
  for (int i = 0; i < NUMPIXELS; i++) {
    matrix.setPixelColor(i, matrix.Color(0, 0, 0));
  }

  // Draw body (blue)
  for (int i = 1; i < snakeLength; i++) {
    int pixel = getPixelIndex(snake[i].row, snake[i].col);
    matrix.setPixelColor(pixel, matrix.Color(0, 0, 100));
  }

  // Draw head (green)
  int headPixel = getPixelIndex(snake[0].row, snake[0].col);
  matrix.setPixelColor(headPixel, matrix.Color(0, 255, 0));

  matrix.show();

  Serial.print("Head pixel index: ");
  Serial.println(headPixel);
}

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
