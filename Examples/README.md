# NeoPixel Example Sketches

These examples are designed for beginners learning to use NeoPixels with Arduino.

## Before You Start

1. Make sure you've installed the **Adafruit NeoPixel** library
2. Wire your NeoPixels correctly (see the main guide)
3. Adjust these settings in each sketch to match YOUR setup:
   ```cpp
   #define PIN 6           // Your data pin
   #define NUMPIXELS 8     // How many LEDs you have
   ```

## Examples in Order

### 01_First_Light
**Start here!** Makes one LED blink red.
- Learn the basic structure
- Understand `setup()` and `loop()`
- See how `show()` works

### 02_All_Colors
Shows different colors on each LED.
- Learn RGB color values
- Control multiple LEDs
- Understand color mixing

### 03_Running_Light
One LED chases along the strip.
- Create animations with loops
- Use `pixels.clear()`
- Control timing with `delay()`

### 04_Rainbow
Display a static rainbow pattern.
- Work with multiple colors
- Create attractive displays
- Combine everything learned

### 05_Fade
Smooth breathing effect.
- Create smooth animations
- Understand brightness control
- Make professional-looking effects

## How to Use These Examples

1. Open the example in Arduino IDE
2. **Change PIN and NUMPIXELS** to match your setup
3. Upload to your Arduino
4. Open Serial Monitor (Tools → Serial Monitor) to see messages
5. Read the comments in the code to understand what's happening
6. Try the experiments suggested at the bottom of each sketch

## Need Help?

- Check the troubleshooting section in the main guide
- Make sure your wiring is correct
- Verify you have common ground (Arduino GND to Power Supply GND)
- Try lowering brightness if LEDs are flickering
- Check that you're using the right color order (NEO_GRB vs NEO_RGB)

## Next Steps

After completing these examples:
- Combine effects (fade + chase, rainbow + rotate)
- Add sensors (light sensor, buttons, motion)
- Explore the advanced examples in the Adafruit library
- Check out the Snake game and other projects in this repository

Happy coding!
