#include "AnmManager.hpp"

#include <stdlib.h>
#include <string.h>

// These two target helpers are also inlined by AnmVmView::Initialize. Their
// chained assignments preserve VC7.1's target-observed right-to-left stores.
void AnmMatrixView::SetIdentity()
{
    values[1] = values[2] = values[3] =
        values[4] = values[6] = values[7] =
        values[8] = values[9] = values[11] =
        values[12] = values[13] = values[14] = 0.0f;
    values[0] = values[5] = values[10] = values[15] = 1.0f;
}

void AnmVmTimerView::Initialize()
{
    current = 0;
    previous = -999999;
    subframe = 0.0f;
}

// Target 0x00402050-0x004020A7 first runs the nine embedded timer
// constructors, then clears the complete 0x3AC-byte VM and marks its active
// sprite index invalid.
AnmVmView::AnmVmView()
{
    memset(this, 0, sizeof(AnmVmView));
    activeSpriteIndex = -1;
}

// Target 0x00401FF0-0x00402011 owns the optional generated-vertex buffer.
AnmVmView::~AnmVmView()
{
    if (generatedVertices != NULL)
        free(generatedVertices);
    generatedVertices = NULL;
}

// Target 0x00401DE0-0x00401EF0 resets the reusable VM while retaining the
// externally owned pointers at +0x20 and +0x340..+0x348.
void AnmVmView::Initialize()
{
    void *saved340 = preserved340;
    void *saved344 = preserved344;
    void *saved348 = preserved348;
    void *savedOwner = persistentOwner020;

    memset(this, 0, sizeof(AnmVmView));

    preserved340 = saved340;
    preserved344 = saved344;
    persistentOwner020 = savedOwner;
    preserved348 = saved348;

    value2FC = -1;
    scaleX = 1.0f;
    scaleY = 1.0f;
    matrix23C.SetIdentity();
    flags35C = 7;
    timer05C.Initialize();

    state0B4 = 0;
    state100 = 0;
    state12C = 0;
    state178 = 0;
    state1B4 = 0;
    state200 = 0;
    state22C = 0;

    listSelf004 = this;
    unknown008 = NULL;
    unknown00C = NULL;
    layerSelf010 = this;
    unknown014 = NULL;
    unknown018 = NULL;
}
