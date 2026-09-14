#ifndef TH10_RECONSTRUCTION_PLAYER_HPP
#define TH10_RECONSTRUCTION_PLAYER_HPP

#include <stddef.h>
#include <string.h>

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
    unsigned char unknown070[0x10];
    unsigned int flags80;
    int previousMode;
    int optionIndex;
    int resetFlag;
    PlayerOptionCallback updateCallback;
    PlayerOptionCallback drawCallback;

    PlayerOptionRuntime() { flags80 &= ~1u; }
};
typedef char PlayerOptionRuntimeSizeIs98[
    (sizeof(PlayerOptionRuntime) == 0x98) ? 1 : -1];
typedef char PlayerOptionRuntimeReplayPair0At34[
    (offsetof(PlayerOptionRuntime, replayPair0) == 0x34) ? 1 : -1];
typedef char PlayerOptionRuntimePrimaryVmIdAt68[
    (offsetof(PlayerOptionRuntime, primaryVmId) == 0x68) ? 1 : -1];
typedef char PlayerOptionRuntimeSecondaryVmIdAt6C[
    (offsetof(PlayerOptionRuntime, secondaryVmId) == 0x6c) ? 1 : -1];
typedef char PlayerOptionRuntimeFlagsAt80[
    (offsetof(PlayerOptionRuntime, flags80) == 0x80) ? 1 : -1];
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

struct PlayerFloat3
{
    float x;
    float y;
    float z;
};
typedef char PlayerFloat3SizeIs0C[
    (sizeof(PlayerFloat3) == 0x0c) ? 1 : -1];

// Maintained timer-shaped view recovered from repeated TH10 Player update
// storage. The original timer type/member names remain unknown.
struct PlayerTimerView
{
    int previous;
    int current;
    float subframe;
    float *scale;
    unsigned int flags;

    PlayerTimerView() { flags &= ~1u; }
};
typedef char PlayerTimerViewSizeIs14[
    (sizeof(PlayerTimerView) == 0x14) ? 1 : -1];

// Maintained view of one 0x6C Player effect row used by the central update
// owner. Field names are descriptive only and expose just target-observed use.
struct PlayerEffectMotionView
{
    unsigned char unknown000[0x08];
    int resetField;
    float angle;
    float angularStep;
    float timerStep;
    float timerStepDelta;
    unsigned int motionFlags;
    PlayerTimerView timer;
    unsigned char unknown034[0x10];
    unsigned int activeFlags;
};
typedef char PlayerEffectMotionViewSizeIs48[
    (sizeof(PlayerEffectMotionView) == 0x48) ? 1 : -1];
typedef char PlayerEffectMotionTimerAt20[
    (offsetof(PlayerEffectMotionView, timer) == 0x20) ? 1 : -1];

struct PlayerEffectRowView
{
    float valueX;
    float deltaX;
    float valueY;
    float deltaY;
    unsigned char unknown010[0x08];
    unsigned char finalizeState[0x0c];
    PlayerEffectMotionView motion;
};
typedef char PlayerEffectRowViewSizeIs6C[
    (sizeof(PlayerEffectRowView) == 0x6c) ? 1 : -1];
typedef char PlayerEffectRowFinalizeAt18[
    (offsetof(PlayerEffectRowView, finalizeState) == 0x18) ? 1 : -1];
typedef char PlayerEffectRowMotionAt24[
    (offsetof(PlayerEffectRowView, motion) == 0x24) ? 1 : -1];

// Neutral four-byte subobject view for draw-VM members whose only reviewed
// constructor effect is clearing flag bit zero. Their original types and
// semantics remain unknown.
struct PlayerConstructorFlagView
{
    unsigned int flags;
    PlayerConstructorFlagView() { flags &= ~1u; }
};
typedef char PlayerConstructorFlagViewSizeIs04[
    (sizeof(PlayerConstructorFlagView) == 0x04) ? 1 : -1];

// Maintained 0x3AC-byte draw-VM storage view beginning at Player +0x14.
// TH10 constructor order proves the neutral flag subobjects below run before
// the containing VM body clears its 0x3AC-byte storage. Only independently
// observed fields are given semantic names; original VM types/names are unknown.
struct PlayerDrawVmView
{
    unsigned char unknown000[0x6c];
    PlayerConstructorFlagView constructorFlags06C;
    unsigned char unknown070[0x40];
    PlayerConstructorFlagView constructorFlags0B0;
    unsigned char unknown0B4[0x48];
    PlayerConstructorFlagView constructorFlags0FC;
    unsigned char unknown100[0x28];
    PlayerConstructorFlagView constructorFlags128;
    unsigned char unknown12C[0x48];
    PlayerConstructorFlagView constructorFlags174;
    unsigned char unknown178[0x38];
    PlayerConstructorFlagView constructorFlags1B0;
    unsigned char unknown1B4[0x48];
    PlayerConstructorFlagView constructorFlags1FC;
    unsigned char unknown200[0x28];
    PlayerConstructorFlagView constructorFlags228;
    unsigned char unknown22C[0xd4];
    unsigned int color;
    unsigned char unknown304[0x3c];
    float positionX;
    float positionY;
    float positionZ;
    unsigned char unknown34C[0x0c];
    void *ownedData358;
    unsigned int flags;
    unsigned char unknown360[0x18];
    PlayerConstructorFlagView constructorFlags378;
    unsigned char unknown37C[0x08];
    unsigned short unknownWord384;
    unsigned char unknown386[0x26];

    PlayerDrawVmView()
    {
        memset(this, 0, sizeof(*this));
        unknownWord384 = 0xffff;
    }
};
typedef char PlayerDrawVmViewSizeIs3AC[
    (sizeof(PlayerDrawVmView) == 0x3ac) ? 1 : -1];
typedef char PlayerDrawVmViewCtorFlagsAt06C[
    (offsetof(PlayerDrawVmView, constructorFlags06C) == 0x06c) ? 1 : -1];
typedef char PlayerDrawVmViewCtorFlagsAt378[
    (offsetof(PlayerDrawVmView, constructorFlags378) == 0x378) ? 1 : -1];
typedef char PlayerDrawVmViewColorAt300[
    (offsetof(PlayerDrawVmView, color) == 0x300) ? 1 : -1];
typedef char PlayerDrawVmViewPositionAt340[
    (offsetof(PlayerDrawVmView, positionX) == 0x340) ? 1 : -1];
typedef char PlayerDrawVmViewOwnedDataAt358[
    (offsetof(PlayerDrawVmView, ownedData358) == 0x358) ? 1 : -1];
typedef char PlayerDrawVmViewFlagsAt35C[
    (offsetof(PlayerDrawVmView, flags) == 0x35c) ? 1 : -1];
typedef char PlayerDrawVmViewWordAt384[
    (offsetof(PlayerDrawVmView, unknownWord384) == 0x384) ? 1 : -1];


// Maintained view of the fixed header of the variable-sized Player .sht data.
// The first three float names remain offset-based because their original
// semantics are not established. Speed fields are named from TH10-local use.
struct PlayerOptionDataView
{
    unsigned short unknown000;
    unsigned short entryCount;
    float value04;
    float value08;
    float value0C;
    float axisSpeedMode0;
    float axisSpeedMode1;
    float diagonalSpeedMode0;
    float diagonalSpeedMode1;
};
typedef char PlayerOptionDataViewHeaderSizeIs20[
    (sizeof(PlayerOptionDataView) == 0x20) ? 1 : -1];
typedef char PlayerOptionDataViewCountAt02[
    (offsetof(PlayerOptionDataView, entryCount) == 0x02) ? 1 : -1];
typedef char PlayerOptionDataViewSpeedsAt10[
    (offsetof(PlayerOptionDataView, axisSpeedMode0) == 0x10) ? 1 : -1];

struct PlayerShotRuntimeView
{
    PlayerTimerView timer;
    unsigned char unknown014[0x48];
};
typedef char PlayerShotRuntimeViewSizeIs5C[
    (sizeof(PlayerShotRuntimeView) == 0x5c) ? 1 : -1];

struct PlayerCallbackNodeView;

// Maintained partial TH10 player layout. Only target-observed fields needed by
// the reviewed Player replay, option, movement, update, and draw seams are
// exposed. This is not an original-symbol or translation-unit ownership claim.
struct Player
{
    unsigned char unknown000[0x08];
    PlayerCallbackNodeView *updateCallbackNode;
    PlayerCallbackNodeView *drawCallbackNode;
    void *resource;
    PlayerDrawVmView drawVm;
    PlayerFloat3 drawPosition;
    int positionX;
    int positionY;
    int axisSpeedMode0;
    int axisSpeedMode1;
    int diagonalSpeedMode0;
    int diagonalSpeedMode1;
    unsigned char unknown3E4[0x0c];
    int movementDeltaX;
    int movementDeltaY;
    unsigned char unknown3F8[0x0c];
    PlayerFloat3 derivedMin0;
    PlayerFloat3 derivedMax0;
    PlayerFloat3 extent0;
    PlayerFloat3 extent1;
    PlayerFloat3 extent2;
    PlayerFloat3 savedDrawPosition;
    int previousHorizontalSpeed;
    int previousVerticalSpeed;
    int movementDirection;
    int runtimeState;
    PlayerOptionDataView *optionData;
    PlayerTimerView updateTimer0;
    PlayerTimerView updateTimer1;
    PlayerTimerView updateTimer2;
    PlayerShotRuntimeView shots[128];
    unsigned int modeVmId;
    PlayerOptionRuntime options[4];
    int optionCount;
    int updateScratch;
    unsigned char updateScratchByte;
    unsigned char unknown3509[0x03];
    PlayerEffectRowView effectRows[33];
    unsigned char unknown42F8[0x10];
    int optionTransitionFrames;
    PlayerTimerView highlightTimer;
    unsigned char unknown4320[0x04];
    PlayerFloat3 derivedVectors[6];
    PlayerPositionPair replayPositionHistory[33];
    int optionMode;

    Player();
    ~Player();
};
typedef char PlayerUpdateCallbackNodeAt08[
    (offsetof(Player, updateCallbackNode) == 0x08) ? 1 : -1];
typedef char PlayerDrawCallbackNodeAt0C[
    (offsetof(Player, drawCallbackNode) == 0x0c) ? 1 : -1];
typedef char PlayerResourceAt10[
    (offsetof(Player, resource) == 0x10) ? 1 : -1];
typedef char PlayerDrawVmViewAt14[
    (offsetof(Player, drawVm) == 0x14) ? 1 : -1];
typedef char PlayerDrawPositionAt3C0[
    (offsetof(Player, drawPosition) == 0x3c0) ? 1 : -1];
typedef char PlayerPositionAt3CC[
    (offsetof(Player, positionX) == 0x3cc) ? 1 : -1];
typedef char PlayerMovementSpeedModesAt3D4[
    (offsetof(Player, axisSpeedMode0) == 0x3d4) ? 1 : -1];
typedef char PlayerDerivedMinAt404[
    (offsetof(Player, derivedMin0) == 0x404) ? 1 : -1];
typedef char PlayerExtent0At41C[
    (offsetof(Player, extent0) == 0x41c) ? 1 : -1];
typedef char PlayerSavedDrawPositionAt440[
    (offsetof(Player, savedDrawPosition) == 0x440) ? 1 : -1];
typedef char PlayerPreviousMovementAt44C[
    (offsetof(Player, previousHorizontalSpeed) == 0x44c) ? 1 : -1];
typedef char PlayerMovementDirectionAt454[
    (offsetof(Player, movementDirection) == 0x454) ? 1 : -1];
typedef char PlayerRuntimeStateAt458[
    (offsetof(Player, runtimeState) == 0x458) ? 1 : -1];
typedef char PlayerOptionDataAt45C[
    (offsetof(Player, optionData) == 0x45c) ? 1 : -1];
typedef char PlayerTimer0At460[
    (offsetof(Player, updateTimer0) == 0x460) ? 1 : -1];
typedef char PlayerTimer1At474[
    (offsetof(Player, updateTimer1) == 0x474) ? 1 : -1];
typedef char PlayerTimer2At488[
    (offsetof(Player, updateTimer2) == 0x488) ? 1 : -1];
typedef char PlayerShotsAt49C[
    (offsetof(Player, shots) == 0x49c) ? 1 : -1];
typedef char PlayerModeVmIdAt329C[
    (offsetof(Player, modeVmId) == 0x329c) ? 1 : -1];
typedef char PlayerOptionsAt32A0[
    (offsetof(Player, options) == 0x32a0) ? 1 : -1];
typedef char PlayerOptionCountAt3500[
    (offsetof(Player, optionCount) == 0x3500) ? 1 : -1];
typedef char PlayerEffectRowsAt350C[
    (offsetof(Player, effectRows) == 0x350c) ? 1 : -1];
typedef char PlayerEffectRowsEndAt42F8[
    (offsetof(Player, unknown42F8) == 0x42f8) ? 1 : -1];
typedef char PlayerOptionTransitionFramesAt4308[
    (offsetof(Player, optionTransitionFrames) == 0x4308) ? 1 : -1];
typedef char PlayerHighlightTimerAt430C[
    (offsetof(Player, highlightTimer) == 0x430c) ? 1 : -1];
typedef char PlayerDerivedVectorsAt4324[
    (offsetof(Player, derivedVectors) == 0x4324) ? 1 : -1];
typedef char PlayerReplayHistoryAt436C[
    (offsetof(Player, replayPositionHistory) == 0x436c) ? 1 : -1];
typedef char PlayerOptionModeAt4474[
    (offsetof(Player, optionMode) == 0x4474) ? 1 : -1];
typedef char PlayerSizeIs4478[
    (sizeof(Player) == 0x4478) ? 1 : -1];

extern Player *g_Player;

// Descriptive maintained name for the reviewed 0x004247F0-0x00424D8A owner.
// The target machine boundary receives Player through live-in EBX and returns
// an int in EAX; the original source identifier, declaration and TU remain
// unknown.
int PlayerInitialize(Player *player);

// Descriptive maintained lifecycle names for reviewed TH10 Player owners. The
// target machine ABIs are tracked separately from these natural C++ spellings.
void PlayerResetRuntimeState(Player *player);
Player *PlayerCreate();

// Descriptive maintained name for the reviewed 0x00426F70-0x0042792D owner.
// The target machine boundary is one stack Player* argument with callee pop 4;
// the original source identifier, source convention, TU, and compiler owner are
// not established.
void RebuildPlayerOptions(Player *player);

#endif
