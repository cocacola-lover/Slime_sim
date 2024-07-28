#include <stdio.h>
#include <stdlib.h>
#define __USE_MISC
#include <math.h>

#include "../headers/agent_field.h"

const float FADING_COEF = 0.005;
const float BLEND_COEF = 0.5;
const float ATTRACTION_COEF = 0.3;
// Cannot be more than M_PI / 2 and be less than 0
const float MAX_RANDOM_NUDGE = M_PI / 2;
const float FIELD_OF_VIEW = M_PI / 4;

// DO NOT CONFIGURE
const float X_BOUND_COS = 0.38268343237;
// const float Y_BOUND_SIN = 0.92387953251;

struct Agent
{
    float x, y;  // x, y describe position
    float angle; // angle describes the angle of movement
};

struct Agent *mallocAgents(struct TraceMap *traceMap, int numberOfAgents)
{
    struct Agent *agentArray = malloc(numberOfAgents * sizeof(struct Agent));

    for (int i = 0; i < numberOfAgents; i++)
    {
        float angle = (float)rand() / (float)(RAND_MAX / (M_PI * 2));
        // float angle = i % 2 == 0 ? 1.0 : 0;

        struct Agent newAgent = {traceMap->width / 2, traceMap->height / 2, angle};
        // struct Agent newAgent;
        // newAgent.angle = angle;
        // if (i % 2 == 0)
        // {
        //     newAgent.x = 10;
        //     newAgent.y = 8;
        // }
        // else
        // {
        //     newAgent.x = 6;
        //     newAgent.y = 8;
        // }
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
                    // sum += fmax(getOld(traceMap, k, l) - FADING_COEF, 0);
                    sum += traceMap->oldMap[k][l];
                    counter += 1;
                }
            }

            // setCurrent(traceMap, i, j, sum / counter);
            // traceMap->curMap[i][j] = fmax(traceMap->oldMap[i][j] - FADING_COEF, 0);
            const float blurredValue = sum / counter;

            const float diffusedValue = fmin(blurredValue, traceMap->oldMap[i][j]) + fabs(traceMap->oldMap[i][j] - blurredValue) * BLEND_COEF;

            const float diffusedAndEvaporatedValue = fmax(diffusedValue - FADING_COEF, 0);
            traceMap->curMap[i][j] = diffusedAndEvaporatedValue;
        }
    }
}

void drawTrace(struct Agent *agentPointer, struct TraceMap *traceMap)
{
    if (isOutOfBounds(*traceMap, agentPointer->x, agentPointer->y))
        return;

    traceMap->curMap[(int)roundf(agentPointer->x)][(int)roundf(agentPointer->y)] = 1;
}

float getFeramonFromAngle(struct Agent *agentPointer, struct TraceMap *traceMap, float angle)
{
    float feromonInDirection = 0;

    float directionCos = cos(angle);
    float directionSin = sin(angle);

    float x = 0;
    float y = 0;

    if (directionCos > X_BOUND_COS)
    {
        x = 1;
    }
    else if (directionCos < -X_BOUND_COS)
    {
        x = -1;
    }

    if (directionSin > X_BOUND_COS)
    {
        y = 1;
    }
    else if (directionSin < -X_BOUND_COS)
    {
        y = -1;
    }

    if (isOutOfBounds(*traceMap, agentPointer->x + x, agentPointer->y + y))
        return 0;

    return traceMap->oldMap[(int)roundf(agentPointer->x + x)][(int)roundf(agentPointer->y + y)];
}

void attractAgent(struct Agent *agentPointer, struct TraceMap *traceMap)
{
    float lookingAngle = agentPointer->angle;
    float toRightAngle = agentPointer->angle - FIELD_OF_VIEW;
    float toLeftAngle = agentPointer->angle + FIELD_OF_VIEW;

    float forwardFeramon = getFeramonFromAngle(agentPointer, traceMap, lookingAngle);
    float toRightFeramon = getFeramonFromAngle(agentPointer, traceMap, toRightAngle);
    float toLeftFeramon = getFeramonFromAngle(agentPointer, traceMap, toLeftAngle);

    if (forwardFeramon >= toRightFeramon)
    {
        if (forwardFeramon >= toLeftFeramon)
        {
            return;
        }
        else
        {
            agentPointer->angle += (M_PI / 4) * ATTRACTION_COEF;
        }
    }
    else if (toRightFeramon >= toLeftFeramon)
    {
        agentPointer->angle -= (M_PI / 4) * ATTRACTION_COEF;
    }
    else
    {
        agentPointer->angle += (M_PI / 4) * ATTRACTION_COEF;
    }
}

void iterateAgents(struct Agent *agentArray, int agentNumber, struct TraceMap *traceMap)
{
    for (int i = 0; i < agentNumber; i++)
    {
        drawTrace(&agentArray[i], traceMap);
        attractAgent(&agentArray[i], traceMap);

        float dx = cos(agentArray[i].angle);
        float dy = sin(agentArray[i].angle);

        if (agentArray[i].x + dx < 0)
        {
            float randomNudge = (float)rand() / (float)(RAND_MAX / MAX_RANDOM_NUDGE);
            // float randomNudge = 0;

            agentArray[i].angle += (M_PI / 2 + randomNudge) * (dy >= 0 ? -1.0 : 1.0);
        }
        // Lessen bound by 0.5 so rounding in drawTrace does not round out of bounds
        else if (agentArray[i].x + dx >= traceMap->width - 0.5)
        {
            float randomNudge = (float)rand() / (float)(RAND_MAX / MAX_RANDOM_NUDGE);
            // float randomNudge = 0;

            agentArray[i].angle += (M_PI / 2 + randomNudge) * (dy >= 0 ? 1.0 : -1.0);
        }
        else if (agentArray[i].y + dy < 0)
        {
            float randomNudge = (float)rand() / (float)(RAND_MAX / MAX_RANDOM_NUDGE);
            // float randomNudge = 0;

            agentArray[i].angle += (M_PI / 2 + randomNudge) * (dx >= 0 ? 1.0 : -1.0);
        }
        else if (agentArray[i].y + dy >= traceMap->height - 0.5)
        {
            float randomNudge = (float)rand() / (float)(RAND_MAX / MAX_RANDOM_NUDGE);
            // float randomNudge = 0;

            agentArray[i].angle += (M_PI / 2 + randomNudge) * (dx >= 0 ? -1.0 : 1.0);
        }

        agentArray[i].x += cos(agentArray[i].angle);
        agentArray[i].y += sin(agentArray[i].angle);
    }
}

void iterateSimulation(struct Agent *agentArray, int agentNumber, struct TraceMap *traceMap)
{
    swapTraceMap(traceMap);
    fadeTrails(traceMap);
    iterateAgents(agentArray, agentNumber, traceMap);

    // agentArray[0].angle
    // printf("Angle - %f\n", agentArray[0].angle);
}