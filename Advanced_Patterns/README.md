# Advanced NeoPixel Patterns

Entertaining and mesmerizing visual effects for your 8x8 LED matrix!

## What's Included

This sketch cycles through 8 different patterns, each running for 20 seconds before transitioning to the next. Each pattern demonstrates different programming techniques and creates unique visual experiences.

### Pattern List

1. **Conway's Game of Life** 🧬
   - Classic cellular automaton simulation
   - Cells live, die, and reproduce based on neighbor count
   - Colors indicate number of neighbors
   - Automatically resets if population dies out

2. **Fire Effect** 🔥
   - Realistic fire simulation rising from the bottom
   - Heat diffusion and cooling algorithm
   - Smooth color gradient: black → red → orange → yellow → white
   - Random intensity variations create flickering

3. **Bouncing Balls** ⚽
   - Physics simulation with gravity and damping
   - 4 colorful balls bouncing around the matrix
   - Realistic collision detection and response
   - Each ball loses energy on impact

4. **Matrix Digital Rain** 💚
   - Inspired by "The Matrix" movie
   - Green cascading characters effect
   - Trails fade as drops fall
   - Random drop speeds and positions

5. **Plasma Wave** 🌊
   - Mathematical sine wave interference patterns
   - Multiple overlapping waves create organic movement
   - Full rainbow color spectrum
   - Hypnotic and smooth animations

6. **Ripple Effect** 💧
   - Water ripple simulation
   - Multiple ripples spawn from random points
   - Expanding circles with fading intensity
   - Blue-cyan gradient colors

7. **Starfield** ⭐
   - 3D starfield flying toward viewer
   - Stars appear distant and move closer
   - Speed variation creates depth perception
   - White stars with brightness based on distance

8. **DNA Helix** 🧬
   - Rotating double helix structure
   - Two complementary strands (blue and pink)
   - Connecting "rungs" between strands
   - Smooth continuous rotation

## How to Use

1. **Upload the sketch** to your Arduino
2. **Open Serial Monitor** (115200 baud) to see which pattern is playing
3. **Watch the show!** Each pattern runs for 20 seconds
4. The cycle repeats infinitely

## Hardware Requirements

- Arduino (any model)
- 8x8 NeoPixel LED Matrix (WS2812B)
- 5V power supply (recommended for full brightness)
- Standard wiring (see main README.md)

## Configuration

Adjust these settings at the top of the sketch:

```cpp
#define PIN 6                               // Data pin
#define PATTERN_DURATION 20000              // Time per pattern (ms)
matrix.setBrightness(40);                   // Brightness (0-255)
```

## Pattern Customization Ideas

### Modify Existing Patterns

**Game of Life:**
- Change initial density: `grid[r][c] = random(2);` → `random(10) < 3` for sparser
- Adjust speed: Change `200` in `if (millis() - lastLifeUpdate >= 200)`

**Fire:**
- Hotter fire: Increase `random(200, 255)` to `random(220, 255)`
- Slower fire: Change cooling `random(0, 10)` to `random(0, 5)`

**Bouncing Balls:**
- More balls: Increase array size `Ball balls[4]` to `Ball balls[6]`
- Stronger gravity: Increase `GRAVITY` from `0.15` to `0.25`
- Bouncier: Increase `DAMPING` from `0.85` to `0.95`

**Plasma:**
- Faster waves: Increase time multipliers in `sin()` functions
- Different colors: Modify `wheel(colorIndex)` calculation

## Technical Notes

### Memory Usage
These patterns use more RAM than basic examples due to:
- Grid arrays for Game of Life (128 bytes)
- Heat map for fire effect (64 bytes)
- Ball/star/ripple structures

If you get memory issues on smaller Arduinos, reduce the number of stars/balls/ripples.

### Performance
All patterns are optimized for 8x8 matrices. Update rates vary:
- Game of Life: 5 FPS (200ms)
- Fire: 20 FPS (50ms)
- Bouncing Balls: 33 FPS (30ms)
- Matrix Rain: 12 FPS (80ms)
- Plasma: 50 FPS (20ms)
- Ripple: 10 FPS (100ms)
- Starfield: 20 FPS (50ms)
- DNA Helix: 20 FPS (50ms)

### Algorithm Insights

**Game of Life Rules:**
- Live cell with 2-3 neighbors: survives
- Dead cell with exactly 3 neighbors: becomes alive
- All other cells: die or stay dead

**Fire Simulation:**
1. Cool each pixel slightly
2. Add heat at bottom
3. Heat rises and diffuses to neighbors
4. Color mapped from heat value

**Physics (Bouncing Balls):**
- Velocity changes position each frame
- Gravity constantly accelerates downward
- Wall collisions reverse velocity with energy loss

## Troubleshooting

**Pattern looks wrong:**
- Check `getPixelIndex()` matches your matrix wiring pattern
- Try different zigzag configurations if needed

**Too bright/dim:**
- Adjust `matrix.setBrightness()` in `setup()`
- Modify individual color values in patterns

**Too fast/slow:**
- Change delay values in each pattern's timing checks
- Modify `PATTERN_DURATION` to change cycle speed

**Choppy animation:**
- Ensure external power supply is adequate
- Lower brightness if underpowered

## Next Steps

Try combining patterns or creating your own:
- Mix fire with ripples for a "lava" effect
- Add Game of Life rules to other patterns
- Create interactive versions with sensors or buttons
- Sync to music using a microphone module

Have fun exploring these patterns! 🎨✨
