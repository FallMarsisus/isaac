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
    chained_list* l = malloc(sizeof(chained_list));
    l->len = 0;
    l->first = NULL;
    l->last = NULL;
    return l;
}

cell* createCell(void* data, cell* next) {
    cell* cellule = malloc(sizeof(cell));
    cellule->data = data;
    cellule->next = next;
    return cellule;
}


void append_elt(chained_list* l, void* element) {

    if (l->len == 0) {
        l->first = createCell(element, NULL);
        l->last = l->first;
        l->len++;
        return;
    }

    l->first = createCell(element, l->first);
    l->len++;
}

void remove_elt(chained_list* l, void* element) {

    if(element == NULL) return;
    if(l == NULL || l->len == 0) return;

    cell* temp = l->first->next;
    cell* last = NULL;

    while (temp != NULL) {

        if (temp->data == element) {
            cell* next = temp->next;
            if (last == NULL) {
                l->first = next; //pour gérer le cas où les premiers élément doivent être suppr 
            } else {
                last->next = next;
            }
            free(temp);
            temp = next;
            l->len--;
        } else {
            last = temp;
            temp = temp->next;
        }
    }
}

void free_list(chained_list* l) {
    if(l == NULL) return;

    cell* temp = l->first;
    while(temp != NULL) {
        cell* next = temp->next;
        if(temp->data != NULL) free(temp->data);
        free(temp);
        temp = next;
    }
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