#include <string.h>

// Independent target 0x004356D0..0x004356E3 (20 bytes). The original receives
// its path pointer in private ESI. This ordinary C++ declaration is a
// semantic/compilation view, not evidence of the original source's ABI or TU.
// The corresponding archive/disk selection logic already has an inline form
// in FileSystem.cpp; this source represents the separate retained target body.
const char *ArchiveEntrySuffix(const char *path)
{
    const char *slash = strchr(path, '/');
    if (slash != NULL)
        return slash + 1;
    return path;
}
