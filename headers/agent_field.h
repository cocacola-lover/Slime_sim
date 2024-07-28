#pragma once
#include "trace_map.h"

struct Agent;
void iterateAgents(struct Agent *agentArray, int agentNumber, struct TraceMap *traceMap);
void iterateSimulation(struct Agent *agentArray, int agentNumber, struct TraceMap *traceMap);

struct Agent *mallocAgents(struct TraceMap *traceMap, int numberOfAgents);
void freeAgents(struct Agent *agentArray);