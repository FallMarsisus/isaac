#include <stdio.h>
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
    da->maxlen = 0;
    da->list = malloc(sizeof(void*) * da->maxlen);
    return da;
}

dyn_array* convert(int len, void** list) {
    assert(list != NULL);
    dyn_array* da = malloc(sizeof(dyn_array));
    da->len = len;
    da->maxlen = 2*da->len;
    da->list = malloc(sizeof(void*) * da->maxlen);

    for(int i = 0; i < len; i++) {
        da->list[i] = list[i];
    }

    return da;
}

void free_array(dyn_array* da) {
    assert(da != NULL); 
    free(da->list);
    free(da);
}

int get_len(dyn_array* da) {
    assert(da != NULL);
    return da->len;
}
void* get_elt(dyn_array* da, int index) {
    assert(da != NULL);
    assert(0 <= index && index < da->len);
    return da->list[index];
}

void set_elt(dyn_array* da, int index, void* elt) {
    assert(da != NULL);
    assert(0 <= index && index < da->len);
    assert(elt != NULL);
    da->list[index] = elt;
}

void append(dyn_array* da, void* elt) {
    assert(da != NULL && elt != NULL);

    if(da->len >= da->maxlen) {
        int new_len = da->maxlen * 2 + 1;
        void** new_list = malloc(sizeof(void*) * new_len);

        for(int i = 0; i < da->len; i++) {
            new_list[i] = da->list[i];
        }
        free(da->list);
        da->list = new_list;
        da->maxlen = new_len;
    }
    da->list[da->len] = elt;
    da->len++;
}

void* pop(dyn_array* da) {
    assert(da != NULL);
    assert(da->len > 0);

    if(da->len * 4 < da->maxlen) {
        int new_len = da->len * 2;
        void** new_list = malloc(sizeof(void*) * new_len);

        for(int i = 0; i < da->len; i++) {
            new_list[i] = da->list[i];
        }
        free(da->list);
        da->list = new_list;
        da->maxlen = new_len;
    }
    da->len--;
    return da->list[da->len];
}

void* remove_dynarr(dyn_array* da, int index) {
    if (da == NULL) {
		fprintf(stderr, "\033[1;31mError: dyn_array is NULL\033[0m\n");
		exit(EXIT_FAILURE);
	}
	
	if (index < 0 || index >= da->len ) {
		fprintf(stderr, "\033[1;31mError: index %d is out of bounds\033[0m\n", index);
		exit(EXIT_FAILURE);
	}

	
    if(da->len * 4 < da->maxlen) {
        int new_len = da->len * 2;
        void** new_list = malloc(sizeof(void*) * new_len);

        for(int i = 0; i < da->len; i++) {
            new_list[i] = da->list[i];
        }
        free(da->list);
        da->list = new_list;
        da->maxlen = new_len;
    }
	
	void* elt = da->list[index];

	for (int i = index; i < da->len-1; i++) {
		da->list[i] = da->list[i+1];
	} 

	da->len--;
	return elt;
}
