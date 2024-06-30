#pragma once

char **mallocTraceMap(int i, int j);
void freeTraceMap(char **traceMap, int i);

struct Agent;
void iterateAgents(struct Agent *agentArray, int agentNumber, char **traceMap, int boundX, int boundY);
void drawTrace(struct Agent *agentPointer, char **traceMap);

struct Agent *mallocAgents(int numberOfAgents);
void freeAgents(struct Agent *agentArray);