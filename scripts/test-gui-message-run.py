#!/usr/bin/env python3
"""Fresh local actual-source repaired interpreter suite. Explicit host seams, no native/remote build.
Full Run/helper namespace and complete canonical header fields are retained.
Timer constructors, SDK-only types, owner lifecycle
and external calls have explicit host seams. Real message construction is excluded.
"""
from pathlib import Path
import argparse,hashlib,json,re,subprocess,sys,os,shutil
from gui_message_run_mutations import MUTATIONS, EXPECTED_MUTATION_COUNT
ROOT=Path(__file__).resolve().parents[1]
FALLBACK=Path('/workspace/shared/th10-baseline-50b971f/src')
SEAMS={'AnmManager.hpp':('AnmVmTimerView() { flags &= ~1u; }','AnmVmTimerView() : flags(0) {}'),'Player.hpp':('PlayerTimerView() { flags &= ~1u; }','PlayerTimerView() : flags(0) {}'),'Enemy.hpp':('EnemyInterpolationTimerView() { flags &= ~1u; }','EnemyInterpolationTimerView() : flags(0) {}')}
def owner(text,sig):
 candidates=[]
 for m in re.finditer(re.escape(sig),text):
  opening=text.find('{',m.end())
  if opening>=0 and ';' not in text[m.end():opening]:candidates.append((m.start(),opening))
 if len(candidates)!=1:raise RuntimeError('owner definition count changed: '+sig)
 start,opening=candidates[0];depth=0
 for m in re.finditer(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|[{}]',text[opening:]):
  if m.group()=='{':depth+=1
  elif m.group()=='}':
   depth-=1
   if depth==0:return text[start:opening+m.end()]
 raise RuntimeError('unclosed owner')
def strip_asserts(text):
 def replace(m):
  if 'GuiMessageScalarWidths' in m.group():return m.group()
  return '// HOST: target layout assertion omitted.' if re.search(r'\?\s*1\s*:\s*-1',m.group()) else m.group()
 return re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;',replace,text)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def source_path(root,fallback,name):
 for folder in (root/'src',fallback):
  if folder is not None and (folder/name).exists():return folder/name
 raise RuntimeError('missing canonical dependency: '+name)
def prepare(root,fallback,out,baseline=True):
 out.mkdir(parents=True,exist_ok=True);inputs={};copied=[]
 pending=['ReplayManager.hpp','Gui.hpp','GuiScore.hpp','EnemySoundQueue.hpp','Main.hpp','Chain.hpp','Player.hpp','Enemy.hpp']
 if not baseline:pending.extend(['GameStageDescriptor.hpp', 'ResultMenu.hpp'])
 while pending:
  name=pending.pop()
  if name in copied:continue
  p=source_path(root,fallback,name);text=p.read_text();inputs[str(p)]=sha(p)
  pending.extend(re.findall(r'^\s*#include\s+"([^"]+)"',text,re.M));text=strip_asserts(text)
  if name in SEAMS:
   before,after=SEAMS[name]
   if text.count(before)!=1:raise RuntimeError('timer seam inventory changed: '+name)
   text=text.replace(before,'// HOST TIMER LIFETIME SEAM.\n    '+after)
  if baseline and name=='Gui.hpp':
   text='struct GuiMessageInstructionView;\nGuiMessageInstructionView *HostRecordNext(GuiMessageInstructionView *);\nint HostRecordIntArgument(const GuiMessageInstructionView *);\n'+text
   for sig,body in [('GuiMessageInstructionView *Next()','{ return HostRecordNext(this); }'),('int IntArgument() const','{ return HostRecordIntArgument(this); }')]:
    original=owner(text,sig);text=text.replace(original,original[:original.index('{')]+body)
  (out/name).write_text(text);copied.append(name)
 (out/'windows.h').write_text('''// HOST SDK-ONLY TYPE STUB. MainSupervisorView is retained in full.
#pragma once
#include <cstdint>
typedef void *HWND;typedef void *HINSTANCE;typedef void *HANDLE;
typedef unsigned int UINT;typedef std::uintptr_t WPARAM;typedef std::intptr_t LPARAM;typedef std::intptr_t LRESULT;
union LARGE_INTEGER {struct {std::uint32_t LowPart;std::int32_t HighPart;};std::int64_t QuadPart;};
struct CRITICAL_SECTION {void *DebugInfo;std::int32_t LockCount;std::int32_t RecursionCount;void *OwningThread;void *LockSemaphore;std::uintptr_t SpinCount;};
''')
 p=source_path(root,fallback,'Gui.cpp');text=p.read_text();inputs[str(p)]=sha(p)
 if not baseline:
  if 'extern unsigned char g_GameStageState[];' not in text or 'th10::leaf::Fn004175E0(g_GameStageState);' not in text:raise RuntimeError('canonical stage-state leaf receiver declaration/call changed')
 run=owner(text,'int __stdcall GuiMessageVmView::Run(GuiMessageVmView *message)')
 (out/'owner.inc').write_text(strip_asserts(text[:text.index(run)+len(run)])+'\n')
 dependencies=[]
 signatures=[('AnmManager.cpp',['AnmVmView *AnmRenderManagerView::FindVm(AnmVmIdView id)','void __stdcall AnmRenderManagerView::SetVmPosition(','void AnmRenderManagerView::MarkVmForDeletion(AnmVmIdView id)','void AnmVmTimerView::Add(float value)']),('AnmVmId.cpp',['void AnmVmIdView::SetInterrupt(short interrupt)','void AnmVmIdView::SetSprite(int spriteIndex)','void AnmVmIdView::SetSpriteWithAnm(AnmLoadedView *loaded, int spriteIndex)'])]
 if not baseline:signatures += [('FrontEnd.cpp',['AnmVmIdView ResultMenuFindScript(AnmVmIdView *id, int scriptIndex)','void ResultMenuPendingThree(AnmVmIdView *id)','int __stdcall FrontEndPlayMusic(int slot, int trackIndex)']),('Player.cpp',['void PlayerSetShotVmInterrupt2(unsigned int *vmId)']),('EnemyEclDispatcher.cpp',['__declspec(noinline) void EnemyMarkPendingInterrupt(int *id)','void EnemySoundQueueView::QueueSoundSample(int soundId, int sample)']),('Main.cpp',['void MainSupervisorView::SetNextGameMode(int mode)'])]
 for name,sigs in signatures:
  p=source_path(root,fallback,name);inputs[str(p)]=sha(p);text=p.read_text();dependencies += [owner(text,s) for s in sigs]
 (out/'dependencies.inc').write_text('\n\n'.join(dependencies)+'\n')
 if not baseline:
  p=source_path(root,fallback,'GameManager.cpp');inputs[str(p)]=sha(p)
  types='typedef unsigned char u8;\nstruct GameChainElementView;\n'+owner(p.read_text(),'struct GameChainOwnerView')+';\n'
  types+=owner(source_path(root,fallback,'EnemyEclDispatcher.cpp').read_text(),'struct EnemySoundCueMetadataView')+';\n'
  (out/'canonical-dependency-types.inc').write_text(types)
 return {'source_inputs_sha256':inputs,'canonical_headers':sorted(copied),'owner_sha256':hashlib.sha256(run.encode()).hexdigest(),'timer_constructor_seams':SEAMS,'record_accessor_seams':['Next','IntArgument'] if baseline else [],'baseline':baseline}
def intended_failure(r,check):return r.returncode==1 and r.stdout=='' and r.stderr=='CHECK '+check+'\n'
def classifier():
 assert intended_failure(subprocess.CompletedProcess([],1,'','CHECK x\n'),'x')
 for code,out,err in [(-11,'','CHECK x\n'),(139,'','CHECK x\n'),(0,'','CHECK x\n'),(2,'','CHECK x\n'),(1,'x','CHECK x\n'),(1,'','CHECK other\n'),(1,'','compiler error'),(1,'','CHECK x\nextra\n')]:assert not intended_failure(subprocess.CompletedProcess([],code,out,err),'x')
def compile_command(out,fixture,sanitize):
 c=['flock','/tmp/gui-message-host-tests.lock','nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Wno-unused-function','-I',str(out),str(fixture),'-o',str(out/'run-test')]
 if sanitize:c[7:7]=['-fsanitize=address,undefined', '-fno-sanitize-recover=all', '-fno-pie', '-no-pie']
 return c
def main():
 if len(MUTATIONS) != EXPECTED_MUTATION_COUNT or len({row[0] for row in MUTATIONS}) != EXPECTED_MUTATION_COUNT:
  raise RuntimeError("strict mutation catalog must contain all 28 unique controls")
 here=Path(__file__).resolve().parent;p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--root',type=Path,default=ROOT);p.add_argument('--fallback-root',type=Path,default=FALLBACK if FALLBACK.exists() else None);p.add_argument('--out',type=Path,default=here/'evidence');p.add_argument('--fixture',type=Path);p.add_argument('--sanitize',action='store_true');p.add_argument('--no-mutations',action='store_true');a=p.parse_args();classifier()
 fixture=a.fixture or next((p for p in [here/'gui-message-run.cpp',here/'fixtures/gui-message-run.cpp'] if p.exists()),here/'gui-message-run.cpp')
 out=a.out.resolve();m=prepare(a.root.resolve(),a.fallback_root,out,False);c=compile_command(out,fixture,a.sanitize)
 r=subprocess.run(c,text=True,capture_output=True,timeout=180);(out/'compile.stdout').write_text(r.stdout);(out/'compile.stderr').write_text(r.stderr)
 m.update(compile_command=c,compile_returncode=r.returncode,driver_sha256=sha(Path(__file__)),fixture_sha256=sha(fixture),mutation_catalog_sha256=sha(here/'gui_message_run_mutations.py'),mutation_catalog_count=len(MUTATIONS),sanitize=a.sanitize,asan_options='detect_leaks=0: host ptrace excludes LeakSanitizer; no leak claim')
 if r.returncode:raise RuntimeError('compile error is not evidence: '+str(out/'compile.stderr'))
 runs=[]
 for case in ['control','portraits','sides','hides','flags','wait','sprites','text','music','names','complete','effects','exit']:
  r=subprocess.run([str(out/'run-test'),case],text=True,capture_output=True,timeout=30,env={**os.environ,'ASAN_OPTIONS':'detect_leaks=0'});(out/(case+'.stdout')).write_text(r.stdout);(out/(case+'.stderr')).write_text(r.stderr)
  runs.append(dict(case=case,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr,accepted=r.returncode==0 and r.stdout==r.stderr==''))
 m['runs']=runs;(out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
 if not all(r['accepted'] for r in runs):raise RuntimeError('positive outcome mismatch: '+str(out/'manifest.json'))
 results=[]
 if not a.no_mutations:
  for name,target,before,after,selection,check,count in MUTATIONS:
   d=out/'mutations'/name;d.mkdir(parents=True,exist_ok=True)
   for path in out.iterdir():
    if path.suffix in ('.h','.hpp','.inc'):shutil.copy2(path,d/path.name)
   path=d/target;text=path.read_text()
   if text.count(before)!=count:raise RuntimeError(name+': mutation anchor changed')
   path.write_text(text.replace(before,after));mc=compile_command(d,fixture,a.sanitize)
   r=subprocess.run(mc,text=True,capture_output=True,timeout=180);(d/'compile.stdout').write_text(r.stdout);(d/'compile.stderr').write_text(r.stderr)
   if r.returncode:raise RuntimeError(name+': compile error is not mutation evidence')
   r=subprocess.run([str(d/'run-test'),selection],text=True,capture_output=True,timeout=30,env={**os.environ,'ASAN_OPTIONS':'detect_leaks=0'});(d/'run.stdout').write_text(r.stdout);(d/'run.stderr').write_text(r.stderr)
   results.append(dict(name=name,target=target,selection=selection,expected_check=check,compile_returncode=0,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr,accepted=intended_failure(r,check)))
   (out/'mutations.json').write_text(json.dumps(results,indent=2)+'\n')
   if not results[-1]['accepted']:raise RuntimeError(name+': incorrect mutation outcome')
 m['mutations']=results
 m['source_hashes_still_current']=all(sha(Path(path))==digest for path,digest in m['source_inputs_sha256'].items())
 (out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
 if not m['source_hashes_still_current']:raise RuntimeError('source changed during test; rerun required')
 print('Fresh repaired suite verified: 13 selections pass; '+str(len(results))+' strict mutants detected.')
if __name__=='__main__':main()
