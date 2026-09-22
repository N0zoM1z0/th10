#include "AnmManager.hpp"

#include <stddef.h>
#include <string.h>

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

struct EnemySoundQueueView
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
        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)
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


// The type-0 request copied by target collision owner 0x0041D880 is 0x77
// dwords (0x1DC bytes).  Its short type/color pair at +0x24/+0x26 and the
// pattern payload at +0x2C are also independently observed by the dispatcher
// and request-apply path.
struct EnemyLaserType0RequestView
{
    EnemyLaserVectorView position;
    float angle;
    float maximumLength;
    float initialLength;
    float terminalDistance;
    float width;
    float speed;
    short type;
    short color;
    unsigned int flags;
    unsigned int patternWords[0x6c];
};

typedef char EnemyLaserType0RequestViewSizeIs1DC[
    (sizeof(EnemyLaserType0RequestView) == 0x1dc) ? 1 : -1];
typedef char EnemyLaserType0RequestAngleAt00C[
    (offsetof(EnemyLaserType0RequestView, angle) == 0x0c) ? 1 : -1];
typedef char EnemyLaserType0RequestMaximumAt010[
    (offsetof(EnemyLaserType0RequestView, maximumLength) == 0x10) ? 1 : -1];
typedef char EnemyLaserType0RequestInitialAt014[
    (offsetof(EnemyLaserType0RequestView, initialLength) == 0x14) ? 1 : -1];
typedef char EnemyLaserType0RequestTypeAt024[
    (offsetof(EnemyLaserType0RequestView, type) == 0x24) ? 1 : -1];
typedef char EnemyLaserType0RequestColorAt026[
    (offsetof(EnemyLaserType0RequestView, color) == 0x26) ? 1 : -1];
typedef char EnemyLaserType0RequestPatternAt02C[
    (offsetof(EnemyLaserType0RequestView, patternWords) == 0x2c) ? 1 : -1];

extern int EnemyLaserSampleBoundsTest(
    EnemyLaserVectorView *position, float width, float height);

struct EnemyLaserHitEffectManagerView
{
    void SpawnCancelEffect(
        EnemyLaserVectorView *position,
        int kind, int color, float angle, float scale);
};

extern EnemyLaserHitEffectManagerView *g_EnemyLaserHitEffectManager;

struct EnemyLaserManagerResourceView
{
    unsigned char unknown000[0x458];
    AnmLoadedView *effectResource;
};

typedef char EnemyLaserManagerResourceAt458[
    (offsetof(EnemyLaserManagerResourceView, effectResource) == 0x458) ? 1 : -1];

// Target 0x0041D880 is the type-0 box-collision owner.  The same high-level
// split algorithm appears in the neighboring laser family, but every field,
// helper and request extent below is retained only where TH10 itself observes
// it.  Original identifiers remain unknown.
struct EnemyLaserType0CollisionView
{
    void *vtable;
    void *previous;
    void *next;
    unsigned char unknown00C[0x24 - 0x0c];
    EnemyLaserVectorView position;
    unsigned char unknown030[0x3c - 0x30];
    float angle;
    float length;
    float value44;
    float value48;
    float tailOffset;
    unsigned char deletionCounter;
    unsigned char unknown051[0x424 - 0x51];
    EnemyLaserType0RequestView request;

    int CheckCollisionBox(
        EnemyLaserVectorView *center,
        EnemyLaserVectorView *size,
        int capture);
    int CheckCollisionCircle(
        EnemyLaserVectorView *center,
        float radius,
        int capture);
};

typedef char EnemyLaserType0CollisionPositionAt024[
    (offsetof(EnemyLaserType0CollisionView, position) == 0x24) ? 1 : -1];
typedef char EnemyLaserType0CollisionAngleAt03C[
    (offsetof(EnemyLaserType0CollisionView, angle) == 0x3c) ? 1 : -1];
typedef char EnemyLaserType0CollisionLengthAt040[
    (offsetof(EnemyLaserType0CollisionView, length) == 0x40) ? 1 : -1];
typedef char EnemyLaserType0CollisionTailAt04C[
    (offsetof(EnemyLaserType0CollisionView, tailOffset) == 0x4c) ? 1 : -1];
typedef char EnemyLaserType0CollisionDeleteAt050[
    (offsetof(EnemyLaserType0CollisionView, deletionCounter) == 0x50) ? 1 : -1];
typedef char EnemyLaserType0CollisionRequestAt424[
    (offsetof(EnemyLaserType0CollisionView, request) == 0x424) ? 1 : -1];

int EnemyLaserType0CollisionView::CheckCollisionBox(
    EnemyLaserVectorView *center,
    EnemyLaserVectorView *size,
    int capture)
{
    EnemyLaserVectorView originalPosition = position;
    unsigned char hits[256];
    memset(hits, 0, sizeof(hits));

    int sampleCount = 0;
    int hitCount = 0;
    float distance = 6.0f;

    EnemyLaserVectorView halfSize;
    halfSize.x = size->x * 0.5f;
    halfSize.y = size->y * 0.5f;
    halfSize.z = size->z * 0.5f;

    EnemyLaserVectorView minimum;
    minimum.x = center->x - halfSize.x;
    minimum.y = center->y - halfSize.y;
    minimum.z = center->z - halfSize.z;

    EnemyLaserVectorView maximum;
    maximum.x = center->x + halfSize.x;
    maximum.y = center->y + halfSize.y;
    maximum.z = center->z + halfSize.z;

    EnemyLaserVectorView step;
    step.z = 0.0f;
    step.FromAngleMagnitude(angle, 6.0f);

    EnemyLaserVectorView sample;
    sample.x = position.x + step.x;
    sample.y = position.y + step.y;
    sample.z = position.z + step.z;

    step.x += step.x;
    step.y += step.y;
    step.z += step.z;

    while (distance + 6.0f < length)
    {
        if (minimum.x <= sample.x && sample.x <= maximum.x &&
            minimum.y <= sample.y && sample.y <= maximum.y)
        {
            hits[sampleCount] = 1;
            ++hitCount;

            if (capture != 0 &&
                EnemyLaserSampleBoundsTest(&sample, 32.0f, 32.0f) == 0)
            {
                g_EnemyLaserHitEffectManager->SpawnCancelEffect(
                    &sample, 8, -1, -1.5707964f, 0.6f);
            }

            EnemyLaserManagerResourceView *manager =
                reinterpret_cast<EnemyLaserManagerResourceView *>(
                    g_EnemyBulletManager);
            manager->effectResource->CreateVmAtWorldVariant0(
                static_cast<int>(request.color) * 2 + 0x11,
                reinterpret_cast<const AnmFloat3View *>(&sample));
        }

        ++sampleCount;
        sample.x += step.x;
        sample.y += step.y;
        sample.z += step.z;
        distance += 12.0f;
    }

    if (hitCount == 0)
        return 0;

    if (sampleCount <= hitCount)
    {
        deletionCounter = 1;
        return hitCount;
    }

    int sampleIndex = 0;
    while (sampleIndex < sampleCount && hits[sampleIndex] != 0)
        ++sampleIndex;

    if (sampleIndex != 0)
    {
        const float prefixDistance =
            static_cast<float>(sampleIndex) * 12.0f;
        const float prefixScale = static_cast<float>(sampleIndex);
        position.x += step.x * prefixScale;
        position.y += step.y * prefixScale;
        position.z += step.z * prefixScale;
        length -= prefixDistance;
        if (length <= 18.0f)
        {
            deletionCounter = 1;
            return hitCount;
        }
        request.maximumLength = length;
        tailOffset = prefixDistance;
    }

    int gapLength = 0;
    while (sampleIndex < sampleCount && hits[sampleIndex] == 0)
    {
        ++sampleIndex;
        ++gapLength;
    }

    if (sampleIndex >= sampleCount)
        return hitCount;

    const float firstGapLength =
        static_cast<float>(gapLength) * 12.0f;
    request.maximumLength -= length - firstGapLength;
    length = firstGapLength;
    if (firstGapLength < 18.0f)
        deletionCounter = 1;

    while (sampleIndex < sampleCount)
    {
        while (sampleIndex < sampleCount && hits[sampleIndex] != 0)
            ++sampleIndex;
        if (sampleIndex >= sampleCount)
            return hitCount;

        gapLength = 0;
        const int gapStart = sampleIndex;
        while (sampleIndex < sampleCount && hits[sampleIndex] == 0)
        {
            ++sampleIndex;
            ++gapLength;
        }

        EnemyLaserType0RequestView args = request;
        const float segmentLength =
            static_cast<float>(gapLength) * 12.0f;
        args.maximumLength = segmentLength;
        args.initialLength = segmentLength;

        if (18.0f < segmentLength)
        {
            const float start = static_cast<float>(gapStart);
            args.position.x = originalPosition.x + step.x * start;
            args.position.y = originalPosition.y + step.y * start;
            args.position.z = originalPosition.z + step.z * start;
            EnemyFireLaser(
                g_EnemyBulletManager,
                reinterpret_cast<EnemyLaserRequestScratch *>(&args),
                0);
        }
    }

    return hitCount;
}


int EnemyLaserType0CollisionView::CheckCollisionCircle(
    EnemyLaserVectorView *center,
    float radius,
    int capture)
{
    EnemyLaserVectorView originalPosition = position;
    unsigned char hits[256];
    memset(hits, 0, sizeof(hits));

    int sampleCount = 0;
    int hitCount = 0;
    float distance = 6.0f;
    const float radiusSquared = radius * radius;

    EnemyLaserVectorView step;
    step.z = 0.0f;
    step.FromAngleMagnitude(angle, 6.0f);

    EnemyLaserVectorView sample;
    sample.x = position.x + step.x;
    sample.y = position.y + step.y;
    sample.z = position.z + step.z;

    step.x += step.x;
    step.y += step.y;
    step.z += step.z;

    while (distance + 6.0f < length)
    {
        const float dx = center->x - sample.x;
        const float dy = center->y - sample.y;
        if (dx * dx + dy * dy <= radiusSquared)
        {
            hits[sampleCount] = 1;
            ++hitCount;

            if (capture != 0 &&
                g_EnemyPlayfieldMinX <= sample.x + 32.0f &&
                sample.x - 32.0f < g_EnemyPlayfieldMaxX &&
                g_EnemyPlayfieldMinY <= sample.y + 32.0f &&
                sample.y - 32.0f < g_EnemyPlayfieldMaxY)
            {
                g_EnemyLaserHitEffectManager->SpawnCancelEffect(
                    &sample, 8, -1, -1.5707964f, 0.6f);
            }

            EnemyLaserManagerResourceView *manager =
                reinterpret_cast<EnemyLaserManagerResourceView *>(
                    g_EnemyBulletManager);
            manager->effectResource->CreateVmAtWorldVariant0(
                static_cast<int>(request.color) * 2 + 0x11,
                reinterpret_cast<const AnmFloat3View *>(&sample));
        }

        ++sampleCount;
        sample.x += step.x;
        sample.y += step.y;
        sample.z += step.z;
        distance += 12.0f;
    }

    if (hitCount == 0)
        return 0;

    if (sampleCount <= hitCount)
    {
        deletionCounter = 1;
        return hitCount;
    }

    int sampleIndex = 0;
    while (sampleIndex < sampleCount && hits[sampleIndex] != 0)
        ++sampleIndex;

    if (sampleIndex != 0)
    {
        const float prefixScale = static_cast<float>(sampleIndex);
        const float prefixDistance = prefixScale * 12.0f;
        position.x += step.x * prefixScale;
        position.y += step.y * prefixScale;
        position.z += step.z * prefixScale;
        length -= prefixDistance;
        if (length <= 18.0f)
        {
            deletionCounter = 1;
            return hitCount;
        }
        request.maximumLength = length;
        tailOffset = prefixDistance;
    }

    int gapLength = 0;
    while (sampleIndex < sampleCount && hits[sampleIndex] == 0)
    {
        ++sampleIndex;
        ++gapLength;
    }

    if (sampleIndex >= sampleCount)
        return hitCount;

    const float firstGapLength =
        static_cast<float>(gapLength) * 12.0f;
    request.maximumLength -= length - firstGapLength;
    length = firstGapLength;
    if (firstGapLength < 18.0f)
        deletionCounter = 1;

    while (sampleIndex < sampleCount)
    {
        while (sampleIndex < sampleCount && hits[sampleIndex] != 0)
            ++sampleIndex;
        if (sampleIndex >= sampleCount)
            return hitCount;

        gapLength = 0;
        const int gapStart = sampleIndex;
        while (sampleIndex < sampleCount && hits[sampleIndex] == 0)
        {
            ++sampleIndex;
            ++gapLength;
        }

        EnemyLaserType0RequestView args = request;
        const float segmentLength =
            static_cast<float>(gapLength) * 12.0f;
        args.maximumLength = segmentLength;
        args.initialLength = segmentLength;

        if (18.0f < segmentLength)
        {
            const float start = static_cast<float>(gapStart);
            args.position.x = originalPosition.x + step.x * start;
            args.position.y = originalPosition.y + step.y * start;
            args.position.z = originalPosition.z + step.z * start;
            EnemyFireLaser(
                g_EnemyBulletManager,
                reinterpret_cast<EnemyLaserRequestScratch *>(&args),
                0);
        }
    }

    return hitCount;
}
