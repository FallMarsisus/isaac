#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct dict_s Dictionary;

Dictionary* create_dictionary();
void free_dictionary(Dictionary* dict);

void* get_from_dictionary(Dictionary* dict, char* key);

void add_to_dictionary(Dictionary* dict, char* key, void* value);
bool remove_from_dictionary(Dictionary *dict, char* key);