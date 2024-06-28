#include <stdio.h>
#include <SDL2/SDL.h>

int main(void)
{
    printf("Hello World\n");

    SDL_Window *window;     // Declare a window
    SDL_Renderer *renderer; // Declare a renderer

    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2
    // Create an application window with the following settings:
    window = SDL_CreateWindow("Game Window",           // window title
                              SDL_WINDOWPOS_UNDEFINED, // initial x position
                              SDL_WINDOWPOS_UNDEFINED, // initial y position
                              640,                     // width, in pixels
                              480,                     // height, in pixels
                              0                        // flags
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rect = {220, 140, 200, 200};
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);

    SDL_Delay(2000);

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    return 0;

    return 0;
}