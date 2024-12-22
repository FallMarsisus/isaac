#include "../../src/game/entities.h"
#include "../../src/game/list_entities.h"

#include <stdio.h>

int main() {
    list_entity l = create_list();

    entity* custom = create_entity(&(SDL_Rect){99, 99, 99, 99}, NULL);
    for(int i = 0; i < 6; i++) {
        SDL_Rect* rect = malloc(sizeof(SDL_Rect));
        rect->x = i; rect->y = 0; rect->w = 0; rect->h = -1;
        if(i == 3) append_entity(l, custom);
        append_entity(l, create_entity(rect, NULL));
    }
    iter(l, &print_entity);

    printf("%d\n", get_len(l));
    print_entity(get_first(l));

    printf("Removing\n");
    remove_entity(l, get_first(l));
    remove_entity(l, custom);

    printf("%d\n", get_len(l));
    iter(l, &print_entity);

    printf("Destroying\n");
    destroy_list(l);

    return 0;
}