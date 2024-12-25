#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "display.h"
#include "player.h"
#include "text.h"
#include "rooms.h"
#include "buttons.h"
#include "menu.h"

int main(int argc, char* argv[]) {


    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *win = SDL_CreateWindow("Hello SDL2", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    player joueur = createPlayer(0, 0, 200, 200, "./assets/images/image.bmp", ren);

    if (ren == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }


    // Set the renderer color to blue
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);

    // Clear the window
    SDL_RenderClear(ren);


    // On affiche un sprite dans le terminal
    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    text texte = createText(ren, 100, 100, 50, 150, 255, "Miam", "./assets/fonts/FRESHFACE.ttf", 100);
    changeTextUnderline(texte, true);
    button bouton = createButton(ren, texte, (SDL_Color){0, 255, 0}, (SDL_Color){0, 150, 0}, 0, 10);

    int win_width, win_height;
    SDL_GetWindowSize(win, &win_width, &win_height);

    SDL_Event event;
    int running = 1;
    bool isMenu = true;
    int mouseX, mouseY;

    button* menu = createMainMenuButtons(ren, win_width, win_height);

while (running) {

    // Update mouse coordinates
    SDL_GetMouseState(&mouseX, &mouseY);

    // On vérifie qu'on quitte pas et on attend un appui de touche 
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
        }

        if (isMenu) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                for (int i = 0; i < 3; i++) {
                    if (mouseInButton(menu[i], mouseX, mouseY)) {
                        printf("Button %d clicked\n", i);
                        if (i == 2) running = 0;
                        else isMenu = false;
                    }
                }
            }
        }
    }

    if (isMenu) {
        displayMainMenu(ren, menu, mouseX, mouseY);   
    } else {
        // On get les appuis de touche
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])  move(joueur, -5, 0);
        if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) move(joueur, 5, 0);
        if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])    move(joueur, 0, -5);
        if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])  move(joueur, 0, 5);

        // On teste si on hit les bordures
        clipPlayer(joueur, win_width - 200, win_height - 200);

        // On affiche le sprite et on render
        displayPlayer(joueur, ren);

        // if (mouseInButton(bouton, mouseX, mouseY)) displayButton(ren, bouton, true);
        // else displayButton(ren, bouton, false);
    }

    SDL_RenderPresent(ren);
    newFrame(ren, 0, 0, 0);
}
    

    // Wait for 0.1 second
    SDL_Delay(100);

    // Clean up
    freePlayer(joueur);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}

// Si y'a un problème c'est clairement d'ici que ça vient
int WinMain(int argc, char *argv[]) {
    return main(argc, argv);
}
