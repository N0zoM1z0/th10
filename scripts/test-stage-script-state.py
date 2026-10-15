#!/usr/bin/env python3
"""Compile maintained stage dispatcher/callback against bounded synthetic sinks.

Host layout is not the 32-bit target ABI. Pinned source assertions establish
that layout separately. No file IO, D3D rendering or real ANM execution runs.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
ROOT = Path(__file__).resolve().parents[1]

class StageScriptState(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_actual_stage_source(self):
        header=(ROOT/'src/StageScript.hpp').read_text()
        header=header.replace('#pragma once','').replace('#include "AnmManager.hpp"','')
        header=header[:header.index('typedef char StageScriptLayoutChecks[')]
        source=(ROOT/'src/StageScript.cpp').read_text().replace('#include "StageScript.hpp"','')
        timer_source=(ROOT/'src/AnmManager.hpp').read_text()
        timer_start=timer_source.index('struct AnmVmTimerView\n{')
        timer_end=timer_source.index('\n};',timer_start)+3
        timer_source=timer_source[timer_start:timer_end]
        program=r'''
#include <cassert>
#include <cstring>
#include <vector>
#include <string>
#include <cmath>
#include <cstddef>
#define __stdcall
static float g_AnmGameSpeed=1.0f;
struct AnmFloat3View {float x,y,z;AnmFloat3View(){} AnmFloat3View(float a,float b,float c):x(a),y(b),z(c){} AnmFloat3View operator-(const AnmFloat3View &b)const{return AnmFloat3View(x-b.x,y-b.y,z-b.z);} };
'''+timer_source+r'''
static std::vector<const void *> evaluations;
struct AnmVmFloat3InterpolationView {AnmFloat3View initial,final,initialTangent,finalTangent;AnmVmTimerView timer;int duration=0,mode=0;AnmFloat3View *Evaluate(AnmFloat3View *out){evaluations.push_back(this);*out=final;return out;}};
struct AnmLoadedView {};
struct AnmVmView {unsigned flags35C=0;void *currentInstruction=NULL;int initializedIndex=-1;AnmLoadedView *initializedOwner=NULL;void InitializeForLoadedScript(AnmLoadedView *owner,int index){initializedOwner=owner;initializedIndex=index;}};
static std::vector<AnmVmView *> executions;static std::vector<float> speeds;
static void StageVmSink(AnmVmView *vm);
struct AnmRenderManagerView {static int ExecuteScript(AnmVmView *vm){executions.push_back(vm);speeds.push_back(g_AnmGameSpeed);StageVmSink(vm);return 0;}};
'''+header+r'''
StageCameraStateView g_StagePublishedCamera;
int g_StageScriptGlobal4923A8;
StageFogView *StageFogInterpolationView::Evaluate(StageFogView *out){evaluations.push_back(this);*out=final;return out;}
static int normalizeCount;
static StageScriptView *orderedStage=NULL;
static AnmVmView *orderedObject=NULL;
static AnmVmView *terminateVm=NULL;
static std::vector<std::string> orderEvents;
static void StageVmSink(AnmVmView *vm){
    if(vm==terminateVm)vm->currentInstruction=NULL;
    if(!orderedStage)return;
    StageScriptView &s=*orderedStage;
    assert(g_StagePublishedCamera.position.x==-999); // publication follows every VM
    assert(s.auxiliaryActive!=0&&s.frame==41); // clear/increment also follow every VM
    if(vm==orderedObject){
        assert(orderEvents==std::vector<std::string>({"normalize"}));
        assert(s.tint==0x808080&&s.camera.normalizedDirection.x==2&&s.camera.direction.x==1);
        assert(s.camera.position.x==10);s.camera.position.x=20;
        orderEvents.push_back("object");return;
    }
    for(int i=0;i<8;++i)if(vm==&s.scriptVms[i]){
        assert(orderEvents.size()==size_t(2+i));
        assert(s.camera.direction.x==9&&s.camera.normalizedDirection.x==2);
        assert(s.camera.positionDelta.x==20); // script position40 minus object-updated20
        assert(s.camera.position.x==(i==0?40:50+i-1));
        s.camera.position.x=float(50+i);orderEvents.push_back("script"+std::to_string(i));return;
    }
    for(int i=0;i<3;++i)if(vm==&s.auxiliaryVms[i]){
        assert(orderEvents.size()==size_t(10+i)&&g_AnmGameSpeed==1);
        assert(s.camera.position.x==(i==0?57:80+i-1));
        s.camera.position.x=float(80+i);s.auxiliaryActive=2;
        orderEvents.push_back("aux"+std::to_string(i));return;
    }
    assert(false);
}
extern "C" AnmFloat3View *D3DXVec3Normalize(AnmFloat3View *out,const AnmFloat3View *in){
    ++normalizeCount;*out=*in;
    if(orderedStage){
        assert(in==&orderedStage->camera.direction&&out==&orderedStage->camera.normalizedDirection);
        assert(orderEvents.empty()&&in->x==1&&orderedStage->camera.positionDelta.x==0&&orderedStage->camera.value0E8==0&&orderedStage->camera.value0EC==0);
        out->x=in->x*2;orderEvents.push_back("normalize");
    }
    return out;
}
'''+source+r'''
static void initialize(StageScriptView &s,StageInstructionView *r){
    s.scriptTimer.flags=0;s.rockingTimer.flags=0;s.directionInterpolation.timer.flags=0;s.positionInterpolation.timer.flags=0;s.fogInterpolation.timer.flags=0;
    s.scriptTimer.SetCurrent(0);s.rockingTimer.SetCurrent(0);s.rockingMode=0;
    s.directionInterpolation.duration=0;s.positionInterpolation.duration=0;s.fogInterpolation.duration=0;
    s.instruction=r;s.scriptBase=reinterpret_cast<unsigned char *>(r);
    s.camera.position=AnmFloat3View(10,20,30);s.camera.direction=AnmFloat3View(1,2,3);
    s.flags=0;s.transitionTimer.SetCurrent(0);s.auxiliaryActive=0;s.frame=0;
}
int main(){
    // Exercise the actual shared timer SetCurrent/Tick source, including lazy initialization.
    AnmVmTimerView lazy{};lazy.flags=0;lazy.SetCurrent(4);
    assert(lazy.current==4&&lazy.previous==3&&lazy.subframe==4&&lazy.scale==&g_AnmGameSpeed&&(lazy.flags&1));
    float slow=0.5f;lazy.scale=&slow;lazy.flags=0x81;lazy.SetCurrent(2);lazy.Tick();
    assert(lazy.scale==&slow&&lazy.flags==0x81&&lazy.current==2&&lazy.previous==2&&lazy.subframe==2.5f);
    StageInstructionView records[20]{};for(int i=0;i<20;++i)records[i].byteSize=sizeof(records[i]);
    StageScriptView s{};initialize(s,records);
    // Opcode0 yields without consuming/ticking but evaluates all active channels in order.
    s.directionInterpolation.duration=1;s.positionInterpolation.duration=1;s.fogInterpolation.duration=1;
    s.directionInterpolation.final=AnmFloat3View(4,5,6);s.positionInterpolation.final=AnmFloat3View(7,8,9);
    s.fogInterpolation.final=StageFogView(1,2,3,4,5,6);
    StageExecuteScript(&s);assert(s.instruction==records&&s.scriptTimer.current==0);
    assert(evaluations==std::vector<const void *>({&s.directionInterpolation,&s.positionInterpolation,&s.fogInterpolation}));
    assert(s.camera.position.x==7&&s.camera.direction.x==4&&s.camera.fog.color==0x06050403);
    // Signed negative record size is followed; a timed future record then ticks.
    initialize(s,records+1);records[1].opcode=-1;records[1].byteSize=-int(sizeof(records[0]));records[0].time=5;
    StageExecuteScript(&s);assert(s.instruction==records&&s.scriptTimer.current==1);
    // Jump changes time and script-relative pointer, without ordinary size advance.
    initialize(s,records);records[0].time=0;records[0].opcode=1;records[0].args.integers[0]=2*sizeof(records[0]);records[0].args.integers[1]=7;records[2].time=8;
    StageExecuteScript(&s);assert(s.instruction==records+2&&s.scriptTimer.current==8);
    // Direct camera, up, scalar, fog, global and VM operations.
    initialize(s,records);for(int i=0;i<20;++i){records[i]=StageInstructionView();records[i].byteSize=sizeof(records[i]);}
    int ops[]={2,4,6,7,8,13,14,14};
    for(int i=0;i<8;++i){records[i].opcode=ops[i];for(int j=0;j<3;++j)records[i].args.reals[j]=float(i*10+j);}
    records[4].args.integers[0]=0xFEDCBA98;records[5].args.integers[0]=123;
    records[6].args.integers[0]=2;records[6].args.integers[1]=-1;s.scriptVms[2].flags35C=7;
    AnmLoadedView loaded;s.anm=&loaded;records[7].args.integers[0]=3;records[7].args.integers[1]=17;
    records[8].time=1;StageExecuteScript(&s);
    assert(s.camera.position.x==0&&s.camera.positionDelta.x==-10&&s.camera.positionDelta.z==-28);
    assert(s.camera.direction.x==10&&s.camera.up.z==22&&s.camera.value048==30);
    assert(s.camera.fog.blue==152&&s.camera.fog.green==186&&s.camera.fog.red==220&&s.camera.fog.alpha==254&&s.camera.fog.start==41);
    assert(g_StageScriptGlobal4923A8==123&&s.scriptVms[2].flags35C==6&&s.scriptVms[3].initializedIndex==17&&s.scriptVms[3].initializedOwner==&loaded);
    // Linear and Hermite setup captures aggregate values before timer/reset and evaluates direction first.
    int setups[]={3,5,9,10,11};
    for(int n=0;n<5;++n){initialize(s,records);records[0].opcode=setups[n];records[0].time=0;records[0].byteSize=sizeof(records[0]);records[1].time=1;records[0].args.integers[0]=9;records[0].args.integers[1]=4;
        for(int k=2;k<11;++k)records[0].args.reals[k]=float(k+100);
        if(n==2){records[0].args.integers[2]=0xA1B2C3D4;records[0].args.reals[3]=55;records[0].args.reals[4]=99;}
        evaluations.clear();StageExecuteScript(&s);
        if(n==2){assert(s.fogInterpolation.mode==4&&s.fogInterpolation.duration==9&&s.fogInterpolation.final.color==0xA1B2C3D4&&s.camera.fog.start==55);}
        else {AnmVmFloat3InterpolationView &v=(n==1||n==4)?s.directionInterpolation:s.positionInterpolation;assert(v.duration==9&&v.timer.current==0&&v.mode==(n>=3?8:4));assert(v.final.x==(n>=3?105:102));assert(v.final.y==(n>=3?106:103)&&v.final.z==(n>=3?107:104));
            assert(v.initial.x==((n==1||n==4)?1:10)&&v.initial.y==((n==1||n==4)?2:20)&&v.initial.z==((n==1||n==4)?3:30));
            if(n>=3)assert(v.initialTangent.x==102&&v.initialTangent.y==103&&v.initialTangent.z==104&&v.finalTangent.x==108&&v.finalTangent.y==109&&v.finalTangent.z==110);}
        assert(evaluations.size()==1);
    }
    initialize(s,records);records[0].opcode=12;records[0].args.integers[0]=1;records[0].time=0;records[1].time=1;
    StageExecuteScript(&s);assert(s.rockingMode==1&&s.rockingTimer.current==1);
    records[0].opcode=0;s.instruction=records;s.rockingTimer.SetCurrent(511);StageExecuteScript(&s);assert(s.rockingTimer.current==0);
    s.rockingMode=2;s.rockingTimer.SetCurrent(5);StageExecuteScript(&s);assert(s.rockingTimer.current==5);
    // Primitive updater preserves active flags while any executed VM has a current instruction.
    struct ObjectFixture {StageObjectView object;StageObjectPrimitiveView end;};
    ObjectFixture first{},second{};first.object.flags=3;second.object.flags=1;
    first.object.firstPrimitive.type=0;first.object.firstPrimitive.byteSize=sizeof(StageObjectPrimitiveView);first.object.firstPrimitive.vmIndex=0;first.end.type=-1;
    second.object.firstPrimitive=first.object.firstPrimitive;second.object.firstPrimitive.vmIndex=1;second.end.type=-1;
    StageDataHeaderView data{2};StageObjectView *objects[]={&first.object,&second.object};AnmVmView objectVms[2];objectVms[0].currentInstruction=&records[0];
    s.data=&data;s.objects=objects;s.objectVms=objectVms;executions.clear();s.UpdateObjectVms();
    assert(executions==std::vector<AnmVmView *>({&objectVms[0],&objectVms[1]})&&first.object.flags==3&&second.object.flags==0);
    executions.clear();first.object.flags=0;second.object.flags=1;second.object.firstPrimitive.type=-1;
    s.UpdateObjectVms();assert(executions.empty()&&first.object.flags==0&&second.object.flags==0);
    data.objectCount=0;first.object.flags=1;s.UpdateObjectVms();assert(executions.empty()&&first.object.flags==1);
    // The executed VM clears its instruction stream; checking before execution would fail.
    data.objectCount=1;first.object.firstPrimitive.type=0;objectVms[0].currentInstruction=&records[0];terminateVm=&objectVms[0];
    s.UpdateObjectVms();assert(executions.size()==1&&first.object.flags==0);terminateVm=NULL;
    objectVms[0].currentInstruction=&records[0];first.object.flags=1;
    // Callback gate, order, all eight VMs, optional three speed-overridden VMs and whole camera copy.
    initialize(s,records);records[0].time=0;records[0].opcode=0;data.objectCount=0;s.data=&data;
    memset(&s.camera,0x5a,sizeof(s.camera));s.camera.direction=AnmFloat3View(1,2,3);
    s.auxiliaryActive=1;g_AnmGameSpeed=0.25f;executions.clear();speeds.clear();normalizeCount=0;
    assert(s.Update()==1&&s.frame==1&&normalizeCount==1&&s.auxiliaryActive==0&&g_AnmGameSpeed==0.25f);
    assert(executions.size()==11&&sizeof(s.camera)==280&&memcmp(&s.camera,&g_StagePublishedCamera,280)==0);
    for(int i=0;i<8;++i)assert(executions[i]==&s.scriptVms[i]&&speeds[i]==0.25f);
    for(int i=0;i<3;++i)assert(executions[8+i]==&s.auxiliaryVms[i]&&speeds[8+i]==1.0f);
    assert(s.camera.positionDelta.x==0&&s.camera.positionDelta.y==0&&s.camera.positionDelta.z==0&&s.camera.value0E8==0&&s.camera.value0EC==0&&s.tint==0x808080);
    executions.clear();s.Update();assert(executions.size()==8&&s.frame==2);
    s.flags=8;executions.clear();s.Update();assert(executions.empty()&&s.frame==2);
    s.flags=4;s.transitionTimer.SetCurrent(60);s.Update();assert(executions.empty());
    s.transitionTimer.SetCurrent(59);s.Update();assert(executions.size()==8&&s.frame==3);
    // Dependent callback sinks distinguish normalize/object/dispatch/all VM/publication order.
    initialize(s,records);records[0].time=0;records[0].opcode=2;records[0].byteSize=sizeof(records[0]);
    records[0].args.reals[0]=40;records[0].args.reals[1]=41;records[0].args.reals[2]=42;
    records[1].time=0;records[1].opcode=4;records[1].byteSize=sizeof(records[1]);
    records[1].args.reals[0]=9;records[1].args.reals[1]=8;records[1].args.reals[2]=7;
    records[2].time=0;records[2].opcode=0;
    data.objectCount=1;first.object.flags=1;s.data=&data;s.objects=objects;s.objectVms=objectVms;
    s.auxiliaryActive=1;s.frame=41;s.camera.value0E8=8;s.camera.value0EC=9;s.camera.positionDelta.x=10;
    g_StagePublishedCamera.position.x=-999;orderedStage=&s;orderedObject=&objectVms[0];
    executions.clear();speeds.clear();orderEvents.clear();g_AnmGameSpeed=0.375f;
    s.Update();orderedStage=NULL;
    assert(orderEvents==std::vector<std::string>({"normalize","object","script0","script1","script2","script3","script4","script5","script6","script7","aux0","aux1","aux2"}));
    assert(executions.size()==12&&s.frame==42&&s.auxiliaryActive==0&&g_AnmGameSpeed==0.375f);
    assert(g_StagePublishedCamera.position.x==82&&memcmp(&s.camera,&g_StagePublishedCamera,280)==0);
    for(int i=0;i<9;++i)assert(speeds[i]==0.375f);
    return 0;
}
'''
        (ROOT/'build').mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='stage-script-',dir=ROOT/'build') as directory:
            src=Path(directory)/'test.cpp';binary=Path(directory)/'test';src.write_text(program)
            subprocess.run([shutil.which('c++'),'-std=c++11','-O0',str(src),'-o',str(binary)],check=True)
            subprocess.run([str(binary)],check=True)

if __name__=='__main__':
    unittest.main()
