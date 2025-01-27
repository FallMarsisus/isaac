#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct dict_s IntDictionary;

IntDictionary* create_int_dictionary();
void free_int_dictionary(IntDictionary* dict);

void* get_from_int_dictionary(IntDictionary* dict, int key);

void add_to_int_dictionary(IntDictionary* dict, int key, void* value);
bool remove_from_int_dictionary(IntDictionary *dict, int key);

void iterate_int_dictionary(IntDictionary* dict, void (*callback)(int key, void* value));