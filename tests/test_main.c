#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../src/game/map.h"

void print_node(int x, int y, void* data) {
    print_room((Room*) data);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Hello SDL2", 100, 100, 640, 360, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    map* m = create_map();

    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            Room* r = create_room(i, j);
            add_room(m, r);
        }
    }

    print_map(m);
    printf("\nRemoing\n");

    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            print_node(i, j, get_room(m, i, j));
            destroy_room(m, i, j);
        }
    }

    printf("\nRemoved\n");
    print_map(m);
    int x = 0, y = 0;
    while(1 == 1) {
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT]) x -= 5;
        if (state[SDL_SCANCODE_RIGHT]) x += 5;
        if (state[SDL_SCANCODE_UP]) y -= 5;
        if (state[SDL_SCANCODE_DOWN]) y += 5;


        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255 );
        SDL_RenderClear(ren);
        
        SDL_Rect dstrect = { x, y, 20, 20 };
        SDL_SetRenderDrawColor(ren , 0, 0, 255, 255 );

        SDL_RenderPresent(ren);
    }

    free_map(m);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}