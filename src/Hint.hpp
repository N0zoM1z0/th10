#pragma once
#include <stddef.h>
#include "AnmManager.hpp"

// Descriptive reconstruction views, not recovered original class names.
// Fields are established by the exporter, loader, constructor and frame owners.
struct HintEntryView;
struct HintLinkView
{
    HintEntryView *entry;
    HintLinkView *next;
    HintLinkView *previous;
};
struct HintEntryView
{
    HintEntryView(); // Constructor-shaped target 0x00418D00; origin remains indeterminate.
    AnmFloat3View position;
    HintLinkView link;
    unsigned char unknown18[4];
    // Storage through +0x5F includes constructor-zeroed bytes after the 65-byte copy.
    // This preserves safe C++ string access without claiming an original array size.
    char text[68];
    int count;
    int alignment;
    int base;
    int time;
    int remaining;
    unsigned char unknown74[8];
    float scale;
    float textWidth; // Loader finalization: byte length * scale * 7.5, then float store.
    unsigned char blue, green, red, alpha;
};
struct HintOwnerView
{
    int ConsumeEntries(HintLinkView *first, int freeEntries);
    int Update();
    unsigned char unknown00[0x10];
    int counter10;
    int counter14;
    HintLinkView loadedStageLists[8]; // Loader appends the same entry type here.
    HintLinkView exportStageLists[8];
    int currentBase;
    AnmVmIdView vmIds[10];
    float cachedPositions[10][3];
    float cachedWidths[10];
    int nextVmSlot;
};
#if defined(_M_IX86)
typedef char HintEntryLayoutCheck[
    sizeof(HintEntryView) == 0x88 &&
    offsetof(HintEntryView, position) == 0x00 &&
    offsetof(HintEntryView, link) == 0x0C &&
    offsetof(HintEntryView, text) == 0x1C &&
    offsetof(HintEntryView, count) == 0x60 &&
    offsetof(HintEntryView, alignment) == 0x64 &&
    offsetof(HintEntryView, base) == 0x68 &&
    offsetof(HintEntryView, time) == 0x6C &&
    offsetof(HintEntryView, remaining) == 0x70 &&
    offsetof(HintEntryView, scale) == 0x7C &&
    offsetof(HintEntryView, textWidth) == 0x80 &&
    offsetof(HintEntryView, blue) == 0x84 &&
    offsetof(HintEntryView, alpha) == 0x87 ? 1 : -1];
typedef char HintOwnerLayoutCheck[
    sizeof(HintOwnerView) == 0x1A8 &&
    offsetof(HintOwnerView, counter10) == 0x10 &&
    offsetof(HintOwnerView, counter14) == 0x14 &&
    offsetof(HintOwnerView, currentBase) == 0xD8 &&
    offsetof(HintOwnerView, vmIds) == 0xDC &&
    offsetof(HintOwnerView, cachedPositions) == 0x104 &&
    offsetof(HintOwnerView, cachedWidths) == 0x17C &&
    offsetof(HintOwnerView, nextVmSlot) == 0x1A4 &&
    offsetof(HintOwnerView, loadedStageLists) == 0x18 &&
    offsetof(HintOwnerView, exportStageLists) == 0x78 &&
    sizeof(HintLinkView) == 12 ? 1 : -1];
#endif

// Target41A200: two stack arguments, RET8. No forced private register ABI.
int __stdcall ExportAutomaticHints(HintOwnerView *owner, const char *path);

// Target 0x00419960: owner/path/mode on stack, RET 12; descriptive name.
int __stdcall LoadHintFile(HintOwnerView *owner, const char *path, int mode);
// Target 0x00419040: both list groups are freed; sentinel pointers are not cleared.
void ClearHintLists(HintOwnerView *owner);
