from pathlib import Path
import json,sys,tomllib,importlib.util,hashlib
sys.path.insert(0,'scripts');spec=importlib.util.spec_from_file_location('compare','scripts/compare-linked-function.py');c=importlib.util.module_from_spec(spec);spec.loader.exec_module(c)
p=Path('.analysis/spt-test-update-20261025');results={}
for name in ['ecl-database-create-host','spt-test-update-callback']:
 r=c.compare_unit(name);assert r['result']=='exact';results[name]=r
for label in ['helpers','draw']:
 out=Path('build/spt-test-update-original-'+label+'-20261025');selected=json.loads((out/'comparison-manifest.json').read_text())
 if label=='draw':
  main=selected['units']['spt-test-draw'];old=selected['units']['ascii-manager-add-small-format-text-gs'];selected['units']['ascii-manager-add-small-format-text-gs']=dict(main,**{k:old[k] for k in ['symbol','functions','target_address','size','linkages']},pdb_source='src/AnmManager.cpp')
  (out/'supplemental-comparison-manifest.json').write_text(json.dumps(selected,indent=2)+'\n')
 for name in selected['units']:
  r=c.compare_unit(name,_unit_manifest=selected);assert r['result']=='exact';results[name]=r;print(name,'exact')
(p/'final-native-recovered-results.json').write_text(json.dumps({'original_native_wrapper_exit':1,'cause':'Final ASCII observer selected primary source instead of actual AnmManager support PDB owner; no build failure','supplement':'Same retained images; corrected only explicit ASCII source-owner graph metadata; no native rerun','results':results},indent=2)+'\n')
frozen=json.loads((p/'native-frozen.json').read_text());assert all(hashlib.sha256(Path(k).read_bytes()).hexdigest()==v for k,v in frozen.items());print('all',len(frozen),'original frozen inputs still unchanged')
