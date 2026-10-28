#!/usr/bin/env python3
"""Actual draw-pass/adapter source with synthetic D3D, camera and layer sinks."""
from pathlib import Path
import shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
class StagePasses(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_pass_source(self):
  s=(ROOT/'src/StageDraw.cpp').read_text();s=s[s.index('// Target0x405300:'):]
  h=(ROOT/'src/AnmManager.hpp').read_text();a=h.index('struct AnmVmTimerView\n{');b=h.index('\n};',a)+3;timer=h[a:b]
  impl=(ROOT/'src/AnmManager.cpp').read_text();a=impl.index('void AnmVmTimerView::Add(float value)');b=impl.index('\n}',a)+2;timer+='\n'+impl[a:b]
  program=r"""
#include <cassert>
#include <cstring>
#include <vector>
#include <string>
#include <cstddef>
#define __stdcall
#define __fastcall
float g_AnmGameSpeed=1.0f;
"""+timer+r"""
struct Fog {float start,end,b,g,r,a;unsigned color;};
struct AnmViewportView {int unused;};
struct StageCameraStateView {float value0E8,value0EC;Fog fog;unsigned char tail[240];AnmViewportView viewport;};
static_assert(sizeof(StageCameraStateView)==280,"camera copy fixture size");
struct AnmViewportOwnerView {float value0E8,value0EC;Fog fog;unsigned char tail[240];AnmViewportView viewport;};
struct AnmVmView {void *loadedSprite=nullptr;};
struct StageScriptView {unsigned flags=0,tint=0;AnmVmTimerView transitionTimer;int frame=0,auxiliaryActive=0,unknown1EE4=0;int visibleObjectCount=0,culledObjectCount=0,drawCount=0;StageCameraStateView camera={};AnmVmView scriptVms[8];};
static std::vector<std::string> trace;
static StageScriptView *active;
static bool checkTint=false,mutateFog=false,mutateFogWords=false,expectReplacement=false;
static void onFlush();
struct Color {unsigned value=0;};
struct AnmRenderManagerView {
 int useMixColor=0;Color mixColor;
 void FlushVertexBuffer(){onFlush();trace.push_back("F");}
 int Draw(AnmVmView*v){if(checkTint)assert(useMixColor==1&&mixColor.value==active->tint);int n=int(v-active->scriptVms);assert(n>=0&&n<8&&v->loadedSprite);trace.push_back("VM"+std::to_string(n));return 0;}
 int DrawLayer(int n){trace.push_back("ANM"+std::to_string(n));return 0;}
} renderer;
AnmRenderManagerView *g_AnmRenderManagerView=&renderer;
StageCameraStateView g_StagePublishedCamera={};
AnmViewportOwnerView *g_AnmViewportOwner=nullptr;
int g_StageViewportConfigured=1,g_StageFogEnabled=0;
struct D3d9ClearRectView {int left,top,right,bottom;};
struct D3d9DeviceView;
struct Vtable {
 long (*SetRenderState)(D3d9DeviceView*,unsigned,unsigned);
 long (*SetViewport)(D3d9DeviceView*,const void*);
 long (*Clear)(D3d9DeviceView*,unsigned,const D3d9ClearRectView*,unsigned,unsigned,float,unsigned);
};
struct D3d9DeviceView {Vtable *vtable;};
static void checkDevice(D3d9DeviceView*);
static long state(D3d9DeviceView*d,unsigned s,unsigned v){checkDevice(d);assert(trace.back()=="F");if(s==28)assert(g_StageFogEnabled==int(v));trace.push_back("S"+std::to_string(s)+"="+std::to_string(v));return 0;}
static long viewport(D3d9DeviceView*,const void *p){assert(p==&g_AnmViewportOwner->viewport);trace.push_back("viewport");return 0;}
static long clear(D3d9DeviceView*,unsigned n,const D3d9ClearRectView*r,unsigned flags,unsigned color,float z,unsigned stencil){
 assert(z==1&&stencil==0);
 if(flags==2){assert(n==0&&r==nullptr&&color==0);trace.push_back("depth-clear");}
 else{assert(flags==1&&n==1&&r->left==32&&r->top==16&&r->right==416&&r->bottom==464);trace.push_back("clear="+std::to_string(color));}
 return 0;
}
Vtable table={state,viewport,clear};D3d9DeviceView device={&table};D3d9DeviceView *g_Direct3DDevice=&device;
D3d9DeviceView replacement={&table};
static void checkDevice(D3d9DeviceView*d){if(expectReplacement)assert(d==&replacement);}
static void onFlush(){if(mutateFog&&!trace.empty()&&trace.back()=="S23=4"){
 mutateFog=false;active->camera.fog.color=0x44556677;g_Direct3DDevice=&replacement;expectReplacement=true;
}if(mutateFogWords&&!trace.empty()&&trace.back()=="S34="+std::to_string(active->camera.fog.color)){
 mutateFogWords=false;unsigned start=0x7fa54321,end=0x80000000;
 memcpy(&active->camera.fog.start,&start,4);memcpy(&active->camera.fog.end,&end,4);
}}

void StageConfigureCamera(StageCameraStateView*c){assert(c==&g_StagePublishedCamera);assert(g_AnmViewportOwner==reinterpret_cast<AnmViewportOwnerView*>(c));trace.push_back("camera");}
struct MainSupervisorView {} g_MainSupervisorView;
void MainConfigureGameplayViewport(int n){assert(n==0);trace.push_back("viewport0");}
void StageDisableSupervisorFog(MainSupervisorView*p){assert(p==&g_MainSupervisorView);g_StageFogEnabled=0;trace.push_back("disable-fog");}
void *StageCreateTransitionEffect(int priority,int kind,int frames,unsigned a,unsigned b,unsigned c){assert(priority==15&&kind==3&&frames==30&&a==0&&b==0&&c==0);trace.push_back("effect-failed");return nullptr;}
int StageDrawInstances(StageScriptView*s,int layer){assert(s==active);if(checkTint)assert(renderer.useMixColor==1&&renderer.mixColor.value==s->tint);trace.push_back("layer"+std::to_string(layer));++s->visibleObjectCount;s->drawCount+=2;return 0;}
"""+s+r"""
static void setup(StageScriptView&s,unsigned flags,int time){
 active=&s;checkTint=false;mutateFog=false;mutateFogWords=false;expectReplacement=false;g_Direct3DDevice=&device;g_AnmGameSpeed=1.0f;s.flags=flags;s.transitionTimer.flags=0;s.transitionTimer.SetCurrent(time);
 s.tint=0xff123456;s.frame=40;s.auxiliaryActive=0;s.unknown1EE4=0;
 s.visibleObjectCount=91;s.culledObjectCount=92;s.drawCount=93;
 s.camera.fog.start=30;s.camera.fog.end=1800;s.camera.fog.color=0xab112233;
 s.camera.value0E8=9;s.camera.value0EC=8;memset(s.camera.tail,0x5a,sizeof(s.camera.tail));
 g_StagePublishedCamera.value0E8=1.25f;g_StagePublishedCamera.value0EC=-2.5f;
 g_StagePublishedCamera.tail[239]=0;
 for(int i=0;i<8;++i)s.scriptVms[i].loadedSprite=nullptr;
 trace.clear();g_StageFogEnabled=0;g_StageViewportConfigured=1;renderer.useMixColor=0;renderer.mixColor.value=0;
}
static std::vector<std::string> E;
static void rs(unsigned a,unsigned b){E.push_back("F");E.push_back("S"+std::to_string(a)+"="+std::to_string(b));}
static void publish(){E.push_back("F");E.push_back("camera");E.push_back("viewport");}
static void fog(){rs(34,0xab112233);rs(36,0x41f00000);rs(37,0x44e10000);}
int main(){
 StageScriptView s;
 setup(s,8,20);assert(StageDrawEarlyCallback(&s)==1&&trace.empty());assert(StageDrawLateCallback(&s)==1&&trace.empty());assert(s.transitionTimer.current==20&&s.visibleObjectCount==91);
 // Full early pass: both clears, VM subset, all8 layers, counter reset and mix restoration.
 setup(s,1,0);s.scriptVms[0].loadedSprite=&s;s.scriptVms[2].loadedSprite=&s;s.scriptVms[7].loadedSprite=&s;s.unknown1EE4=4;
 checkTint=true;StageDrawEarlyCallback(&s);
 E.clear();publish();rs(14,1);rs(23,4);fog();E.push_back("depth-clear");E.push_back("clear="+std::to_string(0x112233));
 E.push_back("viewport0");E.push_back("disable-fog");E.push_back("F");rs(14,0);
 E.push_back("VM0");E.push_back("VM2");E.push_back("VM7");rs(14,1);E.push_back("camera");E.push_back("viewport");
 rs(28,1);for(int i=0;i<8;++i)E.push_back("layer"+std::to_string(i));E.push_back("F");rs(14,0);rs(23,8);
 assert(trace==E);
 assert(s.visibleObjectCount==8&&s.culledObjectCount==0&&s.drawCount==16&&s.unknown1EE4==5);
 assert(renderer.useMixColor==0&&renderer.mixColor.value==0x80808080);
 assert(s.camera.value0E8==1.25f&&s.camera.value0EC==-2.5f&&g_StagePublishedCamera.tail[239]==0x5a);
 // Full later pass: ANM17 then18, layers8..11, no counter reset, duplicated restoration.
 setup(s,1,0);StageDrawLateCallback(&s);
 E.clear();publish();rs(28,1);rs(14,0);rs(23,8);E.push_back("ANM17");rs(23,4);rs(28,0);E.push_back("ANM18");rs(23,4);fog();rs(14,0);rs(28,1);
 for(int i=8;i<12;++i)E.push_back("layer"+std::to_string(i));E.push_back("F");rs(14,0);rs(23,8);rs(14,0);rs(23,8);rs(28,0);
 assert(trace==E);assert(s.visibleObjectCount==95&&s.drawCount==101&&s.culledObjectCount==92);
 // Transition at29 creates effect even if allocation fails, sets timer1 and enables draw.
 setup(s,4,29);s.frame=33;StageDrawEarly(&s);
 assert(s.transitionTimer.current==1&&s.transitionTimer.previous==0&&(s.flags&1));
 assert(std::find(trace.begin(),trace.end(),"effect-failed")!=trace.end());
 assert(std::find(trace.begin(),trace.end(),"clear=0")!=trace.end());
 // At30 alpha/draw are cleared; frame34 selects colored clear, no effect call.
 setup(s,5,30);s.frame=34;StageDrawEarly(&s);assert((s.tint>>24)==0&&(s.flags&1)==0&&s.transitionTimer.current==30);
 assert(std::find(trace.begin(),trace.end(),"effect-failed")==trace.end());
 assert(std::find(trace.begin(),trace.end(),"clear="+std::to_string(0x112233))!=trace.end());
 // >=60 suppresses camera/clear but retains final state restoration.
 setup(s,4,60);StageDrawEarly(&s);assert(trace==std::vector<std::string>({"F","S14=0","F","S23=8"}));
 // Later pass transition completion sets hidden bit from bit2 and clears bits2/4.
 setup(s,6,1);s.auxiliaryActive=1;StageDrawLate(&s);
 assert(s.transitionTimer.current==0&&s.flags==8&&s.tint==0xffffff);
 assert(renderer.useMixColor==1&&renderer.mixColor.value==0xff404040);
 setup(s,4,60);StageDrawLate(&s);assert(s.transitionTimer.current==59);assert(std::find(trace.begin(),trace.end(),"camera")==trace.end());
 setup(s,0,0);StageDrawLate(&s);assert(s.transitionTimer.current==0);
 // Fog value is captured before Flush, while the device is reloaded after it.
 setup(s,0,0);mutateFog=true;StageDrawEarly(&s);
 assert(expectReplacement&&s.camera.fog.color==0x44556677);
 assert(std::find(trace.begin(),trace.end(),"S34="+std::to_string(0xab112233))!=trace.end());
 // Both passes send raw fog words: signed zero, infinity and signaling-NaN payload.
 // A Flush mutation occurs after the start-word capture and before the end capture.
 for(int pass=0;pass<2;++pass){
  const unsigned starts[]={0x80000000,0x7f800000,0x7fa12345};
  for(unsigned word:starts){
   setup(s,0,0);unsigned end=0xffa76543;
   memcpy(&s.camera.fog.start,&word,4);memcpy(&s.camera.fog.end,&end,4);
   if(pass==0)StageDrawEarly(&s);else StageDrawLate(&s);
   assert(std::find(trace.begin(),trace.end(),"S36="+std::to_string(word))!=trace.end());
   assert(std::find(trace.begin(),trace.end(),"S37="+std::to_string(end))!=trace.end());
  }
  setup(s,0,0);unsigned before=0xffa12345;
  memcpy(&s.camera.fog.start,&before,4);mutateFogWords=true;
  if(pass==0)StageDrawEarly(&s);else StageDrawLate(&s);
  assert(!mutateFogWords);
  assert(std::find(trace.begin(),trace.end(),"S36="+std::to_string(before))!=trace.end());
  assert(std::find(trace.begin(),trace.end(),"S37="+std::to_string(0x80000000u))!=trace.end());
 }
 // VM0 is the outer gate even when another script VM is loaded.
 setup(s,1,0);s.scriptVms[2].loadedSprite=&s;checkTint=true;StageDrawEarly(&s);
 assert(std::find(trace.begin(),trace.end(),"VM2")==trace.end());
 assert(std::find(trace.begin(),trace.end(),"viewport0")==trace.end());
 // Actual Add retains fractional time and records the previous integer.
 setup(s,0,2);g_AnmGameSpeed=0.5f;StageDrawLate(&s);
 assert(s.transitionTimer.previous==2&&s.transitionTimer.current==1&&s.transitionTimer.subframe==1.5f);
 StageDrawLate(&s);assert(s.transitionTimer.previous==1&&s.transitionTimer.current==1&&s.transitionTimer.subframe==1.0f);
 StageDrawLate(&s);assert(s.transitionTimer.previous==1&&s.transitionTimer.current==0&&s.transitionTimer.subframe==0.5f);
 g_AnmGameSpeed=1.0f;

}
"""
  program=program.replace('#include <cassert>','#include <cassert>\n#include <algorithm>')
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='stage-passes-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
