#include "list_entities.h"

struct cell_e {
    struct cell_e* next;
    entity* value;
};

typedef struct cell_e cell;

typedef cell* list_entity;

//o(1)
list_entity create_list() {
    cell* c = malloc(sizeof(cell));
    c->value = NULL;
    
    return c;
}

//o(n)
void free_entity_list(list_entity l) {
    if(l == NULL) return;
    cell* temp = l->next;
    if(l->value != NULL) {
        free_entity(l->value);
    }

    free(l);
    free_list(temp);
}

//o(n)
int get_len(list_entity l) {
    if(l->next == NULL) return 0;
    return 1 + get_len(l->next);
}

//o(1)
entity* get_first(list_entity l) {
    if(l == NULL || l->next == NULL) return NULL;
    return l->next->value;
}

//o(1)
void append_entity(list_entity l, entity* e) {
    assert(e != NULL);

    cell* c = malloc(sizeof(cell));
    c->value = e;
    c->next = l->next;
    
    l->next = c;
}

//o(n)
void remove_entity(list_entity l, entity* e) {
    assert(e != NULL);

    if(l == NULL) return;
    if(l->next == NULL) return;

    if(l->next->value == e) {
        cell* temp = l->next;
        l->next = l->next->next;
        free_entity(temp->value);
        free(temp);
        return;
    }
    
    remove_entity(l->next, e);
}

//o(n * f)
void iter(list_entity l, void(*f)(entity*)) {
    if(l == NULL) return;
    if(l->value != NULL) f(l->value);
    iter(l->next, f);
}
