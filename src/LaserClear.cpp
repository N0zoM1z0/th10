#include "EnemyLaserLine.hpp"
#include "ItemManager.hpp"

// Target41E260, virtual slot14h / RET4. Sample the line at six units from
// its origin and then every twelve units, excluding the final six units.
int EnemyLaserLineView::Clear(int spawnItems)
{
    float distance = 6.0f;
    int count = 0;
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
            const int color = request.color;
            AnmLoadedView *resource =
                reinterpret_cast<EnemyLaserManagerStorageView *>(
                    g_EnemyLaserManager)->effectResource;
            ++count;
            const int scriptIndex = color * 2 + 0x11;
            AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
            vm->flags35C |= 0x40000000u;
            vm->renderLayer = 0;
            vm->positionOffset.x = point.x + 224.0f;
            vm->positionOffset.y = point.y + 16.0f;
            vm->positionOffset.z = point.z;
            resource->InitializeAndExecuteScriptIndex(vm, scriptIndex);
            g_AnmRenderManagerView->AddVmVariant0(vm);
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
