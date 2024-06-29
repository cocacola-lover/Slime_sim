#include <stdio.h>
#include <stdlib.h>

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
    int x, y;   // x, y describe position
    int vx, vy; // vx, vy describe movement
};

struct Agent *mallocAgent()
{
    struct Agent a = {1, 1, 1, 1};

    struct Agent *agentArray = malloc(1 * sizeof(struct Agent));
    agentArray[0] = a;

    return agentArray;
}

void iterateAgents(struct Agent *agentArray, int agentNumber, char **traceMap, int boundX, int boundY)
{
    for (int i = 0; i < agentNumber; i++)
    {
        if (agentArray[i].x + agentArray[i].vx >= boundX)
            agentArray[i].vx *= -1;
        if (agentArray[i].y + agentArray[i].vy >= boundY)
            agentArray[i].vy *= -1;

        drawTrace(&agentArray[i], traceMap);

        agentArray[i].x += agentArray[i].vx;
        agentArray[i].y += agentArray[i].vy;
    }
}

void drawTrace(struct Agent *agentPointer, char **traceMap)
{
    traceMap[agentPointer->x + agentPointer->vx][agentPointer->y + agentPointer->vy] = 255;
}