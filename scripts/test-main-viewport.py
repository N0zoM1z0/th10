#!/usr/bin/env python3
"""Actual camera/viewport/fog bodies with synthetic COM and Flush observers.

The fixture checks source contracts, not Windows layout, x87 or D3DX math.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class MainViewport(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_actual_source(self):
        header = (ROOT / 'src/AnmManager.hpp').read_text()
        def record(marker):
            start = header.index(marker + '\n{')
            return header[start:header.index('\n};', start) + 3]
        source = (ROOT / 'src/MainViewport.cpp').read_text()
        source = '\n'.join(line for line in source.splitlines()
                           if not line.startswith('#include'))
        program = r'''
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <new>
#include <vector>
#include <string>
#define __stdcall
''' + record('struct AnmFloat3View') + r'''
struct AnmMatrixView { float values[16]; };
struct AnmViewportView { unsigned x,y,width,height; float minZ,maxZ; };
''' + record('struct AnmViewportOwnerView') + r'''
struct Device;
struct Vtable {
 long (*SetTransform)(Device*,unsigned,const AnmMatrixView*);
 long (*SetViewport)(Device*,const AnmViewportView*);
 long (*SetRenderState)(Device*,unsigned,unsigned);
};
struct Device { Vtable *vtable; int id; };
// Camera records have actual host lifetimes inside reserved byte storage.
// Pointer-bearing supervisor fields are synthetic, not an i386 layout claim.
struct MainSupervisorView {
 alignas(8) unsigned char storage[0x384];
 Device *d3dDevice;
 AnmViewportOwnerView *activeViewport;
 int viewportConfigured;
 unsigned cachedFogEnabled;
};
static std::vector<std::string> events;
static MainSupervisorView *activeSupervisor;
static AnmViewportOwnerView *replacementCamera;
static bool mutateOnFlush=false,replaceDevice=false;
static void onFlush();
struct AnmRenderManagerView {
 float screenShakeX=0,screenShakeY=0;
 void FlushVertexBuffer(){events.push_back("flush");onFlush();}
} rendererA,rendererB;
AnmRenderManagerView *g_AnmRenderManagerView=&rendererA;
Device *g_Direct3DDevice;
static AnmFloat3View seenEye,seenAt,seenUp;
static float seenAspect,seenFov,seenNear,seenFar;
static const AnmMatrixView *seenView,*seenProjection;
static const AnmViewportView *seenViewport;
static long transform(Device*d,unsigned state,const AnmMatrixView*m);
static long viewport(Device*d,const AnmViewportView*v);
static long renderState(Device*d,unsigned state,unsigned value);
Vtable table={transform,viewport,renderState};
Device deviceA={&table,1},deviceB={&table,2},ownerDevice={&table,3};
static void onFlush(){if(mutateOnFlush){
 mutateOnFlush=false;g_AnmRenderManagerView=&rendererB;
 g_Direct3DDevice=&deviceB;activeSupervisor->activeViewport=replacementCamera;
}}
extern "C" AnmMatrixView *D3DXMatrixLookAtLH(AnmMatrixView*out,
 const AnmFloat3View*eye,const AnmFloat3View*at,const AnmFloat3View*up){
 events.push_back("look");seenView=out;seenEye=*eye;seenAt=*at;seenUp=*up;return out;
}
extern "C" AnmMatrixView *D3DXMatrixPerspectiveFovLH(AnmMatrixView*out,
 float fov,float aspect,float nearZ,float farZ){
 events.push_back("perspective");seenProjection=out;
 seenFov=fov;seenAspect=aspect;seenNear=nearZ;seenFar=farZ;return out;
}
static long transform(Device*d,unsigned state,const AnmMatrixView*m){
 events.push_back("transform"+std::to_string(state)+"/"+std::to_string(d->id));
 if(state==2){assert(m==seenView);if(replaceDevice)g_Direct3DDevice=&deviceB;}
 else{assert(state==3&&m==seenProjection);}return 0;
}
static long viewport(Device*d,const AnmViewportView*v){
 assert(d==&ownerDevice);seenViewport=v;events.push_back("viewport");return 0;
}
static long renderState(Device*d,unsigned state,unsigned value){
 assert(d==&ownerDevice&&state==28&&value==0);
 assert(activeSupervisor->cachedFogEnabled==0);
 events.push_back("fog0");return 123;
}
''' + source + r'''
static void setup(MainSupervisorView&s){
 activeSupervisor=&s;s.d3dDevice=&ownerDevice;s.activeViewport=nullptr;
 s.viewportConfigured=-7;s.cachedFogEnabled=0;
 g_AnmRenderManagerView=&rendererA;g_Direct3DDevice=&deviceA;
 rendererA.screenShakeX=rendererA.screenShakeY=0;
 rendererB.screenShakeX=rendererB.screenShakeY=0;
 mutateOnFlush=replaceDevice=false;events.clear();
}
static unsigned word(float f){unsigned v;memcpy(&v,&f,4);return v;}
int main(){
 MainSupervisorView s;setup(s);
 auto*c0=new(s.storage+0x154) MainCameraRecordView;
 auto*c1=new(s.storage+0x26c) MainCameraRecordView;
 MainCameraRecordView*records[]={c0,c1};
 const unsigned dimensions[][2]={{640,480},{384,448},{0xffffffffu,0x80000001u},{0x80000000u,2},{0,480}};
 for(auto&dim:dimensions){
  setup(s);c0->prefix.viewport.width=dim[0];c0->prefix.viewport.height=dim[1];
  c0->screenShakeX=3.5f;c0->screenShakeY=-2.25f;
  MainConfigureCamera(&c0->prefix);
  assert(events==std::vector<std::string>({"flush","look","perspective","transform2/1","transform3/1"}));
  float w=static_cast<float>(dim[0]),h=static_cast<float>(dim[1]);
  assert(seenAt.x==w*.5f&&seenAt.y==h*.5f&&seenAt.z==0);
  assert(seenEye.x==seenAt.x&&seenEye.y==seenAt.y);
  float z=static_cast<float>(double(h*.5f)/std::tan(0.15707963705062866));
  assert(seenEye.z==z&&seenUp.x==0&&seenUp.y==-1&&seenUp.z==0);
  assert(word(seenFov)==0x3ea0d97c&&seenAspect==w/h&&seenNear==1&&seenFar==10000);
  assert(rendererA.screenShakeX==3.5f&&rendererA.screenShakeY==-2.25f);
 }
 for(int index=0;index<2;++index){
  setup(s);auto*c=records[index];c->prefix.viewport.width=640;c->prefix.viewport.height=480;
  c->screenShakeX=11;c->screenShakeY=22;
  MainConfigureGameplayViewport(&s,index);
  assert(s.activeViewport==&c->prefix&&s.viewportConfigured==index);
  assert(seenViewport==&c->prefix.viewport&&events.back()=="viewport");
 }
 // Reload active camera after the configurator, and reload renderer/device globals.
 setup(s);c0->screenShakeX=7;c0->screenShakeY=9;
 replacementCamera=&c1->prefix;mutateOnFlush=true;
 MainConfigureGameplayViewport(&s,0);
 assert(s.activeViewport==replacementCamera&&seenViewport==&c1->prefix.viewport);
 assert(rendererA.screenShakeX==0&&rendererB.screenShakeX==7&&rendererB.screenShakeY==9);
 setup(s);replaceDevice=true;MainConfigureCamera(&c0->prefix);
 assert(events.back()=="transform3/2");
 setup(s);g_AnmRenderManagerView=nullptr;MainConfigureCamera(&c0->prefix);
 assert(events.front()=="look"&&events.size()==4);
 // Only-high-byte nonzero must not bypass the full-word fog cache check.
 for(unsigned value:{0u,1u,0x100u,0xffffffffu}){
  setup(s);s.cachedFogEnabled=value;long result=StageDisableSupervisorFog(&s);
  assert(result==(value==0?0:123));
  if(value==0)assert(events.empty());
  else assert(events==std::vector<std::string>({"flush","fog0"}));
  assert(s.cachedFogEnabled==0);
 }
 c1->~MainCameraRecordView();c0->~MainCameraRecordView();
}
'''
        (ROOT / 'build').mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=ROOT / 'build', prefix='main-viewport-') as d:
            p = Path(d)
            (p / 'test.cpp').write_text(program)
            subprocess.run(['c++', '-std=c++17', '-O0', str(p / 'test.cpp'), '-o', str(p / 'test')], check=True)
            subprocess.run([str(p / 'test')], check=True)


if __name__ == '__main__':
    unittest.main()
