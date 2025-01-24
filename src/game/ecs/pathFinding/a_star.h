#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GRID_WIDTH  ((int) ceil(1280 / 64) + 1)
#define GRID_HEIGHT ((int) ceil(720 / 64) + 1)
#define MAX_NODES (GRID_WIDTH * GRID_HEIGHT)

void a_star(int start_x, int start_y, int goal_x, int goal_y, int** path, int* path_length, int** grid);