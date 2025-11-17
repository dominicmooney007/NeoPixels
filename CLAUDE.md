# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an Arduino-based NeoPixel LED project repository containing educational examples and interactive projects. The codebase is designed for beginners learning to program WS2812B addressable LEDs, with a progression from simple examples to complex games.

## Development Environment

### Hardware Platform
- **Target Board**: Arduino Uno R4 WiFi (compatible with most Arduino boards)
- **LEDs**: WS2812B NeoPixel strips/matrices (8x8 matrix for games)
- **Library**: Adafruit NeoPixel library (required dependency)

### Uploading and Testing
Since this is Arduino firmware, there are no traditional build/test commands. Development workflow:

1. **Open sketches**: Open `.ino` files in Arduino IDE or Arduino CLI
2. **Verify/Compile**: Use Arduino IDE's "Verify" button or `arduino-cli compile`
3. **Upload**: Connect Arduino via USB and upload sketch
4. **Monitor**: Open Serial Monitor at 115200 baud to view debug output
5. **Test**: Observe LED behavior and Serial Monitor output

### Serial Monitor Configuration
All sketches use **115200 baud** for Serial communication. This is critical for interactive projects like Snake_Game which require keyboard input via Serial Monitor.

## Code Architecture

### Pin Configuration Pattern
Every sketch defines hardware configuration at the top:
```cpp
#define PIN 6           // Data pin (typically pin 6)
#define NUMPIXELS 8     // LED count (8 for strip, 64 for 8x8 matrix)
```
**Always verify these match the actual hardware setup before uploading.**

### Color Order Configuration
NeoPixels use different color orders depending on manufacturer. Most use:
```cpp
NEO_GRB + NEO_KHZ800  // Most common (Green-Red-Blue)
```
If colors appear wrong, try `NEO_RGB` or `NEO_BRG`.

### Matrix Coordinate System (8x8 Projects)
Matrix-based projects (Snake_Game, Snake_Auto, NeoPixel_Demo) use a 2D coordinate system:
- **Rows**: 0-7 (top to bottom)
- **Cols**: 0-7 (left to right)
- **Mapping**: `getPixelIndex(row, col)` converts coordinates to linear LED index

The mapping assumes **zigzag wiring pattern** (common in LED matrices):
- Even rows (0, 2, 4, 6): left-to-right
- Odd rows (1, 3, 5, 7): right-to-left

If LEDs appear in wrong positions, the matrix may have different wiring (serpentine vs. parallel).

### Game Loop Architecture (Snake Projects)
All Snake variants follow this pattern:
```cpp
void loop() {
  handleInput();           // Process user input
  if (millis() - lastMoveTime >= moveDelay) {
    moveSnake();          // Update positions
    checkCollisions();    // Detect wall/self hits
    checkFood();          // Handle food consumption
    drawGame();           // Render to LEDs
  }
}
```

Key state management:
- **Snake storage**: Array of `Point` structs (row, col pairs)
- **Direction**: Integer 0-3 (UP, RIGHT, DOWN, LEFT)
- **Collision prevention**: `lastDirection` prevents 180° turns
- **Timing**: `millis()` for non-blocking game speed control

## Project Structure

### Learning Progression
Examples are numbered sequentially for pedagogical flow:
1. `01_First_Light` - Basic LED control
2. `02_All_Colors` - RGB color values
3. `03_Running_Light` - Animation loops
4. `04_Rainbow` - Color algorithms
5. `05_Fade` - Brightness manipulation

### Interactive Projects
- **Snake_Game/**: Manual control via Serial input (WASD keys)
- **Snake_Auto/**: AI-controlled snake with pathfinding logic
- **Snake_Debug/**: Diagnostic tool for troubleshooting LED matrices
- **NeoPixel_Demo/**: Automated showcase of 8 animation techniques

## Common Development Patterns

### Brightness Management
All sketches set `matrix.setBrightness(30)` (out of 255) to:
- Reduce power consumption
- Prevent overwhelming brightness
- Enable USB-only power for testing

When modifying brightness, remember power draw scales with brightness and number of lit LEDs.

### Color Definition
Colors are created using `matrix.Color(red, green, blue)` where each component is 0-255. This must be called **after** `matrix.begin()` because the color order depends on initialized hardware.

Common mistake:
```cpp
// WRONG - created before matrix.begin()
uint32_t color = matrix.Color(255, 0, 0);

void setup() {
  matrix.begin();  // Too late!
}

// CORRECT - created after initialization
void setup() {
  matrix.begin();
  uint32_t color = matrix.Color(255, 0, 0);
}
```

### Non-Blocking Delays
Games use `millis()` timing instead of `delay()` to remain responsive:
```cpp
if (millis() - lastMoveTime >= moveDelay) {
  lastMoveTime = millis();
  // Execute timed action
}
```

This allows input handling to remain responsive while controlling animation speed.

## Hardware-Specific Notes

### Wiring Requirements
- **Common Ground**: Arduino GND must connect to Power Supply GND (critical for signal stability)
- **Data Line**: Use 470Ω resistor between Arduino pin and LED DIN for protection
- **Power Smoothing**: 1000µF capacitor across power supply recommended
- **External Power**: USB power sufficient for testing (<10 LEDs at low brightness), external 5V required for full 64-LED matrices

### Power Calculation
Each WS2812B LED draws up to 60mA at full white brightness. At brightness 30 (12% of max):
- 8 LEDs: ~58mA (USB safe)
- 64 LEDs (8x8): ~461mA (external power recommended)

## Troubleshooting Integration

When helping users debug issues:

1. **No light**: Check PIN and NUMPIXELS definitions match hardware
2. **Wrong colors**: Try different color order (`NEO_GRB`, `NEO_RGB`, `NEO_BRG`)
3. **Incorrect positions**: Verify `getPixelIndex()` zigzag pattern matches matrix wiring
4. **Erratic behavior**: Check for common ground connection between Arduino and power supply
5. **Snake game unresponsive**: Verify Serial Monitor is set to 115200 baud and sending input

## File Modification Guidelines

When editing sketches:
- Preserve existing Serial debug output patterns (users rely on this for learning)
- Maintain consistent configuration pattern (`#define PIN/NUMPIXELS` at top)
- Keep comments beginner-friendly (this is educational code)
- Test with both low and high brightness settings
- Verify changes work with USB power (not everyone has external supplies)
