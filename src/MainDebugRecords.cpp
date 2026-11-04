#include "Main.hpp"
#include <stdio.h>
#include <string.h>

// Native owner 0x00420DD0..0x00420E9C, 205 bytes. The real entry keeps
// the Main supervisor in private EAX and accepts three stack arguments
// (RET 12); this ordinary descriptive member does not assert that ABI.
int MainSupervisorView::MatchDebugRecord(
    const char *name, int first, int second)
{
    const char *cursor = static_cast<const char *>(buffer764);
    if (cursor == NULL)
        return 0;
    unsigned int remaining = bufferBytes760;
    if (strncmp(name, "debug", 5) == 0)
        return 0;
    // The target explicitly compares its fixed "0100a" build-version text
    // with "debug" for six bytes, independently of the incoming name.
    if (strncmp("0100a", "debug", 6) == 0)
        return 0;
    while (remaining != 0)
    {
        if (strncmp(name, cursor, 5) == 0)
        {
            int parsedFirst;
            int parsedSecond;
            sscanf(cursor + 6, "%d %d", &parsedFirst, &parsedSecond);
            if (parsedFirst == first && parsedSecond == second)
                return 0;
        }
        const char *next = strchr(cursor, '\n') + 1;
        remaining -= static_cast<unsigned int>(next - cursor);
        cursor = next;
    }
    return -1;
}
