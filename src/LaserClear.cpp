#include "AnmLoadedStartScript.hpp"
#include "EnemyLaserLine.hpp"
#include "ItemManager.hpp"

// Target41E260, virtual slot14h / RET4. Sample the line at six units from
// its origin and then every twelve units, excluding the final six units.
int EnemyLaserLineView::Clear(int spawnItems)
{
    float distance;
    int count = 0;
    distance = 6.0f;
    EnemyLaserVectorView step;
    step.FromAngleMagnitude(angle, 6.0f);
    AnmFloat3View point;
    point = AnmFloat3View(step.x + position.x, step.y + position.y, position.z);
    step.z = 0.0f;
    step.x += step.x;
    step.y += step.y;
    if (12.0f < length)
    {
        do
        {
            ++count;
            const int scriptIndex = request.color * 2 + 0x11;
            AnmLoadedView *resource =
                reinterpret_cast<EnemyLaserManagerStorageView *>(
                    g_EnemyLaserManager)->effectResource;
            AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
            unsigned int flags = vm->flags35C;
            vm->flags35C = flags | 0x40000000u;
            vm->renderLayer = 0;
            AnmFloat3View *effectPosition = &vm->positionOffset;
            effectPosition->x = point.x + 224.0f;
            effectPosition->z = point.z;
            effectPosition->y = point.y + 16.0f;
            StartAnmLoadedScript(resource, vm, scriptIndex);
            AddVmToPrimaryBack(*g_AnmRenderManagerView, vm);
            if (spawnItems != 0 &&
                !(point.x + 32.0f <= -192.0f) &&
                !(point.x - 32.0f >= 192.0f) &&
                !(point.y + 32.0f <= 0.0f) &&
                !(point.y - 32.0f >= 448.0f))
                g_ItemManager->Spawn(
                    &point, 8, 0xffffffffu, -1.5707964f, 0.6f);
            point.x += step.x;
            point.y += step.y;
            point.z += step.z;
            distance += 12.0f;
        } while (distance + 6.0f < length);
    }
    unknown00C = 1;
    return count;
}
