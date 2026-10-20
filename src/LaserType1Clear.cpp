#include "EnemyLaserType1.hpp"
#include "ItemManager.hpp"

// Target41F400, slot14h / RET4 / integer EAX count. Count every segment;
// only the outer16-unit rectangle permits animation or nested item creation.
int EnemyLaserType1View::Clear(int spawnItems)
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
            if (!(point.x + 16.0f <= -192.0f) &&
                !(point.x - 16.0f >= 192.0f))
            {
                float animationY = point.y + 16.0f;
                if (!(animationY <= 0.0f) &&
                    !(point.y - 16.0f >= 448.0f))
                {
                    int scriptIndex = request.color * 2 + 0x11;
                    AnmLoadedView *resource =
                        reinterpret_cast<EnemyLaserManagerStorageView *>(
                            g_EnemyLaserManager)->effectResource;
                    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
                    vm->flags35C |= 0x40000000u;
                    vm->renderLayer = 0;
                    vm->positionOffset.x = point.x + 224.0f;
                    vm->positionOffset.y = animationY;
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
                }
            }
            point.x += step.x;
            point.y += step.y;
            point.z += step.z;
            distance += 12.0f;
        } while (distance + 6.0f < length);
    }
    state = 1;
    return count;
}
