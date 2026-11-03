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
            ++laser->removalPhase;
            if (laser->removalPhase >= 2)
                expired = true;
        }
        if (expired || laser->unknown00C == 1)
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

int __fastcall EnemyLaserUpdateCallback(void *owner)
{
    EnemyLaserManagerStorageView *manager =
        static_cast<EnemyLaserManagerStorageView *>(owner);
    const unsigned int flags = g_GameManager->m_Flags;
    if (((flags | (flags >> 2)) & 1u) != 0 || (flags & 0x400u) != 0)
        return 1;
    if ((flags & 2u) != 0)
    {
        const float oldSpeed = g_AnmGameSpeed;
        g_AnmGameSpeed = 0.0f;
        manager->UpdateLinkedLasers();
        g_AnmGameSpeed = oldSpeed;
    }
    else
    {
        manager->UpdateLinkedLasers();
    }
    return 1;
}

// The registered draw-chain callback at original 0x0041C4E0, recorded by
// EnemyLaserManagerStorageView::Initialize at 0x0041C17A. This matches the
// pre-existing function-pointer declaration using a void* parameter.
int __fastcall EnemyLaserDrawCallback(void *owner)
{
    EnemyLaserManagerStorageView *manager =
        static_cast<EnemyLaserManagerStorageView *>(owner);
    if ((g_GameManager->m_Flags & 4u) == 0)
    {
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
    }
    return 1;
}
