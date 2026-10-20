# Basic micromouse Dijkstra example

Start with `dijkstra.c`. It uses ordinary integer arrays, loops, and one
function to find a route through a known 4×4 maze. No pointers to manage,
dynamic memory, or extra libraries.

This is a standalone desktop example for reviewing the algorithm. It is
separate from the STM32 firmware build; board adaptation is described below.

## Run in VS Code on your computer

Open `examples/dijkstra` in VS Code, then open **Terminal → New Terminal**.
With a C compiler installed, run:

```sh
cc -std=c11 -Wall -Wextra -pedantic dijkstra.c -o dijkstra
./dijkstra
```

If your Mac reports `ld: tapi error: malformed file` and has the macOS 26.5
SDK installed, the following command was used successfully for local checks:

```sh
cc -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX26.5.sdk -std=c11 -Wall -Wextra -pedantic dijkstra.c -o dijkstra
./dijkstra
```

On Windows with GCC installed, use `gcc` instead of `cc`, compile with
`-o dijkstra.exe`, and run `./dijkstra.exe`.
VS Code is the editor; it also needs a compiler to build C.

Expected output:

```text
Shortest distance: 8 moves
Path: 0 -> 4 -> 8 -> 12 -> 13 -> 14 -> 10 -> 11 -> 15
```

## What the code does

The cells are numbered left to right, starting at the top left:

```text
 0   1   2   3
 4   5   6   7
 8   9  10  11
12  13  14  15
```

`walls[cell][direction]` records each cell's four walls in the order
**up, right, down, left**. A `1` means wall; a `0` means opening.
For example, `walls[0][1]` and `walls[1][3]` describe the same wall,
so always change both together. Keep the maze's outside walls closed.

`distance` starts very large everywhere except the start, which is zero.
Dijkstra repeatedly picks the unvisited cell with the smallest distance.
It checks the neighboring cells through openings and updates their
distances if it finds a shorter route. `previous` remembers how it got
there, so the code can build `path` after reaching the goal.

Every move costs 1, so the answer is the route with the fewest cell moves.
This is Dijkstra's algorithm with equal costs; breadth-first search would
also work here. It does not account for turn time or motor speed.

Change `start` and `goal` in `main()` to try other cells. The demo uses
cell 15 as an easy example goal, rather than the center of a competition
maze. This assumes the walls are already known; sensing walls and exploring
an unknown maze are separate work.

## Using it with the team's blue board

The firmware project targets the **STM32F103C8T6**.
At the reference commit, `Src/main.c` has a PC13 LED example and no maze solver.
Reference commit: `705b7a8ff788c530b815ad29aea75d39567f1e0e`.
The algorithm here is new; the GPIO calls below follow that LED example.

For a first board experiment in your existing STM32 VS Code project:

1. Copy the constants, arrays, and `dijkstra()` function into `Src/main.c`.
2. Keep `#include "gpio.h"` and replace `main()` with the version below.
   The computer demo's `stdio.h` include and printing are unnecessary here.
3. Use the team's existing STM32 build and debug setup. Do not add a second
   `main()` or try to flash the executable built by the computer command above.

```c
int main(void)
{
    gpio_init(GPIOC, 13, OUTPUT_2MHZ, GENERAL_PURPOSE_PUSH_PULL);
    gpio_set(GPIOC, 13, GPIO_HIGH);

    int moves = dijkstra(0, 15);
    if (moves != -1) {
        gpio_set(GPIOC, 13, GPIO_LOW);
    }

    for (;;) {
        /* Put a debugger breakpoint here and inspect path and pathLength. */
    }
}
```

This follows the repository's active-low PC13 LED example: finding a route
turns the LED on. `path` contains cell numbers, not motor commands.
The solver uses fixed arrays and no hardware functions. You will still need
wall sensing, position tracking, and motor control before it can drive a mouse.

The computer demo was tested locally. The board version was compiled to a
Cortex-M3 object file using the repository's GPIO header. It has not been
linked into full firmware, flashed, or tested on physical hardware.

## Optional checks

```sh
cc -std=c11 -Wall -Wextra -pedantic test_dijkstra.c -o test_dijkstra
./test_dijkstra
```

If needed on macOS, add the same `-isysroot` option described above.

Checks cover the sample route, reverse route, matching walls, an unreachable
goal, starting at the goal, invalid cell numbers, and an open maze with many
possible routes.
