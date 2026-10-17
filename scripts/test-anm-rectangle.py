#!/usr/bin/env python3
"""Local host contract for complete actual rectangle source; no game execution.
Extracts actual rectangle/vertex and D3D declarations. Manager is an explicit
bounded dependency mock, so this does not validate full manager ABI/layout.
"""
import pathlib,re,subprocess,tempfile,os,argparse
ROOT=pathlib.Path(__file__).resolve().parents[1]
ap=argparse.ArgumentParser()
ap.add_argument('--root',type=pathlib.Path,default=ROOT)
ROOT=ap.parse_args().root
def read(name):
    p=ROOT/'src'/name
    if not p.exists(): p=ROOT/'baseline/src'/name
    return p.read_text()
header=read('AnmManager.hpp')+'\n'+read('AnmRectangle.hpp')
def struct(name):
    start=header.index('struct '+name+'\n{'); end=header.index('\n};',start)+3
    return header[start:end]
d3d=read('D3d9View.hpp')
d3d=d3d.replace('#pragma once','')
d3d=re.sub(r'typedef char\s+\w+\[.*?\];','',d3d,flags=re.S)
source=read('AnmRectangle.cpp').replace('#include "AnmManager.hpp"','').replace('#include "AnmRectangle.hpp"','')
preamble='''#include <cstdlib>
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "CHECK:%d:%s\\n", __LINE__, #x); exit(86); } } while(0)
#include <cstring>
#include <cstdio>
#include <cstddef>
#define __stdcall
'''+d3d+'\n'+struct('AnmFloatRectView')+'\n'+struct('AnmUntexturedVertexView')+'''
struct AnmRenderManagerView {
 unsigned char currentVertexShader,currentColorOperation,currentBlendMode,currentZWrite;
 void *currentSprite,*currentTexture;
 void FlushVertexBuffer();
};
AnmRenderManagerView managers[2];
AnmRenderManagerView *g_AnmRenderManagerView;
D3d9DeviceView devices[12];
D3d9DeviceView *g_Direct3DDevice;
AnmFloatRectView *input;
AnmFloatRectView expected;
unsigned int expectedColor;
int eventIndex, flushCount;
bool mutateFlush;
void AnmRenderManagerView::FlushVertexBuffer() {
 CHECK(this==&managers[0]); ++flushCount;
 if(mutateFlush) { input->left=19; input->right=-31; input->top=-4; input->bottom=9; }
 expected=*input;
 g_Direct3DDevice=&devices[0];
}
void event(D3d9DeviceView *d) {
 CHECK(d==&devices[eventIndex]);
 ++eventIndex; g_Direct3DDevice=&devices[eventIndex];
}
long stage(D3d9DeviceView *d,unsigned int s,unsigned int t,unsigned int v) {
 CHECK(flushCount==1);
 const unsigned int expectedT[11]={4,1,5,2,0,0,0,4,1,5,2};
 const unsigned int expectedV[11]={2,2,0,0,0,0,0,4,4,2,2};
 CHECK(eventIndex<11 && s==0 && t==expectedT[eventIndex] && v==expectedV[eventIndex]);
 if(eventIndex==7) {
  CHECK(managers[1].currentVertexShader==255 && managers[1].currentColorOperation==255);
  CHECK(managers[1].currentBlendMode==3 && managers[1].currentZWrite==255);
  CHECK(!managers[1].currentSprite && !managers[1].currentTexture);
  CHECK(managers[0].currentBlendMode==77);
 }
 event(d); return -1;
}
long state(D3d9DeviceView *d,unsigned int s,unsigned int v) {
 CHECK(eventIndex==4 && s==20 && v==6); event(d); return -2;
}
long fvf(D3d9DeviceView *d,unsigned int v) {
 CHECK(eventIndex==5 && v==0x44); event(d); return -3;
}
long draw(D3d9DeviceView *d,unsigned int p,unsigned int n,const void *data,unsigned int stride) {
 CHECK(eventIndex==6 && p==5 && n==2 && stride==20);
 const AnmUntexturedVertexView *v=(const AnmUntexturedVertexView*)data;
 for(int i=0;i<4;++i) {
  float x=i%2?expected.right:expected.left;
  float y=i/2?expected.bottom:expected.top;
  CHECK(memcmp(&v[i].x,&x,4)==0 && memcmp(&v[i].y,&y,4)==0);
  CHECK(v[i].z==0 && !memcmp(&v[i].z,"\\0\\0\\0\\0",4));
  CHECK(v[i].w==1 && v[i].color==expectedColor);
 }
 g_AnmRenderManagerView=&managers[1]; event(d); return -4;
}
'''
main='''
int main() {
 D3d9DeviceVtableView table={}; table.SetTextureStageState=stage;
 table.SetRenderState=state; table.SetFVF=fvf; table.DrawPrimitiveUP=draw;
 for(int i=0;i<12;++i) devices[i].vtable=&table;
 unsigned int bits[]={0,0x80000000,0x3f800000,0xbf800000,0x7fc01234,0x7f800000,0xff800000,0x00000001};
 for(int m=0;m<2;++m) for(unsigned int b:bits) for(unsigned int color:bits) {
  AnmFloatRectView r; memcpy(&r.left,&b,4); r.top=-2.5f; r.right=83.25f; r.bottom=0.0f;
  for(int j=0;j<2;++j) {
   managers[j].currentVertexShader=managers[j].currentColorOperation=77;
   managers[j].currentBlendMode=managers[j].currentZWrite=77;
   managers[j].currentSprite=&devices[0]; managers[j].currentTexture=&devices[1];
  }
  g_AnmRenderManagerView=&managers[0];
  g_Direct3DDevice=&devices[0]; expected=r; input=&r; mutateFlush=m; expectedColor=color;
  eventIndex=flushCount=0; DrawColoredRectangle(&r,color);
  CHECK(eventIndex==11 && flushCount==1 && !memcmp(&r,&expected,sizeof(r)));
 }
 puts("128 rectangle contract cases passed");
}
'''
def require(condition, message):
    if not condition:
        raise RuntimeError(message)
def run(body,flags):
    with tempfile.TemporaryDirectory() as tmp:
        p=pathlib.Path(tmp); (p/'test.cpp').write_text(preamble+body+main)
        compiled=subprocess.run(['g++','-std=c++11',*flags,str(p/'test.cpp'),'-o',str(p/'test')],capture_output=True,timeout=60)
        require(compiled.returncode==0,'compilation failed: '+compiled.stderr.decode())
        env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
        return subprocess.run([str(p/'test')],capture_output=True,timeout=20,env=env)
def positive(result,label):
    require(result.returncode==0 and result.stdout==b'128 rectangle contract cases passed\n' and result.stderr==b'',label+' unexpected outcome: '+repr((result.returncode,result.stdout,result.stderr)))
mutations={
 'omit_flush':('g_AnmRenderManagerView->FlushVertexBuffer();',''),
 'wrong_left':('vertices[0].x = rectangle->left;','vertices[0].x = rectangle->right;'),
 'wrong_bottom':('vertices[2].y = rectangle->bottom;','vertices[2].y = rectangle->top;'),
 'wrong_z':('vertices[0].z = 0.0f;','vertices[0].z = 1.0f;'),
 'wrong_w':('vertices[0].w = 1.0f;','vertices[0].w = 0.0f;'),
 'wrong_color':('vertices[0].color = color;','vertices[0].color = color ^ 1;'),
 'wrong_state':('D3D9_VIEW_RS_DESTBLEND','D3D9_VIEW_RS_ZWRITEENABLE'),
 'wrong_primitive':('D3D9_VIEW_PT_TRIANGLESTRIP, 2','D3D9_VIEW_PT_TRIANGLELIST, 2'),
 'wrong_count':('D3D9_VIEW_PT_TRIANGLESTRIP, 2','D3D9_VIEW_PT_TRIANGLESTRIP, 1'),
 'wrong_fvf':('D3D9_VIEW_FVF_XYZRHW | D3D9_VIEW_FVF_DIFFUSE','D3D9_VIEW_FVF_XYZRHW'),
 'stale_manager':('AnmRenderManagerView *manager = g_AnmRenderManagerView;','AnmRenderManagerView *manager = &managers[0];'),
 'omit_texture_reset':('manager->currentTexture = NULL;',''),
 'omit_sprite_reset':('manager->currentSprite = NULL;',''),
 'wrong_cache':('manager->currentBlendMode = 3;','manager->currentBlendMode = 0;'),
 'wrong_restore':('D3D9_VIEW_TOP_MODULATE','D3D9_VIEW_TOP_SELECTARG1'),
 'stale_device':('g_Direct3DDevice','(&devices[0])'),
}
expected_checks={
 'omit_flush':'flushCount==1',
 'wrong_left':'memcmp(&v[i].x,&x,4)==0 && memcmp(&v[i].y,&y,4)==0',
 'wrong_bottom':'memcmp(&v[i].x,&x,4)==0 && memcmp(&v[i].y,&y,4)==0',
 'wrong_z':'v[i].z==0 && !memcmp(&v[i].z,"\\0\\0\\0\\0",4)',
 'wrong_w':'v[i].w==1 && v[i].color==expectedColor',
 'wrong_color':'v[i].w==1 && v[i].color==expectedColor',
 'wrong_state':'eventIndex==4 && s==20 && v==6',
 'wrong_primitive':'eventIndex==6 && p==5 && n==2 && stride==20',
 'wrong_count':'eventIndex==6 && p==5 && n==2 && stride==20',
 'wrong_fvf':'eventIndex==5 && v==0x44',
 'stale_manager':'managers[1].currentVertexShader==255 && managers[1].currentColorOperation==255',
 'omit_texture_reset':'!managers[1].currentSprite && !managers[1].currentTexture',
 'omit_sprite_reset':'!managers[1].currentSprite && !managers[1].currentTexture',
 'wrong_cache':'managers[1].currentBlendMode==3 && managers[1].currentZWrite==255',
 'wrong_restore':'eventIndex<11 && s==0 && t==expectedT[eventIndex] && v==expectedV[eventIndex]',
 'stale_device':'d==&devices[eventIndex]',
}
def diagnostic(body,expression):
    needle='CHECK('+expression+');'
    lines=[i for i,line in enumerate((preamble+body+main).splitlines(),1) if needle in line]
    require(len(lines)==1,'ambiguous/missing check '+expression)
    return ('CHECK:'+str(lines[0])+':'+expression+'\n').encode()
for optimization in ['-O0','-O2']:
    positive(run(source,[optimization]),optimization+' positive')
    for name,(old,new) in mutations.items():
        require(old in source,'missing mutation anchor '+name)
        body=source.replace(old,new)
        expected_stderr=diagnostic(body,expected_checks[name])
        result=run(body,[optimization])
        require(result.returncode==86 and result.stdout==b'' and result.stderr==expected_stderr,
                optimization+' '+name+' wrong failure: '+repr((result.returncode,result.stdout,result.stderr))+' expected '+repr(expected_stderr))
    print(optimization+': 128 cases passed; 16 exact CHECK mutations rejected')
positive(run(source,['-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie']), 'ASan+UBSan positive')
print('ASan+UBSan: 128 cases passed; detect_leaks=0 (local sandbox)')
