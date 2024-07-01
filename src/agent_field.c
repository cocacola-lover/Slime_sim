#include <stdio.h>
#include <stdlib.h>
#define __USE_MISC
#include <math.h>

#include "../headers/agent_field.h"

const float FADING_COEF = 0.01;

float **mallocTraceMap(int i, int j)
{
    float **traceMap = malloc(i * sizeof(float *));

    for (int k = 0; k < i; k++)
    {
        traceMap[k] = malloc(j * sizeof(float));
        for (int l = 0; l < j; l++)
            traceMap[k][l] = 0;
    }

    return traceMap;
}

void freeTraceMap(float **traceMap, int i)
{
    for (int k = 0; k < i; k++)
        free(traceMap[k]);
    free(traceMap);
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

void fadeTrails(float **traceMap, int boundX, int boundY)
{
    for (int i = 0; i < boundX; i++)
    {
        for (int j = 0; j < boundY; j++)
            if (traceMap[i][j] > 0)
                traceMap[i][j] = fmax(traceMap[i][j] - FADING_COEF, 0);
    }
}

void drawTrace(struct Agent *agentPointer, float **traceMap)
{
    traceMap[(int)roundf(agentPointer->x)][(int)roundf(agentPointer->y)] = 1;
}

void iterateAgents(struct Agent *agentArray, int agentNumber, float **traceMap, int boundX, int boundY)
{
    for (int i = 0; i < agentNumber; i++)
    {
        float dx = cos(agentArray[i].angle);
        float dy = sin(agentArray[i].angle);

        // Lessen bound by 0.5 so rounding in drawTrace does not round out of bounds
        if (agentArray[i].x + dx < 0 || agentArray[i].x + dx >= boundX - 0.5)
        {
            dx = -dx;
            agentArray[i].angle = acos(dx);
        }
        else if (agentArray[i].y + dy < 0 || agentArray[i].y + dy >= boundY - 0.5)
        {
            dy = -dy;
            agentArray[i].angle = asin(dy);
        }

        drawTrace(&agentArray[i], traceMap);

        agentArray[i].x += dx;
        agentArray[i].y += dy;
    }
}

void iterateSimulation(struct Agent *agentArray, int agentNumber, float **traceMap, int boundX, int boundY)
{
    fadeTrails(traceMap, boundX, boundY);
    iterateAgents(agentArray, agentNumber, traceMap, boundX, boundY);
}