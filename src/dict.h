struct dict_s;
typedef struct dict_s* dict;

#include <stdbool.h>

dict create();
void* getValue(int x, int y, dict dico);
void add(int x, int y, void* value, dict dico);
bool removeValue(int x, int y, dict dico);
