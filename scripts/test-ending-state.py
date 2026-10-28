#!/usr/bin/env python3
"""Actual Ending source with explicit SDK, asset, ANM and worker host roles.

Host-only clear retains the live worker vptr, while clearing every ordinary
field. Timer/node constructor seeds define prefill for this host scenario.
The host allocation event uses explicit storage, placement construction and
throw cleanup; actual VC7.1 new-expression order is audited separately.
No native ABI/layout, worker scheduling or target runtime proof is claimed.
"""
from pathlib import Path
import argparse, hashlib, json, os, re, subprocess, tempfile
ROOT = Path(__file__).resolve().parents[1]

def body(text, signature):
    if text.count(signature) != 1: raise ValueError('nonunique owner: ' + signature)
    start = text.index(signature); opening = text.index('{', start); depth = 0
    for index in range(opening, len(text)):
        if text[index] == '{': depth += 1
        elif text[index] == '}':
            depth -= 1
            if depth == 0: return text[start:index + 1]
    raise ValueError('unclosed owner: ' + signature)

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize', action='store_true')
    parser.add_argument('--no-mutations', action='store_true')
    parser.add_argument('--out', type=Path, help='retain generated fixture and input manifest')
    args = parser.parse_args()
    if args.out is not None:
        run_suite(args)
    else:
        host_root = ROOT/'build/host-tests'
        host_root.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='ending-state-', dir=host_root) as output:
            args.out = Path(output)
            run_suite(args)
def run_suite(args):
    args.out.mkdir(parents=True, exist_ok=True)
    pending = ['Ending.hpp','Main.hpp','InputState.hpp','FileSystem.hpp','GameErrorContext.hpp','EnemySoundQueue.hpp','TransitionEffect.hpp']; seen=set(); hashes={}
    while pending:
        name = pending.pop()
        if name in seen: continue
        seen.add(name); path=ROOT/'src'/name; text=path.read_text(); hashes[str(path)]=hashlib.sha256(path.read_bytes()).hexdigest()
        pending.extend(re.findall(r'^\s*#\s*include\s*"([^"]+)"', text, re.M))
        text=re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;', '// HOST native-layout assertion omitted.', text)
        if name=='AnmManager.hpp':
            old='AnmVmTimerView() { flags &= ~1u; }'; assert text.count(old)==1
            text=text.replace(old,'AnmVmTimerView() : flags(0) {} // HOST initialized timer prefill.')
        (args.out/name).write_text(text)
    (args.out/'windows.h').write_text('''#pragma once
#include <cstdint>
typedef void *HWND; typedef void *HINSTANCE; typedef void *HANDLE;
typedef unsigned int UINT; typedef std::uintptr_t WPARAM; typedef std::intptr_t LPARAM; typedef std::intptr_t LRESULT;
union LARGE_INTEGER { struct {std::uint32_t LowPart;std::int32_t HighPart;}; std::int64_t QuadPart; };
struct CRITICAL_SECTION {void *DebugInfo;std::int32_t LockCount;std::int32_t RecursionCount;void *OwningThread;void *LockSemaphore;std::uintptr_t SpinCount;};
extern "C" void EnterCriticalSection(CRITICAL_SECTION *);
extern "C" void LeaveCriticalSection(CRITICAL_SECTION *);
extern "C" void Sleep(unsigned int);
extern "C" unsigned int WaitForSingleObject(void *, unsigned int);
extern "C" int CloseHandle(void *);
#define WAIT_TIMEOUT 258
''')
    source=(ROOT/'src/Ending.cpp').read_text(); hashes['src/Ending.cpp']=hashlib.sha256((ROOT/'src/Ending.cpp').read_bytes()).hexdigest()
    assert source.count('memset(this, 0, sizeof(*this));')==2
    source=source.replace('memset(this, 0, sizeof(*this));','HostClearEndingText(this);')
    ctor=body(source,'EndingTaskView()'); assert ctor.count('flags &= ~1u;')==1
    source=source.replace(ctor,ctor.replace('flags &= ~1u;','flags = 0xa5a5a5a5u & ~1u; // HOST defined node prefill.'))
    source=re.sub(r'^\s*#include[^\n]*\n','',source,flags=re.M)
    signatures={'AnmManager.cpp':['AnmVmView *AnmRenderManagerView::FindVm(AnmVmIdView id)','void AnmRenderManagerView::MarkVmForDeletion(AnmVmIdView id)','void AnmRenderManagerView::ReleaseAnm(int index)','void AnmVmTimerView::Initialize()','void AnmVmTimerView::Add(float value)','AnmVmIdView AnmLoadedView::CreateVmVariant0(','AnmVmIdView AnmLoadedView::CreateVmVariant1('], 'AnmVmId.cpp':['AnmVmView *AnmVmIdView::GetVm()','void AnmVmIdView::Release()'], 'Player.cpp':['void PlayerSetShotVmInterrupt2(unsigned int *vmId)'], 'FrontEnd.cpp':['void ResultMenuPendingThree(AnmVmIdView *id)'], 'EnemyEclDispatcher.cpp':['__declspec(noinline) void EnemyMarkPendingInterrupt(int *id)','void EnemySoundQueueView::QueueSoundSample(int soundId, int sample)'], 'ScriptViewer.cpp':['ScriptViewerWorkerView::ScriptViewerWorkerView()'], 'ScriptViewerWorker.cpp':['ScriptViewerWorkerView::~ScriptViewerWorkerView()','void ScriptViewerWorkerView::Stop()','void ScriptViewerWorkerView::Start(']}
    signatures['Chain.cpp']=['ChainElem::ChainElem()', 'Chain::Chain()']
    dependencies=[]
    for name,sigs in signatures.items():
        path=ROOT/'src'/name; text=path.read_text();hashes[str(path)]=hashlib.sha256(path.read_bytes()).hexdigest()
        extracted=[body(text,s) for s in sigs]
        if name=='Chain.cpp':
            dependencies.append('namespace th10 {\n'+'\n'.join(extracted)+'\n}')
        else:
            dependencies.extend(extracted)
    path=ROOT/'src/DecomphelpLeafAccessors.cpp'; hashes[str(path)]=hashlib.sha256(path.read_bytes()).hexdigest(); leaf=path.read_text()
    dependencies.append('namespace th10 { namespace leaf {\n'+body(leaf,'__declspec(noinline) u32 Fn0040BA80(void)')+'\n'+body(leaf,'__declspec(noinline) u8 * Fn00417010(u8 *p0)')+'\n} }')
    (args.out/'dependencies.inc').write_text('\n\n'.join(dependencies))
    variants=[('positive',source,None)]
    if not args.no_mutations:
        old='th10::leaf::Fn00417010(instruction + 4)'; assert source.count(old)==1
        variants.append(('stale_text',source.replace(old,'th10::leaf::Fn00417010(command + 4)'),'text-live-cursor'))
        # Deliberately pass the captured pre-creation cursor to a faulty copy
        # of the ordinary publication helper. Production reads the live cursor
        # inside the helper after creation completes, including under C++03.
        old='text->animationIds[EndingRecordInt(text->instruction)] = created.value;'
        assert source.count(old)==1
        stale=source.replace(old,'text->animationIds[EndingRecordInt(oldCursor)] = created.value;')
        signature='const AnmVmIdView &created)'; assert stale.count(signature)==1
        stale=stale.replace(signature,'const AnmVmIdView &created, const unsigned char *oldCursor)')
        call='CreateVmVariant0(EndingRecordInt(instruction, 2), 15));'; assert stale.count(call)==4
        stale=stale.replace(call,'CreateVmVariant0(EndingRecordInt(instruction, 2), 15), command);')
        variants.append(('stale_animation',stale,'animation-live-destination'))
        old='CreateVmVariant0(EndingRecordInt(instruction, 2), 15)'; assert source.count(old)==4
        variants.append(('secondary_chain',source.replace(old,'CreateVmVariant1(EndingRecordInt(instruction, 2), 15)'),'animation-primary-chain'))
        old='flags |= 2;\n                continue;'; assert source.count(old)==1
        variants.append(('reload_skip',source.replace(old,'flags |= 2;\n                break;'),'reload-first-command'))
        old='g_FrontEndInput.pressed & 0x1001'; assert source.count(old)==2
        variants.append(('wrong_press',source.replace(old,'g_FrontEndInput.current & 0x1001'),'wait-pressed-edge'))
        old='auxiliaryId =\n            g_AsciiManagerView->asciiAnm'; assert source.count(old)==1
        variants.append(('rebound_selection',source.replace(old,'g_AsciiManagerView->auxiliaryVm89A4 =\n            g_AsciiManagerView->asciiAnm'),'selection-original-owner'))
        old='text = new EndingTextVmView(EndingScriptStart(scriptData));'; assert source.count(old)==1
        new='unsigned char *early = EndingScriptStart(scriptData);\n    text = new EndingTextVmView(early);'
        variants.append(('early_header',source.replace(old,new),'allocation-live-header'))
    for name,variant,expected in variants:
        # HOST allocation event is explicit: the native auditor separately checks
        # actual VC7.1 new-expression order. Preserve constructor-throw cleanup.
        opening='text = new EndingTextVmView('; assert variant.count(opening)==1
        start=variant.index(opening); first=start+len(opening); cursor=first; depth=1
        while depth:
            depth+=(variant[cursor]=='(')-(variant[cursor]==')'); cursor+=1
        assert variant[cursor]==';'
        argument=variant[first:cursor-1]
        replacement='{ void *endingStorage = HostAllocateEndingText(this);\n    try { text = new (endingStorage) EndingTextVmView('+argument+'); }\n    catch (...) { ::operator delete(endingStorage); throw; } }'
        variant=variant[:start]+replacement+variant[cursor+1:]
        (args.out/'owner.inc').write_text(variant)
        command=['flock','/tmp/th10-ending-host.lock','nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Wno-unused-function','-Wno-sign-compare','-Wno-class-memaccess','-I',str(args.out),str(ROOT/'scripts/fixtures/ending-state.cpp'),'-o',str(args.out/'fixture')]
        if args.sanitize:command+=['-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']
        built=subprocess.run(command,capture_output=True,text=True)
        if built.returncode:raise RuntimeError(built.stdout+built.stderr)
        run=subprocess.run([str(args.out/'fixture')],capture_output=True,text=True,env={**os.environ,'ASAN_OPTIONS':'detect_leaks=1'})
        want=(0,'','') if expected is None else (1,'','CHECK '+expected+'\n')
        if (run.returncode,run.stdout,run.stderr)!=want:raise RuntimeError(f'{name}: {run.returncode}\n{run.stdout}{run.stderr}')
        print('PASS '+name)
    (args.out/'manifest.json').write_text(json.dumps({'scope':__doc__,'inputs':hashes,'variants':[n for n,_,_ in variants],'sanitize':args.sanitize,'canonical_headers':sorted(seen),'seams':['SDK functions/types','ANM allocation/resource/draw roles','host vptr-preserving clear','explicit host allocation event/placement construction/throw cleanup','defined timer/node prefill','assets/logging/effects/music','static live VM/list objects'],'actual_bodies':signatures},indent=2)+'\n')
    print('Ending actual source and '+str(len(variants)-1)+' strict mutants pass')

if __name__=='__main__':main()
