#include "BulletManager.hpp"

// Native target-read float operands in the shared game constant pool. Only
// their addresses and consumed binary32 values are established here.
extern float g_BulletDrawWorldOffsetX; // 0x00470B4C = +224.0f
extern float g_BulletDrawWorldOffsetY; // 0x00470B48 =  +16.0f
struct BulletUpdateGateView
{
    unsigned char unknown000[0x58];
    unsigned int flags58;
};
extern BulletUpdateGateView *g_BulletUpdateGate;

// Target 0x004066E0, complete 134-byte draw-bucket owner. Its real caller
// passes the manager in EAX and bucket index in ECX after LTCG promotion.
int BulletManagerView::DrawBucket(int bucketIndex)
{
    // Retail's EAX + ECX*4 + 0x14 reads a flat Win32 address word, even
    // when a script-derived bucket number exceeds the declared array.
    BulletRuntimeView *bullet = *reinterpret_cast<BulletRuntimeView *const *>(
        reinterpret_cast<unsigned int>(&drawBucketHeads[0]) +
        static_cast<unsigned int>(bucketIndex) *
        static_cast<unsigned int>(sizeof(BulletRuntimeView *)));
    for (; bullet != NULL; bullet = bullet->nextInDrawBucket)
    {
        const volatile float &worldX = bullet->position.x;
        const volatile float &worldY = bullet->position.y;
        bullet->vm.position.x = worldX + g_BulletDrawWorldOffsetX;
        bullet->vm.position.y = worldY + g_BulletDrawWorldOffsetY;
        bullet->vm.position.z = bullet->position.z;
        if ((bullet->vm.flags35C & 0x08000000u) != 0)
        {
            bullet->vm.rotation.z = AddNormalizeAngle(bullet->angle, 1.5707964f);
            bullet->vm.flags35C |= 4u;
        }
        g_AnmRenderManagerView->Draw(&bullet->vm);
    }
    return 1;
}

// Target 0x004067A0: registered draw chain callback over all six buckets.
int __fastcall BulletManagerDrawCallback(BulletManagerView *manager)
{
    BulletUpdateGateView *gate = g_BulletUpdateGate;
    if (gate == NULL || (gate->flags58 & 4u) == 0)
    {
        for (int bucket = 0; bucket < 6; ++bucket)
            manager->DrawBucket(bucket);
    }
    return 1;
}
