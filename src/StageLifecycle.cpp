#include "Main.hpp"
#include "StageScript.hpp"
#include "Chain.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include <stdlib.h>
#include <string.h>

// Target storage aliases, not storage definitions or whole-product closure.
extern StageScriptView *g_StagePrimary;   // 0x4776E8, GameAnimationSlots view elsewhere
extern StageScriptView *g_StageSecondary; // 0x4776E4, GameMenu view elsewhere
extern int g_GameStartupMode;            // 0x474C7C
extern unsigned int g_GameRuntimeFlags;  // 0x474CA0
extern char g_StageFilePath[];            // 0x497C38, capacity unproved
extern StageCameraStateView g_StagePublishedCamera;

// Target0x402160/188: member constructors precede the whole-object clear.
// The eleven embedded VMs remain real members with reverse destruction order.
StageScriptView::StageScriptView()
{
    memset(this, 0, sizeof(*this));
    lifecycleFlags |= 2;
}

// Target0x403850/308: original cached file is kept separate from the mutable
// copy whose object offsets become pointers. Target omits allocation/count/
// path bounds checks; this is fidelity to the original, not a safety guarantee.
int StageScriptView::LoadData(const char *path)
{
    if (originalData == NULL)
    {
        g_StageFilePath[0] = '\0';
        strcat(g_StageFilePath, path);
        originalData = FileSystem::OpenFile(g_StageFilePath, &originalDataSize, 0);
        if (originalData == NULL)
            return -1;
    }
    const size_t copySize = originalDataSize;
    data = static_cast<StageDataHeaderView *>(malloc(copySize));
    memcpy(data, originalData, originalDataSize);
    anm = g_AnmRenderManagerView->PreloadAnm((unknown2A30 & 1) + 4, data->anmPath);
    if (anm == NULL)
    {
        th10::g_GameErrorContext.Log(
            "\x83\x58\x83\x65\x81\x5b\x83\x57\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
        return -1;
    }
    objects = reinterpret_cast<StageObjectView **>(data + 1);
    instances = reinterpret_cast<StageInstanceView *>(reinterpret_cast<unsigned char *>(data) + data->instancesOffset);
    scriptBase = reinterpret_cast<unsigned char *>(data) + data->scriptOffset;
    for (int i = 0; i < data->objectCount; ++i)
        objects[i] = reinterpret_cast<StageObjectView *>(reinterpret_cast<unsigned char *>(data) + reinterpret_cast<size_t>(objects[i]));
    const size_t vmBytes = data->objectVmCount * sizeof(AnmVmView);
    objectVms = static_cast<AnmVmView *>(malloc(vmBytes));
    return 0;
}

// Target0x402230/522: EAX receiver, ECX path, stack priority offset/RET4.
int StageScriptView::Initialize(const char *path, int priorityOffset)
{
    if (priorityOffset == 0)
        g_StagePrimary = this;
    else
        g_StageSecondary = this;
    unknown2A30 = g_GameStartupMode;
    if (LoadData(path) != 0)
    {
        th10::g_GameErrorContext.Fatal(
            "\x83\x58\x83\x65\x81\x5b\x83\x57\x83\x66\x81\x5b\x83\x5e\x82\xaa\x93\xc7\x82\xdd\x8d\x9e\x82\xdf\x82\xdc\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
        return -1;
    }
    camera = g_StagePublishedCamera;
    camera.position = AnmFloat3View(0, 0, -600);
    camera.direction = AnmFloat3View(0, 300, 600);
    camera.up = AnmFloat3View(0, 1, 0);
    camera.rockingOffset = AnmFloat3View(0, 0, 0);
    cullDistanceSquared = 9610000.0f;
    th10::ChainElem *element = th10::Chain::AllocElem(reinterpret_cast<th10::ChainCallback>(StageUpdateCallback));
    element->arg = this;
    element->flags &= ~2u;
    th10::g_Chain->AddToCalcChain(element, priorityOffset + 12);
    updateChain = element;
    element = th10::Chain::AllocElem(reinterpret_cast<th10::ChainCallback>(StageDrawEarlyCallback));
    element->flags &= ~2u;
    element->arg = this;
    th10::g_Chain->AddToDrawChain(element, priorityOffset + 7);
    earlyDrawChain = element;
    element = th10::Chain::AllocElem(reinterpret_cast<th10::ChainCallback>(StageDrawLateCallback));
    element->arg = this;
    element->flags &= ~2u;
    th10::g_Chain->AddToDrawChain(element, priorityOffset + 10);
    lateDrawChain = element;
    frame = 0;
    scriptTimer.SetCurrent(0);
    flags |= 1;
    directionInterpolation.duration = 0;
    positionInterpolation.duration = 0;
    return 0;
}

static void StageRemoveChain(th10::ChainElem *element)
{
    th10::Chain *chain = th10::g_Chain;
    if (element != NULL)
    {
        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        ++g_MainSupervisorView.criticalSectionLockCounts[0];
        chain->UnregisterElem(element);
        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);
        --g_MainSupervisorView.criticalSectionLockCounts[0];
    }
}
// Target0x402440/499. Dynamically allocated objectVms are raw storage here;
// only the eleven embedded VMs have automatic destructors in this owner.
StageScriptView::~StageScriptView()
{
    StageRemoveChain(updateChain);
    StageRemoveChain(earlyDrawChain);
    StageRemoveChain(lateDrawChain);
    if (data != NULL) { free(data); data = NULL; }
    if (originalData != NULL) { free(originalData); originalData = NULL; }
    originalData = NULL;
    if (objectVms != NULL) { free(objectVms); objectVms = NULL; }
    if ((g_GameRuntimeFlags & 1) == 0)
        g_AnmRenderManagerView->ReleaseAnm((unknown2A30 & 1) + 4);
    if (g_StagePrimary == this) g_StagePrimary = NULL;
    if (g_StageSecondary == this) g_StageSecondary = NULL;
}

// Target0x402640/151. Target calls Initialize even if operator new returned
// null; no synthetic safety repair is introduced. Factory has RET8.
StageScriptView *__stdcall StageCreate(const char *path, int priorityOffset)
{
    StageScriptView *stage = new StageScriptView;
    if (stage->Initialize(path, priorityOffset) != 0)
    {
        delete stage;
        return NULL;
    }
    return stage;
}
int __fastcall StageUpdateCallback(StageScriptView *stage)
{
    return stage->Update();
}
