from pathlib import Path
import os,json,subprocess,hashlib,sys,tomllib,importlib.util
root=Path.cwd();p=root/'.analysis/spt-test-update-20261025';sys.path.insert(0,'scripts');import ltcg_link as l
spec=importlib.util.spec_from_file_location('compare','scripts/compare-linked-function.py');c=importlib.util.module_from_spec(spec);spec.loader.exec_module(c)
assert os.getpriority(os.PRIO_PROCESS,0)>=10 and len(os.sched_getaffinity(0))==1
files={root/x for x in subprocess.check_output(['git','ls-files','-z','--cached','--others','--exclude-standard']).decode().split('\0') if x}
for f in root.glob('docs/evidence/*/retained-artifacts.json'):
 try:obj=json.loads(f.read_text())
 except ValueError:continue
 if isinstance(obj,dict):
  for name in obj:
   z=root/name
   if z.is_file() and z.resolve().is_relative_to(root):files.add(z)
frozen={str(z.relative_to(root)):hashlib.sha256(z.read_bytes()).hexdigest() for z in files if z.is_file()};(p/'native-frozen.json').write_text(json.dumps(frozen,indent=2)+'\n')
results={}
try:
 assert not (root/'build/match-linked/SptTestUpdate').exists()
 subprocess.run([str(root/'scripts/repo-python'),'scripts/build-match-unit.py','--unit','ecl-database-create-host'],check=True)
 for name in ['ecl-database-create-host','spt-test-update-callback']:
  r=c.compare_unit(name);assert r['result']=='exact';results[name]=r;print(name,r['result'],flush=True)
 m=tomllib.loads(Path('config/match-units.toml').read_text());linker,env=l.tool_environment()
 for label,key,keys in [('helpers','spt-test-constructor',['spt-test-constructor','spt-test-register-tasks','spt-test-clear-script-names']),('draw','spt-test-draw',['spt-test-draw','spt-test-draw-callback','ascii-manager-add-small-format-text-gs'])]:
  u=m['units'][key];out=root/('build/spt-test-update-original-'+label+'-20261025');assert not out.exists()
  native=l.cold_link(root/u['source'],out,u['entry_symbol'],linker,env,profile=u['profile'],support_sources=[root/x for x in u.get('support_sources',[])])
  (out/'native.json').write_text(json.dumps(native,indent=2,default=str)+'\n')
  selected={'target_sha256':m['target_sha256'],'units':{k:dict(m['units'][k],output_dir=str(out.relative_to(root))) for k in keys}}
  (out/'comparison-manifest.json').write_text(json.dumps(selected,indent=2)+'\n')
  for name in keys:
   r=c.compare_unit(name,_unit_manifest=selected);assert r['result']=='exact';results[name]=r;print(name,r['result'],flush=True)
 (p/'final-native-results.json').write_text(json.dumps(results,indent=2)+'\n')
finally:
 changed=[z for z,h in frozen.items() if not (root/z).is_file() or hashlib.sha256((root/z).read_bytes()).hexdigest()!=h]
 (p/'native-freeze-verification.json').write_text(json.dumps({'frozen_count':len(frozen),'changed':changed},indent=2)+'\n');assert not changed,changed
