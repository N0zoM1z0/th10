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
        EnemyRuntimeView *enemy = &node->enemy->runtime;
        EnemyListNodeView *next = node->next;
        if ((enemy->flags & 0xc050u) == 0 || (enemy->flags & 0x80u) != 0)
        {
            if (enemy->deathEffectScript >= 0)
            {
                reinterpret_cast<AnmLoadedView *>(
                    g_EnemyManager->effectResources[enemy->deathEffectResourceIndex])->
                    CreateVmAtWorldVariant0(enemy->deathEffectScript,
                        reinterpret_cast<const AnmFloat3View *>(
                            &enemy->worldMotion.position));
            }
            enemy->flags |= 0x20000u;
        }
        node = next;
    }

    manager->timer.previous = manager->timer.current;
    if (*manager->timer.scale > 0.99f && *manager->timer.scale < 1.01f)
    {
        ++manager->timer.current;
        manager->timer.subframe += 1.0f;
    }
    else
    {
        manager->timer.Add(*manager->timer.scale);
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
    message->vmIds[GUI_MESSAGE_TEXT_LINE_0] = anm->AddVmVariant0(vm);

    text = g_AsciiManagerView->textAnm;
    vm = anm->AllocateVm();
    vm->renderLayer = 15;
    vm->flags35C |= 0x40000000u;
    text->InitializeVm(vm, 1);
    anm = g_AnmRenderManagerView;
    message->vmIds[GUI_MESSAGE_TEXT_LINE_1] = anm->AddVmVariant0(vm);

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
    for (int i = 0; i < 2000; ++i)
    {
        BulletRuntimeView *bullet = &bullets->bullets[i];
        if (bullet->state != 0 && bullet->state != 3)
            EnemyCancelBulletRecord(reinterpret_cast<unsigned char *>(bullet));
    }

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
