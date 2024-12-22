#include "chained_list.h"
struct cell_s {
    void* data;
    struct cell_s* next;
};

typedef struct cell_s cell;

struct chained_list_s {
    cell* first;
    cell* last;
    int len;
};

typedef struct chained_list_s chained_list;

chained_list* create_list(Datatype type) {
    chained_list* c = malloc(sizeof(chained_list));
    c->len = 0;
    c->first = malloc(sizeof(cell));
    c->last = malloc(sizeof(cell));
    c->first->next = c->last;
    return c;
}

void free_cell(cell* c) {
    if(c == NULL) return;
    if(c->data != NULL) free(c->data);
    cell* temp = c->next;
    free(c);
    free_cell(temp);
}

void free_list(chained_list* l) {
    assert(l != NULL);
    free_cell(l->first);
    free(l);
}

void append_elt(chained_list* l, void* element) {
    assert(element != NULL);
    cell* c = malloc(sizeof(cell));
    c->data = element;

    c->next = l->first->next;
    l->first->next = c;
}

void remove_elt(chained_list* l, void* element) {
    if(element == NULL) return;
    if(l == NULL) return;

    cell* temp = l->first;
    while(temp->next != NULL) {
        if(temp->next->data == element) {
            temp->next = temp->next->next;
        }
        temp = temp->next;
    }
}

void iter(chained_list* l, void (*f)(void*)) {
    if(l == NULL) return;
    if(f == NULL) return;

    cell* temp = l->first;
    while(temp != NULL) {
        if(temp->data != NULL) f(temp->data);
        temp = temp->next;
    }
}