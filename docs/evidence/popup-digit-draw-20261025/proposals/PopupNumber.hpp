#pragma once
#include "AnmManager.hpp"
struct PopupNumberRecordView {
    unsigned char digits[12];
    AnmFloat3View position;
    unsigned int color;
    int previousAge;
    int age;
    float ageFloat;
    float *scale;
    unsigned int timerFlags;
    unsigned char unknown30[8];
    unsigned char active;
    unsigned char digitCount;
    unsigned char unknown3A[6];
};
struct PopupNumberView {
    unsigned char unknown000[8];
    void *updateChain;
    void *drawChain;
    AnmLoadedView *loaded;
    unsigned int insertionIndex;
    AnmVmView vm;
    PopupNumberRecordView records[723];
    int Draw();
};
typedef char PopupLayout[(sizeof(PopupNumberRecordView)==0x40 && offsetof(PopupNumberView,vm)==0x18 && offsetof(PopupNumberView,records)==0x3c4 && sizeof(PopupNumberView)==0xb884)?1:-1];
int __fastcall PopupNumberDrawCallback(void *context);
