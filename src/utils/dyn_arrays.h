#ifndef DYN_ARRAYS_H_
#define DYN_ARRAYS_H_

#include <stdlib.h>

typedef struct dyn_array_s dyn_array;

dyn_array* create_array();
dyn_array* convert(int len, void** list);

void free_array(dyn_array* da);

int get_len(dyn_array* da);
void* get_elt(dyn_array* da, int index);

void set_elt(dyn_array* da, int index, void* elt);
void append(dyn_array* da, void* elt);
void* pop(dyn_array* da);

#endif