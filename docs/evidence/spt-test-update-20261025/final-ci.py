from pathlib import Path
import os,json,subprocess,hashlib,sys
root=Path.cwd();out=root/'.analysis/spt-test-update-20261025';assert os.getpriority(os.PRIO_PROCESS,0)>=10;assert len(os.sched_getaffinity(0))==1
files={root/x for x in subprocess.check_output(['git','ls-files','-z','--cached','--others','--exclude-standard']).decode().split('\0') if x}
for f in root.glob('docs/evidence/*/retained-artifacts.json'):
 try:obj=json.loads(f.read_text())
 except ValueError:continue
 if isinstance(obj,dict):
  for name in obj:
   p=root/name
   if p.is_file() and p.resolve().is_relative_to(root):files.add(p)
frozen={str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in files if p.is_file()};(out/'final-ci-frozen.json').write_text(json.dumps(frozen,indent=2)+'\n')
result=subprocess.run([str(root/'scripts/repo-python'),'scripts/ci.py'],cwd=root)
changed=[p for p,h in frozen.items() if not(root/p).is_file() or hashlib.sha256((root/p).read_bytes()).hexdigest()!=h]
(out/'final-ci-verification.json').write_text(json.dumps({'ci_exit':result.returncode,'frozen_count':len(frozen),'changed':changed},indent=2)+'\n');assert not changed,changed
sys.exit(result.returncode)
