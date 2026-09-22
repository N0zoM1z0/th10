#include <stddef.h>

struct EnemyLaserVectorView
{
    float x;
    float y;
    float z;

    void FromAngleMagnitude(float angle, float magnitude);
};

typedef char EnemyLaserVectorViewSizeIs0C[
    (sizeof(EnemyLaserVectorView) == 0x0c) ? 1 : -1];

void EnemyLaserVectorView::FromAngleMagnitude(float angle, float magnitude)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul magnitude
        fstp [eax]
        fmul magnitude
        fstp [eax + 4]
    }
#else
    (void)angle;
    (void)magnitude;
    x = 0.0f;
    y = 0.0f;
#endif
}


struct EnemyLaserRequestScratch
{
    unsigned char bytes[0x1f8];
};

typedef char EnemyLaserRequestScratchSizeIs1F8[
    (sizeof(EnemyLaserRequestScratch) == 0x1f8) ? 1 : -1];

struct EnemyLaserSoundQueueView
{
    void QueueSoundSample(int soundId, int sample);
};

extern unsigned char g_MainSoundOwner[];
extern float __stdcall EnemyWrapAngle(float angle);
extern float g_EnemyPlayfieldMinX;
extern float g_EnemyPlayfieldMaxX;
extern float g_EnemyPlayfieldMinY;
extern float g_EnemyPlayfieldMaxY;
extern float g_EnemyPi;
extern void *g_EnemyBulletManager;
extern unsigned int EnemyFireLaser(
    void *manager, EnemyLaserRequestScratch *request, int type);

struct EnemyLaserBoundaryStateView
{
    unsigned char unknown000[0x24];
    float positionX;
    float positionY;
    float positionZ;
    unsigned char unknown030[0x3c - 0x30];
    float angle;
    float magnitude;
    unsigned char unknown044[0x13c - 0x44];
    int requestOwner;
    unsigned char unknown140[0x404 - 0x140];
    unsigned int flags;
    unsigned char unknown408[0x40c - 0x408];
    int soundId;
    unsigned char unknown410[0x424 - 0x410];
    EnemyLaserRequestScratch request;

    void UpdateBoundary();
};

typedef char EnemyLaserBoundaryPositionXAt024[
    (offsetof(EnemyLaserBoundaryStateView, positionX) == 0x24) ? 1 : -1];
typedef char EnemyLaserBoundaryAngleAt03C[
    (offsetof(EnemyLaserBoundaryStateView, angle) == 0x3c) ? 1 : -1];
typedef char EnemyLaserBoundaryRequestOwnerAt13C[
    (offsetof(EnemyLaserBoundaryStateView, requestOwner) == 0x13c) ? 1 : -1];
typedef char EnemyLaserBoundaryFlagsAt404[
    (offsetof(EnemyLaserBoundaryStateView, flags) == 0x404) ? 1 : -1];
typedef char EnemyLaserBoundarySoundIdAt40C[
    (offsetof(EnemyLaserBoundaryStateView, soundId) == 0x40c) ? 1 : -1];
typedef char EnemyLaserBoundaryRequestAt424[
    (offsetof(EnemyLaserBoundaryStateView, request) == 0x424) ? 1 : -1];

void EnemyLaserBoundaryStateView::UpdateBoundary()
{
    EnemyLaserVectorView projected;
    projected.FromAngleMagnitude(angle, magnitude);
    projected.x += positionX;
    projected.y += positionY;
    projected.z = 0.0f;

    if (!(projected.x <= g_EnemyPlayfieldMinX ||
          projected.x >= g_EnemyPlayfieldMaxX ||
          projected.y <= g_EnemyPlayfieldMinY ||
          projected.y >= g_EnemyPlayfieldMaxY)) {
        return;
    }

    int fired = 0;
    if (soundId >= 0) {
        reinterpret_cast<EnemyLaserSoundQueueView *>(g_MainSoundOwner)
            ->QueueSoundSample(soundId, 0);
    }

    float *requestWords = reinterpret_cast<float *>(&request);
    if (projected.x < g_EnemyPlayfieldMinX ||
        projected.x >= g_EnemyPlayfieldMaxX) {
        requestWords[0] = projected.x;
        requestWords[1] = projected.y;
        requestWords[2] = projected.z;
        requestWords[3] = EnemyWrapAngle(-angle - g_EnemyPi);
        reinterpret_cast<int *>(&request)[0x20 / 4] = requestOwner;
        EnemyFireLaser(g_EnemyBulletManager, &request, 0);
        fired = 1;
    }

    if ((flags & 0x08000000u) != 0 ||
        (projected.y >= g_EnemyPlayfieldMinY &&
         (projected.y < g_EnemyPlayfieldMaxY ||
          (flags & 0x400u) == 0))) {
        if (!fired)
            return;
    }
    else {
        requestWords[0] = projected.x;
        requestWords[1] = projected.y;
        requestWords[2] = projected.z;
        requestWords[3] = -angle;
        reinterpret_cast<int *>(&request)[0x20 / 4] = requestOwner;
        EnemyFireLaser(g_EnemyBulletManager, &request, 0);
    }

    flags &= 0xf7fff3ffu;
}
