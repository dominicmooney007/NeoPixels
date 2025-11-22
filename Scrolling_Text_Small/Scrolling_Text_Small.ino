/*
 * Scrolling Text Display - Small Font Version
 * For 8x8 NeoPixel Matrix
 *
 * Uses a compact 3x5 pixel font to display more text at once.
 * Preserves lowercase letters for better readability.
 *
 * Hardware Setup:
 * - 8x8 NeoPixel Matrix (64 LEDs) connected to pin 6
 * - Arduino Uno R4 WiFi (or compatible board)
 *
 * Usage:
 * 1. Upload sketch to Arduino
 * 2. Open Serial Monitor at 115200 baud
 * 3. Type your message and press Enter
 * 4. Watch it scroll across the matrix!
 */

#include <Adafruit_NeoPixel.h>

// Hardware configuration
#define PIN 6           // Data pin connected to NeoPixel DIN
#define NUMPIXELS 64    // 8x8 matrix = 64 LEDs
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8

// Create NeoPixel object
Adafruit_NeoPixel matrix(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Scrolling parameters
int scrollSpeed = 80;         // Milliseconds between scroll steps
unsigned long lastScrollTime = 0;
int scrollPosition = MATRIX_WIDTH;  // Start from right edge

// Message buffer
String currentMessage = " olleh";  // Default message reversed for R-to-L scroll
String inputBuffer = "";           // Buffer for incoming serial data

// Text color (can be changed)
uint32_t textColor;
uint32_t backgroundColor;

// Vertical offset to center 5-pixel tall font on 8-pixel display
#define FONT_HEIGHT 5
#define VERTICAL_OFFSET 1  // Centers the 5px font (leaves 1px top, 2px bottom)

// 3x5 Font definition - each character is 3 columns wide, 5 rows tall
// Stored as 3 bytes per character, each byte represents one column (LSB = top row)
// Only uses lower 5 bits of each byte
const uint8_t font3x5[][3] PROGMEM = {
  {0x00, 0x00, 0x00}, // Space (32)
  {0x00, 0x17, 0x00}, // ! (33)
  {0x03, 0x00, 0x03}, // " (34)
  {0x1F, 0x0A, 0x1F}, // # (35)
  {0x16, 0x1F, 0x0D}, // $ (36)
  {0x19, 0x04, 0x13}, // % (37)
  {0x0A, 0x15, 0x1A}, // & (38)
  {0x00, 0x03, 0x00}, // ' (39)
  {0x00, 0x0E, 0x11}, // ( (40)
  {0x11, 0x0E, 0x00}, // ) (41)
  {0x05, 0x02, 0x05}, // * (42)
  {0x04, 0x0E, 0x04}, // + (43)
  {0x10, 0x08, 0x00}, // , (44)
  {0x04, 0x04, 0x04}, // - (45)
  {0x00, 0x10, 0x00}, // . (46)
  {0x18, 0x04, 0x03}, // / (47)
  {0x1F, 0x11, 0x1F}, // 0 (48)
  {0x12, 0x1F, 0x10}, // 1 (49)
  {0x1D, 0x15, 0x17}, // 2 (50)
  {0x15, 0x15, 0x1F}, // 3 (51)
  {0x07, 0x04, 0x1F}, // 4 (52)
  {0x17, 0x15, 0x1D}, // 5 (53)
  {0x1F, 0x15, 0x1D}, // 6 (54)
  {0x01, 0x01, 0x1F}, // 7 (55)
  {0x1F, 0x15, 0x1F}, // 8 (56)
  {0x17, 0x15, 0x1F}, // 9 (57)
  {0x00, 0x0A, 0x00}, // : (58)
  {0x10, 0x0A, 0x00}, // ; (59)
  {0x04, 0x0A, 0x11}, // < (60)
  {0x0A, 0x0A, 0x0A}, // = (61)
  {0x11, 0x0A, 0x04}, // > (62)
  {0x01, 0x15, 0x03}, // ? (63)
  {0x0E, 0x15, 0x16}, // @ (64)
  {0x1E, 0x05, 0x1E}, // A (65)
  {0x1F, 0x15, 0x0A}, // B (66)
  {0x0E, 0x11, 0x11}, // C (67)
  {0x1F, 0x11, 0x0E}, // D (68)
  {0x1F, 0x15, 0x15}, // E (69)
  {0x1F, 0x05, 0x05}, // F (70)
  {0x0E, 0x11, 0x1D}, // G (71)
  {0x1F, 0x04, 0x1F}, // H (72)
  {0x11, 0x1F, 0x11}, // I (73)
  {0x08, 0x10, 0x0F}, // J (74)
  {0x1F, 0x04, 0x1B}, // K (75)
  {0x1F, 0x10, 0x10}, // L (76)
  {0x1F, 0x02, 0x1F}, // M (77)
  {0x1F, 0x0E, 0x1F}, // N (78)
  {0x0E, 0x11, 0x0E}, // O (79)
  {0x1F, 0x05, 0x02}, // P (80)
  {0x0E, 0x19, 0x1E}, // Q (81)
  {0x1F, 0x05, 0x1A}, // R (82)
  {0x12, 0x15, 0x09}, // S (83)
  {0x01, 0x1F, 0x01}, // T (84)
  {0x0F, 0x10, 0x0F}, // U (85)
  {0x07, 0x18, 0x07}, // V (86)
  {0x1F, 0x08, 0x1F}, // W (87)
  {0x1B, 0x04, 0x1B}, // X (88)
  {0x03, 0x1C, 0x03}, // Y (89)
  {0x19, 0x15, 0x13}, // Z (90)
  {0x00, 0x1F, 0x11}, // [ (91)
  {0x03, 0x04, 0x18}, // backslash (92)
  {0x11, 0x1F, 0x00}, // ] (93)
  {0x02, 0x01, 0x02}, // ^ (94)
  {0x10, 0x10, 0x10}, // _ (95)
  {0x01, 0x02, 0x00}, // ` (96)
  {0x0C, 0x12, 0x1E}, // a (97)
  {0x1F, 0x12, 0x0C}, // b (98)
  {0x0C, 0x12, 0x12}, // c (99)
  {0x0C, 0x12, 0x1F}, // d (100)
  {0x0C, 0x1A, 0x14}, // e (101)
  {0x04, 0x1E, 0x05}, // f (102)
  {0x14, 0x1A, 0x0C}, // g (103) - descender approximated
  {0x1F, 0x02, 0x1C}, // h (104)
  {0x00, 0x1D, 0x00}, // i (105)
  {0x10, 0x1D, 0x00}, // j (106)
  {0x1F, 0x08, 0x14}, // k (107)
  {0x00, 0x1F, 0x10}, // l (108)
  {0x1E, 0x04, 0x1E}, // m (109)
  {0x1E, 0x02, 0x1C}, // n (110)
  {0x0C, 0x12, 0x0C}, // o (111)
  {0x1E, 0x0A, 0x04}, // p (112) - descender approximated
  {0x04, 0x0A, 0x1E}, // q (113) - descender approximated
  {0x1E, 0x02, 0x04}, // r (114)
  {0x14, 0x1E, 0x0A}, // s (115)
  {0x02, 0x0F, 0x12}, // t (116)
  {0x0E, 0x10, 0x1E}, // u (117)
  {0x06, 0x18, 0x06}, // v (118)
  {0x1E, 0x08, 0x1E}, // w (119)
  {0x12, 0x0C, 0x12}, // x (120)
  {0x16, 0x18, 0x06}, // y (121) - descender approximated
  {0x1A, 0x1E, 0x16}, // z (122)
  {0x04, 0x1F, 0x11}, // { (123)
  {0x00, 0x1F, 0x00}, // | (124)
  {0x11, 0x1F, 0x04}, // } (125)
  {0x02, 0x06, 0x04}, // ~ (126)
};

#define CHAR_WIDTH 3      // Each character is 3 pixels wide
#define CHAR_SPACING 1    // 1 pixel gap between characters

// Calculate total width of message in pixels
int getMessageWidth() {
  return currentMessage.length() * (CHAR_WIDTH + CHAR_SPACING);
}

// Convert (row, col) to linear pixel index for zigzag wiring
int getPixelIndex(int row, int col) {
  // Boundary check
  if (row < 0 || row >= MATRIX_HEIGHT || col < 0 || col >= MATRIX_WIDTH) {
    return -1;
  }

  // Zigzag pattern: even rows go left-to-right, odd rows go right-to-left
  if (row % 2 == 0) {
    return row * MATRIX_WIDTH + col;
  } else {
    return row * MATRIX_WIDTH + (MATRIX_WIDTH - 1 - col);
  }
}

// Get a column of pixel data for a character at a specific column offset
uint8_t getCharColumn(char c, int col) {
  // Handle characters outside our font range
  if (c < 32 || c > 126) {
    c = ' ';  // Replace with space
  }

  // Get font index (font starts at ASCII 32)
  int fontIndex = c - 32;

  // Return the column data from PROGMEM
  // Reverse column order to fix horizontal mirroring
  if (col >= 0 && col < CHAR_WIDTH) {
    return pgm_read_byte(&font3x5[fontIndex][CHAR_WIDTH - 1 - col]);
  }
  return 0;  // Return empty for spacing
}

// Get pixel state at a specific position in the scrolling message
bool getPixelFromMessage(int x, int row) {
  // Adjust row for vertical offset (font is centered)
  int fontRow = row - VERTICAL_OFFSET;

  // Return false if outside font height
  if (fontRow < 0 || fontRow >= FONT_HEIGHT) {
    return false;
  }

  // Calculate which character and which column within that character
  int charWidth = CHAR_WIDTH + CHAR_SPACING;
  int charIndex = x / charWidth;
  int columnInChar = x % charWidth;

  // Handle wrapping for continuous scroll
  int messageLen = currentMessage.length();
  if (messageLen == 0) return false;

  charIndex = charIndex % messageLen;
  if (charIndex < 0) charIndex += messageLen;

  // If we're in the spacing area, return false
  if (columnInChar >= CHAR_WIDTH) {
    return false;
  }

  // Get the character
  char c = currentMessage.charAt(charIndex);

  // Get column data for this character
  uint8_t columnData = getCharColumn(c, columnInChar);

  // Check if the specific row bit is set
  return (columnData >> fontRow) & 0x01;
}

// Draw the current frame of scrolling text
void drawFrame() {
  matrix.clear();

  int messageWidth = getMessageWidth();
  if (messageWidth == 0) return;

  // Draw each pixel on the matrix
  for (int col = 0; col < MATRIX_WIDTH; col++) {
    // Calculate which position in the message this column represents
    // Add messageWidth to handle negative scroll positions
    int messageX = ((scrollPosition + col) % messageWidth + messageWidth) % messageWidth;

    for (int row = 0; row < MATRIX_HEIGHT; row++) {
      int pixelIndex = getPixelIndex(row, col);
      if (pixelIndex >= 0) {
        if (getPixelFromMessage(messageX, row)) {
          matrix.setPixelColor(pixelIndex, textColor);
        } else {
          matrix.setPixelColor(pixelIndex, backgroundColor);
        }
      }
    }
  }

  matrix.show();
}

// Handle incoming serial data
void handleSerialInput() {
  while (Serial.available()) {
    char c = Serial.read();

    // Check for end of line
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        // Keep original case - don't convert to uppercase

        // Reverse the string for right-to-left scrolling
        String reversed = " ";  // Start with space for gap
        for (int i = inputBuffer.length() - 1; i >= 0; i--) {
          reversed += inputBuffer.charAt(i);
        }
        currentMessage = reversed;

        // Reset scroll position to start from right edge
        scrollPosition = MATRIX_WIDTH;

        Serial.print("Now displaying: ");
        Serial.println(inputBuffer);  // Show original message
        Serial.println("Enter new message anytime...");

        inputBuffer = "";
      }
    } else {
      // Add character to buffer
      inputBuffer += c;
    }
  }
}

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect
  }

  // Initialize the NeoPixel matrix
  matrix.begin();
  matrix.setBrightness(30);  // Low brightness for USB power
  matrix.clear();
  matrix.show();

  // Set colors after matrix.begin()
  textColor = matrix.Color(0, 255, 0);      // Green text
  backgroundColor = matrix.Color(0, 0, 0);   // Black background

  // Print welcome message
  Serial.println("================================");
  Serial.println("  NeoPixel Scrolling Text");
  Serial.println("  Small Font Version (3x5)");
  Serial.println("================================");
  Serial.println();
  Serial.println("Type a message and press Enter");
  Serial.println("to display it on the LED matrix.");
  Serial.println();
  Serial.println("Features:");
  Serial.println("  - Lowercase preserved");
  Serial.println("  - Compact 3x5 font");
  Serial.println("  - +/- to adjust speed");
  Serial.println();
  Serial.println("Default message: hello");
  Serial.println();
}

void loop() {
  // Handle serial input
  handleSerialInput();

  // Check for speed adjustment commands
  if (Serial.available()) {
    char c = Serial.peek();
    if (c == '+') {
      Serial.read();
      scrollSpeed = max(20, scrollSpeed - 20);
      Serial.print("Speed increased. Delay: ");
      Serial.print(scrollSpeed);
      Serial.println("ms");
    } else if (c == '-') {
      Serial.read();
      scrollSpeed = min(500, scrollSpeed + 20);
      Serial.print("Speed decreased. Delay: ");
      Serial.print(scrollSpeed);
      Serial.println("ms");
    }
  }

  // Update scroll position at regular intervals (non-blocking)
  if (millis() - lastScrollTime >= scrollSpeed) {
    lastScrollTime = millis();

    // Move scroll position (decrement for right-to-left scrolling)
    scrollPosition--;

    // Wrap around when we've scrolled the entire message
    int messageWidth = getMessageWidth();
    if (scrollPosition < -messageWidth) {
      scrollPosition = 0;
    }

    // Draw the current frame
    drawFrame();
  }
}
