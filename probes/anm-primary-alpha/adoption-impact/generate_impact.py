from pathlib import Path
import hashlib,json,tomllib,re,collections
import argparse
parser=argparse.ArgumentParser(description="Read-only pinned alpha source/config dependency audit; no builds")
parser.add_argument('--source-root',required=True,type=Path)
parser.add_argument('--config',required=True,type=Path)
parser.add_argument('--manifest',required=True,type=Path)
parser.add_argument('--output-dir',required=True,type=Path)
a=parser.parse_args();out=a.output_dir;mp=a.manifest;cp=a.config
manifest=json.loads(mp.read_text())
expected_config='8811679fb37ab220216a8926a9ce46b8637335e47cea7cd8d5d8a78b6004cc88'
if manifest['canonical_config_sha256']!=expected_config or hashlib.sha256(cp.read_bytes()).hexdigest()!=expected_config:
 raise SystemExit('Canonical configuration pin mismatch')
pins=manifest['source_files']
if len(pins)!=83 or hashlib.sha256(json.dumps(pins,sort_keys=True,separators=(',',':')).encode()).hexdigest()!='68e2f9b68b1999ac809596765cdd6e9674c8f7903cb9c09f34ba1d7fc539a888':
 raise SystemExit('Source inventory pin mismatch')
u=tomllib.loads(cp.read_text())['units'];sources={};locations={}
for rel,h in pins.items():
 if Path(rel).is_absolute() or '..' in Path(rel).parts:
  raise SystemExit('Invalid source-relative path')
 p=a.source_root/rel
 if hashlib.sha256(p.read_bytes()).hexdigest()!=h:
  raise SystemExit('Source pin mismatch: '+rel)
 sources[rel]=p.read_text();locations[rel]=rel
includes={};unresolved=[]
for rel,t in sources.items():
 includes[rel]=[]
 for inc in re.findall(r'^\s*#\s*include\s*"([^"]+)"',t,re.M):
  target=str(Path(rel).parent/inc)
  if target in sources: includes[rel].append(target)
  else:unresolved.append([rel,inc])
def path_to_header(s,seen=()):
 if s=='src/AnmManager.hpp':return [s]
 if s in seen:return None
 for i in includes.get(s,[]):
  p=path_to_header(i,seen+(s,))
  if p:return [s]+p
 return None
keys=['artifact_kind','source','profile','support_sources','normal_support_sources','link_profile','harness','entry_symbol']
groups={};affected={};unaffected=[]
for name,v in u.items():
 tus=([v['source']] if 'source'in v else [])+v.get('support_sources',[])+v.get('normal_support_sources',[])
 paths={s:path_to_header(s) for s in tus if path_to_header(s)}
 if not paths and 'src/AnmManager.cpp' not in tus:unaffected.append(name);continue
 recipe={k:v.get(k,[] if k.endswith('sources') else None) for k in keys}
 if v['artifact_kind']=='coff':recipe['object']=v.get('object')
 key=json.dumps(recipe,sort_keys=True);gid=hashlib.sha256(key.encode()).hexdigest()[:12]
 g=groups.setdefault(gid,{'recipe':recipe,'units':[],'output_dirs':[],'header_paths':paths,'contains_changed_cpp':'src/AnmManager.cpp' in tus})
 g['units'].append(name)
 if v.get('output_dir') and v['output_dir'] not in g['output_dirs']:g['output_dirs'].append(v['output_dir'])
 affected[name]={'group':gid,'function':v['functions'],'symbol':v['symbol'],'target_address':hex(v['target_address']),'size':v['size'],'pdb_source':v.get('pdb_source'),'owner_visibility':v.get('owner_visibility')}
refs=[]
for rel,t in sources.items():
 for n,line in enumerate(t.splitlines(),1):
  if 'StartPrimaryAlphaInterpolation' in line:refs.append({'file':rel,'line':n,'text':line.strip()})
config_refs=[name for name,v in u.items() if 'StartPrimaryAlphaInterpolation' in json.dumps(v)]
d={'config_sha256':manifest['canonical_config_sha256'],'source_manifest_sha256':hashlib.sha256(mp.read_bytes()).hexdigest(),'source_pins':manifest['source_files'],'source_locations':locations,'unresolved_quoted_includes':unresolved,'units_total':len(u),'affected_count':len(affected),'recipe_group_count':len(groups),'groups':groups,'affected_units':affected,'unaffected_units':unaffected,'helper_source_references':refs,'helper_config_references':config_refs}
if unresolved:
 raise SystemExit('Unresolved quoted includes: '+str(unresolved))
if len(affected)!=256 or len(groups)!=93:
 raise SystemExit('Expected affected-unit or recipe count changed')
if out.resolve()==a.source_root.resolve() or out.resolve().is_relative_to(a.source_root.resolve()):
 raise SystemExit('Output directory must be outside source root')
for parent in (out,*out.parents):
 if parent.is_symlink():
  raise SystemExit('Symlink output path is unsupported')
out.mkdir(parents=True,exist_ok=False)
(out/'impact.json').write_text(json.dumps(d,indent=2)+'\n')
print(json.dumps({'source_files':len(sources),'units_total':len(u),'affected_count':len(affected),'recipe_group_count':len(groups),'impact_sha256':hashlib.sha256((out/'impact.json').read_bytes()).hexdigest()},sort_keys=True))
