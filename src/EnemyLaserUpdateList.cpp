#include "EnemyLaser.hpp"
#include "GameManager.hpp"
#include <new>

// Native 0x0041C330..0x0041C441 (274 bytes). The compiler-private incoming
// manager receiver lives in EDI after its real update-chain caller. This
// descriptive member declaration is not an assertion of historical C++ ABI.
int EnemyLaserManagerStorageView::UpdateLinkedLasers()
{
    EnemyLaserConstructionBaseView *laser =
        reinterpret_cast<EnemyLaserConstructionBaseView *>(sentinel.next);
    if (laser == NULL)
        return 1;

    do
    {
        EnemyLaserConstructionBaseView *next =
            reinterpret_cast<EnemyLaserConstructionBaseView *>(laser->next);
        bool expired = false;
        if (laser->removalPhase != 0)
        {
            unsigned char phase = static_cast<unsigned char>(laser->removalPhase + 1);
            laser->removalPhase = phase;
            expired = phase >= 2;
        }
        if (expired || laser->unknown00C == 1)
        {
            laser->Unlink();
            --count;
            laser->previous->next = laser->next;
            if (laser->next != NULL)
                laser->next->previous = laser->previous;
            if (tail == reinterpret_cast<EnemyLaserCommonStorageView *>(laser))
                tail = laser->previous;
            ::operator delete(laser);
        }
        else if (laser->Update() != 0)
        {
            laser->Unlink();
            --count;
            EnemyLaserCommonStorageView *previous = laser->previous;
            EnemyLaserCommonStorageView *following = laser->next;
            previous->next = following;
            if (following != NULL)
                following->previous = previous;
            if (tail == reinterpret_cast<EnemyLaserCommonStorageView *>(laser))
                tail = previous;
            ::operator delete(laser);
        }
        else
        {
            // The actual AnmVmTimerView uses previous/current/subframe,
            // plus a scale pointer. It includes the target's 0.99 / 1.01
            // normal-speed lane and its native float-to-int conversion lane.
            laser->timer.Tick();
        }
        laser = next;
    } while (laser != NULL);
    return 1;
}

// Original registered Chain callback at 0x0041C480. The gate word is the
// existing real GameManager flags at +0x58 of global 0x00477810; do not
// introduce another shadow manager or allocation for those same bytes.

struct LaserFrameFlagBits
{
    unsigned int paused : 1;
    unsigned int frozen : 1;
    unsigned int idle : 1;
    unsigned int reserved03 : 7;
    unsigned int deferred : 1;
    unsigned int reserved0B : 21;
};
struct LaserFrameFlagView
{
    unsigned char unused000[0x58];
    LaserFrameFlagBits bits;
};
typedef char LaserFrameFlagSizeIs4[(sizeof(LaserFrameFlagBits) == 4) ? 1 : -1];
typedef char LaserFrameFlagOffsetIs58[
    (offsetof(LaserFrameFlagView, bits) == 0x58) ? 1 : -1];

int __fastcall EnemyLaserUpdateCallback(void *owner)
{
    EnemyLaserManagerStorageView *manager =
        static_cast<EnemyLaserManagerStorageView *>(owner);
    const LaserFrameFlagView *const game =
        reinterpret_cast<const LaserFrameFlagView *>(g_GameManager);
    if (game->bits.paused | game->bits.idle)
        return 1;
    if (game->bits.deferred)
        return 1;
    if (game->bits.frozen)
    {
        const float oldSpeed = g_AnmGameSpeed;
        g_AnmGameSpeed = 0.0f;
        const int result = manager->UpdateLinkedLasers();
        g_AnmGameSpeed = oldSpeed;
        return result;
    }
    return manager->UpdateLinkedLasers();
}

// The registered draw-chain callback at original 0x0041C4E0, recorded by
// EnemyLaserManagerStorageView::Initialize at 0x0041C17A. Its real C++
// callee is retained as a separately compiled 43-byte owner by VC7.1, but
// LTCG inlines the traversal here (native 46-byte callback has no E8 call).
int __fastcall EnemyLaserDrawList(void *owner);
int __fastcall EnemyLaserDrawCallback(void *owner)
{
    if ((g_GameManager->m_Flags & 4u) == 0)
        return EnemyLaserDrawList(owner);
    return 1;
}

// Target 0x0041C450..0x0041C47A is a separate 43-byte laser draw-list
// function, though no independent incoming reference or authored C++ name
// was established. It consumes manager in the native private EAX register.
int __fastcall EnemyLaserDrawList(void *owner)
{
    EnemyLaserManagerStorageView *manager =
        static_cast<EnemyLaserManagerStorageView *>(owner);
    EnemyLaserConstructionBaseView *laser =
        reinterpret_cast<EnemyLaserConstructionBaseView *>(manager->sentinel.next);
    while (laser != NULL)
    {
        EnemyLaserConstructionBaseView *next =
            reinterpret_cast<EnemyLaserConstructionBaseView *>(laser->next);
        if (laser->unknown00C != 1)
            laser->Draw();
        laser = next;
    }
    return 1;
}

// Target 0x0041C880..0x0041C8B5 is a physically independent 54-byte
// accumulation over non-state-1 lasers. It takes manager privately in EAX
// and two stack words; no actual original caller is independently known.
int EnemyLaserManagerStorageView::AccumulatePointChecks(
    EnemyLaserVectorView *point, float radius)
{
    int total = 0;
    EnemyLaserConstructionBaseView *laser =
        reinterpret_cast<EnemyLaserConstructionBaseView *>(sentinel.next);
    while (laser != NULL)
    {
        EnemyLaserConstructionBaseView *next =
            reinterpret_cast<EnemyLaserConstructionBaseView *>(laser->next);
        if (laser->unknown00C != 1)
            total += laser->CheckPoint(point, radius);
        laser = next;
    }
    return total;
}
