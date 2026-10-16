#!/usr/bin/env python3
"""Source-extracted HUD contract tests with synthetic dependency seams.

The callbacks deliberately replace owner pointers or scalar values to expose
native reload/lifetime contracts. They do not establish that real game callees
perform those changes. No native runtime or exact-codegen claim is made.
"""
from pathlib import Path
import argparse
import shutil
import subprocess
import tempfile
import unittest

SOURCE = Path(__file__).resolve().parents[1] / 'src/Gui.cpp'


def extract(source, signature):
    start = source.index(signature)
    brace = source.index('{', start)
    depth, end = 1, brace + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


def owner_source():
    source = SOURCE.read_text()
    if '// HUD boss-gauge phase begin:' in source:
        # Keep the existing dependency seam for these unrelated HUD checks.
        # The complete current lexical phase is executed independently by
        # test-gui-boss-gauge-phase.py; no historical phase copy is substituted.
        start = source.index('// HUD boss-gauge phase begin:')
        brace = source.index('{', start)
        depth, end = 1, brace + 1
        while depth:
            depth += (source[end] == '{') - (source[end] == '}')
            end += 1
        source = source[:brace] + 'UpdateBossGauge(gui);' + source[end:]
    return '\n'.join(extract(source, signature) for signature in (
        'static void UpdateSpellAndBossPosition(GuiView *gui)',
        'int __stdcall GuiView::UpdateStageElements(GuiView *gui)'))


PROGRAM = r'''
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>
#define __stdcall
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"line %d: %s\n",__LINE__,#x); return 1; } } while(0)
struct Timer { int ticks=0; void Tick() { ++ticks; } };
struct Position { float x=0,y=0; };
struct Color { unsigned char alpha=0; };
struct Vm { short pendingInterrupt=0; Position positionOffset; Color primaryColor; };
struct GuiMessageVmView { Timer lifetimeTimer; static int Run(GuiMessageVmView *); };
struct Loaded { int tag; void SetSprite(Vm *,int); };
struct GuiView {
    unsigned guiFlags=0; int endingCounter=0;
    Vm displayVms2[9],displayVms3[4],displayVms4[2],displayVms5[7],bossPositionVm;
    Loaded *frontAnm=nullptr; GuiMessageVmView *message=nullptr;
    int spellSeconds=-1,previousSpellSeconds=-1; Timer frameTimer;
    static int UpdateStageElements(GuiView *);
};
struct GuiEnemyView { unsigned flags=0; int gaugeThresholdValue=900; float positionX=0; };
struct GuiEnemyManagerView { GuiEnemyView *activeBoss; GuiEnemyManagerView(GuiEnemyView *boss=nullptr): activeBoss(boss) {} };
struct GuiPlayerView { float positionX=0,positionY=0; };
struct GuiGameOwnerView { unsigned flags378C=0; };
static GuiEnemyManagerView *g_GuiEnemyManager=nullptr;
static GuiPlayerView *g_GuiPlayer=nullptr;
static GuiGameOwnerView *g_GuiGameOwner=nullptr;
static int g_GuiCounter4C4C=0,g_GuiGlobalMode=0;
static unsigned g_GuiSupervisorFlags=0;
struct EnemySoundQueueView { void QueueSoundSample(int,int); };
static EnemySoundQueueView soundOwner;
static unsigned char *g_MainSoundOwner = reinterpret_cast<unsigned char *>(&soundOwner);
struct Event { int kind,value; const void *receiver; const void *target; };
static std::vector<Event> events;
static GuiView *activeGui;
static GuiEnemyManagerView *replacementManager,*markerReplacementManager;
static GuiMessageVmView *replacementMessage,*runArgument,*finishedMessage;
static Loaded *replacementAnm;
static bool changeCounter,changeSpriteOwner,changeMarkerOwner,replaceMessage;
static int runResult;
static bool soundArgumentsValid;
static void Reset(GuiView &gui) {
    gui=GuiView(); activeGui=&gui; events.clear();
    g_GuiEnemyManager=nullptr; g_GuiPlayer=nullptr; g_GuiGameOwner=nullptr;
    replacementManager=markerReplacementManager=nullptr;
    replacementMessage=runArgument=finishedMessage=nullptr;
    replacementAnm=nullptr;
    changeCounter=changeSpriteOwner=changeMarkerOwner=replaceMessage=false;
    runResult=0; soundArgumentsValid=true; g_GuiCounter4C4C=0; g_GuiGlobalMode=0;g_GuiSupervisorFlags=0;
}
void EnemySoundQueueView::QueueSoundSample(int sound,int sample) {
    events.push_back({1,sound,this,nullptr});
    if (sample!=0 || activeGui->displayVms4[0].pendingInterrupt!=(sound==0x24?9:8) ||
        activeGui->displayVms4[1].pendingInterrupt!=(sound==0x24?9:8)) soundArgumentsValid=false;
}
static void GuiPlaySound(int id) { events.push_back({-1,id,nullptr,nullptr}); }
void Loaded::SetSprite(Vm *vm,int sprite) {
    events.push_back({2,sprite,this,vm});
    if(changeSpriteOwner && vm==&activeGui->displayVms4[0]) {
        g_GuiEnemyManager=replacementManager;
        activeGui->frontAnm=replacementAnm;
        activeGui->spellSeconds=19;
    }
}
struct AnmRenderManagerView { static int ExecuteScript(Vm *vm) {
    events.push_back({3,0,nullptr,vm});
    if(changeCounter && vm==&activeGui->displayVms5[0]) g_GuiCounter4C4C=99999;
    if(changeMarkerOwner && vm==&activeGui->bossPositionVm)
        g_GuiEnemyManager=markerReplacementManager;
    return 0;
} };
int GuiMessageVmView::Run(GuiMessageVmView *message) {
    runArgument=message; events.push_back({4,0,nullptr,message});
    if(replaceMessage) activeGui->message=replacementMessage;
    return runResult;
}
static void UpdateBossGauge(GuiView *) {}
static void FinishMessage(GuiView *gui) {
    finishedMessage=gui->message; gui->message=nullptr;
    events.push_back({5,0,nullptr,finishedMessage});
}
@OWNERS@
static int CounterAndMessageContracts() {
    GuiView gui; Loaded anm{1}; Reset(gui); gui.frontAnm=&anm;
    changeCounter=true;g_GuiCounter4C4C=12345;
    CHECK(GuiView::UpdateStageElements(&gui)==1);
    int digit=1;
    for(const Event &event:events) if(event.kind==2) {
        CHECK(event.receiver==&anm);
        CHECK(event.target==&gui.displayVms5[digit]);
        CHECK(event.value==0x1e + digit);++digit;
    }
    CHECK(digit==6);CHECK(g_GuiCounter4C4C==99999);CHECK(gui.frameTimer.ticks==1);
    for(int outcome=0;outcome<2;++outcome) for(int clear=0;clear<2;++clear) {
        Reset(gui);GuiMessageVmView original,replacement;
        gui.message=&original;replaceMessage=true;
        replacementMessage=clear?nullptr:&replacement;runResult=outcome;
        CHECK(GuiView::UpdateStageElements(&gui)==1);
        CHECK(runArgument==&original);
        CHECK(original.lifetimeTimer.ticks==(outcome==0?1:0));
        CHECK(replacement.lifetimeTimer.ticks==0);
        if(outcome) {CHECK(finishedMessage==replacementMessage);CHECK(gui.message==nullptr);}
        else CHECK(gui.message==replacementMessage);
        CHECK(gui.frameTimer.ticks==1);
    }
    return 0;
}
static int CountdownContracts() {
    GuiView gui;Loaded anm{1};GuiEnemyView boss;GuiEnemyManagerView manager{&boss};
    const int seconds[]={0,5,6,10,11};const int sounds[]={0x24,0x24,0x1b,0x1b,0};
    for(unsigned k=0;k<sizeof(seconds)/sizeof(*seconds);++k) {
        Reset(gui);gui.frontAnm=&anm;g_GuiEnemyManager=&manager;
        gui.spellSeconds=seconds[k];gui.previousSpellSeconds=seconds[k]+1;
        UpdateSpellAndBossPosition(&gui);
        CHECK(soundArgumentsValid);
        CHECK(gui.previousSpellSeconds==seconds[k]);
        const int offset=sounds[k]?1:0;
        CHECK(events.size()==unsigned(offset+3));
        if(offset) { CHECK(events[0].kind==1);CHECK(events[0].value==sounds[k]);
                     CHECK(events[0].receiver==g_MainSoundOwner); }
        CHECK(events[offset].kind==2 && events[offset].receiver==&anm);
        CHECK(events[offset].target==&gui.displayVms4[0]);
        CHECK(events[offset].value==seconds[k]/10+8);
        CHECK(events[offset+1].kind==2 && events[offset+1].target==&gui.displayVms4[1]);
        CHECK(events[offset+1].value==seconds[k]%10+8);
        CHECK(events[offset+2].kind==3 && events[offset+2].target==&gui.bossPositionVm);
    }
    // Increasing seconds interrupts 7 with no sound; equal seconds skip sprites.
    for(int equal=0;equal<2;++equal) {
        Reset(gui);gui.frontAnm=&anm;g_GuiEnemyManager=&manager;
        gui.spellSeconds=7;gui.previousSpellSeconds=equal?7:6;
        UpdateSpellAndBossPosition(&gui);
        CHECK(events.size()==unsigned(equal?1:3));
        CHECK(gui.displayVms4[0].pendingInterrupt==(equal?0:7));
        CHECK(gui.displayVms4[1].pendingInterrupt==(equal?0:7));
        for(const Event &event:events) CHECK(event.kind!=1 && event.kind!=-1);
    }
    // Timer gates do not fabricate callback events or advance previous seconds.
    for(int gate=0;gate<4;++gate) {
        Reset(gui);gui.frontAnm=&anm;g_GuiEnemyManager=&manager;manager.activeBoss=&boss;
        GuiMessageVmView message;gui.spellSeconds=5;gui.previousSpellSeconds=6;
        if(gate==0) g_GuiEnemyManager=nullptr;
        if(gate==1) manager.activeBoss=nullptr;
        if(gate==2) gui.spellSeconds=-1;
        if(gate==3) gui.message=&message;
        UpdateSpellAndBossPosition(&gui);
        CHECK(gui.previousSpellSeconds==6);
        CHECK(events.size()==unsigned(gate<2?0:1));
    }
    return 0;
}
static int BossReloadContract() {
    GuiView gui;Loaded first{1},second{2};GuiPlayerView player;
    GuiEnemyView oldBoss,newBoss,laterBoss;
    oldBoss.flags=0x11;newBoss.positionX=32;laterBoss.positionX=100;
    GuiEnemyManagerView oldManager{&oldBoss},newManager{&newBoss},laterManager{&laterBoss};
    Reset(gui);gui.frontAnm=&first;g_GuiEnemyManager=&oldManager;g_GuiPlayer=&player;
    gui.spellSeconds=5;gui.previousSpellSeconds=6;
    changeSpriteOwner=true;replacementManager=&newManager;replacementAnm=&second;
    changeMarkerOwner=true;markerReplacementManager=&laterManager;
    UpdateSpellAndBossPosition(&gui);
    CHECK(soundArgumentsValid);
    CHECK(events.size()==4);
    CHECK(events[0].kind==1 && events[0].value==0x24 && events[0].receiver==g_MainSoundOwner);
    CHECK(events[1].kind==2 && events[1].receiver==&first && events[1].value==8);
    CHECK(events[2].kind==2 && events[2].receiver==&second && events[2].value==17);
    CHECK(gui.previousSpellSeconds==19);
    CHECK(events[3].kind==3 && events[3].target==&gui.bossPositionVm);
    CHECK(g_GuiEnemyManager==&laterManager);
    CHECK(gui.bossPositionVm.positionOffset.x==256.0f);
    CHECK(gui.bossPositionVm.positionOffset.y==480.0f);
    CHECK(gui.bossPositionVm.primaryColor.alpha==159);
    return 0;
}
int main() {
    if(CounterAndMessageContracts()) return 1;
    if(CountdownContracts()) return 1;
    if(BossReloadContract()) return 1;
    return 0;
}
'''


class HudLifetimeContracts(unittest.TestCase):
    def run_owner(self, source):
        compiler=shutil.which('c++')
        if compiler is None: self.skipTest('host C++ compiler unavailable')
        with tempfile.TemporaryDirectory(prefix='th10-hud-') as folder:
            path=Path(folder)/'hud.cpp';exe=Path(folder)/'hud'
            path.write_text(PROGRAM.replace('@OWNERS@',source))
            result=subprocess.run([compiler,'-std=c++11','-O0','-fsanitize=undefined',
                                   '-fno-sanitize-recover=undefined',str(path),'-o',str(exe)],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stdout+result.stderr)
            return subprocess.run([str(exe)],capture_output=True,text=True)

    def test_actual_source_contracts(self):
        result=self.run_owner(owner_source())
        self.assertEqual(result.returncode,0,result.stdout+result.stderr)

    def test_mutation_controls(self):
        source=owner_source()
        replacements=[
            ('counter-after-call',
             'int value = g_GuiCounter4C4C;\n    AnmRenderManagerView::ExecuteScript(&gui->displayVms5[0]);',
             'AnmRenderManagerView::ExecuteScript(&gui->displayVms5[0]);\n    int value = g_GuiCounter4C4C;'),
            ('reload-success-message','message->lifetimeTimer.Tick();','gui->message->lifetimeTimer.Tick();'),
            ('reload-boss-after-marker', 'AnmRenderManagerView::ExecuteScript(&gui->bossPositionVm);', 'AnmRenderManagerView::ExecuteScript(&gui->bossPositionVm);\n    boss = g_GuiEnemyManager->activeBoss;'),
            ('stale-countdown-manager','        manager = g_GuiEnemyManager;', '        /* stale manager */'),
            ('wrong-failure-owner', 'FinishMessage(gui);', '{ gui->message = message; FinishMessage(gui); }'),
            ('wrong-low-sound','QueueSoundSample(0x24, 0);','QueueSoundSample(0x1b, 0);'),
            ('wrong-high-sound','QueueSoundSample(0x1b, 0);','QueueSoundSample(0x24, 0);'),
            ('threshold-five','gui->spellSeconds < 6','gui->spellSeconds < 5'),
            ('threshold-ten','gui->spellSeconds < 11','gui->spellSeconds < 10'),
        ]
        for name,old,new in replacements:
            with self.subTest(mutation=name):
                self.assertIn(old,source)
                result=self.run_owner(source.replace(old,new))
                self.assertEqual(result.returncode,1,result.stdout+result.stderr)
                self.assertRegex(result.stderr,r'line [0-9]+: ')
                self.assertNotIn('runtime error:',result.stderr)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,default=SOURCE)
    args,rest=parser.parse_known_args();SOURCE=args.source
    unittest.main(argv=[__file__]+rest)
