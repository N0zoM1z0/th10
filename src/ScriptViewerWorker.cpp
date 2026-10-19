#include "Main.hpp"
#include "ScriptViewer.hpp"
#include <process.h>

// Adapted in part from levzzz5154/th10-decomp (MIT), copyright (c) 2026 levzzz,
// commit c6791d82dc9f02e55eca45a5314f900a85481efd. See docs/licenses/th10-decomp-MIT.txt.
// This is a descriptive shared worker prefix, not an original class/TU claim.

void ScriptViewerWorkerView::Stop()
{
    if (thread)
    {
        stopRequested = 1;
        running = 0;
        while (WaitForSingleObject(thread, 200) == WAIT_TIMEOUT)
        {
            stopRequested = 1;
            running = 0;
            Sleep(1);
        }
        CloseHandle(thread);
        thread = NULL;
        entry018 = NULL;
    }
}

void ScriptViewerWorkerView::Start(void *context, int (*entry)())
{
    Stop();
    entry018 = reinterpret_cast<void *>(entry);
    running = 1;
    stopRequested = 0;
    // The browser passes its no-argument, plain-RET load entry unchanged.
    // Preserve that address and the CRT API's actual callback type separately.
    thread = reinterpret_cast<void *>(_beginthreadex(
        NULL, 0, reinterpret_cast<unsigned (__stdcall *)(void *)>(entry),
        context, 0, &threadId));
}
