#include <stdio.h>
#include <SDL2/SDL.h>
#include "headers/agent_field.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int NUMBER_OF_AGENTS = 10;
const int MAX_ITERATION = 1000;
const int ITERATION_EVERY = 10;

const unsigned int RANDOM_SEED = 0;

int main(void)
{
    srand(RANDOM_SEED);

    char **traceMap = mallocTraceMap(SCREEN_WIDTH, SCREEN_HEIGHT);
    struct Agent *agentArray = mallocAgents(NUMBER_OF_AGENTS);

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Slime Simulation",
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        SCREEN_WIDTH,            // width, in pixels
        SCREEN_HEIGHT,           // height, in pixels
        0                        // flags
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    for (int i = 0; i < MAX_ITERATION; i++)
    {

        iterateAgents(agentArray, NUMBER_OF_AGENTS, traceMap, SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int j = 0; j < SCREEN_WIDTH; j++)
        {
            for (int k = 0; k < SCREEN_HEIGHT; k++)
            {
                char alpha = traceMap[j][k];
                SDL_SetRenderDrawColor(renderer, alpha, alpha, alpha, 255);
                SDL_RenderDrawPoint(renderer, j, k);
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(ITERATION_EVERY);
    }

    freeTraceMap(traceMap, SCREEN_WIDTH);
    freeAgents(agentArray);

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();

    return 0;
}