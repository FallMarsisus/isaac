struct dict;

dict create();
void* getValue(int x, int y, dict dico);
void add(int x, int y, void* value, dict dico);
bool remove(int x, int y, dict dico);
