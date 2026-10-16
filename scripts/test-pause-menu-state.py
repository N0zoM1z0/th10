#!/usr/bin/env python3
"""Host synthetic contracts for the current TH10 pause drafts, not target/runtime proof.

Extracts production definitions on every run; no snapshot or upstream implementation
is used as an oracle. Requires Python 3 and a host C++17 compiler. Layout, ABI,
float-to-int exceptional inputs, x87 behavior, rendering and real engine callbacks
are deliberately outside this test. The timer implementation and pending/script
helpers are mocks. Busy-capture tests verify lookup and unchanged fields, not
conversion-before-rejection exceptions. Run with --root PATH (default: repository root).
"""
import argparse
import os
from pathlib import Path
import re
import subprocess
import tempfile


def lexical_mask(text):
    # Keep offsets/newlines while hiding braces in comments and literals.
    pattern = r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    return re.sub(pattern, lambda m: ''.join('\n' if c == '\n' else ' ' for c in m[0]), text)


def extract(files, name, struct=False):
    found = []
    if struct:
        pat = re.compile(r'^\s*struct\s+' + re.escape(name) + r'\s*\{', re.M)
    else:
        # A definition must start on its own line with its return type. Do not
        # match extern prototypes or a call followed by some later opening brace.
        pat = re.compile(r'^\s*(?:int|void)\s+(?:(?:__stdcall|__fastcall)\s+)?'
                         + re.escape(name) + r'\s*\([^;{}]*\)\s*\{', re.M)
    for path, text in files:
        mask = lexical_mask(text)
        for match in pat.finditer(mask):
            start = mask.index('{', match.start(), match.end())
            depth = 1
            end = start + 1
            while end < len(mask) and depth:
                depth += (mask[end] == '{') - (mask[end] == '}')
                end += 1
            if depth:
                raise RuntimeError(f'Unclosed definition: {name} in {path}')
            found.append((path, text[match.start():end] + (';' if struct else '')))
    if len(found) != 1:
        raise RuntimeError(f'Expected one definition for {name}; found {len(found)}: '
                           + ', '.join(str(p) for p, _ in found))
    return found[0][1]


PREFIX = r'''
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#define __stdcall
#define __fastcall
using std::string;
std::vector<string> events;
std::function<void(const string&)> callback;
void event(string s) { events.push_back(s); if(callback) callback(s); }
void expect(std::initializer_list<string> xs) { assert(events == std::vector<string>(xs)); }
struct FrontEndCursorView;
int FrontEndCursorSetCurrent(int, FrontEndCursorView*);
struct FrontEndInputView { unsigned short pressed=0,repeated=0; int IsRepeated(unsigned short); };
struct ReplayManager {};
struct AnmVmTimerView { int current=0; void SetCurrent(int n) { current=n; event("timer:"+std::to_string(n)); } };
struct AnmVmIdView { int value=0; void SetInterrupt(short n) { event("interrupt:"+std::to_string(value)+":"+std::to_string(n)); } };
struct AnmSpriteView { float height=47.9f,width=31.2f,sourceY=-5.8f,sourceX=3.7f; };
struct AnmVmView;
struct AnmLoadedView { int anmFileIndex=71; string tag; void InitializeVm(AnmVmView*,int); };
struct AnmVmView { int renderLayer=0; unsigned flags35C=0x21; AnmSpriteView* loadedSprite=nullptr; AnmLoadedView* anmFile=nullptr; };
struct AnmRenderManagerView {
 string tag; AnmVmView* vm=nullptr; AnmVmView* lastAdded=nullptr; int nextId=0;
 int captureSurfaceIndex=-1,captureDestinationX=901,captureSourceX=902,captureDestinationY=903,
 captureDestinationWidth=904,captureSourceHeight=905,captureSourceY=906,captureDestinationHeight=907,
 captureSourceWidth=908,captureRequestState=909;
 AnmVmView* FindVm(int id) { auto* result=vm; event(tag+":find:"+std::to_string(id)); return result; }
 AnmVmView* AllocateVm() { auto* result=vm; event(tag+":allocate"); return result; }
 AnmVmIdView AddVmVariant1(AnmVmView* v) { assert(v); lastAdded=v; event(tag+":add"); return {nextId}; }
 void MarkVmForDeletion(AnmVmIdView id) { event(tag+":delete:"+std::to_string(id.value)); }
 int RequestCaptureRectangle(int,int,int,int,int);
};
void AnmLoadedView::InitializeVm(AnmVmView* v,int script) {
 assert(v->renderLayer==15 && v->flags35C==0x40000021);
 v->anmFile=this; event(tag+":initialize:"+std::to_string(script));
}
struct ResultGameManager { unsigned flags58=0; int initializationMode=0; } game;
struct Ascii { AnmLoadedView* captureAnm=nullptr; } ascii;
struct Gui { AnmLoadedView* frontAnm=nullptr; } gui;
ResultGameManager* g_ResultGameManager=&game;
Ascii* g_AsciiManagerView=&ascii;
Gui* g_GuiView=&gui;
AnmRenderManagerView* g_AnmRenderManagerView=nullptr;
float g_AnmGameSpeed=0.5f;
FrontEndInputView g_FrontEndInput;
int g_GameTransitionState=77;
struct Supervisor { void SetNextGameMode(int n) { event("mode:"+std::to_string(n)); } } g_MainSupervisorView;
void ResultMenuPendingThree(AnmVmIdView* id) { event("three:"+std::to_string(id->value)); }
void ResultMenuPendingSix(AnmVmIdView* id) { event("six:"+std::to_string(id->value)); }
void ResultMenuSound(int n) { event("sound:"+std::to_string(n)); }
void FrontEndQueueSoundCommand(const char* s,int n,int zero) { assert(zero==0); event(string(s)+":"+std::to_string(n)); }
AnmVmIdView ResultMenuFindScript(AnmVmIdView* id,int script) { event("script:"+std::to_string(id->value)+":"+std::to_string(script)); return {script+1000}; }
void EnemyMarkPendingInterrupt(int* id) { event("pending:"+std::to_string(*id)); }
'''

TESTS = r'''
AnmSpriteView sprite,spriteB,spriteC;
AnmLoadedView capture,front,other;
AnmVmView va,vb,vc;
AnmRenderManagerView a,b,c;
ResultMenuView menu;
void reset(int state=2,int time=35,int choice=0,int mode=0) {
 callback={}; events.clear(); game={0x95u,mode}; g_ResultGameManager=&game;
 capture={71,"capture"}; front={72,"front"}; other={73,"other"};
 sprite={47.9f,31.2f,-5.8f,3.7f}; va={}; vb={}; vc={};
 spriteB={61.9f,45.2f,-9.8f,8.7f};spriteC={83.9f,69.2f,-13.8f,11.7f};
 va.loadedSprite=&sprite;vb.loadedSprite=&spriteB;vc.loadedSprite=&spriteC;
 va.anmFile=&capture;vb.anmFile=&front;vc.anmFile=&other;
 a={}; b={}; c={}; a.tag="a"; b.tag="b"; c.tag="c";
 a.vm=&va; b.vm=&vb; c.vm=&vc; a.nextId=101;b.nextId=202;c.nextId=303;
 g_AnmRenderManagerView=&a; ascii.captureAnm=&capture;gui.frontAnm=&front;
 g_AnmGameSpeed=.5f;g_GameTransitionState=77;g_FrontEndInput={};
 menu={};menu.state=state;menu.timer.current=time; menu.cursor.count=mode?2:3;
 menu.cursor.current=choice; menu.cursor.wraps=1; menu.mainVm={11};menu.secondaryVm={22};menu.auxiliaryVm={33};menu.savedSpeed=.25f;
}
void pendingAll() { expect({"pending:22","pending:11","pending:33","timer:0"}); }
void testCapture() {
 reset();g_AnmRenderManagerView=&b;
 assert(a.RequestCaptureRectangle(88,-7,8,99,100)==0);
 expect({"b:find:88"});
 assert(a.captureSurfaceIndex==72 && a.captureDestinationX==-7 && a.captureDestinationY==8);
 assert(a.captureDestinationWidth==99 && a.captureDestinationHeight==100);
 assert(a.captureSourceX==8 && a.captureSourceY==-9 && a.captureSourceWidth==45 && a.captureSourceHeight==61 && a.captureRequestState==0);
 assert(b.captureSurfaceIndex==-1 && b.captureDestinationX==901);
 reset();g_AnmRenderManagerView=&c;
 assert(a.RequestCaptureRectangle(90,17,18,109,110)==0);
 expect({"c:find:90"});
 assert(a.captureSurfaceIndex==73 && a.captureSourceX==11 && a.captureSourceY==-13 && a.captureSourceWidth==69 && a.captureSourceHeight==83);
 assert(a.captureDestinationX==17 && a.captureDestinationY==18 && a.captureDestinationWidth==109 && a.captureDestinationHeight==110 && a.captureRequestState==0);
 assert(c.captureSurfaceIndex==-1 && c.captureDestinationX==901);
 reset();a.captureSurfaceIndex=0;g_AnmRenderManagerView=&b;
 assert(a.RequestCaptureRectangle(89,1,2,3,4)==-1);expect({"b:find:89"});
 assert(a.captureSurfaceIndex==0 && a.captureDestinationX==901 && a.captureSourceX==902 && a.captureDestinationY==903 && a.captureDestinationWidth==904 && a.captureSourceHeight==905 && a.captureSourceY==906 && a.captureDestinationHeight==907 && a.captureSourceWidth==908 && a.captureRequestState==909);
}
void testOpen() {
 for(int mode: {0,1}) {
  reset(8,90,0,mode);game.flags58=0x85u;
  callback=[&](const string& s) {
   if(s=="a:allocate") { ascii.captureAnm=&other;g_AnmRenderManagerView=&b; }
   if(s=="b:add") { g_AnmRenderManagerView=&c; }
   if(s=="b:allocate") { gui.frontAnm=&other; }
   if(s=="sound:32") g_AnmGameSpeed=.75f;
   if(s=="Pause:6") g_AnmGameSpeed=.875f;
  };
  ResultMenuOpenPause(&menu);
  std::vector<string> wanted={"timer:0","a:allocate","capture:initialize:0","b:add","c:find:202","b:allocate","front:initialize:121","c:add","three:303","sound:32","Pause:6"};
  if(mode) { wanted.push_back("script:303:117");wanted.push_back("c:delete:1117"); }
  assert(events==wanted);assert(menu.state==1 && menu.timer.current==0 && game.flags58==0x95u);
  assert(menu.secondaryVm.value==202 && menu.mainVm.value==303 && menu.auxiliaryVm.value==33);
  assert(menu.frontAnm==&front && menu.savedSpeed==.875f && g_AnmGameSpeed==1);
  assert(va.anmFile==&capture && vb.anmFile==&front);
  assert(b.lastAdded==&va && c.lastAdded==&vb && a.lastAdded==nullptr);
  assert(b.captureSurfaceIndex==73 && b.captureDestinationX==32 && b.captureDestinationY==16 && b.captureDestinationWidth==384 && b.captureDestinationHeight==448);
  assert(b.captureSourceX==11 && b.captureSourceY==-13 && b.captureSourceWidth==69 && b.captureSourceHeight==83 && b.captureRequestState==0);
  assert(a.captureSurfaceIndex==-1 && c.captureSurfaceIndex==-1);
 }
}
void testResume() {
 for(int aux: {0,33}) {
  reset(7,19,2);menu.auxiliaryVm.value=aux;
  callback=[&](const string& s) {
   if(s=="UnPause:7") { assert(game.flags58==0x85u);assert(g_AnmGameSpeed==.5f);g_AnmRenderManagerView=&b; }
   if(s=="b:delete:"+std::to_string(aux)) { assert(menu.auxiliaryVm.value==aux);assert(g_AnmGameSpeed==.5f); }
  };
  ResultMenuResume(&menu);
  assert(events==std::vector<string>({"UnPause:7","b:delete:"+std::to_string(aux)}));
  assert(menu.auxiliaryVm.value==0 && menu.mainVm.value==11 && menu.secondaryVm.value==22 && menu.state==7 && menu.timer.current==19);
  assert(menu.cursor.current==2 && game.flags58==0x85u && g_AnmGameSpeed==.25f);
 }
}
void testEntryAndOutside() {
 for(int state: {-1,0,6,99}) { reset(state);g_FrontEndInput.pressed=0xffff;ResultMenuUpdatePause(&menu);assert(events.empty() && menu.state==state); }
 for(int mode: {0,1}) for(int t: {9,10,11}) {
  reset(1,t,2,mode);g_FrontEndInput.pressed=0xffff;ResultMenuUpdatePause(&menu);
  if(t<10) assert(events.empty() && menu.state==1);
  else {expect({"interrupt:11:7"});assert(menu.state==2 && menu.cursor.current==0 && menu.cursor.count==(mode?2:3) && menu.cursor.wraps==1);}
  assert(menu.timer.current==t);
 }
}
void testNavigation() {
 for(int state: {2,4}) for(int mode: {0,1}) for(int channel: {0,1,2}) for(int bits: {0x10,0x20,0x30}) {
  reset(state,31,0,mode); if(state==4) menu.cursor.count=2;
  int count=menu.cursor.count;
  if(channel!=1) g_FrontEndInput.pressed=bits;
  if(channel!=0) g_FrontEndInput.repeated=bits;
  ResultMenuUpdatePause(&menu);
  int wanted=bits==0x10?count-1:bits==0x20?1:0;
  assert(menu.cursor.current==wanted && menu.cursor.previous==0);
  if(wanted==0) assert(events.empty());
  else assert(events==std::vector<string>({"interrupt:11:"+std::to_string(wanted+(state==2?7:15)),"sound:12"}));
 }
 // The production Move helper skips disabled choices and honors non-wrapping cursors.
 reset();menu.cursor.disabledEntries[0]=1;menu.cursor.disabledEntryCount=1;
 assert(menu.cursor.Move(1)==2);menu.cursor.disabledEntryCount=0;menu.cursor.wraps=0;
 assert(menu.cursor.Move(1)==2);menu.cursor.SetCurrent(99);assert(menu.cursor.current==2);
 menu.cursor.SetCurrent(-1);assert(menu.cursor.current==0);
}
void testSelectionsAndPrecedence() {
 for(int mode: {0,1}) for(int choice=0;choice<(mode?2:3);++choice) for(int confirm: {1,0x1000}) {
  reset(2,35,choice,mode);g_FrontEndInput.pressed=confirm;ResultMenuUpdatePause(&menu);
  int state=choice==0 || (choice==1 && mode)?3:4;
  assert(menu.state==state && menu.timer.current==0);
  if(choice==0) expect({"sound:10","pending:22","pending:11","pending:33","timer:0"});
  else assert(events==std::vector<string>({"sound:10","script:11:"+std::to_string(choice+115),"six:"+std::to_string(choice+1115),"timer:0"}));
 }
 // Enumerate all combinations: confirm, restart shortcut, title shortcut, cancel.
 for(int mode: {0,1}) for(int mask=0;mask<16;++mask) {
  reset(2,35,0,mode);unsigned bits=(mask&1?1:0)|(mask&2?0x4000:0)|(mask&4?0x200:0)|(mask&8?8:0);
  g_FrontEndInput.pressed=bits;ResultMenuUpdatePause(&menu);
  std::vector<string> want;
  auto append=[&](std::initializer_list<string> xs){want.insert(want.end(),xs.begin(),xs.end());};
  if(mask&1) append({"sound:10","pending:22","pending:11","pending:33","timer:0"});
  if(mask&2) append({"sound:10","script:11:117","six:1117","timer:0","pending:22"});
  if(mask&4) append({"sound:10","script:11:116","six:1116","timer:0"});
  if(mask&8) append({"pending:22","pending:11","pending:33","timer:0"});
  assert(events==want);int chosen=(mask&8)?0:(mask&4)?1:(mask&2)?(mode?1:2):0;
  assert(menu.cursor.current==chosen && menu.state==(mask?3:2));
 }
 // Repeated confirm/shortcut/cancel bits alone do not activate actions.
 reset();g_FrontEndInput.repeated=0x5209;ResultMenuUpdatePause(&menu);assert(events.empty() && menu.state==2);
}
void testExit() {
 for(int mode: {0,1}) for(int choice: {0,1,2,8}) for(int t: {11,12,19}) {
  reset(3,t,choice,mode);g_FrontEndInput.pressed=0xffff;ResultMenuUpdatePause(&menu);
  if(t<12) {assert(events.empty() && menu.state==3);continue;}
  assert(menu.state==0 && menu.timer.current==t);
  if(choice==0) {expect({"UnPause:7","a:delete:33"});assert(g_AnmGameSpeed==.25f && menu.auxiliaryVm.value==0);}
  if(choice==1) expect({"pending:22","pending:11","mode:4"});
  if(choice==2) {expect({"pending:11"});assert(g_GameTransitionState==10);}
  if(choice==8) assert(events.empty());
 }
}
void testConfirmation() {
 for(int t: {19,20,21,29,30,31}) {
  reset(4,t,2);menu.cursor.disabledEntryCount=1;menu.cursor.disabledEntries[0]=99;
  ResultMenuUpdatePause(&menu);
  if(t==20) {expect({"interrupt:11:14"});assert(menu.cursor.saveDepth==1 && menu.cursor.current==1 && menu.cursor.count==2 && menu.cursor.savedCurrent[0]==2 && menu.cursor.savedCount[0]==3 && menu.cursor.disabledEntryCount==0);}
  else {assert(menu.cursor.saveDepth==0 && menu.cursor.current==2 && menu.cursor.count==3);if(t==30)expect({"interrupt:11:17"});else assert(events.empty());}
 }
 // Sequential timer crossing, including the exact entry and ready frames.
 reset(4,20,2);ResultMenuUpdatePause(&menu);menu.timer.current=21;events.clear();ResultMenuUpdatePause(&menu);assert(events.empty() && menu.cursor.saveDepth==1);
 menu.timer.current=29;ResultMenuUpdatePause(&menu);assert(events.empty());menu.timer.current=30;ResultMenuUpdatePause(&menu);expect({"interrupt:11:16"});
 // Inputs before ready frame are ignored, including cancel.
 for(int t:{19,20,21,29}) {reset(4,t,2);g_FrontEndInput.pressed=0xffff;ResultMenuUpdatePause(&menu);assert(menu.state==4);}
 for(int choice:{0,1,9}) for(int cancel:{0,8}) {
  reset(4,31,choice);g_FrontEndInput.pressed=1|cancel;ResultMenuUpdatePause(&menu);
  std::vector<string> want={"sound:10"};
  if(choice<2) {want.push_back("script:11:"+std::to_string(119+choice));want.push_back("six:"+std::to_string(1119+choice));}
  want.push_back("timer:0");
  if(cancel) {want.insert(want.end(),{"pending:22","pending:11","pending:33","timer:0"});}
  assert(events==want && menu.state==(cancel?3:choice<2?5:4));
  assert(menu.timer.current==0 && menu.cursor.current==(cancel?0:choice));
 }
 // The frame-30 ready interrupt precedes navigation and confirmation.
 reset(4,30,1);menu.cursor.count=2;g_FrontEndInput.pressed=0x11;
 ResultMenuUpdatePause(&menu);
 expect({"interrupt:11:16","interrupt:11:15","sound:12","sound:10","script:11:119","six:1119","timer:0"});
 assert(menu.state==5 && menu.cursor.current==0);
 // Shortcuts are specific to state 2 and do not operate in state 4.
 reset(4,31,1);g_FrontEndInput.pressed=0x4200;ResultMenuUpdatePause(&menu);assert(events.empty() && menu.state==4);
 for(int choice:{0,1,9}) for(int t:{19,20,21}) {
  reset(5,t,choice);menu.cursor.saveDepth=1;menu.cursor.savedCurrent[0]=2;menu.cursor.savedCount[0]=3;menu.cursor.count=2;
  g_FrontEndInput.pressed=0xffff;ResultMenuUpdatePause(&menu);
  if(t<20) {assert(events.empty() && menu.state==5 && menu.cursor.saveDepth==1);continue;}
  if(choice==0) expect({"pending:22","pending:11","timer:0"});
  if(choice==1) expect({"interrupt:11:9","timer:0"});
  if(choice==9) expect({"timer:0"});
  assert(menu.state==(choice==0?3:choice==1?2:5));
  assert(menu.timer.current==0 && menu.cursor.saveDepth==(choice<2?0:1));
  if(choice<2) assert(menu.cursor.current==2 && menu.cursor.count==3);
 }
}
int main() {
 testCapture();testOpen();testResume();testEntryAndOutside();testNavigation();
 testSelectionsAndPrecedence();testExit();testConfirmation();
 std::cout << "PASS: host synthetic pause contracts (not target exactness/runtime proof)\n";
}
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--cxx', default=os.environ.get('CXX', 'c++'))
    args = parser.parse_args()
    implementation = args.root / 'src/FrontEnd.cpp'
    cursor_header = args.root / 'src/FrontEnd.hpp'
    menu_header = args.root / 'src/ResultMenu.hpp'
    files = [(implementation, implementation.read_text())]
    names = ['FrontEndCursorSetCurrent', 'FrontEndCursorView::Push',
             'FrontEndCursorView::Pop', 'FrontEndCursorView::Move',
             'FrontEndInputView::IsRepeated', 'AnmRenderManagerView::RequestCaptureRectangle',
             'ResultMenuOpenPause', 'ResultMenuResume', 'ResultMenuUpdatePause']
    source = PREFIX + extract([(cursor_header, cursor_header.read_text())], 'FrontEndCursorView', True) + '\n'
    source += extract([(menu_header, menu_header.read_text())], 'ResultMenuView', True) + '\n'
    source += '\n'.join(extract(files, name) for name in names) + '\n' + TESTS
    with tempfile.TemporaryDirectory(prefix='th10-pause-contracts-') as temp:
        cpp, binary = Path(temp)/'contracts.cpp', Path(temp)/'contracts'
        cpp.write_text(source)
        subprocess.run([args.cxx, '-std=c++17', '-Wall', '-Wextra', '-Werror', '-O0',
                        str(cpp), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    main()
