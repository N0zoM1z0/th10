#!/usr/bin/env python3
"""Exercise actual lifecycle bodies against TH10-reviewed event contracts.

Host dependency/member sinks isolate ordering, pointer capture and ownership.
They are not Windows/i386 runtime, layout, EH or exact-codegen evidence.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


def body(path, signature):
    source = (ROOT / path).read_text()
    start = source.index(signature)
    end, depth = source.index('{', start) + 1, 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


class ScriptViewerLifecycle(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_actual_lifecycle_order(self):
        program = r'''
#include <cassert>
#include <string>
#include <vector>
#include <cstdlib>
#include <new>
#define __declspec(x)
using std::string;using std::vector;
static vector<string> events;
static vector<void *> releasedVertices;
static void ReleaseVertexBuffer(void *buffer){releasedVertices.push_back(buffer);}
struct PlayerDrawVmView {
 unsigned prefix=42;void *ownedData358;
 explicit PlayerDrawVmView(void *buffer):ownedData358(buffer){}
 ~PlayerDrawVmView();
};
static void record(const string &event){events.push_back(event);}
struct Allocation {void *pointer;const char *event;};
static Allocation allocations[64]{};
template<class T>T *own(T *pointer,const char *event){
 for(auto &slot:allocations)if(!slot.pointer){slot={pointer,event};return pointer;}
 assert(false);return nullptr;
}
void *operator new(std::size_t size){if(void *p=std::malloc(size?size:1))return p;throw std::bad_alloc();}
static void release(void *pointer){
 const char *event=nullptr;
 for(auto &slot:allocations)if(pointer&&slot.pointer==pointer){event=slot.event;slot.pointer=nullptr;break;}
 if(event)record(event);std::free(pointer);
}
void operator delete(void *p)noexcept{release(p);}
void operator delete(void *p,std::size_t)noexcept{release(p);}
namespace th10 {
 struct ChainElem {int id;};
 struct Chain {static void UnregisterElem(ChainElem *,Chain *);};
 static Chain chainA,chainB;static Chain *g_Chain=&chainA;
}
struct MainSupervisorView {int criticalSections[1];unsigned char criticalSectionLockCounts[1];};
static MainSupervisorView g_MainSupervisorView;
struct ScriptViewerView;struct ScriptViewerBombOwnerView;struct ScriptViewerPopupOwnerView;struct BulletManagerView;
struct GuiView {~GuiView()noexcept(false);};
struct Player {~Player();};
struct EnemyManagerView {~EnemyManagerView();};
struct ItemManagerView {~ItemManagerView();};
static GuiView *g_GuiView;
static Player *g_Player;
static EnemyManagerView *g_EnemyManager;
static ItemManagerView *g_ItemManager;
static BulletManagerView *g_ScriptViewerBullets;
static void *g_ScriptViewerBomb,*g_ScriptViewerPopups;
static ScriptViewerView *g_ScriptViewer;
static bool markedBullets,browserVmDestroyed;
struct Vm {
 string label;int bulletIndex=-1;
 ~Vm(){
  if(bulletIndex>=0){assert(g_ScriptViewerBullets==nullptr&&markedBullets);record("bullet-vm"+std::to_string(bulletIndex));}
  else if(label=="popup-vm"){assert(g_ScriptViewerPopups==nullptr);record(label);}
  else if(label=="browser-vm"){assert(g_ScriptViewer==nullptr);browserVmDestroyed=true;record(label);}
 }
};
struct ScriptViewerWorkerView {
 virtual ~ScriptViewerWorkerView();
 void Stop(){assert(g_ScriptViewer==nullptr&&browserVmDestroyed);record("worker-stop");}
};
struct BulletRuntimeView {Vm vm;};
struct AnmLoadedView {};
struct BulletManagerView {
 th10::ChainElem *updateTask=nullptr,*drawTask=nullptr;
 BulletRuntimeView bullets[2001];AnmLoadedView *bulletAnm=nullptr;
 ~BulletManagerView();
};
struct ScriptViewerBombOwnerView {
 th10::ChainElem *updateTask=nullptr,*drawTask=nullptr;
 ~ScriptViewerBombOwnerView();
};
struct ScriptViewerPopupOwnerView {
 th10::ChainElem *updateTask=nullptr,*drawTask=nullptr;Vm vm;
 ~ScriptViewerPopupOwnerView();
};
struct ScriptViewerView {
 th10::ChainElem *updateTask=nullptr,*drawTask=nullptr;
 ScriptViewerWorkerView worker;Vm vm;
 ~ScriptViewerView();
 void ClearScriptNames(){assert(g_ScriptViewer==this);record("names");}
};
struct AnmRenderManagerView {
 void MarkLoadedVmsForDeletion(AnmLoadedView *resource){
  assert(g_ScriptViewerBullets&&resource==g_ScriptViewerBullets->bulletAnm);
  assert(!markedBullets);markedBullets=true;record("mark-bullets");
 }
};
static AnmRenderManagerView renderer;static AnmRenderManagerView *g_AnmRenderManagerView=&renderer;
static th10::Chain *capturedBeforeEnter;
static unsigned char depthBeforeEnter;
static th10::ChainElem replacement{3};
static void EnterCriticalSection(int *section){
 assert(section==&g_MainSupervisorView.criticalSections[0]);record("enter");
 capturedBeforeEnter=th10::g_Chain;depthBeforeEnter=g_MainSupervisorView.criticalSectionLockCounts[0];
 th10::g_Chain=(th10::g_Chain==&th10::chainA)?&th10::chainB:&th10::chainA;
}
static void LeaveCriticalSection(int *section){
 assert(section==&g_MainSupervisorView.criticalSections[0]);
 assert(g_MainSupervisorView.criticalSectionLockCounts[0]==static_cast<unsigned char>(depthBeforeEnter+1));record("leave");
}
void th10::Chain::UnregisterElem(ChainElem *task,Chain *chain){
 assert(chain==capturedBeforeEnter&&chain!=g_Chain);
 assert(g_MainSupervisorView.criticalSectionLockCounts[0]==static_cast<unsigned char>(depthBeforeEnter+1));
 record("remove"+std::to_string(task->id));
 if(task->id==1){assert(g_ScriptViewer);g_ScriptViewer->drawTask=&replacement;}
}
static bool failGui;
GuiView::~GuiView()noexcept(false){record("gui");if(failGui)throw 42;g_GuiView=nullptr;}
Player::~Player(){record("player");g_Player=nullptr;}
EnemyManagerView::~EnemyManagerView(){record("enemies");g_EnemyManager=nullptr;}
ItemManagerView::~ItemManagerView(){record("items");g_ItemManager=nullptr;}
'''
        program += '\n#define free ReleaseVertexBuffer\n'
        program += body('src/Player.cpp', 'PlayerDrawVmView::~PlayerDrawVmView()')
        program += '\n#undef free\n'
        program += body('src/ScriptViewerOwners.hpp', 'template <class T>')
        for path, signature in [
            ('src/ScriptViewer.cpp', 'ScriptViewerView::~ScriptViewerView()'),
            ('src/ScriptViewerWorker.cpp', 'ScriptViewerWorkerView::~ScriptViewerWorkerView()'),
            ('src/ScriptViewerLifecycle.cpp', 'ScriptViewerBombOwnerView::~ScriptViewerBombOwnerView()'),
            ('src/ScriptViewerLifecycle.cpp', 'ScriptViewerPopupOwnerView::~ScriptViewerPopupOwnerView()'),
            ('src/ScriptViewerLifecycle.cpp', 'BulletManagerView::~BulletManagerView()'),
            *[('src/ScriptViewerLifecycle.cpp', '__declspec(noinline) void ScriptViewerRelease' + name + '(')
              for name in ['Gui', 'Player', 'Bullets', 'Bomb', 'Items', 'Popups', 'Enemies']],
        ]:
            program += body(path, signature) + '\n'
        program += r'''
static void appendRemoval(vector<string> &expected,int id){
 expected.push_back("enter");expected.push_back("remove"+std::to_string(id));expected.push_back("leave");
}
static void reset(){
 for(auto &slot:allocations)assert(slot.pointer==nullptr);
 events.clear();th10::g_Chain=&th10::chainA;g_MainSupervisorView.criticalSectionLockCounts[0]=255;
 markedBullets=false;browserVmDestroyed=false;
}
int main(){
 for(void *buffer:{static_cast<void *>(nullptr),static_cast<void *>(&events)}){
  releasedVertices.clear();delete new PlayerDrawVmView(buffer);
  assert(releasedVertices==(buffer?vector<void *>({buffer}):vector<void *>()));
 }
 releasedVertices.clear();try{PlayerDrawVmView member(&events);throw 1;}catch(int){}
 assert(releasedVertices==vector<void *>({&events}));
 reset();ScriptViewerReleaseGui(nullptr);ScriptViewerReleasePlayer(nullptr);
 ScriptViewerReleaseBullets(nullptr);ScriptViewerReleaseBomb(nullptr);
 ScriptViewerReleaseItems(nullptr);ScriptViewerReleasePopups(nullptr);ScriptViewerReleaseEnemies(nullptr);
 assert(events.empty());
 g_GuiView=own(new GuiView,"free-gui");g_Player=own(new Player,"free-player");g_EnemyManager=own(new EnemyManagerView,"free-enemies");g_ItemManager=own(new ItemManagerView,"free-items");
 ScriptViewerReleaseGui(g_GuiView);ScriptViewerReleasePlayer(g_Player);
 ScriptViewerReleaseEnemies(g_EnemyManager);ScriptViewerReleaseItems(g_ItemManager);
 assert(events==vector<string>({"gui","free-gui","player","free-player","enemies","free-enemies","items","free-items"}));
 events.clear();auto *failed=own(new GuiView,"free-gui");g_GuiView=failed;failGui=true;
 try{ScriptViewerReleaseGui(failed);assert(false);}catch(int value){assert(value==42);}
 assert(events==vector<string>({"gui"})&&g_GuiView==failed);
 failGui=false;g_GuiView=nullptr;::operator delete(failed);
 assert(events==vector<string>({"gui","free-gui"}));
 // Null managers/tasks still destroy browser VM before the worker, after publication clears.
 reset();auto *empty=own(new ScriptViewerView,"free-browser");g_ScriptViewer=empty;empty->vm.label="browser-vm";delete empty;
 assert(events==vector<string>({"names","browser-vm","worker-stop","free-browser"}));
 assert(g_MainSupervisorView.criticalSectionLockCounts[0]==255);
 // Actual browser and three maintained manager destructor bodies are composed.
 reset();th10::ChainElem browserTasks[]={{1},{2}},bulletTask{11},bombTasks[]={{21},{22}},popupTask{31};
 auto *view=own(new ScriptViewerView,"free-browser");g_ScriptViewer=view;view->vm.label="browser-vm";
 view->updateTask=&browserTasks[0];view->drawTask=&browserTasks[1];
 g_GuiView=own(new GuiView,"free-gui");g_Player=own(new Player,"free-player");g_EnemyManager=own(new EnemyManagerView,"free-enemies");g_ItemManager=own(new ItemManagerView,"free-items");
 auto *bullets=own(new BulletManagerView,"free-bullets");g_ScriptViewerBullets=bullets;bullets->updateTask=&bulletTask;
 AnmLoadedView resource;bullets->bulletAnm=&resource;
 for(int i=0;i<2001;++i)bullets->bullets[i].vm.bulletIndex=i;
 auto *bomb=own(new ScriptViewerBombOwnerView,"free-bomb");g_ScriptViewerBomb=bomb;bomb->updateTask=&bombTasks[0];bomb->drawTask=&bombTasks[1];
 auto *popup=own(new ScriptViewerPopupOwnerView,"free-popup");g_ScriptViewerPopups=popup;popup->updateTask=&popupTask;popup->vm.label="popup-vm";
 delete view;
 vector<string> expected={"names"};appendRemoval(expected,1);appendRemoval(expected,3);
 for(const char *s:{"gui","free-gui","player","free-player"})expected.push_back(s);
 appendRemoval(expected,11);expected.push_back("mark-bullets");
 for(int i=2000;i>=0;--i)expected.push_back("bullet-vm"+std::to_string(i));
 for(const char *s:{"free-bullets","enemies","free-enemies"})expected.push_back(s);
 appendRemoval(expected,21);appendRemoval(expected,22);expected.push_back("free-bomb");
 expected.push_back("items");expected.push_back("free-items");appendRemoval(expected,31);
 for(const char *s:{"popup-vm","free-popup","browser-vm","worker-stop","free-browser"})expected.push_back(s);
 assert(events==expected&&g_MainSupervisorView.criticalSectionLockCounts[0]==255);
 // All independently nullable callbacks; no owner pointer is cleared before unlinking.
 th10::ChainElem tasks[]={{41},{42}};
 for(int mask=0;mask<4;++mask){
  reset();auto *object=own(new ScriptViewerBombOwnerView,"free-bomb");g_ScriptViewerBomb=object;
  object->updateTask=(mask&1)?&tasks[0]:nullptr;object->drawTask=(mask&2)?&tasks[1]:nullptr;
  ScriptViewerReleaseBomb(object);expected.clear();if(mask&1)appendRemoval(expected,41);if(mask&2)appendRemoval(expected,42);expected.push_back("free-bomb");
  assert(events==expected&&g_MainSupervisorView.criticalSectionLockCounts[0]==255);
  reset();auto *p=own(new ScriptViewerPopupOwnerView,"free-popup");g_ScriptViewerPopups=p;p->vm.label="popup-vm";
  p->updateTask=(mask&1)?&tasks[0]:nullptr;p->drawTask=(mask&2)?&tasks[1]:nullptr;
  ScriptViewerReleasePopups(p);expected.pop_back();expected.push_back("popup-vm");expected.push_back("free-popup");
  assert(events==expected&&g_MainSupervisorView.criticalSectionLockCounts[0]==255);
 }
}
'''
        with tempfile.TemporaryDirectory(prefix='th10-browser-lifecycle-') as tmp:
            source, binary = Path(tmp) / 'fixture.cpp', Path(tmp) / 'fixture'
            source.write_text(program)
            subprocess.run(['c++', '-std=c++17', '-O2', str(source), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    unittest.main()
