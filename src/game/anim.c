#include "anim.h"

anim_core* create_core(SDL_Renderer* ren, SDL_Texture* tex, int sprite_width, int sprite_height) {
    anim_core* core = malloc(sizeof(anim_core));
    if (core == NULL) {
        fprintf(stderr, "Failed to allocate memory for anim_core\n");
        return NULL;
    }
    core->sprite_sheet = tex;
    core->current_img = malloc(sizeof(SDL_Rect));

    core->angle = 0.; // set the angle.
    core->center = malloc(sizeof(SDL_Point)); // the center where the texture will be rotated.
    core->center->x = 0; core->center->y = 0;
    core->flip = SDL_FLIP_NONE; // the flip of the texture.

    core->animations = create_array();
    core->playing = false;
    core->anim_index = 0;
    core->current = 0;

    core->sprite_width = sprite_width;
    core->sprite_height = sprite_height;

    *core->current_img = (SDL_Rect) {
        core->current * core->sprite_width,
        core->anim_index * core->sprite_height,
        core->sprite_width,
        core->sprite_height
    };

    core->prev = malloc(sizeof(struct timeval));
    core->now = malloc(sizeof(struct timeval));
    gettimeofday(core->prev, NULL);
    gettimeofday(core->now, NULL);

    return core;
}
void free_core(anim_core* core) {
    for(int i = 0; i < get_len(core->animations); i++) {
        free((anim*) get_elt(core->animations, i));
    }
    free_array(core->animations);

    free(core->current_img);
    free(core->center);
    free(core->prev);
    free(core->now);
    free(core);
}

int add_anim(anim_core* core, int line, float interval, int amount) {
    anim* a = malloc(sizeof(anim));
    a->amount = amount;
    a->interval = interval;
    a->line = line;
    append(core->animations, a);
    //return get_len(core->animations) - 1;
    return -1;
}

void set_active_anim(anim_core* core, int index) {
    if(index >= get_len(core->animations)) {
        return;
    }
    if(core->anim_index == index) return;
    core->anim_index = index;
    core->current = 0;
}
void play_anim(anim_core* core) {
    core->playing = true;
}
void pause_anim(anim_core* core) {
    core->playing = false;
}
void stop_anim(anim_core* core) {
    core->playing = false;
    core->current = 0;
}

void draw_core(SDL_Renderer* ren, SDL_Rect* pos, anim_core* core) {
    if(core == NULL) return;
    if(core->sprite_sheet == NULL) return;
    
    if (core->playing) {
        gettimeofday(core->now, NULL);
        double delta_time = (core->now->tv_sec - core->prev->tv_sec) + (core->now->tv_usec - core->prev->tv_usec) * 1e-6;
        if (core->anim_index >= get_len(core->animations)) return;

        anim* curren_anim = get_elt(core->animations, core->anim_index);
        if (curren_anim == NULL) return;

        if (delta_time >= curren_anim->interval) {
            core->current++;
            if (core->current >= curren_anim->amount) core->current = 0;
            gettimeofday(core->prev, NULL);
        }

        core->current_img->x = core->current * core->sprite_width;
        core->current_img->y = core->anim_index * core->sprite_height;
    }

    SDL_RenderCopyEx(ren, core->sprite_sheet, core->current_img, pos, core->angle, core->center, core->flip);
}