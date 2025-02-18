#include "roomGeneration.h"

void generate_walls(int rX, int rY, uint32_t player_id, int layout_type) {
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
}

void generate_obstacles(int rX, int rY, uint32_t player_id, int layout_type) {
    int start_x = rX * 1920; int start_y = rY * 1080;
    int max_x = start_x + 1920;
    int max_y = start_y + 1080;
    
    switch(layout_type) {
        case 0: // Cross pattern
            for(int i = 0; i < 20; i++) {
                // Horizontal line
                add_block(start_x + (5 + i) * 64, start_y + 8 * 64, get_sprites()->cobble_texture);
                // Vertical line
                add_block(start_x + 13 * 64, start_y + i * 64, get_sprites()->cobble_texture);
            }
            break;
            
        case 1: // Circular arena
            for(int angle = 0; angle < 360; angle += 15) {
                float rad = angle * PI / 180.0f;
                if(angle % 180 == 90) continue;
                int x = start_x + (14.5 * 64) + cos(rad) * 500;
                int y = start_y + (8.5 * 64) + sin(rad) * 500;
                add_block(x, y, get_sprites()->cobble_texture);
            }
            break;
            
        case 2: // Maze-like corridors
            for(int i = 0; i < 4; i++) {
                int x = start_x + ((4 + i * 6) * 64);
                for(int y = 2; y < 14; y++) {
                    if(rand() % 2 == 0) {
                        add_block(x, start_y + y * 64, get_sprites()->cobble_texture);
                    }
                }
            }
            break;
            
        case 3: // Pillars and platforms
            for(int i = 0; i < 5; i++) {
                for(int j = 0; j < 3; j++) {
                    add_block(start_x + (4 + i * 6) * 64, start_y + (4 + j * 4) * 64, get_sprites()->cobble_texture);
                }
            }
            break;
            
        case 4: // Diagonal barriers
            for(int i = 0; i < 15; i++) {
                if(rand() % 2 == 0) {
                    add_block(start_x + (2 + i) * 64, start_y + (2 + i) * 64, get_sprites()->cobble_texture);
                } else {
                    add_block(start_x + (17 + i) * 64, start_y + (17 - i) * 64, get_sprites()->cobble_texture);
                }
            }
            break;
    }
}

void generate_enemies(int rX, int rY, uint32_t player_id, int layout_type) {
    int start_x = rX * 1920; int start_y = rY * 1080;
    int max_x = start_x + 1920;
    int max_y = start_y + 1080;

    switch(layout_type) {
        case 0: // Cross pattern - enemies in the corners
            add_goblin(start_x + 4 * 64, start_y + 4 * 64, player_id);
            add_goblin(start_x + 25 * 64, start_y + 4 * 64, player_id);
            add_slime(start_x + 4 * 64, start_y + 12 * 64, player_id);
            add_slime(start_x + 25 * 64, start_y + 12 * 64, player_id);
            break;

        case 1: // Circular arena - enemies in a circle
            add_boss(start_x + 14 * 64, start_y + 8 * 64, player_id);
            break;

        case 2: // Maze-like corridors - enemies at corridor intersections
            for(int i = 1; i < 4; i++) {
                for(int j = 1; j < 3; j++) {
                    if(rand() % 2 == 0) {
                        add_alien(start_x + (6 * i) * 64, start_y + (5 * j) * 64, player_id);
                    }
                }
            }
            break;

        case 3: // Pillars and platforms - enemies between pillars
            for(int i = 0; i < 4; i++) {
                for(int j = 0; j < 2; j++) {
                    if (rand() % 3 == 0) {
                        add_alien(start_x + (7 + i * 6) * 64, start_y + (5 + j * 4) * 64, player_id);
                    } else {
                        add_goblin(start_x + (7 + i * 6) * 64, start_y + (5 + j * 4) * 64, player_id);
                    }
                }
            }
            break;

        case 4: // Diagonal barriers - enemies along the gaps
            for(int i = 0; i < 3; i++) {
                add_slime(start_x + (6 + i * 8) * 64, start_y + (6 + i * 4) * 64, player_id);
                add_goblin(start_x + (22 - i * 8) * 64, start_y + (14 - i * 4) * 64, player_id);
            }
            break;
    }
}

void generate_items(int rX, int rY, uint32_t player_id, int layout_type) {
    int start_x = rX * 1920; int start_y = rY * 1080;
    int max_x = start_x + 1920;
    int max_y = start_y + 1080;
    
    switch(layout_type) {
        case 0:
            add_item_entity(start_x + 14 * 64, start_y + 8 * 64, apple, -1, false);
            break;
        case 1:
            add_chest(start_x + 14 * 64, start_y + 8 * 64);
            break;
        case 2:
            add_item_entity(start_x + 14 * 64, start_y + 8 * 64, apple, -1, false);
            add_chest(start_x + 14 * 64, start_y + 8 * 64);
            break;
    }
}

void generate_room(int rX, int rY, uint32_t player_id) {
    srand(pow(2, rX) * pow(3, rY)); // Use room coordinates to seed RNG

    // Choose a random room layout type
    int layout_type = rand() % 5;

    //generate_walls(rX, rY, player_id, layout_type);
    //generate_obstacles(rX, rY, player_id, layout_type);
    generate_enemies(rX, rY, player_id, layout_type);
    //generate_items(rX, rY, player_id, layout_type);
}
