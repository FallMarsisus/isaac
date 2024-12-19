
typedef struct entity_s entity;

//Listes doublement chainées pour une suppression facile en o(1)
typedef struct list_entity_s list_entity;

list_entity create_list();
void destroy_list(list_entity l);

int get_len(list_entity l);
entity* get_first(list_entity l);
entity* get_last(list_entity l);

void append(list_entity l, entity* e);
void remove(list_entity l, entity* e);