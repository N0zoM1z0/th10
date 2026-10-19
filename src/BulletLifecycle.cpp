#include "BulletManager.hpp"

#include <string.h>

// TH10 0x405ED0..0x405EFC. The final array entry is explicitly set to state
// 5 after clearing all 2001 records; the surrounding draw buckets/count and
// loaded-resource pointer are outside that clear. Original source owner unknown.
void BulletManagerView::ResetPool()
{
    g_AnmRenderManagerView->MarkLoadedVmsForDeletion(bulletAnm);
    memset(bullets, 0, sizeof(bullets));
    bulletCursor = bullets;
    bullets[2000].state = 5;
}
