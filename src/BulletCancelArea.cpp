#include "BulletManager.hpp"
#include "ItemManager.hpp"

// Three target data-array bases used to tint newly created cancellation VMs.
// The physical table owners and original symbol names are not yet known.
extern unsigned int g_BulletCancelPaletteSmall[16];
extern unsigned int g_BulletCancelPaletteMedium[8];
extern unsigned int g_BulletCancelPaletteLarge[4];

// Target 0x00408210: cancels a rectangular slice of the active 2000-bullet
// pool. The manager supplies the area center and full dimensions; the target
// precomputes all three components, although overlap tests use only X and Y.
// This private method receives its manager in EBX and spawnItems on the stack.
int BulletManagerView::CancelBulletsInArea(int spawnItems)
{
    const AnmFloat3View half = cancelAreaSize * 0.5f;
    const AnmFloat3View lower = cancelAreaCenter - half;
    const AnmFloat3View upper = cancelAreaCenter + half;

    BulletRuntimeView *bullet = bullets;
    for (int remaining = 2000; remaining != 0; --remaining, ++bullet)
    {
        if (bullet->state == 0 || bullet->state == 3)
            continue;

        float halfWidth = bullet->collisionWidth * 0.5f;
        float halfHeight = bullet->collisionHeight * 0.5f;
        if (bullet->position.x + halfWidth < lower.x ||
            bullet->position.x - halfWidth > upper.x ||
            bullet->position.y + halfHeight < lower.y ||
            bullet->position.y - halfHeight > upper.y)
            continue;

        bullet->flags |= 8;
        if (spawnItems != 0)
            g_ItemManager->Spawn(
                reinterpret_cast<const AnmFloat3View *>(&bullet->position),
                8, static_cast<unsigned int>(-1),
                -1.5707964f, 0.6f);

        AnmVmIdView vmId = bulletAnm->CreateVmAtWorldVariant0(
            0x173, reinterpret_cast<const AnmFloat3View *>(
                &bullet->position));
        AnmVmView *effectVm = vmId.GetVm();
        AnmSpriteView *sprite = bullet->vm.loadedSprite;
        if (sprite != NULL)
        {
            if (!(sprite->width > 16.0f))
                effectVm->primaryColor.value =
                    g_BulletCancelPaletteSmall[bullet->color];
            else if (!(sprite->width > 32.0f))
                effectVm->primaryColor.value =
                    g_BulletCancelPaletteMedium[bullet->color];
            else
                effectVm->primaryColor.value =
                    g_BulletCancelPaletteLarge[bullet->color];
        }
        bullet->unknown450 = 0;
    }
    return 0;
}
