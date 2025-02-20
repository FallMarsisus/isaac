#include "renderSystems.h"

void init_sprite_component(SpriteComponent* sprite, int width, int height, SDL_Texture* tex) {
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

void free_sprite_component(SpriteComponent* sprite) {
    free(sprite->center);
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
        ECS_ClearComponent(id, SPRITE);
    }
    AnimationComponent* anim = ECS_GetComponent(id, ANIMATION);
    if(anim) {
        free_anim_component(anim);
        ECS_ClearComponent(id, ANIMATION);
    }
}
int add_anim(AnimationComponent* animation, float interval, int amount) {

    Anim* a = malloc(sizeof(Anim));
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

void render_background(SDL_Rect cam, SDL_Renderer* renderer, SDL_Texture* background) {
    int offsetX = cam.x % 64; int offsetY = cam.y % 64;
    for (int y = -64; y < 1024 + 64; y += 64) {
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
        
        anim->current_img->x = anim->counter * anim->current_img->w;
        anim->current_img->y = anim->anim_index * anim->current_img->h;
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