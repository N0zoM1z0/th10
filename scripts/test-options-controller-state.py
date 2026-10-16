#!/usr/bin/env python3
"""Actual-source TH10 UpdateOptions host regression, not native/ABI proof.

Extracts owner and transitive helpers from --source, cursor/input implementations
from that source (or explicit --support-source) and the actual cursor header. Independent target-order model follows
options-update-target.txt and helper disassembly 449670/42C770. Timer, creation,
refresh, immediate interrupt, screen setters and sound are explicit event mocks.
Invalid null parents, native layouts/resources and real callbacks are excluded.
D3D colorMode is a distinct untouched canary; options use hintMode.
Historical --expect-failure uses only safe 0..2 color indices (its table is unsafe
outside those values); all signed-byte colors are tested on proposed source.
Baseline also excludes INT_MAX callbacks because its int addition is undefined;
proposed source tests them under UBSan without -fwrapv. Input bit positions and
screen destination integers are synthetic labels, not native numeric ABI checks.
"""
import argparse, re, subprocess, tempfile
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
def mask(s):
    return re.sub(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',lambda m: ''.join('\n' if c=='\n' else ' ' for c in m[0]),s)
def extract(s,name,optional=False):
    m=mask(s)
    pattern=r'^\s*(?:static\s+)?(?:(?:inline|__forceinline)\s+)?(?:void|int|AnmVmIdView|AnmVmView\s*\*)\s*(?:(?:__stdcall|__fastcall)\s+)?'+re.escape(name)+r'\s*\([^;{}]*\)\s*\{'
    hits=list(re.finditer(pattern,m,re.M))
    if optional and not hits:return ''
    if len(hits)!=1:raise ValueError(f'{name}: expected one definition, got {len(hits)}')
    a=hits[0].start();b=m.index('{',a,hits[0].end())+1;depth=1
    while depth and b<len(m):depth+=(m[b]=='{')-(m[b]=='}');b+=1
    if depth:raise ValueError('Unclosed '+name)
    return s[a:b]

def extract_struct(s,name):
    m=mask(s)
    hits=list(re.finditer(r'^\s*struct\s+'+re.escape(name)+r'\s*\{',m,re.M))
    if len(hits)!=1:raise ValueError(f'{name}: expected one struct definition, got {len(hits)}')
    a=hits[0].start();b=m.index('{',a,hits[0].end())+1;depth=1
    while depth and b<len(m):depth+=(m[b]=='{')-(m[b]=='}');b+=1
    if depth:raise ValueError('Unclosed struct '+name)
    return s[a:b]+';'

PREFIX=r'''
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <climits>
#include <cassert>
#define __stdcall
#define __fastcall
#ifndef __forceinline
#define __forceinline inline
#endif
using namespace std;
vector<string> trace; function<void(string)> hook;
void event(string s) {trace.push_back(s);if(hook)hook(s);}
struct FrontEndCursorView; int FrontEndCursorSetCurrent(int,FrontEndCursorView*);
struct FrontEndInputView { unsigned short pressed=0,repeated=0;int IsRepeated(unsigned short); } g_FrontEndInput;
'''
FIXTURE=r'''
enum {FRONT_END_ROOT_VM=1,FRONT_END_OPTIONS_INITIALIZE=0,FRONT_END_OPTIONS_OPENING=1,FRONT_END_OPTIONS_ACTIVE=2,FRONT_END_OPTIONS_UNUSED=3,FRONT_END_OPTIONS_CLOSING=4};
enum {FRONT_END_INPUT_UP=1,FRONT_END_INPUT_DOWN=2,FRONT_END_INPUT_LEFT=4,FRONT_END_INPUT_RIGHT=8,FRONT_END_INPUT_CANCEL=16,FRONT_END_INPUT_CONFIRM=32};
enum {FRONT_END_SOUND_MOVE=12,FRONT_END_SOUND_SELECT=10,FRONT_END_SOUND_CANCEL=11,FRONT_END_SOUND_PREVIEW=4,FRONT_END_SCREEN_KEY_CONFIG=100,FRONT_END_SCREEN_MAIN_MENU_RETURN=101};
struct Supervisor {signed char colorMode=85,hintMode=0,bgmVolume=100,sfxVolume=80;} g_MainSupervisorView;
struct AnmVmView;
struct AnmLoadedView {int identity;void SetSprite(AnmVmView*,int);};
struct AnmVmLayerNodeView {void* owner=nullptr;AnmVmLayerNodeView* next=nullptr;AnmVmLayerNodeView* previous=nullptr;};
struct AnmVmView {int id=0;short scriptIndex=-1,pendingInterrupt=-99;int sprite=-99;AnmVmLayerNodeView layerNode;AnmLoadedView* anmFile=nullptr;};
struct AnmVmIdView {int value;AnmVmIdView():value(0){} AnmVmIdView(int n):value(n){} void SetSprite(int);};
struct AnmRenderManagerView {int identity;map<int,AnmVmView*> vms;AnmVmView* FindVm(AnmVmIdView id) {auto* v=vms.count(id.value)?vms[id.value]:nullptr;event("find:"+to_string(identity)+":"+to_string(id.value));return v;}} managers[2];
AnmRenderManagerView* g_AnmRenderManagerView;
void AnmLoadedView::SetSprite(AnmVmView* v,int n){v->sprite=n;event("sprite:"+to_string(identity)+":"+to_string(v->id)+":"+to_string(n));}
// 449670: every invocation resolves its typed ID using the current global manager.
void AnmVmIdView::SetSprite(int n){auto* v=g_AnmRenderManagerView->FindVm(*this);if(v)v->anmFile->SetSprite(v,n);}
struct Timer {int current=0;};
struct FrontEndControllerView {int screenState=2,screen=0;FrontEndCursorView cursor{};Timer stateTimer;AnmVmIdView vmIds[100];void RefreshOptionsDisplay();void SetScreenStateTarget(int);void SetScreenTarget(int);static int UpdateOptions(FrontEndControllerView*);} controller;
void FrontEndControllerView::RefreshOptionsDisplay(){event("refresh");}
void SetScreenState(FrontEndControllerView* c,int n){c->screenState=n;c->stateTimer.current=0;event("state:"+to_string(n));}
void SetScreen(FrontEndControllerView* c,int n){c->screen=n;c->screenState=0;c->stateTimer.current=0;event("screen:"+to_string(n));}
void FrontEndControllerView::SetScreenStateTarget(int n){SetScreenState(this,n);}
void FrontEndControllerView::SetScreenTarget(int n){SetScreen(this,n);}
void CreateVm(FrontEndControllerView*,int n){event("create:"+to_string(n));}
void FrontEndPlaySound(int n){event("sound:"+to_string(n));}
// Native sound owner/layout and QueueSoundSample internals are mocked; sample
// argument zero and the owner-specific extracted wrapper are still checked.
struct EnemySoundQueueView {void QueueSoundSample(int n,int sample){assert(sample==0);event("sound:"+to_string(n));}} optionsSoundQueue;
void* g_MainSoundOwner=&optionsSoundQueue;
void InterruptVmNow(FrontEndControllerView*,int n,short k){event("now:"+to_string(n)+":"+to_string(k));}
int InputRepeated(unsigned short n){int r=g_FrontEndInput.IsRepeated(n);event("input:"+to_string(n));return r;}
bool TimerReachedMultiple(Timer* t,int n){event("timer:"+to_string(n));return t->current%n==0;}
void FrontEndMarkVmInterrupt(FrontEndControllerView*,int,short);
// Historical InterruptVm is the same deferred/propagating 42C770 seam.
void InterruptVm(FrontEndControllerView* c,int i,short n){FrontEndMarkVmInterrupt(c,i,n);}
'''
ORACLE=r'''
int signedByte(int n){n&=255;return n>=128?n-256:n;}
short lowWord(int n,int add){return static_cast<short>((static_cast<unsigned>(n)+add)&65535);}
void targetDeferred(int index,short n){auto* v=g_AnmRenderManagerView->FindVm(controller.vmIds[index]);if(!v)return;v->pendingInterrupt=n;if(!v->layerNode.previous)for(auto* node=v->layerNode.next;node;node=node->next)static_cast<AnmVmView*>(node->owner)->pendingInterrupt=n;}
int targetChild(AnmRenderManagerView* m,int label){auto* root=m->FindVm(controller.vmIds[1]);assert(root);for(auto* p=&root->layerNode;p;p=p->next){auto* v=static_cast<AnmVmView*>(p->owner);if(v->scriptIndex==label)return v->id;}return 0;}
void targetColors(AnmRenderManagerView* m){int mode=g_MainSupervisorView.hintMode;int sprite=mode==0?46:mode==1?48:44;for(int i=0;i<2;++i){int id=targetChild(m,31+i);auto* v=g_AnmRenderManagerView->FindVm(AnmVmIdView(id));if(v)v->anmFile->SetSprite(v,sprite+i);}}
void targetClose(int sound){targetDeferred(1,6);FrontEndPlaySound(sound);SetScreenState(&controller,4);}
void target(){auto& c=controller;auto& p=c.cursor;auto& s=g_MainSupervisorView;
 if(c.screenState==0){p.count=6;p.SetCurrent(0);CreateVm(&c,1);c.RefreshOptionsDisplay();SetScreenState(&c,1);}
 if(c.screenState==1){if(c.stateTimer.current>6){SetScreenState(&c,2);InterruptVmNow(&c,1,3);targetColors(g_AnmRenderManagerView);targetDeferred(1,lowWord(p.current,17));}return;}
 if(c.screenState==4){if(c.stateTimer.current<10)return;if(p.current==3){SetScreen(&c,100);p.Push();}else if(p.current==5){InterruptVmNow(&c,90,2);InterruptVmNow(&c,91,2);SetScreen(&c,101);p.Pop();}return;}
 if(c.screenState!=2)return;
 p.previous=p.current;
 if(InputRepeated(1))p.Move(-1);if(InputRepeated(2))p.Move(1);
 if(p.previous!=p.current){FrontEndPlaySound(12);InterruptVmNow(&c,1,3);targetDeferred(1,lowWord(p.current,7));}
 if(g_FrontEndInput.pressed&16){if(p.current==5)targetClose(11);else{FrontEndPlaySound(11);p.SetCurrent(5);InterruptVmNow(&c,1,3);targetDeferred(1,lowWord(p.current,7));}return;}
 if(p.current==2 && TimerReachedMultiple(&c.stateTimer,60))FrontEndPlaySound(4);
 int left=InputRepeated(4);auto* manager=g_AnmRenderManagerView;
 for(int direction=0;direction<2;++direction){int active=direction?InputRepeated(8):left;if(!active)continue;int row=p.current;
  if(row==0){int old=s.hintMode;s.hintMode=direction?((old&255)<2?old+1:0):(old==0?2:signedByte(old-1));if(direction)FrontEndPlaySound(12);targetColors(manager);if(!direction)FrontEndPlaySound(12);}
  else if(row==1||row==2){auto& v=row==1?s.bgmVolume:s.sfxVolume;int next=direction?signedByte(int(v)+5):(v<5?0:int(v)-5);if(direction&&next>100)next=100;v=next;c.RefreshOptionsDisplay();}
 }
 if(g_FrontEndInput.pressed&32){if(p.current==3||p.current==5){targetClose(p.current==3?10:11);return;}if(p.current==4){s.bgmVolume=100;s.sfxVolume=80;s.hintMode=0;c.RefreshOptionsDisplay();targetColors(manager);FrontEndPlaySound(10);}}
}
struct Case {string name;int state=2,row=0,timer=1,pressed=0,repeated=0,volume=100,color=0,count=6;string trigger="";int change=0;int missing=0;};
AnmVmView roots[2][2],nodes[2][2][2],resolved[2][4];AnmLoadedView files[2];
void reset(const Case& x){hook=nullptr;trace.clear();controller={};controller.screenState=x.state;controller.cursor.count=x.count;controller.cursor.current=x.row;controller.cursor.previous=-8;controller.cursor.wraps=1;controller.cursor.saveDepth=1;controller.cursor.savedCurrent[0]=4;controller.cursor.savedCount[0]=7;controller.stateTimer.current=x.timer;controller.vmIds[1]=1;g_MainSupervisorView.colorMode=85;g_MainSupervisorView.hintMode=x.color;g_MainSupervisorView.bgmVolume=x.volume;g_MainSupervisorView.sfxVolume=x.volume;g_FrontEndInput.pressed=x.pressed;g_FrontEndInput.repeated=x.repeated;g_AnmRenderManagerView=&managers[0];
 for(int m=0;m<2;++m){managers[m].identity=m;managers[m].vms.clear();files[m].identity=m;for(int i=0;i<4;++i){auto& v=resolved[m][i];v={};v.id=100+i;v.anmFile=&files[m];managers[m].vms[v.id]=x.missing==2&&i==0?nullptr:&v;}
 for(int r=0;r<2;++r){auto& root=roots[m][r];root={};root.id=r+1;root.layerNode.owner=&root;root.layerNode.next=&nodes[m][r][0].layerNode;managers[m].vms[r+1]=&root;for(int i=0;i<2;++i){auto& v=nodes[m][r][i];v={};v.id=100+r*2+i;v.scriptIndex=x.missing==1&&i==0?-1:31+i;v.layerNode.owner=&v;v.layerNode.next=i==0?&nodes[m][r][1].layerNode:nullptr;}}}
 hook=[x, fired=false](string e) mutable {if(fired||x.trigger.empty()||e.rfind(x.trigger,0)!=0)return;fired=true;switch(x.change){case 1:g_AnmRenderManagerView=&managers[1];break;case 2:controller.vmIds[1]=2;break;case 3:g_MainSupervisorView.hintMode=2;break;case 4:controller.cursor.current=INT_MAX;break;case 5:controller.cursor.current=2;break;case 6:controller.stateTimer.current=7;break;}};
}
vector<string> finish(){hook=nullptr;auto& p=controller.cursor;trace.push_back("final:"+to_string(controller.screenState)+":"+to_string(controller.screen)+":"+to_string(controller.stateTimer.current)+":"+to_string(p.current)+":"+to_string(p.previous)+":"+to_string(p.count)+":"+to_string(p.saveDepth)+":"+to_string(p.disabledEntryCount)+":"+to_string(controller.vmIds[1].value)+":"+to_string(int(g_MainSupervisorView.bgmVolume))+":"+to_string(int(g_MainSupervisorView.sfxVolume))+":"+to_string(int(g_MainSupervisorView.hintMode))+":"+to_string(int(g_MainSupervisorView.colorMode)));
 for(int i=0;i<16;++i)trace.push_back("saved:"+to_string(p.savedCurrent[i])+":"+to_string(p.savedCount[i]));
 for(int m=0;m<2;++m){for(int r=0;r<2;++r){trace.push_back("pending-root:"+to_string(roots[m][r].pendingInterrupt));for(int i=0;i<2;++i)trace.push_back("pending-child:"+to_string(nodes[m][r][i].pendingInterrupt));}for(auto& v:resolved[m])trace.push_back("sprite-final:"+to_string(v.sprite));}return trace;}
int main(){vector<Case> cases;
 for(int state=0;state<=4;++state)for(int row=0;row<6;++row)for(int timer:{6,7,9,10})cases.push_back({"states",state,row,timer});
 for(int row=0;row<6;++row)for(int bits=0;bits<64;++bits)cases.push_back({"all-input-precedence",2,row,60,bits});
 for(int row:{1,2})for(int value=-128;value<128;++value)for(int bits:{4,8,12}){Case c{"signed-volume",2,row,1,bits};c.volume=value;cases.push_back(c);}
#ifndef SAFE_BASELINE
 for(int value=-128;value<128;++value)for(int bits:{4,8,12}){Case c{"all-color-bytes",2,0,1,bits};c.color=value;cases.push_back(c);}
#endif
 for(int count:{0,1,3,6}){Case c{"cancel-count-clamp",2,0,1,16};c.count=count;cases.push_back(c);}
 for(int row=0;row<6;++row)cases.push_back({"repeated-input",2,row,1,0,15});
 for(string trigger:{"input:4","input:8","now:1:3","sprite:","find:0:1","refresh"})for(int change:{1,2,3})for(int branch=0;branch<4;++branch){Case c{"callback-"+trigger+"-"+to_string(change)+"-"+to_string(branch)};c.trigger=trigger;c.change=change;c.state=branch==0?1:2;c.timer=7;c.row=branch==3?4:0;c.pressed=branch==1?4:branch==2?8:branch==3?32:0;cases.push_back(c);}
 for(int missing:{1,2}){Case c{"missing-child"};c.pressed=4;c.missing=missing;cases.push_back(c);}
 for(int state:{1,2})for(int change:{4,5}){
#ifdef SAFE_BASELINE
 if(change==4)continue; // Historical int addition overflows; semantic-only baseline excludes it.
#endif
 Case c{"post-immediate-cursor",state,0,7,state==2?16:0};c.trigger="now:1:3";c.change=change;cases.push_back(c);}
 Case fall{"initialize-fallthrough",0};fall.trigger="state:1";fall.change=6;cases.push_back(fall);
 int failed=0,canaryFailed=0;
 // Directly exercise the typed child VM helper, including lookup of a found ID
 // (the list node itself is deliberately different from the resolved VM).
 for(int missing:{0,1,2})for(int change:{0,1,2}){
  Case c{"typed-child-vm"};c.missing=missing;c.trigger="find:0:1";c.change=change;
  reset(c);auto* expectedVm=managers[0].FindVm(AnmVmIdView(targetChild(&managers[0],31)));auto expected=finish();
  reset(c);auto* actualVm=FindOptionsChildVm(&controller,&managers[0],31);auto actual=finish();
  if(expectedVm!=actualVm||expected!=actual){++failed;cerr<<"FAIL typed child VM missing="<<missing<<" change="<<change<<"\n";}
 }
 for(int scenario=0;scenario<3;++scenario){
  Case c{"deferred-helper"};
  auto prepare=[&](){reset(c);if(scenario==1)roots[0][0].layerNode.previous=&nodes[0][0][0].layerNode;if(scenario==2)managers[0].vms[1]=nullptr;};
  prepare();targetDeferred(1,37);auto expected=finish();prepare();FrontEndMarkVmInterrupt(&controller,1,37);auto actual=finish();
  if(expected!=actual){++failed;cerr<<"FAIL standalone deferred helper scenario="<<scenario<<"\n";}
 }
 for(auto& c:cases){reset(c);target();auto expected=finish();reset(c);int ret=controller.UpdateOptions(&controller);if(g_MainSupervisorView.colorMode!=85)++canaryFailed;auto actual=finish();if(expected!=actual||ret!=1){++failed;if(failed<=10){size_t i=0;while(i<expected.size()&&i<actual.size()&&expected[i]==actual[i])++i;cerr<<"FAIL "<<c.name<<" row="<<c.row<<" value="<<c.volume<<" color="<<c.color<<" inputs="<<c.pressed<<" event="<<i<<" expected="<<(i<expected.size()?expected[i]:"<end>")<<" actual="<<(i<actual.size()?actual[i]:"<end>")<<"\n";}}}
 cout<<"cases="<<cases.size()+12<<" passed="<<cases.size()+12-failed<<" functional_failures="<<failed<<" canary_failures="<<canaryFailed<<"\n";
 cout<<"Scope: extracted owner/helpers/cursor/input; synthetic target-order model, plain signed bytes and typed IDs; excludes ABI/native runtime, invalid null parents; creation/refresh/timer/sound/immediate callbacks mocked.\n";return failed?1:0;}
'''

def build_code(source,support_source,cursor_header):
    s=source.read_text();owner=extract(s,'FrontEndControllerView::UpdateOptions')
    # Discover only called helpers: historical owners include BeginOptionsClose;
    # lexical owners execute their close statements directly in the extracted owner.
    names=['ResolveVm','FindChildVmId','FindChildVm','SetChildSprite','FindOptionsChildId','FindOptionsChildVm','SetHintModeSprites','QueueOptionsSound','BeginOptionsClose']
    selected={};todo=owner
    while True:
        added=False
        for name in names:
            if name not in selected and re.search(r'\b'+name+r'\s*\(',mask(todo)):
                selected[name]=extract(s,name);todo+='\n'+selected[name];added=True
        if not added:break
    # Include and exercise FindOptionsChildVm even when only Refresh uses it.
    selected['FindOptionsChildVm']=extract(s,'FindOptionsChildVm')
    cursor=extract_struct(cursor_header.read_text(),'FrontEndCursorView')
    support=support_source.read_text() if support_source else s
    cursor_names=['FrontEndCursorSetCurrent','FrontEndCursorView::Push','FrontEndCursorView::Pop','FrontEndCursorView::Move','FrontEndInputView::IsRepeated']
    bodies='\n'.join(extract(s,n,optional=True) or extract(support,n) for n in cursor_names)
    helper=extract(s,'FrontEndMarkVmInterrupt')
    actual='\n'.join(selected[n] for n in names if n in selected)+'\n'+helper+'\n'+owner
    if re.search(r'\bg_MainSupervisorView\s*\.\s*colorMode\b',mask(actual)):
        raise ValueError('Options controller must use hintMode; colorMode is the independent D3D field')
    return PREFIX+cursor+bodies+FIXTURE,actual

def mutate_close_sound(actual):
    # Mutate the key-config close only, never the defaults/select sound. Match
    # either historical helper ownership or the current lexical sound/state tail.
    pattern=(r'\bBeginOptionsClose\(controller,\s*(?P<helper>FRONT_END_SOUND_SELECT)\s*\)'
             r'|\b(?:QueueOptionsSound|FrontEndPlaySound)\(\s*(?P<lexical>FRONT_END_SOUND_SELECT)\s*\);'
             r'\s*controller->SetScreenStateTarget\(FRONT_END_OPTIONS_CLOSING\)')
    hits=list(re.finditer(pattern,mask(actual)))
    if len(hits)!=1:
        raise ValueError(f'wrong-close-sound: expected one select-close site, got {len(hits)}')
    hit=hits[0];a,b=hit.span('helper' if hit.group('helper') else 'lexical')
    return actual[:a]+'FRONT_END_SOUND_CANCEL'+actual[b:]

def run(prefix,actual,safe=False,capture=False):
    with tempfile.TemporaryDirectory(prefix='options-controller-') as td:
        cpp=Path(td)/'test.cpp';exe=Path(td)/'test';cpp.write_text(prefix+actual+ORACLE)
        subprocess.run(['g++','-std=c++17','-O0','-fsanitize=undefined','-fno-sanitize-recover=undefined',*(['-DSAFE_BASELINE'] if safe else []),str(cpp),'-o',str(exe)],check=True)
        return subprocess.run([str(exe)],capture_output=capture,text=True)

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'src/FrontEnd.cpp');ap.add_argument('--support-source',type=Path,help='Explicit cursor/input implementation file for snippet tests');ap.add_argument('--cursor-header',type=Path,default=ROOT/'src/FrontEnd.hpp');ap.add_argument('--expect-failure',action='store_true');ap.add_argument('--mutation-controls',action='store_true');args=ap.parse_args()
    prefix,actual=build_code(args.source,args.support_source,args.cursor_header);r=run(prefix,actual,args.expect_failure,capture=True)
    print(r.stdout,end='');print(r.stderr,end='')
    if args.expect_failure:
        failures=re.search(r'functional_failures=(\d+)',r.stdout)
        if r.returncode!=1 or not failures or int(failures[1])==0:raise SystemExit('Expected semantic baseline failure, not success/crash')
        print('Expected historical semantic failures confirmed; unsafe baseline color-table inputs excluded.');return
    if r.returncode:raise SystemExit(r.returncode)
    if args.mutation_controls:
        sound_call='QueueOptionsSound' if 'QueueOptionsSound' in actual else 'FrontEndPlaySound'
        mutations={
          'wrong-config-field':actual.replace('g_MainSupervisorView.hintMode','g_MainSupervisorView.colorMode'),
          'wrong-close-sound':mutate_close_sound(actual),
          'unsigned-right-color-lost':actual.replace('static_cast<unsigned char>(g_MainSupervisorView.hintMode) < 2','g_MainSupervisorView.hintMode < 2'),
          'left-volume-wrap-first':actual.replace('if (g_MainSupervisorView.bgmVolume < 5)','g_MainSupervisorView.bgmVolume -= 5;\n                if (g_MainSupervisorView.bgmVolume < 0)').replace('g_MainSupervisorView.bgmVolume -= 5;\n                controller','g_MainSupervisorView.bgmVolume = g_MainSupervisorView.bgmVolume;\n                controller'),
          'cancel-skips-clamp':actual.replace('controller->cursor.SetCurrent(5);','controller->cursor.current = 5;'),
          'cancel-hardcoded-deferred':actual.replace('static_cast<short>(static_cast<short>(controller->cursor.current) + 7)','static_cast<short>(12)'),
          'reload-parent-manager':actual.replace('manager->FindVm(*parentId)','g_AnmRenderManagerView->FindVm(*parentId)'),
          'cache-root-id':actual.replace('manager->FindVm(*parentId)','manager->FindVm(AnmVmIdView(1))'),
          'resample-mode-between-labels':actual.replace('manager, 0x20).SetSprite(0x2f)', 'manager, 0x20).SetSprite(g_MainSupervisorView.hintMode == 0 ? 0x2f : 0x2d)'),
          'read-default-mode-before-refresh':actual.replace('controller->RefreshOptionsDisplay();\n                SetHintModeSprites(controller, manager);','SetHintModeSprites(controller, manager);\n                controller->RefreshOptionsDisplay();'),
          'cache-manager-before-left-getter':actual.replace('int leftRepeated = InputRepeated(FRONT_END_INPUT_LEFT);\n        AnmRenderManagerView *manager = g_AnmRenderManagerView;', 'AnmRenderManagerView *manager = g_AnmRenderManagerView;\n        int leftRepeated = InputRepeated(FRONT_END_INPUT_LEFT);'),
          'right-sound-after-pair':actual.replace(sound_call+'(FRONT_END_SOUND_MOVE);\n                SetHintModeSprites(controller, manager);','SetHintModeSprites(controller, manager);\n                '+sound_call+'(FRONT_END_SOUND_MOVE);'),
          'return-list-node-instead-of-relookup':actual.replace('return manager->FindVm(FindOptionsChildId(controller, manager, scriptIndex));', 'AnmVmView *root = manager->FindVm(controller->vmIds[FRONT_END_ROOT_VM]); for (AnmVmLayerNodeView *node = &root->layerNode; node; node = node->next) { AnmVmView *v = static_cast<AnmVmView *>(node->owner); if (v->scriptIndex == scriptIndex) return v; } return NULL;'),
          'drop-child-propagation':actual.replace('if (vm->layerNode.previous == NULL)','if (false)'),
        }
        for name,broken in mutations.items():
            if broken==actual:raise SystemExit('Mutation did not apply: '+name)
            r=run(prefix,broken,capture=True)
            failures=re.search(r'functional_failures=(\d+)',r.stdout)
            if r.returncode!=1 or not failures or int(failures[1])==0:raise SystemExit('Mutation failed to produce semantic failure: '+name+'\n'+r.stdout+r.stderr)
            canary=re.search(r'canary_failures=(\d+)',r.stdout)
            if name=='wrong-config-field' and (not canary or int(canary[1])==0):
                raise SystemExit('Wrong-field mutation did not corrupt the independent D3D canary')
            print('DETECTED '+name+' functional_failures='+failures[1]+' canary_failures='+canary[1])
if __name__=='__main__':main()
