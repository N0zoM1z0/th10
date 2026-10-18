#!/usr/bin/env python3
"""Read-only full-owner comparison. Never build, normalize or emit native bytes."""
import argparse,csv,hashlib,importlib.util,json,os,pathlib,resource,sys,tomllib
sys.dont_write_bytecode=True
if not __debug__:
 print('{"state":"failed","error_class":"OptimizedPythonDisabled"}');sys.exit(1)
P=pathlib.Path
PHASE='packet_validation'
RUNNER='440f0071ab7324d9a788b2c31fafa7162e951f8377ca72801ea794c8e92d1ac7'
MANIFEST='550addeeb5dc774e923a8af9f86e967f66603e57ef8f7330b1f98341f063bd7f'
def require(v,m):
 if not v:raise ValueError(m)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def load(name,path):
 spec=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(spec)
 previous=sys.modules.get(name);sys.modules[name]=m
 try:spec.loader.exec_module(m)
 except BaseException:
  if previous is None:sys.modules.pop(name,None)
  else:sys.modules[name]=previous
  raise
 return m
def gate(m):
 require(m['execution_ready'] is True,'not ready')
 require(m['runner_sha256']==RUNNER and m['manifest_sha256']==MANIFEST,'producer drift')
 require(m['target_address']==0x442300 and m['size']==130,'extent drift')
 require(m['candidate_run']=='.analysis/anm-alpha-int-candidate-20261018-a001','run drift')
 require(m['acceptance_authority']=='none','authority drift')
def inventory(report):
 require(report['normalization_complete'] is True and report['decoded_bytes']==130,'incomplete decode')
 counts={'DIR32':0,'REL8':0,'REL16':0,'REL32':0};occupied=set()
 for f in report['fields']:
  kind=f['type'];require(kind in counts,'unsupported field');width={'DIR32':4,'REL8':1,'REL16':2,'REL32':4}[kind]
  require(type(f['offset']) is int and type(f['width']) is int and f['width']==width,'bad field')
  extent=set(range(f['offset'],f['offset']+width));require(min(extent)>=0 and max(extent)<130 and not occupied&extent,'invalid field extent');occupied|=extent;counts[kind]+=1
 return {'decoded_bytes':130,'decode_complete':True,'field_count':sum(counts.values()),'field_counts':counts,'field_bytes':len(occupied)}
def raw_summary(left,right):
 require(len(left)==len(right)==130,'whole owner required')
 matched=sum(a==b for a,b in zip(left,right))
 return {'size':130,'compared_size':130,'raw_equal':left==right,'raw_matched_bytes':matched,'raw_mismatched_bytes':130-matched,'candidate_sha256':hashlib.sha256(left).hexdigest(),'target_sha256':hashlib.sha256(right).hexdigest()}
def target_boundary(path):
 with path.open() as stream:rows=list(csv.DictReader(stream))
 matches=[x for x in rows if int(x['address'],0)==0x442300]
 require(len(matches)==1,'ambiguous target owner');v=matches[0]
 require(int(v['size'])==130 and int(v['span_end'],0)==0x442381 and v['proposed_name']=='AnmVmView::StartPrimaryAlphaInterpolation','target boundary drift')
def artifact_identity(image,metadata,pdb):
 codeview=image.codeview_record()
 require(metadata['timestamp']==image.timestamp,'map timestamp drift')
 require(metadata['image_base']==image.image_base,'map image base drift')
 require(codeview['guid']==pdb['guid'] and codeview['age']==pdb['pdb_age'],'PDB CodeView identity drift')
def compare(repo,m):
 global PHASE
 PHASE='producer_validation'
 gate(m);run=repo/m['candidate_run']
 for p in (repo,run):
  for q in (p,*p.parents):require(not q.is_symlink(),'symlink directory')
 require(sha(run/'runner.py')==RUNNER and sha(run/'manifest.json')==MANIFEST,'frozen producer drift')
 r=load('frozen_candidate_runner',run/'runner.py');producer=json.loads((run/'manifest.json').read_text());tree=run/'candidate';build=tree/r.OUTPUT
 r.postcheck(repo,tree,producer)
 require(sha(r.safe(repo,'config/functions.csv'))==m['functions_csv_sha256'],'target inventory drift');target_boundary(repo/'config/functions.csv')
 paths={k:build/('source.'+ext) for k,ext in [('image','exe'),('map','map'),('pdb','pdb')]}
 def artifact_gate():
  for k,path in paths.items():require(sha(r.safe(build,path.name))==m['artifact_sha256'][k],'artifact drift')
 artifact_gate()
 PHASE='parser_import'
 sys.path.insert(0,str(tree/'scripts'));li=load('linked_image',tree/'scripts/linked_image.py');sys.modules['linked_image']=li
 c=load('canonical_comparator',tree/'scripts/compare-linked-function.py');li.verify_capstone(tomllib.loads((tree/'config/tools.lock.toml').read_text())['capstone'])
 PHASE='owner_validation'
 image=li.PEImage(paths['image']);publics=li.map_publics(paths['map']);maps=publics+li.map_statics(paths['map']);pdb=li.pdb_section_contributions(paths['pdb']);spec=r.OWNERS['primary_alpha']
 artifact_identity(image,li.map_metadata(paths['map']),pdb)
 owner=r.measure(li,image,maps,pdb,build,spec);require(owner['size']==130 and owner['sha256']==m['helper_sha256'],'complete owner drift')
 row=next(x for x in maps if x['symbol']==spec['symbol']);address=row['address'];target=c.verified_target()
 PHASE='full130_comparison'
 result=raw_summary(image.read_address(address,130),target.read_address(0x442300,130))
 result['candidate_inventory']=inventory(li.linked_code_fields(image,publics,address,130));result['target_inventory']=inventory(li.linked_code_fields(target,[],0x442300,130))
 PHASE='postcheck'
 artifact_gate();r.postcheck(repo,tree,producer);require(sha(repo/'config/functions.csv')==m['functions_csv_sha256'],'target inventory postcheck drift')
 result.update(state='completed',normalization_tested=False,acceptance_authority='none',runtime_executed=False,cold_link_calls=0,production_clean=True,complete_owner_verified=True,artifact_sha256=m['artifact_sha256'])
 return result
def main():
 a=argparse.ArgumentParser();a.add_argument('--repo',type=P,required=True);a.add_argument('--expected-script-sha256',required=True);a.add_argument('--expected-manifest-sha256',required=True);v=a.parse_args();script=P(__file__).resolve();manifest=script.with_name('manifest.json')
 require(sha(script)==v.expected_script_sha256 and sha(manifest)==v.expected_manifest_sha256,'packet drift');require(v.repo.is_absolute(),'absolute repo required')
 os.nice(10);cpus=os.sched_getaffinity(0);os.sched_setaffinity(0,{min(cpus)});resource.setrlimit(resource.RLIMIT_CPU,(30,30))
 print(json.dumps(compare(v.repo,json.loads(manifest.read_text())),sort_keys=True))
if __name__=='__main__':
 try:main()
 except Exception as e:print(json.dumps({'state':'failed','error_class':type(e).__name__,'phase':PHASE,'normalization_tested':False,'acceptance_authority':'none'}));sys.exit(1)
