/* Optional checks. The renamed main lets us test the same source file. */
#include <assert.h>
#define main demo_main
#include "dijkstra.c"
#undef main

int main(void)
{
    int expected[] = {0, 4, 8, 12, 13, 14, 10, 11, 15};

    /* Check that the sample maze's openings agree on both sides. */
    for (int cell = 0; cell < CELLS; cell++) {
        if (cell % SIZE < SIZE - 1) {
            assert(walls[cell][1] == walls[cell + 1][3]);
        }
        if (cell / SIZE < SIZE - 1) {
            assert(walls[cell][2] == walls[cell + SIZE][0]);
        }
    }

    assert(dijkstra(0, 15) == 8);
    assert(pathLength == 9);
    for (int i = 0; i < pathLength; i++) {
        assert(path[i] == expected[i]);
    }

    /* The same corridor should work in reverse. */
    assert(dijkstra(15, 0) == 8);
    for (int i = 0; i < pathLength; i++) {
        assert(path[i] == expected[8 - i]);
    }

    /* Close both exits from the start, on both sides of each wall. */
    walls[0][1] = walls[1][3] = 1;
    walls[0][2] = walls[4][0] = 1;
    assert(dijkstra(0, 15) == -1);
    assert(pathLength == 0);

    assert(dijkstra(0, 0) == 0);
    assert(pathLength == 1 && path[0] == 0);
    assert(dijkstra(-1, 15) == -1);
    assert(dijkstra(0, CELLS) == -1);
    assert(pathLength == 0);

    /* Open every internal wall. There are now many routes to compare. */
    for (int cell = 0; cell < CELLS; cell++) {
        walls[cell][0] = (cell / SIZE == 0);
        walls[cell][1] = (cell % SIZE == SIZE - 1);
        walls[cell][2] = (cell / SIZE == SIZE - 1);
        walls[cell][3] = (cell % SIZE == 0);
    }
    assert(dijkstra(0, 15) == 6);
    assert(pathLength == 7);

    printf("All checks passed.\n");
    return 0;
}
