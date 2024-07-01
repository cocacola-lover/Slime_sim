#pragma once

float **mallocTraceMap(int i, int j);
void freeTraceMap(float **traceMap, int i);

struct Agent;
void iterateAgents(struct Agent *agentArray, int agentNumber, float **traceMap, int boundX, int boundY);
void iterateSimulation(struct Agent *agentArray, int agentNumber, float **traceMap, int boundX, int boundY);

struct Agent *mallocAgents(int numberOfAgents);
void freeAgents(struct Agent *agentArray);