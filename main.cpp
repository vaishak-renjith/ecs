#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <vector>

using namespace std;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *win = SDL_CreateWindow("Engine", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Set the renderer color (blue)
    SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);

    // Clear the window
    SDL_RenderClear(ren);

    // Show the window
    SDL_RenderPresent(ren);

    // Wait for 5 seconds
    SDL_Delay(5000);

    // Cleanup
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;   
}