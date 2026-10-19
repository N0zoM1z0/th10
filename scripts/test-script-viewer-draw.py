#!/usr/bin/env python3
"""Observe actual browser Draw source against TH10-local display/call contracts.

Minimal typed host sinks cover state exits, colors, positions, global reloads and
callback forwarding. They do not establish i386 layout, x87 edge behavior or a
Windows renderer/runtime. Float-to-int witnesses stay finite and in int range.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


def body(signature):
    source = (ROOT / 'src/ScriptViewer.cpp').read_text()
    start = source.index(signature)
    end, depth = source.index('{', start) + 1, 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


class BrowserDraw(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_actual_draw_contracts(self):
        program = r'''
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>
#define __fastcall
struct AnmFloat3View {
 float x,y,z;
 AnmFloat3View(float x=0,float y=0,float z=0):x(x),y(y),z(z){}
};
static bool same(AnmFloat3View a,AnmFloat3View b){return a.x==b.x&&a.y==b.y&&a.z==b.z;}
struct AnmVmView {AnmFloat3View position;};
struct EclVmSubroutineEntry {const char *name;};
struct EclVmScriptDatabase {EclVmSubroutineEntry *subroutines;};
using EnemyEclResourceView=EclVmScriptDatabase;
struct EnemyManagerView {EnemyEclResourceView *scriptDatabase;int activeEnemyCount;};
struct AsciiManagerView {
 int id;unsigned color;
 void AddSmallFormatText(AnmFloat3View *,const char *,...);
};
struct AnmRenderManagerView {int id;int DrawNoRotation(AnmVmView *);};
struct Cursor {int current=0;};
struct ScriptViewerView {
 int state=0;char **scriptNames=nullptr;int scriptCount=0;
 Cursor files,entries,menu;AnmVmView vm;AnmFloat3View position;unsigned flags=0;
 int Draw();
};
static AsciiManagerView asciiA{1,0},asciiB{2,0},asciiC{3,0};
static AsciiManagerView *g_AsciiManagerView;
static EnemyManagerView *g_EnemyManager;
static AnmRenderManagerView rendererA{11},rendererB{12},*g_AnmRenderManagerView;
struct Text {int manager;unsigned color;AnmFloat3View position;std::string format,text;};
static std::vector<Text> texts;
static std::function<void()> afterText;
static AnmVmView *rendered;
static int renderingManager;
static AnmFloat3View renderedPosition;
void AsciiManagerView::AddSmallFormatText(AnmFloat3View *position,const char *format,...){
 char buffer[128];va_list args;va_start(args,format);std::vsnprintf(buffer,sizeof buffer,format,args);va_end(args);
 texts.push_back({id,color,*position,format,buffer});if(afterText)afterText();
}
int AnmRenderManagerView::DrawNoRotation(AnmVmView *vm){
 assert(!rendered);rendered=vm;renderingManager=id;renderedPosition=vm->position;return -77;
}
'''
        program += body('int ScriptViewerView::Draw()') + '\n'
        program += body('int __fastcall ScriptViewerDrawCallback(') + '\n'
        program += r'''
static char name0[]="alpha.ecl",name1[]="beta.ecl";static char *names[]={name0,name1};
static EclVmSubroutineEntry entries[]={{"First"},{"Second"}},alternateEntries[]={{"Changed"},{"AfterFile"}};
static EclVmScriptDatabase database{entries},alternateDatabase{alternateEntries};
static EnemyManagerView enemies{&database,37},alternateEnemies{&alternateDatabase,81};
static ScriptViewerView reset(int state,unsigned flags=0){
 texts.clear();afterText={};rendered=nullptr;renderingManager=0;
 asciiA.color=0xff123456;asciiB.color=0xff654321;asciiC.color=0xffabcdef;
 g_AsciiManagerView=&asciiA;g_EnemyManager=&enemies;g_AnmRenderManagerView=&rendererA;
 ScriptViewerView v;v.state=state;v.flags=flags;v.scriptNames=names;v.scriptCount=2;
 v.files.current=1;v.entries.current=1;v.vm.position=AnmFloat3View(7,8,9);v.position=AnmFloat3View(42.9f,-2.9f,13);
 return v;
}
static void text(unsigned i,int manager,unsigned color,AnmFloat3View position,const char *expected){
 assert(i<texts.size());auto &t=texts[i];assert(t.manager==manager&&t.color==color&&same(t.position,position)&&t.text==expected);
}
static void title(){text(0,1,0xff123456,AnmFloat3View(0,0,0),"SprtView\n");assert(texts[0].format=="SprtView\n");}
static void run(ScriptViewerView &v){assert(ScriptViewerDrawCallback(&v)==1);title();}
static void drawCheck(ScriptViewerView &v,bool expected,int manager=11){
 if(expected){assert(rendered==&v.vm&&renderingManager==manager&&same(renderedPosition,AnmFloat3View(320,240,0)));}
 else assert(!rendered&&same(v.vm.position,AnmFloat3View(7,8,9)));
}
int main(){
 for(int state:{-1,0,5,2147483647})for(unsigned flags:{0u,2u}){
  auto v=reset(state,flags);run(v);assert(texts.size()==1&&asciiA.color==0xff123456);drawCheck(v,false);
 }
 for(unsigned flags:{0u,2u}){
  auto v=reset(2,flags);v.scriptCount=0;run(v);assert(texts.size()==2);
  text(1,1,0xffff4040,AnmFloat3View(32,16,0),"Loading beta.ecl");assert(asciiA.color==0xffffffff);drawCheck(v,false);
  v=reset(4,flags);run(v);assert(texts.size()==2);
  text(1,1,0xff123456,AnmFloat3View(500,240,0),"Enemy 37");assert(asciiA.color==0xff123456);drawCheck(v,false);
 }
 for(bool files:{false,true})for(bool manager:{false,true})for(int selection:{-2,0,3})for(unsigned flags:{0u,2u}){
  auto v=reset(1,flags);v.scriptCount=files?2:0;v.menu.current=selection;if(!manager)g_EnemyManager=nullptr;
  run(v);assert(texts.size()==5);
  text(1,1,0xff123456,AnmFloat3View(42,16,0),files?"File beta.ecl":"File not found.");
  text(2,1,0xff123456,AnmFloat3View(42,26,0),manager?"Ecl Second":"Ecl 1");
  text(3,1,0xff123456,AnmFloat3View(42,36,0),"Quit");
  text(4,1,0xff123456,AnmFloat3View(32,selection*10.0f+16.0f,0),">");
  assert(asciiA.color==0xffffffff);drawCheck(v,(flags&2)!=0);
 }
 for(unsigned flags:{0u,2u}){
  auto v=reset(3,flags);run(v);assert(texts.size()==2);
  text(1,1,0xffa0a0a0,AnmFloat3View(32,16,0),"Pos 042 -002");
  assert(asciiA.color==0xffffffff&&same(v.position,AnmFloat3View(42.9f,-2.9f,13)));drawCheck(v,(flags&2)!=0);
 }
 // Calls may replace published managers; later calls and the final color store reload them.
 auto v=reset(2,2);afterText=[](){if(texts.size()==1)g_AsciiManagerView=&asciiB;else g_AsciiManagerView=&asciiC;};
 run(v);text(1,2,0xffff4040,AnmFloat3View(32,16,0),"Loading beta.ecl");
 assert(asciiB.color==0xffff4040&&asciiC.color==0xffffffff);drawCheck(v,false);
 v=reset(1);afterText=[&](){
  if(texts.size()==2){g_AsciiManagerView=&asciiB;g_EnemyManager=&alternateEnemies;}
  if(texts.size()==5){g_AsciiManagerView=&asciiC;g_AnmRenderManagerView=&rendererB;v.flags=2;}
 };
 run(v);text(2,2,0xff654321,AnmFloat3View(42,26,0),"Ecl AfterFile");
 assert(asciiC.color==0xffffffff&&asciiA.color==0xff123456&&asciiB.color==0xff654321);drawCheck(v,true,12);
 v=reset(3,2);afterText=[](){if(texts.size()==2)g_AsciiManagerView=&asciiB;};run(v);
 assert(asciiA.color==0xffa0a0a0&&asciiB.color==0xffffffff);drawCheck(v,true);
}
'''
        with tempfile.TemporaryDirectory(prefix='th10-browser-draw-') as tmp:
            source, binary = Path(tmp) / 'fixture.cpp', Path(tmp) / 'fixture'
            source.write_text(program)
            subprocess.run(['c++', '-std=c++17', '-O2', str(source), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    unittest.main()
