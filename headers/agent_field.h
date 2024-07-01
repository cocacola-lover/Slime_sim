#pragma once

// TraceMap needs to hold the current frame and the frame before that
struct TraceMap
{
    float **oldMap;
    float **curMap;
    int width, height;
};
struct TraceMap mallocTraceMap(int width, int height);
void freeTraceMap(struct TraceMap traceMap);

struct Agent;
void iterateAgents(struct Agent *agentArray, int agentNumber, struct TraceMap *traceMap);
void iterateSimulation(struct Agent *agentArray, int agentNumber, struct TraceMap *traceMap);

struct Agent *mallocAgents(int numberOfAgents);
void freeAgents(struct Agent *agentArray);