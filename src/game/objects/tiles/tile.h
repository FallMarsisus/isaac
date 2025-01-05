

#include "../../anim.h"
#include "../../../utils/vector.h"

typedef struct {
    Vector* pos;
    SDL_Rect* hitbox;
    anim_core* core;
} Tile;

Tile* create_tile(float x, float y, float width, float height, char* texture);
void free_tile(Tile* tile);

void draw_tile(Tile* tile);