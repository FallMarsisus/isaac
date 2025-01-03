#include "anim.h"

anim_core* create_core(SDL_Renderer* ren, char* path, int sprite_width, int sprite_height) {
    anim_core* core = malloc(sizeof(anim_core));
    core->animations = create_array();
    core->playing = false;
    core->anim_index = 0;
    core->current = 0;
    core->prev = malloc(sizeof(struct timeval));
    core->now = malloc(sizeof(struct timeval));
    gettimeofday(core->prev, NULL);
    gettimeofday(core->now, NULL);

    core->sprite_width = sprite_width;
    core->sprite_height = sprite_height;

    SDL_Surface* temp = SDL_LoadBMP(path);

    core->sprite_sheet = SDL_CreateTextureFromSurface(ren, temp);

    SDL_FreeSurface(temp);

    return core;
}

void free_core(anim_core* core) {
    for(int i = 0; i < get_len(core->animations); i++) {
        free((anim*) get_elt(core->animations, i));
    }
    free_array(core->animations);
    SDL_DestroyTexture(core->sprite_sheet);
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
        printf("NEAHHHAHHA\n");
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
    if(core->playing) {
        gettimeofday(core->now, NULL);
        double delta_time = (core->now->tv_sec - core->prev->tv_sec) + (core->now->tv_usec - core->prev->tv_usec) * 1e-6;
        if(core->anim_index >= get_len(core->animations)) return;
        
        anim* curren_anim = get_elt(core->animations, core->anim_index);
        if(curren_anim == NULL) return;

        if(delta_time >= curren_anim->interval) {
            core->current++;
            if(core->current >= curren_anim->amount) core->current = 0;
            gettimeofday(core->prev, NULL);
        }
    }

    SDL_Rect rect = {
        core->current * core->sprite_width,
        core->anim_index * core->sprite_height,
        core->sprite_width,
        core->sprite_height
    };

    SDL_RenderCopy(ren, core->sprite_sheet, &rect, pos);
}