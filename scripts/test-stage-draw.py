#!/usr/bin/env python3
"""Exercise actual stage draw/bounds source with controlled projection outputs.

Synthetic D3DX/renderer sinks establish call and branch behavior, not native
D3DX math, x87 precision, object lifetime, or runtime/global alias closure.
"""
from pathlib import Path
import shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
class StageDraw(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_source(self):
  source=(ROOT/'src/StageDraw.cpp').read_text().replace('#include "StageScript.hpp"','')
  # The drawing fixture keeps a camera sink; a separate test runs that actual body.
  a=source.index('void StageConfigureCamera(StageCameraStateView *camera)\n{')
  b=source.index('// Target 0x403090',a)
  source=source[:a]+source[b:]
  h=(ROOT/'src/AnmManager.hpp').read_text();a=h.index('struct AnmFloat3View\n{');b=h.index('\n};',a)+3;vector=h[a:b]
  program=r"""
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>
#include <string>
#include <cstring>
#include <cstddef>
#define __stdcall
"""+vector+r"""
struct AnmMatrixView {float values[16];void SetIdentity(){for(int i=0;i<16;++i)values[i]=(i%5==0)?1:0;}};
struct AnmViewportView {unsigned x,y,width,height;float minZ,maxZ;};
struct AnmViewportOwnerView {char a[0x30];AnmFloat3View right;char b[0x10];AnmMatrixView view,projection;AnmViewportView viewport;};
struct StageCameraStateView {AnmFloat3View position,direction,up,normal;char a[12];float rockingOffset,rockingOffsetY,rockingOffsetZ,value048;AnmMatrixView viewMatrix,projectionMatrix;AnmViewportView viewport;};
struct StageObjectPrimitiveView {short type,byteSize,unknown004,vmIndex;};
struct StageQuadPrimitiveView {StageObjectPrimitiveView header;AnmFloat3View position;float width,height;};
struct StageObjectView {short id;unsigned char layer,flags;AnmFloat3View position,extent;StageObjectPrimitiveView firstPrimitive;};
struct StageInstanceView {short objectIndex,unknown002;AnmFloat3View position;};
struct Sprite {float width,height;};
struct AnmVmView {unsigned flags35C=0;AnmFloat3View position;float scaleX=1,scaleY=1;Sprite *loadedSprite=nullptr;};
struct StageScriptView {StageInstanceView *instances;StageObjectView **objects;AnmVmView *objectVms;float cullDistanceSquared;int culledObjectCount=0,visibleObjectCount=0,drawCount=0;};
static std::vector<std::string> trace;
static StageScriptView *active=nullptr;
static std::vector<AnmVmView*> drawn;
struct AnmRenderManagerView {
 unsigned char currentCameraMode=0;
 void FlushVertexBuffer(){trace.push_back("flush");}
 int Draw(AnmVmView *v){assert(currentCameraMode==1);drawn.push_back(v);trace.push_back("draw");return 0;}
};
struct D3d9DeviceView;
struct Vtable {long (*SetViewport)(D3d9DeviceView*,const void*);long (*SetRenderState)(D3d9DeviceView*,unsigned,unsigned);};
struct D3d9DeviceView {Vtable *vtable;};
StageCameraStateView g_StagePublishedCamera={};
AnmViewportOwnerView *g_AnmViewportOwner=nullptr;
int g_StageViewportConfigured=1,g_StageFogEnabled=0;
AnmRenderManagerView renderer;AnmRenderManagerView *g_AnmRenderManagerView=&renderer;
static long viewport(D3d9DeviceView*,const void *p){assert(p==&g_AnmViewportOwner->viewport);trace.push_back("viewport");return 0;}
static long renderState(D3d9DeviceView*,unsigned state,unsigned v){assert(state==28&&g_StageFogEnabled==int(v));assert(trace.back()=="flush");trace.push_back(v?"fog1":"fog0");return 0;}
Vtable vtable={viewport,renderState};D3d9DeviceView device={&vtable};D3d9DeviceView *g_Direct3DDevice=&device;
static int projects=0,translations=0;
static AnmFloat3View fixture[8],seenCorners[8],translation;
static const StageCameraStateView *expectedCamera;
static bool verifyCorners=false;
void StageConfigureCamera(StageCameraStateView*c){assert(c==&g_StagePublishedCamera);assert(g_AnmViewportOwner==reinterpret_cast<AnmViewportOwnerView*>(c));trace.push_back("camera");}
extern "C" AnmMatrixView *D3DXMatrixTranslation(AnmMatrixView*out,float x,float y,float z){
 ++translations;for(int i=0;i<16;++i)assert(out->values[i]==((i%5==0)?1:0));
 translation=AnmFloat3View(x,y,z);out->values[12]=x;out->values[13]=y;out->values[14]=z;return out;
}
extern "C" AnmFloat3View *D3DXVec3ProjectArray(AnmFloat3View*out,unsigned os,const AnmFloat3View*in,unsigned is,const AnmViewportView*vp,const AnmMatrixView*pr,const AnmMatrixView*view,const AnmMatrixView*world,unsigned n){
 ++projects;assert(os==12&&is==12&&n==8);
 assert(vp==&expectedCamera->viewport&&pr==&expectedCamera->projectionMatrix&&view==&expectedCamera->viewMatrix);
 assert(world->values[12]==translation.x&&world->values[13]==translation.y&&world->values[14]==translation.z);
 for(unsigned i=0;i<8;++i){seenCorners[i]=in[i];out[i]=fixture[i];}
 return out;
}
"""+source+r"""
static void visible(){for(int i=0;i<8;++i)fixture[i]=AnmFloat3View(100,100,0.5f);}
static int cull(StageCameraStateView &c,StageObjectView &o,const AnmFloat3View &p,float distance){expectedCamera=&c;return StageBoundsCulled(&c,&o,&p,distance);}
struct ObjectData {short id;unsigned char layer,flags;AnmFloat3View position,extent;StageQuadPrimitiveView p[3];StageObjectPrimitiveView end;};
int main(){
 StageCameraStateView c={};StageObjectView o={};o.position=AnmFloat3View(1,2,3);o.extent=AnmFloat3View(2,4,6);AnmFloat3View p(10,20,30);
 visible();assert(cull(c,o,p,1694)==0&&projects==1&&translations==1);
 for(int i=0;i<8;++i){assert(seenCorners[i].x==((i&4)?0:2));assert(seenCorners[i].y==((i&2)?0:4));assert(seenCorners[i].z==((i&1)?0:6));}
 assert(translation.x==10&&translation.y==20&&translation.z==30);
 assert(cull(c,o,p,std::nextafter(1694.0f,0.0f))==1&&projects==1);
 // Camera position plus all three rocking components contributes to distance.
 c.position=AnmFloat3View(1,2,3);c.rockingOffset=10;c.rockingOffsetY=20;c.rockingOffsetZ=30;
 assert(cull(c,o,p,0)==0);
 float nan=std::numeric_limits<float>::quiet_NaN();
 assert(cull(c,o,p,nan)==0); // unordered distance comparison does not reject
 // Both exact depth boundaries are included; the next float above1 is excluded.
 for(int i=0;i<8;++i)fixture[i].z=1;assert(cull(c,o,p,0)==0);
 for(int i=0;i<8;++i)fixture[i].z=std::nextafter(1.0f,std::numeric_limits<float>::infinity());assert(cull(c,o,p,0)==1);
 for(int i=0;i<8;++i)fixture[i].z=-0.1f;assert(cull(c,o,p,0)==1);
 for(int i=0;i<8;++i)fixture[i].z=nan;assert(cull(c,o,p,0)==1);
 for(int i=0;i<8;++i)fixture[i].z=0;assert(cull(c,o,p,0)==0);
 // The original else-if extrema means a lone point only updates minima.
 for(int i=0;i<8;++i)fixture[i].z=std::nextafter(1.0f,std::numeric_limits<float>::infinity());fixture[0].z=0;assert(cull(c,o,p,0)==1);
 visible();
 for(float x: {32.0f,416.0f}){for(int i=0;i<8;++i)fixture[i].x=x;assert(cull(c,o,p,0)==0);}
 for(float x: {31.999f,416.001f}){for(int i=0;i<8;++i)fixture[i].x=x;assert(cull(c,o,p,0)==1);}
 visible();
 for(float y: {16.0f,464.0f}){for(int i=0;i<8;++i)fixture[i].y=y;assert(cull(c,o,p,0)==0);}
 for(float y: {15.999f,464.001f}){for(int i=0;i<8;++i)fixture[i].y=y;assert(cull(c,o,p,0)==1);}
 visible();for(int i=0;i<8;++i)fixture[i].x=nan;assert(cull(c,o,p,0)==1);
 // Precision witnesses are models, not execution of the original or pinned image.
 // The actual host source also checks the float32 center-rounding expectation.
 StageCameraStateView savedC=c;StageObjectView savedO=o;AnmFloat3View savedP=p;
 c={};c.position.x=16777216.0f;o.position=AnmFloat3View(16777216.0f,0,0);p=AnmFloat3View(1,0,0);visible();
 assert(cull(c,o,p,0)==0);
 long double wideCenter=16777216.0L+1.0L;
 float roundedCenter=static_cast<float>(wideCenter);
 assert(roundedCenter==16777216.0f&&wideCenter-16777216.0L==1.0L);
 // x87 64-significand-bit accumulation distinguishes (Z2+Y2)+X2 from (X2+Y2)+Z2.
 if(std::numeric_limits<long double>::digits==64){
  long double big=18446744073709551616.0L,one=1.0L;
  long double targetSum=(one+one)+big,oldSum=(big+one)+one;
  assert(targetSum>big&&oldSum==big);
 }
 c=savedC;o=savedO;p=savedP;
 // Actual caller: signed layer, per-instance culling, primitive order, fog and draw counters.
 ObjectData data={};data.layer=255;data.extent=AnmFloat3View(2,4,6);data.end.type=-1;
 for(int i=0;i<3;++i){data.p[i].header.type=0;data.p[i].header.byteSize=sizeof(StageQuadPrimitiveView);data.p[i].header.vmIndex=i;data.p[i].position=AnmFloat3View(1+i,2+i,3+i);}
 data.p[0].width=20;data.p[0].height=12;
 data.p[1].width=0;data.p[1].height=0;data.p[2].width=70;data.p[2].height=90;
 StageObjectView *objects[]={reinterpret_cast<StageObjectView*>(&data)};
 StageInstanceView instances[3]={};instances[0].objectIndex=0;instances[0].position=AnmFloat3View(10,20,30);instances[1].objectIndex=0;instances[1].position=AnmFloat3View(1000,0,0);instances[2].objectIndex=-1;
 Sprite sprite={10,3};AnmVmView vms[3];for(int i=0;i<3;++i)vms[i].loadedSprite=&sprite;
 vms[0].flags35C=0x2000000;vms[1].flags35C=0x2000000;vms[2].flags35C=3;vms[2].position=AnmFloat3View(71,72,73);vms[2].scaleX=7;vms[2].scaleY=9;
 StageScriptView s={instances,objects,vms,2000};active=&s;expectedCamera=&g_StagePublishedCamera;
 visible();trace.clear();drawn.clear();g_StageFogEnabled=0;assert(StageDrawInstances(&s,-1)==0);
 assert(trace==std::vector<std::string>({"camera","viewport","flush","fog1","draw","draw","flush","fog0","draw"}));
 assert(drawn==std::vector<AnmVmView*>({&vms[0],&vms[1],&vms[2]}));
 assert(vms[0].position.x==11&&vms[0].position.y==22&&vms[0].position.z==33);
 assert(vms[0].scaleX==2&&vms[0].scaleY==4&&(vms[0].flags35C&8));
 assert(vms[1].scaleX==1&&vms[1].scaleY==1&&(vms[1].flags35C&8)==0);
 assert(vms[2].flags35C==3&&vms[2].position.x==71&&vms[2].position.y==72&&vms[2].position.z==73&&vms[2].scaleX==7&&vms[2].scaleY==9);
 assert(s.visibleObjectCount==1&&s.culledObjectCount==1&&s.drawCount==3&&(data.flags&2));
 assert(g_StageViewportConfigured==0&&renderer.currentCameraMode==1);
 trace.clear();drawn.clear();assert(StageDrawInstances(&s,255)==0);assert(drawn.empty()&&trace==std::vector<std::string>({"camera","viewport"}));
 // Nonzero primitive type is skipped; full dword fog value 256 must transition to 0.
 data.p[0].header.type=1;data.p[1].header.type=1;data.p[2].header.type=0;instances[1].objectIndex=-1;
 trace.clear();drawn.clear();g_StageFogEnabled=256;StageDrawInstances(&s,-1);
 assert(drawn==std::vector<AnmVmView*>({&vms[2]}));assert(trace==std::vector<std::string>({"camera","viewport","flush","fog0","draw"}));
}
"""
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='stage-draw-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
