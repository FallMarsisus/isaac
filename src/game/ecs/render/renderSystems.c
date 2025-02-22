#include "renderSystems.h"

typedef struct {
    float interval;
    int amount;

    int start_x;
    int start_y;
} Anim;

void init_sprite_component(SpriteComponent* sprite, int width, int height, SDL_Texture* tex) {
    sprite->layer = 0;
    sprite->width = width; sprite->height = height;

    sprite->texture = tex;
    if (!sprite->texture) {
		fprintf(stderr, "\033[1;31mFailed to load a texture.\033[0m\n");
        exit(1);
    }

    sprite->angle = 0;
    sprite->flip = SDL_FLIP_NONE;
    sprite->center = malloc(sizeof(SDL_Point));
    *(sprite->center) = (SDL_Point) {width / 2, height / 2};
}
void init_anim_component(AnimationComponent* animation, int sprite_width, int sprite_height) {
    animation->animations = create_array();
    animation->anim_index = 0;
    animation->counter = 0;
    animation->playing = false;
    animation->current_img = malloc(sizeof(SDL_Rect));
    *(animation->current_img) = (SDL_Rect) {0, 0, sprite_width, sprite_height};
    animation->last_change = SDL_GetTicks();
}
void init_tile_component(TileComponent* tile, int tile_x, int tile_y, int tile_width, int tile_height) {
    tile->tile_x = tile_x;
    tile->tile_y = tile_y;
    tile->tile_width = tile_width;
    tile->tile_height = tile_height;
}

void free_sprite_component(SpriteComponent* sprite) {
    if(sprite->center) {
        free(sprite->center);
    }
    sprite->center = NULL;
}
void free_anim_component(AnimationComponent* animation) {
    free(animation->current_img);
    for(int i = 0; i < get_len(animation->animations); i++) {
        free((Anim*) get_elt(animation->animations, i));
    }
    free_array(animation->animations);
}
void free_all_render_components(uint32_t id) {
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);
    if(sprite) {
        free_sprite_component(sprite);
    }
    AnimationComponent* anim = ECS_GetComponent(id, ANIMATION);
    if(anim) {
        free_anim_component(anim);
    }
}

int add_anim(AnimationComponent* animation, float interval, int amount) {
    Anim* a = malloc(sizeof(Anim));
    a->start_x = 0;
    a->start_y = get_len(animation->animations);
    a->amount = amount;
    a->interval = interval;
    append(animation->animations, a);
    return get_len(animation->animations) - 1;
}
int add_anim_tile(AnimationComponent* animation, int start_x, int start_y, float interval, int amount) {
    Anim* a = malloc(sizeof(Anim));
    a->start_x = start_x;
    a->start_y = start_y;
    a->amount = amount;
    a->interval = interval;
    append(animation->animations, a);
    return get_len(animation->animations) - 1;
}

void set_active_anim(AnimationComponent* anim, int index) {
    if(index >= get_len(anim->animations)) {
        return;
    }
    if(anim->anim_index == index) return;
    anim->anim_index = index;
    anim->counter = 0;
}
void play_anim(AnimationComponent* anim) {
    anim->playing = true;
}
void pause_anim(AnimationComponent* anim) {
    anim->playing = false;
}
void stop_anim(AnimationComponent* anim) {
    anim->playing = false;
    anim->counter = 0;
}

void update_anim(uint32_t id) {
    AnimationComponent* anim = ECS_GetComponent(id, ANIMATION);
    if(anim && get_len(anim->animations) > 0) {
        double interval = (SDL_GetTicks() - anim->last_change) / 1000.;
        if(anim->anim_index >= get_len(anim->animations)) return;
        
        Anim* curren_anim = get_elt(anim->animations, anim->anim_index);
        if (curren_anim == NULL) return;
        
        if (interval >= curren_anim->interval && anim->playing) {
            anim->counter++;
            if (anim->counter >= curren_anim->amount) anim->counter = 0;
            anim->last_change = SDL_GetTicks();
        }
        
        anim->current_img->x = (curren_anim->start_x + anim->counter) * anim->current_img->w;
        anim->current_img->y = curren_anim->start_y * anim->current_img->h;
    }
}

void render_background(SDL_Rect cam, SDL_Renderer* renderer, SDL_Texture* background) {
    int offsetX = cam.x % 64; int offsetY = cam.y % 64;
    for (int y = -64; y < 1280 + 64; y += 64) {
        for (int x = -64; x < 1920 + 64; x += 64) {
            SDL_Rect dest = {x - offsetX, y - offsetY, 64, 64};
            SDL_RenderCopy(renderer, background, NULL, &dest);
        }
    }
}

void render_component(uint32_t id, SDL_Rect cam, SDL_Renderer* renderer) {
    PositionComponent* position = ECS_GetComponent(id, POSITION);
    SpriteComponent* sprite = ECS_GetComponent(id, SPRITE);

    if (!position || !sprite || !sprite->texture) return;

    SDL_Rect* srcRect = NULL;

    TileComponent* tile_comp = ECS_GetComponent(id, TILE);
    if(tile_comp) {
        srcRect = & (SDL_Rect) {
            tile_comp->tile_x * tile_comp->tile_width,
            tile_comp->tile_y * tile_comp->tile_height,
            tile_comp->tile_width,
            tile_comp->tile_height
        };
    }

    AnimationComponent* anim = ECS_GetComponent(id, ANIMATION);
    if(anim && get_len(anim->animations) > 0) {
        srcRect = anim->current_img;
    }

    SDL_Rect dest = {
        (int)position->x - cam.x,
        (int)position->y - cam.y,
        sprite->width,
        sprite->height
    };
    if(position->camFixed) {
        dest.x = position->x;
        dest.y = position->y;
    }
    
    SDL_RenderCopyEx(renderer, sprite->texture, srcRect, &dest, sprite->angle, sprite->center, sprite->flip);
}