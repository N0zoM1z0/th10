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
    if (((spellFlags & 1u) != 0 &&
         state->spellId >= 93 && state->spellId <= 96) ||
        *reinterpret_cast<volatile const int *>(&state->spellId) == 109)
        reinterpret_cast<EnemyCancelManagerView *>(g_ScriptViewerBullets)->
            CancelBulletPattern(reinterpret_cast<const PlayerFloat3 *>(&position30),
                collisionRadius, (~spellFlags) & 1u, 0);
    else
        reinterpret_cast<EnemyCancelManagerView *>(g_ScriptViewerBullets)->
            CancelBulletPattern(reinterpret_cast<const PlayerFloat3 *>(&position30),
                collisionRadius, (~spellFlags) & 1u, 1);

    const unsigned int laserMode = (~g_EnemyGameState->flags378C) & 1u;
    reinterpret_cast<EnemyEffectWaitManager *>(g_EnemyLaserManager)->
        ApplyPatternCancel(
            reinterpret_cast<const PlayerFloat3 *>(&position30),
            collisionRadius, laserMode);
    return 0;
}
