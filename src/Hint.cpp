#include "Main.hpp" // Must shield ANM imports before the Windows SDK.
#include "Hint.hpp"
#include "FileSystem.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <direct.h>

namespace
{
struct HintNameValue
{
    const char *name;
    int value;
};
// Target474494 and4744B0. Logical tables only: physical storage still open.
const HintNameValue alignmentNames[] = {
    {"center", 0}, {"left", 1}, {"right", 2}
};
const HintNameValue baseNames[] = {
    {"start",0},
    {"converse_1",1}, {"converse_2",2}, {"converse_3",3},
    {"converse_4",4}, {"converse_5",5},
    {"mboss_1",6}, {"mboss_2",7}, {"mboss_3",8}, {"mboss_4",9},
    {"mboss_5",10}, {"mboss_6",11}, {"mboss_7",12}, {"mboss_8",13}, {"mboss_9",14},
    {"mboss_card_1",15}, {"mboss_card_2",16}, {"mboss_card_3",17},
    {"mboss_card_4",18}, {"mboss_card_5",19}, {"mboss_card_6",20},
    {"mboss_card_7",21}, {"mboss_card_8",22}, {"mboss_card_9",23},
    {"boss_1",24}, {"boss_2",25}, {"boss_3",26}, {"boss_4",27}, {"boss_5",28},
    {"boss_6",29}, {"boss_7",30}, {"boss_8",31}, {"boss_9",32}, {"boss_10",33},
    {"boss_11",34}, {"boss_12",35}, {"boss_13",36}, {"boss_14",37},
    {"boss_15",38}, {"boss_16",39}, {"boss_17",40}, {"boss_18",41}, {"boss_19",42},
    {"boss_card_1",43}, {"boss_card_2",44}, {"boss_card_3",45},
    {"boss_card_4",46}, {"boss_card_5",47}, {"boss_card_6",48},
    {"boss_card_7",49}, {"boss_card_8",50}, {"boss_card_9",51},
    {"boss_card_10",52}, {"boss_card_12",53}, {"boss_card_13",54},
    {"boss_card_14",55}, {"boss_card_15",56}, {"boss_card_16",57},
    {"boss_card_17",58}, {"boss_card_18",59}, {"boss_card_19",60}
};
}

int __stdcall ExportAutomaticHints(HintOwnerView *owner, const char *path)
{
    char *buffer = static_cast<char *>(malloc(4096));
    _mkdir("hint");
    if (FileSystem::OpenWriteFile(path) == 0)
    {
        sprintf(buffer, "# ========================================================= \r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        // CP932 literals46D510,46D4D8,46D490, not UTF-8 replacements.
        sprintf(buffer, "# \x93\x8c\x95\xfb\x95\x97\x90\x5f\x98\x5e\x81\x40\x8d\x55\x97\xaa\x83\x71\x83\x93\x83\x67\x83\x74\x83\x40\x83\x43\x83\x8b \r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "#\r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "# \x82\xb1\x82\xcc\x83\x74\x83\x40\x83\x43\x83\x8b\x82\xcd\x81\x41\x8e\xa9\x93\xae\x93\x49\x82\xc9\x90\xb6\x90\xac\x81\x41\x8f\xe3\x8f\x91\x82\xab\x82\xb3\x82\xea\x82\xdc\x82\xb7 \r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "# \x8e\xa9\x95\xaa\x82\xc5\x8d\x55\x97\xaa\x83\x71\x83\x93\x83\x67\x82\xf0\x8d\x5c\x90\xac\x82\xb5\x82\xbd\x82\xa2\x8f\xea\x8d\x87\x82\xcd\x81\x41\x95\xca\x82\xcc\x83\x74\x83\x40\x83\x43\x83\x8b\x82\xf0\x8e\x67\x97\x70\x82\xb5\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2 \r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "#\r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        time_t now;
        time(&now);
        tm *stamp = localtime(&now);
        sprintf(buffer, "#                                Time-stamp: <%.4d/%.2d/%.2d %.2d:%.2d>\r\n",
                stamp->tm_year + 1900, stamp->tm_mon + 1, stamp->tm_mday,
                stamp->tm_hour, stamp->tm_min);
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "\r\n\r\n");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
        sprintf(buffer, "Version = %s\r\n\r\n", "0.0");
        FileSystem::WriteToOpenFile(buffer, strlen(buffer));

        for (int stage = 0; stage < 8; ++stage)
        {
            int emitted = 0;
            for (HintLinkView *node = owner->exportStageLists[stage].next; node; )
            {
                HintLinkView *next = node->next;
                HintEntryView *entry = static_cast<HintEntryView *>(node->payload);
                if (entry->remaining != 0)
                {
                    if (emitted == 0)
                    {
                        sprintf(buffer, "# ================================== \r\n");
                        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                        sprintf(buffer, "Stage : %d\r\n\r\n", stage);
                        FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    }
                    sprintf(buffer, "Tips\r\n", stage);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    if (entry->remaining > 0)
                        sprintf(buffer, "\tRemain\t: %d\r\n", entry->remaining);
                    // Native negative-remaining path writes the prior Tips again.
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tText\t: \"%s\"\r\n", entry->text);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tPos\t\t: %d, %d\r\n",
                            static_cast<int>(entry->position.x), static_cast<int>(entry->position.y));
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tCount\t: %d\r\n", entry->count);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    const char *baseName = 0;
                    for (int baseIndex = 0; baseIndex < 61; ++baseIndex)
                        if (baseNames[baseIndex].value == entry->base)
                        {
                            baseName = baseNames[baseIndex].name;
                            break;
                        }
                    sprintf(buffer, "\tBase\t: %s\r\n", baseName);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    const char *alignmentName = 0;
                    for (int alignmentIndex = 0; alignmentIndex < 3; ++alignmentIndex)
                        if (alignmentNames[alignmentIndex].value == entry->alignment)
                        {
                            alignmentName = alignmentNames[alignmentIndex].name;
                            break;
                        }
                    sprintf(buffer, "\tAlign\t: %s\r\n", alignmentName);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tTime\t: %d\r\n", entry->time);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tAlpha\t: %d\r\n", static_cast<int>(entry->alpha));
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tColor\t: %d, %d, %d\r\n", static_cast<int>(entry->red),
                            static_cast<int>(entry->green), static_cast<int>(entry->blue));
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "\tScale\t: %.1f\r\n", static_cast<double>(entry->scale));
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    sprintf(buffer, "End\r\n\r\n", stage);
                    FileSystem::WriteToOpenFile(buffer, strlen(buffer));
                    ++emitted;
                    if (emitted >= 255)
                        break;
                }
                node = next;
            }
            if (emitted != 0)
            {
                sprintf(buffer, "StageEnd\r\n");
                FileSystem::WriteToOpenFile(buffer, strlen(buffer));
            }
        }
    }
    FileSystem::CloseWriteFile();
    free(buffer);
    return 0;
}

// Target418D00. Constructor-shaped source does not resolve the ledger's
// authored-versus-generated origin. The original declaration remains unknown.
HintEntryView::HintEntryView()
{
    memset(this, 0, sizeof(*this));
    link.next = 0;
    link.previous = 0;
    link.payload = this;
    color = 0xffffffffu;
    time = 300;
    scale = 1.0f;
    remaining = -1;
}

// Target41AB10. Preserve the final next-pointer recheck and both link updates.
void HintLinkView::Append(HintLinkView *node)
{
    HintLinkView *head = this;
    while (head->next != 0)
        head = head->next;
    if (head->next != 0)
    {
        node->next = head->next;
        head->next->previous = node;
    }
    head->next = node;
    node->previous = head;
}

// Target419040. Entries have trivial destruction; the target call4524A1 is a
// free-tail runtime wrapper, whose original delete/free symbol is unresolved.
// Natural deletion is a lifetime candidate, not a new runtime identity claim.
void ClearHintLists(HintOwnerView *owner)
{
    for (int stage = 0; stage < 8; ++stage)
    {
        HintLinkView *node = owner->loadedStageLists[stage].next;
        while (node != 0)
        {
            HintLinkView *next = node->next;
            delete static_cast<HintEntryView *>(node->payload);
            node = next;
        }
        node = owner->exportStageLists[stage].next;
        while (node != 0)
        {
            HintLinkView *next = node->next;
            delete static_cast<HintEntryView *>(node->payload);
            node = next;
        }
    }
}

// Target418D40. Sentinel payloads carry the owner, never a fake entry.
HintOwnerView::HintOwnerView()
{
    memset(this, 0, sizeof(*this));
    flags |= 2;
    for (int stage = 0; stage < 8; ++stage)
    {
        loadedStageLists[stage].payload = this;
        exportStageLists[stage].payload = this;
    }
    g_GameOwner477814 = this;
}

// Target418E30. Register the actual callbacks, preserving bit-2 clearing.
int HintOwnerView::Initialize()
{
    th10::ChainElem *element = th10::Chain::AllocElem(OnUpdate);
    element->arg = this;
    element->flags &= ~2u;
    th10::g_Chain->AddToCalcChain(element, 25);
    updateChain = element;
    element = th10::Chain::AllocElem(OnDraw);
    element->arg = this;
    element->flags &= ~2u;
    th10::g_Chain->AddToDrawChain(element, 44);
    drawChain = element;
    counter10 = 0;
    if (g_MainSupervisorView.configuration.hintMode != 0)
    {
        LoadHintFile(this, "hint/hint_auto.txt", 0);
        LoadHintFile(this, "hint/hint_user.txt", 1);
    }
    return 0;
}

// Target418EE0. Chain pointers and freed sentinels are intentionally not reset.
void __stdcall CleanupHintOwner(HintOwnerView *owner)
{
    th10::ChainElem *element = owner->updateChain;
    th10::Chain *chain = th10::g_Chain;
    if (element != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(element);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    element = owner->drawChain;
    chain = th10::g_Chain;
    if (element != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(element);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    for (int index = 0; index < 10; ++index)
    {
        if (owner->vmIds[index].value != 0)
            manager->MarkVmForDeletion(owner->vmIds[index]);
        owner->vmIds[index].value = 0;
    }
    if (g_MainSupervisorView.configuration.hintMode == 2)
        ExportAutomaticHints(owner, "hint/hint_auto.txt");
    ClearHintLists(owner);
    g_GameOwner477814 = NULL;
}

// Target419090. No extra null-allocation recovery is invented.
// Allocation-failure ABI/exception behavior remains outside source-test scope.
HintOwnerView *CreateHintOwner()
{
    HintOwnerView *owner = new HintOwnerView;
    if (owner->Initialize() != 0)
    {
        if (owner != NULL)
        {
            CleanupHintOwner(owner);
            delete owner;
        }
        return NULL;
    }
    return owner;
}

// Target4198A0 and4198B0: genuine ChainCallback address escapes.
int __fastcall HintOwnerView::OnUpdate(void *owner)
{
    return static_cast<HintOwnerView *>(owner)->Update();
}

int __fastcall HintOwnerView::OnDraw(void *)
{
    return 1;
}

// Target419120. Natural member boundary; no forced private EAX/stack ABI.
#include "AnmManager.hpp"
#include "GameEffectUpdate.hpp"
#include "Player.hpp"
#include <math.h>

// Canonical shared storage. In this owner these are stage index and base match.
extern int g_GameStartupMode;          // 0x474C7C
extern int g_GameFrameRestorePending;  // 0x474C84

extern "C" void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)

// Observe the selected branch's resource read; original qualifiers are unknown.
static inline AnmLoadedView *ReadHintMessageResource()
{
    GameEffectUpdateView *effect = g_GameEffectUpdate;
    return *static_cast<AnmLoadedView *volatile *>(&effect->loadedResource);
}

int HintOwnerView::ConsumeEntries(HintLinkView *first, int freeEntries)
{
    if (first != 0)
    {
        AnmRenderManagerView *manager = g_AnmRenderManagerView;
        do
        {
            HintLinkView *next = first->next;
            HintEntryView *entry = static_cast<HintEntryView *>(first->payload);
            if (entry->base == 0 || entry->base == g_GameFrameRestorePending)
                --entry->count;
            if (entry->count <= 0)
            {
                AnmVmIdView *oldId = &vmIds[nextVmSlot];
                const unsigned int oldValue = oldId->value;
                if (oldValue != 0)
                {
                    AnmVmLayerNodeView *oldNode = manager->primaryVmListHead;
                    while (oldNode != NULL)
                    {
                        if (static_cast<AnmVmView *>(oldNode->owner)->id == oldValue)
                            goto MarkOldHintVm;
                        oldNode = oldNode->next;
                    }
                    oldNode = manager->secondaryVmListHead;
                    while (oldNode != NULL)
                    {
                        if (static_cast<AnmVmView *>(oldNode->owner)->id == oldValue)
                            goto MarkOldHintVm;
                        oldNode = oldNode->next;
                    }
                    goto ClearOldHintId;
                MarkOldHintVm:
                    AnmVmView *oldVm = static_cast<AnmVmView *>(oldNode->owner);
                    if (oldVm != NULL)
                    {
                        oldVm->flags35C |= 0x04000000u;
                        if (oldVm->layerNode.previous == NULL)
                        {
                            AnmVmLayerNodeView *child = oldVm->layerNode.next;
                            while (child != NULL)
                            {
                                static_cast<AnmVmView *>(child->owner)->flags35C |= 0x04000000u;
                                child = child->next;
                            }
                        }
                    }
                }
            ClearOldHintId:
                oldId->value = 0;
                entry->position.y = entry->scale * 8.0f + entry->position.y;
                // Ordered comparisons select the branch before creation; NaN takes else.
                // Native continues into VM writes even after failed resolution.
                AnmVmView *vm;
                const int scriptBase = nextVmSlot;
                if (entry->scale <= 1.0f)
                {
                    vmIds[nextVmSlot] = ReadHintMessageResource()->CreateVmAtWorldVariant0(
                        scriptBase + 2, &entry->position);
                    manager = g_AnmRenderManagerView;
                    AnmVmIdView *id = &vmIds[nextVmSlot];
                    AnmVmView *createdVm = manager->FindVm(*id);
                    if (createdVm == 0)
                        id->value = 0;
                    createdVm->glyphWidth = createdVm->glyphHeight = 15;
                    AnmFloat2View initial, final;
                    initial.x = entry->scale * 1.5f;
                    initial.y = 0.0f;
                    final.x = entry->scale;
                    final.y = entry->scale;
                    vm = createdVm;
                    createdVm->StartScaleInterpolation(8, 4, &initial, &final);
                }
                else
                {
                    const int script = scriptBase + 12;
                    if (entry->scale <= 2.0f)
                    {
                        vmIds[nextVmSlot] = ReadHintMessageResource()->CreateVmAtWorldVariant0(
                            script, &entry->position);
                        manager = g_AnmRenderManagerView;
                        AnmVmIdView *id = &vmIds[nextVmSlot];
                        AnmVmView *createdVm = manager->FindVm(*id);
                        if (createdVm == 0)
                            id->value = 0;
                        unsigned char glyphSize = static_cast<unsigned char>(
                            static_cast<int>(entry->scale * 15.0f));
                        createdVm->glyphWidth = createdVm->glyphHeight = glyphSize;
                        AnmFloat2View initial, final;
                        initial.x = entry->scale * 0.75f;
                        initial.y = 0.0f;
                        final.y = final.x = entry->scale * 0.5f;
                        vm = createdVm;
                        createdVm->StartScaleInterpolation(8, 4, &initial, &final);
                        manager = g_AnmRenderManagerView;
                    }
                    else
                    {
                        _ReadWriteBarrier();
                        vmIds[nextVmSlot] = ReadHintMessageResource()->CreateVmAtWorldVariant0(
                            script, &entry->position);
                        manager = g_AnmRenderManagerView;
                        AnmVmIdView *id = &vmIds[nextVmSlot];
                        AnmVmView *createdVm = manager->FindVm(*id);
                        if (createdVm == 0)
                            id->value = 0;
                        createdVm->glyphWidth = createdVm->glyphHeight = 30;
                        AnmFloat2View initial, final;
                        initial.x = entry->scale * 0.75f;
                        initial.y = 0.0f;
                        final.y = final.x = entry->scale * 0.5f;
                        vm = createdVm;
                        createdVm->StartScaleInterpolation(8, 4, &initial, &final);
                    }
                }
                switch (entry->alignment)
                {
                case 2:
                    *reinterpret_cast<AnmFloat3View *>(cachedPositions[nextVmSlot]) = vm->positionOffset;
                    cachedPositions[nextVmSlot][0] -= entry->textWidth * 0.5f;
                    cachedWidths[nextVmSlot] = entry->textWidth;
                    manager->DrawTextRight(vm, 0x00FFFFFF, entry->text);
                    vm->flags35C = (vm->flags35C & ~0x00040000) | 0x00080000;
                    manager = g_AnmRenderManagerView;
                    break;
                case 1:
                    *reinterpret_cast<AnmFloat3View *>(cachedPositions[nextVmSlot]) = vm->positionOffset;
                    cachedPositions[nextVmSlot][0] += entry->textWidth * 0.5f;
                    cachedWidths[nextVmSlot] = entry->textWidth;
                    manager->DrawTextLeft(vm, 0x00FFFFFF, entry->text);
                    vm->flags35C = (vm->flags35C & ~0x00080000) | 0x00040000;
                    manager = g_AnmRenderManagerView;
                    break;
                case 0:
                    *reinterpret_cast<AnmFloat3View *>(cachedPositions[nextVmSlot]) = vm->positionOffset;
                    cachedWidths[nextVmSlot] = entry->textWidth;
                    manager->DrawTextCentered(vm, 0x00FFFFFF, entry->text);
                    manager = g_AnmRenderManagerView;
                    break;
                }
                vm->primaryColor.value = entry->color;
                vm->primaryColor.alpha = 0;
                vm->intVar0 = entry->time;
                vm->intVar1 = entry->alpha;
                nextVmSlot = (nextVmSlot + 1) % 10;
                if (entry->link.next != 0)
                    entry->link.next->previous = entry->link.previous;
                if (entry->link.previous != 0)
                    entry->link.previous->next = entry->link.next;
                entry->link.next = 0;
                entry->link.previous = 0;
                if (freeEntries != 0)
                {
                    delete entry;
                    manager = g_AnmRenderManagerView;
                }
            }
            first = next;
        } while (first != 0);
    }
    return 0;
}

// Complete failed-ID clearing before the per-frame interpolation gate.
static inline AnmVmView *ResolveHintFrameVm(AnmVmIdView *id)
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(*id);
    if (vm == 0)
        id->value = 0;
    return vm;
}

// Target419650. Player coordinates and horizontal distance stay in x87;
// only the observed dy/width/outer-width values are materialized as floats.
int HintOwnerView::Update()
{
    if (currentBase != g_GameFrameRestorePending)
        counter14 = 0;
    ConsumeEntries(loadedStageLists[g_GameStartupMode].next, 1);
    const float playerX = g_Player->drawPosition.x + 224.0f;
    const float playerY = g_Player->drawPosition.y + 16.0f;
    for (int slot = 0; slot < 10; ++slot)
    {
        AnmVmView *vm = ResolveHintFrameVm(&vmIds[slot]);
        if (vm == 0 || vm->primaryAlphaInterpolation.duration != 0)
            continue;
        const float dx = static_cast<float>(fabs(playerX - cachedPositions[slot][0]));
        const float dy = static_cast<float>(fabs(playerY - cachedPositions[slot][1]));
        int alpha;
        int q;
        if (dx < cachedWidths[slot] * 0.5f && vm->spriteHeight * vm->scaleY * 0.5f > dy)
            vm->primaryColor.alpha = static_cast<unsigned char>(vm->intVar1) >> 2;
        else
        {
            const float outerW = cachedWidths[slot] * 0.5f + 32.0f;
            if (dx < outerW && vm->spriteHeight * vm->scaleY * 0.5f > dy)
            {
                alpha = vm->intVar1;
                q = (3 * alpha) / 4;
                vm->primaryColor.alpha = static_cast<unsigned char>(static_cast<int>(
                    alpha - (outerW - dx) * q * 0.03125f));
            }
            else if (dx < cachedWidths[slot] * 0.5f && dy < vm->spriteHeight * vm->scaleY * 0.5f + 32.0f)
            {
                alpha = vm->intVar1;
                q = (3 * alpha) / 4;
                vm->primaryColor.alpha = static_cast<unsigned char>(static_cast<int>(
                    alpha - ((vm->spriteHeight * vm->scaleY * 0.5f + 32.0f) - dy) * q * 0.03125f));
            }
            else
                vm->primaryColor.alpha = static_cast<unsigned char>(vm->intVar1);
        }
    }
    ++counter10;
    ++counter14;
    return 1;
}

// Shared game counters already used by GameManager; these are not new storage.
extern int g_GameFrameCounter0; // target474C88
extern int g_GameFrameCounter1; // target474C8C

// Target41A120. Append before copying the supplied position and text, preserving
// the target's allocation/publication order and 64-byte bounded copy.
HintEntryView *HintOwnerView::AddAutomaticEntry(
    const char *text, const PlayerFloat3 *position)
{
    HintEntryView *entry = new HintEntryView;
    exportStageLists[g_GameStartupMode].Append(&entry->link);
    entry->remaining = 30;
    memcpy(&entry->position, position, sizeof(entry->position));
    strncpy(entry->text, text, 64);
    entry->alignment = 0;
    entry->base = g_GameFrameRestorePending;
    entry->count = g_GameFrameRestorePending != 0
        ? g_GameFrameCounter1 : g_GameFrameCounter0;
    return entry;
}

// Target424650. Existing positive remaining counts are decremented once before
// the new caution entry is published. The position is a real second argument.
void HintOwnerView::AddCaution(
    const char *text, const PlayerFloat3 *position)
{
    for (HintLinkView *node = exportStageLists[g_GameStartupMode].next; node != 0; )
    {
        HintLinkView *next = node->next;
        HintEntryView *entry = static_cast<HintEntryView *>(node->payload);
        if (entry->remaining > 0)
            --entry->remaining;
        node = next;
    }
    HintEntryView *entry = AddAutomaticEntry(text, position);
    entry->count -= 60;
    if (entry->count <= 0)
        entry->count = 1;
    entry->time = 90;
    entry->color = 0xffff8080u;
    entry->remaining = 30;
}
