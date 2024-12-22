#include "../../src/game/rooms.h"

int main() {
    map* m = create_map();
    room* r = create_room(0, 0);
    add_room(m, r);
    destroy_room(m, r);
    free_map(m);
    return 0;
}