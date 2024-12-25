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

chained_list* create_list() {
    chained_list* c = malloc(sizeof(chained_list));
    c->len = 0;
    c->first = malloc(sizeof(cell));
    c->first->data = NULL;
    c->last = malloc(sizeof(cell));
    c->last->data = NULL;
    c->first->next = c->last;
    c->last->next = NULL;
    return c;
}

void append_elt(chained_list* l, void* element) {
    assert(element != NULL);
    cell* c = malloc(sizeof(cell));
    c->data = element;

    c->next = l->first->next;
    l->first->next = c;

    l->len++;
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

    l->len--;
}

void free_list(chained_list* l) {
    if(l == NULL) return;

    cell* temp = l->first->next;
    while(temp != l->last) {
        cell* next = temp->next;
        if(temp->data != NULL) free(temp->data);
        free(temp);
        temp = next;
    }
    free(l->first);
    free(l->last);
    free(l);
}

void iter(chained_list* l, void (*f)(void*)) {
    if(l == NULL) return;
    if(f == NULL) return;

    cell* temp = l->first->next;
    while(temp != l->last) {
        cell* next = temp->next;
        if(temp->data != NULL) f(temp->data);
        temp = next;
    }
}

cell* get_first(chained_list* l) {
    return l->first->next;
}
cell* get_next(cell* c) {
    return c->next;
}
void* get_data(cell* c) {
    return c->data;
}