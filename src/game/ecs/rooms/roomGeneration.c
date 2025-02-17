#include "roomGeneration.h"


void generate_room(int rX, int rY, uint32_t player_id) {
    int start_x = rX * 1920; int start_y = rY * 1080;
    int max_x = start_x + 1920;
    int max_y = start_y + 1080;
    
    // Fixed door positions - centered on each wall
    int north_door = 14; // (1920/64)/2 - 1 = 14 (center of north wall)
    int south_door = 14; // Same as north for symmetry
    int east_door = 8;   // (1080/64)/2 - 1 = 8 (center of east wall)
    int west_door = 8;   // Same as east for symmetry
    
    // Add outer walls with fixed doors
    for(int x = 0; x < 30; x++) {
        if(x < north_door || x >= north_door + 2) {
            add_block(start_x + x * 64, start_y, get_sprites()->cobble_texture);
        }
        if(x < south_door || x >= south_door + 2) {
            add_block(start_x + x * 64, start_y + 1024, get_sprites()->cobble_texture);
        }
    }
    for(int y = 0; y < 17; y++) {
        if(y < west_door || y >= west_door + 2) {
            add_block(start_x, start_y + y * 64, get_sprites()->cobble_texture);
        }
        if(y < east_door || y >= east_door + 2) {
            add_block(start_x + 1856, start_y + y * 64, get_sprites()->cobble_texture);
        }
    }
    
    // Add a cluster of items rather than just one
    int num_items = rand() % 4 + 1; // 1-4 items in a cluster
    for(int i = 0; i < num_items; i++) {
        int item_x = start_x + ((2 + rand() % 24) * 64);
        int item_y = start_y + ((2 + rand() % 14) * 64);
        if(item_x < max_x && item_y < max_y) {
            // Randomly choose between different item types
            if(rand() % 2 == 0) {
                add_item_entity(item_x, item_y, apple, -1, false);
            } else {
                add_chest(item_x, item_y);
            }
        }
    }
    
    // Add slimes with varying behavior
    int num_slimes = rand() % 5 + 2; // 2-6 slimes for more challenge
    for(int i = 0; i < num_slimes; i++) {
        int enemy_x = start_x + ((2 + rand() % 24) * 64);
        int enemy_y = start_y + ((2 + rand() % 14) * 64);
        if(enemy_x < max_x && enemy_y < max_y) {
            int enemy_type = rand() % 3;
            switch(enemy_type) {
                case 0:
                    add_slime(enemy_x, enemy_y, player_id);
                    break;
                case 1:
                    add_alien(enemy_x, enemy_y, player_id);
                    break;
                case 2:
                    add_goblin(enemy_x, enemy_y, player_id);
                    break;
            }
        }
    }
    
    // Create maze-like structures instead of simple pillars
    int num_maze_segments = rand() % 6 + 3; // 3-8 maze segments
    for(int i = 0; i < num_maze_segments; i++) {
        int maze_x = start_x + ((2 + rand() % 24) * 64);
        int maze_y = start_y + ((2 + rand() % 14) * 64);
        
        if(maze_x + (4 * 64) < max_x && maze_y + (4 * 64) < max_y) {
            // Create random zigzag patterns
            int length = rand() % 4 + 2;
            for(int j = 0; j < length; j++) {
                if(rand() % 2 == 0) {
                    add_block(maze_x + j * 64, maze_y + j * 64, get_sprites()->cobble_texture);
                } else {
                    add_block(maze_x + j * 64, maze_y + (length-j) * 64, get_sprites()->cobble_texture);
                }
            }
        }
    }
}