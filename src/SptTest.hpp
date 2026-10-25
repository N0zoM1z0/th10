#ifndef TH10_RECONSTRUCTION_SPT_TEST_HPP
#define TH10_RECONSTRUCTION_SPT_TEST_HPP
#include "FrontEnd.hpp"
struct EclVmScriptDatabase;
// Provisional view of the separate "Spt Test" display owner. Only consumed
// offsets are established; original class, allocation size and TU are unknown.
struct SptTestView
{
    unsigned char unknown000[0x30];
    int state;
    char **scriptNames;
    int scriptCount;
    FrontEndCursorView menu;
    FrontEndCursorView files;
    FrontEndCursorView entries;
    AnmFloat3View position;
    unsigned char unknown2D0[8];
    EclVmScriptDatabase *scriptDatabase;
    int Draw();
};
typedef char SptTestDrawLayout[(offsetof(SptTestView, state) == 0x30 &&
    offsetof(SptTestView, menu) == 0x3c &&
    offsetof(SptTestView, files) == 0x114 &&
    offsetof(SptTestView, entries) == 0x1ec &&
    offsetof(SptTestView, position) == 0x2c4 &&
    offsetof(SptTestView, scriptDatabase) == 0x2d8) ? 1 : -1];
int __fastcall SptTestDrawCallback(void *context);
#endif
