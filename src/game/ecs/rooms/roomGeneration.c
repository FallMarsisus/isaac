#include "roomGeneration.h"

void parse_map(int rX, int rY, uint32_t player_id, int layout_type) {
    int start_x = rX * 1920; int start_y = rY * 1280;
    int max_x = start_x + 1920;
    int max_y = start_y + 1280;
    
    char map_name[20];
    sprintf(map_name, "assets/maps/map%d", layout_type);
    FILE *map_file = fopen(map_name, "r");
    if (!map_file) {
        printf("Failed to open map file\n");
        return;
    }

    char line[200];
    int j = 0;
    while (fgets(line, sizeof(line), map_file)) {
        for(int i = 0; i < strlen(line); i++) {
            if(line[i] == '1') {
                add_block(start_x + i * 64, start_y + j * 64);
            }
            if(line[i] == '.') {
                add_door(start_x + i * 64, start_y + j * 64);
            }
            if(line[i] == '_') {
                add_trap(start_x + i * 64, start_y + j * 64);
            }
        }
        j++;
    }

    fclose(map_file);
}

void generate_obstacles(int rX, int rY, uint32_t player_id, int layout_type) {
    int start_x = rX * 1920; int start_y = rY * 1280;
    int max_x = start_x + 1920;
    int max_y = start_y + 1280;

    //Room dimensions : 1920x1280
    
    
    /*switch(layout_type) {
        case 0:
            add_door(start_x + 128, start_y + 128);
            break;
            
        case 1:
            add_door(start_x + 128, start_y + 128);
            break;
            
        case 2:
            add_door(start_x + 128, start_y + 128);
            break;
            
        case 3:
            add_door(start_x + 128, start_y + 128);
            break;
            
        case 4:
            add_door(start_x + 128, start_y + 128);
            break;
    }*/
}

void generate_enemies(int rX, int rY, uint32_t player_id, int layout_type) {
    int start_x = rX * 1920; int start_y = rY * 1280;
    int max_x = start_x + 1920;
    int max_y = start_y + 1280;

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
    int start_x = rX * 1920; int start_y = rY * 1280;
    int max_x = start_x + 1920;
    int max_y = start_y + 1280;
    
    switch(layout_type) {
        case 0:
            add_item_entity(start_x + 14 * 64, start_y + 8 * 64, apple, -1, false);
            break;
        case 1:
            //add_chest(start_x + 14 * 64, start_y + 8 * 64);
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

    parse_map(rX, rY, player_id, layout_type);
    generate_obstacles(rX, rY, player_id, layout_type);
    generate_enemies(rX, rY, player_id, layout_type);
    generate_items(rX, rY, player_id, layout_type);
}
