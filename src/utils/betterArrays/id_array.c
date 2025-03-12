#include "id_array.h"

typedef struct id_array_s {
    uint32_t* ids;
    int len;
    int capacity;
} ID_array;

ID_array* create_id_array() {
    ID_array* arr = (ID_array*)malloc(sizeof(ID_array));
    arr->len = 0;
    arr->capacity = 10;
    arr->ids = (uint32_t*)malloc(arr->capacity * sizeof(uint32_t));
    return arr;
}
void free_id_array(ID_array* arr) {
    free(arr->ids);
    free(arr);
}

int get_ids_len(ID_array* arr) {
    return arr->len;
}
uint32_t get_id(ID_array* arr, int index) {
    assert(index < arr->len);
    return arr->ids[index];
}
uint32_t* get_ids(ID_array* arr) {
    return arr->ids;
}

int get_i(ID_array* arr, uint32_t id) {
    for(int i = 0; i < arr->len; i++) {
        if(arr->ids[i] == id) return i;
    }
    return -1;
}
void add_id(ID_array* arr, uint32_t id) {
    if(get_i(arr, id) != -1) return;
    if(arr->len >= arr->capacity) {
        arr->capacity *= 2;
        arr->ids = (uint32_t*)realloc(arr->ids, arr->capacity * sizeof(uint32_t));
    }
    arr->ids[arr->len++] = id;
}
void remove_id(ID_array* arr, uint32_t id) {
    int i = get_i(arr, id);
    if(i == -1) return;

    arr->ids[i] = arr->ids[arr->len - 1];
    arr->len--;

    if(arr->len < arr->capacity /  4) {
        arr->capacity /= 2;
        arr->ids = (uint32_t*)realloc(arr->ids, arr->capacity * sizeof(uint32_t));
    }
}