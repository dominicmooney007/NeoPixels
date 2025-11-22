# Pi Pico NeoPixel Library Reference

A beginner-friendly guide to using the `pi_pico_neopixel` library with Raspberry Pi Pico and WS2812B LEDs.

**Library:** [github.com/blaz-r/pi_pico_neopixel](https://github.com/blaz-r/pi_pico_neopixel)

---

## Installation

1. Download `neopixel.py` from the GitHub repository
2. Connect your Pico to your computer
3. Open Thonny IDE
4. Open the downloaded `neopixel.py` file
5. Go to **File > Save As** and select **MicroPython device**
6. Save with the filename `neopixel.py`

The library is now installed on your Pico.

---

## Quick Start

```python
from neopixel import Neopixel

# Create a strip with 64 LEDs on GPIO pin 0
pixels = Neopixel(64, 0, 0, "GRB")

# Set brightness (0-255)
pixels.brightness(30)

# Set first pixel to red
pixels.set_pixel(0, (255, 0, 0))

# Update the LEDs
pixels.show()
```

---

## Constructor

```python
pixels = Neopixel(num_leds, state_machine, pin, mode)
```

| Parameter       | Type   | Description                              |
|-----------------|--------|------------------------------------------|
| `num_leds`      | int    | Total number of LEDs                     |
| `state_machine` | int    | PIO state machine ID (use `0`)           |
| `pin`           | int    | GPIO pin number connected to data line   |
| `mode`          | string | Color order of your LEDs                 |

### Color Mode Options

| Mode     | Use For                        |
|----------|--------------------------------|
| `"GRB"`  | Most WS2812B strips (default)  |
| `"RGB"`  | Some older strips              |
| `"GRBW"` | SK6812 RGBW strips             |
| `"RGBW"` | Some RGBW strips               |

If your colors look wrong, try a different mode.

---

## API Reference

### Setting Pixels

#### `set_pixel(index, color)`

Set a single LED to a color.

```python
# Set pixel 0 to red
pixels.set_pixel(0, (255, 0, 0))

# Set pixel 5 to green
pixels.set_pixel(5, (0, 255, 0))

# Set pixel 10 to blue
pixels.set_pixel(10, (0, 0, 255))
```

#### `set_pixel_line(start, end, color)`

Set a range of LEDs to the same color. Both start and end are inclusive.

```python
# Set pixels 0 through 7 to yellow
pixels.set_pixel_line(0, 7, (255, 255, 0))
```

#### `set_pixel_line_gradient(start, end, color1, color2)`

Create a smooth gradient between two colors.

```python
# Gradient from red to blue across pixels 0-15
pixels.set_pixel_line_gradient(0, 15, (255, 0, 0), (0, 0, 255))
```

#### `fill(color)`

Set all LEDs to the same color.

```python
# Fill all pixels with purple
pixels.fill((128, 0, 128))

# Clear all pixels (turn off)
pixels.fill((0, 0, 0))
```

---

### Updating the Display

#### `show()`

Send the color data to the LEDs. **You must call this after making changes.**

```python
pixels.set_pixel(0, (255, 0, 0))
pixels.show()  # LEDs won't change without this!
```

---

### Brightness

#### `brightness(level)`

Set the global brightness level.

```python
pixels.brightness(30)   # Low brightness (good for testing)
pixels.brightness(100)  # Medium brightness
pixels.brightness(255)  # Maximum brightness
```

**Tip:** Start with low brightness (30) when testing. Higher brightness draws more power.

---

### HSV Colors

#### `colorHSV(hue, saturation, value)`

Convert HSV color values to an RGB tuple. Useful for rainbow effects.

| Parameter    | Range     | Description                    |
|--------------|-----------|--------------------------------|
| `hue`        | 0-65535   | Color wheel position           |
| `saturation` | 0-255     | Color intensity (255 = vivid)  |
| `value`      | 0-255     | Brightness (255 = brightest)   |

```python
# Create a rainbow effect
for i in range(64):
    hue = int(i * 65535 / 64)
    color = pixels.colorHSV(hue, 255, 150)
    pixels.set_pixel(i, color)
pixels.show()
```

**Common Hue Values:**

| Hue   | Color   |
|-------|---------|
| 0     | Red     |
| 10922 | Orange  |
| 21845 | Yellow  |
| 32768 | Green   |
| 43690 | Cyan    |
| 54613 | Blue    |
| 60000 | Purple  |

---

## 8x8 Matrix Setup

For an 8x8 LED matrix, you need to convert row/column coordinates to LED indices.

### Zigzag Wiring Pattern

Most matrices use a zigzag wiring pattern:

```
Row 0: LED 0  → LED 1  → LED 2  → ... → LED 7   (left to right)
Row 1: LED 15 ← LED 14 ← LED 13 ← ... ← LED 8   (right to left)
Row 2: LED 16 → LED 17 → LED 18 → ... → LED 23  (left to right)
...
```

### Coordinate Helper Function

```python
def get_pixel_index(row, col):
    """
    Convert (row, col) coordinates to LED index.
    Assumes zigzag wiring pattern.

    Args:
        row: 0-7 (top to bottom)
        col: 0-7 (left to right)

    Returns:
        LED index (0-63)
    """
    if row % 2 == 0:
        return row * 8 + col        # Even rows: left to right
    else:
        return row * 8 + (7 - col)  # Odd rows: right to left
```

### Usage Example

```python
# Set top-left corner to red
pixels.set_pixel(get_pixel_index(0, 0), (255, 0, 0))

# Set bottom-right corner to blue
pixels.set_pixel(get_pixel_index(7, 7), (0, 0, 255))

# Set center pixel to green
pixels.set_pixel(get_pixel_index(3, 3), (0, 255, 0))

pixels.show()
```

---

## Complete Starter Template

Save this as `main.py` on your Pico to run automatically on boot.

```python
from neopixel import Neopixel
from time import sleep

# ============================================
# CONFIGURATION
# ============================================
PIN = 0              # GPIO pin for data line
ROWS = 8             # Matrix rows
COLS = 8             # Matrix columns
NUMPIXELS = ROWS * COLS
BRIGHTNESS = 30      # 0-255

# ============================================
# INITIALIZATION
# ============================================
pixels = Neopixel(NUMPIXELS, 0, PIN, "GRB")
pixels.brightness(BRIGHTNESS)

# ============================================
# HELPER FUNCTIONS
# ============================================
def get_pixel_index(row, col):
    """Convert (row, col) to LED index for zigzag wiring."""
    if row % 2 == 0:
        return row * 8 + col
    else:
        return row * 8 + (7 - col)

def clear():
    """Turn off all pixels."""
    pixels.fill((0, 0, 0))
    pixels.show()

def set_matrix_pixel(row, col, color):
    """Set a pixel using matrix coordinates."""
    pixels.set_pixel(get_pixel_index(row, col), color)

# ============================================
# COLORS
# ============================================
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
WHITE = (255, 255, 255)
OFF = (0, 0, 0)

# ============================================
# MAIN PROGRAM
# ============================================
print("NeoPixel 8x8 Matrix Ready!")
clear()

while True:
    # Your code here

    # Example: Light up diagonal
    clear()
    for i in range(8):
        set_matrix_pixel(i, i, RED)
    pixels.show()
    sleep(1)

    # Example: Fill with color
    pixels.fill(BLUE)
    pixels.show()
    sleep(1)
```

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| No LEDs light up | Check wiring and GPIO pin number |
| Wrong colors | Try different mode: `"RGB"` instead of `"GRB"` |
| LEDs in wrong positions | Verify zigzag pattern matches your matrix wiring |
| Code doesn't run | Must run from file, not REPL interpreter |
| Erratic behavior | Add common ground between Pico and power supply |
| Dim or flickering | Use external 5V power for more than ~10 LEDs |

---

## Power Requirements

| LED Count | Current Draw (max) | Power Source          |
|-----------|--------------------|-----------------------|
| 8         | ~480mA             | USB (with low brightness) |
| 64        | ~3.8A              | External 5V supply    |

**Rule of thumb:** Each LED draws up to 60mA at full white brightness. At brightness level 30, this drops to about 7mA per LED.

---

## Additional Resources

- [Library GitHub](https://github.com/blaz-r/pi_pico_neopixel)
- [MicroPython Documentation](https://docs.micropython.org/)
- [Raspberry Pi Pico Pinout](https://datasheets.raspberrypi.com/pico/Pico-R3-A4-Pinout.pdf)
