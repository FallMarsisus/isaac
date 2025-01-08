#include "../../src/utils/chained_list.h"

void print_int(void* i) {
    printf("an int : %d\n", *((int*) i));
}

int main() {
    chained_list* l = create_list();

    int* e = malloc(sizeof(int));
    *e = 5;
    append_elt(l, e);
    for(int i = 0; i < 10; i++) {
        int* e = malloc(sizeof(int));
        *e = i;
        append_elt(l, e);
    }
    iter(l, print_int);

    remove_elt(l, e);
    iter(l, print_int);

    free_list(l);
    return 0;
}