#include "Player.hpp"
#include "AnmManager.hpp"
#include "EnemySoundQueue.hpp"
extern unsigned char g_MainSoundOwner[0x52d0];

struct EnemyRuntimeVectorView {
    float x, y, z;
    void FromAngleMagnitude(float angle, float magnitude);
};
extern float __stdcall EnemyWrapAngle(float angle);
extern short g_PlayerPower;
typedef void (__fastcall *PlayerShotSpawnCallback)(Player *, PlayerShotRuntimeView *, int);

int PlayerCreateShot(Player *player, const PlayerShotDescriptorView *descriptor, int frame)
{
    if (descriptor->type == 3 && player->shotSourceActive[descriptor->sourceIndex - 1])
        return 0;
    int index = 0;
    for (; index < 128; ++index)
        if (player->shots[index].state == 0) break;
    if (index == 128) return 0;
    PlayerShotRuntimeView *shot = &player->shots[index];
    shot->state = 1;
    shot->descriptor = descriptor;
    shot->timer.SetCurrent(0);
    if (descriptor->sourceIndex == 0)
        shot->motion.position = player->drawPosition;
    else {
        PlayerFloat3 position;
        position.x = static_cast<float>(player->options[descriptor->sourceIndex - 1].replayPair1.x) * 0.01f;
        position.y = static_cast<float>(player->options[descriptor->sourceIndex - 1].replayPair1.y) * 0.01f;
        position.z = 0.0f;
        shot->motion.position = position;
    }
    if (descriptor->type == 3)
        player->shotSourceActive[descriptor->sourceIndex - 1] = 1;
    shot->motion.speedOrAngleStep = descriptor->speed;
    shot->motion.angle = EnemyWrapAngle(descriptor->angle);
    if (!(shot->motion.flags & 1)) {
        reinterpret_cast<EnemyRuntimeVectorView *>(&shot->motion.velocity)->FromAngleMagnitude(shot->motion.angle, shot->motion.speedOrAngleStep);
        shot->motion.velocity.z = 0.0f;
    } else {
        shot->motion.polarMagnitude += shot->motion.polarMagnitudeDelta;
        shot->motion.angle = EnemyWrapAngle(shot->motion.speedOrAngleStep + shot->motion.angle);
    }
    shot->motion.position.x += descriptor->spawnOffsetX - shot->motion.velocity.x;
    shot->motion.position.y += descriptor->spawnOffsetY - shot->motion.velocity.y;
    shot->primaryVmId = static_cast<AnmLoadedView *>(player->resource)->CreateVmVariant0(descriptor->animationScript + 5, 15).value;
    AnmVmIdView id; id.value = shot->primaryVmId;
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(id);
    if (!vm) shot->primaryVmId = 0;
    if (vm->flags35C & 0x08000000) {
        vm->rotation.z = descriptor->angle;
        vm->flags35C |= 4;
    }
    if (descriptor->type == 3)
        shot->secondaryVmId = static_cast<AnmLoadedView *>(player->resource)->CreateVmVariant0(16, 15).value;
    else shot->secondaryVmId = 0;
    if (descriptor->spawnCallback)
        reinterpret_cast<PlayerShotSpawnCallback>(descriptor->spawnCallback)(player, shot, frame);
    if (descriptor->soundId >= 0)
        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundCue(descriptor->soundId, shot->motion.position.x);
    return 0;
}
int PlayerFireShots(Player *player, int frame)
{
    int index = static_cast<unsigned short>(g_PlayerPower) / 20;
    if (index > 4) index = 4;
    if (player->optionMode) index += 5;
    const PlayerShotDescriptorView *descriptor = *reinterpret_cast<const PlayerShotDescriptorView **>(
        reinterpret_cast<unsigned char *>(player->optionData) + 0x110 + index * 8);
    for (; descriptor->fireInterval >= 0; ++descriptor)
        if (frame % descriptor->fireInterval == descriptor->fireFrame)
            PlayerCreateShot(player, descriptor, frame);
    return 0;
}
