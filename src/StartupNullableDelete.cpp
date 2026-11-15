#include "Startup.hpp"

// Separate TH10 physical 0x0041FD70..0x0041FD81 nullable destruction.
// Retail takes the StartupWorkerView* privately in ESI.
void StartupNullableDelete(StartupWorkerView *worker)
{
    if (worker != NULL)
        delete worker;
}
