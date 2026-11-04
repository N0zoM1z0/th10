#include "Startup.hpp"

// Separate TH10 physical 0x0041FD70..0x0041FD81 nullable destruction.
// Retail takes the existing StartupWorkerView* privately in ESI; the natural
// C++ entry does not claim that original register/producer TU or authorship.
void StartupNullableDelete(StartupWorkerView *worker)
{
    if (worker != NULL)
        delete worker;
}
