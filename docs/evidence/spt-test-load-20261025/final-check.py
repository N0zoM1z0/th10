from pathlib import Path
import json,hashlib,sys,importlib.util
root=Path.cwd();sys.path.insert(0,'scripts')
spec=importlib.util.spec_from_file_location('compare','scripts/compare-linked-function.py');c=importlib.util.module_from_spec(spec);spec.loader.exec_module(c)
p=root/'docs/evidence/spt-test-load-20261025';retained=json.loads((p/'retained-artifacts.json').read_text())
for path,digest in retained.items():assert hashlib.sha256((root/path).read_bytes()).hexdigest()==digest,path
results={}
for label in ['first','context']:
 b=root/('build/spt-test-load-'+label+'-20261025');m=json.loads((b/'comparison-manifest.json').read_text());iv=json.loads((b/'inventory.json').read_text());fr=json.loads((b/'frozen-inputs.json').read_text())
 for path,digest in fr.items():assert hashlib.sha256((root/path).read_bytes()).hexdigest()==digest,path
 assert len(iv['load']['target']['fields'])==8 and len(iv['callback']['target']['fields'])==2
 assert len(iv['load']['target']['locals'])==6
 if label=='context':assert iv['load']['target']['locals']==iv['load']['candidate']['locals']
 rows={}
 for k in ['spt-test-draw','ascii-manager-add-small-format-text-gs','enemy-ecl-resource-base-constructor','callback']:
  r=c.compare_unit(k,_unit_manifest=m);assert r['result']=='exact',(label,k);rows[k]={'result':r['result'],'size':r['size'],'fields':len(r['linkages'])}
 try:r=c.compare_unit('load',_unit_manifest=m)
 except ValueError as e:r={'result':'rejected','error':str(e)}
 assert r['result']==('rejected' if label=='first' else 'mismatch')
 results[label]={'guards':rows,'load_result':r['result'],'frozen_inputs':len(fr)}
print(json.dumps({'retained_artifacts_unchanged':len(retained),'results':results},indent=2))
