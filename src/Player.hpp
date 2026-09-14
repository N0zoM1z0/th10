#ifndef TH10_RECONSTRUCTION_PLAYER_HPP
#define TH10_RECONSTRUCTION_PLAYER_HPP

#include <stddef.h>

struct PlayerPositionPair
{
    int x;
    int y;
};
typedef char PlayerPositionPairSizeIs08[
    (sizeof(PlayerPositionPair) == 0x08) ? 1 : -1];

// Maintained view of the 0x98-byte option records observed in the TH10 player
// owner. The original field identifiers remain unknown.
struct PlayerOptionRuntime;
// Maintained spelling of the observed ECX-bound callback ABI. The original
// source declaration/convention remains unknown.
typedef int (__fastcall *PlayerOptionCallback)(PlayerOptionRuntime *option);

struct PlayerOptionRuntime
{
    int state;
    unsigned char unknown004[0x30];
    PlayerPositionPair replayPair0;
    PlayerPositionPair replayPair1;
    PlayerPositionPair replayPair2;
    PlayerPositionPair replayPair3;
    unsigned char unknown054[0x14];
    unsigned int primaryVmId;
    unsigned int secondaryVmId;
    unsigned char unknown070[0x14];
    int previousMode;
    int optionIndex;
    int resetFlag;
    PlayerOptionCallback updateCallback;
    PlayerOptionCallback drawCallback;
};
typedef char PlayerOptionRuntimeSizeIs98[
    (sizeof(PlayerOptionRuntime) == 0x98) ? 1 : -1];
typedef char PlayerOptionRuntimeReplayPair0At34[
    (offsetof(PlayerOptionRuntime, replayPair0) == 0x34) ? 1 : -1];
typedef char PlayerOptionRuntimePrimaryVmIdAt68[
    (offsetof(PlayerOptionRuntime, primaryVmId) == 0x68) ? 1 : -1];
typedef char PlayerOptionRuntimeSecondaryVmIdAt6C[
    (offsetof(PlayerOptionRuntime, secondaryVmId) == 0x6c) ? 1 : -1];
typedef char PlayerOptionRuntimePreviousModeAt84[
    (offsetof(PlayerOptionRuntime, previousMode) == 0x84) ? 1 : -1];
typedef char PlayerOptionRuntimeIndexAt88[
    (offsetof(PlayerOptionRuntime, optionIndex) == 0x88) ? 1 : -1];
typedef char PlayerOptionRuntimeResetAt8C[
    (offsetof(PlayerOptionRuntime, resetFlag) == 0x8c) ? 1 : -1];
typedef char PlayerOptionRuntimeCallbackAt90[
    (offsetof(PlayerOptionRuntime, updateCallback) == 0x90) ? 1 : -1];
typedef char PlayerOptionRuntimeDrawCallbackAt94[
    (offsetof(PlayerOptionRuntime, drawCallback) == 0x94) ? 1 : -1];

// Maintained partial draw-VM view beginning at Player +0x14. Only the
// target-observed draw position and flags offsets are exposed; the original
// VM type/field identifiers and physical object extent remain unknown.
struct PlayerDrawVmView
{
    unsigned char unknown000[0x340];
    float positionX;
    float positionY;
    float positionZ;
    unsigned char unknown34C[0x10];
    unsigned int flags;
};
typedef char PlayerDrawVmViewPositionAt340[
    (offsetof(PlayerDrawVmView, positionX) == 0x340) ? 1 : -1];
typedef char PlayerDrawVmViewFlagsAt35C[
    (offsetof(PlayerDrawVmView, flags) == 0x35c) ? 1 : -1];

// Maintained partial TH10 player layout. Only target-observed fields needed by
// the reviewed replay and option-rebuild seams are exposed. This is not an
// original-symbol or translation-unit ownership claim.
struct Player
{
    unsigned char unknown000[0x10];
    void *resource;
    PlayerDrawVmView drawVm;
    unsigned char unknown374[0x4c];
    float drawPositionX;
    float drawPositionY;
    float drawPositionZ;
    int positionX;
    int positionY;
    unsigned char unknown3D4[0x84];
    int runtimeState;
    unsigned char *optionData;
    unsigned char unknown460[0x2e40];
    PlayerOptionRuntime options[4];
    int optionCount;
    unsigned char unknown3504[0xe68];
    PlayerPositionPair replayPositionHistory[33];
    int optionMode;
};
typedef char PlayerResourceAt10[
    (offsetof(Player, resource) == 0x10) ? 1 : -1];
typedef char PlayerDrawVmViewAt14[
    (offsetof(Player, drawVm) == 0x14) ? 1 : -1];
typedef char PlayerDrawPositionAt3C0[
    (offsetof(Player, drawPositionX) == 0x3c0) ? 1 : -1];
typedef char PlayerPositionAt3CC[
    (offsetof(Player, positionX) == 0x3cc) ? 1 : -1];
typedef char PlayerRuntimeStateAt458[
    (offsetof(Player, runtimeState) == 0x458) ? 1 : -1];
typedef char PlayerOptionDataAt45C[
    (offsetof(Player, optionData) == 0x45c) ? 1 : -1];
typedef char PlayerOptionsAt32A0[
    (offsetof(Player, options) == 0x32a0) ? 1 : -1];
typedef char PlayerOptionCountAt3500[
    (offsetof(Player, optionCount) == 0x3500) ? 1 : -1];
typedef char PlayerReplayHistoryAt436C[
    (offsetof(Player, replayPositionHistory) == 0x436c) ? 1 : -1];
typedef char PlayerOptionModeAt4474[
    (offsetof(Player, optionMode) == 0x4474) ? 1 : -1];

extern Player *g_Player;

// Descriptive maintained name for the reviewed 0x00426F70-0x0042792D owner.
// The target machine boundary is one stack Player* argument with callee pop 4;
// the original source identifier, source convention, TU, and compiler owner are
// not established.
void RebuildPlayerOptions(Player *player);

#endif
