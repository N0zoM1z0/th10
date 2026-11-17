#include "Gui.hpp"
#include "Enemy.hpp"
#include "EnemyLaser.hpp"
#include "BulletManager.hpp"

#include <new>
#include <string.h>

extern void *g_EnemyBulletManager;
extern EnemyManagerView *g_EnemyManager;
unsigned int EnemyCancelBulletRecord(unsigned char *bullet);

// Complete target 0x0040E6A0 (182 bytes). The argument owns the traversal and
// timer; effect resources deliberately come from the global enemy manager.
void __stdcall EnemyKillAll(EnemyManagerView *manager)
{
    EnemyListNodeView *node = manager->enemyListHead;
    while (node != 0)
    {
        EnemyFullObjectView *enemy = node->enemy;
        EnemyListNodeView *next = node->next;
        if (((enemy->runtime.flags & 0x50u) == 0 && (enemy->runtime.flags & 0xc000u) == 0) ||
            (enemy->runtime.flags & 0x80u) != 0)
        {
            if (enemy->runtime.deathEffectScript >= 0)
            {
                reinterpret_cast<AnmLoadedView *>(
                    g_EnemyManager->effectResources[enemy->runtime.deathEffectResourceIndex])->
                    CreateVmAtWorldVariant0(enemy->runtime.deathEffectScript,
                        reinterpret_cast<const AnmFloat3View *>(
                            &enemy->runtime.worldMotion.position));
            }
            enemy->runtime.flags |= 0x20000u;
        }
        node = next;
    }

    // Retail reads the pointed-to scale as volatile before the subframe;
    // keep that operand live to preserve the native x87 addition order.
    volatile float *const timerScale = manager->timer.scale;
    manager->timer.previous = manager->timer.current;
    if (*timerScale > 0.99f && *timerScale < 1.01f)
    {
        ++manager->timer.current;
        manager->timer.subframe += 1.0f;
    }
    else
    {
        manager->timer.subframe = *timerScale + manager->timer.subframe;
        manager->timer.current = static_cast<int>(manager->timer.subframe);
    }
}

// Complete target 0x00415B00 (683 bytes). Member construction precedes
// the target bulk clear; ordinary new in the real caller owns failure cleanup.
GuiMessageVmView::GuiMessageVmView(const unsigned char *record)
{
    GuiMessageVmView *message = this;
    memset(message, 0, sizeof(*message));
    message->lifetimeTimer.SetCurrent(0);
    message->instructionTimer.SetCurrent(0);
    message->unknown58 = 0;
    message->waitTimer.SetCurrent(0);
    message->currentInstruction = reinterpret_cast<GuiMessageInstructionView *>(
        const_cast<unsigned char *>(record));

    AnmRenderManagerView *anm = g_AnmRenderManagerView;
    AnmLoadedView *text = g_AsciiManagerView->textAnm;
    AnmVmView *vm = anm->AllocateVm();
    vm->renderLayer = 15;
    vm->flags35C |= 0x40000000u;
    text->InitializeVm(vm, 0);
    anm = g_AnmRenderManagerView;
    AnmVmIdView firstTextId = anm->AddVmVariant0(vm);
    message->vmIds[GUI_MESSAGE_TEXT_LINE_0].value = firstTextId.value;

    text = g_AsciiManagerView->textAnm;
    vm = anm->AllocateVm();
    vm->renderLayer = 15;
    vm->flags35C |= 0x40000000u;
    text->InitializeVm(vm, 1);
    anm = g_AnmRenderManagerView;
    firstTextId = anm->AddVmVariant0(vm);
    message->vmIds[GUI_MESSAGE_TEXT_LINE_1].value = firstTextId.value;

    vm = anm->FindVm(message->vmIds[GUI_MESSAGE_TEXT_LINE_0]);
    if (vm == 0) message->vmIds[GUI_MESSAGE_TEXT_LINE_0].value = 0;
    vm->glyphWidth = 16;
    vm = anm->FindVm(message->vmIds[GUI_MESSAGE_TEXT_LINE_0]);
    if (vm == 0) message->vmIds[GUI_MESSAGE_TEXT_LINE_0].value = 0;
    vm->glyphHeight = 16;
    vm = anm->FindVm(message->vmIds[GUI_MESSAGE_TEXT_LINE_1]);
    if (vm == 0) message->vmIds[GUI_MESSAGE_TEXT_LINE_1].value = 0;
    vm->glyphWidth = 16;
    vm = anm->FindVm(message->vmIds[GUI_MESSAGE_TEXT_LINE_1]);
    if (vm == 0) message->vmIds[GUI_MESSAGE_TEXT_LINE_1].value = 0;
    vm->glyphHeight = 16;

    message->textPositions[0] = AnmFloat3View(8.0f, 0.0f, 0.0f);
    message->textPositions[1] = AnmFloat3View(24.0f, 0.0f, 0.0f);
    message->alternatingLine = 0;
    message->activeSide = 0;
    message->textColors[0] = 0x00f8f08fu;
    message->textColors[1] = 0x008088ffu;

    BulletManagerView *bullets = reinterpret_cast<BulletManagerView *>(g_EnemyBulletManager);
    BulletRuntimeView *bullet = &bullets->bullets[0];
    int remainingBullets = 2000;
    do
    {
        if (bullet->state != 0 && bullet->state != 3)
            EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(bullet));
        ++bullet;
    } while (--remainingBullets != 0);

    EnemyEffectWaitNode *laser = reinterpret_cast<EnemyEffectWaitManager *>(
        g_EnemyLaserManager)->head;
    while (laser != 0)
    {
        EnemyEffectWaitNode *next = laser->next;
        if (laser->state != 1)
            laser->ApplyClear(0);
        laser = next;
    }
    EnemyKillAll(g_EnemyManager);
}
