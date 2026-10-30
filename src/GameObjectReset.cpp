#include "Main.hpp"
#include "EnemyLaser.hpp"
#include <new>

struct GameChainOwnerView;

typedef char GameLaserResetLinks[
    (offsetof(EnemyLaserCommonStorageView, previous) == 4 &&
     offsetof(EnemyLaserCommonStorageView, next) == 8 &&
     offsetof(EnemyLaserManagerStorageView, sentinel) +
         offsetof(EnemyLaserConstructionBaseView, next) == 0x18) ? 1 : -1];

// Target417770 walks the existing laser owner's intrusive list. The virtual
// slot4 call precedes the link rereads and ordinary scalar deallocation.
// Original class/TU ownership and the historical helper name are unknown.
void GameClearLinkedObjects(GameChainOwnerView *owner)
{
    EnemyLaserManagerStorageView *manager =
        reinterpret_cast<EnemyLaserManagerStorageView *>(owner);
    EnemyLaserCommonStorageView *node = manager->sentinel.next;
    while (node != NULL)
    {
        EnemyLaserCommonStorageView *next = node->next;
        reinterpret_cast<EnemyLaserConstructionBaseView *>(node)->Unlink();
        node->previous->next = node->next;
        if (node->next != NULL)
            node->next->previous = node->previous;
        ::operator delete(node);
        node = next;
    }
}
