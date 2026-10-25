import sys,json,tomllib,importlib.util,copy
from pathlib import Path
sys.path.insert(0,'scripts')
spec=importlib.util.spec_from_file_location('compare','scripts/compare-linked-function.py');c=importlib.util.module_from_spec(spec);spec.loader.exec_module(c)
model=sys.argv[1];folder={'first':'candidate','typed':'typed','ordered':'ordered','tail-only':'tail-only'}[model]
p=Path('build/statistics-initializers-'+model+'-20261025')
m=json.loads((p/'comparison-manifest.json').read_text());base=copy.deepcopy(m['units']['load']);base.pop('linkages')
string='??_C@_08GGFKNFHG@?5?5?5?5?5?5?5?5?$AA@'
links=[dict(offset=o,type='DIR32',symbol=string,target=0x46e354,addend=a) for o,a in [(0x1f,0),(0x2a,4),(0x33,8)]]
links += [dict(offset=o,type='DIR32',symbol='?g_RngView@@3URngView@@A',target=0x4918b0,addend=a) for o,a in [(0x51,0),(0x57,4),(0x7b,0),(0x81,4)]]
m['units']['tail']=dict(base,symbol='?InitializeStatisticsTail42ADD0@@YGXPAUGameStatisticsTail@@@Z',functions=['InitializeStatisticsTail42ADD0'],size=142,target_address=0x42add0,linkages=links)
g=json.loads(Path('.analysis/gpt-dots/statistics-load-20261025/canonical-guards-manifest.json').read_text())
for name,unit in g['units'].items():
 unit.update({k:base[k] for k in ['source','support_sources','normal_support_sources','profile','output_dir','entry_symbol','harness','link_profile']});m['units'][name]=unit
canon=tomllib.loads(Path('config/match-units.toml').read_text())
for name,unit in canon['units'].items():
 if unit.get('source')=='src/RandomMath.cpp':
  unit=copy.deepcopy(unit);unit.update({k:base[k] for k in ['source','support_sources','normal_support_sources','profile','output_dir','entry_symbol','harness','link_profile']});unit['pdb_source']='src/RandomMath.cpp';unit['artifact_kind']='linked-pe';unit['linkages']=unit.pop('relocations',[]);unit.pop('object',None);m['units'][name]=unit
(p/'full-comparison-manifest.json').write_text(json.dumps(m,indent=2)+'\n')
results={}
for name in m['units']:
 try:results[name]=c.compare_unit(name,_unit_manifest=m)
 except Exception as e:results[name]={'result':'rejected','error':str(e)}
 print(name,results[name].get('result'),results[name].get('error',''))
(p/'full-comparison.json').write_text(json.dumps(results,indent=2)+'\n')
