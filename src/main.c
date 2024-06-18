#define SDL_MAIN_HANDLED
#define SDL_MAIN_USE_CALLBACKS
#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_main.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "component_ecs.h"
#include "system_ecs.h"
#include "entity_ecs.h"

// copy boilerplate
// dont forget to initalize registry
struct EntityManager manager = {0, {0}, {0}, {0}, {0}};
struct ComponentRegistry registry = {{{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0}}};

SDL_Window* window;
SDL_Renderer* renderer;
SDL_bool app_quit;

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Window", 500, 500, 600, 800, SDL_WINDOW_RESIZABLE);
    if (!window){
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer){
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		return false;
    }

    return true;
}

void close() {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!init()) {
        printf("Failed to initialize");
        return -1;
    }

    SDL_Log("Application started successfully!");

    initEntityManager(&manager);
    srand(time(NULL));

    for (int i = 0; i < 100000; i++) {
      Entity ent = addEntity(&manager, TRANS|COLOR);
      registry.transforms_[manager.entity_to_idx_[ent]] = (struct TransformComponent){rand()%501, rand()%501, rand()%10, rand()%10};
      registry.colors_[manager.entity_to_idx_[ent]] = (struct ColorComponent){rand()%256, rand()%256, rand()%256, 255};
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        if (SDL_GetTicks() % 10 == 0) {
            updateTransforms(&manager, &registry);
            renderColors(&manager, &registry, renderer);
            updateCollisions(&manager, &registry);
        }
    }

    return 0;
}
