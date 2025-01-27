#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef struct dictNode_s {
	int x, y;
	void* value;
	struct dictNode_s* next;
} node; //meant to never be discorverd by the user of the lib

typedef struct dict_s {
	int size;
	int count;
	node** array;
}* dict;

const int defaultSize = 8;

int hashFunction(int x, int y, int size) {

	//J'ai volé cette fonction de hash (askip elle est efficace)
	uint64_t hash = 0xcbf29ce484222325; //Base de l'offcet de FNV
	hash ^= (uint64_t)x;
	hash *= 0x100000001b3; // FNV prime
	hash ^= (uint64_t)y;
	hash *= 0x100000001b3;

	return (int) (hash % size);
}


// ========= Node functions ========== (all private)
node* cons(int x, int y, void* value, node* next) {
	node* noeud = malloc(sizeof(node));
	noeud->next = next;
	noeud->x = x;
	noeud->y = y;
	noeud->value = value;
	return noeud;
}

node* addInNode(int x, int y, void* value, node* head, bool* isRealAddition) {

	if (head == NULL) {
		return cons(x, y, value, NULL);
	}

	node* current = head;

	//Check chaque noeud
	while (current->next != NULL)	{
		if (current->x == x && current->y == y) {
			current->value = value;
			*isRealAddition = false;
			return head;
		}
		current = current->next;
	}

	//check le dernier noeud
	if (current->x == x && current->y == y) {
			current->value = value;
			*isRealAddition = false;
			return head;
	}
	
	//Ajoute la valeur si elle ne correspondait à aucun noeud
	current->next = cons(x, y, value, NULL);
	*isRealAddition = true;
	return head;
}

node* removeOneNode(int x, int y, node* noeud, bool* hasRemoved) {
	if (noeud == NULL) {
		fprintf(stderr, "No element to remove");
		*hasRemoved = false;
		return NULL;
	} else if (noeud->x == x && noeud->y == y) {
		node* next = noeud->next;
		*hasRemoved = true;
		free(noeud);
		return next;
	} else {
		noeud->next = removeOneNode(x, y, noeud->next, hasRemoved);
		return noeud;
	}
}

void* getValueInNode(int x, int y, node* noeud) {

    while (noeud != NULL) {
		if (noeud->x == x && noeud->y == y) {
			return noeud->value;
		}
        noeud = noeud->next;
    }

    return NULL;
}

bool isIsNode(int x, int y, node* noeud) {
	while (noeud != NULL) {
		if (noeud->x == x && noeud->y == y) {
			return true;
		}
		noeud = noeud->next;
	}
	return false;
}

void freeNode(node* noeud, bool freeElements) {
	if (noeud == NULL) {
		return;
	}
	node* next = noeud->next;
	while (noeud->next != NULL) {
		if (freeElements) free(noeud->value);
		free(noeud);
		noeud = next;
		next = noeud->next;
	}
	free(noeud);
}

// ========= Dict functions =========

//Private
void resize(dict dico, int newSize) {
	node** newArr = malloc(sizeof(node*)*newSize);
	for (int i = 0; i < newSize; i++) {
		newArr[i] = NULL;
	}

	node* current = NULL;
	int h;
	for (int i = 0; i < dico->size; i++) {
		current = dico->array[i];
		while (current != NULL) {
			h = hashFunction(current->x, current->y, newSize);
			newArr[h] = cons(current->x, current->y, current->value, newArr[h]);
			current = current->next;
		}
	}

	for (int i = 0; i < dico->size; i++) {
		freeNode(dico->array[i], false);
	}

	node** oldArr = dico->array;
	dico->array = newArr;
	dico->size = newSize;

	free(oldArr);
}


// public
dict create_dict() {
	node** arr = malloc(sizeof(node*)*defaultSize);
	for (int i = 0; i < defaultSize; i++) {
		arr[i] = NULL;
	}

	dict dico = malloc(sizeof(struct dict_s));
	dico->array = arr;
	dico->count = 0;
	dico->size = defaultSize;
	return dico;
}

void* getValue(const int x, const int y, dict dico) {
	//Return null if value does not exist

	int h = hashFunction(x, y, dico->size);
	return getValueInNode(x, y, dico->array[h]);
}

bool add(const int x, const int y, void* value, dict dico) {

	//Si le nombre d'éléments devient plus grand que la taille du dico
	if (dico->count + 1 >= dico->size) {
		resize(dico, dico->size * 2);
	}

	int h = hashFunction(x, y, dico->size);
	bool isRealAddition = true;
	dico->array[h] = addInNode(x, y, value, dico->array[h], &isRealAddition);

	if (isRealAddition) dico->count++;

	return isRealAddition;
}

bool removeValue(const int x, const int y, dict dico) {

	// Si le nombre d'éléments est bcp plus petit que la taille du dico
	if (dico->count * 4 < dico->size + 4 && dico->size >= 2*defaultSize) {
		resize(dico, dico->size / 2);
	}

	int h = hashFunction(x, y, dico->size);
	bool hasBeenRemoved;
	dico->array[h] = removeOneNode(x, y, dico->array[h], &hasBeenRemoved);

	if (hasBeenRemoved) dico->count--;

	return hasBeenRemoved;
}

bool mem(const int x, const int y, dict dico) {
	return isIsNode(x, y, dico->array[hashFunction(x, y, dico->size)]);
}

void free_dict(dict dico, bool freeElements) {
	for (int i = 0; i < dico->size; i++) {
		freeNode(dico->array[i], freeElements);
	}
	free(dico->array);
	free(dico);
}

void iter_dict(void (*f)(int, int, void*), dict dico) {
	for(int i = 0; i < dico->size; i++) {
		node* current = dico->array[i];
		if(current != NULL) {
			while(current) {
				f(current->x, current->y, current->value);
				current = current->next;
			}
		}
	}
}