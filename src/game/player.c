#include "player.h"

struct player_s {
    bool* keys; //Keys pressed
    int map_x; int map_y; //Map Coords

    Vector* dir;
    int speed;
    
    entity* body;

    SDL_Texture* up_texture;
    SDL_Texture* down_texture;
    SDL_Texture* left_texture;
    SDL_Texture* right_texture;
};

typedef struct player_s player;

player* create_player(int x, int y) {
    player* p = malloc(sizeof(player));
    
    p->body = create_entity(x, y, 32, 32);
    p->speed = 5;
    p->map_x = 0; p->map_y = 0;
    p->dir = malloc(sizeof(Vector));
    p->dir->x = 0; p->dir->y = 0;
    
    p->keys = malloc(sizeof(bool) * 4);
    for(int i = 0; i < 4; i ++) {
        p->keys[i] = false;
    }
    return p;
}

void load_player_textures(player* p, SDL_Texture* up_texture, SDL_Texture* down_texture, SDL_Texture* left_texture, SDL_Texture* right_texture) {
    p->up_texture = up_texture;
    p->down_texture = down_texture;
    p->left_texture = left_texture;
    p->right_texture = right_texture;

    set_sprite(p->body, p->down_texture);
}

void free_player(player* p) {
    SDL_DestroyTexture(p->up_texture);
    SDL_DestroyTexture(p->down_texture);
    SDL_DestroyTexture(p->left_texture);
    SDL_DestroyTexture(p->right_texture);

    free_entity(p->body);
    free(p->dir);
    free(p->keys);
    free(p);
}

//Maps the keys to the keys array
void get_inputs(player* p) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    p->keys[0] = state[SDL_SCANCODE_UP];
    p->keys[1] = state[SDL_SCANCODE_DOWN];
    p->keys[2] = state[SDL_SCANCODE_LEFT];
    p->keys[3] = state[SDL_SCANCODE_RIGHT];
}

//Changes the room if the player is at the edge of the screen
room* change_room(player* p, int win_width, int win_height, map* m, room* current) {
    SDL_Rect* pos = get_pos(p->body);
    room* new_room = current;

    if (pos->x < 0) {
        pos->x = win_width - pos->w;
        p->map_x--;
        new_room = get_room(m, p->map_x, p->map_y);
        if(new_room == NULL) {
            new_room = create_room(p->map_x, p->map_y);
            add_room(m, new_room);
        }
    }
    if (pos->y < 0) {
        pos->y = win_height - pos->h;
        p->map_y--;
        new_room = get_room(m, p->map_x, p->map_y);
        if(new_room == NULL) {
            new_room = create_room(p->map_x, p->map_y);
            add_room(m, new_room);
        }
    }
    if (pos->x > win_width - pos->w) {
        pos->x = 0;
        p->map_x++;
        new_room = get_room(m, p->map_x, p->map_y);
        if(new_room == NULL) {
            new_room = create_room(p->map_x, p->map_y);
            add_room(m, new_room);
        }
    }
    if (pos->y > win_height - pos->h) {
        pos->y = 0;
        p->map_y++;
        new_room = get_room(m, p->map_x, p->map_y);
        if(new_room == NULL) {
            new_room = create_room(p->map_x, p->map_y);
            add_room(m, new_room);
        }
    }

    return new_room;
}

//Moves the player
void move(player* p) {
    SDL_Rect* pos = get_pos(p->body);
    p->dir->x = 0; p->dir->y = 0;

    if(p->keys[0]) p->dir->y -= 1;
    if(p->keys[1]) p->dir->y += 1;
    if(p->keys[2]) p->dir->x -= 1;
    if(p->keys[3]) p->dir->x += 1;

    normalize(p->dir);
    
    if(fabs(p->dir->x) > 0.1) {
        pos->x += p->dir->x * p->speed;
    }
    if(fabs(p->dir->y) > 0.1) {
        pos->y += p->dir->y * p->speed;
    }
}

//Do the whole shit
room* update(player* p, int win_width, int win_height, map* m, room* current) {
    //SDL_Rect* pos = get_pos(p->body);

    move(p);
    room* new_room = change_room(p, win_width, win_height, m, current);
    return new_room;
}

void update_player_sprite(player* p) {
    if(p->dir->x < -0.1) set_sprite(p->body, p->left_texture);
    else if(p->dir->x > 0.1) set_sprite(p->body, p->right_texture);
    else if(p->dir->y < -0.1) set_sprite(p->body, p->up_texture);
    else if(p->dir->y > 0.1) set_sprite(p->body, p->down_texture);
}

void draw_player(SDL_Renderer* ren, player* p) {
    update_player_sprite(p);

    draw_entity(ren, p->body);
}