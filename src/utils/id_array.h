#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef struct id_array_s ID_array;

ID_array* create_id_array();
void free_id_array(ID_array* arr);

int get_ids_len(ID_array* arr);
uint32_t get_id(ID_array* arr, int index);
uint32_t* get_ids(ID_array* arr);

void add_id(ID_array* arr, uint32_t id);
void remove_id(ID_array* arr, uint32_t id);