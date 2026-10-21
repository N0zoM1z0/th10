#!/usr/bin/env python3
"""Local host semantic checks only. Never builds or executes the target.

Proposed callback bodies come directly from --source. Dependencies are extracted
from pinned canonical files. See docs/TRANSITION_EFFECT_HOST_TESTS.md for every host adaptation.
Recovered complete Chain dispatchers run through explicit host lock/allocation roles.
"""
from pathlib import Path
import argparse, hashlib, json, os, re, subprocess, sys

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_CANON = ROOT / 'src'

def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def normalize_crlf(data):
    # Only this byte sequence is normalized. Bare CR, whitespace, BOM, and all
    # other bytes remain unchanged and cannot match a different pinned input.
    return data.replace(b'\r\n',b'\n')
def read(p): return normalize_crlf(Path(p).read_bytes()).decode('utf-8')
def fingerprint(p):
    raw=Path(p).read_bytes(); normalized=normalize_crlf(raw)
    return {'sha256_raw':hashlib.sha256(raw).hexdigest(),
        'sha256_lf':hashlib.sha256(normalized).hexdigest(),
        'bytes_raw':len(raw),'bytes_lf':len(normalized),
        'crlf_count':raw.count(b'\r\n'),'bare_cr_count':normalized.count(b'\r')}
DEPENDENCIES=['Chain.hpp','Chain.cpp','Rng.hpp','RandomMath.cpp','AnmManager.hpp','AnmManager.cpp','AnmRectangle.hpp']
def validate_canonical(canonical,pinfile):
    config=json.loads(read(pinfile))
    if config.get('schema_version')!=2 or config.get('normalization')!='crlf-to-lf-only':
        raise RuntimeError('HOST_PIN_SCHEMA_MISMATCH: expected version2 crlf-to-lf-only')
    pins=config.get('sha256_lf')
    if not isinstance(pins,dict):raise RuntimeError('HOST_PIN_SCHEMA_MISMATCH: sha256_lf map required')
    observations={}
    for name in DEPENDENCIES:
        path=Path(canonical)/name; expected=pins.get('src/'+name)
        if not path.is_file() or not isinstance(expected,str) or not re.fullmatch(r'[0-9a-f]{64}',expected):
            raise RuntimeError(f'CANONICAL_PIN_MISMATCH: {path}; missing input or strict LF-normalized pin')
        observed=fingerprint(path)
        if observed['bare_cr_count']!=0 or observed['sha256_lf']!=expected:
            raise RuntimeError(f'CANONICAL_PIN_MISMATCH: {path}; only CRLF-to-LF differences are permitted')
        observations[str(path)]=observed
    return observations
def once(s, old, new):
    n = s.count(old)
    if n != 1: raise RuntimeError(f'exact-one replacement expected, got {n}: {old!r}')
    return s.replace(old, new, 1)
def block(text, marker):
    if text.count(marker) != 1: raise RuntimeError(f'nonunique extraction: {marker}')
    start=text.index(marker); left=text.index('{',start); depth=0
    for i in range(left,len(text)):
        if text[i]=='{': depth+=1
        elif text[i]=='}':
            depth-=1
            if depth==0: return text[start:i+1]
    raise RuntimeError(f'unbalanced extraction: {marker}')
def no_includes(s): return re.sub(r'^#include[^\n]*\n', '', s, flags=re.M)

def build(source, out, canonical=DEFAULT_CANON, sanitize=False):
    source=Path(source); out=Path(out); canonical=Path(canonical); out.mkdir(parents=True,exist_ok=True)
    pinfile=ROOT/'config/transition-effect-host-inputs.json'
    canonical_observations=validate_canonical(canonical,pinfile)
    inputs=[pinfile,source,source.with_suffix('.hpp'),canonical/'Chain.hpp',canonical/'Chain.cpp',canonical/'Rng.hpp',canonical/'RandomMath.cpp',canonical/'AnmManager.hpp',canonical/'AnmManager.cpp',canonical/'AnmRectangle.hpp',ROOT/'scripts/fixtures/transition-effect.cpp',Path(__file__)]
    h=read(source.with_suffix('.hpp'))
    h=re.sub(r'typedef char TransitionEffectPhysicalLayout\[.*?\];', '', h, count=1, flags=re.S)
    if 'TransitionEffectPhysicalLayout' in h: raise RuntimeError('layout assertion extraction failed')
    chain=read(canonical/'Chain.cpp')
    # AllocElem alone encodes 32-bit byte offsets. Adapt the same lvalues to
    # named fields so the unchanged dependency algorithm can run on 64-bit.
    substitutions={
        'new ChainElem':'HostNewNode()',
        '*(ChainCallback *)((u8 *)elem + 0x0c)':'elem->addedCallback',
        '*(ChainCallback *)((u8 *)elem + 0x10)':'elem->deletedCallback',
        '*(ChainCallback *)((u8 *)elem + 0x8)':'elem->callback',
        'free(elem);':'HostFree(elem);',
    }
    adaptation_counts={}
    for old,new in substitutions.items():
        n=chain.count(old)
        expected_count=1
        if n!=expected_count: raise RuntimeError(f'canonical adaptation count {n}, expected {expected_count}: {old}')
        adaptation_counts[old]=n;chain=chain.replace(old,new)
    # The dispatchers use the reviewed MainSupervisor members. Project these
    # two storage views onto the existing host lock/counter roles only.
    global_view_adaptations={}
    for old,new in {
        '&g_MainSupervisorView.criticalSections[0]':'&th10::g_CriticalSections[0]',
        'g_MainSupervisorView.criticalSectionLockCounts[0]':'th10::g_ChainNestCounter',
    }.items():
        n=chain.count(old)
        if n!=12: raise RuntimeError(f'canonical global-view adaptation count {n}, expected 12: {old}')
        global_view_adaptations[old]=n;chain=chain.replace(old,new)
    # RemoveAllFromList alone has a raw +0x18 host-incompatible load; retain
    # that body except translating this documented list-head field access.
    chain=once(chain,'node = *(ListNode **)(listBase + 0x18);','node = reinterpret_cast<ListNode *>(listBase)->next;')
    proposed=no_includes(read(source))
    # The native source uses the target's one-argument ordinary allocation.
    # This existing host allocator seam controls failure and prefill; it does
    # not claim modern ordinary-new failure/exception behavior.
    proposed=once(proposed,'new TransitionEffectView','HostNewEffect()')
    proposed=once(proposed,'delete effect;','HostDeleteEffect(effect);')
    timer=block(read(canonical/'AnmManager.hpp'),'struct AnmVmTimerView\n')+';\n'
    viewport=block(read(canonical/'AnmManager.hpp'),'struct AnmViewportView\n')+';\n'
    rng_helper=block(read(canonical/'AnmManager.cpp'),'unsigned int __fastcall AnmRandomU32InRange(')
    rectangle=block(read(canonical/'AnmRectangle.hpp'),'struct AnmFloatRectView\n')+';\n'
    pre='''#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits.h>
#include <map>
#include <mutex>
#include <new>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#define __fastcall
#define __stdcall
#define __declspec(x)
struct CRITICAL_SECTION { std::recursive_mutex mutex; int depth=0; };
void EnterCriticalSection(CRITICAL_SECTION *);
void LeaveCriticalSection(CRITICAL_SECTION *);
extern float g_AnmGameSpeed;
'''
    seam='''
struct D3d9DeviceView;
struct HostDeviceVtable { int (*SetViewport)(D3d9DeviceView *, AnmViewportView *); };
struct D3d9DeviceView { HostDeviceVtable *vtable; int id; };
struct AnmRenderManagerView { int id; void FlushVertexBuffer(); };
extern AnmRenderManagerView *g_AnmRenderManagerView;
extern D3d9DeviceView *g_Direct3DDevice;
void DrawColoredRectangle(AnmFloatRectView *, unsigned int);
th10::ChainElem *HostNewNode();
void HostFree(void *);
'''
    alloc='\nTransitionEffectView *HostNewEffect();\nvoid HostDeleteEffect(TransitionEffectView *);\n'
    generated=pre+no_includes(read(canonical/'Chain.hpp'))+'\n'+no_includes(read(canonical/'Rng.hpp'))+'\n'+timer+viewport+rectangle+seam+no_includes(chain)+'\n'+no_includes(read(canonical/'RandomMath.cpp'))+'\n'+rng_helper+'\n'+no_includes(h)+alloc+proposed+'\nconst char *host_proposed_source = R"HOST_SOURCE('+read(source)+')HOST_SOURCE";\n'+read(ROOT/'scripts/fixtures/transition-effect.cpp')
    cpp=out/'harness.cpp';cpp.write_text(generated)
    cmd=['g++','-std=c++17','-O0','-g','-fno-fast-math','-fno-strict-aliasing','-Wall','-Wextra','-Wno-class-memaccess','-Wno-unknown-pragmas',str(cpp),'-o',str(out/'harness')]
    if sanitize:cmd[1:1]=['-fsanitize=address,undefined','-fno-omit-frame-pointer','-no-pie']
    completed=subprocess.run(cmd,text=True,capture_output=True)
    (out/'compile.stdout').write_text(completed.stdout);(out/'compile.stderr').write_text(completed.stderr)
    manifest={'claim':'HOST SEMANTICS ONLY; actual dispatcher algorithm, no target ABI/layout, native compiler, Windows concurrency, renderer or whole-game runtime proof','inputs':{str(p):sha(p) for p in inputs},'input_fingerprints':{str(p):fingerprint(p) for p in inputs},'canonical_observations':canonical_observations,'pin_normalization':'crlf-to-lf-only','generated_sha256':sha(cpp),'extracted_hashes':{'timer_struct':hashlib.sha256(timer.encode()).hexdigest(),'range_helper':hashlib.sha256(rng_helper.encode()).hexdigest(),'adapted_chain':hashlib.sha256(chain.encode()).hexdigest()},'command':cmd,'compile_returncode':completed.returncode,'alloc_field_adaptations':adaptation_counts,'global_view_adaptations':global_view_adaptations,'sanitizers':sanitize}
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    if completed.returncode:raise RuntimeError('HOST COMPILE FAILED: '+completed.stderr)
    return out/'harness'

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'src/TransitionEffect.cpp');ap.add_argument('--canonical',type=Path,default=DEFAULT_CANON);ap.add_argument('--out',type=Path,default=ROOT/'.analysis/transition-effect-host/baseline');ap.add_argument('--sanitize',action='store_true');ap.add_argument('--scenario',default='all');a=ap.parse_args()
    exe=build(a.source,a.out,a.canonical,a.sanitize)
    env=os.environ.copy()
    if a.sanitize: env['ASAN_OPTIONS']='detect_leaks=0' # container tracing blocks LeakSanitizer; allocator ledger checks releases.
    r=subprocess.run([str(exe),a.scenario],text=True,capture_output=True,env=env,timeout=15)
    (a.out/'run.stdout').write_text(r.stdout);(a.out/'run.stderr').write_text(r.stderr)
    manifest=json.loads((a.out/'manifest.json').read_text());manifest.update({'run_returncode':r.returncode,'run_environment':({'ASAN_OPTIONS':'detect_leaks=0'} if a.sanitize else {}),'run_command':[str(exe),a.scenario],'run_stdout_sha256':sha(a.out/'run.stdout'),'run_stderr_sha256':sha(a.out/'run.stderr')});(a.out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print(r.stdout,end='');print(r.stderr,end='',file=sys.stderr)
    return r.returncode
if __name__=='__main__':sys.exit(main())
