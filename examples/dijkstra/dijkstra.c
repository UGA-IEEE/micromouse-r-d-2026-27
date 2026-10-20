#include <stdio.h>

#define SIZE 4
#define CELLS (SIZE * SIZE)
#define INF 9999

/* Cell numbers:
    0  1  2  3
    4  5  6  7
    8  9 10 11
   12 13 14 15
*/

/* Each row is one cell. Directions: up, right, down, left.
   1 means a wall, 0 means an opening.
   A wall between two cells must be recorded in BOTH cells. */
int walls[CELLS][4] = {
    {1, 0, 0, 1}, /* 0 */
    {1, 0, 1, 0}, /* 1 */
    {1, 0, 1, 0}, /* 2 */
    {1, 1, 0, 0}, /* 3 */
    {0, 1, 0, 1}, /* 4 */
    {1, 0, 0, 1}, /* 5 */
    {1, 0, 1, 0}, /* 6 */
    {0, 1, 1, 0}, /* 7 */
    {0, 1, 0, 1}, /* 8 */
    {0, 1, 1, 1}, /* 9 */
    {1, 0, 0, 1}, /* 10 */
    {1, 1, 0, 0}, /* 11 */
    {0, 0, 1, 1}, /* 12 */
    {1, 0, 1, 0}, /* 13 */
    {0, 1, 1, 0}, /* 14 */
    {0, 1, 1, 1}  /* 15 */
};

int distance[CELLS]; /* Best distance found so far. */
int visited[CELLS];  /* 1 when a cell's shortest distance is settled. */
int previous[CELLS]; /* Cell we came from on the shortest route. */
int path[CELLS];     /* Final route, from start to goal. */
int pathLength;

/* Returns the number of moves, or -1 if there is no route
   or the start/goal is outside the maze. */
int dijkstra(int start, int goal)
{
    int rowChange[4] = {-1, 0, 1, 0};
    int colChange[4] = {0, 1, 0, -1};
    pathLength = 0;

    for (int i = 0; i < CELLS; i++) {
        distance[i] = INF;
        visited[i] = 0;
        previous[i] = -1;
        path[i] = -1;
    }

    if (start < 0 || start >= CELLS || goal < 0 || goal >= CELLS) {
        return -1;
    }

    distance[start] = 0;

    while (1) {
        int current = -1;
        int smallest = INF;

        /* Pick the closest cell we have not finished yet.
           ponytail: scanning is enough for this small demo;
           use a priority queue if a larger maze needs faster searches. */
        for (int i = 0; i < CELLS; i++) {
            if (visited[i] == 0 && distance[i] < smallest) {
                smallest = distance[i];
                current = i;
            }
        }

        if (current == -1) {
            return -1; /* No more reachable cells. */
        }

        visited[current] = 1;
        if (current == goal) {
            break;
        }

        /* Check the four cells next to the current cell. */
        for (int direction = 0; direction < 4; direction++) {
            int row = current / SIZE + rowChange[direction];
            int col = current % SIZE + colChange[direction];

            if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
                continue;
            }
            if (walls[current][direction] == 1) {
                continue;
            }

            int next = row * SIZE + col;
            int newDistance = distance[current] + 1; /* One move costs 1. */

            if (visited[next] == 0 && newDistance < distance[next]) {
                distance[next] = newDistance;
                previous[next] = current;
            }
        }
    }

    /* Follow previous[] backwards from the goal. */
    int current = goal;
    while (current != -1) {
        path[pathLength] = current;
        pathLength++;
        current = previous[current];
    }

    /* Reverse the route so the start comes first. */
    for (int i = 0; i < pathLength / 2; i++) {
        int temp = path[i];
        path[i] = path[pathLength - 1 - i];
        path[pathLength - 1 - i] = temp;
    }

    return distance[goal];
}

int main(void)
{
    int start = 0;
    int goal = 15;
    int moves = dijkstra(start, goal);

    if (moves == -1) {
        printf("No path found.\n");
        return 0;
    }

    printf("Shortest distance: %d moves\n", moves);
    printf("Path: ");
    for (int i = 0; i < pathLength; i++) {
        printf("%d", path[i]);
        if (i < pathLength - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
    return 0;
}
