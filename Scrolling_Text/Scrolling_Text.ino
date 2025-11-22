/*
 * Scrolling Text Display for 8x8 NeoPixel Matrix
 *
 * This sketch displays scrolling text messages on an 8x8 WS2812B LED matrix.
 * Users can enter custom messages via Serial Monitor, and the text will
 * scroll continuously from right to left.
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
int scrollSpeed = 100;        // Milliseconds between scroll steps (lower = faster)
unsigned long lastScrollTime = 0;
int scrollPosition = MATRIX_WIDTH;  // Start from right edge

// Message buffer
String currentMessage = " OLLEH";  // Default message reversed for R-to-L scroll
String inputBuffer = "";           // Buffer for incoming serial data

// Text color (can be changed)
uint32_t textColor;
uint32_t backgroundColor;

// 5x8 Font definition - each character is 5 columns wide, 8 rows tall
// Stored as 5 bytes per character, each byte represents one column (LSB = top row)
const uint8_t font5x8[][5] PROGMEM = {
  {0x00, 0x00, 0x00, 0x00, 0x00}, // Space (32)
  {0x00, 0x00, 0x5F, 0x00, 0x00}, // ! (33)
  {0x00, 0x07, 0x00, 0x07, 0x00}, // " (34)
  {0x14, 0x7F, 0x14, 0x7F, 0x14}, // # (35)
  {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $ (36)
  {0x23, 0x13, 0x08, 0x64, 0x62}, // % (37)
  {0x36, 0x49, 0x55, 0x22, 0x50}, // & (38)
  {0x00, 0x05, 0x03, 0x00, 0x00}, // ' (39)
  {0x00, 0x1C, 0x22, 0x41, 0x00}, // ( (40)
  {0x00, 0x41, 0x22, 0x1C, 0x00}, // ) (41)
  {0x08, 0x2A, 0x1C, 0x2A, 0x08}, // * (42)
  {0x08, 0x08, 0x3E, 0x08, 0x08}, // + (43)
  {0x00, 0x50, 0x30, 0x00, 0x00}, // , (44)
  {0x08, 0x08, 0x08, 0x08, 0x08}, // - (45)
  {0x00, 0x60, 0x60, 0x00, 0x00}, // . (46)
  {0x20, 0x10, 0x08, 0x04, 0x02}, // / (47)
  {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0 (48)
  {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1 (49)
  {0x42, 0x61, 0x51, 0x49, 0x46}, // 2 (50)
  {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3 (51)
  {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4 (52)
  {0x27, 0x45, 0x45, 0x45, 0x39}, // 5 (53)
  {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6 (54)
  {0x01, 0x71, 0x09, 0x05, 0x03}, // 7 (55)
  {0x36, 0x49, 0x49, 0x49, 0x36}, // 8 (56)
  {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9 (57)
  {0x00, 0x36, 0x36, 0x00, 0x00}, // : (58)
  {0x00, 0x56, 0x36, 0x00, 0x00}, // ; (59)
  {0x00, 0x08, 0x14, 0x22, 0x41}, // < (60)
  {0x14, 0x14, 0x14, 0x14, 0x14}, // = (61)
  {0x41, 0x22, 0x14, 0x08, 0x00}, // > (62)
  {0x02, 0x01, 0x51, 0x09, 0x06}, // ? (63)
  {0x32, 0x49, 0x79, 0x41, 0x3E}, // @ (64)
  {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A (65)
  {0x7F, 0x49, 0x49, 0x49, 0x36}, // B (66)
  {0x3E, 0x41, 0x41, 0x41, 0x22}, // C (67)
  {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D (68)
  {0x7F, 0x49, 0x49, 0x49, 0x41}, // E (69)
  {0x7F, 0x09, 0x09, 0x01, 0x01}, // F (70)
  {0x3E, 0x41, 0x41, 0x51, 0x32}, // G (71)
  {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H (72)
  {0x00, 0x41, 0x7F, 0x41, 0x00}, // I (73)
  {0x20, 0x40, 0x41, 0x3F, 0x01}, // J (74)
  {0x7F, 0x08, 0x14, 0x22, 0x41}, // K (75)
  {0x7F, 0x40, 0x40, 0x40, 0x40}, // L (76)
  {0x7F, 0x02, 0x04, 0x02, 0x7F}, // M (77)
  {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N (78)
  {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O (79)
  {0x7F, 0x09, 0x09, 0x09, 0x06}, // P (80)
  {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q (81)
  {0x7F, 0x09, 0x19, 0x29, 0x46}, // R (82)
  {0x46, 0x49, 0x49, 0x49, 0x31}, // S (83)
  {0x01, 0x01, 0x7F, 0x01, 0x01}, // T (84)
  {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U (85)
  {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V (86)
  {0x7F, 0x20, 0x18, 0x20, 0x7F}, // W (87)
  {0x63, 0x14, 0x08, 0x14, 0x63}, // X (88)
  {0x03, 0x04, 0x78, 0x04, 0x03}, // Y (89)
  {0x61, 0x51, 0x49, 0x45, 0x43}, // Z (90)
  {0x00, 0x00, 0x7F, 0x41, 0x41}, // [ (91)
  {0x02, 0x04, 0x08, 0x10, 0x20}, // backslash (92)
  {0x41, 0x41, 0x7F, 0x00, 0x00}, // ] (93)
  {0x04, 0x02, 0x01, 0x02, 0x04}, // ^ (94)
  {0x40, 0x40, 0x40, 0x40, 0x40}, // _ (95)
  {0x00, 0x01, 0x02, 0x04, 0x00}, // ` (96)
  {0x20, 0x54, 0x54, 0x54, 0x78}, // a (97)
  {0x7F, 0x48, 0x44, 0x44, 0x38}, // b (98)
  {0x38, 0x44, 0x44, 0x44, 0x20}, // c (99)
  {0x38, 0x44, 0x44, 0x48, 0x7F}, // d (100)
  {0x38, 0x54, 0x54, 0x54, 0x18}, // e (101)
  {0x08, 0x7E, 0x09, 0x01, 0x02}, // f (102)
  {0x08, 0x14, 0x54, 0x54, 0x3C}, // g (103)
  {0x7F, 0x08, 0x04, 0x04, 0x78}, // h (104)
  {0x00, 0x44, 0x7D, 0x40, 0x00}, // i (105)
  {0x20, 0x40, 0x44, 0x3D, 0x00}, // j (106)
  {0x00, 0x7F, 0x10, 0x28, 0x44}, // k (107)
  {0x00, 0x41, 0x7F, 0x40, 0x00}, // l (108)
  {0x7C, 0x04, 0x18, 0x04, 0x78}, // m (109)
  {0x7C, 0x08, 0x04, 0x04, 0x78}, // n (110)
  {0x38, 0x44, 0x44, 0x44, 0x38}, // o (111)
  {0x7C, 0x14, 0x14, 0x14, 0x08}, // p (112)
  {0x08, 0x14, 0x14, 0x18, 0x7C}, // q (113)
  {0x7C, 0x08, 0x04, 0x04, 0x08}, // r (114)
  {0x48, 0x54, 0x54, 0x54, 0x20}, // s (115)
  {0x04, 0x3F, 0x44, 0x40, 0x20}, // t (116)
  {0x3C, 0x40, 0x40, 0x20, 0x7C}, // u (117)
  {0x1C, 0x20, 0x40, 0x20, 0x1C}, // v (118)
  {0x3C, 0x40, 0x30, 0x40, 0x3C}, // w (119)
  {0x44, 0x28, 0x10, 0x28, 0x44}, // x (120)
  {0x0C, 0x50, 0x50, 0x50, 0x3C}, // y (121)
  {0x44, 0x64, 0x54, 0x4C, 0x44}, // z (122)
  {0x00, 0x08, 0x36, 0x41, 0x00}, // { (123)
  {0x00, 0x00, 0x7F, 0x00, 0x00}, // | (124)
  {0x00, 0x41, 0x36, 0x08, 0x00}, // } (125)
  {0x08, 0x08, 0x2A, 0x1C, 0x08}, // -> (126)
};

#define CHAR_WIDTH 5      // Each character is 5 pixels wide
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
    return pgm_read_byte(&font5x8[fontIndex][CHAR_WIDTH - 1 - col]);
  }
  return 0;  // Return empty for spacing
}

// Get pixel state at a specific position in the scrolling message
bool getPixelFromMessage(int x, int row) {
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
  return (columnData >> row) & 0x01;
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
        // Convert to uppercase for better visibility on small matrix
        inputBuffer.toUpperCase();

        // Reverse the string for right-to-left scrolling
        String reversed = " ";  // Start with space for gap
        for (int i = inputBuffer.length() - 1; i >= 0; i--) {
          reversed += inputBuffer.charAt(i);
        }
        currentMessage = reversed;

        // Reset scroll position to start from right edge
        scrollPosition = MATRIX_WIDTH;

        Serial.print("Now displaying: ");
        Serial.println(inputBuffer);  // Show original message, not reversed
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
    ; // Wait for serial port to connect (needed for some boards)
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
  Serial.println("================================");
  Serial.println();
  Serial.println("Type a message and press Enter");
  Serial.println("to display it on the LED matrix.");
  Serial.println();
  Serial.println("Commands:");
  Serial.println("  Type message + Enter = Display");
  Serial.println("  +/- = Adjust scroll speed");
  Serial.println();
  Serial.println("Default message: HELLO");
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
