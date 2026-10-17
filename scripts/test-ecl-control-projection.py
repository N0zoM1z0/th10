#!/usr/bin/env python3
"""Local source-slice projection. Never compiles or edits the production project."""
from pathlib import Path
import argparse, hashlib, subprocess, json, sys, tempfile
if not __debug__:
    raise SystemExit("Refusing optimized Python (-O/-OO): assertion gates are required")
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--source-root',type=Path,required=True,help='Repository root containing src/EclVm.cpp and src/EclVm.hpp')
parser.add_argument('--output-dir',type=Path,help='New or empty output directory (default: unique system temporary directory)')
args=parser.parse_args()
SRC=args.source_root.resolve()/'src'/'EclVm.cpp'
ROOT=args.output_dir.resolve() if args.output_dir else Path(tempfile.mkdtemp(prefix='ecl-control-projection-'))
if ROOT.exists() and any(ROOT.iterdir()):
    raise SystemExit('Output directory must be empty; refusing stale or overlapping artifacts')
ROOT.mkdir(parents=True,exist_ok=True)
HDR=SRC.with_suffix('.hpp')
s=SRC.read_text(); h=HDR.read_text()
EXPECTED='8f236b6d2bdfa73bbf8747b16e934eb62264d2d54a811ac5f5cec789a1457efb'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(SRC)==EXPECTED, 'Source changed: review extraction before running'
assert sha(HDR)=='a01e19c6bb2659e258444cd57018947fe6f5444522b75d2b5777e13f55ffe551'
def part(a,b):
    assert s.count(a)==1 and s.count(b)==1
    return s[s.index(a):s.index(b)]
# Exact contiguous source slices, in original order. Unsupported switch arms omitted.
run=(part('int EclVmContext::Run(float timeDelta)','            case ECL_VM_SPAWN_THREAD:')+
     part('            case ECL_VM_CALL:','            case ECL_VM_JUMP_IF_TRUE:')+
     s[s.index('            default:\n                if (host->DispatchEclInstruction() == -1)'):])
helpers=(part('int EclVmStackView::Pop(', 'int EclVmStackView::EnterFrame(')+
         part('int EclVmStackView::LeaveFrame()', '// The target selector table'))
instr=h[h.index('struct EclVmInstruction\n'):h.index('\nclass EclVmHost;')]
prefix='''#include <cstring>
#include <iostream>
#include <functional>
#include <stdexcept>
using std::memcpy;
'''+instr+'''
struct EclVmStackView {
 alignas(16) unsigned char data[0x1000]{}; int stackTop=0, frameBase=0;
 int Pop(unsigned char,int,void*); int LeaveFrame();
};
struct EclVmContext;
struct EclVmHost {
 EclVmContext *activeContext=nullptr;
 std::function<int()> dispatch; int dispatches=0;
 virtual int DispatchEclInstruction() { ++dispatches; return dispatch(); }
};
struct EclVmContext {
 float currentTime=0; EclVmInstruction *instruction=nullptr;
 EclVmStackView stack; EclVmHost *host=nullptr; unsigned char difficultyMask=1;
 EclVmInstruction *callTarget=nullptr; int callStatus=0, calls=0;
 // Concrete helper outcome stub: deterministic, no callback is exposed here.
 // Does NOT validate StartSubroutine frame, lookup or argument semantics.
 int StartSubroutine(EclVmContext *caller,unsigned first) {
  if(caller!=this || first!=0) throw std::runtime_error("CALL arguments");
  ++calls; instruction=callTarget; currentTime=0; return callStatus;
 }
 int Run(float);
};
enum { ECL_VM_NOP=0, ECL_VM_TERMINATE=1, ECL_VM_RETURN=10, ECL_VM_CALL=11 };
'''
fixtures=r'''
#define CHECK(x) do { if(!(x)) throw std::runtime_error(#x); } while(0)
struct Script {
 alignas(16) unsigned char bytes[512]{};
 EclVmInstruction *at(int offset,int opcode=0,int time=100,int size=32,int mask=1) {
  auto *p=reinterpret_cast<EclVmInstruction*>(bytes+offset);
  p->opcode=opcode; p->time=time; p->size=size; p->difficultyMask=mask; return p;
 }
};
struct Fixture {
 Script s; EclVmHost host; EclVmContext c, other;
 Fixture() { c.host=&host; other.host=&host; host.activeContext=&c; host.dispatch=[] {return 0;}; }
};
int count=0;
void test(const char *name,std::function<void()> f) { f(); ++count; std::cout<<"PASS "<<name<<"\n"; }
void frame(EclVmContext &c,EclVmInstruction *saved,float time) {
 // Host-width projection: saved pointer takes sizeof(pointer), not target's four bytes.
 int n=sizeof(saved); memcpy(c.stack.data,&time,4); memcpy(c.stack.data+4,&saved,n);
 c.stack.frameBase=4+n; c.stack.stackTop=4+n+4;
 int previous=0; memcpy(c.stack.data+4+n,&previous,4);
}
int main() { try {
 test("entry-null-no-tick",[]{Fixture f; f.c.currentTime=7; CHECK(f.c.Run(2)==-1); CHECK(f.c.currentTime==7);});
 test("future-masked-not-skipped",[]{Fixture f; auto *a=f.s.at(0,11,20,32,0); f.c.instruction=a; CHECK(f.c.Run(2)==0); CHECK(f.c.instruction==a); CHECK(f.c.currentTime==2); CHECK(f.c.calls==0);});
 for(int op: {0,1,10,11,2,88,-1}) test("due-difficulty-skip",[op]{Fixture f; f.c.instruction=f.s.at(0,op,0,48,0); auto *b=f.s.at(48); CHECK(f.c.Run(2)==0); CHECK(f.c.instruction==b); CHECK(f.c.currentTime==2); CHECK(f.c.calls==0 && f.host.dispatches==0);});
 for(int op: {2,88,-1}) for(int result: {-1,0,1,-2}) test("default-result-and-opcode",[op,result]{Fixture f; auto *a=f.s.at(0,op,0); auto *b=f.s.at(32); f.c.instruction=a; f.host.dispatch=[result]{return result;}; CHECK(f.c.Run(2)==0); CHECK(f.c.instruction==(result==-1?a:b)); CHECK(f.c.currentTime==(result==-1?0:2)); CHECK(f.host.dispatches==1);});
 test("default-reloads-live-cursor-size-time",[]{Fixture f; f.c.instruction=f.s.at(0,2,0,32); f.s.at(32); auto *b=f.s.at(128,0,0,64); auto *end=f.s.at(192); f.host.dispatch=[&]{f.c.instruction=b; b->size=48; end=f.s.at(176); f.c.currentTime=7; return 0;}; CHECK(f.c.Run(2)==0); CHECK(f.c.instruction==end); CHECK(f.c.currentTime==9); CHECK(f.host.dispatches==1);});
 test("default-minus-one-preserves-null-and-time",[]{Fixture f; f.c.instruction=f.s.at(0,2,0); f.host.dispatch=[&]{f.c.instruction=nullptr; f.c.currentTime=7; return -1;}; CHECK(f.c.Run(2)==0); CHECK(f.c.instruction==nullptr && f.c.currentTime==7);});
 test("active-context-does-not-rebind-run",[]{Fixture f; f.c.instruction=f.s.at(0,2,0); auto *end=f.s.at(32); auto *other=f.s.at(128); f.other.instruction=other; f.host.dispatch=[&]{f.host.activeContext=&f.other; return 0;}; CHECK(f.c.Run(2)==0); CHECK(f.c.instruction==end); CHECK(f.other.instruction==other); CHECK(f.host.activeContext==&f.other);});
 test("callback-difficulty-observed-next-iteration",[]{Fixture f; f.c.instruction=f.s.at(0,2,0); f.s.at(32,1,0); auto *end=f.s.at(64); f.host.dispatch=[&]{f.c.difficultyMask=0; return 0;}; CHECK(f.c.Run(2)==0); CHECK(f.c.instruction==end); CHECK(f.host.dispatches==1);});
 test("call-success-bypasses-common-advance",[]{Fixture f; f.c.instruction=f.s.at(0,11,0); auto *callee=f.s.at(128); f.c.callTarget=callee; f.s.at(160); CHECK(f.c.Run(2)==0); CHECK(f.c.instruction==callee); CHECK(f.c.currentTime==2 && f.c.calls==1);});
 for(int status: {-1,1}) test("call-failure-no-tick",[status]{Fixture f; f.c.instruction=f.s.at(0,11,0); f.c.currentTime=7; f.c.callStatus=status; CHECK(f.c.Run(2)==-1); CHECK(f.c.instruction==nullptr); CHECK(f.c.currentTime==0 && f.c.calls==1);});
 test("return-restores-cursor-live-size-and-time",[]{Fixture f; f.c.instruction=f.s.at(0,10,0,32); auto *saved=f.s.at(128,0,0,48); auto *end=f.s.at(176,0,20); frame(f.c,saved,7); CHECK(f.c.Run(2)==0); CHECK(f.c.instruction==end); CHECK(f.c.currentTime==9); CHECK(f.c.stack.stackTop==0 && f.c.stack.frameBase==0);});
 test("return-empty-frame-no-pops-no-tick",[]{Fixture f; f.c.instruction=f.s.at(0,10,0); f.c.currentTime=7; f.c.stack.stackTop=16; f.c.stack.frameBase=0; int old=12; memcpy(f.c.stack.data+12,&old,4); CHECK(f.c.Run(2)==-1); CHECK(f.c.instruction==nullptr && f.c.currentTime==7); CHECK(f.c.stack.stackTop==0 && f.c.stack.frameBase==12);});
 test("return-null-still-pops-time-no-tick",[]{Fixture f; f.c.instruction=f.s.at(0,10,0); frame(f.c,nullptr,7); CHECK(f.c.Run(2)==-1); CHECK(f.c.instruction==nullptr && f.c.currentTime==7); CHECK(f.c.stack.stackTop==0);});
 test("malformed-return-pop-underflow-ignored",[]{Fixture f; f.c.instruction=f.s.at(0,10,0); auto *end=f.s.at(32); f.c.stack.frameBase=1; f.c.stack.stackTop=0; CHECK(f.c.Run(2)==0); CHECK(f.c.instruction==end && f.c.currentTime==2); CHECK(f.c.stack.stackTop==1);});
 test("helper-pop-underflow-preserves-output-top",[]{EclVmStackView s; int out=99; s.stackTop=2; CHECK(s.Pop(0,4,&out)==-1); CHECK(out==99 && s.stackTop==2);});
 test("helper-leaveframe-low-top-restores-old-frame",[]{EclVmStackView s; s.frameBase=12; s.stackTop=2; CHECK(s.LeaveFrame()==0); CHECK(s.stackTop==12 && s.frameBase==12);});
 test("terminate-no-tick",[]{Fixture f; f.c.instruction=f.s.at(0,1,0); CHECK(f.c.Run(2)==-1); CHECK(f.c.instruction==nullptr && f.c.currentTime==0);});
 std::cout<<"TOTAL "<<count<<" passed; pointer_bytes="<<sizeof(void*)<<"\n"; return 0;
 } catch(const std::exception &e) { std::cerr<<"FAIL "<<e.what()<<"\n"; return 1; }
}
'''
mutations={
 'due-boundary-exclusive':('->time) <= currentTime', '->time) < currentTime'),
 'stale-common-cursor':('        current = *instructionCursor;\n        instructionOffset', '        /* mutated: stale current */\n        instructionOffset'),
 'call-common-advance':('                continue;','                break;'),
 'default-negative-stops':('host->DispatchEclInstruction() == -1','host->DispatchEclInstruction() < 0'),
 'default-stop-ticks':('                    return 0;','                    { currentTime += timeDelta; return 0; }'),
 'difficulty-inverted':('(difficultyMask & current->difficultyMask) != 0','(difficultyMask & current->difficultyMask) == 0'),
 'drop-normal-tick':('    currentTime += timeDelta;','    /* mutated: tick omitted */'),
 'return-omits-time-pop':('                stack.Pop(0, sizeof(currentTime), &currentTime);','                /* mutated: time pop omitted */'),
 'call-failure-ticks':('if (StartSubroutine(this, 0) != 0)\n                    return -1;', 'if (StartSubroutine(this, 0) != 0)\n                    { currentTime += timeDelta; return -1; }'),
 'null-return-check-before-time-pop':('                stack.Pop(0, sizeof(currentTime), &currentTime);\n                if (*instructionCursor == NULL)\n                    return -1;', '                if (*instructionCursor == NULL)\n                    return -1;\n                stack.Pop(0, sizeof(currentTime), &currentTime);'),
 'empty-return-keeps-cursor':('                    instruction = NULL;','                    /* mutated: retains cursor */'),
}
log=[]
def execute(name,code,baseline):
 p=ROOT/(name+'.cpp'); p.write_text(prefix+helpers+code+fixtures)
 exe=ROOT/name
 build=subprocess.run(['g++','-std=c++17','-O0','-g','-fsanitize=undefined','-fno-sanitize-recover=all',str(p),'-o',str(exe)],capture_output=True,text=True,timeout=60)
 assert build.returncode==0, build.stderr
 result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=5)
 log.append(f'=== {name}: exit {result.returncode} ===\n'+result.stdout+result.stderr)
 if baseline: assert result.returncode==0,log[-1]
 else: assert result.returncode==1 and 'FAIL ' in result.stderr, 'Mutation must fail an assertion, not crash: '+log[-1]
execute('baseline',run,True)
for name,(old,new) in mutations.items():
 assert run.count(old)==1,(name,run.count(old))
 execute('mutant-'+name,run.replace(old,new),False)
assert sha(SRC)==EXPECTED
assert sha(HDR)=='a01e19c6bb2659e258444cd57018947fe6f5444522b75d2b5777e13f55ffe551'
(ROOT/'test.log').write_text('\n'.join(log))
manifest={'runner_sha256':sha(Path(__file__)), 'source':{'path':str(SRC),'sha256':sha(SRC)},'header':{'path':str(HDR),'sha256':sha(HDR)},'extracted_run_sha256':hashlib.sha256(run.encode()).hexdigest(),'mutation_count':len(mutations),'files':{p.name:sha(p) for p in sorted(ROOT.iterdir()) if p.is_file() and p.name!='manifest.json'}}
(ROOT/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
print(log[0]); print(f'{len(mutations)} strict mutants killed by assertion; source/header unchanged. Artifacts: {ROOT}')
