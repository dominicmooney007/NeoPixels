/*
 * Auto-Playing Snake for 8x8 NeoPixel LED Matrix
 * Snake moves automatically using simple AI
 * No input required - just watch!
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

#define PIN 6
#define ROWS 8
#define COLS 8
#define NUMPIXELS (ROWS * COLS)

#define INITIAL_SPEED 300
#define SPEED_INCREMENT 15
#define MIN_SPEED 100

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

struct Point {
  int row;
  int col;
};

Point snake[NUMPIXELS];
int snakeLength = 3;
Point food;

int direction = 1;  // 0=UP, 1=RIGHT, 2=DOWN, 3=LEFT
unsigned long lastMoveTime = 0;
int moveDelay = INITIAL_SPEED;
int score = 0;
bool gameOver = false;

// Colors
uint32_t colorSnakeHead;
uint32_t colorSnakeBody;
uint32_t colorFood;
uint32_t colorEmpty;

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setBrightness(30);
  matrix.show();

  // Initialize colors
  colorSnakeHead = matrix.Color(0, 255, 0);
  colorSnakeBody = matrix.Color(0, 100, 0);
  colorFood = matrix.Color(255, 0, 0);
  colorEmpty = matrix.Color(0, 0, 0);

  randomSeed(analogRead(0));

  Serial.println("Auto-Playing Snake - Watch the snake find food!");

  initGame();
}

void loop() {
  if (!gameOver) {
    if (millis() - lastMoveTime >= moveDelay) {
      lastMoveTime = millis();

      // Simple AI: move toward food
      autoMove();

      moveSnake();
      checkCollisions();

      if (!gameOver) {
        checkFood();
        drawGame();
      }
    }
  } else {
    // Auto-restart after game over
    delay(3000);
    Serial.println("\n--- Auto Restarting ---");
    initGame();
  }
}

void initGame() {
  snakeLength = 3;
  direction = 1;
  score = 0;
  gameOver = false;
  moveDelay = INITIAL_SPEED;

  // Start in middle
  snake[0].row = 3;
  snake[0].col = 3;
  snake[1].row = 3;
  snake[1].col = 2;
  snake[2].row = 3;
  snake[2].col = 1;

  spawnFood();
  drawGame();

  Serial.println("Game Started!");
}

// Simple AI to move toward food
void autoMove() {
  int currentDir = direction;
  int rowDiff = food.row - snake[0].row;
  int colDiff = food.col - snake[0].col;

  // Try to move toward food, but avoid reversing direction
  if (abs(rowDiff) > abs(colDiff)) {
    // Prioritize vertical movement
    if (rowDiff < 0 && currentDir != 2) {
      direction = 0;  // UP
    } else if (rowDiff > 0 && currentDir != 0) {
      direction = 2;  // DOWN
    } else if (colDiff < 0 && currentDir != 1) {
      direction = 3;  // LEFT
    } else if (colDiff > 0 && currentDir != 3) {
      direction = 1;  // RIGHT
    }
  } else {
    // Prioritize horizontal movement
    if (colDiff < 0 && currentDir != 1) {
      direction = 3;  // LEFT
    } else if (colDiff > 0 && currentDir != 3) {
      direction = 1;  // RIGHT
    } else if (rowDiff < 0 && currentDir != 2) {
      direction = 0;  // UP
    } else if (rowDiff > 0 && currentDir != 0) {
      direction = 2;  // DOWN
    }
  }

  // Simple collision avoidance - check if next move hits wall or self
  Point nextPos = snake[0];
  switch(direction) {
    case 0: nextPos.row--; break;
    case 1: nextPos.col++; break;
    case 2: nextPos.row++; break;
    case 3: nextPos.col--; break;
  }

  // If next move is bad, try a different direction
  if (nextPos.row < 0 || nextPos.row >= ROWS ||
      nextPos.col < 0 || nextPos.col >= COLS ||
      wouldHitSelf(nextPos)) {
    // Try all directions to find a safe one
    for (int d = 0; d < 4; d++) {
      if (d == (currentDir + 2) % 4) continue;  // Skip reverse direction

      Point testPos = snake[0];
      switch(d) {
        case 0: testPos.row--; break;
        case 1: testPos.col++; break;
        case 2: testPos.row++; break;
        case 3: testPos.col--; break;
      }

      if (testPos.row >= 0 && testPos.row < ROWS &&
          testPos.col >= 0 && testPos.col < COLS &&
          !wouldHitSelf(testPos)) {
        direction = d;
        break;
      }
    }
  }
}

bool wouldHitSelf(Point pos) {
  for (int i = 0; i < snakeLength; i++) {
    if (pos.row == snake[i].row && pos.col == snake[i].col) {
      return true;
    }
  }
  return false;
}

void moveSnake() {
  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i] = snake[i - 1];
  }

  switch(direction) {
    case 0: snake[0].row--; break;
    case 1: snake[0].col++; break;
    case 2: snake[0].row++; break;
    case 3: snake[0].col--; break;
  }
}

void checkCollisions() {
  if (snake[0].row < 0 || snake[0].row >= ROWS ||
      snake[0].col < 0 || snake[0].col >= COLS) {
    endGame("Hit wall!");
    return;
  }

  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].row == snake[i].row && snake[0].col == snake[i].col) {
      endGame("Hit self!");
      return;
    }
  }
}

void checkFood() {
  if (snake[0].row == food.row && snake[0].col == food.col) {
    snakeLength++;
    score++;
    moveDelay = max(MIN_SPEED, moveDelay - SPEED_INCREMENT);

    Serial.print("Score: ");
    Serial.print(score);
    Serial.print(" | Length: ");
    Serial.println(snakeLength);

    if (snakeLength >= NUMPIXELS) {
      endGame("PERFECT SCORE!");
      return;
    }

    spawnFood();
  }
}

void spawnFood() {
  bool validPosition = false;

  while (!validPosition) {
    food.row = random(ROWS);
    food.col = random(COLS);

    validPosition = true;
    for (int i = 0; i < snakeLength; i++) {
      if (food.row == snake[i].row && food.col == snake[i].col) {
        validPosition = false;
        break;
      }
    }
  }
}

void drawGame() {
  for (int i = 0; i < NUMPIXELS; i++) {
    matrix.setPixelColor(i, colorEmpty);
  }

  int foodPixel = getPixelIndex(food.row, food.col);
  matrix.setPixelColor(foodPixel, colorFood);

  for (int i = 1; i < snakeLength; i++) {
    int pixel = getPixelIndex(snake[i].row, snake[i].col);
    matrix.setPixelColor(pixel, colorSnakeBody);
  }

  int headPixel = getPixelIndex(snake[0].row, snake[0].col);
  matrix.setPixelColor(headPixel, colorSnakeHead);

  matrix.show();
}

void endGame(const char* message) {
  gameOver = true;
  Serial.println("GAME OVER!");
  Serial.println(message);
  Serial.print("Final Score: ");
  Serial.println(score);
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
