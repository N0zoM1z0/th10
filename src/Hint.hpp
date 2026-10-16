#pragma once
#include <stddef.h>

// Descriptive reconstruction views, not recovered original class names.
// Only fields established by the exporter, allocator and constructor are named.
struct HintEntryView;
struct HintLinkView
{
    HintEntryView *entry;
    HintLinkView *next;
    HintLinkView *previous;
};
struct HintEntryView
{
    float position[3];
    HintLinkView link;
    unsigned char unknown18[4];
    char text[64];
    unsigned char unknown5C[4];
    int count;
    int alignment;
    int base;
    int time;
    int remaining;
    unsigned char unknown74[8];
    float scale;
    unsigned char unknown80[4];
    unsigned char blue, green, red, alpha;
};
struct HintOwnerView
{
    unsigned char unknown00[0x18];
    unsigned char firstStageListStorage[0x60]; // First list payload type is unaudited.
    HintLinkView exportStageLists[8];
    unsigned char unknownD8[0xD0];
};
#if defined(_M_IX86)
typedef char HintEntryLayoutCheck[
    sizeof(HintEntryView) == 0x88 &&
    offsetof(HintEntryView, link) == 0x0C &&
    offsetof(HintEntryView, text) == 0x1C &&
    offsetof(HintEntryView, count) == 0x60 &&
    offsetof(HintEntryView, remaining) == 0x70 &&
    offsetof(HintEntryView, scale) == 0x7C &&
    offsetof(HintEntryView, blue) == 0x84 ? 1 : -1];
typedef char HintOwnerLayoutCheck[
    sizeof(HintOwnerView) == 0x1A8 &&
    offsetof(HintOwnerView, exportStageLists) == 0x78 &&
    sizeof(HintLinkView) == 12 ? 1 : -1];
#endif

// Target41A200: two stack arguments, RET8. No forced private register ABI.
int __stdcall ExportAutomaticHints(HintOwnerView *owner, const char *path);
