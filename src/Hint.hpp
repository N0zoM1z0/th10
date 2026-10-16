#pragma once
#include <stddef.h>

// Descriptive reconstruction views, not recovered original class names.
// Only fields established by the exporter, loader and constructor are named.
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
    float position[3];
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
    unsigned char unknown00[0x18];
    HintLinkView loadedStageLists[8]; // Loader appends the same entry type here.
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
    offsetof(HintEntryView, textWidth) == 0x80 &&
    offsetof(HintEntryView, blue) == 0x84 ? 1 : -1];
typedef char HintOwnerLayoutCheck[
    sizeof(HintOwnerView) == 0x1A8 &&
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
