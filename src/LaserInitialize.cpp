#include "Main.hpp"
#include "EnemyLaserLine.hpp"
#include "EnemySoundQueue.hpp"

extern int g_BulletSpriteScriptBase[];
extern unsigned char g_MainSoundOwner[0x52d0];

static inline float DeceleratedLaserSpeed(float speed, float elapsed, int duration)
{
    return speed - (speed * elapsed) / duration;
}

// Target41C8C0: virtual slot4, one stack request, RET4. This initializes an
// existing line; C++ construction/vtable installation remain separate owners.
int EnemyLaserLineView::Initialize(const EnemyLaserType0RequestView *initial)
{
    request = *initial;
    unknown00C = 2;
    AnmVmView *vm = &primaryVm;
    vm->InitializeForLoadedScript(
        reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->effectResource,
        request.color + g_BulletSpriteScriptBase[request.type]);
    primaryVm.pendingInterrupt = 2;
    AnmRenderManagerView::ExecuteScript(vm);
    if ((request.flags & 1u) != 0)
        vm->flags35C = (vm->flags35C & ~0x20u) | 0x10u;
    primaryVm.flags35C = (primaryVm.flags35C & 0xfc63ffffu) | 0x00600000u;

    vm = &secondaryVm;
    vm->InitializeForLoadedScript(
        reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->effectResource,
        request.color + 0x103);
    secondaryVm.pendingInterrupt = 2;
    AnmRenderManagerView::ExecuteScript(vm);
    vm->flags35C = (vm->flags35C & ~0x20u) | 0x10u;
    secondaryVm.flags35C = (secondaryVm.flags35C & 0xfc7fffffu) | 0x00400000u;
    holdTimer.SetCurrent(30);
    position = request.position;
    soundId = 24;
    length = request.initialLength;
    width = request.width;
    growSpeed = request.speed;
    angle = request.angle;
    traveled = length > 0.0f ? 0.01f : 0.0f;
    velocity.FromAngleMagnitude(angle, growSpeed);
    return 0;
}

// Target41D170, virtual motion slot30h / flag40h. Record3 resets its timer
// after each turn; clearing the flag does not skip the final polar/timer work.
void EnemyLaserLineView::Motion0040()
{
    EnemyLaserPatternRecordView &motion = patterns[3];
    int duration = motion.values[5].integer;
    float magnitude;
    if (motion.timer.current >= duration)
    {
        if (soundId >= 0)
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                QueueSoundSample(soundId, 0);
        ++motion.values[7].integer;
        if (motion.values[7].integer >= motion.values[6].integer)
            flags &= ~0x40u;
        angle += motion.values[1].scalar;
        growSpeed = motion.values[0].scalar;
        magnitude = growSpeed;
        motion.timer.SetCurrent(0);
    }
    else
        magnitude = DeceleratedLaserSpeed(growSpeed, motion.timer.subframe, duration);
    velocity.FromAngleMagnitude(angle, magnitude);
    motion.timer.Tick();
}
