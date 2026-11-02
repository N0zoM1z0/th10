#include "Main.hpp"
#include "ScriptViewerOwners.hpp"
#include "EnemyGameState.hpp"
#include "EnemyCancel.hpp"
#include "EnemyLaser.hpp"
#include "Player.hpp"
#include "BulletManager.hpp"

// Target 0x405AC0: one-shot Bomb clearing at the owner's current world
// center, using the actual bullet and enemy-laser cancellation interfaces.
// Special spell IDs have the observed distinct cancel-flags behavior.
extern BulletManagerView *g_ScriptViewerBullets;
int ScriptViewerBombOwnerView::EmitAuxiliary()
{
    if (mode28 == 0)
        return 0;

    EnemyGameStateView *state = g_EnemyGameState;
    const unsigned int spellFlags = state->flags378C;
    const int spellId = state->spellId;
    const bool useNoItemDrop =
        ((spellFlags & 1u) != 0 && spellId >= 93 && spellId <= 96) ||
        spellId == 109;
    const int cancelMode = (~spellFlags) & 1u;
    reinterpret_cast<EnemyCancelManagerView *>(g_ScriptViewerBullets)->
        CancelBulletPattern(
            reinterpret_cast<const PlayerFloat3 *>(&position30),
            *reinterpret_cast<const float *>(&unknown03C),
            cancelMode, useNoItemDrop ? 0 : 1);

    const unsigned int laserMode = (~g_EnemyGameState->flags378C) & 1u;
    reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager)->
        ApplyPatternCancel(
            reinterpret_cast<const PlayerFloat3 *>(&position30),
            *reinterpret_cast<const float *>(&unknown03C), laserMode);
    return 0;
}
