#pragma once

// Consumed shared prefix ends at+1C. Ending allocates EC with this member at D0;
// the browser preserves its separate unknown bytes at+2C. Original full type unknown.
struct ScriptViewerWorkerView
{
    void *thread;
    unsigned int threadId;
    int stopRequested;
    int running;
    unsigned int unknown014;
    void *entry018;
    ScriptViewerWorkerView();
    virtual ~ScriptViewerWorkerView();
    // Target Ending destruction has no unwind edge around this wait/close role.
    __declspec(noinline) void Stop() throw();
    __declspec(noinline) void Start(void *context, int (*entry)());
};

typedef char SharedWorkerPrefixIs1C[(sizeof(ScriptViewerWorkerView) == 0x1c) ? 1 : -1];
