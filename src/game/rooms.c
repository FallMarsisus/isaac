#include "rooms.h"

#include <stdlib.h>
#include <assert.h>
#include "utils/utils.h"

struct room {
    int x;
    int y;

    struct room* up;
    struct room* down;
    struct room* left;
    struct room* right;

    list_entity entities;
};

typedef struct room room;

struct cell_room {
    room* value;
    struct cell_room* next;
};

typedef struct cell_room cell_room;

typedef struct {
    cell_room* first;
    cell_room* last;
    int len;
} cell_list;

struct map {
    cell_list** dict_rooms;
    int amount;
    int maxlen;
};

typedef struct map map;

int h(map* m, int posx, int posy) {
    return ipow(2, posx) * ipow(3, posy) % m->maxlen;
}

void add_room(map* m, int posx, int posy) {
    cell_room* new_room = malloc(sizeof(cell_room));
    int index = h(m, posx, posy);

    //First : Add in Dictionnary
    assert(0 <= index && index < m->maxlen);
    cell_room* last = m->dict_rooms[index]->last;
    if(last == NULL) {
        if(m->dict_rooms[index]->first == NULL) {
            m->dict_rooms[index]->first = new_room;
        }
        last = new_room;
    }
    else {
        last->next = new_room;
        last = new_room;
    }

    //Create the room
    new_room->value = malloc(sizeof(room));
    new_room->value->x = posx;
    new_room->value->y = posy;
    
    room* up = get_up(m, new_room->value);
    if(up != NULL) {
        up->down = new_room->value;
        new_room->value->up = up;
    }
    room* down = get_down(m, new_room->value);
    if(down != NULL) {
        down->up = new_room->value;
        new_room->value->down = down;
    }
    room* left = get_left(m, new_room->value);
    if(left != NULL) {
        left->right = new_room->value;
        new_room->value->left = left;
    }
    room* right = get_right(m, new_room->value);
    if(right != NULL) {
        right->left = new_room->value;
        new_room->value->right = right;
    }

    new_room->value->entities = create_list();
}

void destroy_room(map* m, int posx, int posy);

room* get_room(map* m, int posx, int posy);

room* get_up(map* m, room* r);
room* get_down(map* m, room* r);
room* get_left(map* m, room* r);
room* get_right(map* m, room* r);

map* create_map() {
    map* m = malloc(sizeof(map));
    m->amount = 0;
    m->maxlen = 10;
    m->dict_rooms = malloc(sizeof(cell_list*) * m->maxlen);
    return m;
}

void free_map(map* m) {
    assert(m != NULL);
    for(int i = 0; i < m->maxlen; i++) {
        cell_room* temp = m->dict_rooms[i]->first;
        cell_room* prev = m->dict_rooms[i]->first;
        while(temp->next != NULL) {
            temp = temp->next;
            destroy_room(m, prev->value->x, prev->value->y);
            prev = temp;
        }
    }
    free(m);
}