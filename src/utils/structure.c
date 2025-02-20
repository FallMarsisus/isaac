#include "structure.h"

// Hash function
int caca_hash_function(int id, int capacity) {
    return id % capacity;
}

// Add a key-value pair to the dictionary
Node* add_to_dict(Structure* st, int key, int value) {
    assert(st != NULL && st->dict != NULL);
    IntDictionary* dict = st->dict;

    // Resize if load factor exceeds 0.75
    if ((float)dict->count / dict->capacity > 0.75) {
        resize_dict(st, dict->capacity * 2);
    }

    int index = caca_hash_function(key, dict->capacity);

    Node* new_pair = malloc(sizeof(Node));
    if (!new_pair) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    new_pair->key = key;
    new_pair->value = value;
    new_pair->next = dict->array[index];
    dict->array[index] = new_pair;
    dict->count++;

    return new_pair;
}

// Add a node to the array
void add_to_array(NodeArray* nArray, Node* node) {
    assert(nArray != NULL && node != NULL);

    // Resize if array is full
    if (nArray->len >= nArray->capacity) {
        nArray->capacity = nArray->capacity * 2;
        Node** new_array = realloc(nArray->array, sizeof(Node*) * nArray->capacity);
        if (!new_array) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        nArray->array = new_array;
    }

    nArray->array[nArray->len] = node;
    nArray->len++;
}

// Remove the last element from the array
Node* remove_last(NodeArray* nArray) {
    assert(nArray != NULL && nArray->len > 0);

    // Resize if array is less than 25% full
    if (nArray->len * 4 < nArray->capacity && nArray->capacity > 8) {
        int new_capacity = nArray->capacity / 2;
        Node** new_array = malloc(sizeof(Node*) * new_capacity);
        if (!new_array) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nArray->len; i++) {
            new_array[i] = nArray->array[i];
        }

        free(nArray->array);
        nArray->array = new_array;
        nArray->capacity = new_capacity;
    }

    nArray->len--;
    return nArray->array[nArray->len];
}

// Get a node from the dictionary by key
Node* get_node(IntDictionary* dict, int key) {
    int index = caca_hash_function(key, dict->capacity);
    Node* current = dict->array[index];
    while (current) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Key not found
}

// Remove a key-value pair from the dictionary
bool remove_from_the_dictionary(Structure* st, int key) {
    IntDictionary* dict = st->dict;
    int index = caca_hash_function(key, dict->capacity);
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
            dict->count--;

            // Resize if load factor is less than 25%
            if (4 * dict->count < dict->capacity && dict->capacity > 8) {
                resize_dict(st, dict->capacity / 2);
            }

            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

// Resize the dictionary
void resize_dict(Structure* st, int new_capacity) {
    assert(st != NULL && st->dict != NULL);
    Node** prev_array = st->dict->array;
    int prev_capacity = st->dict->capacity;

    st->dict->array = calloc(new_capacity, sizeof(Node*));
    if (!st->dict->array) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    st->dict->capacity = new_capacity;
    st->dict->count = 0;

    // Rehash all nodes into the new array
    for (int i = 0; i < prev_capacity; i++) {
        Node* current = prev_array[i];
        while (current) {
            Node* next = current->next;
            int new_index = caca_hash_function(current->key, new_capacity);

            // Insert into the new array
            current->next = st->dict->array[new_index];
            st->dict->array[new_index] = current;
            st->dict->count++;

            current = next;
        }
    }

    free(prev_array);
}

// Create a new structure
Structure* create_structure() {
	Structure* st = malloc(sizeof(Structure));
	if (!st) {
		fprintf(stderr, "\033[1;31mMemory allocation failed\033[0m\n");
		exit(EXIT_FAILURE);
	}

	// Initialize array
	st->array = malloc(sizeof(NodeArray));
	if (!st->array) {
		fprintf(stderr, "\033[1;31mMemory allocation failed\033[0m\n");
		exit(EXIT_FAILURE);
	}
	st->array->capacity = 200;
	st->array->len = 0;
	st->array->array = calloc(st->array->capacity, sizeof(Node*));
	if (!st->array->array) {
		fprintf(stderr, "\033[1;31mMemory allocation failed\033[0m\n");
		exit(EXIT_FAILURE);
	}

	// Initialize dictionary
	st->dict = malloc(sizeof(IntDictionary));
	if (!st->dict) {
		fprintf(stderr, "\033[1;31mMemory allocation failed\033[0m\n");
		exit(EXIT_FAILURE);
	}
	st->dict->capacity = 100;
	st->dict->count = 0;
	st->dict->array = calloc(st->dict->capacity, sizeof(Node*));
	if (!st->dict->array) {
		fprintf(stderr, "\033[1;31mMemory allocation failed\033[0m\n");
		exit(EXIT_FAILURE);
	}

	return st;
}

// Free the structure and all associated memory
void free_structure(Structure* st) {
    if (!st) return;

    // Free dictionary
    if (st->dict) {
        for (int i = 0; i < st->dict->capacity; i++) {
            Node* current = st->dict->array[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(st->dict->array);
        free(st->dict);
    }

    // Free array
    if (st->array) {
        free(st->array->array);
        free(st->array);
    }

    free(st);
}

// Get the index of an element by ID
int get_index(Structure* st, int id) {
    Node* node = get_node(st->dict, id);
    if (!node) return -1;
    return node->value;
}

// Create a new element
int create_element(Structure* st) {
    int id = st->array->len;
    int index = st->array->len;
    Node* node = add_to_dict(st, id, index);
    add_to_array(st->array, node);
    return id;
}

// Remove an element by ID
int remove_element(Structure* st, int id) {
    Node* to_remove = get_node(st->dict, id);
    if (!to_remove) return -1; // Avoid NULL dereference

    Node* temp = remove_last(st->array);

    int index_switch = temp->value;
    temp->value = to_remove->value;
    st->array->array[to_remove->value] = temp;

    // Update dictionary to reflect new value for temp->key
    Node* node = get_node(st->dict, temp->key);
    if(!node) return index_switch;

    node->value = temp->value;

    remove_from_the_dictionary(st, id);

    return index_switch;
}