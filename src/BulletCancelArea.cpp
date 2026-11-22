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
    AnmFloat3View half;
    AnmFloat3View lower;
    AnmFloat3View upper;
    half = cancelAreaSize * 0.5f;
    lower = cancelAreaCenter - half;
    upper = half + cancelAreaCenter;

    BulletRuntimeView *bullet = bullets;
    for (int remaining = 2000; remaining != 0; --remaining, ++bullet)
    {
        if (bullet->state == 0 || bullet->state == 3)
            continue;

        PlayerFloat3 bulletLower;
        PlayerFloat3 bulletUpper;
        float halfWidth = bullet->collisionWidth * 0.5f;
        bulletLower.x = bullet->position.x - halfWidth;
        float halfHeight = bullet->collisionHeight * 0.5f;
        bulletLower.y = bullet->position.y - halfHeight;
        bulletUpper.x = bullet->position.x + halfWidth;
        bulletUpper.y = bullet->position.y + halfHeight;
        if (bulletUpper.x < lower.x || bulletLower.x > upper.x ||
            bulletUpper.y < lower.y || bulletLower.y > upper.y)
            continue;

        bullet->flags |= 8;
        if (spawnItems != 0)
            g_ItemManager->Spawn(
                reinterpret_cast<const AnmFloat3View *>(&bullet->position),
                8, static_cast<unsigned int>(-1),
                -1.5707964f, 0.6f);

        AnmVmView *effectVm = g_AnmRenderManagerView->FindVm(
            bulletAnm->CreateVmAtWorldVariant0(
                0x173, reinterpret_cast<const AnmFloat3View *>(
                    &bullet->position)));
        AnmSpriteView *sprite = bullet->vm.loadedSprite;
        if (sprite != NULL)
        {
            if (sprite->width <= 16.0f)
                effectVm->primaryColor.value =
                    g_BulletCancelPaletteSmall[bullet->color];
            else if (sprite->width <= 32.0f)
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
