#include "map.h"

map* create_map(sprite_list* sprites) {
    map* m = malloc(sizeof(map));
    m->dict_rooms = create_dict();
    m->map_x = 0;
    m->map_y = 0;
    m->sprites = sprites;

    m->current_room = create_room(0, 0, m->sprites);
    add_room(m, m->current_room);
    
    m->p = create_player(320, 180, m->sprites);

    for(int i = 0; i < 5; i++) {
        int m1_x = i, m1_y = i;
        int m2_x = -i, m2_y = -i;

        room* r1 = get_room(m, m1_x, m1_y);
        if(r1 == NULL) {
            r1 = create_room(m1_x, m1_y, m->sprites);
            add_room(m, r1);
        }
        room* r2 = get_room(m, m2_x, m2_y);
        if(r2 == NULL) {
            r2 = create_room(m2_x, m2_y, m->sprites);
            add_room(m, r2);
        }

        Teleporter_pair* pair = create_pair_teleporter(m1_x, m1_y, m2_x, m2_y, sprites);

        add_tile_to_room(r1, pair->teleporter1);
        add_tile_to_room(r2, pair->teleporter2);
    }
    return m;
}

void load_textures(map* m, SDL_Renderer* ren) {
    assert(m != NULL);
    
    load_player_textures(m->p, ren);
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
        r = create_room(x, y, m->sprites);
        add_room(m, r);
    }
    m->current_room = r;
    m->map_x = x;
    m->map_y = y;
}

//Changes the room if the player is at the edge of the screen
void update_map(map* m, int win_width, int win_height) {
    if(m->current_room == NULL) return;
    get_inputs(m->p);

    update_player(m->p, m->current_room->entities, m->current_room->tiles);

    Vector* pos = m->p->body->pos;

    if (pos->x < m->current_room->bg_rect->x) {
        int nb_rooms = 1 + abs(pos->x / win_width);
        change_room(m, m->map_x - nb_rooms, m->map_y);
        
        int new_pos = (int) pos->x % win_width;
        if(-m->current_room->bg_rect->x < new_pos) {
            new_pos = m->current_room->bg_rect->x + m->current_room->bg_rect->w - m->p->body->hitbox->w;
        }
        if(new_pos < 0) new_pos += win_width;
        set_entity_position(m->p->body, new_pos, pos->y);
    }
    if (pos->y < m->current_room->bg_rect->y) {
        int nb_rooms = 1 + abs(pos->y / win_height);
        change_room(m, m->map_x, m->map_y - nb_rooms);

        int new_pos = (int) pos->y % win_height;
        if(-m->current_room->bg_rect->y < new_pos) {
            new_pos = m->current_room->bg_rect->y + m->current_room->bg_rect->h - m->p->body->hitbox->h;
        }
        if(new_pos < 0) new_pos += win_height;
        set_entity_position(m->p->body, pos->x, new_pos);
    }
    if (pos->x + m->p->body->hitbox->w > m->current_room->bg_rect->x + m->current_room->bg_rect->w) {
        int nb_rooms = abs(pos->x + m->current_room->bg_rect->x + m->p->body->hitbox->w) / win_width;
        change_room(m, m->map_x + nb_rooms, m->map_y);
        
        int new_pos = (int) pos->x % win_width;
        if(pos->x < win_width + m->current_room->bg_rect->x) {
            new_pos = m->current_room->bg_rect->x;
        }

        if(new_pos > win_width) new_pos -= win_width;
        
        set_entity_position(m->p->body, new_pos, pos->y);
    }
    if (pos->y + m->p->body->hitbox->h > m->current_room->bg_rect->y + m->current_room->bg_rect->h) {
        int nb_rooms = abs(pos->y + m->current_room->bg_rect->y + m->p->body->hitbox->h) / win_height;
        change_room(m, m->map_x, m->map_y + nb_rooms);

        int new_pos = (int) pos->y % win_height;
        if(pos->y < win_height + m->current_room->bg_rect->y) {
            new_pos = m->current_room->bg_rect->y;
        }
        if(new_pos > win_height) new_pos -= win_height;

        set_entity_position(m->p->body, pos->x, new_pos);
    }

    update_room(m->p, m->current_room);
}

void draw_map(map* m, SDL_Renderer* ren) {
    assert(ren != NULL && m != NULL);
    
    draw_room(ren, m->current_room);
    draw_player(ren, m->p);
}