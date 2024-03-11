#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>
#include "Engine.h"

using namespace std;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Engine::mesh globalMesh;
Engine::mat4x4 globalMat;

int main(int argc, char** argv) {
    Engine::initialize(SCREEN_WIDTH, SCREEN_HEIGHT, globalMesh, globalMat);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("error");
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL Cool", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("error");
        return -1;
    }

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    float deltaTime = 0.0f;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
        }

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        float deltaTime = (float)((NOW - LAST) / (float)SDL_GetPerformanceFrequency());

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        Engine::transform(globalMesh, globalMat, SCREEN_WIDTH, SCREEN_HEIGHT, *renderer, deltaTime);
        SDL_RenderPresent(renderer);
    }   

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

    

}