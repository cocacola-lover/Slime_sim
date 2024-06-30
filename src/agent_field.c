#include <stdio.h>
#include <stdlib.h>
#define __USE_MISC
#include <math.h>

#include "../headers/agent_field.h"

char **mallocTraceMap(int i, int j)
{
    char **traceMap = malloc(i * sizeof(char *));

    for (int k = 0; k < i; k++)
    {
        traceMap[k] = malloc(j * sizeof(char));
        for (int l = 0; l < j; l++)
            traceMap[k][l] = 0;
    }

    return traceMap;
}

void freeTraceMap(char **traceMap, int i)
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

void iterateAgents(struct Agent *agentArray, int agentNumber, char **traceMap, int boundX, int boundY)
{
    for (int i = 0; i < agentNumber; i++)
    {

        float dx = cos(agentArray[i].angle);
        float dy = sin(agentArray[i].angle);

        if (agentArray[i].x + dx >= boundX)
        {
            dx = -dx;
            agentArray[i].angle = acos(dx);
        }
        else if (agentArray[i].y + dy >= boundY)
        {
            dy = -dy;
            agentArray[i].angle = asin(dy);
        }

        drawTrace(&agentArray[i], traceMap);

        agentArray[i].x += dx;
        agentArray[i].y += dy;
    }
}

void drawTrace(struct Agent *agentPointer, char **traceMap)
{
    traceMap[(int)roundf(agentPointer->x)][(int)roundf(agentPointer->y)] = 255;
}