#include "map.h"

struct map_s {
    player* p;

    dict dict_rooms;

    int map_x; int map_y; //Map Coords
    room* current_room;
};

typedef struct map_s map;

map* create_map() {
    map* m = malloc(sizeof(map));
    m->dict_rooms = create_dict();
    m->map_x = 0;
    m->map_y = 0;

    m->current_room = create_room(0, 0);
    add_room(m, m->current_room);
    
    m->p = create_player(320, 180);
    return m;
}

void load_textures(map* m, SDL_Renderer* ren) {
    assert(m != NULL);

    //=========Loads textures===========
    load_player_textures(
        m->p, 
        ren,
        "assets/player/sprite_sheet.bmp"
    );
}

void free_map(map* m) {
    assert(m != NULL && m->dict_rooms != NULL);
    free_dict(m->dict_rooms, true);
    free_player(m->p);
    free(m);
}

int get_map_x(map* m) {
    assert(m != NULL);
    return m->map_x;
}
int get_map_y(map* m) {
    assert(m != NULL);
    return m->map_y;
}
room* get_current_room(map* m) {
    assert(m != NULL);
    return m->current_room;
}
room* get_room(map* m, int posx, int posy) {
    assert(m != NULL);
    if(!mem(posx, posy, m->dict_rooms)) {
        return NULL;
    }
    return (room*) getValue(posx, posy, m->dict_rooms);
}

void print_rm(int x, int y, void* data) {
    print_room((room*) data);
}
void print_map(map* m) {
    iter_dict(print_rm, m->dict_rooms);
}

void add_room(map* m, room* r) {
    assert(m != NULL && r != NULL && m->dict_rooms != NULL);
    int x = getX(r);
    int y = getY(r);
    add(x, y, r, m->dict_rooms);

    room* up = get_room(m, x, y - 1);
    if(up != NULL) setUp(r, up);
    room* down = get_room(m, x, y + 1);
    if(down != NULL) setDown(r, down);
    room* left = get_room(m, x - 1, y);
    if(left != NULL) setLeft(r, left);
    room* right = get_room(m, x + 1, y);
    if(right != NULL) setRight(r, right);
}
void destroy_room(map* m, int x, int y) {
    assert(m != NULL);
    room* r = get_room(m, x, y);
    if(r == NULL) return;

    room* up = get_room(m, x, y - 1);
    if(up != NULL) clearUp(r, up);
    room* down = get_room(m, x, y + 1);
    if(down != NULL) clearDown(r, down);
    room* left = get_room(m, x - 1, y);
    if(left != NULL) clearLeft(r, left);
    room* right = get_room(m, x + 1, y);
    if(right != NULL) clearRight(r, right);

    free_room(r);
    removeValue(x, y, m->dict_rooms);
}

void set_map_x(map* m, int x) {
    assert(m != NULL);
    m->map_x = x;
}
void set_map_y(map* m, int y) {
    assert(m != NULL);
    m->map_y = y;
}
void set_current_room(map* m, room* r) {
    assert(m != NULL && r != NULL);
    m->current_room = r;
}
void change_room(map* m, int x, int y) {
    assert(m != NULL);
    room* r = get_room(m, x, y);
    if(r == NULL) {
        r = create_room(x, y);
        add_room(m, r);
    }
    set_current_room(m, r);
    set_map_x(m, x);
    set_map_y(m, y);
}

//Changes the room if the player is at the edge of the screen
void update_map(map* m, int win_width, int win_height) {
    get_inputs(m->p);

    update_player(m->p, win_width, win_height);

    SDL_Rect* pos = get_player_pos(m->p);

    if (pos->x < 0) {
        pos->x = win_width - pos->w;
        change_room(m, get_map_x(m) - 1, get_map_y(m));
    }
    if (pos->y < 0) {
        pos->y = win_height - pos->h;
        change_room(m, get_map_x(m), get_map_y(m) - 1);
    }
    if (pos->x > win_width - pos->w) {
        pos->x = 0;
        change_room(m, get_map_x(m) + 1, get_map_y(m));
    }
    if (pos->y > win_height - pos->h) {
        pos->y = 0;
        change_room(m, get_map_x(m), get_map_y(m) + 1);
    }

    update_room(m->p, m->current_room);
    set_player_pos(m->p, pos->x, pos->y);
}

void draw_map(map* m, SDL_Renderer* ren) {
    assert(ren != NULL && m != NULL);
    
    draw_room(ren, m->current_room);
    draw_player(ren, m->p);
}