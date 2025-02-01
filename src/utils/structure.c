#include "structure.h"

typedef struct node_s Node;

typedef struct int_dict_s IntDictionary;

typedef struct node_array_s NodeArray;

typedef struct structure_s Structure;

int caca_hash_function(int id, int capacity) {
    return id % capacity;
}

Node* add_to_dict(Structure* st, int key, int value) {
    assert(st != NULL && st->dict != NULL);
    IntDictionary* dict = st->dict;

    if ((float) dict->count / dict->capacity > 0.75) {
        resize_dict(st, dict->capacity * 2);
    }

    int index = caca_hash_function(key, dict->capacity);

    Node* new_pair = malloc(sizeof(Node));
    
    new_pair->key = key;
    new_pair->value = value;

    new_pair->next = dict->array[index];
    dict->array[index] = new_pair;
    dict->count++;

    return new_pair;
}

void add_to_array(NodeArray* nArray, Node* node) {
    assert(nArray != NULL && node != NULL);

    if(nArray->len >= nArray->capacity) {
        nArray->capacity = nArray->capacity * 2;
        nArray->array = realloc(nArray->array, sizeof(Node*) * nArray->capacity);
    }

    nArray->array[nArray->len] = node;
    nArray->len++;
}

Node* remove_last(NodeArray* nArray) {
    assert(nArray != NULL && nArray->len > 0);

    if(nArray->len * 4 < nArray->capacity) {
        int new_len = nArray->len * 2;
        Node** new_list = malloc(sizeof(Node*) * new_len);

        for(int i = 0; i < nArray->len; i++) {
            new_list[i] = nArray->array[i];
        }

        free(nArray->array);
        nArray->array = new_list;
        nArray->capacity = new_len;
    }

    nArray->len--;
    return nArray->array[nArray->len];
}

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

void resize_dict(Structure* st, int new_capacity) {
    assert(st != NULL && st->dict != NULL);
    Node** prev_array = st->dict->array;
    int prev_capacity = st->dict->capacity;

    st->dict->array = calloc(new_capacity, sizeof(Node*));
    st->dict->capacity = new_capacity;
    st->dict->count = 0;

    for (int i = 0; i < prev_capacity; i++) {
        Node* current = prev_array[i];
        while (current) {
            //Add to dict and change the array
            Node* added = add_to_dict(st, current->key, current->value);
            if(added->value < st->array->capacity) st->array->array[added->value] = added;
            else return;

            //Free the node and go next
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(prev_array);
}

//Actual Functions
Structure* create_structure() {
    Structure* st = malloc(sizeof(Structure));

    //Array
    st->array = malloc(sizeof(NodeArray));
    st->array->capacity = 200;
    st->array->array = calloc(st->array->capacity, sizeof(Node*));
    st->array->len = 0;

    //Dictionary
    st->dict = malloc(sizeof(IntDictionary));
    st->dict->capacity = 100;
    st->dict->array = calloc(st->dict->capacity, sizeof(Node*));
    st->dict->count = 0;

    return st;
}

void free_structure(Structure* st) {
    for (int i = 0; i < st->dict->capacity; i++) {
        Node* current = st->dict->array[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp); // Free the full linked list
        }
    }
    free(st->dict->array);
    free(st->dict);

    free(st->array->array);
    free(st->array);

    free(st);
}

int get_index(Structure* st, int id) {
    Node* node = get_node(st->dict, id);
    if(!node) return -1;
    return node->value;
}

int get_structure_len(Structure* st) {
    return st->array->len;
}
int get_structure_capacity(Structure* st) {
    return st->array->capacity;
}

int create_element(Structure* st) {
    int id = st->array->len;
    int index = st->array->len;
    Node* node = add_to_dict(st, id, index);
    add_to_array(st->array, node);

    return id;
}
int remove_element(Structure* st, int id) {
    Node* to_remove = get_node(st->dict, id);
    if (!to_remove) return -1; // Avoid NULL dereference

    Node* temp = remove_last(st->array);

    int index_switch = temp->value;

    temp->value = to_remove->value;
    st->array->array[to_remove->value] = temp;

    // Update dictionary to reflect new value for temp->key
    get_node(st->dict, temp->key)->value = temp->value;

    remove_from_the_dictionary(st, id);

    return index_switch;
}
