#include "dict.h"

typedef struct dictNode_s {
	int x, y;
	void* value;
	struct dictNode_s* next;
} node;

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


// ========= Node functions ==========
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

	fprintf(stderr, "/!\\ Element does not exist\n");
	exit(EXIT_FAILURE);
}


// ========= Dict functions =========
dict create() {
	node** arr = malloc(sizeof(node*)*defaultSize);
	dict dico = malloc(sizeof(struct dict_s));
	dico->array = arr;
	dico->count = 0;
	dico->size = defaultSize;
	return dico;
}

void free_dict(dict dico) {
	for(int i = 0; i < dico->size; i++) {
		node* current = dico->array[i];
		if(current == NULL) continue;
		while(current != NULL) {
			node* temp = current->next;
			free(current->value);
			free(current);
			current = temp;
		}
	}
	free(dico->array);
	free(dico);
}

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

	node** oldArr = dico->array;
	dico->array = newArr;
	dico->size = newSize;

	free(oldArr);
}

void* getValue(int x, int y, dict dico) {
	int h = hashFunction(x, y, dico->size);
	return getValueInNode(x, y, dico->array[h]);
}

void add(int x, int y, void* value, dict dico) {

	//Si le nombre d'éléments devient plus grand que la talle du dico
	if (dico->count + 1 >= dico->size) {
		resize(dico, dico->size * 2);
	}

	int h = hashFunction(x, y, dico->size);
	bool isRealAddition;
	dico->array[h] = addInNode(x, y, value, dico->array[h], &isRealAddition);

	if (isRealAddition) dico->count++;
}

bool removeValue(int x, int y, dict dico) {

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