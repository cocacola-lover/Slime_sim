#include <stdio.h>
#include <SDL2/SDL.h>
#include "headers/agent_field.h"

const int NUMBER_OF_AGENTS = 10;

int main(void)
{
    char **traceMap = mallocTraceMap(640, 480);
    struct Agent *agentArray = mallocAgents(NUMBER_OF_AGENTS);

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

    for (int i = 0; i < 100; i++)
    {

        iterateAgents(agentArray, NUMBER_OF_AGENTS, traceMap, 640, 480);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int j = 0; j < 640; j++)
        {
            for (int k = 0; k < 480; k++)
            {
                char alpha = traceMap[j][k];
                SDL_SetRenderDrawColor(renderer, alpha, alpha, alpha, 255);
                SDL_RenderDrawPoint(renderer, j, k);
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }

    freeTraceMap(traceMap, 640);
    freeAgents(agentArray);

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();

    return 0;
}