#include "EnemyLaser.hpp"

// TH10 0x0041C800: update the shared world-space cancellation center and
// propagate one pattern-cancel operation through the live effect list.
unsigned int EnemyEffectWaitManager::ApplyPatternCancel(
    const PlayerFloat3 *position, float radius, int mode)
{
    EnemyEffectWaitNode *node = head;
    cancelPosition = *position;
    unsigned int applied = 0;
    while (node != NULL)
    {
        EnemyEffectWaitNode *next = node->next;
        if (node->state != 1)
            applied += node->ApplyPatternCancel(position, radius, mode);
        node = next;
    }
    return applied;
}
