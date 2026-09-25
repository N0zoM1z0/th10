#include "BulletManager.hpp"

struct EnemySoundQueueView
{
    unsigned char unknown000[0x408];
    int cueValues[134];
    int activeSoundIds[12];
    int sampleCounts[12];
    int samples[12][128];

    void QueueSoundCue(int soundId, float positionX);
};

struct BulletPositionView
{
    float x;
    float y;
    float z;

    int IsOutsidePlayfield(float width, float height);
};

extern void *g_EnemyBulletManager;
extern unsigned char g_MainSoundOwner[];
extern void BulletUpdateRelativeDirectionChange(BulletRuntimeView *bullet);
extern void BulletUpdateAbsoluteDirectionChange(BulletRuntimeView *bullet);
extern void BulletUpdateAimedDirectionChange(BulletRuntimeView *bullet);
extern void BulletUpdateBoundaryBounce(BulletRuntimeView *bullet);
extern void BulletUpdateHorizontalWrap(BulletRuntimeView *bullet);
extern void BulletUpdateVerticalWrap(BulletRuntimeView *bullet);
extern void BulletUpdateState8(BulletRuntimeView *bullet);
extern int BulletCheckPlayerCollision(
    PlayerFloat3 *position, Player *player, const float *collisionSize);

// Target 0x00406240. TH10 passes the bullet as one callee-clean stack argument.
__declspec(noinline) int __stdcall BulletUpdateRuntime(BulletRuntimeView *bullet)
{
    if ((bullet->flags & 8u) != 0) {
        bullet->Deactivate();
        return -1;
    }

    if (bullet->state == 2) {
        bullet->position.x += bullet->velocity.x * g_AnmGameSpeed * 0.5f;
        bullet->position.y += bullet->velocity.y * g_AnmGameSpeed * 0.5f;
        bullet->position.z += bullet->velocity.z * g_AnmGameSpeed * 0.5f;
        if (bullet->vm.intVar0 != 0) {
            bullet->state = 1;
            goto updateActive;
        }
    }
    else if (bullet->state == 3) {
        bullet->position.x += bullet->velocity.x * g_AnmGameSpeed * 0.5f;
        bullet->position.y += bullet->velocity.y * g_AnmGameSpeed * 0.5f;
        bullet->position.z += bullet->velocity.z * g_AnmGameSpeed * 0.5f;
    }
    else if (bullet->state == 1) {
updateActive:
        bullet->AdvanceTransformProgram();
        if (bullet->activeTransformFlags != 0) {
            if ((bullet->activeTransformFlags & 0x00000001u) != 0)
                bullet->UpdateDeceleration();
            if ((bullet->activeTransformFlags & 0x00000010u) != 0)
                bullet->UpdateVectorAcceleration();
            if ((bullet->activeTransformFlags & 0x00000020u) != 0)
                bullet->UpdatePolarAcceleration();
            if ((bullet->activeTransformFlags & 0x00000040u) != 0)
                BulletUpdateRelativeDirectionChange(bullet);
            if ((bullet->activeTransformFlags & 0x00000100u) != 0)
                BulletUpdateAbsoluteDirectionChange(bullet);
            if ((bullet->activeTransformFlags & 0x00000080u) != 0)
                BulletUpdateAimedDirectionChange(bullet);
            if ((bullet->activeTransformFlags & 0x08000c00u) != 0)
                BulletUpdateBoundaryBounce(bullet);
            if ((bullet->activeTransformFlags & 0x04000000u) != 0)
                BulletUpdateState8(bullet);
            if ((bullet->activeTransformFlags & 0x00008000u) != 0) {
                if (bullet->exStates[5].timer.current <= 0)
                    bullet->activeTransformFlags ^= 0x00008000u;
                else
                    bullet->exStates[5].timer.Add(-1.0f);
            }
        }

        bullet->position.x += bullet->velocity.x * g_AnmGameSpeed;
        bullet->position.y += bullet->velocity.y * g_AnmGameSpeed;
        bullet->position.z += bullet->velocity.z * g_AnmGameSpeed;

        if ((bullet->flags & 2u) != 0) {
            int collision = BulletCheckPlayerCollision(
                &bullet->position, g_Player, &bullet->collisionWidth);
            if (collision == 1) {
                bullet->state = 3;
                bullet->vm.pendingInterrupt = 1;
                if (bullet->effectScript >= 0) {
                    reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager)->
                        bulletAnm->CreateVmAtWorldVariant0(
                            bullet->effectScript,
                            reinterpret_cast<const AnmFloat3View *>(
                                &bullet->position));
                }
            }
            else if (collision == 2 && (bullet->flags & 4u) == 0) {
                bullet->flags |= 4u;
                reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager)->
                    bulletAnm->CreateVmAtWorldVariant0(
                        0x1b2,
                        reinterpret_cast<const AnmFloat3View *>(
                            &bullet->position));
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                    QueueSoundCue(0x1c, bullet->position.x);
            }
        }
    }

    if (bullet->vm.loadedSprite != 0) {
        if ((bullet->activeTransformFlags & 0x00100000u) != 0)
            BulletUpdateHorizontalWrap(bullet);
        if ((bullet->activeTransformFlags & 0x00200000u) != 0)
            BulletUpdateVerticalWrap(bullet);
        if (bullet->offscreenCullDelayFrames <= 0 &&
            reinterpret_cast<BulletPositionView *>(&bullet->position)->
                IsOutsidePlayfield(
                    bullet->vm.loadedSprite->width,
                    bullet->vm.loadedSprite->height) != 0) {
            bullet->Deactivate();
            return -1;
        }
    }

    if (bullet->cancelBehavior != 0)
        --bullet->cancelBehavior;
    if (bullet->offscreenCullDelayFrames > 0)
        --bullet->offscreenCullDelayFrames;

    if (AnmRenderManagerView::ExecuteScript(&bullet->vm) != 0) {
        bullet->Deactivate();
        return -1;
    }
    return 0;
}
