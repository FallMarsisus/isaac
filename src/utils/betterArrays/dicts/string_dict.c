#include "string_dict.h"

// Define a key-value pair
typedef struct node_s {
    char* key;
    void* value;
    struct node_s* next;
} Node;

typedef struct dict_s {
    Node** array;
    int capacity;
    int size;
} Dictionary;

int hash_function(char* key, int capacity) {
    unsigned long hash = 5381;
    int c;

    while (c = *key++) hash = ((hash << 5) + hash) + c;
    return (hash % capacity) % capacity;
}

Dictionary* create_dictionary() {
    Dictionary *dict = malloc(sizeof(Dictionary));
    dict->capacity = 20;
    dict->array = calloc(dict->capacity, sizeof(Node*));
    dict->size = 0;
    return dict;
}
void free_dictionary(Dictionary* dict) {
    for (int i = 0; i < dict->capacity; i++) {
        Node* current = dict->array[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(dict->array);
    free(dict);
}

void resize_dictionary(Dictionary* dict, int new_capacity) {
    Node** prev_array = dict->array;
    int prev_capacity = dict->capacity;

    dict->array = calloc(new_capacity, sizeof(Node*));
    dict->capacity = new_capacity;
    dict->size = 0;

    for (int i = 0; i < prev_capacity; i++) {
        Node* current = prev_array[i];
        while (current) {
            add_to_dictionary(dict, current->key, current->value);
            Node* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }

    free(prev_array);
}

void* get_from_dictionary(Dictionary* dict, char* key) {
    int index = hash_function(key, dict->capacity);
    Node* current = dict->array[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL; // Key not found
}

void add_to_dictionary(Dictionary* dict, char* key, void* value) {
    if ((float) dict->size / dict->capacity > 0.75) {
        resize_dictionary(dict, dict->capacity * 2);
    }

    int index = hash_function(key, dict->capacity);
    Node* new_pair = malloc(sizeof(Node));
    new_pair->key = strdup(key);
    new_pair->value = value;
    new_pair->next = dict->array[index];
    dict->array[index] = new_pair;
    dict->size++;
}
bool remove_from_dictionary(Dictionary* dict, char* key) {
    int index = hash_function(key, dict->size);
    Node* current = dict->array[index];
    Node* previous = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (previous) {
                previous->next = current->next;
            } else {
                dict->array[index] = current->next;
            }

            free(current->key);
            free(current);

            dict->size--;

            if (4 * dict->size < dict->capacity && dict->capacity > 8) {
                resize_dictionary(dict, dict->capacity / 2);
            }

            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

void iterate_dictionary(Dictionary* dict, void (*callback)(char* key, void* value)) {
    for (size_t i = 0; i < dict->capacity; i++) {
        Node* current = dict->array[i];
        while (current) {
            callback(current->key, current->value);
            current = current->next;
        }
    }
}