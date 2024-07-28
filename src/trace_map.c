#include <stdlib.h>
#include "../headers/trace_map.h"

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

int isOutOfBounds(struct TraceMap traceMap, float x, float y)
{
    if (x < 0)
    {
        return 1;
    }
    else if (y < 0)
    {
        return 1;
    }
    else if (traceMap.height - 1 < y)
    {
        return 1;
    }
    else if (traceMap.width - 1 < x)
    {
        return 1;
    }

    return 0;
}