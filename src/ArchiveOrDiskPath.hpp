#pragma once

#include <windows.h>

class PbgArchive;

// Target-call view: ECX is the address of a NUL-terminated path, rather than
// an owning object. The descriptive type does not establish an original class.
struct ArchiveOrDiskPathView
{
    PbgArchive *FindArchive();
    BYTE *Read(BYTE *optionalBuffer);
    DWORD GetSize();
};
