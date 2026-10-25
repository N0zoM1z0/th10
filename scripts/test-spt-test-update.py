#!/usr/bin/env python3
"""Extracted SptTest update/factory contracts, not a native/runtime oracle.

Dependencies and layouts are test doubles. Factory constructor supplies a valid
context solely to exercise field-write ordering; production initialization is
NOT inferred. Null Run and failed allocation/enumeration paths are source-only.
"""
from pathlib import Path
import os
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


def sources():
    s = (ROOT / "src/SptTest.cpp").read_text()
    update = s[s.index("static inline void SelectScriptViewerEntry"):]
    s = (ROOT / "src/Enemy.cpp").read_text()
    factory = s[s.index("EnemyEclHostBaseView *EclVmScriptDatabase::CreateHost"):]
    return update, factory


def source_contracts(update, factory):
    # Do not execute the target's null-this or uninitialized-context hazards.
    run = update.index("reinterpret_cast<EclVmHost *>(activeVm)->Run(1.0f)")
    assert run < update.index("if (!activeVm")
    start = update.index("worker.Start(this, SptTestLoadCallback)")
    assert start < update.index("state = 2", start)
    assert update.count("static_cast<unsigned short>(g_FrontEndInput.IsRepeated") == 2
    assert update.count('FindFirstFileA("../../data/*.ecl"') == 2
    assert update.count("malloc(") == 2
    enumeration = update[update.index("case 0:\n        ClearScriptNames()"): ]
    assert enumeration.count("if (") == 3  # No added allocation/second-pass guards.
    assert "ResetEclState" not in factory and "activeEclContext =" not in factory
    assert factory.count("host->activeEclContext->") == 2
    assert "if (" not in factory


PRELUDE = r"""
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <string>
#include <vector>
#define __fastcall
struct SptTestView;
static std::vector<std::string> events;
static std::function<void()> onRun, onMove, onRepeat, onFind, onLookup;
static SptTestView *g_SptTestView;
struct Context {void *currentInstruction=nullptr; int value00=91;};
struct EnemyEclResourceView {};
struct EnemyEclInstructionView {};
struct EnemyEclHostBaseView;
static EnemyEclHostBaseView *lastCreated;
struct EnemyEclHostBaseView {
 Context own, *activeEclContext=&own;
 EnemyEclResourceView *scriptDatabase=nullptr;
 EnemyEclHostBaseView(){lastCreated=this;events.push_back("new");}
 ~EnemyEclHostBaseView(){events.push_back("delete");}
 void Run(float dt){assert(dt==1.0f);events.push_back("run");if(onRun)onRun();}
};
using EclVmHost=EnemyEclHostBaseView;
struct Entry {const char *name;};
struct EclVmScriptDatabase {
 Entry *subroutines;
 EnemyEclHostBaseView *CreateHost(const char *);
 void *FindSubroutine(const char *name){events.push_back(std::string("lookup:")+name);if(onLookup)onLookup();return reinterpret_cast<void*>(0x1234);}
};
struct FrontEndCursorView {
 int current=0,previous=17,count=0,wraps=0;
 void Move(int n){current+=n;events.push_back("move");if(onMove)onMove();}
};
struct Input {
 unsigned pressed=0,mask008=0,active=0;int repeated80=0,repeated40=0;
 bool IsActiveInline(unsigned m){events.push_back("active:"+std::to_string(m));return (active&m)!=0;}
 int IsRepeated(unsigned m){events.push_back("repeat:"+std::to_string(m));int r=m==0x80?repeated80:repeated40;if(onRepeat)onRepeat();return r;}
} g_FrontEndInput;
struct Supervisor {int mode=-1;void SetNextGameMode(int n){mode=n;events.push_back("quit");}} g_MainSupervisorView;
static bool synchronous=false;
static void *workerContext;static int (*workerCallback)();
int SptTestLoadCallback();
struct Worker {void Start(void *c,int(*f)()){workerContext=c;workerCallback=f;events.push_back("start");if(synchronous)f();}};
struct Position {float x=9,y=9,z=9;};
struct SptTestView {
 int state=0,scriptCount=0;char **scriptNames=nullptr;
 FrontEndCursorView menu,files,entries;Worker worker;Position position;
 int value2D0=0;unsigned flags2D4=7;
 EclVmScriptDatabase *scriptDatabase=nullptr;EnemyEclHostBaseView *activeVm=nullptr;
 int Update();void ClearScriptNames(){events.push_back("clear");}
};
int SptTestLoadCallback(){assert(g_SptTestView);g_SptTestView->state=1;events.push_back("load");return 0;}
struct WIN32_FIND_DATAA {char cFileName[64];};
using HANDLE=int;
const HANDLE INVALID_HANDLE_VALUE=-1;
static std::vector<std::string> listing;
static int pass=0,indexInPass=0,closes=0;
HANDLE FindFirstFileA(const char *pattern,WIN32_FIND_DATAA *f){
 assert(std::string(pattern)=="../../data/*.ecl");++pass;indexInPass=0;
 events.push_back("first");if(onFind)onFind();
 if(listing.empty())return INVALID_HANDLE_VALUE;
 std::strcpy(f->cFileName,listing[0].c_str());return pass;
}
bool FindNextFileA(HANDLE h,WIN32_FIND_DATAA *f){
 assert(h>0);events.push_back("next");
 if(++indexInPass>=int(listing.size()))return false;
 std::strcpy(f->cFileName,listing[indexInPass].c_str());return true;
}
void FindClose(HANDLE h){++closes;events.push_back(h==-1?"close-invalid":"close");}
"""

MAIN = r"""
static void reset(){
 events.clear();onRun={};onMove={};onRepeat={};onFind={};onLookup={};
 g_FrontEndInput={};g_MainSupervisorView.mode=-1;synchronous=false;
 workerContext=nullptr;workerCallback=nullptr;listing.clear();pass=closes=0;
}
static void tick(SptTestView &v){assert(SptTestUpdateCallback(&v)==1);}
int main(){
 for(int state:{-1,2,3,5,2147483647}){reset();SptTestView v;v.state=state;tick(v);assert(v.state==state&&events.empty());}
 for(bool nonempty:{false,true}){
  reset();SptTestView v;if(nonempty)listing={"first.ecl","second.ecl"};tick(v);
  assert(v.state==1&&v.scriptCount==(nonempty?2:0)&&pass==(nonempty?2:1)&&closes==pass);
  assert(events.front()=="clear"&&v.menu.count==3&&v.files.count==v.scriptCount&&v.entries.count==1);
  assert(v.menu.current==0&&v.files.current==0&&v.entries.current==0);
  assert(v.menu.wraps==1&&v.files.wraps==1&&v.entries.wraps==1);
  assert(v.position.x==0&&v.position.y==32&&v.position.z==0&&v.value2D0==1000);
  if(nonempty){assert(std::string(v.scriptNames[0])=="first.ecl"&&std::string(v.scriptNames[1])=="second.ecl");for(int i=0;i<2;++i)free(v.scriptNames[i]);free(v.scriptNames);}
  else assert(events.back()=="close-invalid");
 }
 // Independent input reads: menu Move mutates the next direction and selection.
 {reset();SptTestView v;v.state=1;g_FrontEndInput.active=0x20;int moves=0;
 onMove=[&](){++moves;g_FrontEndInput.active=0x10;};
 tick(v);assert(moves==2&&v.menu.current==0&&v.menu.previous==0);}
 // Start may invoke the global loader synchronously, but the caller writes 2 last.
 {reset();SptTestView v,other;v.state=1;other.state=8;g_SptTestView=&other;
 synchronous=true;g_FrontEndInput.pressed=1;tick(v);
 assert(workerContext==&v&&workerCallback==SptTestLoadCallback);
 assert(v.state==2&&other.state==1&&v.flags2D4==5);}
 {reset();SptTestView v;v.state=1;v.menu.current=2;
 g_FrontEndInput.mask008=1;tick(v);assert(g_MainSupervisorView.mode==-1);
 g_FrontEndInput.pressed=0x1000;tick(v);assert(g_MainSupervisorView.mode==3);}
 Entry entries[]={{"zero"},{"one"}};EclVmScriptDatabase db{entries};
 // 0x10000 is false after ushort truncation; -1 is true. Second read is fresh.
 {reset();SptTestView v;v.state=1;v.menu.current=1;v.entries.current=1;v.scriptDatabase=&db;
 g_FrontEndInput.repeated80=0x10000;
 onRepeat=[&](){g_FrontEndInput.repeated40=-1;};
 tick(v);assert(v.entries.current==0&&v.entries.previous==1);}
 {reset();SptTestView v;v.state=1;v.menu.current=1;v.scriptDatabase=&db;
 v.activeVm=new EnemyEclHostBaseView;events.clear();g_FrontEndInput.mask008=1;tick(v);
 assert(v.state==4&&v.activeVm&&v.activeVm->scriptDatabase==reinterpret_cast<EnemyEclResourceView*>(&db));
 assert(v.activeVm->activeEclContext->currentInstruction==reinterpret_cast<void*>(0x1234)&&v.activeVm->activeEclContext->value00==0);
 auto deletion=std::find(events.begin(),events.end(),"delete"),creation=std::find(events.begin(),events.end(),"new");assert(deletion<creation);
 delete v.activeVm;}
 // Run can replace the owner's VM. The subsequent tests/delete must reread it.
 {reset();SptTestView v;v.state=4;auto *original=new EnemyEclHostBaseView;auto *replacement=new EnemyEclHostBaseView;
 original->own.currentInstruction=reinterpret_cast<void*>(1);v.activeVm=original;
 onRun=[&](){v.activeVm=replacement;};events.clear();tick(v);
 assert(v.state==1&&v.activeVm==nullptr&&events==std::vector<std::string>({"run","delete"}));delete original;}
 {reset();SptTestView v;v.state=4;auto *original=new EnemyEclHostBaseView;v.activeVm=original;
 onRun=[&](){v.activeVm=nullptr;};events.clear();tick(v);
 assert(v.state==1&&v.activeVm==nullptr&&events==std::vector<std::string>({"run"}));delete original;}
 {reset();SptTestView v;v.state=4;v.activeVm=new EnemyEclHostBaseView;
 Context other;other.currentInstruction=reinterpret_cast<void*>(1);
 onRun=[&](){v.activeVm->activeEclContext=&other;};tick(v);assert(v.state==4);delete v.activeVm;}
 // Factory's second context access must reload after FindSubroutine.
 {reset();Context replacement;onLookup=[&](){lastCreated->activeEclContext=&replacement;};
 auto *host=db.CreateHost("one");assert(host->own.value00==91&&replacement.value00==0);delete host;}
}
"""


class UpdateContracts(unittest.TestCase):
    def test_source_hazards_and_mutation_rejections(self):
        update, factory = sources()
        source_contracts(update, factory)
        mutations = [
            (update.replace("Run(1.0f)", "Run(0.0f)"), factory),
            (update.replace("state = 2", "state = 1"), factory),
            (update.replace("static_cast<unsigned short>", "static_cast<unsigned int>"), factory),
            (update, factory.replace("new EnemyEclHostBaseView;", "new EnemyEclHostBaseView; host->ResetEclState();")),
        ]
        for u, f in mutations:
            with self.assertRaises((AssertionError, ValueError)):
                source_contracts(u, f)

    def test_extracted_update_and_factory(self):
        update, factory = sources()
        source_contracts(update, factory)
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory)
            (p / "test.cpp").write_text("#include <algorithm>\n" + PRELUDE + update + factory + MAIN)
            subprocess.run(["c++", "-std=c++17", "-O1", "-g",
                            "-fsanitize=address,undefined", "-fno-omit-frame-pointer", "-fno-pie", "-no-pie",
                            "-fno-sanitize-recover=all",
                            str(p / "test.cpp"), "-o", str(p / "test")],
                           check=True, timeout=60)
            subprocess.run([str(p / "test")], check=True, timeout=20,
                           env={**os.environ, "ASAN_OPTIONS": "detect_leaks=1:halt_on_error=1",
                                "UBSAN_OPTIONS": "halt_on_error=1"})


if __name__ == "__main__":
    unittest.main()
