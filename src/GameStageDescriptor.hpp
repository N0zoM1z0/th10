#ifndef TH10_GAME_STAGE_DESCRIPTOR_HPP
#define TH10_GAME_STAGE_DESCRIPTOR_HPP
#include <stddef.h>
struct GameStageUpdateView
{
    int resourceIndex;
    const char *menuPath;
    unsigned int unknown008;
    const char *enemyPath;
    const char *musicPaths[2];
    unsigned char unknown018[0x0c];
    int musicIndex; // +0x24, unchanged existing field
    int value28; // +0x28, neutral until independently identified
    unsigned int unknown02C;
};
extern GameStageUpdateView *g_GameStageUpdate;
typedef char GameStageDescriptorFields[(offsetof(GameStageUpdateView, musicIndex) == 0x24 && offsetof(GameStageUpdateView, value28) == 0x28 && sizeof(GameStageUpdateView) == 0x30) ? 1 : -1];
#endif
