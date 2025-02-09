#include "a_star.h"

// Directions for neighbors (up, down, left, right, diagonals if allowed)
int neighbor_offsets[4][2] = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1} // Up, down, left, right
    //{-1, -1}, {-1, 1}, {1, -1}, {1, 1} // Diagonals
};

// Node structure
typedef struct node_s {
    int x, y;         // Position
    float g_cost;     // Cost from start to this node
    float h_cost;     // Heuristic cost to the goal
    float f_cost;     // Total cost (g_cost + h_cost)
    int in_open;      // Flag to indicate if it's in the open list
    int in_closed;    // Flag to indicate if it's in the closed list
    struct node_s* parent; // Pointer to parent node for path reconstruction
} Node;

// Allocate a 2D array of nodes
Node** allocate_node_grid() {
    Node** nodes = malloc(GRID_HEIGHT * sizeof(Node*));
    for (int i = 0; i < GRID_HEIGHT; i++) {
        nodes[i] = malloc(GRID_WIDTH * sizeof(Node));
    }
    return nodes;
}

// Free the 2D array of nodes
void free_node_grid(Node** nodes) {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        free(nodes[i]);
    }
    free(nodes);
}

// Heuristic function: Manhattan distance
float calculate_heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Reconstruct path
void reconstruct_path(Node* current, int** path, int* path_length) {
    int max_path = GRID_WIDTH * GRID_HEIGHT;
    *path = malloc(max_path * 2 * sizeof(int));
    *path_length = 0;

    while (current) {
        (*path)[(*path_length) * 2] = current->x;
        (*path)[(*path_length) * 2 + 1] = current->y;
        (*path_length)++;
        current = current->parent;
    }

    // Reverse the path
    for (int i = 0; i < *path_length / 2; i++) {
        int temp_x = (*path)[i * 2];
        int temp_y = (*path)[i * 2 + 1];
        (*path)[i * 2] = (*path)[(*path_length - i - 1) * 2];
        (*path)[i * 2 + 1] = (*path)[(*path_length - i - 1) * 2 + 1];
        (*path)[(*path_length - i - 1) * 2] = temp_x;
        (*path)[(*path_length - i - 1) * 2 + 1] = temp_y;
    }
}

// A* Pathfinding Algorithm
void a_star(int start_x, int start_y, int goal_x, int goal_y, int** path, int* path_length, int** grid) {
    Node** nodes = allocate_node_grid();
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            nodes[y][x] = (Node){x, y, INFINITY, INFINITY, INFINITY, 0, 0, NULL};
        }
    }

    Node* open_list[MAX_NODES];
    int open_count = 0;

    Node* start_node = &nodes[start_y][start_x];
    start_node->g_cost = 0;
    start_node->h_cost = calculate_heuristic(start_x, start_y, goal_x, goal_y);
    start_node->f_cost = start_node->h_cost;
    open_list[open_count++] = start_node;
    start_node->in_open = 1;

    while (open_count > 0) {
        // Find node with lowest f_cost in open list
        int current_index = 0;
        for (int i = 1; i < open_count; i++) {
            if (open_list[i]->f_cost < open_list[current_index]->f_cost) {
                current_index = i;
            }
        }

        Node* current = open_list[current_index];

        // Remove current node from open list
        open_list[current_index] = open_list[--open_count];
        current->in_open = 0;
        current->in_closed = 1;

        // Check if goal is reached
        if (current->x == goal_x && current->y == goal_y) {
            reconstruct_path(current, path, path_length);
            free_node_grid(nodes);
            return;
        }

        // Process neighbors
        for (int i = 0; i < 8; i++) {
            int nx = current->x + neighbor_offsets[i][0];
            int ny = current->y + neighbor_offsets[i][1];

            // Skip out-of-bounds or non-walkable nodes
            if (nx < 0 || ny < 0 || nx >= GRID_WIDTH || ny >= GRID_HEIGHT || grid[ny][nx] == 1) {
                continue;
            }

            Node* neighbor = &nodes[ny][nx];

            if (neighbor->in_closed) continue;

            float tentative_g_cost = current->g_cost + ((i < 4) ? 1 : 1.414);

            if (!neighbor->in_open || tentative_g_cost < neighbor->g_cost) {
                neighbor->g_cost = tentative_g_cost;
                neighbor->h_cost = calculate_heuristic(nx, ny, goal_x, goal_y);
                neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
                neighbor->parent = current;

                if (!neighbor->in_open) {
                    open_list[open_count++] = neighbor;
                    neighbor->in_open = 1;
                }
            }
        }
    }

    // No path found
    *path = NULL;
    *path_length = 0;
    free_node_grid(nodes);
}
