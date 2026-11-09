#include "AnmManager.hpp"
#include "AnmLoadedStartScript.hpp"
#include "EnemyLaser.hpp"
#include "EnemyLaserLine.hpp"
#include "EnemyLaserType1.hpp"
#include "ItemManager.hpp"
#include "EnemySoundQueue.hpp"
#include <new>

#include <stddef.h>
#include <string.h>

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

// This local view method matches the bounds helper called by the type-0 and
// type-1 laser collision owners. The original class and production TU remain
// unknown; the third target caller is still unmapped.
extern "C" {
    extern const float g_EnemyLaserPlayfieldMinimumX;
    extern const float g_EnemyLaserPlayfieldMaximumX;
    extern const float g_EnemyLaserPlayfieldMinimumY;
    extern const float g_EnemyLaserPlayfieldMaximumY;
}

int EnemyLaserVectorView::IsOutsidePlayfield(
    float extentX, float extentY)
{
    if (!(x + extentX <= g_EnemyLaserPlayfieldMinimumX) &&
        !(x - extentX >= g_EnemyLaserPlayfieldMaximumX) &&
        !(y + extentY <= g_EnemyLaserPlayfieldMinimumY) &&
        !(y - extentY >= g_EnemyLaserPlayfieldMaximumY))
    {
        return 0;
    }
    return 1;
}



extern unsigned char g_MainSoundOwner[];
extern float __stdcall EnemyWrapAngle(float angle);
extern float g_EnemyPlayfieldMinX;
extern float g_EnemyPlayfieldMaxX;
extern float g_EnemyPlayfieldMinY;
extern float g_EnemyPlayfieldMaxY;
extern float g_EnemyPi;

// The type-0 request copied by target collision owner 0x0041D880 is 0x77
// dwords (0x1DC bytes).  Its short type/color pair at +0x24/+0x26 and the
// pattern payload at +0x2C are also independently observed by the dispatcher
// and request-apply path.
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
                sample.IsOutsidePlayfield(32.0f, 32.0f) == 0)
            {
                g_ItemManager->Spawn(
                    reinterpret_cast<const AnmFloat3View *>(&sample),
                    8, 0xffffffffu, -1.5707964f, 0.6f);
            }

            EnemyLaserManagerResourceView *manager =
                reinterpret_cast<EnemyLaserManagerResourceView *>(
                    g_EnemyLaserManager);
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
                g_EnemyLaserManager,
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
                g_ItemManager->Spawn(
                    reinterpret_cast<const AnmFloat3View *>(&sample),
                    8, 0xffffffffu, -1.5707964f, 0.6f);
            }

            EnemyLaserManagerResourceView *manager =
                reinterpret_cast<EnemyLaserManagerResourceView *>(
                    g_EnemyLaserManager);
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
                g_EnemyLaserManager,
                reinterpret_cast<EnemyLaserRequestScratch *>(&args),
                0);
        }
    }

    return hitCount;
}


// Natural vector/counter structure follows levzzz5154/th10-decomp, MIT
// copyright (c) 2026 levzzz, d1f31212514781c928fba145d8207397c8fb5650,
// matching2/msvc/src/game/laser/laser_rectangle_split.cpp.
// License: docs/licenses/th10-decomp-MIT.txt. TH10 vtable46DA10+18 ->41EB00.
int EnemyLaserType1View::CheckCollisionBox(
    EnemyLaserVectorView *center,
    EnemyLaserVectorView *size,
    int capture)
{
    int hitCount = 0;
    int sampleCount = 0;
    AnmFloat3View originalPosition =
        reinterpret_cast<const AnmFloat3View &>(position);
    unsigned char hits[256];
    memset(hits, 0, sizeof(hits));
    float distance = 6.0f;

    AnmFloat3View maximum;
    maximum = reinterpret_cast<const AnmFloat3View &>(*size) * 0.5f;
    AnmFloat3View minimum;
    minimum = reinterpret_cast<const AnmFloat3View &>(*center) - maximum;
    maximum = maximum + reinterpret_cast<const AnmFloat3View &>(*center);

    AnmFloat3View step;
    step.z = 0.0f;
    reinterpret_cast<EnemyLaserVectorView *>(&step)->FromAngleMagnitude(angle, 6.0f);
    AnmFloat3View sample;
    sample = step + reinterpret_cast<const AnmFloat3View &>(position);
    sample.z = 0.0f;
    step += step;

    while (distance + 6.0f < length)
    {
        if (!(sample.x < minimum.x) && !(sample.x > maximum.x) &&
            !(sample.y < minimum.y) && !(sample.y > maximum.y))
        {
            hits[sampleCount] = 1;
            ++hitCount;
            EnemyLaserVectorView *samplePosition =
                reinterpret_cast<EnemyLaserVectorView *>(&sample);
            if (capture != 0 &&
                samplePosition->IsOutsidePlayfield(32.0f, 32.0f) == 0)
            {
                g_ItemManager->Spawn(
                    reinterpret_cast<const AnmFloat3View *>(samplePosition),
                    8, 0xffffffffu, -1.5707964f, 0.6f);
            }
            EnemyLaserManagerResourceView *manager =
                reinterpret_cast<EnemyLaserManagerResourceView *>(g_EnemyLaserManager);
            manager->effectResource->CreateVmAtWorldVariant0(
                static_cast<int>(request.color) * 2 + 0x11, &sample);
        }
        sample += step;
        distance += 12.0f;
        ++sampleCount;
    }

    if (hitCount)
    {
        int sampleIndex = 0;
        while (sampleIndex < sampleCount && hits[sampleIndex] != 0)
            ++sampleIndex;
        if (sampleIndex != 0)
        {
            length = 0.0f;
        }
        else
        {
            int prefixLength = 0;
            while (sampleIndex < sampleCount && hits[sampleIndex] == 0)
            {
                ++sampleIndex;
                ++prefixLength;
            }
            if (sampleIndex >= sampleCount)
                goto done;
            length = static_cast<float>(prefixLength) * 12.0f;
        }
        while (sampleIndex < sampleCount)
        {
            while (sampleIndex < sampleCount && hits[sampleIndex] != 0)
                ++sampleIndex;
            if (sampleIndex >= sampleCount)
                break;
            int gapLength = 0;
            int gapStart = sampleIndex;
            while (sampleIndex < sampleCount && hits[sampleIndex] == 0)
            {
                ++sampleIndex;
                ++gapLength;
            }
            EnemyLaserType0RequestView args;
            memset(&args, 0, sizeof(args));
            args.maximumLength = static_cast<float>(gapLength) * 12.0f;
            args.initialLength = args.maximumLength;
            args.speed = 8.0f;
            reinterpret_cast<AnmFloat3View &>(args.position) =
                originalPosition + step * static_cast<float>(gapStart);
            args.angle = angle;
            args.width = width;
            args.type = request.type;
            args.color = request.color;
            args.terminalDistance = request.maximumLength - gapStart * 12.0f;
            EnemyFireLaser(g_EnemyLaserManager,
                reinterpret_cast<EnemyLaserRequestScratch *>(&args), 0);
        }
    }
done:
    return hitCount;
}

// Circle source leads: levzzz5154/th10-decomp, MIT (c) 2026 levzzz,
// d1f31212514781c928fba145d8207397c8fb5650,
// matching2/msvc/src/game/laser/laser_circle_split.cpp.
// License: docs/licenses/th10-decomp-MIT.txt. TH10 vtable46DA10+1C ->41EFA0.
int EnemyLaserType1View::CheckCollisionCircle(
    EnemyLaserVectorView *center, float radius, int capture)
{
    int hitCount = 0;
    int sampleCount = 0;
    AnmFloat3View originalPosition =
        reinterpret_cast<const AnmFloat3View &>(position);
    unsigned char hits[256];
    memset(hits, 0, sizeof(hits));
    float distance = 6.0f;
    AnmFloat3View step;
    step.z = 0.0f;
    reinterpret_cast<EnemyLaserVectorView *>(&step)->FromAngleMagnitude(angle, 6.0f);
    AnmFloat3View sample;
    sample = step + reinterpret_cast<const AnmFloat3View &>(position);
    sample.z = 0.0f;
    step += step;
    radius *= radius;

    while (distance + 6.0f < length)
    {
        EnemyLaserVectorView *samplePosition =
            reinterpret_cast<EnemyLaserVectorView *>(&sample);
        float dy = center->y - samplePosition->y;
        float dx = center->x - samplePosition->x;
        if (!(dx * dx + dy * dy > radius))
        {
            hits[sampleCount] = 1;
            ++hitCount;
            if (capture != 0 &&
                samplePosition->IsOutsidePlayfield(32.0f, 32.0f) == 0)
            {
                g_ItemManager->Spawn(
                    reinterpret_cast<const AnmFloat3View *>(samplePosition),
                    8, 0xffffffffu, -1.5707964f, 0.6f);
            }
            int scriptIndex = static_cast<int>(request.color) * 2 + 0x11;
            AnmLoadedView *resource =
                reinterpret_cast<EnemyLaserManagerResourceView *>(
                    g_EnemyLaserManager)->effectResource;
            AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
            unsigned int vmFlags = vm->flags35C;
            vm->flags35C = vmFlags | 0x40000000u;
            unsigned int *renderLayer = &vm->renderLayer;
            *renderLayer = 0;
            vm->positionOffset.x = samplePosition->x + 224.0f;
            vm->positionOffset.z = samplePosition->z;
            vm->positionOffset.y = samplePosition->y + 16.0f;
            StartAnmLoadedScript(resource, vm, scriptIndex);
            g_AnmRenderManagerView->AddVmVariant0(vm);
        }
        sample += step;
        distance += 12.0f;
        ++sampleCount;
    }

    if (hitCount)
    {
        int sampleIndex = 0;
        while (sampleIndex < sampleCount && hits[sampleIndex] != 0)
            ++sampleIndex;
        if (sampleIndex != 0)
            length = 0.0f;
        else
        {
            int prefixLength = 0;
            while (sampleIndex < sampleCount && hits[sampleIndex] == 0)
            {
                ++sampleIndex;
                ++prefixLength;
            }
            if (sampleIndex >= sampleCount)
                goto done;
            length = static_cast<float>(prefixLength) * 12.0f;
        }
        while (sampleIndex < sampleCount)
        {
            while (sampleIndex < sampleCount && hits[sampleIndex] != 0)
                ++sampleIndex;
            if (sampleIndex >= sampleCount)
                break;
            int gapLength = 0;
            int gapStart = sampleIndex;
            while (sampleIndex < sampleCount && hits[sampleIndex] == 0)
            {
                ++sampleIndex;
                ++gapLength;
            }
            EnemyLaserType0RequestView args;
            memset(&args, 0, sizeof(args));
            args.maximumLength = static_cast<float>(gapLength) * 12.0f;
            args.initialLength = args.maximumLength;
            args.speed = 8.0f;
            reinterpret_cast<AnmFloat3View &>(args.position) =
                originalPosition + step * static_cast<float>(gapStart);
            args.angle = angle;
            args.width = width;
            args.type = request.type;
            args.color = request.color;
            args.terminalDistance = request.maximumLength - gapStart * 12.0f;
            EnemyFireLaser(g_EnemyLaserManager,
                reinterpret_cast<EnemyLaserRequestScratch *>(&args), 0);
        }
    }
done:
    return hitCount;
}

// Construction seam for the four dispatcher laser groups. The 19-slot tables
// are target-bound external storage, not replacement two-slot C++ vtables.
// Only slot 1 is invoked here: both targets use ECX, a stack request and RET 4.
// Construction view of the nineteen surveyed virtual slots.
// Declarations encode reviewed i386 protocols; original names, full table extent,
// callback implementations and production data ownership remain unknown.
// These virtual methods are declarations only; the diagnostic link records
// unresolved address anchors and is not a runnable product.
struct EnemyLaserType0RequestConstructionView : EnemyLaserType0RequestView
{
    EnemyLaserType0RequestConstructionView()
    {
        memset(this, 0, sizeof(*this));
    }
};
struct EnemyLaserType1RequestConstructionView : EnemyLaserType1RequestView
{
    EnemyLaserType1RequestConstructionView()
    {
        memset(this, 0, sizeof(*this));
        fixed2C = 8.0f;
    }
};
struct EnemyLaserType0StorageView : EnemyLaserConstructionBaseView
{
    EnemyLaserType0RequestConstructionView request;
    AnmVmView primaryVm;
    AnmVmView secondaryVm;

    EnemyLaserType0StorageView() {}
    virtual void RunCommands();
    virtual int ApplyRequest(const EnemyLaserRequestScratch *request);
    virtual int Update();
    virtual int Draw();
    virtual int Unlink();
    virtual int Clear(int mode);
    virtual int CheckBox(EnemyLaserVectorView *, EnemyLaserVectorView *, int);
    virtual int CheckCircle(EnemyLaserVectorView *, float, int);
    virtual int CheckPoint(EnemyLaserVectorView *, float);
    virtual void Motion0010();
    virtual void Motion0040();
    virtual void Bounce();
};
struct EnemyLaserType1StorageView : EnemyLaserConstructionBaseView
{
    EnemyLaserType1RequestConstructionView request;
    AnmVmView primaryVm;
    AnmVmView secondaryVm;

    EnemyLaserType1StorageView() {}
    virtual int ApplyRequest(const EnemyLaserRequestScratch *request);
    virtual int Update();
    virtual int Draw();
    virtual int Unlink();
    virtual int Clear(int mode);
    virtual int CheckBox(EnemyLaserVectorView *, EnemyLaserVectorView *, int);
    virtual int CheckCircle(EnemyLaserVectorView *, float, int);
    virtual int CheckPoint(EnemyLaserVectorView *, float);
};
typedef char EnemyLaserConstructionBaseLayout[
    (sizeof(EnemyLaserConstructionBaseView) == 0x424 &&
     offsetof(EnemyLaserConstructionBaseView, timer) == 0x10 &&
     offsetof(EnemyLaserConstructionBaseView, id) == 0x54 &&
     sizeof(EnemyLaserType0RequestConstructionView) == 0x1dc &&
     sizeof(EnemyLaserType1RequestConstructionView) == 0x1f8) ? 1 : -1];
typedef char EnemyLaserType0Layout[
    (sizeof(EnemyLaserType0StorageView) == 0xd58 &&
     offsetof(EnemyLaserType0StorageView, request) == 0x424 &&
     offsetof(EnemyLaserType0StorageView, primaryVm) == 0x600 &&
     offsetof(EnemyLaserType0StorageView, secondaryVm) == 0x9ac) ? 1 : -1];
typedef char EnemyLaserType1Layout[
    (sizeof(EnemyLaserType1StorageView) == 0xd74 &&
     offsetof(EnemyLaserType1StorageView, request) == 0x424 &&
     offsetof(EnemyLaserType1StorageView, primaryVm) == 0x61c &&
     offsetof(EnemyLaserType1StorageView, secondaryVm) == 0x9c8) ? 1 : -1];

// Target 0x0040CF40. Keep the unusual raw request initialization independent of
// the enclosing object, as used by the real dispatcher and type-1 constructor.
EnemyLaserRequestScratch *EnemyInitializeLaserConfig(
    EnemyLaserRequestScratch *request)
{
    memset(request, 0, sizeof(*request));
    request->type1.fixed2C = 8.0f;
    return request;
}

// Target 0x0041C510. Allocation failure still reaches the target's null write;
// no new failure recovery is invented. Unknown types consume an ID but do not
// allocate or insert. The existing tail is a required sentinel/list node.
unsigned int EnemyFireLaser(
    void *managerStorage, EnemyLaserRequestScratch *request, int type)
{
    EnemyLaserManagerStorageView *manager =
        static_cast<EnemyLaserManagerStorageView *>(managerStorage);
    if (manager->count >= 256)
        return 0;
    if (++manager->nextId == 0)
        manager->nextId = 1;
    EnemyLaserConstructionBaseView *laser;
    switch (type)
    {
    case 0:
        {
            laser = new EnemyLaserType0StorageView;
        }
        break;
    case 1:
        {
            laser = new EnemyLaserType1StorageView;
        }
        break;
    default:
        return manager->nextId;
    }
    laser->id = manager->nextId;
    EnemyLaserCommonStorageView *previousTail = manager->tail;
    laser->previous = previousTail;
    previousTail->next = reinterpret_cast<EnemyLaserCommonStorageView *>(laser);
    ++manager->count;
    manager->tail = reinterpret_cast<EnemyLaserCommonStorageView *>(laser);
    laser->ApplyRequest(request);
    return manager->nextId;
}

// Target41CFD0: actual type-0 construction virtual, slot3C of46DA60.
void EnemyLaserType0StorageView::Bounce()
{
    EnemyLaserLineView &state = *reinterpret_cast<EnemyLaserLineView *>(this);
    EnemyLaserVectorView projected;
    projected.FromAngleMagnitude(state.angle, state.length);
    projected.x += state.position.x;
    projected.y += state.position.y;
    projected.z = 0.0f;

    if (!(projected.x <= g_EnemyPlayfieldMinX ||
          projected.x >= g_EnemyPlayfieldMaxX ||
          projected.y <= g_EnemyPlayfieldMinY ||
          projected.y >= g_EnemyPlayfieldMaxY)) {
        return;
    }

    int fired = 0;
    if (state.soundId >= 0) {
        reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)
            ->QueueSoundSample(state.soundId, 0);
    }

    if (projected.x < g_EnemyPlayfieldMinX ||
        projected.x >= g_EnemyPlayfieldMaxX) {
        state.request.position = projected;
        state.request.angle = EnemyWrapAngle(-state.angle - g_EnemyPi);
        state.request.speed = state.patterns[4].values[0].scalar;
        EnemyFireLaser(g_EnemyLaserManager,
            reinterpret_cast<EnemyLaserRequestScratch *>(&state.request), 0);
        fired = 1;
    }

    if ((state.flags & 0x08000000u) == 0 &&
        (projected.y < g_EnemyPlayfieldMinY ||
         (projected.y >= g_EnemyPlayfieldMaxY &&
          (state.flags & 0x400u) != 0))) {
        state.request.position = projected;
        state.request.angle = -state.angle;
        state.request.speed = state.patterns[4].values[0].scalar;
        EnemyFireLaser(g_EnemyLaserManager,
            reinterpret_cast<EnemyLaserRequestScratch *>(&state.request), 0);
    }
    else if (!fired) {
        return;
    }

    state.flags &= 0xf7fff3ffu;
}
