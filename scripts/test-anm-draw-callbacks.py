#!/usr/bin/env python3
"""Actual ANM callback bodies with camera/COM observers; no native layout claim."""
from pathlib import Path
import signal
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
LAYERS = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 19]


class Callbacks(unittest.TestCase):
    def program(self, source):
        source = '\n'.join(s for s in source.splitlines() if not s.startswith('#include'))
        declarations = '\n'.join(f'static int __fastcall DrawLayer{n}(AnmRenderManagerView*);' for n in LAYERS)
        return r'''
#include <cassert>
#include <vector>
#include <string>
#define __fastcall
struct AnmViewportView {int identity;};
struct AnmViewportOwnerView {AnmViewportView viewport;};
struct StageCameraStateView {AnmViewportOwnerView prefix;float value0E8,value0EC;};
struct Device;
struct Vtable {
 long (*SetViewport)(Device*,const AnmViewportView*);
 long (*SetRenderState)(Device*,unsigned,unsigned);
};
struct Device {Vtable *vtable;int identity;};
struct MainSupervisorView {
 unsigned char *unknown154;AnmViewportOwnerView gameplayViewport;
 AnmViewportOwnerView *activeViewport;Device *d3dDevice;
 int viewportConfigured;unsigned cachedFogEnabled;
} g_MainSupervisorView;
static std::vector<std::string> events;
static StageCameraStateView background;
static AnmViewportOwnerView alternate;
static bool swapCamera=false,swapDevice=false;
static Device *expectedDevice;
static const AnmViewportView *expectedViewport;
struct AnmRenderManagerView {
 float screenShakeX=4,screenShakeY=5;
 int DrawLayer(int n){events.push_back("layer"+std::to_string(n));return 100+n;}
 int UpdatePrimaryVms(){return 201;}
 int UpdateSecondaryVms(){return 202;}
 void FlushVertexBuffer();
 static int __fastcall OnUpdatePrimary(AnmRenderManagerView*);
 static int __fastcall OnUpdateSecondary(AnmRenderManagerView*);
''' + declarations + r'''
} renderer;
AnmRenderManagerView *g_AnmRenderManagerView=&renderer;
static long viewport(Device*d,const AnmViewportView*v){
 assert(d==expectedDevice&&v==expectedViewport);
 assert(g_MainSupervisorView.viewportConfigured==-1);
 events.push_back("viewport");return 0;
}
static long state(Device*d,unsigned n,unsigned value){
 assert(d==expectedDevice&&n==28&&value==0);
 assert(g_MainSupervisorView.cachedFogEnabled==0&&g_MainSupervisorView.viewportConfigured==0);
 events.push_back("fog0");return 0;
}
Vtable table={viewport,state};Device first={&table,1},second={&table,2};
void MainConfigureCamera(AnmViewportOwnerView*c){
 assert(g_MainSupervisorView.activeViewport==c);
 events.push_back(c==&background.prefix?"camera0":"camera1");
 if(swapCamera){g_MainSupervisorView.activeViewport=&alternate;expectedViewport=&alternate.viewport;}
 if(swapDevice){g_MainSupervisorView.d3dDevice=&second;expectedDevice=&second;}
}
void AnmRenderManagerView::FlushVertexBuffer(){
 assert(this==&renderer);events.push_back("flush");
 if(swapDevice){g_MainSupervisorView.d3dDevice=&second;expectedDevice=&second;}
}
''' + source + r'''
static void setup(){
 events.clear();background.prefix.viewport.identity=0;
 background.value0E8=13;background.value0EC=17;alternate.viewport.identity=2;
 g_MainSupervisorView.unknown154=reinterpret_cast<unsigned char*>(&background);
 g_MainSupervisorView.gameplayViewport.viewport.identity=1;
 g_MainSupervisorView.activeViewport=nullptr;g_MainSupervisorView.d3dDevice=&first;
 g_MainSupervisorView.viewportConfigured=-1;g_MainSupervisorView.cachedFogEnabled=0;
 renderer.screenShakeX=4;renderer.screenShakeY=5;
 swapCamera=swapDevice=false;expectedDevice=&first;expectedViewport=nullptr;
}
int main(){
 AnmRenderManagerView incoming;setup();
 assert(incoming.OnUpdatePrimary(&incoming)==201&&incoming.OnUpdateSecondary(&incoming)==202);
 using Callback=int(*)(AnmRenderManagerView*);
 Callback plain[]={''' + ','.join(f'AnmRenderManagerView::DrawLayer{n}' for n in LAYERS if n not in [3,14,15,16,19]) + r'''};
 int layers[]={0,1,2,4,5,6,7,8,9,10,11,12,13};
 for(unsigned n=0;n<sizeof(plain)/sizeof(*plain);++n){setup();assert(plain[n](&incoming)==100+layers[n]);assert(events==std::vector<std::string>({"layer"+std::to_string(layers[n])}));}
 for(unsigned cache:{0u,1u,0x100u,0xffffffffu}){
  setup();g_MainSupervisorView.cachedFogEnabled=cache;expectedViewport=&background.prefix.viewport;
  assert(incoming.DrawLayer3(&incoming)==103&&g_MainSupervisorView.viewportConfigured==0);
  auto expected=std::vector<std::string>({"camera0","viewport"});
  if(cache){expected.push_back("flush");expected.push_back("fog0");}
  expected.push_back("layer3");assert(events==expected);
  assert(g_MainSupervisorView.cachedFogEnabled==0);
 }
 setup();incoming.screenShakeX=21;incoming.screenShakeY=22;
 assert(incoming.DrawLayer14(&incoming)==114);
 assert(background.value0E8==0&&background.value0EC==0);
 assert(renderer.screenShakeX==0&&renderer.screenShakeY==0);
 assert(incoming.screenShakeX==21&&incoming.screenShakeY==22);
 Callback screen[]={AnmRenderManagerView::DrawLayer15,AnmRenderManagerView::DrawLayer16,AnmRenderManagerView::DrawLayer19};
 int screenLayers[]={15,16,19};
 for(int n=0;n<3;++n){
  setup();swapCamera=swapDevice=true;g_MainSupervisorView.cachedFogEnabled=0x100;
  expectedViewport=&g_MainSupervisorView.gameplayViewport.viewport;
  assert(screen[n](&incoming)==100+screenLayers[n]);
  assert(events==std::vector<std::string>({"camera1","viewport","layer"+std::to_string(screenLayers[n])}));
  assert(g_MainSupervisorView.viewportConfigured==1&&g_MainSupervisorView.cachedFogEnabled==0x100);
 }
 setup();swapCamera=swapDevice=true;g_MainSupervisorView.cachedFogEnabled=0x100;
 expectedViewport=&background.prefix.viewport;assert(incoming.DrawLayer3(&incoming)==103);
 assert(events==std::vector<std::string>({"camera0","viewport","flush","fog0","layer3"}));
}
'''

    def execute(self, source, rejection=None):
        with tempfile.TemporaryDirectory(dir=ROOT / 'build', prefix='anm-callbacks-') as d:
            p = Path(d);(p / 'test.cpp').write_text(self.program(source))
            subprocess.run(['c++', '-std=c++17', '-O0', str(p / 'test.cpp'), '-o', str(p / 'test')], check=True)
            result = subprocess.run([str(p / 'test')], capture_output=True, text=True)
            if rejection is None:
                self.assertEqual(result.returncode, 0, result.stderr)
            else:
                self.assertEqual(result.returncode, -signal.SIGABRT, result.stderr)
                self.assertIn(rejection, result.stderr)

    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_bodies_and_controls(self):
        (ROOT / 'build').mkdir(exist_ok=True)
        source = (ROOT / 'src/AnmDrawCallbacks.cpp').read_text()
        self.execute(source)
        for before, after, check in [
            ('g_MainSupervisorView.cachedFogEnabled != 0', 'static_cast<unsigned char>(g_MainSupervisorView.cachedFogEnabled) != 0', 'events==expected'),
            ('&g_MainSupervisorView.activeViewport->viewport', '&camera->viewport', 'v==expectedViewport'),
            ('camera->value0EC = 0;', '/* omitted second shake clear */', 'background.value0EC==0'),
            ('g_MainSupervisorView.viewportConfigured = 1;', 'g_MainSupervisorView.viewportConfigured = 0;', 'viewportConfigured==1'),
        ]:
            self.assertIn(before, source)
            self.execute(source.replace(before, after), check)


if __name__ == '__main__':
    unittest.main()
