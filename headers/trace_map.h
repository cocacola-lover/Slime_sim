// TraceMap needs to hold the current frame and the frame before that
struct TraceMap
{
    float **oldMap;
    float **curMap;
    int width, height;
};
struct TraceMap mallocTraceMap(int width, int height);
void freeTraceMap(struct TraceMap traceMap);
void swapTraceMap(struct TraceMap *traceMap);

int isOutOfBounds(struct TraceMap traceMap, float x, float y);