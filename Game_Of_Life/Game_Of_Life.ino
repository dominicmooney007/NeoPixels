/*
 * Conway's Game of Life - 8x8 NeoPixel LED Matrix
 *
 * The classic cellular automaton simulation on your LED matrix!
 * Watch cells live, die, and evolve according to simple rules.
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
 * Controls (via Serial Monitor at 115200 baud):
 * - 'r' or 'R' = Random pattern
 * - 'g' or 'G' = Glider pattern
 * - 'b' or 'B' = Blinker pattern
 * - 'p' or 'P' = Pulsar pattern
 * - 'l' or 'L' = Lightweight spaceship
 * - 's' or 'S' = Slower speed
 * - 'f' or 'F' = Faster speed
 * - ' ' (space) = Pause/Resume
 * - 'c' or 'C' = Clear grid
 * - 'h' or 'H' = Show help
 *
 * Rules:
 * - Any live cell with 2-3 neighbors survives
 * - Any dead cell with exactly 3 neighbors becomes alive
 * - All other cells die or stay dead
 */

#include <Adafruit_NeoPixel.h>

// Pin and matrix configuration
#define PIN 6
#define ROWS 8
#define COLS 8
#define NUMPIXELS (ROWS * COLS)

// Create NeoPixel object
Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Game state
byte grid[ROWS][COLS];
byte newGrid[ROWS][COLS];
unsigned long lastUpdate = 0;
int updateDelay = 300;  // milliseconds between generations
int generation = 0;
bool paused = false;

// Color scheme
enum ColorMode {
  COLOR_AGE,      // Color based on cell age
  COLOR_NEIGHBORS, // Color based on neighbor count
  COLOR_CLASSIC,   // Just green/black
  COLOR_RAINBOW    // Cycle through rainbow
};

ColorMode colorMode = COLOR_NEIGHBORS;
byte cellAge[ROWS][COLS];  // Track how long cells have been alive

// Statistics
int totalBorn = 0;
int totalDied = 0;
int stableGenerations = 0;
int lastPopulation = 0;

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setBrightness(40);
  matrix.show();

  randomSeed(analogRead(0));

  Serial.println("========================================");
  Serial.println("     CONWAY'S GAME OF LIFE");
  Serial.println("     8x8 NeoPixel Edition");
  Serial.println("========================================");
  printHelp();

  // Start with random pattern
  randomPattern(50);  // 50% density
}

void loop() {
  // Handle serial input
  if (Serial.available() > 0) {
    char input = Serial.read();
    handleInput(input);
  }

  // Update game state
  if (!paused && (millis() - lastUpdate >= updateDelay)) {
    lastUpdate = millis();
    updateGeneration();
    drawGrid();
    checkStability();
  }
}

// ========== GAME LOGIC ==========

void updateGeneration() {
  int born = 0;
  int died = 0;
  int population = 0;

  // Calculate next generation
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      int neighbors = countNeighbors(r, c);

      // Apply Conway's rules
      if (grid[r][c] == 1) {
        // Live cell
        if (neighbors == 2 || neighbors == 3) {
          newGrid[r][c] = 1;
          cellAge[r][c]++;  // Cell gets older
          population++;
        } else {
          newGrid[r][c] = 0;
          cellAge[r][c] = 0;
          died++;
        }
      } else {
        // Dead cell
        if (neighbors == 3) {
          newGrid[r][c] = 1;
          cellAge[r][c] = 1;
          born++;
          population++;
        } else {
          newGrid[r][c] = 0;
          cellAge[r][c] = 0;
        }
      }
    }
  }

  // Copy new grid to current grid
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      grid[r][c] = newGrid[r][c];
    }
  }

  generation++;
  totalBorn += born;
  totalDied += died;

  // Print generation info
  Serial.print("Gen ");
  Serial.print(generation);
  Serial.print(" | Pop: ");
  Serial.print(population);
  Serial.print(" | Born: ");
  Serial.print(born);
  Serial.print(" | Died: ");
  Serial.print(died);
  Serial.print(" | Speed: ");
  Serial.print(updateDelay);
  Serial.println("ms");

  lastPopulation = population;

  // Auto-reset if population dies
  if (population == 0) {
    Serial.println("\n>>> Population extinct! Generating new random pattern...\n");
    delay(1000);
    randomPattern(40);
  }
}

int countNeighbors(int row, int col) {
  int count = 0;

  // Check all 8 surrounding cells
  for (int dr = -1; dr <= 1; dr++) {
    for (int dc = -1; dc <= 1; dc++) {
      if (dr == 0 && dc == 0) continue;  // Skip the cell itself

      // Wrap around edges (toroidal topology)
      int r = (row + dr + ROWS) % ROWS;
      int c = (col + dc + COLS) % COLS;

      count += grid[r][c];
    }
  }

  return count;
}

void checkStability() {
  // Check if the pattern has stabilized
  bool identical = true;
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      if (grid[r][c] != newGrid[r][c]) {
        identical = true;  // Pattern changed
        stableGenerations = 0;
        return;
      }
    }
  }

  // If we get here, pattern is identical (still life)
  stableGenerations++;
  if (stableGenerations == 1) {
    Serial.println("\n>>> Pattern stabilized (still life detected)");
  } else if (stableGenerations >= 20) {
    Serial.println(">>> Stable for 20 generations, generating new pattern...\n");
    randomPattern(40);
  }
}

// ========== DRAWING ==========

void drawGrid() {
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      int pixel = getPixelIndex(r, c);

      if (grid[r][c] == 1) {
        // Cell is alive - choose color based on mode
        uint32_t color;
        switch (colorMode) {
          case COLOR_AGE:
            color = getAgeColor(cellAge[r][c]);
            break;
          case COLOR_NEIGHBORS:
            color = getNeighborColor(countNeighbors(r, c));
            break;
          case COLOR_CLASSIC:
            color = matrix.Color(0, 255, 0);
            break;
          case COLOR_RAINBOW:
            color = wheel((generation * 10 + r * 32 + c * 32) % 256);
            break;
        }
        matrix.setPixelColor(pixel, color);
      } else {
        // Cell is dead
        matrix.setPixelColor(pixel, matrix.Color(0, 0, 0));
      }
    }
  }
  matrix.show();
}

uint32_t getAgeColor(byte age) {
  // Young cells are bright blue, old cells are red
  if (age == 1) return matrix.Color(0, 100, 255);      // Newborn: bright blue
  else if (age < 5) return matrix.Color(0, 200, 200);  // Young: cyan
  else if (age < 10) return matrix.Color(0, 255, 100); // Middle: green
  else if (age < 20) return matrix.Color(200, 200, 0); // Old: yellow
  else return matrix.Color(255, 100, 0);                // Ancient: orange-red
}

uint32_t getNeighborColor(int neighbors) {
  // Color indicates how many neighbors (helps visualize rules)
  switch(neighbors) {
    case 0:
    case 1: return matrix.Color(50, 0, 100);    // Lonely: purple
    case 2: return matrix.Color(0, 150, 255);   // Stable: blue
    case 3: return matrix.Color(0, 255, 50);    // Perfect: bright green
    case 4: return matrix.Color(255, 200, 0);   // Crowded: yellow
    case 5:
    case 6: return matrix.Color(255, 100, 0);   // Overcrowded: orange
    default: return matrix.Color(255, 0, 0);    // Too crowded: red
  }
}

// ========== PATTERNS ==========

void randomPattern(int density) {
  generation = 0;
  totalBorn = 0;
  totalDied = 0;
  stableGenerations = 0;

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      grid[r][c] = (random(100) < density) ? 1 : 0;
      cellAge[r][c] = grid[r][c];
    }
  }

  Serial.println("\n--- New Random Pattern ---");
  Serial.print("Density: ");
  Serial.print(density);
  Serial.println("%");
  drawGrid();
}

void clearGrid() {
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      grid[r][c] = 0;
      cellAge[r][c] = 0;
    }
  }
  generation = 0;
  totalBorn = 0;
  totalDied = 0;
  stableGenerations = 0;

  Serial.println("\n--- Grid Cleared ---");
  drawGrid();
}

void gliderPattern() {
  clearGrid();

  // Classic glider (travels diagonally)
  grid[1][2] = 1;
  grid[2][3] = 1;
  grid[3][1] = 1;
  grid[3][2] = 1;
  grid[3][3] = 1;

  // Another glider
  grid[5][5] = 1;
  grid[6][6] = 1;
  grid[4][4] = 1;
  grid[4][5] = 1;
  grid[4][6] = 1;

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      cellAge[r][c] = grid[r][c];
    }
  }

  Serial.println("\n--- Glider Pattern Loaded ---");
  Serial.println("Watch the gliders travel across the matrix!");
  drawGrid();
}

void blinkerPattern() {
  clearGrid();

  // Horizontal blinkers
  grid[2][2] = 1;
  grid[2][3] = 1;
  grid[2][4] = 1;

  grid[5][3] = 1;
  grid[5][4] = 1;
  grid[5][5] = 1;

  // Vertical area
  grid[1][6] = 1;
  grid[2][6] = 1;
  grid[3][6] = 1;

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      cellAge[r][c] = grid[r][c];
    }
  }

  Serial.println("\n--- Blinker Pattern Loaded ---");
  Serial.println("Oscillating pattern (period 2)");
  drawGrid();
}

void pulsarPattern() {
  clearGrid();

  // Simplified pulsar (modified for 8x8)
  // Create symmetric pattern
  int pattern[4][2] = {{1,3}, {1,4}, {3,1}, {4,1}};

  for (int i = 0; i < 4; i++) {
    int r = pattern[i][0];
    int c = pattern[i][1];

    // Create 4-way symmetry
    grid[r][c] = 1;
    grid[r][COLS-1-c] = 1;
    grid[ROWS-1-r][c] = 1;
    grid[ROWS-1-r][COLS-1-c] = 1;
  }

  // Add center cross
  grid[3][3] = 1;
  grid[3][4] = 1;
  grid[4][3] = 1;
  grid[4][4] = 1;

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      cellAge[r][c] = grid[r][c];
    }
  }

  Serial.println("\n--- Pulsar Pattern Loaded ---");
  Serial.println("Oscillating pattern (modified for 8x8)");
  drawGrid();
}

void spaceshipPattern() {
  clearGrid();

  // Lightweight spaceship (LWSS)
  grid[2][2] = 1;
  grid[2][3] = 1;
  grid[2][4] = 1;
  grid[2][5] = 1;
  grid[3][1] = 1;
  grid[3][5] = 1;
  grid[4][5] = 1;
  grid[5][1] = 1;
  grid[5][4] = 1;

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      cellAge[r][c] = grid[r][c];
    }
  }

  Serial.println("\n--- Lightweight Spaceship Loaded ---");
  Serial.println("Watch it travel horizontally!");
  drawGrid();
}

// ========== INPUT HANDLING ==========

void handleInput(char input) {
  switch(input) {
    case 'r':
    case 'R':
      randomPattern(random(30, 60));
      paused = false;
      break;

    case 'g':
    case 'G':
      gliderPattern();
      paused = false;
      break;

    case 'b':
    case 'B':
      blinkerPattern();
      paused = false;
      break;

    case 'p':
    case 'P':
      pulsarPattern();
      paused = false;
      break;

    case 'l':
    case 'L':
      spaceshipPattern();
      paused = false;
      break;

    case 's':
    case 'S':
      updateDelay = min(1000, updateDelay + 50);
      Serial.print("Speed: ");
      Serial.print(updateDelay);
      Serial.println("ms (slower)");
      break;

    case 'f':
    case 'F':
      updateDelay = max(50, updateDelay - 50);
      Serial.print("Speed: ");
      Serial.print(updateDelay);
      Serial.println("ms (faster)");
      break;

    case ' ':
      paused = !paused;
      Serial.println(paused ? ">>> PAUSED <<<" : ">>> RESUMED <<<");
      break;

    case 'c':
    case 'C':
      clearGrid();
      paused = true;
      break;

    case 'm':
    case 'M':
      colorMode = (ColorMode)((colorMode + 1) % 4);
      Serial.print("Color mode: ");
      switch(colorMode) {
        case COLOR_AGE: Serial.println("AGE (blue=new, red=old)"); break;
        case COLOR_NEIGHBORS: Serial.println("NEIGHBORS (shows crowding)"); break;
        case COLOR_CLASSIC: Serial.println("CLASSIC (green)"); break;
        case COLOR_RAINBOW: Serial.println("RAINBOW"); break;
      }
      drawGrid();
      break;

    case 'i':
    case 'I':
      printStats();
      break;

    case 'h':
    case 'H':
      printHelp();
      break;
  }
}

void printHelp() {
  Serial.println("\n======== CONTROLS ========");
  Serial.println("R = Random pattern");
  Serial.println("G = Glider");
  Serial.println("B = Blinker");
  Serial.println("P = Pulsar");
  Serial.println("L = Lightweight Spaceship");
  Serial.println("---");
  Serial.println("F = Faster");
  Serial.println("S = Slower");
  Serial.println("SPACE = Pause/Resume");
  Serial.println("M = Change color mode");
  Serial.println("C = Clear grid");
  Serial.println("I = Show statistics");
  Serial.println("H = Show this help");
  Serial.println("==========================\n");
}

void printStats() {
  Serial.println("\n======== STATISTICS ========");
  Serial.print("Generation: ");
  Serial.println(generation);
  Serial.print("Population: ");
  Serial.println(lastPopulation);
  Serial.print("Total Born: ");
  Serial.println(totalBorn);
  Serial.print("Total Died: ");
  Serial.println(totalDied);
  Serial.print("Speed: ");
  Serial.print(updateDelay);
  Serial.println("ms");
  Serial.print("Status: ");
  Serial.println(paused ? "PAUSED" : "RUNNING");
  Serial.println("===========================\n");
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
