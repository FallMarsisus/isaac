#include "map.h"

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
        ren
    );
}

void free_room_modif(int x, int y, void* data) {
    free_room((room*) data);
}
void free_map(map* m) {
    assert(m != NULL && m->dict_rooms != NULL);
    iter_dict(free_room_modif, m->dict_rooms);
    free_dict(m->dict_rooms, false);
    free_player(m->p);
    free(m);
}

room* get_room(map* m, int posx, int posy) {
    assert(m != NULL);
    if(!mem(posx, posy, m->dict_rooms)) {
        return NULL;
    }
    return (room*) getValue(posx, posy, m->dict_rooms);
}

void add_room(map* m, room* r) {
    assert(m != NULL && r != NULL && m->dict_rooms != NULL);
    int x = r->x;
    int y = r->y;
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

void change_room(map* m, int x, int y) {
    assert(m != NULL);
    room* r = get_room(m, x, y);
    if(r == NULL) {
        r = create_room(x, y);
        add_room(m, r);
    }
    m->current_room = r;
    m->map_x = x;
    m->map_y = y;
}

//Changes the room if the player is at the edge of the screen
void update_map(map* m, int win_width, int win_height) {
    get_inputs(m->p);

    update_player(m->p, win_width, win_height);

    Vector* pos = m->p->body->pos;

    if (pos->x < 0) {
        set_entity_position(m->p->body, win_width - m->p->body->hitbox->w, pos->y);
        change_room(m, m->map_x - 1, m->map_y);
    }
    if (pos->y < 0) {
        set_entity_position(m->p->body, pos->x, win_height - m->p->body->hitbox->h);
        change_room(m, m->map_x, m->map_y - 1);
    }
    if (pos->x > win_width - m->p->body->hitbox->w) {
        set_entity_position(m->p->body, 0, pos->y);
        change_room(m, m->map_x + 1, m->map_y);
    }
    if (pos->y > win_height - m->p->body->hitbox->h) {
        set_entity_position(m->p->body, pos->x, 0);
        change_room(m, m->map_x, m->map_y + 1);
    }

    update_room(m->p, m->current_room);
}

void draw_map(map* m, SDL_Renderer* ren) {
    assert(ren != NULL && m != NULL);
    
    draw_room(ren, m->current_room);
    draw_player(ren, m->p);
}