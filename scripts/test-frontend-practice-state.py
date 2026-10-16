#!/usr/bin/env python3
"""Extract the maintained practice owner and helpers into a mocked host contract.

Evidence scope
--------------
This is host semantic evidence only. It does not establish native register ABI,
original authorship, object layout, exact target bytes, PE build closure, actual
input/UI execution, storage ownership or portability. The target passes the
unlock profile receiver in EAX; this ordinary C++ signature does not prove the
compiler will reproduce that private ABI. Helper origin remains indeterminate.

The script extracts maintained cursor setter/Move/Pop, record-count, row-retire,
secret-sequence, unlock and UpdatePractice bodies. It consumes neither target
machine code nor decompilation. Animation, input, sound, state-transition and
refresh dependencies are mocks with deliberately callback-sensitive behavior.
The plain variant preserves each extracted body. The instrumented variant adds
only an unlock-entry observation. Mock dependencies are not verified game code.

Valid fixture domain
--------------------
Controller and profile storage must remain writable. Shot cursors are 0..5,
difficulty cursors 0..4, page cursors/counts bounded, and closing Pop starts at
saveDepth 1. Disabled cursor cycles are excluded. Keyboard buffers have 256
bytes; the 22-entry secret table maps to keys 0..255. Entry idle counters are
0..300, so timeout increments cannot overflow. Callback mutations preserve
these invariants and cannot invalidate storage. Negative secret progress and
INT_MIN are intentionally tested as terminal unsigned bit patterns, never as
valid key indices. On this 32-bit-int host, -1 also covers UINT_MAX's bit pattern.

The host VM slot array has 0xAD entries, matching the recovered minimum native
array extent through slot 0xAC. The native header partitions former anonymous
padding as vmIds[0xAD] plus unknown578[0x58], preserving difficultyAux at 0x5D0.
The simplified host controller still does not prove native physical layout.
Unlock requires a profile buffer through offset 0x1D891; the test scans every
byte of a larger sentinel buffer to verify precisely 160 modified bytes.

Negative controls
-----------------
A bounds-checked secret-key proxy reports a functional assertion before an
invalid index can touch memory. Historical signed-progress behavior must fail
normally with that exact assertion. Compile failure, signals/crashes and UBSan
diagnostics never count as a successfully rejected negative control.

Requires g++ with UBSan. Default execution runs plain and instrumented variants
plus twelve mutations. --work-dir retains generated sources, executables, logs
and JSON; otherwise temporary artifacts are removed. --skip-mutations runs only
the positive variants. Place this script under scripts/ for the default source
path, or pass --source explicitly.
"""

import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile


HARNESS = r'''
#include <algorithm>
#include <array>
#include <climits>
#include <cstring>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#define __stdcall
#define __fastcall
using std::string; using std::vector;
static vector<string> trace;
static std::function<void(const string&)> callback;
static void event(const string&s){trace.push_back(s);if(callback)callback(s);}
static void require(bool b,const string&m){if(!b)throw std::runtime_error(m);}
static void exact(vector<string> e){
#ifndef INSTRUMENTED
 e.erase(std::remove(e.begin(),e.end(),"unlock"),e.end());
#endif
 if(trace!=e){std::ostringstream o;o<<"trace mismatch\nexpected:";for(auto&s:e)o<<" "<<s;o<<"\nactual:";for(auto&s:trace)o<<" "<<s;throw std::runtime_error(o.str());}}
static int pos(const string&s){auto i=std::find(trace.begin(),trace.end(),s);require(i!=trace.end(),"missing "+s);return int(i-trace.begin());}
enum{FRONT_END_PRACTICE_INITIALIZE=0,FRONT_END_PRACTICE_OPENING=1,FRONT_END_PRACTICE_ACTIVE=2,FRONT_END_PRACTICE_CLOSING=3,FRONT_END_SCREEN_MAIN_MENU_RETURN=2,FRONT_END_RENDER_LAYER=15,FRONT_END_PRACTICE_RECORD_COUNT=110,FRONT_END_PRACTICE_ROWS_PER_PAGE=10,FRONT_END_INPUT_UP=0x10,FRONT_END_INPUT_DOWN=0x20,FRONT_END_INPUT_LEFT=0x40,FRONT_END_INPUT_RIGHT=0x80,FRONT_END_INPUT_CONFIRM=0x1001,FRONT_END_INPUT_CANCEL=0xa,FRONT_END_SOUND_MOVE=12,FRONT_END_SOUND_SELECT=10,FRONT_END_SOUND_CANCEL=11,FRONT_END_SOUND_UNLOCK=44};
struct FrontEndCursorView{int current=0,previous=0,count=0,savedCurrent[16]={},savedCount[16]={},saveDepth=0,disabledEntries[16]={},wraps=0,disabledEntryCount=0;void SetCurrent(int);int Move(int);void Pop();};
struct Timer{int current=0;void SetCurrent(int x){current=x;}};
struct AnmVmIdView{int value=0;operator int()const{return value;} AnmVmIdView&operator=(int i){value=i;return *this;}};
struct AnmVmView{};
struct Anm{int CreateVmVariant0(int script,int layer){require(layer==15,"layer");event("row-create:"+std::to_string(script));return script+1000;}};
struct Ascii{Anm*asciiAnm;}; Anm anm;Ascii ascii{&anm};Ascii*g_AsciiManagerView=&ascii;
struct Manager{bool exists=false;AnmVmView vm;AnmVmView*FindVm(int id){event("find:"+std::to_string(id));return exists?&vm:nullptr;}}manager;Manager*g_AnmRenderManagerView=&manager;
struct FrontEndControllerView{int screen=11,screenState=0;FrontEndCursorView cursor,practiceDifficultyCursor,practicePageCursor;Timer stateTimer;AnmVmIdView vmIds[0xad],difficultyAuxVmId,practiceRowVmIds[10];static int UpdatePractice(FrontEndControllerView*);int RefreshPracticeRecords(){event("refresh");return 1;}};
struct Input{unsigned int current=0;unsigned short repeated=0,pressed=0;}g_FrontEndInput;
int g_FrontEndPracticeSecretIdleFrames=0,g_FrontEndPracticeSecretProgress=0;
unsigned char g_FrontEndKeyboardState[256],g_FrontEndPreviousKeyboardState[256],g_FrontEndKeyboardTransitions[256],nextKeys[256];
signed char g_FrontEndPracticeDifficulties[110];
struct CheckedKeys{std::array<unsigned int,22> keys;unsigned int operator[](int i)const{require(i>=0&&i<22,"FUNCTIONAL_REJECTION: secret key index out of range "+std::to_string(i));event("key:"+std::to_string(i));return keys.at(i);}}g_FrontEndPracticeSecretKeys;
std::array<unsigned char,0x1d900> profileA,profileB;
unsigned char*g_FrontEndProfileData=profileA.data();
int readResult=1;
int FrontEndReadKeyboardState(unsigned char*p){event("keyboard-read");std::memcpy(p,nextKeys,256);return readResult;}
void FrontEndPlaySound(int i){event("sound:"+std::to_string(i));}

int InputRepeated(unsigned short m){event("input:"+std::to_string(m));return ((g_FrontEndInput.repeated|g_FrontEndInput.pressed)&m)!=0;}
void CreateVm(FrontEndControllerView*c,int i){event("create:"+std::to_string(i));c->vmIds[i]=1000+i;}
void DeleteVm(FrontEndControllerView*c,int i){event("delete:"+std::to_string(i));c->vmIds[i]=0;}
void SetScreenState(FrontEndControllerView*c,int i){event("state:"+std::to_string(i));c->screenState=i;c->stateTimer.SetCurrent(0);}
void SetScreen(FrontEndControllerView*c,int i){event("screen:"+std::to_string(i));c->screen=i;c->screenState=0;c->stateTimer.SetCurrent(0);}
void InterruptVmNow(FrontEndControllerView*,int i,short n){event("now:"+std::to_string(i)+":"+std::to_string(n));}
void InterruptVm(FrontEndControllerView*,int i,short n){event("interrupt:"+std::to_string(i)+":"+std::to_string(n));}
void InterruptVmId(int i,short n){event("id:"+std::to_string(i)+":"+std::to_string(n));}
AnmVmView*ResolveVm(AnmVmIdView*id){auto*p=manager.FindVm(*id);if(!p)id->value=0;return p;}
// SOURCE_INSERTION_POINT
void FrontEndCursorView::SetCurrent(int i){FrontEndCursorSetCurrent(i,this);}
static FrontEndControllerView reset(int state=2){callback={};trace.clear();manager.exists=false;g_FrontEndInput={};g_FrontEndPracticeSecretProgress=0;g_FrontEndPracticeSecretIdleFrames=0;readResult=1;memset(nextKeys,0,256);memset(g_FrontEndKeyboardState,0,256);memset(g_FrontEndPreviousKeyboardState,0,256);memset(g_FrontEndKeyboardTransitions,0,256);for(int i=0;i<22;++i)g_FrontEndPracticeSecretKeys.keys[i]=i+60;for(int i=0;i<110;++i)g_FrontEndPracticeDifficulties[i]=i%5;profileA.fill(0x55);profileB.fill(0xaa);g_FrontEndProfileData=profileA.data();FrontEndControllerView c;c.screenState=state;c.cursor.count=6;c.cursor.wraps=1;c.practiceDifficultyCursor.count=5;c.practiceDifficultyCursor.current=1;c.practiceDifficultyCursor.wraps=1;c.practicePageCursor.count=4;c.practicePageCursor.wraps=1;for(int i=0;i<10;++i)c.practiceRowVmIds[i]=2000+i;return c;}
static vector<string> idleInput(){return {"input:16","input:32","input:64","input:128"};}
static int tests=0;
static void test(const char*name,const std::function<void()>&fn){try{fn();++tests;std::cout<<"PASS "<<name<<"\n";}catch(const std::exception&e){std::cerr<<"FAIL "<<name<<": "<<e.what()<<"\n";throw;}}
int main(){try{
 test("initialization full order",[]{auto c=reset(0);require(c.UpdatePractice(&c)==1,"return");exact({"find:0","create:94","row-create:8","create:102","state:1","create:152","create:154","create:161","create:168","create:169","create:170","create:171","create:165","create:166","create:167","create:172"});require(c.screenState==1&&c.cursor.current==0&&c.cursor.count==6&&c.practiceDifficultyCursor.current==1&&c.practiceDifficultyCursor.count==5&&c.practicePageCursor.count==4,"init fields");});
 test("initial stale ID remains visible to create",[]{auto c=reset(0);c.vmIds[0x5e]=777;callback=[&](const string&s){if(s=="create:94")require(c.vmIds[0x5e].value==777,"stale ID cleared before create");};c.UpdatePractice(&c);});
 test("initial existing VM skips auxiliary",[]{auto c=reset(0);manager.exists=true;c.vmIds[0x5e]=999;c.UpdatePractice(&c);require(trace[0]=="find:999"&&trace[1]=="create:102","existing path");});
 test("initial final callback timer falls through despite changed state",[]{auto c=reset(0);callback=[&](const string&s){if(s=="create:172"){c.stateTimer.current=7;c.screenState=3;}};c.UpdatePractice(&c);require(c.screenState==2&&trace.back()=="state:2","init did not inspect callback timer");});
 test("opening boundaries",[]{for(int t:{-1,0,6,7}){auto c=reset(1);c.stateTimer.current=t;c.UpdatePractice(&c);require(c.screenState==(t>6?2:1),"opening threshold");exact(t>6?vector<string>{"state:2"}:vector<string>{});}});
 test("active idle snapshots and no extra work",[]{auto c=reset();c.cursor.current=3;c.cursor.previous=5;c.practiceDifficultyCursor.previous=4;c.practicePageCursor.current=2;c.practicePageCursor.previous=3;c.UpdatePractice(&c);exact(idleInput());require(c.cursor.previous==3&&c.practiceDifficultyCursor.previous==1&&c.practicePageCursor.previous==2,"previous snapshots");});
 test("difficulty callback reloads across sound delete refresh",[]{auto c=reset();std::fill(g_FrontEndPracticeDifficulties,g_FrontEndPracticeDifficulties+110,3);c.practicePageCursor.current=2;g_FrontEndInput.repeated=0x10;callback=[&](const string&s){if(s=="sound:12"){c.practiceDifficultyCursor.previous=3;c.practiceDifficultyCursor.current=2;}if(s=="delete:163")c.practiceDifficultyCursor.current=4;if(s=="refresh")c.practiceDifficultyCursor.current=3;};c.UpdatePractice(&c);exact({"input:16","now:170:2","input:32","sound:12","delete:163","create:164","refresh","input:64","input:128"});require(c.practicePageCursor.current==1&&c.practicePageCursor.count==12,"page reload");});
 test("main callback reloads",[]{auto c=reset();g_FrontEndInput.repeated=0x80;callback=[&](const string&s){if(s=="sound:12"){c.cursor.previous=1;c.cursor.current=3;}if(s=="delete:152")c.cursor.current=4;if(s=="create:153")c.cursor.previous=2;if(s=="delete:156")c.cursor.current=5;if(s=="create:159")c.practicePageCursor.current=2;};c.UpdatePractice(&c);exact({"input:16","input:32","input:64","input:128","now:169:2","sound:12","delete:152","create:153","delete:156","create:159","refresh"});});
 test("opposing inputs are independent ordered tests",[]{auto c=reset();g_FrontEndInput.repeated=0xf0;c.UpdatePractice(&c);exact({"input:16","now:170:2","input:32","now:171:2","input:64","now:168:2","input:128","now:169:2"});});
 test("confirm creates rows before page refresh sound",[]{auto c=reset();g_FrontEndInput.pressed=1;c.UpdatePractice(&c);auto e=idleInput();for(int i=0;i<10;++i)e.push_back("row-create:"+std::to_string(23+i));e.push_back("refresh");e.push_back("sound:10");exact(e);require(c.practicePageCursor.current==1,"page advanced");});
 test("confirm wraps page retires rows without clearing IDs",[]{auto c=reset();c.practicePageCursor.current=3;g_FrontEndInput.pressed=1;c.UpdatePractice(&c);auto e=idleInput();for(int i=0;i<10;++i)e.push_back("id:"+std::to_string(2000+i)+":1");e.push_back("sound:10");exact(e);for(int i=0;i<10;++i)require(c.practiceRowVmIds[i].value==2000+i,"row id cleared");});
 test("confirm and cancel coexist",[]{auto c=reset();g_FrontEndInput.pressed=3;c.UpdatePractice(&c);require(pos("sound:10")<pos("state:3")&&pos("state:3")<pos("sound:11")&&pos("sound:11")<pos("delete:161"),"confirm/cancel order");require(c.screenState==3&&c.practicePageCursor.current==1,"cancel state");require(trace.back()=="id:1032:1","last row retired");});
 test("confirm row callbacks affect subsequent page move",[]{auto c=reset();g_FrontEndInput.pressed=1;callback=[&](const string&s){if(s=="row-create:32")c.practicePageCursor.current=3;};c.UpdatePractice(&c);require(c.practicePageCursor.current==0,"cached page before creates");require(pos("row-create:32")<pos("id:1023:1")&&trace.back()=="sound:10","retire order");});
 test("cancel reloads cursors after sound and each delete",[]{auto c=reset();g_FrontEndInput.pressed=2;callback=[&](const string&s){if(s=="sound:11"){c.practiceDifficultyCursor.current=3;c.cursor.current=2;}if(s=="delete:163")c.cursor.current=4;if(s=="delete:158")c.cursor.current=5;};c.UpdatePractice(&c);require(pos("delete:163")<pos("delete:158")&&pos("delete:158")<pos("delete:153"),"cancel callback reloads");});
 test("closing exact order thresholds and cursor restore",[]{for(int t:{5,6,7}){auto c=reset(3);c.stateTimer.current=t;c.cursor.saveDepth=1;c.cursor.savedCurrent[0]=4;c.cursor.savedCount[0]=8;c.difficultyAuxVmId=501;c.UpdatePractice(&c);if(t<6)exact({});else{exact({"delete:102","interrupt:90:8","interrupt:91:8","delete:94","id:501:1","screen:2"});require(c.cursor.current==4&&c.cursor.count==8&&c.cursor.saveDepth==0&&c.screen==2&&c.screenState==0,"close restore");}}});
 test("secret unsigned terminal values",[]{for(int n:{-1,INT_MIN,22,23,INT_MAX}){auto c=reset();(void)c;g_FrontEndPracticeSecretProgress=n;callback=[&](const string&s){if(s=="sound:44")require(g_FrontEndPracticeSecretProgress==n,"progress reset before sound");};UpdatePracticeSecretSequence();exact({"keyboard-read","unlock","sound:44"});require(g_FrontEndPracticeSecretProgress==0&&g_FrontEndPracticeSecretIdleFrames==1,"terminal reset");}});
 test("secret last key unlocks only on following successful read",[]{auto c=reset();(void)c;g_FrontEndPracticeSecretProgress=21;nextKeys[81]=0x80;UpdatePracticeSecretSequence();exact({"keyboard-read","key:21"});require(g_FrontEndPracticeSecretProgress==22&&g_FrontEndPracticeSecretIdleFrames==1,"advance21");trace.clear();readResult=0;UpdatePracticeSecretSequence();exact({"keyboard-read"});require(g_FrontEndPracticeSecretProgress==22,"read failure consumed progress");trace.clear();readResult=1;UpdatePracticeSecretSequence();exact({"keyboard-read","unlock","sound:44"});});
 test("keyboard previous copy transitions and held keys",[]{auto c=reset();(void)c;g_FrontEndKeyboardState[60]=0x80;nextKeys[60]=0x80;g_FrontEndKeyboardState[120]=0x12;nextKeys[120]=0x91;callback=[](const string&s){if(s=="keyboard-read")require(g_FrontEndPreviousKeyboardState[120]==0x12,"copy must precede read");};UpdatePracticeSecretSequence();require(g_FrontEndPracticeSecretProgress==0&&g_FrontEndKeyboardTransitions[60]==0&&g_FrontEndKeyboardTransitions[120]==0x81,"transition formula");});
 test("wrong keys 0..56 reset but 57 does not",[]{for(int key:{0,56,57}){auto c=reset();(void)c;g_FrontEndPracticeSecretProgress=5;nextKeys[key]=0x80;UpdatePracticeSecretSequence();require(g_FrontEndPracticeSecretProgress==(key<57?0:5),"wrong key range");}});
 test("expected key takes priority over wrong key",[]{auto c=reset();(void)c;nextKeys[60]=0x80;nextKeys[1]=0x80;g_FrontEndPracticeSecretIdleFrames=300;UpdatePracticeSecretSequence();require(g_FrontEndPracticeSecretProgress==1&&g_FrontEndPracticeSecretIdleFrames==1,"expected branch priority/reset tick");});
 test("timeout strict greater and failure ticks",[]{for(int read:{0,1})for(int idle:{299,300}){auto c=reset();(void)c;readResult=read;g_FrontEndPracticeSecretProgress=5;g_FrontEndPracticeSecretIdleFrames=idle;UpdatePracticeSecretSequence();require(g_FrontEndPracticeSecretProgress==(idle==300?0:5)&&g_FrontEndPracticeSecretIdleFrames==(idle==300?0:300),"timeout");}});
 test("secret gated by both cursors and reset input before keyboard",[]{for(int shot:{1,2})for(int diff:{3,4}){auto c=reset();c.cursor.current=shot;c.practiceDifficultyCursor.current=diff;g_FrontEndPracticeSecretProgress=10;g_FrontEndPracticeSecretIdleFrames=50;g_FrontEndInput.pressed=0x400;callback=[](const string&s){if(s=="keyboard-read")require(g_FrontEndPracticeSecretProgress==0&&g_FrontEndPracticeSecretIdleFrames==0,"input reset precedes keyboard");};c.UpdatePractice(&c);require((std::find(trace.begin(),trace.end(),"keyboard-read")!=trace.end())==(shot==2&&diff==4),"gate");}});
 test("secret runs after confirm before cancel",[]{auto c=reset();c.cursor.current=2;c.practiceDifficultyCursor.current=4;g_FrontEndInput.pressed=3;c.UpdatePractice(&c);require(pos("sound:10")<pos("keyboard-read")&&pos("keyboard-read")<pos("state:3"),"secret order");});
 test("unlock exact 160-byte write set preserves full buffer",[]{auto c=reset();(void)c;FrontEndUnlockPracticeRecords(profileA.data());int writes=0;for(size_t i=0;i<profileA.size();++i){bool expected=(i>=0x1d882&&i<0x1d892);for(int g=0;g<6;++g)for(int row=0;row<24;++row)expected=expected||i==size_t(0x4e9+g*0x437c+row*8);require(profileA[i]==(expected?1:0x55),"unlock write set mismatch offset "+std::to_string(i));writes+=expected;}require(writes==160,"write count");});
 test("unlock uses fresh profile after keyboard callback and resets after sound",[]{auto c=reset();(void)c;g_FrontEndPracticeSecretProgress=22;callback=[](const string&s){if(s=="keyboard-read")g_FrontEndProfileData=profileB.data();if(s=="sound:44"){require(profileB[0x1d882]==1&&profileB[0x4e9]==1,"unlock not completed before sound");require(g_FrontEndPracticeSecretProgress==22,"reset too early");g_FrontEndPracticeSecretProgress=99;}};UpdatePracticeSecretSequence();require(g_FrontEndPracticeSecretProgress==0,"reset must overwrite sound callback");require(std::all_of(profileA.begin(),profileA.end(),[](unsigned char c){return c==0x55;}),"cached profile was modified");});
 std::cout<<"PASS total="<<tests<<" host semantic contract only; no native ABI or exactness proof\n";return 0;
}catch(...){return 1;}}
'''

variants = {'historical-stale-clear': ('if (g_AnmRenderManagerView->FindVm(\n'
                            '                controller->vmIds[0x5e]) == NULL)',
                            'if (ResolveVm(&controller->vmIds[0x5e]) == NULL)',
                            'stale ID cleared before create'),
 'historical-no-fallthrough': ('// Fall through: creation callbacks can advance the '
                               'opening timer.',
                               'break;',
                               'init did not inspect callback timer'),
 'historical-signed-progress': ('static_cast<unsigned '
                                'int>(g_FrontEndPracticeSecretProgress) >= 22u',
                                'g_FrontEndPracticeSecretProgress >= 22',
                                'FUNCTIONAL_REJECTION: secret key index out of range '
                                '-1'),
 'opening-inclusive': ('controller->stateTimer.current > 6',
                       'controller->stateTimer.current >= 6',
                       'opening threshold'),
 'timeout-inclusive': ('g_FrontEndPracticeSecretIdleFrames > 300',
                       'g_FrontEndPracticeSecretIdleFrames >= 300',
                       'timeout'),
 'wrong-key-inclusive': ('key < 57', 'key <= 57', 'wrong key range'),
 'unlock-short-first-set': ('entry < 16', 'entry < 15', 'unlock write set mismatch'),
 'unlock-short-group-set': ('group < 6', 'group < 5', 'unlock write set mismatch'),
 'unlock-wrong-stride': ('group * 0x437c +\n'
                         '                    (difficulty * 6 + stage) * 8',
                         'group * 0x437c +\n'
                         '                    (difficulty * 6 + stage) * 7',
                         'unlock write set mismatch'),
 'early-progress-reset': ('FrontEndUnlockPracticeRecords(g_FrontEndProfileData);\n'
                          '        FrontEndPlaySound(FRONT_END_SOUND_UNLOCK);',
                          'FrontEndUnlockPracticeRecords(g_FrontEndProfileData);\n'
                          '        g_FrontEndPracticeSecretProgress = 0;\n'
                          '        FrontEndPlaySound(FRONT_END_SOUND_UNLOCK);',
                          'progress reset before sound'),
 'cached-profile': ('static void UpdatePracticeSecretSequence()\n{',
                    'static void UpdatePracticeSecretSequence()\n'
                    '{\n'
                    '    unsigned char *cachedProfile = g_FrontEndProfileData;',
                    'unlock not completed before sound'),
 'cached-difficulty-previous': ('FrontEndPlaySound(FRONT_END_SOUND_MOVE);\n'
                                '            DeleteVm(controller, 0xa0 + '
                                'difficulty->previous);',
                                'int cachedPrevious = difficulty->previous;\n'
                                '            FrontEndPlaySound(FRONT_END_SOUND_MOVE);\n'
                                '            DeleteVm(controller, 0xa0 + '
                                'cachedPrevious);',
                                'trace mismatch')}

SIGNATURES = ['int __fastcall FrontEndCursorSetCurrent(',
 'void FrontEndCursorView::Pop()',
 'int FrontEndCursorView::Move(',
 'static int CountPracticeRecordsForDifficulty(',
 'static void InterruptPracticeRows(',
 'void FrontEndUnlockPracticeRecords(unsigned char *profile)',
 'static void UpdatePracticeSecretSequence()',
 'int __stdcall FrontEndControllerView::UpdatePractice(']


def extract(source, signature):
    start = source.index(signature)
    brace = source.index('{', start)
    depth = 1
    end = brace + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


def run(source, work, name, instrument):
    parts = [extract(source, sig) for sig in SIGNATURES]
    if instrument:
        parts = [part.replace('void FrontEndUnlockPracticeRecords(unsigned char *profile)\n{', 'void FrontEndUnlockPracticeRecords(unsigned char *profile)\n{\n    event("unlock");') for part in parts]
    cpp = work / (name + '.cpp')
    exe = work / name
    cpp.write_text(HARNESS.replace('// SOURCE_INSERTION_POINT', '\n\n'.join(parts)))
    cmd = ['g++', '-std=c++17', '-Wall', '-Wextra', '-Werror', '-Wno-implicit-fallthrough', '-O2', '-fsanitize=undefined', '-fno-sanitize-recover=all', str(cpp), '-o', str(exe)]
    if instrument:
        cmd.append('-DINSTRUMENTED')
    subprocess.run(cmd, check=True)
    result = subprocess.run([str(exe)], capture_output=True, text=True)
    output = result.stdout + result.stderr
    (work / (name + '.log')).write_text(output)
    return (result.returncode, output)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=Path(__file__).resolve().parents[1] / 'src' / 'FrontEnd.cpp')
    ap.add_argument('--work-dir', type=Path)
    ap.add_argument('--skip-mutations', action='store_true')
    args = ap.parse_args()
    source = args.source.read_text()
    temporary = tempfile.TemporaryDirectory(prefix='th10-practice-host-') if args.work_dir is None else None
    work = Path(temporary.name) if temporary else args.work_dir
    work.mkdir(parents=True, exist_ok=True)
    results = []
    for instrument in (False, True):
        name = 'instrumented' if instrument else 'plain'
        code, output = run(source, work, name, instrument)
        print(name + ': ' + output.strip().splitlines()[-1])
        if code:
            raise RuntimeError(output)
        results.append({'name': name, 'passed': True})
    if not args.skip_mutations:
        for name, (old, new, reason) in variants.items():
            if old not in source:
                raise RuntimeError('mutation anchor missing: ' + name)
            if name == 'opening-inclusive':
                begin = source.index(SIGNATURES[-1])
                mutant = source[:begin] + source[begin:].replace(old, new, 1)
            else:
                mutant = source.replace(old, new, 1)
            if name == 'cached-profile':
                mutant = mutant.replace('FrontEndUnlockPracticeRecords(g_FrontEndProfileData);', 'FrontEndUnlockPracticeRecords(cachedProfile);')
            code, output = run(mutant, work, name, True)
            # Compilation failure, crash, UBSan or an unexpected assertion is
            # never accepted as a successful negative-control rejection.
            if code != 1 or reason not in output or 'runtime error:' in output:
                raise RuntimeError('invalid control ' + name + ': ' + output)
            results.append({'name': name, 'rejected': True, 'reason': reason})
            print('REJECTED ' + name + ': ' + reason)
    report = {'source_sha256': hashlib.sha256(args.source.read_bytes()).hexdigest(), 'semantic_host_only': True, 'results': results}
    (work / 'results.json').write_text(json.dumps(report, indent=2) + '\n')
    print('PASS host contract; source SHA256 ' + report['source_sha256'])
    if temporary:
        temporary.cleanup()


if __name__ == "__main__":
    main()
