#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include "menu/menuManager.h"

#include <time.h>

#include "display.h"

int running = 1;
double dt = 1/60.;

void on_quit(Event event) {
    running = 0;
}

int main() {
    srand(time(NULL));

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
    {
        SDL_Log("Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    // Create a window
	int true_width = 1280, true_height = 720;
    SDL_Window* win = SDL_CreateWindow("Hello SDL2", 100, 100, true_width, true_height, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    init_renderer(win);

    load_sprites();
	load_fonts(get_renderer());
    
    double t = 0.;

    double current_time = SDL_GetTicks() / 1000.;
    double accumulator = 0.;

    ECS_CreateManager();

    init_event_system();
	initDefaultItems();
    init_timer_system();

    register_listener(EVENT_QUIT, on_quit);
    
    register_listener(EVENT_PLAYER_MOVED, on_player_move);
    register_listener(EVENT_CHEST_OPENED, on_chest_open);
    register_listener(EVENT_STATE_CHANGE, on_state_change);
    register_listener(EVENT_COLLISION, on_collision);
    register_listener(EVENT_ENTITY_CREATED, on_entity_created);
    register_listener(EVENT_ENTITY_REMOVED, on_entity_removed);

    init_menu_manager(win, get_renderer());

    Mix_Music* music = Mix_LoadMUS("assets/AssetPack/Musics/21 - Dungeon.ogg");
    if (music == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement de la musique : %s", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }
    Mix_PlayMusic(music, -1); // Joue notre musique
    Mix_VolumeMusic(16); // Mets le volume a 0

    SDL_Event event;
    while (running) {
        while(SDL_PollEvent(&event)) {
            handle_menu_manager_input(event);
        }

        double new_time = SDL_GetTicks() / 1000.;
        double frame_time = new_time - current_time;
        if(frame_time > 0.25) frame_time = 0.25;
        current_time = new_time;

        accumulator += frame_time;

        while(accumulator >= dt) {
            update_menu_manager(dt);
            t += dt;
            accumulator -= dt;
        }
        
        draw_menu_manager(get_renderer(), true_width, true_height);
        SDL_RenderPresent(get_renderer());
    }

    unregister_listener(EVENT_QUIT, on_quit);
    unregister_listener(EVENT_PLAYER_MOVED, on_player_move);
    unregister_listener(EVENT_CHEST_OPENED, on_chest_open);
    unregister_listener(EVENT_STATE_CHANGE, on_state_change);
    unregister_listener(EVENT_COLLISION, on_collision);
    unregister_listener(EVENT_ENTITY_CREATED, on_entity_created);
    unregister_listener(EVENT_ENTITY_REMOVED, on_entity_removed);

    Mix_FreeMusic(music); // Libére en mémoire notre musique

    free_menu_manager();

    ECS_DestroyManager();

    shutdown_timer_system();
    free_event_system();
	freeDefaultItems();
	free_fonts();

    Mix_CloseAudio();

    SDL_DestroyRenderer(get_renderer());
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
