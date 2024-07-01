#include <stdio.h>
#include <stdlib.h>
#define __USE_MISC
#include <math.h>

#include "../headers/agent_field.h"

const float FADING_COEF = 0.01;

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

        struct Agent newAgent = {50, 50, angle};
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
            traceMap->curMap[i][j] = fmax(traceMap->oldMap[i][j] - FADING_COEF, 0);
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

        // Lessen bound by 0.5 so rounding in drawTrace does not round out of bounds
        if (agentArray[i].x + dx < 0 || agentArray[i].x + dx >= traceMap->width - 0.5)
        {
            dx = -dx;
            agentArray[i].angle = acos(dx);
        }
        else if (agentArray[i].y + dy < 0 || agentArray[i].y + dy >= traceMap->height - 0.5)
        {
            dy = -dy;
            agentArray[i].angle = asin(dy);
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