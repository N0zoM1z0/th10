#include "ScriptViewer.hpp"

// Original standalone body 0x0040A430..0x0040A441 uses a private ESI
// ScriptViewerView* and releases it through the real destructor/free route.
// This is a natural source-ABI view; the physical original generated-vs-
// authored status and the register transport from any caller are unknown.
void ScriptViewerNullableDelete(ScriptViewerView *viewer)
{
    if (viewer != NULL)
        delete viewer;
}
