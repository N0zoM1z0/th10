#!/usr/bin/env python3
"""Portable canonical ReplayManager Run controls; synthetic score ordering witnesses.
No native/PE execution or target layout/lifecycle equivalence claim.
"""
import argparse, importlib.util, json, os, shutil, subprocess, sys
from pathlib import Path
HERE=Path(__file__).resolve().parent
if str(HERE) not in sys.path:sys.path.insert(0,str(HERE))
spec=importlib.util.spec_from_file_location('run_driver',HERE/'test-gui-message-run.py')
run=importlib.util.module_from_spec(spec);spec.loader.exec_module(run)
# Predeclared diagnostics; every mutation has a unique exact source occurrence.
MUTATIONS=[]
for stage,index in [(6,0),(7,1)]:
 for label,expression,check in [
  ('nonzero','g_ReplayManager->mode != 0','replay.canonical_mode'),
  ('constant-record','false','replay.canonical_mode'),
  ('constant-playback','true','replay.canonical_mode'),
  ('wrong-owner','hostReplayDecoy->mode == 1','replay.canonical_mode'),
  ('stale-pointer','cachedReplay->mode == 1',f'replay.synthetic_post_score{stage}'),
  ('stale-mode','cachedReplayMode == 1',f'replay.synthetic_post_score{stage}')]:
  MUTATIONS.append((f'{label}-{stage}',index,expression,check))
EXPECTED_MUTATION_COUNT=12
def replace_once(text,before,after,label):
 if text.count(before)!=1:raise RuntimeError(label+': expected exactly one anchor')
 return text.replace(before,after)
def structural_guard(source):
 if 'g_GuiModeOwner' in source or 'GuiModeOwnerView' in source:raise RuntimeError('retired shadow owner survives')
 if source.count('g_ReplayManager->mode == 1')!=2:raise RuntimeError('canonical mode read inventory changed')
 if source.count('#include "ReplayManager.hpp"')!=1:raise RuntimeError('canonical replay header include inventory changed')
def structural_negative_controls(source):
 controls=[('retired-shadow',source+'\nextern void *g_GuiModeOwner;\n'),('replaced-read',source.replace('g_ReplayManager->mode == 1','false',1))]
 results=[]
 for name,changed in controls:
  try:structural_guard(changed)
  except RuntimeError as error:results.append(dict(name=name,rejected=True,diagnostic=str(error)))
  else:raise RuntimeError('structural negative accepted: '+name)
 return results
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--root',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--sanitize',action='store_true');p.add_argument('--fallback-root',type=Path);p.add_argument('--no-mutations',action='store_true');a=p.parse_args()
 if len(MUTATIONS)!=EXPECTED_MUTATION_COUNT or len({row[0] for row in MUTATIONS})!=EXPECTED_MUTATION_COUNT:raise RuntimeError('replay catalog must contain 12 unique controls')
 run.classifier();out=a.out.resolve()
 if out.exists():raise RuntimeError('evidence output must not already exist: '+str(out))
 tracked=[Path(__file__).resolve(),HERE/'test-gui-message-run.py',HERE/'gui_message_run_mutations.py',HERE/'fixtures/gui-message-run.cpp']
 test_inputs={str(path):run.sha(path) for path in tracked}
 m=run.prepare(a.root.resolve(),a.fallback_root,out,False)
 source=(out/'owner.inc').read_text()
 structural_guard(source)
 m['structural_negative_controls']=structural_negative_controls(source)
 m['test_inputs_sha256']=test_inputs
 fixture=(HERE/'fixtures/gui-message-run.cpp').read_text()
 fixture=replace_once(fixture,'#include "owner.inc"','#include "ReplayManager.hpp"\nstatic ReplayManager *hostReplayDecoy;\n#include "owner.inc"','fixture canonical decoy declaration')
 fixture=replace_once(fixture,'g_ReplayManager=&mode;','g_ReplayManager=&mode;hostReplayDecoy=&mode;','fixture canonical decoy assignment')
 (out/'fixture.cpp').write_text(fixture)
 score=out/'GuiScore.hpp';s=score.read_text();old=run.owner(s,'void Add(int amount)')
 # Deliberate SYNTHETIC replacement retaining arithmetic for existing tests.
 new=old[:-1]+'    if (syntheticAfterScore) syntheticAfterScore();\n    }'
 if not old.endswith('}'):raise RuntimeError('synthetic score body boundary changed')
 score.write_text(replace_once(s,old,new,'synthetic score dependency'))
 m.update(synthetic_dependency='GuiScoreView::Add replaced with arithmetic plus callback; no claim actual score changes replay owner',controls=[],sanitize=a.sanitize)
 def execute(folder,label,check=None):
  (folder/'fixture.cpp').write_text(fixture)
  c=run.compile_command(folder,folder/'fixture.cpp',a.sanitize);c.insert(7,'-DHOST_SYNTHETIC_SCORE_REPLACEMENT')
  b=subprocess.run(c,capture_output=True,text=True,timeout=120)
  (folder/'compile.stdout').write_text(b.stdout);(folder/'compile.stderr').write_text(b.stderr)
  if b.returncode:raise RuntimeError(label+': compilation failure is not evidence')
  r=subprocess.run([str(folder/'run-test'),'replay'],capture_output=True,text=True,timeout=15,env={**os.environ,'ASAN_OPTIONS':'detect_leaks=0'})
  accepted=run.intended_failure(r,check) if check else r.returncode==0 and r.stdout==r.stderr==''
  m['controls'].append(dict(name=label,expected_check=check,compile_returncode=b.returncode,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr,accepted=accepted))
  (out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
  if not accepted:raise RuntimeError(label+': unexpected result '+repr((r.returncode,r.stdout,r.stderr)))
 execute(out,'positive')
 for label,index,expression,check in ([] if a.no_mutations else MUTATIONS):
  d=out/label;d.mkdir(exist_ok=True)
  for file in out.iterdir():
   if file.suffix in ('.h','.hpp','.inc'):shutil.copy2(file,d/file.name)
  t=source
  needle='g_ReplayManager->mode == 1';positions=[match.start() for match in run.re.finditer(run.re.escape(needle),t)]
  pos=positions[index];t=t[:pos]+expression+t[pos+len(needle):]
  if label.startswith('stale-'):
   sig='int __stdcall GuiMessageVmView::Run(GuiMessageVmView *message)'
   opening=t.index('{',t.index(sig))
   capture='ReplayManager *cachedReplay = g_ReplayManager;' if label.startswith('stale-pointer') else 'int cachedReplayMode = g_ReplayManager->mode;'
   t=t[:opening+1]+'\n    '+capture+t[opening+1:]
  (d/'owner.inc').write_text(t);execute(d,label,check)
 m['source_hashes_still_current']=all(run.sha(Path(p))==h for p,h in m['source_inputs_sha256'].items())
 m['test_hashes_still_current']=all(run.sha(Path(p))==h for p,h in test_inputs.items())
 m['driver_sha256']=test_inputs[str(Path(__file__).resolve())];m['fixture_sha256']=test_inputs[str(HERE/'fixtures/gui-message-run.cpp')]
 (out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
 if not m['source_hashes_still_current'] or not m['test_hashes_still_current']:raise RuntimeError('source changed during tests')
 print('PASS: canonical replay positive and '+str(len(m['controls'])-1)+' strict predeclared controls')
if __name__=='__main__':main()
