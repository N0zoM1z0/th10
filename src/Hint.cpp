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
    remaining = -1;
    time = 300;
    scale = 1.0f;
}

// Target41A0A0. Shift the terminating NUL along with each leading byte.
static char *HintTrimWhitespace(char *text)
{
    while (*text == ' ' || *text == '\t' || *text == '\n' || *text == '\r')
    {
        int length = static_cast<int>(strlen(text));
        for (int i = 0; i < length; ++i)
            text[i] = text[i + 1];
    }
    int last = static_cast<int>(strlen(text)) - 1;
    while (last >= 0 && (text[last] == ' ' || text[last] == '\t' ||
                         text[last] == '\n' || text[last] == '\r'))
        text[last--] = 0;
    return text;
}

// Target419F40. The unbounded input scans and uncapped final copy are native
// quirks. No sentinel, delimiter normalization, or final-tail trim is invented.
static char *__stdcall HintReadLine(char *cursor, char *line, int *remaining,
                          unsigned int capacity)
{
    memset(line, 0, capacity);
    while (*line == 0)
    {
        char *end = strchr(cursor, '\n');
        if (end == 0)
            end = strchr(cursor, '\r');
        if (end == 0)
        {
            memcpy(line, cursor, *remaining);
            *remaining = 0;
            break;
        }
        int copySize = static_cast<int>(end - cursor);
        if (copySize >= static_cast<int>(capacity - 1))
            copySize = capacity - 1;
        memcpy(line, cursor, copySize);
        *remaining -= static_cast<int>(end - cursor);
        cursor = end;
        while (*cursor == '\n' || *cursor == '\r')
        {
            ++cursor;
            --*remaining;
        }
        char *comment = strchr(line, '#');
        if (comment != 0)
        {
            memset(comment, 0, capacity - (comment - line));
            *comment = 0;
        }
        HintTrimWhitespace(line);
    }
    return cursor;
}

// Target41A050. A colonless directive does not reset the value buffer.
static void HintSplitLine(const char *line, char *key, char *value)
{
    strcpy(key, line);
    char *colon = strchr(key, ':');
    if (colon == 0)
    {
        HintTrimWhitespace(key);
        return;
    }
    strcpy(value, colon + 1);
    *colon = 0;
    HintTrimWhitespace(key);
    HintTrimWhitespace(value);
}

// Target4198C0. An unmatched token selects value zero.
static int __stdcall HintLookupValue(const char *name, const HintNameValue *table, int count)
{
    for (int i = 0; i < count; ++i)
        if (strcmp(name, table[i].name) == 0)
            return table[i].value;
    return 0;
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

int __stdcall LoadHintFile(HintOwnerView *owner, const char *path, int mode)
{
    int stage = -1;
    int entries = 0;
    int remaining;
    char *file = reinterpret_cast<char *>(FileSystem::OpenFile(path, &remaining, 1));
    if (file == 0)
        return -1;

    char *line = static_cast<char *>(malloc(4096));
    char *key = static_cast<char *>(malloc(4096));
    char *value = static_cast<char *>(malloc(4096));
    char *cursor = file;
    while (remaining > 0)
    {
        cursor = HintReadLine(cursor, line, &remaining, 4096);
        // Both passes are present in the native outer loop.
        HintSplitLine(line, key, value);
        HintSplitLine(line, key, value);
        if (strcmp(key, "Version") == 0)
        {
            if (strcmp(value, "0.0") != 0)
            {
                ClearHintLists(owner);
                break;
            }
        }
        else if (strcmp(key, "Stage") == 0)
        {
            stage = atol(value);
            entries = 0;
            if (stage < 1 || stage > 7)
                stage = -1;
        }
        else if (strcmp(key, "StageEnd") == 0)
            stage = -1;
        else if (strcmp(key, "Tips") == 0 && stage > 0)
        {
            if (entries >= 255)
            {
                stage = -1;
                continue;
            }
            HintEntryView *entry = new HintEntryView;
            ++entries;
            memset(entry->unknown18, 0, sizeof(entry->unknown18));
            while (remaining > 0)
            {
                cursor = HintReadLine(cursor, line, &remaining, 4096);
                HintSplitLine(line, key, value);
                if (strcmp(key, "Pos") == 0)
                {
                    char *comma = strchr(value, ',');
                    if (comma != 0)
                    {
                        *comma = 0;
                        ++comma;
                        HintTrimWhitespace(value);
                        HintTrimWhitespace(comma);
                        entry->position.x = static_cast<float>(atol(value));
                        entry->position.y = static_cast<float>(atol(comma));
                    }
                }
                else if (strcmp(key, "Text") == 0)
                {
                    char *first = strchr(value, '"');
                    if (first != 0)
                    {
                        ++first;
                        char *last = strrchr(first, '"');
                        if (last == 0)
                            break;
                        *last = 0;
                        strncpy(entry->text, first, 65);
                    }
                }
                else if (strcmp(key, "Count") == 0)
                    entry->count = atol(value);
                else if (strcmp(key, "Time") == 0)
                    entry->time = atol(value);
                else if (strcmp(key, "Base") == 0)
                    entry->base = HintLookupValue(value, baseNames, 61);
                else if (strcmp(key, "Align") == 0)
                    entry->alignment = HintLookupValue(value, alignmentNames, 3);
                else if (strcmp(key, "Remain") == 0)
                    entry->remaining = atol(value);
                else if (strcmp(key, "Scale") == 0)
                {
                    double parsedScale = atof(value);
                    entry->scale = static_cast<float>(parsedScale);
                    if (parsedScale > 4.0)
                        entry->scale = 4.0f;
                    else if (parsedScale < -4.0)
                        entry->scale = -4.0f;
                }
                else if (strcmp(key, "Color") == 0)
                {
                    char *first = strchr(value, ',');
                    if (first != 0)
                    {
                        *first = 0;
                        ++first;
                        HintTrimWhitespace(value);
                        entry->red = static_cast<unsigned char>(atol(value));
                        char *second = strchr(first, ',');
                        if (second != 0)
                        {
                            *second = 0;
                            HintTrimWhitespace(first);
                            entry->green = static_cast<unsigned char>(atol(first));
                            entry->blue = static_cast<unsigned char>(atol(HintTrimWhitespace(second + 1)));
                        }
                    }
                }
                else if (strcmp(key, "Alpha") == 0)
                    entry->alpha = static_cast<unsigned char>(atol(value));
                else if (strcmp(key, "End") == 0)
                    break;
            }
            // VC7.1 keeps this x87 expression live until the final float store.
            entry->textWidth = static_cast<float>(strlen(entry->text)) * entry->scale * 7.5f;
            owner->loadedStageLists[stage].Append(&entry->link);
            if (mode == 0)
            {
                HintEntryView *copy = new HintEntryView;
                memcpy(copy, entry, sizeof(*copy));
                copy->link.payload = copy;
                copy->link.next = 0;
                copy->link.previous = 0;
                owner->exportStageLists[stage].Append(&copy->link);
            }
        }
    }
    free(line);
    free(key);
    free(value);
    free(file);
    return 0;
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
    th10::Chain::AddToCalcChain(element, 25, th10::g_Chain);
    updateChain = element;
    element = th10::Chain::AllocElem(OnDraw);
    element->arg = this;
    element->flags &= ~2u;
    th10::Chain::AddToDrawChain(element, 44, th10::g_Chain);
    drawChain = element;
    counter10 = 0;
    if (g_MainSupervisorView.hintMode != 0)
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
        th10::Chain::UnregisterElem(element, chain);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
    element = owner->drawChain;
    chain = th10::g_Chain;
    if (element != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        th10::Chain::UnregisterElem(element, chain);
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
    if (g_MainSupervisorView.hintMode == 2)
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

int HintOwnerView::ConsumeEntries(HintLinkView *first, int freeEntries)
{
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    for (HintLinkView *node = first; node != 0; )
    {
        HintLinkView *next = node->next;
        HintEntryView *entry = static_cast<HintEntryView *>(node->payload);
        if (entry->base == 0 || entry->base == g_GameFrameRestorePending)
            --entry->count;
        if (entry->count > 0)
        {
            node = next;
            continue;
        }
        int slot = nextVmSlot;
        if (vmIds[slot].value != 0)
        {
            AnmVmView *oldVm = manager->FindVm(vmIds[slot]);
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
        vmIds[slot].value = 0;
        entry->position.y = static_cast<float>(
            static_cast<double>(entry->scale) * 8.0 + entry->position.y);
        // Ordered comparisons select the branch before creation; NaN takes else.
        // Native continues into VM writes even after failed resolution.
        AnmVmView *vm;
        if (entry->scale <= 1.0f)
        {
            AnmVmIdView created = g_GameEffectUpdate->loadedResource->CreateVmAtWorldVariant0(
                nextVmSlot + 2, &entry->position);
            slot = nextVmSlot;
            vmIds[slot] = created;
            manager = g_AnmRenderManagerView;
            vm = manager->FindVm(vmIds[slot]);
            if (vm == 0)
                vmIds[slot].value = 0;
            vm->glyphWidth = vm->glyphHeight = 15;
            AnmFloat2View initial, final;
            initial.x = static_cast<float>(static_cast<double>(entry->scale) * 1.5);
            initial.y = 0.0f;
            final.x = final.y = entry->scale;
            vm->StartScaleInterpolation(8, 4, &initial, &final);
        }
        else if (entry->scale <= 2.0f)
        {
            AnmVmIdView created = g_GameEffectUpdate->loadedResource->CreateVmAtWorldVariant0(
                nextVmSlot + 12, &entry->position);
            slot = nextVmSlot;
            vmIds[slot] = created;
            manager = g_AnmRenderManagerView;
            vm = manager->FindVm(vmIds[slot]);
            if (vm == 0)
                vmIds[slot].value = 0;
            unsigned char glyphSize = static_cast<unsigned char>(
                static_cast<int>(static_cast<double>(entry->scale) * 15.0));
            vm->glyphWidth = vm->glyphHeight = glyphSize;
            AnmFloat2View initial, final;
            initial.x = static_cast<float>(static_cast<double>(entry->scale) * 0.75);
            initial.y = 0.0f;
            final.x = final.y = static_cast<float>(static_cast<double>(entry->scale) * 0.5);
            vm->StartScaleInterpolation(8, 4, &initial, &final);
            manager = g_AnmRenderManagerView;
        }
        else
        {
            AnmVmIdView created = g_GameEffectUpdate->loadedResource->CreateVmAtWorldVariant0(
                nextVmSlot + 12, &entry->position);
            slot = nextVmSlot;
            vmIds[slot] = created;
            manager = g_AnmRenderManagerView;
            vm = manager->FindVm(vmIds[slot]);
            if (vm == 0)
                vmIds[slot].value = 0;
            vm->glyphWidth = vm->glyphHeight = 30;
            AnmFloat2View initial, final;
            initial.x = static_cast<float>(static_cast<double>(entry->scale) * 0.75);
            initial.y = 0.0f;
            final.x = final.y = static_cast<float>(static_cast<double>(entry->scale) * 0.5);
            vm->StartScaleInterpolation(8, 4, &initial, &final);
        }
        slot = nextVmSlot;
        if (entry->alignment >= 0 && entry->alignment <= 2)
        {
            cachedPositions[slot][0] = vm->positionOffset.x;
            cachedPositions[slot][1] = vm->positionOffset.y;
            cachedPositions[slot][2] = vm->positionOffset.z;
            cachedWidths[slot] = entry->textWidth;
            if (entry->alignment == 2)
            {
                cachedPositions[slot][0] = static_cast<float>(
                    static_cast<double>(cachedPositions[slot][0]) - entry->textWidth * 0.5);
                manager->DrawTextRight(vm, 0x00FFFFFF, entry->text);
                vm->flags35C = (vm->flags35C & ~0x00040000) | 0x00080000;
            }
            else if (entry->alignment == 1)
            {
                cachedPositions[slot][0] = static_cast<float>(
                    static_cast<double>(cachedPositions[slot][0]) + entry->textWidth * 0.5);
                manager->DrawTextLeft(vm, 0x00FFFFFF, entry->text);
                vm->flags35C = (vm->flags35C & ~0x00080000) | 0x00040000;
            }
            else
                manager->DrawTextCentered(vm, 0x00FFFFFF, entry->text);
            manager = g_AnmRenderManagerView;
        }
        vm->primaryColor.blue = entry->blue;
        vm->primaryColor.green = entry->green;
        vm->primaryColor.red = entry->red;
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
        node = next;
    }
    return 0;
}

// Target419650. Widened expressions represent retained x87 temporaries; the
// native control word is unknown. This is not a universal host/native FP claim.
int HintOwnerView::Update()
{
    if (currentBase != g_GameFrameRestorePending)
        counter14 = 0;
    ConsumeEntries(loadedStageLists[g_GameStartupMode].next, 1);
    const double playerX = static_cast<double>(g_Player->drawPosition.x) + 224.0;
    const double playerY = static_cast<double>(g_Player->drawPosition.y) + 16.0;
    for (int slot = 0; slot < 10; ++slot)
    {
        if (vmIds[slot].value == 0)
            continue;
        AnmVmView *vm = g_AnmRenderManagerView->FindVm(vmIds[slot]);
        if (vm == 0)
        {
            vmIds[slot].value = 0;
            continue;
        }
        if (vm->primaryAlphaInterpolation.duration != 0)
            continue;
        const double dx = fabs(playerX - cachedPositions[slot][0]);
        const float dy = static_cast<float>(fabs(playerY - cachedPositions[slot][1]));
        const double h = static_cast<double>(vm->spriteHeight) * vm->scaleY * 0.5;
        const float w = static_cast<float>(static_cast<double>(cachedWidths[slot]) * 0.5);
        const float outerW = static_cast<float>(static_cast<double>(w) + 32.0);
        const int alpha = vm->intVar1;
        if (dx < w && dy < h)
            vm->primaryColor.alpha = static_cast<unsigned char>(alpha) >> 2;
        else if (dx < outerW && dy < h)
        {
            const int q = (3 * alpha) / 4;
            vm->primaryColor.alpha = static_cast<unsigned char>(static_cast<int>(
                alpha - (outerW - dx) * q * 0.03125));
        }
        else if (dx < w && dy < h + 32.0)
        {
            const int q = (3 * alpha) / 4;
            vm->primaryColor.alpha = static_cast<unsigned char>(static_cast<int>(
                alpha - ((h + 32.0) - dy) * q * 0.03125));
        }
        else
            vm->primaryColor.alpha = static_cast<unsigned char>(alpha);
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
    entry->position.x = position->x;
    entry->position.y = position->y;
    entry->position.z = position->z;
    strncpy(entry->text, text, 64);
    entry->alignment = 0;
    entry->base = g_GameFrameRestorePending;
    entry->count = g_GameFrameRestorePending == 0
        ? g_GameFrameCounter0 : g_GameFrameCounter1;
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
