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
    
    // Add outer walls with fixed holes in each direction
    for(int x = 0; x < 30; x++) {
        // Skip north door (2 blocks wide hole)
        if(x < north_door || x >= north_door + 2) {
            add_block(start_x + x * 64, start_y, get_sprites()->cobble_texture);
        }
        // Skip south door (2 blocks wide hole)
        if(x < south_door || x >= south_door + 2) {
            add_block(start_x + x * 64, start_y + 1024, get_sprites()->cobble_texture);
        }
    }
    for(int y = 0; y < 17; y++) {
        // Skip west door (2 blocks tall hole)
        if(y < west_door || y >= west_door + 2) {
            add_block(start_x, start_y + y * 64, get_sprites()->cobble_texture);
        }
        // Skip east door (2 blocks tall hole)
        if(y < east_door || y >= east_door + 2) {
            add_block(start_x + 1856, start_y + y * 64, get_sprites()->cobble_texture);
        }
    }
    
    // Add random decorative elements, ensuring they stay within room bounds
    // 29 blocks max (29 * 64 = 1856) to ensure entities stay within bounds
    int item_x = start_x + ((2 + rand() % 24) * 64);
    int item_y = start_y + ((2 + rand() % 14) * 64);
    if(item_x < max_x && item_y < max_y) {
        add_item_entity(item_x, item_y, apple, -1, false);
    }
    
    int chest_x = start_x + ((2 + rand() % 24) * 64);
    int chest_y = start_y + ((2 + rand() % 14) * 64);
    if(chest_x < max_x && chest_y < max_y) {
        add_chest(chest_x, chest_y);
    }
    
    // Add multiple slimes in random positions
    int num_slimes = rand() % 3 + 1; // 1-3 slimes
    for(int i = 0; i < num_slimes; i++) {
        int slime_x = start_x + ((2 + rand() % 24) * 64);
        int slime_y = start_y + ((2 + rand() % 14) * 64);
        if(slime_x < max_x && slime_y < max_y) {
            add_slime(slime_x, slime_y, player_id);
        }
    }
    
    // Create a more complex room layout with pillars and inner walls
    // Add corner pillars
    add_block(start_x + 128, start_y + 128, get_sprites()->cobble_texture);
    add_block(start_x + 1728, start_y + 128, get_sprites()->cobble_texture);
    add_block(start_x + 128, start_y + 896, get_sprites()->cobble_texture);
    add_block(start_x + 1728, start_y + 896, get_sprites()->cobble_texture);
    
    // Add random inner structures
    int num_structures = rand() % 4 + 2;
    for(int i = 0; i < num_structures; i++) {
        int struct_x = start_x + ((2 + rand() % 24) * 64);
        int struct_y = start_y + ((2 + rand() % 14) * 64);
        
        // Only create structure if it will fit entirely within room bounds
        if(struct_x + (3 * 64) < max_x && struct_y + (3 * 64) < max_y) {
            // Create L-shaped walls
            for(int j = 0; j < 3; j++) {
                add_block(struct_x + j * 64, struct_y, get_sprites()->cobble_texture);
                add_block(struct_x, struct_y + j * 64, get_sprites()->cobble_texture);
            }
        }
    }
    
    // Add some teleporters with random destinations within the room
    if(rand() % 3 == 0) { // 33% chance for teleporters
        int x1 = start_x + ((2 + rand() % 24) * 64);
        int y1 = start_y + ((2 + rand() % 14) * 64);
        int x2 = start_x + ((2 + rand() % 24) * 64);
        int y2 = start_y + ((2 + rand() % 14) * 64);
        if(x1 < max_x && y1 < max_y && x2 < max_x && y2 < max_y) {
            add_teleporter(x1, y1, x2, y2);
        }
    }
}