/*
 * Snake Game for 8x8 NeoPixel LED Matrix
 * Control the snake using Serial Monitor input
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
 *
 * Controls (via Serial Monitor at 115200 baud):
 * - 'w' or 'W' = Move Up
 * - 's' or 'S' = Move Down
 * - 'a' or 'A' = Move Left
 * - 'd' or 'D' = Move Right
 * - 'r' or 'R' = Restart Game
 */

#include <Adafruit_NeoPixel.h>

// Pin and matrix configuration
#define PIN 6
#define ROWS 8
#define COLS 8
#define NUMPIXELS (ROWS * COLS)

// Game configuration
#define INITIAL_SPEED 500      // milliseconds between moves
#define SPEED_INCREMENT 20     // speed up by this amount per food eaten
#define MIN_SPEED 150          // fastest possible speed

// Create NeoPixel object
Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Snake structure
struct Point {
  int row;
  int col;
};

Point snake[NUMPIXELS];  // Maximum possible snake length
int snakeLength = 3;     // Starting length
Point food;

// Direction: 0=UP, 1=RIGHT, 2=DOWN, 3=LEFT
int direction = 1;       // Start moving right
int lastDirection = 1;   // Prevent reversing into itself

unsigned long lastMoveTime = 0;
int moveDelay = INITIAL_SPEED;
int score = 0;
bool gameOver = false;

// Colors (initialized in setup after matrix.begin())
uint32_t colorSnakeHead;
uint32_t colorSnakeBody;
uint32_t colorFood;
uint32_t colorEmpty;

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setBrightness(30);
  matrix.show();

  // Initialize colors AFTER matrix.begin()
  colorSnakeHead = matrix.Color(0, 255, 0);    // Green
  colorSnakeBody = matrix.Color(0, 100, 0);    // Dark green
  colorFood = matrix.Color(255, 0, 0);         // Red
  colorEmpty = matrix.Color(0, 0, 0);          // Off

  randomSeed(analogRead(0));  // Seed random number generator

  Serial.println("========================================");
  Serial.println("      SNAKE GAME - 8x8 LED Matrix      ");
  Serial.println("========================================");
  Serial.println("Controls:");
  Serial.println("  W = Up    | S = Down");
  Serial.println("  A = Left  | D = Right");
  Serial.println("  R = Restart Game");
  Serial.println("========================================");

  initGame();
}

void loop() {
  // Handle serial input for direction changes
  if (Serial.available() > 0) {
    char input = Serial.read();
    handleInput(input);
  }

  if (!gameOver) {
    // Move snake at specified interval
    if (millis() - lastMoveTime >= moveDelay) {
      lastMoveTime = millis();

      Serial.print("Moving... Head: (");
      Serial.print(snake[0].row);
      Serial.print(",");
      Serial.print(snake[0].col);
      Serial.print(") -> ");

      moveSnake();

      Serial.print("(");
      Serial.print(snake[0].row);
      Serial.print(",");
      Serial.print(snake[0].col);
      Serial.println(")");

      checkCollisions();
      if (!gameOver) {
        checkFood();
        drawGame();
      }
    }
  } else {
    // Game over animation
    gameOverAnimation();
  }
}

// Initialize game state
void initGame() {
  snakeLength = 3;
  direction = 1;      // Moving right
  lastDirection = 1;
  score = 0;
  gameOver = false;
  moveDelay = INITIAL_SPEED;

  // Initialize snake in the middle, moving right
  snake[0].row = 4;  // Head
  snake[0].col = 4;
  snake[1].row = 4;
  snake[1].col = 3;
  snake[2].row = 4;
  snake[2].col = 2;

  spawnFood();
  drawGame();

  Serial.println("\nGame Started!");
  Serial.print("Speed: ");
  Serial.println(moveDelay);
  Serial.print("Initial Snake - Head: (");
  Serial.print(snake[0].row);
  Serial.print(",");
  Serial.print(snake[0].col);
  Serial.println(")");
  Serial.print("Food at: (");
  Serial.print(food.row);
  Serial.print(",");
  Serial.print(food.col);
  Serial.println(")");
}

// Handle keyboard input
void handleInput(char input) {
  switch(input) {
    case 'w':
    case 'W':
      if (lastDirection != 2) direction = 0;  // Can't go up if moving down
      break;
    case 'd':
    case 'D':
      if (lastDirection != 3) direction = 1;  // Can't go right if moving left
      break;
    case 's':
    case 'S':
      if (lastDirection != 0) direction = 2;  // Can't go down if moving up
      break;
    case 'a':
    case 'A':
      if (lastDirection != 1) direction = 3;  // Can't go left if moving right
      break;
    case 'r':
    case 'R':
      Serial.println("\n--- RESTARTING GAME ---");
      initGame();
      break;
  }
}

// Move the snake in current direction
void moveSnake() {
  // Shift body segments
  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i] = snake[i - 1];
  }

  // Move head based on direction
  switch(direction) {
    case 0:  // UP
      snake[0].row--;
      break;
    case 1:  // RIGHT
      snake[0].col++;
      break;
    case 2:  // DOWN
      snake[0].row++;
      break;
    case 3:  // LEFT
      snake[0].col--;
      break;
  }

  lastDirection = direction;
}

// Check for collisions with walls or self
void checkCollisions() {
  // Check wall collision
  if (snake[0].row < 0 || snake[0].row >= ROWS ||
      snake[0].col < 0 || snake[0].col >= COLS) {
    endGame("Hit the wall!");
    return;
  }

  // Check self collision
  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].row == snake[i].row && snake[0].col == snake[i].col) {
      endGame("Hit yourself!");
      return;
    }
  }
}

// Check if snake ate food
void checkFood() {
  if (snake[0].row == food.row && snake[0].col == food.col) {
    // Snake ate food - grow!
    snakeLength++;
    score++;

    // Speed up the game
    moveDelay = max(MIN_SPEED, moveDelay - SPEED_INCREMENT);

    Serial.print("FOOD! Score: ");
    Serial.print(score);
    Serial.print(" | Length: ");
    Serial.print(snakeLength);
    Serial.print(" | Speed: ");
    Serial.println(moveDelay);

    // Check for win condition (filled entire matrix)
    if (snakeLength >= NUMPIXELS) {
      endGame("YOU WIN! Perfect score!");
      return;
    }

    spawnFood();
  }
}

// Spawn food in a random empty location
void spawnFood() {
  bool validPosition = false;

  while (!validPosition) {
    food.row = random(ROWS);
    food.col = random(COLS);

    // Check if food spawned on snake
    validPosition = true;
    for (int i = 0; i < snakeLength; i++) {
      if (food.row == snake[i].row && food.col == snake[i].col) {
        validPosition = false;
        break;
      }
    }
  }
}

// Draw the game state to the matrix
void drawGame() {
  // Clear matrix
  for (int i = 0; i < NUMPIXELS; i++) {
    matrix.setPixelColor(i, colorEmpty);
  }

  // Draw food
  int foodPixel = getPixelIndex(food.row, food.col);
  matrix.setPixelColor(foodPixel, colorFood);

  // Draw snake body
  for (int i = 1; i < snakeLength; i++) {
    int pixel = getPixelIndex(snake[i].row, snake[i].col);
    matrix.setPixelColor(pixel, colorSnakeBody);
  }

  // Draw snake head (on top so it's visible)
  int headPixel = getPixelIndex(snake[0].row, snake[0].col);
  matrix.setPixelColor(headPixel, colorSnakeHead);

  matrix.show();
}

// End the game
void endGame(const char* message) {
  gameOver = true;
  Serial.println("\n========================================");
  Serial.println("           GAME OVER!");
  Serial.println("========================================");
  Serial.println(message);
  Serial.print("Final Score: ");
  Serial.println(score);
  Serial.print("Final Length: ");
  Serial.println(snakeLength);
  Serial.println("\nPress 'R' to restart");
  Serial.println("========================================");
}

// Game over animation
void gameOverAnimation() {
  // Flash the snake red a few times
  static int flashCount = 0;
  static unsigned long lastFlash = 0;
  static bool flashOn = false;

  if (millis() - lastFlash > 300) {
    lastFlash = millis();
    flashOn = !flashOn;
    flashCount++;

    if (flashCount < 12) {  // Flash 6 times (12 toggles)
      if (flashOn) {
        // Draw snake in red
        for (int i = 0; i < snakeLength; i++) {
          int pixel = getPixelIndex(snake[i].row, snake[i].col);
          matrix.setPixelColor(pixel, matrix.Color(255, 0, 0));
        }
      } else {
        // Clear
        for (int i = 0; i < NUMPIXELS; i++) {
          matrix.setPixelColor(i, colorEmpty);
        }
      }
      matrix.show();
    } else {
      // After flashing, just keep it dark
      flashCount = 12;  // Stop counting
    }
  }
}

// Get pixel index from row and column (zigzag pattern)
int getPixelIndex(int row, int col) {
  // Bounds checking
  if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
    return 0;  // Return first pixel if out of bounds
  }

  // Standard zigzag pattern
  if (row % 2 == 0) {
    return row * COLS + col;
  } else {
    return row * COLS + (COLS - 1 - col);
  }
}
