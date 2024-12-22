#include "dyn_arrays.h"

struct dyn_array_s {
    int maxlen;
    int len;
    void** list;
};

typedef struct dyn_array_s dyn_array;

dyn_array* create_array() {
    dyn_array* da = malloc(sizeof(dyn_array));
    da->len = 0;
    da->maxlen = 10;
    da->list = malloc(sizeof(void*) * da->maxlen);
    return da;
}

dyn_array* convert(int len, void** list) {
    dyn_array* da = malloc(sizeof(dyn_array));
    da->len = len;
    da->maxlen = 2*da->len;
    da->list = malloc(sizeof(void*) * da->maxlen);

    for(int i = 0; i < len; i++) {
        da->list[i] = list[i];
    }

    return da;
}

void free_array(dyn_array* da);

int get_len(dyn_array* da);
void* get_elt(dyn_array* da, int index);

void set_elt(dyn_array* da, int index, void* elt);
void append(dyn_array* da, void* elt);
void* pop(dyn_array* da);