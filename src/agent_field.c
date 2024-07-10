#include <stdio.h>
#include <stdlib.h>
#define __USE_MISC
#include <math.h>

#include "../headers/agent_field.h"

const float FADING_COEF = 0.005;
// Cannot be more than M_PI / 2 and be less than 0
const float MAX_RANDOM_NUDGE = M_PI / 4;

// To avoid wasting time on memory allocation TraceMap simply switches frames
void swapTraceMap(struct TraceMap *traceMap)
{
    float **t = traceMap->curMap;
    traceMap->curMap = traceMap->oldMap;
    traceMap->oldMap = t;
}

struct TraceMap mallocTraceMap(int width, int height)
{
    float **curMap = malloc(width * sizeof(float *));
    float **oldMap = malloc(width * sizeof(float *));

    for (int i = 0; i < width; i++)
    {
        curMap[i] = malloc(height * sizeof(float));
        oldMap[i] = malloc(height * sizeof(float));

        for (int j = 0; j < height; j++)
        {
            curMap[i][j] = 0;
            oldMap[i][j] = 0;
        }
    }

    struct TraceMap traceMap;
    traceMap.oldMap = oldMap;
    traceMap.curMap = curMap;
    traceMap.width = width;
    traceMap.height = height;

    return traceMap;
}

void freeTraceMap(struct TraceMap traceMap)
{
    for (int i = 0; i < traceMap.width; i++)
    {
        free(traceMap.oldMap[i]);
        free(traceMap.curMap[i]);
    }

    free(traceMap.oldMap);
    free(traceMap.curMap);
}

struct Agent
{
    float x, y;  // x, y describe position
    float angle; // angle describes the angle of movement
};

struct Agent *mallocAgents(int numberOfAgents)
{
    struct Agent *agentArray = malloc(numberOfAgents * sizeof(struct Agent));

    for (int i = 0; i < numberOfAgents; i++)
    {
        float angle = (float)rand() / (float)(RAND_MAX / (M_PI * 2));
        // float angle = 1;

        struct Agent newAgent = {160, 90, angle};
        agentArray[i] = newAgent;
    }

    return agentArray;
}

void freeAgents(struct Agent *agentArray)
{
    free(agentArray);
}

void fadeTrails(struct TraceMap *traceMap)
{
    for (int i = 0; i < traceMap->width; i++)
    {
        for (int j = 0; j < traceMap->height; j++)
        {
            // Apply blur
            float sum = 0;
            float counter = 0;
            for (int k = (i == 0 ? 0 : i - 1); k < (i == traceMap->width - 1 ? traceMap->width : i + 2); k++)
            {
                for (int l = (j == 0 ? 0 : j - 1); l < (j == traceMap->height - 1 ? traceMap->height : j + 2); l++)
                {
                    sum += traceMap->oldMap[k][l];
                    counter += 1;
                }
            }

            traceMap->curMap[i][j] = fmax(sum / counter - FADING_COEF, 0);
        }
    }
}

void drawTrace(struct Agent *agentPointer, struct TraceMap *traceMap)
{
    traceMap->curMap[(int)roundf(agentPointer->x)][(int)roundf(agentPointer->y)] = 1;
}

void iterateAgents(struct Agent *agentArray, int agentNumber, struct TraceMap *traceMap)
{
    for (int i = 0; i < agentNumber; i++)
    {
        float dx = cos(agentArray[i].angle);
        float dy = sin(agentArray[i].angle);

        if (agentArray[i].x + dx < 0)
        {
            float randomNudge = (float)rand() / (float)(RAND_MAX / MAX_RANDOM_NUDGE);

            agentArray[i].angle += (M_PI / 2 + randomNudge) * (dy >= 0 ? -1.0 : 1.0);
            dx = cos(agentArray[i].angle);
            dy = sin(agentArray[i].angle);
        }
        // Lessen bound by 0.5 so rounding in drawTrace does not round out of bounds
        else if (agentArray[i].x + dx >= traceMap->width - 0.5)
        {
            float randomNudge = (float)rand() / (float)(RAND_MAX / MAX_RANDOM_NUDGE);

            agentArray[i].angle += (M_PI / 2 + randomNudge) * (dy >= 0 ? 1.0 : -1.0);
            dx = cos(agentArray[i].angle);
            dy = sin(agentArray[i].angle);
        }
        else if (agentArray[i].y + dy < 0)
        {
            float randomNudge = (float)rand() / (float)(RAND_MAX / MAX_RANDOM_NUDGE);

            agentArray[i].angle += (M_PI / 2 + randomNudge) * (dx >= 0 ? 1.0 : -1.0);
            dx = cos(agentArray[i].angle);
            dy = sin(agentArray[i].angle);
        }
        else if (agentArray[i].y + dy >= traceMap->height - 0.5)
        {
            float randomNudge = (float)rand() / (float)(RAND_MAX / MAX_RANDOM_NUDGE);

            agentArray[i].angle += (M_PI / 2 + randomNudge) * (dx >= 0 ? -1.0 : 1.0);
            dx = cos(agentArray[i].angle);
            dy = sin(agentArray[i].angle);
        }

        drawTrace(&agentArray[i], traceMap);

        agentArray[i].x += dx;
        agentArray[i].y += dy;
    }
}

void iterateSimulation(struct Agent *agentArray, int agentNumber, struct TraceMap *traceMap)
{
    swapTraceMap(traceMap);
    fadeTrails(traceMap);
    iterateAgents(agentArray, agentNumber, traceMap);
}