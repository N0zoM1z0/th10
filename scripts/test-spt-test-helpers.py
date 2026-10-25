#!/usr/bin/env python3
"""Actual helper-source host checks; dependencies/layout are explicit test seams."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
class Helpers(unittest.TestCase):
 def test_helpers(self):
  s=(ROOT/'src/SptTest.cpp').read_text();ctor=s[s.index('SptTestView::SptTestView()'):s.index('SptTestView *SptTestCreate()')];names=s[s.index('void SptTestView::ClearScriptNames()'):s.index('// Destructor42BC30')]
  program=r'''
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <new>
#include <vector>
#include <functional>
using std::memset;
struct SptTestView;
static SptTestView *g_SptTestView;
static int workerConstructed=0,cursorsConstructed=0;
struct Worker {void *thread;unsigned id;int stop,running;unsigned other;void *entry;Worker(){++workerConstructed;thread=nullptr;id=stop=running=0;}virtual ~Worker(){} };
struct Cursor {int saveDepth,current,disabledEntryCount,wraps,count;Cursor(){++cursorsConstructed;saveDepth=current=disabledEntryCount=0;wraps=1;count=999;}};
int SptTestUpdateCallback(void*){return 1;}int SptTestDrawCallback(void*){return 1;}
namespace th10 {struct ChainElem {unsigned flags;int (*callback)(void*);void *arg;};struct Chain {static ChainElem *AllocElem(int (*cb)(void*));static int AddToCalcChain(ChainElem*,int,Chain*);static int AddToDrawChain(ChainElem*,int,Chain*);};static Chain first,second,*g_Chain=&first;}
struct SptTestView {unsigned flags,unknown;th10::ChainElem *updateTask,*drawTask;Worker worker;unsigned unknown2c;int state;char **scriptNames;int scriptCount;Cursor menu,files,entries;unsigned char tail[32];SptTestView();int RegisterTasks();void ClearScriptNames();};
static th10::ChainElem tasks[2];static int allocations;static std::vector<int> events;
th10::ChainElem *th10::Chain::AllocElem(int (*cb)(void*)){assert(allocations<2);auto *t=&tasks[allocations++];t->flags=0x81;t->callback=cb;t->arg=nullptr;events.push_back(allocations*10);return t;}
int th10::Chain::AddToCalcChain(ChainElem*t,int priority,Chain*c){assert(t==tasks&&t->flags==0x83&&t->callback==SptTestUpdateCallback&&priority==5&&c==&first);auto*v=static_cast<SptTestView*>(t->arg);assert(v&&v->updateTask==nullptr);g_Chain=&second;events.push_back(11);return -7;}
int th10::Chain::AddToDrawChain(ChainElem*t,int priority,Chain*c){assert(t==tasks+1&&t->flags==0x83&&t->callback==SptTestDrawCallback&&priority==0x27&&c==&second);auto*v=static_cast<SptTestView*>(t->arg);assert(v&&v->updateTask==tasks&&v->drawTask==nullptr);events.push_back(21);return -9;}
static std::vector<void*> freed;static std::function<void(void*)> onFree;
static void observed_free(void*p){assert(p);freed.push_back(p);if(onFree)onFree(p);}
#define free observed_free
'''+ctor+names+r'''
#undef free
int main(){
 void *storage=std::malloc(sizeof(SptTestView));assert(storage);std::memset(storage,0xa5,sizeof(SptTestView));auto *v=new(storage)SptTestView;
 assert(workerConstructed==1&&cursorsConstructed==3&&g_SptTestView==v&&v->flags==2);
 auto *bytes=static_cast<unsigned char*>(storage);for(unsigned i=sizeof(v->flags);i<sizeof(SptTestView);++i)assert(bytes[i]==0);
 // The real constructor deliberately erases its embedded worker vptr/cursors.
 assert(v->menu.count==0&&v->menu.wraps==0);assert(v->RegisterTasks()==0);assert(v->updateTask==tasks&&v->drawTask==tasks+1);assert(events==std::vector<int>({10,11,20,21}));
 char one=1,two=2,three=3;char *table[3]={&one,nullptr,&two};v->scriptNames=table;v->scriptCount=3;v->ClearScriptNames();assert(freed==std::vector<void*>({&one,&two,table}));assert(table[0]==nullptr&&table[1]==nullptr&&table[2]==nullptr&&v->scriptNames==nullptr&&v->scriptCount==3);
 for(int count:{-2,0}){freed.clear();char *t[1]={&one};v->scriptNames=t;v->scriptCount=count;v->ClearScriptNames();assert(freed==std::vector<void*>({t})&&t[0]==&one&&v->scriptNames==nullptr&&v->scriptCount==count);}
 freed.clear();v->scriptNames=nullptr;v->scriptCount=5;v->ClearScriptNames();assert(freed.empty()&&v->scriptCount==5);
 // free() is a synthetic mutation seam: table must be reread before clear,
 // and the signed count reread before deciding whether to continue.
 char *a[2]={&one,&two},*b[2]={&three,&two};v->scriptNames=a;v->scriptCount=2;onFree=[&](void*p){if(p==&one){v->scriptNames=b;v->scriptCount=1;}};v->ClearScriptNames();assert(freed==std::vector<void*>({&one,b}));assert(a[0]==&one&&b[0]==nullptr&&b[1]==&two&&v->scriptNames==nullptr&&v->scriptCount==1);onFree={};
 // No automatic destructor is exercised: its source remains unresolved.
 std::free(storage);
}
'''
  with tempfile.TemporaryDirectory() as td:
   p=Path(td);(p/'test.cpp').write_text(program);subprocess.run(['c++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-sanitize=vptr',str(p/'test.cpp'),'-o',str(p/'test')],check=True);subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
