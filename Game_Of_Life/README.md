# Conway's Game of Life - 8x8 LED Matrix

Experience the mesmerizing beauty of cellular automaton evolution on your NeoPixel matrix!

## What is Game of Life?

Conway's Game of Life is a classic "zero-player game" - a cellular automaton where simple rules create complex, evolving patterns. It was invented by mathematician John Conway in 1970 and has fascinated programmers and mathematicians ever since.

### The Rules (Simple!)

1. **Survival**: A live cell with 2 or 3 neighbors stays alive
2. **Birth**: A dead cell with exactly 3 neighbors becomes alive
3. **Death**: All other cells die (loneliness or overcrowding)

That's it! From these three rules, complex patterns emerge: oscillators, spaceships, still lifes, and chaotic evolution.

## Features

### 🎨 Multiple Color Modes

Press **M** to cycle through visualization modes:

- **NEIGHBORS**: Colors show how crowded each cell is
  - Purple = lonely (0-1 neighbors)
  - Blue = stable (2 neighbors)
  - Green = perfect (3 neighbors)
  - Yellow/Orange/Red = overcrowded (4+ neighbors)

- **AGE**: Colors show how long cells have been alive
  - Blue = newborn (just appeared)
  - Cyan = young (2-4 generations)
  - Green = middle-aged (5-9 generations)
  - Yellow = old (10-19 generations)
  - Orange/Red = ancient (20+ generations)

- **CLASSIC**: Traditional green cells on black background

- **RAINBOW**: Psychedelic rainbow colors

### 📊 Built-in Patterns

Load famous Game of Life patterns instantly:

**Glider** (G)
- Travels diagonally across the matrix
- Period: 4 generations
- Classic "spaceship" that moves

**Blinker** (B)
- Simple oscillator
- Period: 2 generations
- Flips between horizontal and vertical

**Pulsar** (P)
- Complex oscillator (modified for 8x8)
- Creates pulsing symmetric pattern
- Beautiful to watch

**Lightweight Spaceship** (L)
- Travels horizontally
- Period: 4 generations
- More complex than glider

**Random** (R)
- Generates random starting pattern
- Different density each time (30-60%)
- Often creates interesting emergent behavior

### 🎮 Interactive Controls

**Pattern Selection:**
- `R` - Random pattern
- `G` - Glider
- `B` - Blinker
- `P` - Pulsar
- `L` - Lightweight Spaceship

**Playback Control:**
- `SPACE` - Pause/Resume
- `F` - Faster (decrease delay by 50ms)
- `S` - Slower (increase delay by 50ms)
- `M` - Change color mode

**Utilities:**
- `C` - Clear grid
- `I` - Show statistics
- `H` - Show help

### 📈 Statistics Tracking

Press **I** to see:
- Current generation number
- Population count
- Total cells born (all-time)
- Total cells died (all-time)
- Current speed
- Running/paused status

### 🤖 Auto-Management

The sketch automatically handles:
- **Extinction**: Generates new random pattern if population dies
- **Stability detection**: Resets after 20 generations of no change
- **Toroidal topology**: Edges wrap around (cells on the edge interact with opposite edge)

## How to Use

### Quick Start

1. Upload sketch to Arduino
2. Open Serial Monitor (115200 baud)
3. Watch the random pattern evolve
4. Press **H** for help anytime

### Recommended Workflow

1. **Learn the rules**: Press **M** until "NEIGHBORS" mode is active
   - Watch how green (3 neighbors) cells create new life
   - See how overcrowded (red/orange) cells die

2. **Explore patterns**: Try each preset pattern
   - Press **G** for gliders - watch them travel
   - Press **B** for blinkers - see them oscillate
   - Press **P** for pulsars - enjoy the symmetry

3. **Experiment with speed**:
   - Press **F** repeatedly for super-fast evolution
   - Press **S** to slow down and study patterns

4. **Create chaos**: Press **R** repeatedly to generate random patterns
   - Some evolve into stable patterns
   - Some oscillate forever
   - Some die out completely
   - Some create "Garden of Eden" patterns

### Tips for Observation

- **Gliders**: Look for small patterns that move diagonally
- **Blinkers**: Watch for patterns that flip back and forth
- **Still lifes**: Patterns that never change (blocks, boats, beehives)
- **Methuselahs**: Small patterns that evolve for many generations

## Understanding the Serial Output

Each generation prints:
```
Gen 42 | Pop: 18 | Born: 3 | Died: 2 | Speed: 300ms
```

- **Gen**: Generation number (how many iterations)
- **Pop**: Current living cells
- **Born**: Cells that became alive this generation
- **Died**: Cells that died this generation
- **Speed**: Milliseconds between updates

## Famous Patterns (That Fit on 8x8)

### Stable Patterns (Still Lifes)
These never change once formed:
- Block (2x2 square)
- Beehive
- Loaf
- Boat

### Oscillators
These repeat after N generations:
- Blinker (period 2)
- Toad (period 2)
- Beacon (period 2)
- Pulsar (period 3)

### Spaceships
These travel across the grid:
- Glider (moves diagonally)
- Lightweight Spaceship (moves horizontally)

## Customization Ideas

### Modify Update Speed Range
```cpp
// In handleInput(), change speed limits:
updateDelay = max(20, updateDelay - 50);   // Faster: min 20ms
updateDelay = min(2000, updateDelay + 50); // Slower: max 2 seconds
```

### Change Starting Density
```cpp
// In randomPattern(), adjust density:
randomPattern(70);  // 70% full (more crowded)
randomPattern(20);  // 20% full (more sparse)
```

### Create Your Own Pattern
Add this to the code:
```cpp
void myCustomPattern() {
  clearGrid();

  // Add your cells (row, col)
  grid[3][3] = 1;
  grid[3][4] = 1;
  grid[4][3] = 1;
  grid[4][4] = 1;  // Creates a 2x2 block (stable)

  // Initialize cell ages
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      cellAge[r][c] = grid[r][c];
    }
  }

  drawGrid();
}
```

Then add to `handleInput()`:
```cpp
case 'x':
case 'X':
  myCustomPattern();
  break;
```

### Different Boundary Conditions

Currently uses **toroidal** (wrap-around) boundaries. To use **fixed** boundaries instead:

```cpp
// In countNeighbors(), replace:
int r = (row + dr + ROWS) % ROWS;
int c = (col + dc + COLS) % COLS;

// With:
int r = row + dr;
int c = col + dc;
if (r < 0 || r >= ROWS || c < 0 || c >= COLS) continue;
```

## Troubleshooting

**Pattern dies immediately:**
- Try different random patterns (press R multiple times)
- Some starting configurations are unstable
- Try preset patterns (G, B, P, L)

**Pattern stabilizes and resets:**
- This is normal! Many patterns eventually stabilize
- The sketch auto-resets after 20 stable generations
- Try faster speeds to see more evolution

**Can't see colors well:**
- Adjust brightness: `matrix.setBrightness(60);` in setup()
- Try different color modes (press M)

**Too fast/slow:**
- Press F/S to adjust speed
- Default is 300ms, range is 50ms to 1000ms

## Learn More

### Online Resources
- [Conway's Game of Life - Wikipedia](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
- [LifeWiki](https://conwaylife.com/wiki/) - Comprehensive pattern database
- [Online simulator](https://playgameoflife.com/) - Experiment with larger grids

### Interesting Facts
- Game of Life is **Turing complete** (can compute anything!)
- Some patterns grow forever
- No one has found a perfect algorithm to predict if a pattern will die
- Used in computer science to study complexity and emergence

## Next Steps

After exploring Game of Life:
1. Try implementing different cellular automata rules
2. Add button control to manually place cells
3. Create a "paint mode" to draw starting patterns
4. Implement Brian's Brain or other CA variants
5. Connect multiple matrices for larger simulations

Have fun watching evolution unfold! 🌱🧬✨
