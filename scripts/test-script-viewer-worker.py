#!/usr/bin/env python3
"""Check maintained worker bodies against target API/state observations.

Synthetic host callbacks are never executed as threads. This does not validate
Windows runtime, i386 calling conventions or exact native code generation.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


def body(source, signature):
    start = source.index(signature)
    end = source.index('{', start) + 1
    depth = 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


class ScriptViewerWorker(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_wait_close_and_restart(self):
        source = (ROOT / 'src/ScriptViewerWorker.cpp').read_text()
        program = r'''
#include <cassert>
#include <cstdint>
#include <vector>
#define __stdcall
#define WAIT_TIMEOUT 258u
struct ScriptViewerWorkerView {
 const void *vtable;void *thread;unsigned threadId;
 int stopRequested,running;unsigned unknown014;void *entry018;unsigned unknown01C;
 void Stop();void Start(void *,int (*)());
};
static ScriptViewerWorkerView *active;
static std::vector<int> events;
static std::vector<unsigned> statuses;
static unsigned waitIndex;
static void *originalEntry,*expectedContext,*nextHandle;
static int entry(){return 0;}
static void *handle(unsigned value){return reinterpret_cast<void *>(uintptr_t(value));}
static unsigned WaitForSingleObject(void *thread,unsigned timeout){
 assert(thread==active->thread&&timeout==200);
 assert(active->stopRequested==1&&active->running==0);
 assert(active->entry018==originalEntry);
 events.push_back(1);assert(waitIndex<statuses.size());
 unsigned status=statuses[waitIndex++];
 // Model another observer changing the flags. Every timeout iteration must
 // restore them before Sleep, and re-read the handle for the next wait.
 if(status==WAIT_TIMEOUT){active->stopRequested=7;active->running=9;active->thread=handle(30+waitIndex);}
 return status;
}
static void Sleep(unsigned duration){
 assert(duration==1&&active->stopRequested==1&&active->running==0);
 assert(active->entry018==originalEntry);events.push_back(2);
}
static int CloseHandle(void *thread){
 assert(thread==active->thread&&thread!=nullptr);
 assert(active->stopRequested==1&&active->running==0&&active->entry018==originalEntry);
 events.push_back(3);return 0; // Return value is ignored, including failure.
}
static uintptr_t _beginthreadex(void *security,unsigned stack,
 unsigned (*callback)(void *),void *context,unsigned flags,unsigned *threadId){
 assert(security==nullptr&&stack==0&&flags==0&&context==expectedContext);
 assert(reinterpret_cast<void *>(callback)==reinterpret_cast<void *>(entry));
 assert(threadId==&active->threadId&&active->thread==nullptr);
 assert(active->stopRequested==0&&active->running==1);
 assert(active->entry018==reinterpret_cast<void *>(entry));
 *threadId=91;events.push_back(4);return reinterpret_cast<uintptr_t>(nextHandle);
}
'''
        program += body(source, 'void ScriptViewerWorkerView::Stop()')
        program += body(source, 'void ScriptViewerWorkerView::Start(')
        program += r'''
static void reset(ScriptViewerWorkerView &worker,void *thread){
 worker={handle(90),thread,18,7,9,0xa5,handle(21),0x5a};active=&worker;
 originalEntry=worker.entry018;events.clear();waitIndex=0;
}
static void unchanged(const ScriptViewerWorkerView &worker){
 assert(worker.vtable==handle(90)&&worker.unknown014==0xa5&&worker.unknown01C==0x5a);
}
int main(){
 ScriptViewerWorkerView worker;
 reset(worker,nullptr);worker.Stop();unchanged(worker);
 assert(events.empty()&&worker.thread==nullptr&&worker.threadId==18);
 assert(worker.entry018==handle(21)&&worker.stopRequested==7&&worker.running==9);
 // WAIT_OBJECT_0, WAIT_ABANDONED, and WAIT_FAILED all terminate the loop.
 for(unsigned terminal:{0u,128u,0xffffffffu}){
  for(unsigned retries:{0u,1u,3u}){
   reset(worker,handle(11));statuses.assign(retries,WAIT_TIMEOUT);statuses.push_back(terminal);
   worker.Stop();unchanged(worker);assert(waitIndex==retries+1);
   assert(worker.thread==nullptr&&worker.entry018==nullptr&&worker.threadId==18);
   assert(worker.stopRequested==1&&worker.running==0);
   std::vector<int> expected;
   for(unsigned i=0;i<retries;++i){expected.push_back(1);expected.push_back(2);}
   expected.push_back(1);expected.push_back(3);assert(events==expected);
  }
 }
 for(bool oldThread:{false,true})for(void *created:{handle(0),handle(77)}){
  reset(worker,oldThread?handle(11):nullptr);statuses={WAIT_TIMEOUT,0u};
  expectedContext=handle(99);nextHandle=created;worker.Start(expectedContext,entry);unchanged(worker);
  assert(worker.thread==created&&worker.threadId==91&&worker.stopRequested==0&&worker.running==1);
  assert(worker.entry018==reinterpret_cast<void *>(entry));
  assert(events==(oldThread?std::vector<int>({1,2,1,3,4}):std::vector<int>({4})));
 }
}
'''
        with tempfile.TemporaryDirectory(prefix='th10-worker-') as tmp:
            source_path, binary = Path(tmp) / 'fixture.cpp', Path(tmp) / 'fixture'
            source_path.write_text(program)
            subprocess.run(['c++', '-std=c++17', '-O2', str(source_path), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    unittest.main()
