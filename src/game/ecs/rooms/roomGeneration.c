#include "roomGeneration.h"

int layer2[] = {37, 101, 96, 7, 10};
int layer2_len = 5;

void parse_tiled_map(int rX, int rY, uint32_t player_id, int layout_type) {
    int start_x = rX * 1920;
    int start_y = rY * 1280;

    // Open the tile map file
    char map_name[50];
    sprintf(map_name, "assets/maps/tiled/test_Tile Layer 2.csv");
    FILE *map_file = fopen(map_name, "r");
    if (!map_file) {
        printf("Failed to open map file\n");
        return;
    }

    // Open the collision map file
    char collision_name[50];
    sprintf(collision_name, "assets/maps/tiled/test_collisions.csv");
    FILE *collision_file = fopen(collision_name, "r");
    if (!collision_file) {
        printf("Failed to open collision file\n");
        fclose(map_file); // Close the map file before exiting
        return;
    }

    char line_map[200];
    char line_collision[200];
    int y = 0;

    // Read both files line by line
    while (fgets(line_map, sizeof(line_map), map_file) &&
           fgets(line_collision, sizeof(line_collision), collision_file) &&
           y < 20) {
        char *saveptr_map, *saveptr_collision;
        char *map_token = strtok_r(line_map, ",", &saveptr_map);
        char *collision_token = strtok_r(line_collision, ",", &saveptr_collision);
        int x = 0;

        // Process each token (tile and collision ID) in the current line
        while (map_token && collision_token && x < 30) {
            int tile_id = atoi(map_token);
            int collision_id = atoi(collision_token);

            if (tile_id != -1) { // Skip empty tiles
                int world_x = start_x + (x * 64);
                int world_y = start_y + (y * 64);
                int tile_x = tile_id % 12; // Assuming 12 columns in the tileset
                int tile_y = tile_id / 12;
                bool has_collision = (collision_id != -1); // Collision data determines flag
                bool is_layer2 = false;
                for(int i = 0; i < layer2_len; i++) {
                    if(layer2[i] == tile_id) {
                        is_layer2 = true;
                        break;
                    }
                }

                add_tile(world_x, world_y, tile_x, tile_y, get_sprites()->tileset_texture_tiled, has_collision, (is_layer2 ? 2 : 0));
            }

            // Move to the next tokens
            map_token = strtok_r(NULL, ",", &saveptr_map);
            collision_token = strtok_r(NULL, ",", &saveptr_collision);
            x++;
        }
        y++;
    }

    // Cleanup: Close both files
    fclose(map_file);
    fclose(collision_file);
}

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
                add_tile(start_x + i * 64, start_y + j * 64, 0, 0, get_sprites()->tileset_texture, true, 0);
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

    parse_tiled_map(rX, rY, player_id, layout_type);
    generate_enemies(rX, rY, player_id, layout_type);
    generate_items(rX, rY, player_id, layout_type);
}
