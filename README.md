# NeoPixel Project Collection

Welcome to your NeoPixel learning and project repository!

## What's in This Folder?

### 📚 Learning Materials

#### **NeoPixel_Beginners_Guide.md**
Complete beginner's guide covering:
- What NeoPixels are and how they work
- Hardware requirements and wiring
- Installing the library
- All essential functions explained
- Color creation and RGB values
- Practical examples with code
- Troubleshooting common issues
- Best practices and tips

**Start here if you're new to NeoPixels!**

### 🎓 Example Sketches (`Examples/` folder)

Progressive examples to learn NeoPixel programming:

1. **01_First_Light** - Make your first LED blink red
2. **02_All_Colors** - Display different colors, learn RGB
3. **03_Running_Light** - Create an LED chaser animation
4. **04_Rainbow** - Show a rainbow across all LEDs
5. **05_Fade** - Smooth breathing/fading effect

Each example includes:
- Clear comments explaining the code
- Serial Monitor output to see what's happening
- Suggested experiments to try
- Progressive difficulty

### 🎮 Project Sketches

#### **Snake_Game/**
Full-featured Snake game for 8x8 LED matrix
- Control with WASD keys via Serial Monitor
- Food collection and scoring
- Progressive difficulty (speeds up)
- Game over detection and restart

#### **Snake_Auto/**
Self-playing Snake with simple AI
- No input required - just watch!
- Automatic food finding
- Auto-restart when game over
- Great for testing and demonstrations

#### **Snake_Debug/**
Diagnostic version for troubleshooting
- Automatic movement
- Serial output for debugging
- Tests LED matrix functionality
- Helps identify wiring issues

#### **NeoPixel_Demo/**
Comprehensive demonstration of NeoPixel capabilities
- 8 different animation effects
- Cycles through all demos automatically
- Shows various techniques:
  - Color fills
  - Rainbow waves
  - Theater chase
  - Spiral patterns
  - Random sparkles
  - Breathing effects
  - Animated patterns

## Quick Start Guide

### For Complete Beginners

1. **Read the guide first**: Open `NeoPixel_Beginners_Guide.md`
2. **Install the library**: Follow the installation section in the guide
3. **Wire your NeoPixels**: Use the wiring diagrams in the guide
4. **Try the examples**: Start with `Examples/01_First_Light`
5. **Work through all examples**: Each builds on previous knowledge

### For Testing Your Setup

1. Upload `Snake_Auto.ino` - should work immediately
2. Open Serial Monitor (115200 baud) to see output
3. Watch the snake move automatically
4. If it works, your setup is correct!

### Quick Troubleshooting

**LEDs don't work?**
- Check power connections (5V and GND)
- Verify common ground (Arduino GND to Power GND)
- Make sure data goes to DIN (not DOUT)
- Increase brightness: `pixels.setBrightness(100);`
- Check that PIN and NUMPIXELS match your setup

**Wrong colors?**
- Try different color orders in code:
  - `NEO_GRB + NEO_KHZ800` (most common)
  - `NEO_RGB + NEO_KHZ800`
  - `NEO_BRG + NEO_KHZ800`

**Only first few LEDs work?**
- Not enough power - use external 5V supply
- Check connections between LED segments
- Verify NUMPIXELS setting is correct

## Hardware Requirements

### Minimum Setup
- Arduino (any model)
- NeoPixel LEDs (any type: strip, ring, matrix)
- USB cable for programming
- Jumper wires

### Recommended Setup
- Arduino Uno R4 WiFi (or similar)
- 8x8 NeoPixel LED Matrix (for games)
- 5V 2A power supply (for external power)
- 1000µF capacitor (power smoothing)
- 470Ω resistor (data line protection)

### Wiring Basics
```
Power Supply 5V ──┬── NeoPixel +5V
                  │
Power Supply GND ─┼── NeoPixel GND
                  │
                  └── Arduino GND (IMPORTANT!)

Arduino Pin 6 ────[470Ω]──── NeoPixel DIN
```

## Customizing the Code

All sketches have these settings at the top:

```cpp
#define PIN 6           // Change to your data pin
#define NUMPIXELS 8     // Change to your LED count
```

**Always adjust these to match YOUR hardware!**

## Project Ideas to Try

After working through the examples, try:

1. **Button-controlled animations** - Use buttons to switch effects
2. **Music visualizer** - Use a microphone to react to sound
3. **Temperature display** - Show temperature with color gradients
4. **Clock display** - Show time on LED ring or matrix
5. **Reaction game** - Light random LEDs, tap buttons to match
6. **Scrolling text** - Display messages on LED matrix
7. **Ambient lighting** - Smooth color transitions for room lighting

## Resources

### Official Documentation
- Adafruit NeoPixel Library: https://github.com/adafruit/Adafruit_NeoPixel
- NeoPixel Überguide: https://learn.adafruit.com/adafruit-neopixel-uberguide
- Arduino Reference: https://docs.arduino.cc/

### Tools
- RGB Color Picker: Use online tools to find RGB values
- LED Power Calculator: Calculate power requirements
- Serial Monitor: View debug output (Tools → Serial Monitor)

### Community
- Arduino Forum: https://forum.arduino.cc/
- Adafruit Forums: https://forums.adafruit.com/
- Reddit: r/arduino, r/led

## File Structure

```
NeoPixels/
├── README.md                          (This file)
├── NeoPixel_Beginners_Guide.md       (Complete tutorial)
│
├── Examples/                          (Learning examples)
│   ├── README.md
│   ├── 01_First_Light/
│   ├── 02_All_Colors/
│   ├── 03_Running_Light/
│   ├── 04_Rainbow/
│   └── 05_Fade/
│
├── Snake_Game/                        (Manual control)
├── Snake_Auto/                        (AI player)
├── Snake_Debug/                       (Diagnostics)
└── NeoPixel_Demo/                     (8 effects demo)
```

## Tips for Success

1. **Start simple**: Don't jump to complex projects first
2. **Test incrementally**: Make small changes and test often
3. **Use Serial Monitor**: See what your code is doing
4. **Manage power**: Start with low brightness
5. **Read the comments**: Code comments explain everything
6. **Experiment**: Change values and see what happens
7. **Join communities**: Ask questions when stuck

## License and Attribution

These examples use the Adafruit NeoPixel library.
Feel free to modify and use this code for your own projects!

---

**Ready to get started?**

1. Open `NeoPixel_Beginners_Guide.md` for the full tutorial
2. Check `Examples/README.md` for the example progression
3. Try `01_First_Light.ino` for your first program

**Have fun making things glow!** 🌈✨
