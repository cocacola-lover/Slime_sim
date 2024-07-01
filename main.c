#include <stdio.h>
#include <SDL2/SDL.h>
#include "headers/agent_field.h"

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 180;

const int NUMBER_OF_AGENTS = 250;
const int MAX_ITERATION = 1000;
const int ITERATION_EVERY = 10;

const unsigned int RANDOM_SEED = 0;

int processEvents()
{
    SDL_Event event;
    int done = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
            done = 1;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                done = 1;
            break;
        case SDL_QUIT:
            done = 1;
            break;
        }
    }

    return done;
}

int main(void)
{
    srand(RANDOM_SEED);

    struct TraceMap traceMap = mallocTraceMap(SCREEN_WIDTH, SCREEN_HEIGHT);
    struct Agent *agentArray = mallocAgents(NUMBER_OF_AGENTS);

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Slime Simulation",
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        SCREEN_WIDTH * 4,        // width, in pixels
        SCREEN_HEIGHT * 4,       // height, in pixels
        0                        // flags
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetScale(renderer, 4, 4);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < MAX_ITERATION; i++)
    {
        if (processEvents() != 0)
            break;

        iterateSimulation(agentArray, NUMBER_OF_AGENTS, &traceMap);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int j = 0; j < SCREEN_WIDTH; j++)
        {
            for (int k = 0; k < SCREEN_HEIGHT; k++)
            {
                char alpha = traceMap.curMap[j][k] * 255;
                SDL_SetRenderDrawColor(renderer, 112, 255, 219, alpha);
                SDL_RenderDrawPoint(renderer, j, k);
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(ITERATION_EVERY);
    }

    freeTraceMap(traceMap);
    freeAgents(agentArray);

    // Close and destroy the window
    if (window)
        SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();

    return 0;
}