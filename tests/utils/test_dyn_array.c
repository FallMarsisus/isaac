#include "../../src/utils/dyn_arrays.h"
#include <stdio.h>


void print_int(void* i) {
    printf("%d, ", *((int*) i));
}

void print_array(dyn_array* da) {
    for(int i = 0; i < get_len(da); i++) {
        print_int(get_elt(da, i));
    }
    printf("\n");
}

int main() {
    dyn_array* l = create_array();

    int* e = malloc(sizeof(int));
    *e = 5;
    append(l, e);
    for(int i = 0; i < 10; i++) {
        int* e = malloc(sizeof(int));
        *e = i;
        append(l, e);
        print_array(l);
    }

    for(int i = 0; i < 5; i++) {
        print_int(pop(l));
        print_array(l);
    }


    free_array(l);
    return 0;
}