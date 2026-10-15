#!/usr/bin/env python3
"""Run maintained music-room/parsers on bounded synthetic state and event sinks.

This does not execute game IO or establish runtime/codegen closure. Sentinel
padding bounds these fixtures. Read-before-count ordering is source-audited;
these output tests do not prove that ordering or production input safety.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
ROOT = Path(__file__).resolve().parents[1]


def function_source(signature):
    source = (ROOT / 'src/FrontEnd.cpp').read_text()
    start = source.index(signature)
    brace = source.index('{', start)
    depth, end = 1, brace + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


class MusicRoomState(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_actual_owner_and_parser_bodies(self):
        program = r'''
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <string>
#include <vector>
#define __stdcall
struct AnmFloat3View { float x,y,z; AnmFloat3View(float a=0,float b=0,float c=0):x(a),y(b),z(c){} };
struct Timer { int current=0, previous=-1; void SetCurrent(int n) {previous=n-1;current=n;} };
struct Interpolation { int duration=0, mode=0; AnmFloat3View initialTangent,finalTangent,initial,final; Timer timer; };
struct AnmVmView {
    unsigned flags35C=0; short pendingInterrupt=0; AnmFloat3View position; Interpolation positionInterpolation;
    void StartPositionInterpolation(const AnmFloat3View *,const AnmFloat3View *,int,unsigned char);
};
static AnmVmView pool[512]; static int nextId;
struct AnmVmIdView { int value=0; AnmVmView *GetVm() {return value?&pool[value]:NULL;} void SetInterrupt(short n) {if(value)pool[value].pendingInterrupt=n;} };
struct Loaded { AnmVmIdView CreateVmVariant0(int,unsigned layer) { assert(layer==15); AnmVmIdView id;id.value=++nextId;assert(nextId<512);pool[nextId]=AnmVmView();return id;} };
struct Cursor {
    int count=0,current=0,previous=0,pops=0;
    void SetCurrent(int n) {current=count==0?0:(n>=count?count-1:(n<0?0:n));}
    void Move(int n) {current+=n;if(current>=count)current=0;if(current<0)current=count-1;}
    void Pop() {++pops;}
};
struct FrontEndControllerView {
    int screen=0,screenState=0; Timer stateTimer; Cursor cursor;
    Loaded *frontAnm=NULL; AnmVmIdView vmIds[256], difficultyAuxVmId;
    AnmVmIdView musicRowVmIds[20],musicCommentVmIds[8];
    int musicTrackCount=0,musicCommentLine=0,musicSelectedTrack=0,musicWarning=0,musicListOffset=0;
    char musicPaths[32][0x40]{},musicTitles[32][0x42]{},musicComments[32][8][0x42]{};
    char *musicText=NULL;
    void SetScreenStateTarget(int n) {screenState=n;stateTimer.SetCurrent(0);}
    void SetScreenTarget(int n) {screen=n;screenState=0;stateTimer.SetCurrent(0);}
};
struct Input {unsigned short pressed=0,repeated=0;} g_FrontEndInput;
static unsigned char profile[0x20000]; static unsigned char *g_FrontEndProfileData=profile;
static unsigned g_FrontEndSupervisorFlags;
static AnmFloat3View g_AnmZeroVector491C14;
struct Ascii {Loaded *asciiAnm;} ascii;
static Ascii *g_AsciiManagerView=&ascii;
static Loaded *g_FrontEndMusicCommentAnm;
static std::vector<std::string> events,drawn;
static std::vector<unsigned> colors; static std::vector<AnmVmView *> drawReceivers;
struct Render {
    AnmVmView *FindVm(AnmVmIdView id) {return id.GetVm();}
    void DrawTextLeft(AnmVmView *vm,unsigned color,const char *text,...) {assert(vm);drawn.push_back(text);colors.push_back(color);drawReceivers.push_back(vm);}
} render;
static Render *g_AnmRenderManagerView=&render;
static const char g_MusicLockedTitleFormat[]="locked %d";
static const char *g_MusicWarningLines[8]={"w0","w1","w2","w3","w4","w5","w6","w7"};
static std::string fileText; static bool fileFail;
namespace FileSystem { unsigned char *OpenFile(const char *path,int *size,int mode) {
    assert(std::string(path)=="musiccmt.txt" && mode==0); *size=int(fileText.size()); if(fileFail)return NULL;
    char *p=(char *)calloc(fileText.size()+8,1);memcpy(p,fileText.data(),fileText.size());return (unsigned char *)p;
} }
static void CreateVm(FrontEndControllerView *c,int n){c->vmIds[n]=c->frontAnm->CreateVmVariant0(n,15);}
static AnmVmView *ResolveVm(AnmVmIdView *id){return id->GetVm();}
static int InputRepeated(unsigned short mask){return ((g_FrontEndInput.pressed|g_FrontEndInput.repeated)&mask)!=0;}
static void ResetStateTimer(FrontEndControllerView *c){c->stateTimer.SetCurrent(0);}
static void SetScreenState(FrontEndControllerView *c,int n){c->SetScreenStateTarget(n);}
static void FrontEndPlaySound(int n){events.push_back("sound:"+std::to_string(n));}
static void FrontEndQueueSoundCommand(const char *s,int mode,int value){assert(std::string(s)=="dummy" && value==0);events.push_back("command:"+std::to_string(mode));}
static void FrontEndLoadMusic(int slot,const char *path){assert(slot==0);events.push_back("load:"+std::string(path));}
static void FrontEndPlayMusic(int slot,int loop){assert(slot==0&&loop==0);events.push_back("play");}
static void EnemyMarkPendingInterrupt(int *id){if(*id)pool[*id].pendingInterrupt=1;}
static void DeleteVm(FrontEndControllerView *c,int n){c->vmIds[n].SetInterrupt(1);c->vmIds[n].value=0;}
static void InterruptVmNow(FrontEndControllerView *c,int n,short interrupt){c->vmIds[n].SetInterrupt(interrupt);events.push_back("interrupt:"+std::to_string(n)+":"+std::to_string(interrupt));}
'''
        for signature in ['static char *FrontEndMusicSkipLine(', 'static char *__stdcall FrontEndMusicCopyLine(', 'void AnmVmView::StartPositionInterpolation(', 'int __stdcall FrontEndUpdateMusicRoom(FrontEndControllerView *controller)\n{']:
            program += '\n' + function_source(signature) + '\n'
        program += r'''
int main() {
    char skipped[]="#x\r\n\nY\0\0";int remaining=6;
    assert(FrontEndMusicSkipLine(skipped,&remaining)==skipped+5 && remaining==1);
    char cp932[]={char(0x82),char(0xa0),'\r','\n','X',0,0};char copied[16]="keep";remaining=5;
    assert(FrontEndMusicCopyLine(cp932,copied,&remaining)==cp932+4 && remaining==1);
    assert((unsigned char)copied[0]==0x82 && (unsigned char)copied[1]==0xa0 && copied[2]==0 && cp932[2]==0);
    char unterminated[]="abc\0\0";strcpy(copied,"keep");remaining=3;
    assert(FrontEndMusicCopyLine(unterminated,copied,&remaining)==unterminated+3 && remaining==0);
    assert(std::string(copied)=="keep");remaining=0;
    assert(FrontEndMusicCopyLine(unterminated+3,copied,&remaining)==unterminated+3 && std::string(copied)=="keep");
    Loaded front,comments,asciiResource;ascii.asciiAnm=&asciiResource;g_FrontEndMusicCommentAnm=&comments;
    FrontEndControllerView c;c.frontAnm=&front;c.stateTimer.current=1;
    fileText="# heading\r\n";
    for(int track=0;track<2;++track){fileText += track?"@bgm/b.mid\r\nTitleB\r\n":"@bgm/a.mid\r\nTitleA\r\n";for(int line=0;line<8;++line)fileText+=std::string(track?"B":"A")+std::to_string(line)+"\r\n";}
    profile[0x1d892]=1;
    assert(FrontEndUpdateMusicRoom(&c)==0);
    assert(c.musicTrackCount==2 && c.cursor.count==2 && c.musicText);
    assert(std::string(c.musicPaths[0])=="bgm/a.mid" && std::string(c.musicPaths[1])=="bgm/b.mid");
    assert(std::string(c.musicTitles[1])=="TitleB" && std::string(c.musicComments[1][7])=="B7");
    assert(drawn==std::vector<std::string>({"TitleA","locked %d"}));
    assert(pool[c.musicRowVmIds[0].value].positionInterpolation.final.x==60);
    assert(pool[c.musicRowVmIds[1].value].positionInterpolation.final.y==116);
    c.stateTimer.current=10;FrontEndUpdateMusicRoom(&c);assert(c.screenState==1&&c.stateTimer.current==0);
    drawn.clear();FrontEndUpdateMusicRoom(&c);assert(c.musicCommentLine==1&&drawn.back()=="A0");
    c.stateTimer.current=6;FrontEndUpdateMusicRoom(&c);assert(c.screenState==2&&c.musicCommentLine==2&&c.stateTimer.current==0);
    c.stateTimer.current=1;g_FrontEndInput.pressed=0x20;
    AnmVmView &second=pool[c.musicRowVmIds[1].value];second.position.y=std::numeric_limits<float>::quiet_NaN();second.positionInterpolation.duration=77;
    FrontEndUpdateMusicRoom(&c);assert(c.cursor.current==1&&c.musicWarning==0);
    assert(second.position.y==116&&second.position.x==60&&second.positionInterpolation.duration==77);
    events.clear();g_FrontEndInput.pressed=0x1001;FrontEndUpdateMusicRoom(&c);
    assert(c.musicWarning==1&&c.musicSelectedTrack==1&&c.musicCommentLine==0);
    assert(events==std::vector<std::string>({"command:3"}));
    // Warning rendering uses the selected comment VM and warning color.
    g_FrontEndInput.pressed=0;drawn.clear();colors.clear();drawReceivers.clear();
    c.stateTimer.current=0;FrontEndUpdateMusicRoom(&c);
    assert(drawn==std::vector<std::string>({"w0"})&&colors==std::vector<unsigned>({0x8080ff}));
    assert(drawReceivers[0]==&pool[c.musicCommentVmIds[0].value]&&drawReceivers[0]->pendingInterrupt==2);
    // Confirm wins over simultaneous cancel, including the successful play path.
    g_FrontEndInput.pressed=0x100b;
    c.stateTimer.current=1;events.clear();profile[0x1d892]=0;g_FrontEndSupervisorFlags=0x10;
    FrontEndUpdateMusicRoom(&c);assert(c.musicWarning==0);
    assert(events==std::vector<std::string>({"load:bgm/b.mid","command:4","command:2"}));
    assert(profile[0x1d892]==1&&profile[0x1d893]==0); // target writes track zero
    assert(c.screenState==2&&c.musicText!=NULL&&c.cursor.pops==0);
    c.stateTimer.current=1;g_FrontEndInput.pressed=0x0a;events.clear();
    int rowId=c.musicRowVmIds[0].value;FrontEndUpdateMusicRoom(&c);
    assert(c.musicText==NULL&&c.screenState==3&&c.cursor.pops==1);
    assert(c.musicRowVmIds[0].value==rowId&&pool[rowId].pendingInterrupt==1);
    assert(events==std::vector<std::string>({"sound:11"}));
    c.stateTimer.current=10;events.clear();FrontEndUpdateMusicRoom(&c);
    assert(c.screen==2&&c.screenState==0&&c.cursor.pops==2);
    assert(events==std::vector<std::string>({"interrupt:90:8","interrupt:91:8","load:bgm/th10_02.wav","play"}));
    // Twelve rows distinguish both scroll boundaries and visible-window flags.
    FrontEndControllerView scrolling;scrolling.frontAnm=&front;scrolling.screenState=2;
    scrolling.stateTimer.current=1;scrolling.musicTrackCount=12;scrolling.cursor.count=12;scrolling.cursor.current=9;
    for(int i=0;i<12;++i){scrolling.musicRowVmIds[i]=front.CreateVmVariant0(i,15);pool[scrolling.musicRowVmIds[i].value].flags35C=0x80;}
    g_FrontEndInput.pressed=0x20;FrontEndUpdateMusicRoom(&scrolling);
    assert(scrolling.cursor.current==10&&scrolling.musicListOffset==1);
    for(int i=0;i<12;++i)assert(pool[scrolling.musicRowVmIds[i].value].flags35C==unsigned(0x80|((i>=1&&i<11)?2:0)));
    scrolling.cursor.current=1;g_FrontEndInput.pressed=0x10;FrontEndUpdateMusicRoom(&scrolling);
    assert(scrolling.cursor.current==0&&scrolling.musicListOffset==0);
    for(int i=0;i<12;++i)assert(pool[scrolling.musicRowVmIds[i].value].flags35C==unsigned(0x80|(i<10?2:0)));
    // Exercise the actual owner branch on either side of the exact threshold.
    float deltas[]={39.999f,40.0f,40.001f,std::numeric_limits<float>::quiet_NaN()};
    for(int n=0;n<4;++n){
        scrolling.cursor.current=0;scrolling.musicListOffset=0;g_FrontEndInput.pressed=0x20;
        AnmVmView &vm=pool[scrolling.musicRowVmIds[1].value];vm.position=AnmFloat3View(7,116+deltas[n],9);vm.positionInterpolation.duration=77;
        float originalY=vm.position.y;FrontEndUpdateMusicRoom(&scrolling);
        if(n==0){assert(vm.positionInterpolation.duration==4&&vm.position.y==originalY);assert(vm.positionInterpolation.final.x==60&&vm.positionInterpolation.final.y==116);}
        else {assert(vm.positionInterpolation.duration==77&&vm.position.x==60&&vm.position.y==116&&vm.position.z==0);}
    }
    FrontEndControllerView failed;failed.frontAnm=&front;failed.stateTimer.current=1;fileFail=true;events.clear();
    FrontEndUpdateMusicRoom(&failed);assert(failed.screenState==3&&failed.musicText==NULL&&failed.cursor.pops==1);
    assert(events==std::vector<std::string>({"sound:11"}));
    failed.screenState=99;events.clear();assert(FrontEndUpdateMusicRoom(&failed)==0&&events.empty());
    return 0;
}
'''
        (ROOT / 'build').mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='music-room-', dir=ROOT / 'build') as directory:
            source=Path(directory)/'test.cpp';binary=Path(directory)/'test';source.write_text(program)
            subprocess.run([shutil.which('c++'),'-std=c++11','-O0',str(source),'-o',str(binary)],check=True)
            subprocess.run([str(binary)],check=True)

if __name__ == '__main__':
    unittest.main()
