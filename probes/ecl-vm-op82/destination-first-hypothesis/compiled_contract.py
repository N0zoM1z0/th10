#!/usr/bin/env python3
"""Compile extracted authored case against host seams, never game/target code."""
import hashlib
import json
from pathlib import Path
import resource
import subprocess
import sys
import tempfile

ROOT=Path(__file__).parent
BASE=Path('/workspace/shared/gui-opcode20-isolated/candidate/src/EclVm.cpp')
OLD='''            case ECL_VM_NORMALIZE_ANGLE:
                *ResolveFloat(0) = EnemyWrapAngle(ReadFloat(0));
                break;'''
NEW='''            case ECL_VM_NORMALIZE_ANGLE:
            {
                float *destination = ResolveFloat(0);
                *destination = EnemyWrapAngle(ReadFloat(0));
                break;
            }'''
PREFIX=r'''
#include <cstdio>
#include <cstring>
static char events[32];
static int event_count, resolve_count, read_count, wrap_count;
static void event(char ch) { if (event_count < 32) events[event_count] = ch; ++event_count; }
// Pure arithmetic provider seam; not an implementation of the game normalizer.
static float EnemyWrapAngle(float value) { ++wrap_count; event('W'); return value * 0.5f; }
enum { ECL_VM_NORMALIZE_ANGLE = 82 };
struct Host {
    float values[4];
    int selected, frame_base, stack_top, flags, operand;
    bool frame;
    Host(int slot, float value, bool is_frame) : selected(slot), frame_base(12), stack_top(64), flags(7), frame(is_frame) {
        values[0]=9.0f; values[1]=10.0f; values[2]=11.0f; values[3]=12.0f;
        values[selected]=value;
        operand=frame ? selected*4 : -9981+selected;
    }
    int cell(int index) const {
        const int encoded = index == 0 ? operand : (frame ? ((selected+1)%4)*4 : -9981+(selected+1)%4);
        return frame ? encoded/4 : encoded+9981;
    }
    float *ResolveFloat(unsigned int index) { ++resolve_count; event('D'); return &values[cell(index)]; }
    float ReadFloat(unsigned int index) { ++read_count; event('R'); return values[cell(index)]; }
    void Run() {
        switch (ECL_VM_NORMALIZE_ANGLE) {
'''
SUFFIX=r'''
        }
    }
};
static int fail(const char *message) { std::puts(message); return 17; }
int main() {
    const float input[] = {0.0f, -0.0f, 1.25f, -2.5f, 3.0f, -3.0f};
    int checked=0;
    for (int frame=0; frame<2; ++frame) for (int slot=0; slot<4; ++slot) for (unsigned int v=0; v<sizeof(input)/sizeof(input[0]); ++v) {
        Host host(slot,input[v],frame!=0);
        float before[4]; std::memcpy(before,host.values,sizeof(before));
        event_count=resolve_count=read_count=wrap_count=0;
        host.Run();
        if (resolve_count!=1 || read_count!=1 || wrap_count!=1 || event_count!=3) return fail("CALL_COUNT");
        const float expected=input[v]*0.5f;
        if (std::memcmp(&host.values[slot],&expected,sizeof(float))!=0) return fail("DESTINATION_VALUE");
        for (int i=0; i<4; ++i) if (i!=slot && std::memcmp(&host.values[i],&before[i],sizeof(float))!=0) return fail("OTHER_DESTINATION");
        if (host.frame_base!=12 || host.stack_top!=64 || host.flags!=7) return fail("STATE_CHANGED");
        if (CHECK_ORDER && (events[0]!='D' || events[1]!='R' || events[2]!='W')) return fail("EVENT_ORDER");
        ++checked;
    }
    if (checked!=48) return fail("COVERAGE");
    std::puts("PASS_48"); return 0;
}
'''

def bounded_child():
    resource.setrlimit(resource.RLIMIT_CPU,(10,12))
    resource.setrlimit(resource.RLIMIT_AS,(512*1024*1024,512*1024*1024))

def execute():
    if not __debug__:raise RuntimeError('OPTIMIZED_PYTHON_REJECTED')
    source=BASE.read_bytes();manifest=json.loads((ROOT/'manifest.json').read_text())
    assert hashlib.sha256(source).hexdigest()==manifest['base_source_sha256']
    assert source.decode().count(OLD)==1
    candidate=source.decode().replace(OLD,NEW)
    assert hashlib.sha256(candidate.encode()).hexdigest()==manifest['hypothetical_source_sha256']
    # Extract the exact proposed case, not a retyped substitute for compilation.
    first=candidate.index('            case ECL_VM_NORMALIZE_ANGLE:')
    following=candidate.index('            case ECL_VM_POINT_ANGLE:',first)
    extracted=candidate[first:following].rstrip()
    assert extracted==NEW
    variants=[('proposed',extracted,True,'PASS_48'),('baseline_pure_only',OLD,False,'PASS_48'),
      ('read_first',extracted.replace('float *destination = ResolveFloat(0);\n                *destination = EnemyWrapAngle(ReadFloat(0));','float value = EnemyWrapAngle(ReadFloat(0));\n                float *destination = ResolveFloat(0);\n                *destination = value;'),True,'EVENT_ORDER'),
      ('wrong_destination',extracted.replace('ResolveFloat(0)','ResolveFloat(1)'),True,'OTHER_DESTINATION'),
      ('duplicate_read',extracted.replace('float *destination =','(void)ReadFloat(0);\n                float *destination ='),True,'CALL_COUNT'),
      ('missing_wrap',extracted.replace('EnemyWrapAngle(ReadFloat(0))','ReadFloat(0)'),True,'CALL_COUNT'),
      ('missing_read',extracted.replace('ReadFloat(0)','1.25f'),True,'CALL_COUNT')]
    mutant_cases=[case for name,case,_,_ in variants if name not in ('proposed','baseline_pure_only')]
    assert all(case!=extracted for case in mutant_cases)
    assert len(set(mutant_cases))==len(mutant_cases)
    report={'host_only':True,'target_accessed':False,'production_edited':False,'native_vc71_claim':False,
            'candidate_sha256':manifest['hypothetical_source_sha256'],'tests':[]}
    report['compiler']=subprocess.run(['/usr/bin/g++','--version'],capture_output=True,text=True,timeout=2,check=True).stdout.splitlines()[0]
    with tempfile.TemporaryDirectory(prefix='op82-host-contract-') as temporary:
      folder=Path(temporary)
      for name,case,order,expected in variants:
        harness='#define CHECK_ORDER '+('1' if order else '0')+'\n'+PREFIX+case+SUFFIX
        src=folder/(name+'.cpp');binary=folder/name;src.write_text(harness)
        compilation=subprocess.run(['/usr/bin/g++','-std=c++98','-O0','-Wall','-Wextra','-pedantic-errors',str(src),'-o',str(binary)],capture_output=True,text=True,timeout=15,preexec_fn=bounded_child)
        if compilation.returncode:raise RuntimeError(name+' did not compile: '+compilation.stderr[:2000])
        result=subprocess.run([str(binary)],capture_output=True,text=True,timeout=2,preexec_fn=bounded_child)
        expected_code=0 if name in ('proposed','baseline_pure_only') else 17
        assert result.returncode==expected_code,(name,result.returncode,result.stdout)
        assert result.stdout.strip()==expected,(name,result.stdout)
        assert not result.stderr
        report['tests'].append({'name':name,'compiled':True,'expected_exit':expected_code,'actual_exit':result.returncode,
            'expected_diagnostic':expected,'actual_diagnostic':result.stdout.strip(),'harness_sha256':hashlib.sha256(harness.encode()).hexdigest()})
    assert hashlib.sha256(BASE.read_bytes()).hexdigest()==manifest['base_source_sha256']
    report['base_unchanged']=True;report['all_passed']=True
    return report

if __name__=='__main__':
    if not __debug__:raise SystemExit('OPTIMIZED_PYTHON_REJECTED')
    result=execute();(ROOT/'compiled-contract-result.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'all_passed':result['all_passed'],'compiled_variants':len(result['tests']),'compiler':result['compiler']}))
