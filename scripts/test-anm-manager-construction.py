#!/usr/bin/env python3
"""Actual ANM constructor with explicit member/allocation/Chain fixture seams."""
from pathlib import Path
import shutil
import signal
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
NAMES = ['OnUpdatePrimary', 'OnUpdateSecondary'] + [f'DrawLayer{n}' for n in [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,19]]
PRIORITIES = [0x1a,8,9,0xb,0xd,0xf,0x10,0x11,0x12,0x13,0x15,0x17,0x18,0x1a,0x1c,0x21,0x24,0x29,0x2a,0x2d]


def constructor_source():
    s = (ROOT / 'src/AnmManager.cpp').read_text()
    start = s.index('AnmRenderManagerView::AnmRenderManagerView()')
    return s[start:s.index('// Target 0x004462F0', start)]


class Construction(unittest.TestCase):
    def program(self, source):
        return r'''
#include <cassert>
#include <cstring>
#include <vector>
#define __fastcall
#define __stdcall
#define __declspec(x)
#include "Chain.hpp"
static int constructedVms=0,initializedVms=0,constructedNodes=0;
struct Vm {
 unsigned marker;
 Vm(){marker=55;++constructedVms;}
 void Initialize(){assert(marker==0);marker=19;++initializedVms;}
};
struct Vertex {float rhw,u,v,untouched;};
Vertex g_AnmQuadVerticesNoDiffuse[4],g_AnmQuadVertices[4];
struct Device;
struct Vtable {long(*SetVertexShader)(Device*,void*);};
struct Device {Vtable*vtable;};
static std::vector<th10::ChainElem*> nodes;
static std::vector<int> priorities,lists;
static bool shaderSet=false;
static long shader(Device*,void*v){assert(!v&&nodes.size()==20);shaderSet=true;return 0;}
Vtable table={shader};Device device={&table};Device*g_Direct3DDevice=&device;
struct AnmRenderManagerView {
 Vm vmPool[0x1000],primaryVm,drawLayerSentinels[20];
 void*quadVertexBuffer;void*currentTexture;
 unsigned currentBlendMode,currentColorOperation,currentTextureFactor,currentVertexShader;
 unsigned currentCameraMode,currentZWrite,currentTextureFilter;
 int captureAnmIndex,captureSurfaceIndex;
 AnmRenderManagerView();
''' + '\n'.join(f'static int __fastcall {n}(AnmRenderManagerView*){{return 1;}}' for n in NAMES) + r'''
};
namespace th10 {
// Deliberate dependency seam: seed known high flags. This is not target
// allocator initialization or the actual partial-bit constructor's runtime.
ChainElem::ChainElem(){
 ++constructedNodes;flags=0xa4;callback=addedCallback=deletedCallback=nullptr;
 prio=0;self=this;next=prev=nullptr;arg=nullptr;
}
Chain::Chain(){}
Chain chain;Chain*g_Chain=&chain;
static void observe(ChainElem*e,int p,Chain*c,int list){
 assert(c==g_Chain&&e->flags==0xa7&&e->addedCallback==nullptr&&e->deletedCallback==nullptr);
 assert(e->prio==0&&e->self==e&&e->next==nullptr&&e->prev==nullptr);
 nodes.push_back(e);priorities.push_back(p);lists.push_back(list);
}
int Chain::AddToCalcChain(ChainElem*e,int p,Chain*c){observe(e,p,c,0);return 7;}
int Chain::AddToDrawChain(ChainElem*e,int p,Chain*c){observe(e,p,c,1);return 7;}
}
''' + source + r'''
int main(){
 int priorNodes=constructedNodes;
 for(int i=0;i<4;++i){g_AnmQuadVertices[i].untouched=42;g_AnmQuadVerticesNoDiffuse[i].untouched=43;}
 auto*m=new AnmRenderManagerView;
 assert(constructedVms==0x1000+1+20&&initializedVms==0x1000);
 assert(m->primaryVm.marker==0&&m->drawLayerSentinels[19].marker==0);
 for(auto&vm:m->vmPool)assert(vm.marker==19);
 assert(constructedNodes-priorNodes==20&&nodes.size()==20&&shaderSet);
 th10::ChainCallback callbacks[]={''' + ','.join(f'reinterpret_cast<th10::ChainCallback>(AnmRenderManagerView::{n})' for n in NAMES) + r'''};
 int expectedPriorities[]={''' + ','.join(str(x) for x in PRIORITIES) + r'''};
 for(int n=0;n<20;++n){
  assert(nodes[n]->callback==callbacks[n]&&nodes[n]->arg==m);
  assert(priorities[n]==expectedPriorities[n]&&lists[n]==(n<2?0:1));
 }
 for(int i=0;i<4;++i){
  auto&a=g_AnmQuadVertices[i];auto&b=g_AnmQuadVerticesNoDiffuse[i];
  assert(a.rhw==1&&b.rhw==1&&a.u==(i%2?1:0)&&b.u==a.u);
  assert(a.v==(i>=2?1:0)&&b.v==a.v&&a.untouched==42&&b.untouched==43);
 }
 assert(!m->quadVertexBuffer&&!m->currentTexture);
 assert(m->currentBlendMode==0&&m->currentColorOperation==0&&m->currentTextureFactor==1);
 assert(m->currentVertexShader==0&&m->currentCameraMode==0xff&&m->currentZWrite==0&&m->currentTextureFilter==0);
 assert(m->captureAnmIndex==-1&&m->captureSurfaceIndex==-1);
 for(auto*node:nodes)delete node;delete m;
}
'''

    def execute(self, source, rejection=None):
        with tempfile.TemporaryDirectory(dir=ROOT / 'build', prefix='anm-construction-') as d:
            p=Path(d);(p/'test.cpp').write_text(self.program(source))
            subprocess.run(['c++','-std=c++17','-O0','-I'+str(ROOT/'src'),str(p/'test.cpp'),'-o',str(p/'test')],check=True)
            result=subprocess.run([str(p/'test')],capture_output=True,text=True)
            if rejection is None:
                self.assertEqual(result.returncode,0,result.stderr)
            else:
                self.assertEqual(result.returncode,-signal.SIGABRT,result.stderr)
                self.assertIn(rejection,result.stderr)

    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_source_and_controls(self):
        (ROOT/'build').mkdir(exist_ok=True)
        source=constructor_source();self.execute(source)
        for before,after,check in [
            ('element->flags |= 3u;', 'element->flags |= 1u;', 'e->flags==0xa7'),
            ('(OnUpdatePrimary);', '(OnUpdateSecondary);', 'nodes[n]->callback==callbacks[n]'),
            ('element, 0x2d, th10::g_Chain', 'element, 0x2c, th10::g_Chain', 'priorities[n]==expectedPriorities[n]'),
        ]:
            self.assertIn(before,source);self.execute(source.replace(before,after),check)


if __name__=='__main__':
    unittest.main()
