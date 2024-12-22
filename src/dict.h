#pragma once

#include <stdbool.h>

struct dict_s;
typedef struct dict_s* dict;

dict createDict();
void* getValue(const int x, const int y, dict dico);
bool add(const int x, const int y, void* value, dict dico);
bool removeValue(const int x, const int y, dict dico);
bool mem(const int x, const int y, dict dico);
void destroyDict(dict dico);