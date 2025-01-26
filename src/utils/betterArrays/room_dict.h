#ifndef DICT_H_
#define DICT_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef struct dict_s* dict;

//Casually stolen from Antonin

dict create_dict();
void free_dict(dict dico ,bool free_elts);

void* getValue(int x, int y, dict dico);
bool mem(const int x, const int y, dict dico);

void add(int x, int y, void* value, dict dico);
bool removeValue(int x, int y, dict dico);
void iter_dict(void (*f)(int, int, void*), dict dico);

#endif