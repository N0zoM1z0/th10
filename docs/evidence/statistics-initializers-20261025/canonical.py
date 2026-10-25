from pathlib import Path
import sys,json,hashlib,os,subprocess
root=Path.cwd();assert os.getpriority(os.PRIO_PROCESS,0)>=10;assert len(os.sched_getaffinity(0))==1
out=root/'.analysis/gpt-dots/statistics-initializers-20261025';assert not (root/'build/match-linked/GameStatisticsTail').exists()
paths=[*root.glob('src/*'),root/'config/match-units.toml',root/'config/tools.lock.toml',*root.glob('docs/evidence/d3d-adapter-format/*')]
frozen={str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths if p.is_file()};(out/'canonical-frozen.json').write_text(json.dumps(frozen,indent=2)+'\n')
try:
 for name,cmd in [('cold',['scripts/repo-python','scripts/build-match-unit.py','--unit','game-statistics-load']),('compare',['scripts/repo-python','scripts/compare-linked-function.py','--unit','game-statistics-load','--json'])]:
  r=subprocess.run(cmd,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True);(out/(name+'.log')).write_text(r.stdout);(out/(name+'.exit')).write_text(str(r.returncode)+'\n');print(name,r.returncode,flush=True)
  assert r.returncode==0,r.stdout
finally:
 for p,h in frozen.items():assert hashlib.sha256((root/p).read_bytes()).hexdigest()==h,p
