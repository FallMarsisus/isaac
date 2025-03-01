#include "int_dict.h"

// Define a key-value pair
typedef struct node_s {
    int key;
    void* value;
    struct node_s* next;
} Node;

typedef struct dict_s {
    Node** array;
    int capacity;
    int size;
} IntDictionary;

int hash_int_function(int key, int capacity) {
    return key % capacity;
}

IntDictionary* create_int_dictionary() {
    IntDictionary *dict = malloc(sizeof(IntDictionary));
    dict->capacity = 10;
    dict->array = calloc(dict->capacity, sizeof(Node*));
    dict->size = 0;
    return dict;
}
void free_int_dictionary(IntDictionary* dict) {
    for (int i = 0; i < dict->capacity; i++) {
        Node* current = dict->array[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(dict->array);
    free(dict);
}

void resize_int_dictionary(IntDictionary* dict, int new_capacity) {
    Node** prev_array = dict->array;
    int prev_capacity = dict->capacity;

    dict->array = calloc(new_capacity, sizeof(Node*));
    dict->capacity = new_capacity;
    dict->size = 0;

    for (int i = 0; i < prev_capacity; i++) {
        Node* current = prev_array[i];
        while (current) {
            add_to_int_dictionary(dict, current->key, current->value);
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(prev_array);
}

void* get_from_int_dictionary(IntDictionary* dict, int key) {
    int index = hash_int_function(key, dict->capacity);
    Node* current = dict->array[index];
    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return NULL; // Key not found
}

void add_to_int_dictionary(IntDictionary* dict, int key, void* value) {
    if ((float) dict->size / dict->capacity > 0.75) {
        resize_int_dictionary(dict, dict->capacity * 2);
    }

    int index = hash_int_function(key, dict->capacity);
    Node* new_pair = malloc(sizeof(Node));
    new_pair->value = value;
    new_pair->next = dict->array[index];
    dict->array[index] = new_pair;
    dict->size++;
}

bool remove_from_int_dictionary(IntDictionary* dict, int key) {
    int index = hash_int_function(key, dict->capacity);
    Node* current = dict->array[index];
    Node* previous = NULL;

    while (current) {
        if (key == current->key) {
            if (previous) {
                previous->next = current->next;
            } else {
                dict->array[index] = current->next;
            }

            free(current);

            dict->size--;

            if (4 * dict->size < dict->capacity && dict->capacity > 8) {
                resize_int_dictionary(dict, dict->capacity / 2);
            }

            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

void iterate_int_dictionary(IntDictionary* dict, void (*callback)(int key, void* value)) {
    for (int i = 0; i < dict->capacity; i++) {
        Node* current = dict->array[i];
        while (current) {
            callback(current->key, current->value);
            current = current->next;
        }
    }
}