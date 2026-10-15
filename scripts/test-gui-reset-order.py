#!/usr/bin/env python3
"""Exercise the maintained full GUI reset body against synthetic event sinks.

No game IO, target execution, allocation implementation, or codegen claim.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


def function_source(path, signature):
    source = (ROOT / path).read_text()
    start = source.index(signature)
    brace = source.index('{', start)
    depth, end = 1, brace + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


def interrupt_blocks():
    source = (ROOT / 'src/Gui.cpp').read_text()
    blocks = []
    for region in (1, 2):
        start = source.index('// Reset interrupt region ' + str(region))
        brace = source.index('{', start)
        depth, end = 1, brace + 1
        while depth:
            depth += (source[end] == '{') - (source[end] == '}')
            end += 1
        blocks.append(source[brace:end])
    return blocks


def reset_source():
    return function_source('src/AnmManager.cpp', 'AnmVmView *AnmRenderManagerView::FindVm(AnmVmIdView id)') + "\n" + function_source('src/Gui.cpp', 'void GameResetGui(GuiView *gui)') + "\nstatic void ExerciseResetInterrupt(AnmVmIdView id) { GuiView storage{}; GuiView *gui=&storage; gui->specialVmId9E50=id;\n" + interrupt_blocks()[0] + "\n}\n"


class GuiResetOrder(unittest.TestCase):
    def test_both_target_local_walks_agree(self):
        self.assertEqual(*interrupt_blocks())

    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_actual_owner_with_synthetic_dependencies(self):
        program = r'''
#include <cassert>
#include <cstddef>
#include <vector>
#include <utility>
struct Id { unsigned value; };
typedef Id AnmVmIdView;
static std::vector<std::pair<int,int> > events;
static std::vector<unsigned> interrupted;
static unsigned nextId;
static std::vector<std::pair<const void *, const void *> > initializationTargets, spriteTargets;
struct PendingInterrupt {
    unsigned owner; short value;
    PendingInterrupt &operator=(short n) { assert(n==3); value=n; interrupted.push_back(owner); events.push_back({4,int(owner)}); return *this; }
};
struct AnmVmLayerNodeView { void *owner; AnmVmLayerNodeView *next, *previous; };
struct Vm {
    unsigned flags35C, id;
    PendingInterrupt pendingInterrupt;
    AnmVmLayerNodeView layerNode, managerNode;
};
typedef Vm AnmVmView;
struct AnmRenderManagerView {
    AnmVmLayerNodeView *primaryVmListHead, *secondaryVmListHead;
    AnmVmView *FindVm(AnmVmIdView id);
};
static AnmRenderManagerView manager{};
static AnmRenderManagerView *g_AnmRenderManagerView=&manager;
static Vm allocated[2048];
static void RegisterVm(unsigned id) {
    assert(id<2048); Vm &vm=allocated[id]; vm.id=id; vm.flags35C=0x55;
    vm.pendingInterrupt.owner=id; vm.pendingInterrupt.value=0;
    vm.layerNode={&vm,NULL,NULL};
    vm.managerNode={&vm,manager.primaryVmListHead,NULL};
    manager.primaryVmListHead=&vm.managerNode;
}
static void ResetLists(unsigned existing=0) {
    manager.primaryVmListHead=manager.secondaryVmListHead=NULL;
    if(existing) RegisterVm(existing);
}
struct Loaded {
    int tag;
    Id CreateVmVariant0(int script, unsigned layer) {
        assert(layer == 15); events.push_back({tag, script}); RegisterVm(++nextId); return Id{nextId};
    }
    void InitializeAndExecuteScriptIndex(Vm *vm, int script) { initializationTargets.push_back({this,vm}); events.push_back({2, script}); }
    void SetSprite(Vm *vm, int sprite) { spriteTargets.push_back({this,vm}); events.push_back({3, sprite}); }
};
struct GuiResetCallbackView { unsigned unused, flags; };
struct GuiResetSessionView { int value5C; };
struct Ascii { Loaded *asciiAnm; };
struct GuiView {
    void *updateCallback, *drawCallback;
    Vm displayVms0[10], displayVms1[10], displayVms2[9], displayVms3[4];
    Vm displayVms4[2], displayVms5[7], bossPositionVm;
    Id specialVmId9E50, specialVmId9E54, specialVmId9E58, specialVmId9E5C;
    Loaded *frontAnm, *stageUiAnm;
    int bossGaugeSegmentCount;
    void SetLivesDisplayCount(int count) {
        events.push_back({5,count});
        int i=0; for (;i<count;++i) displayVms2[i].flags35C |= 2;
        for (;i<9;++i) displayVms2[i].flags35C &= ~2u;
    }
};
volatile int g_PlayerLivesDisplayCount;
short g_GuiPower;
int g_GameTransitionState, g_GameNewSession, g_GameStartingStage;
int g_GameStartupMode, g_GameStartupValue474C90;
unsigned g_GameRuntimeFlags;
GuiResetSessionView *g_GuiResetSession;
Ascii *g_AsciiManagerView;
''' + reset_source() + r'''
static std::vector<int> values(int kind) {
    std::vector<int> result;
    for (auto e:events) if(e.first==kind) result.push_back(e.second);
    return result;
}
int main() {
    Loaded front{10}, stage{11}, asciiResource{12};
    Ascii ascii{&asciiResource}; g_AsciiManagerView=&ascii;
    GuiResetSessionView session{0}; g_GuiResetSession=&session;
    GuiResetCallbackView update{0,8}, draw{0,16};
    GuiView gui{}; gui.frontAnm=&front; gui.stageUiAnm=&stage;
    gui.updateCallback=&update; gui.drawCallback=&draw;
    gui.bossGaugeSegmentCount=7;
    nextId=100; g_PlayerLivesDisplayCount=2; g_GuiPower=57;
    g_GameStartupMode=1; g_GameStartingStage=2; g_GameNewSession=1;
    GameResetGui(&gui);
    assert(update.flags==10 && draw.flags==18);
    assert(gui.specialVmId9E58.value==101);
    assert(gui.specialVmId9E5C.value==0); // source must discard ID102
    assert(gui.specialVmId9E50.value==106 && gui.specialVmId9E54.value==107);
    assert(interrupted==std::vector<unsigned>({106,106}));
    assert(interrupted.back()!=gui.specialVmId9E54.value);
    assert(values(10)==std::vector<int>({0,1,0x4f,0x68,0x6d}));
    assert(values(11)==std::vector<int>({0,1}));
    std::vector<int> expected;
    for(int i=0;i<10;++i) { expected.push_back(i+10); expected.push_back(i+20); }
    for(int i=0;i<9;++i) expected.push_back(i+30);
    for(int i=0;i<4;++i) expected.push_back(i+47);
    for(int i=0;i<2;++i) expected.push_back(i+80);
    for(int i=0;i<7;++i) expected.push_back(i+51);
    expected.push_back(0); // boss-position VM initialization, after power digits
    assert(values(2)==expected);
    std::vector<std::pair<const void *, const void *> > expectedTargets;
    for(int i=0;i<10;++i) { expectedTargets.push_back({&front,&gui.displayVms0[i]}); expectedTargets.push_back({&front,&gui.displayVms1[i]}); }
    for(int i=0;i<9;++i) expectedTargets.push_back({&front,&gui.displayVms2[i]});
    for(int i=0;i<4;++i) expectedTargets.push_back({&front,&gui.displayVms3[i]});
    for(int i=0;i<2;++i) expectedTargets.push_back({&front,&gui.displayVms4[i]});
    for(int i=0;i<7;++i) expectedTargets.push_back({&front,&gui.displayVms5[i]});
    expectedTargets.push_back({&asciiResource,&gui.bossPositionVm});
    assert(initializationTargets==expectedTargets);
    const std::vector<std::pair<const void *, const void *> > expectedSpriteTargets = {
        {&front,&gui.displayVms3[0]}, {&front,&gui.displayVms3[2]}, {&front,&gui.displayVms3[3]}
    };
    assert(spriteTargets==expectedSpriteTargets);

    assert(values(3)==std::vector<int>({10,16,13}));
    std::vector<std::pair<int,int> > ordered = {{10,0},{10,1}};
    for (std::size_t i=0;i+1<expected.size();++i) ordered.push_back({2,expected[i]});
    ordered.push_back({5,2});
    ordered.push_back({3,10}); ordered.push_back({3,16}); ordered.push_back({3,13});
    ordered.push_back({11,0}); ordered.push_back({11,1}); ordered.push_back({2,0});
    ordered.push_back({10,0x4f}); ordered.push_back({10,0x68}); ordered.push_back({4,106});
    ordered.push_back({10,0x6d}); ordered.push_back({4,106});
    assert(events==ordered);

    assert(gui.bossGaugeSegmentCount==0);
    for(int i=0;i<9;++i) assert((gui.displayVms2[i].flags35C&2)==(i<2?2u:0u));
    // Distinguish the final old-ID interrupt from publication of the new ID.
    events.clear(); interrupted.clear(); initializationTargets.clear(); spriteTargets.clear(); nextId=100; ResetLists(900);
    gui.specialVmId9E58.value=700; gui.specialVmId9E5C.value=800;
    gui.specialVmId9E50.value=900; gui.displayVms0[0].flags35C=1;
    gui.updateCallback=gui.drawCallback=NULL;
    g_GameTransitionState=8; g_GameRuntimeFlags=0x20;
    g_GameStartupMode=0; g_GameNewSession=0; g_PlayerLivesDisplayCount=0;
    g_GuiPower=-1;
    GameResetGui(&gui);
    assert(gui.specialVmId9E58.value==700 && gui.specialVmId9E5C.value==800);
    assert(gui.specialVmId9E50.value==900 && gui.specialVmId9E54.value==102);
    assert(interrupted==std::vector<unsigned>({900}));
    assert(values(10)==std::vector<int>({0x71,0x6d}));
    assert(values(11).empty()); assert(values(2)==std::vector<int>({0}));
    assert(values(3)==std::vector<int>({8,8,3}));
    expectedTargets.clear(); expectedTargets.push_back({&asciiResource,&gui.bossPositionVm});
    assert(initializationTargets==expectedTargets);
    assert(spriteTargets==expectedSpriteTargets);

    // Independent startup suppressors. A null session pointer in mode 0 makes
    // an accidental loss of short-circuit evaluation fail observably.
    g_GameTransitionState=8; g_GameRuntimeFlags=0; g_GameNewSession=0;
    for (int gate=0;gate<4;++gate) {
        events.clear(); interrupted.clear(); initializationTargets.clear(); spriteTargets.clear();
        ResetLists(900);
        g_GameStartupMode = gate==0 ? 0 : 1;
        g_GuiResetSession = gate==0 ? NULL : &session;
        session.value5C = gate==1 ? 1 : 0;
        g_GameStartupValue474C90 = gate==2 ? 1 : 0;
        GameResetGui(&gui);
        const std::vector<int> expectedFront = gate==3 ? std::vector<int>({0x4f,0x6d}) : std::vector<int>({0x6d});
        assert(values(10)==expectedFront); assert(values(11).empty());
        assert(interrupted==std::vector<unsigned>({900}));
        assert(initializationTargets==expectedTargets); assert(spriteTargets==expectedSpriteTargets);
    }
    // All four independent transition-state/runtime-flag combinations.
    g_GameStartupMode=0; g_GuiResetSession=NULL;
    for (int mode=0;mode<2;++mode) for(int flagged=0;flagged<2;++flagged) {
        events.clear(); interrupted.clear(); initializationTargets.clear(); spriteTargets.clear();
        ResetLists(900);
        g_GameTransitionState=mode ? 8 : 0;
        g_GameRuntimeFlags=flagged ? 0x20 : 0;
        GameResetGui(&gui);
        const std::vector<int> expectedStage = !mode && !flagged ? std::vector<int>({0,1}) : std::vector<int>();
        const std::vector<int> expectedFront = flagged ? std::vector<int>({0x71,0x6d}) : std::vector<int>({0x6d});
        assert(values(11)==expectedStage); assert(values(10)==expectedFront);
        assert(interrupted==std::vector<unsigned>({900}));
        assert(initializationTargets==expectedTargets); assert(spriteTargets==expectedSpriteTargets);
    }

    // Execute the actual local walk and actual maintained two-list FindVm.
    events.clear(); interrupted.clear(); ResetLists();
    Vm primary{}, secondary{}, childA{}, childB{};
    primary.id=501; primary.pendingInterrupt.owner=501; primary.flags35C=0x12;
    secondary.id=502; secondary.pendingInterrupt.owner=502; secondary.flags35C=0x34;
    childA.id=503; childA.pendingInterrupt.owner=503; childA.flags35C=0x56;
    childB.id=504; childB.pendingInterrupt.owner=504; childB.flags35C=0x78;
    primary.managerNode={&primary,NULL,NULL}; secondary.managerNode={&secondary,NULL,NULL};
    manager.primaryVmListHead=&primary.managerNode; manager.secondaryVmListHead=&secondary.managerNode;
    childA.layerNode={&childA,&childB.layerNode,&secondary.layerNode};
    childB.layerNode={&childB,NULL,&childA.layerNode};
    secondary.layerNode={&secondary,&childA.layerNode,NULL};
    ExerciseResetInterrupt(Id{0}); ExerciseResetInterrupt(Id{999}); assert(interrupted.empty());
    ExerciseResetInterrupt(Id{502}); assert(interrupted==std::vector<unsigned>({502,503,504}));
    interrupted.clear(); primary.layerNode={&primary,&childA.layerNode,&secondary.layerNode};
    ExerciseResetInterrupt(Id{501}); assert(interrupted==std::vector<unsigned>({501}));
    interrupted.clear(); secondary.id=501; // primary-list lookup wins
    ExerciseResetInterrupt(Id{501}); assert(interrupted==std::vector<unsigned>({501}));
    assert(primary.flags35C==0x12 && secondary.flags35C==0x34 && childA.flags35C==0x56 && childB.flags35C==0x78);
    return 0;
}
'''
        (ROOT / 'build').mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='gui-reset-', dir=ROOT / 'build') as directory:
            source = Path(directory) / 'test.cpp'
            binary = Path(directory) / 'test'
            source.write_text(program)
            subprocess.run([shutil.which('c++'), '-std=c++11', '-O0', str(source), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    unittest.main()
