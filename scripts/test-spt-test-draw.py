#!/usr/bin/env python3
"""Focused host observation of actual SptTest Draw; no x87/runtime claim."""
from pathlib import Path
import subprocess
import tempfile
import unittest
ROOT = Path(__file__).resolve().parents[1]

class SptTestDraw(unittest.TestCase):
    def test_actual_source_states_and_reloads(self):
        source = (ROOT/'src/SptTest.cpp').read_text()
        source = source[:source.index('SptTestView::SptTestView()')]
        source = '\n'.join(line for line in source.splitlines() if not line.startswith('#include'))
        program = r'''
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>
#define __fastcall
struct AnmFloat3View { float x,y,z; AnmFloat3View(float x=0,float y=0,float z=0):x(x),y(y),z(z){} };
struct Entry {const char *name;};
struct EclVmScriptDatabase {Entry *subroutines;};
struct Cursor {int current=0;};
struct SptTestView {int state=0;char **scriptNames=nullptr;int scriptCount=0;Cursor menu,files,entries;AnmFloat3View position;EclVmScriptDatabase *scriptDatabase=nullptr;int Draw();};
struct AsciiManagerView {int id;unsigned color;void AddSmallFormatText(AnmFloat3View *,const char *,...);};
static AsciiManagerView a{1,0},b{2,0},c{3,0},*g_AsciiManagerView;
struct Text {int manager;unsigned color;AnmFloat3View pos;std::string format,text;};
static std::vector<Text> texts;static std::function<void()> after;
void AsciiManagerView::AddSmallFormatText(AnmFloat3View *p,const char *fmt,...){char out[128];va_list args;va_start(args,fmt);vsnprintf(out,sizeof out,fmt,args);va_end(args);texts.push_back({id,color,*p,fmt,out});if(after)after();}
'''+source+r'''
static char n0[]="zero.ecl",n1[]="one.ecl";static char *names[]={n0,n1};
static Entry es[]={{"Zero"},{"One"}},other[]={{"Changed"},{"Later"}};static EclVmScriptDatabase db{es},alternate{other};
static SptTestView reset(int state){texts.clear();after={};a.color=0xff123456;b.color=0xff654321;c.color=0xffabcdef;g_AsciiManagerView=&a;SptTestView v;v.state=state;v.scriptNames=names;v.scriptCount=2;v.files.current=1;v.entries.current=1;v.scriptDatabase=&db;v.position={42.9f,-2.9f,13};return v;}
static void check(unsigned i,int manager,unsigned color,float x,float y,const char *s){assert(i<texts.size());auto &t=texts[i];assert(t.manager==manager&&t.color==color&&t.pos.x==x&&t.pos.y==y&&t.pos.z==0&&t.text==s);}
static void run(SptTestView &v){assert(SptTestDrawCallback(&v)==1);check(0,1,0xff123456,0,0,"Spt Test\n");}
int main(){
 for(int state:{-1,0,4,5,2147483647}){auto v=reset(state);run(v);assert(texts.size()==1&&a.color==0xff123456);}
 {auto v=reset(2);v.scriptCount=0;run(v);assert(texts.size()==2);check(1,1,0xffff4040,32,16,"Loading one.ecl");assert(a.color==0xffffffff);}
 {auto v=reset(3);run(v);assert(texts.size()==2);check(1,1,0xffa0a0a0,32,16,"Pos 042 -002");assert(a.color==0xffffffff);}
 for(bool files:{false,true})for(bool loaded:{false,true})for(int selection:{-2,0,3}){
  auto v=reset(1);v.scriptCount=files?2:0;v.scriptDatabase=loaded?&db:nullptr;v.menu.current=selection;run(v);assert(texts.size()==5);
  check(1,1,0xff123456,42,16,files?"File one.ecl":"File not found.");check(2,1,0xff123456,42,26,loaded?"Ecl One":"Ecl not load");
  check(3,1,0xff123456,42,36,"Quit");check(4,1,0xff123456,32,selection*10+16,">");assert(a.color==0xffffffff);
 }
 // Title can mutate state and globally published manager; each sink is a call.
 {auto v=reset(0);after=[&](){if(texts.size()==1){v.state=2;v.files.current=0;g_AsciiManagerView=&b;}else g_AsciiManagerView=&c;};run(v);check(1,2,0xffff4040,32,16,"Loading zero.ecl");assert(c.color==0xffffffff&&b.color==0xffff4040);}
 {auto v=reset(1);after=[&](){if(texts.size()==1)g_AsciiManagerView=&b;if(texts.size()==2){v.scriptDatabase=&alternate;v.entries.current=0;}if(texts.size()==3){g_AsciiManagerView=&c;v.menu.current=4;}if(texts.size()==5)g_AsciiManagerView=&a;};run(v);check(1,2,0xff654321,42,16,"File one.ecl");check(2,2,0xff654321,42,26,"Ecl Changed");check(3,3,0xffabcdef,42,36,"Quit");check(4,3,0xffabcdef,32,56,">");assert(a.color==0xffffffff&&c.color==0xffabcdef);}
 {auto v=reset(3);after=[&](){if(texts.size()==1){g_AsciiManagerView=&b;v.position={-9.8f,7.9f,0};}else g_AsciiManagerView=&c;};run(v);check(1,2,0xffa0a0a0,32,16,"Pos -009 007");assert(c.color==0xffffffff&&b.color==0xffa0a0a0);}
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            p=Path(tmp);(p/'test.cpp').write_text(program)
            subprocess.run(['c++','-std=c++17','-O2',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
            subprocess.run([str(p/'test')],check=True)

if __name__ == '__main__': unittest.main()
