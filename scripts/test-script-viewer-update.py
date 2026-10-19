#!/usr/bin/env python3
"""Exercise actual browser source against target-reviewed event fixtures.

Synthetic host sinks do not establish Windows/i386 ABI, runtime or exactness.
Native whole-owner comparison and target layout assertions are separate.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


def function(source, signature):
    start = source.index(signature)
    opening = source.index('{', start)
    depth = 1
    end = opening + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


class ScriptViewerUpdate(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_target_reviewed_paths(self):
        source = (ROOT / 'src/ScriptViewer.cpp').read_text()
        extracted = '\n'.join(function(source, name) for name in [
            'static inline void ResumeScriptViewerOwner',
            'static inline void SuspendScriptViewerOwner',
            '__declspec(noinline) void ScriptViewerSuspendPlayer',
            'void ScriptViewerView::ClearScriptNames',
            'static inline void SelectScriptViewerEntry',
            'static inline void RememberScriptViewerCursor',
            'int ScriptViewerView::Update()',
            'int __fastcall ScriptViewerUpdateCallback',
        ])
        program = r'''
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
#define __declspec(x)
#define __fastcall
using std::memset; using std::malloc; using std::free;
static std::vector<std::string> events;
namespace th10 { struct ChainElem {unsigned flags;}; }
struct ScriptViewerChainOwnerView { th10::ChainElem *update,*draw; };
struct Player : ScriptViewerChainOwnerView {};
struct BulletManagerView : ScriptViewerChainOwnerView {void ResetPool();};
struct ScriptViewerItemsView : ScriptViewerChainOwnerView {unsigned char slots[0x21cea0];};
struct FrontEndInputView {
 unsigned short raw=0,repeated=0,pressed=0,previousHeld=0,previousRaw=0,frameMask032=0;
 unsigned mask008=0;
 void UpdateDebugRepeat();
 int IsRepeated(unsigned short mask){return ((repeated|pressed)&mask)!=0;}
 bool IsActiveInline(unsigned short mask)const{return (repeated|pressed)&mask;}
};

struct FrontEndCursorView {
 int current=0,previous=0,count=0,wraps=0;
 void Move(int amount){current+=amount;}
};
struct Float3 {float x,y,z;};
struct EnemySpawnRequestView {Float3 position;int score,item,life,flags0,flags1,variables[8];};
struct Entry {const char *name;};
struct EclVmScriptDatabase {Entry *subroutines;};
struct EnemyManagerView : ScriptViewerChainOwnerView {EclVmScriptDatabase *scriptDatabase;void SuspendCallbacks();};
struct GuiView {};
struct ScriptViewerWorkerView {void Start(void *context,int (*entry)());};
struct ScriptViewerView {
 int state=0,scriptCount=0,value680=0; char **scriptNames=nullptr;
 FrontEndCursorView menu,files,entries; Float3 position{};
 unsigned flags=0; ScriptViewerWorkerView worker;
 void ClearScriptNames(); int Update();
};
using HANDLE=void *;
#define INVALID_HANDLE_VALUE ((HANDLE)-1)
struct WIN32_FIND_DATAA {char cFileName[260];};
static std::vector<std::string> directory;
static size_t findIndex; static int findFirstCount,findCloseCount;
static HANDLE FindFirstFileA(const char *pattern,WIN32_FIND_DATAA *out){
 assert(std::string(pattern)=="../../data/*.ecl");++findFirstCount;findIndex=0;
 if(directory.empty())return INVALID_HANDLE_VALUE;
 std::strcpy(out->cFileName,directory[0].c_str());return (HANDLE)1;
}
static int FindNextFileA(HANDLE search,WIN32_FIND_DATAA *out){
 assert(search==(HANDLE)1);
 if(++findIndex>=directory.size())return 0;
 std::strcpy(out->cFileName,directory[findIndex].c_str());return 1;
}
static int FindClose(HANDLE search){
 assert(search==(HANDLE)1||search==INVALID_HANDLE_VALUE);++findCloseCount;return 1;
}
static FrontEndInputView g_FrontEndInput;
void FrontEndInputView::UpdateDebugRepeat(){events.push_back("input");}
static th10::ChainElem tasks[8];
static Player player; static Player *g_Player=&player;
static BulletManagerView bullets; static BulletManagerView *g_ScriptViewerBullets=&bullets;
static ScriptViewerItemsView items; static ScriptViewerItemsView *g_ScriptViewerItems=&items;
static EnemyManagerView enemies; static EnemyManagerView *g_EnemyManager=&enemies;
static GuiView gui; static GuiView *g_GuiView=&gui;
static void *g_ScriptViewerBomb=(void *)2,*g_ScriptViewerPopups=(void *)3;
struct MainSupervisorView {void SetNextGameMode(int mode);};
static MainSupervisorView g_MainSupervisorView;
static ScriptViewerView *activeView;
static const char *spawnedEntry;
static EnemySpawnRequestView spawnedRequest;
static void RebuildPlayerOptions(Player *p){assert(p==g_Player);events.push_back("rebuild");}
static void PlayerResetRuntimeState(Player *p){assert(p==g_Player);events.push_back("player-reset");}
void BulletManagerView::ResetPool(){assert(this==g_ScriptViewerBullets);events.push_back("bullet-reset");}
static void EnemyManagerClear(EnemyManagerView *p){assert(p==g_EnemyManager);events.push_back("enemy-clear");}
void EnemyManagerView::SuspendCallbacks(){assert(this==g_EnemyManager);events.push_back("enemy-suspend");}
static void GameResetGui(GuiView *p){assert(p==g_GuiView);events.push_back("gui-reset");}
static void EnemySpawn(EnemyManagerView *p,const char *name,const EnemySpawnRequestView *r){
 assert(p==g_EnemyManager);events.push_back("spawn");spawnedEntry=name;spawnedRequest=*r;
}
void MainSupervisorView::SetNextGameMode(int mode){assert(this==&g_MainSupervisorView&&mode==3);events.push_back("mode3");}
static void ScriptViewerReleaseGui(GuiView *p){assert(p==g_GuiView);events.push_back("release-gui");}
static void ScriptViewerReleasePlayer(Player *p){assert(p==g_Player);events.push_back("release-player");}
static void ScriptViewerReleaseBullets(BulletManagerView *p){assert(p==g_ScriptViewerBullets);events.push_back("release-bullets");}
static void ScriptViewerReleaseBomb(void *p){assert(p==g_ScriptViewerBomb);events.push_back("release-bomb");}
static void ScriptViewerReleaseItems(ScriptViewerItemsView *p){assert(p==g_ScriptViewerItems);events.push_back("release-items");}
static void ScriptViewerReleasePopups(void *p){assert(p==g_ScriptViewerPopups);events.push_back("release-popups");}
static void ScriptViewerReleaseEnemies(EnemyManagerView *p){assert(p==g_EnemyManager);events.push_back("release-enemies");}
static int ScriptViewerLoadThread(){return 0;}
void ScriptViewerWorkerView::Start(void *context,int (*entry)()){
 assert(context==activeView&&this==&activeView->worker&&entry==ScriptViewerLoadThread);events.push_back("worker");
}
'''
        program += extracted + r'''
static void reset(){
 events.clear();g_FrontEndInput=FrontEndInputView();spawnedEntry=nullptr;
 for(auto &task:tasks)task.flags=4;
 player.update=&tasks[0];player.draw=&tasks[1];bullets.update=&tasks[2];bullets.draw=&tasks[3];
 enemies.update=&tasks[4];enemies.draw=&tasks[5];items.update=&tasks[6];items.draw=&tasks[7];
 memset(items.slots,0xa5,sizeof items.slots);
 findFirstCount=findCloseCount=0;
}
int main(){
 Entry names[]={{"SubA"},{"SubB"}};EclVmScriptDatabase db{names};enemies.scriptDatabase=&db;
 reset();ScriptViewerView idle;idle.state=3;
 assert(ScriptViewerUpdateCallback(&idle)==1&&idle.state==3&&events.empty());
 // Target state4 updates input before resuming and clearing managers.
 reset();ScriptViewerView running;running.state=4;
 for(auto &task:tasks)task.flags=6;
 g_FrontEndInput.previousRaw=0x20;g_FrontEndInput.raw=0x40;g_FrontEndInput.pressed=8;
 assert(running.Update()==1&&running.state==1);
 assert(g_FrontEndInput.previousHeld==0x20&&g_FrontEndInput.previousRaw==0x40&&g_FrontEndInput.frameMask032==8);
 assert(events==std::vector<std::string>({"input","player-reset","bullet-reset","enemy-clear"}));
 for(auto &task:tasks)assert(task.flags==4);
 assert(std::all_of(items.slots,items.slots+sizeof items.slots,[](unsigned char c){return c==0;}));
 reset();running.state=4;g_FrontEndInput.pressed=0;running.Update();
 assert(running.state==4&&events==std::vector<std::string>({"input"}));
 // Entry selection uses the selected name and clears the entire spawn packet.
 reset();ScriptViewerView spawn;spawn.state=1;spawn.menu.current=1;
 g_FrontEndInput.repeated=0x80;g_FrontEndInput.mask008=0x1001;
 assert(spawn.Update()==1&&spawn.state==4&&spawn.entries.current==1);
 assert(spawn.menu.previous==1&&spawn.entries.previous==0);
 assert(events==std::vector<std::string>({"rebuild","player-reset","bullet-reset","enemy-suspend","enemy-clear","gui-reset","spawn"}));
 assert(std::string(spawnedEntry)=="SubB");
 assert(spawnedRequest.position.x==0&&spawnedRequest.position.y==0&&spawnedRequest.position.z==0&&spawnedRequest.life==10000);
 assert(spawnedRequest.score==0&&spawnedRequest.item==0&&spawnedRequest.flags0==0&&spawnedRequest.flags1==0);
 for(int v:spawnedRequest.variables)assert(v==0);
 assert(tasks[0].flags==6&&tasks[1].flags==6&&tasks[2].flags==6&&tasks[3].flags==6&&tasks[6].flags==6&&tasks[7].flags==6);
 // The entry branch reads +8, whereas reload/quit read +6.
 reset();spawn.state=1;g_FrontEndInput.pressed=1;spawn.Update();
 assert(spawn.state==1&&events.empty());
 reset();g_EnemyManager=nullptr;spawn.Update();assert(events.empty());g_EnemyManager=&enemies;
 // Reload passes every current owner in target order, then starts the worker.
 reset();ScriptViewerView reload;activeView=&reload;reload.state=1;reload.flags=10;
 reload.files.current=2;reload.files.previous=-9;
 g_FrontEndInput.pressed=0x1001;reload.Update();
 assert(reload.state==2&&reload.flags==8);
 assert(reload.files.previous==2&&reload.files.current==2);
 assert(events==std::vector<std::string>({"release-gui","release-player","release-bullets","release-bomb","release-items","release-popups","release-enemies","worker"}));
 reset();reload.state=1;g_FrontEndInput.mask008=1;reload.Update();
 assert(reload.state==1&&events.empty());
 reset();ScriptViewerView quit;quit.state=1;quit.menu.current=2;g_FrontEndInput.pressed=1;quit.Update();
 assert(events==std::vector<std::string>({"mode3"}));
 // Empty search still closes INVALID_HANDLE_VALUE, matching target control flow.
 reset();directory.clear();ScriptViewerView empty;empty.Update();
 assert(findFirstCount==1&&findCloseCount==1&&empty.scriptCount==0&&empty.scriptNames==nullptr);
 assert(empty.menu.count==3&&empty.files.count==0&&empty.entries.count==1&&empty.state==1);
 assert(empty.menu.wraps==1&&empty.files.wraps==1&&empty.entries.wraps==1&&empty.position.y==32&&empty.value680==1000);
 reset();directory={"a.ecl","b.ecl"};ScriptViewerView listing;listing.Update();
 assert(findFirstCount==2&&findCloseCount==2&&listing.scriptCount==2);
 assert(std::string(listing.scriptNames[0])=="a.ecl"&&std::string(listing.scriptNames[1])=="b.ecl");
 listing.ClearScriptNames();assert(listing.scriptNames==nullptr);
 // Null callbacks are allowed only by the null-tolerant resume/suspend helper.
 ScriptViewerChainOwnerView missing{};ResumeScriptViewerOwner(&missing);SuspendScriptViewerOwner(&missing);
}
'''
        with tempfile.TemporaryDirectory(prefix='th10-script-viewer-') as tmp:
            path = Path(tmp) / 'fixture.cpp'
            binary = Path(tmp) / 'fixture'
            path.write_text(program)
            subprocess.run(['c++', '-std=c++17', '-O2', str(path), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    unittest.main()
