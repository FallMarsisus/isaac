#include "display.h"

sprite_list* sl;
SDL_Renderer* renderer;

void init_renderer(SDL_Window* win) {
    // Create a renderer
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    int win_width = 960, win_height = 540;
    SDL_RenderSetLogicalSize(renderer, win_width, win_height);
}

// Load a texture from a file
SDL_Texture* load_texture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        fprintf(stderr, "Failed to load file: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void load_sprites() {
    sl = malloc(sizeof(sprite_list));
    sl->player_texture = load_texture("assets/entities/player.bmp");
    sl->alien_texture = load_texture("assets/entities/alien.bmp");
    sl->goblin_texture = load_texture("assets/entities/goblin.bmp");
    sl->slime_texture = load_texture("assets/entities/slime.bmp");
    sl->flame_texture = load_texture("assets/AssetPack/FX/Projectile/Fireball.png");
    sl->boss_texture = load_texture("assets/entities/flame.png");

    sl->inv_texture = load_texture("assets/inventory/inv_grid.bmp");
    sl->apple_item_texture = load_texture("assets/inventory/apple.bmp");
    sl->unknown_item_texture = load_texture("assets/inventory/unknown_item.bmp");
    sl->sword_item_texture = load_texture("assets/inventory/sword_item.bmp");
	sl->sword_slot_texture = load_texture("assets/inventory/swordSlot.bmp");
	sl->armor_slot_texture = load_texture("assets/inventory/chestplate.bmp");
	
    sl->sword_texture = load_texture("assets/sword/spritesheet.bmp");
	
    sl->cobble_texture = load_texture("assets/tilemap/cobble.bmp");
    sl->iconE_texture = load_texture("assets/tilemap/iconE.bmp");
    sl->teleporter_texture = load_texture("assets/tilemap/teleporter.bmp");
    sl->chest_opened_texture = load_texture("assets/tilemap/chest_opened.bmp");
    sl->chest_closed_texture = load_texture("assets/tilemap/chest_closed.bmp");
    sl->background_texture = load_texture("assets/tilemap/background.bmp");

    sl->tileset_texture = load_texture("assets/AssetPack/Backgrounds/Tilesets/TilesetDungeon.png");
    sl->tileset_texture_tiled = load_texture("assets/maps/tiled/Dungeon_Tileset_nobg.png");

    sl->red_heart_full_texture = load_texture("assets/health/red_heart_full.bmp");
    sl->red_heart_half_texture = load_texture("assets/health/red_heart_half.bmp");
    sl->heart_empty_texture = load_texture("assets/health/heart_empty.bmp");
}

sprite_list* get_sprites() {
    return sl;
}

SDL_Renderer* get_renderer() {
    return renderer;
}