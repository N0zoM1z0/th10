#!/usr/bin/env python3
"""Test actual camera setup with observable D3DX/COM sinks, not native math."""
from pathlib import Path
import shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
class StageCamera(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_camera_source(self):
  source=(ROOT/'src/StageDraw.cpp').read_text()
  a=source.index('void StageConfigureCamera(StageCameraStateView *camera)\n{')
  b=source.index('// Target 0x403090',a);source=source[a:b]
  header=(ROOT/'src/AnmManager.hpp').read_text()
  a=header.index('struct AnmFloat3View\n{');b=header.index('\n};',a)+3
  vector=header[a:b]
  program=r"""
#include <cassert>
#include <cmath>
#include <vector>
#include <string>
#include <cstddef>
"""+vector+r"""
struct AnmMatrixView {float values[16];};
struct AnmViewportView {unsigned width,height;};
struct StageCameraStateView {AnmFloat3View position,direction,up,cameraRight;float rockingOffset,rockingOffsetY,rockingOffsetZ,value048;AnmViewportView viewport;AnmMatrixView viewMatrix,projectionMatrix;float value0E8,value0EC;};
static std::vector<std::string> trace;
static StageCameraStateView *active;
struct AnmRenderManagerView {float screenShakeX=0,screenShakeY=0;void FlushVertexBuffer(){trace.push_back("flush");}};
AnmRenderManagerView first,second;AnmRenderManagerView *g_AnmRenderManagerView=&first;
struct D3d9DeviceView;
struct Vtable {long (*SetTransform)(D3d9DeviceView*,unsigned,const void*);};
struct D3d9DeviceView {Vtable *vtable;};
static D3d9DeviceView *g_Direct3DDevice,*otherDevice;
static bool lateNull=false;
static long transform(D3d9DeviceView *d,unsigned state,const void *matrix){
 assert(d==g_Direct3DDevice);
 if(state==2){assert(trace.back()=="perspective"&&matrix==&active->viewMatrix);assert(active->viewMatrix.values[0]==21);trace.push_back("view");g_Direct3DDevice=otherDevice;}
 else{assert(state==3&&trace.back()=="view"&&matrix==&active->projectionMatrix);assert(active->projectionMatrix.values[0]==31);trace.push_back("projection");}
 return 0;
}
Vtable table={transform};D3d9DeviceView device1={&table},device2={&table};
static AnmMatrixView *D3DXMatrixLookAtLH(AnmMatrixView*out,const AnmFloat3View*eye,const AnmFloat3View*at,const AnmFloat3View*up){
 assert(out==&active->viewMatrix&&up==&active->up);
 assert(eye->x==11&&eye->y==22&&eye->z==33);
 assert(at->x==13&&at->y==24&&at->z==35);
 assert(up->x==7&&up->y==11&&up->z==13);
 out->values[0]=21;trace.push_back("look-at");return out;
}
static AnmMatrixView *D3DXMatrixPerspectiveFovLH(AnmMatrixView*out,float fov,float aspect,float nearZ,float farZ){
 assert(trace.back()=="look-at"&&out==&active->projectionMatrix&&fov==0.75f);
 assert(nearZ==30&&farZ==1800);
 float expected=float(active->viewport.width)/float(active->viewport.height);
 assert(aspect==expected);out->values[0]=31;trace.push_back("perspective");return out;
}
static AnmFloat3View *D3DXVec3Normalize(AnmFloat3View*out,const AnmFloat3View*in){
 assert(trace.back()=="projection"&&out==&active->cameraRight&&in==out);
 assert(in->x==-3&&in->y==-4&&in->z==5);
 *out=AnmFloat3View(-0.3f,-0.4f,0.5f);
 g_AnmRenderManagerView=lateNull?nullptr:&second;trace.push_back("normalize");return out;
}
"""+source+r"""
int main(){
 StageCameraStateView c={};active=&c;c.position=AnmFloat3View(10,20,30);
 c.direction=AnmFloat3View(3,4,5);c.up=AnmFloat3View(7,11,13);
 c.rockingOffset=1;c.rockingOffsetY=2;c.rockingOffsetZ=3;c.value048=0.75f;
 c.viewport.width=640;c.viewport.height=480;c.value0E8=1.25f;c.value0EC=-2.5f;
 g_Direct3DDevice=&device1;otherDevice=&device2;StageConfigureCamera(&c);
 assert(trace==std::vector<std::string>({"flush","look-at","perspective","view","projection","normalize"}));
 assert(first.screenShakeX==0&&first.screenShakeY==0&&second.screenShakeX==1.25f&&second.screenShakeY==-2.5f);
 assert(c.cameraRight.x==-0.3f&&c.cameraRight.y==-0.4f&&c.cameraRight.z==0.5f);
 // High unsigned dimensions exercise conversion, and missing initial renderer skips only flush.
 trace.clear();g_AnmRenderManagerView=nullptr;second.screenShakeX=99;second.screenShakeY=98;c.viewport.width=0xF0000000u;c.viewport.height=0x80000000u;
 StageConfigureCamera(&c);assert(trace==std::vector<std::string>({"look-at","perspective","view","projection","normalize"}));
 assert(g_AnmRenderManagerView==&second&&second.screenShakeX==1.25f&&second.screenShakeY==-2.5f);
 // A renderer removed by the last external call must not receive publication.
 trace.clear();g_AnmRenderManagerView=&first;lateNull=true;second.screenShakeX=99;second.screenShakeY=98;
 c.viewport.width=640;c.viewport.height=0;StageConfigureCamera(&c);
 assert(g_AnmRenderManagerView==nullptr&&second.screenShakeX==99&&second.screenShakeY==98);
}
"""
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='stage-camera-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
