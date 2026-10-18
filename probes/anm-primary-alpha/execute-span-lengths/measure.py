#!/usr/bin/env python3
"""Read-only, bounded physical-span size diagnostic. No native content output."""
import argparse,csv,hashlib,importlib.util,json,os,pathlib,resource,struct,sys,tomllib
sys.dont_write_bytecode=True
if not __debug__:
 print('{"state":"failed","error_class":"OptimizedPythonDisabled"}');sys.exit(1)
P=pathlib.Path;PHASE='packet_validation'
RUNNER='a59bfec32f6f52a5ac722f79ef05736b5ae36fd65f4ef7d95703d672f145cb63'
PRODUCER='9738c814281e531774a97d3700ed5bf865ce5cbe7cd87b210a4a3f7092f71f56'
RUN='.analysis/anm-execute-baseline-20261018-a001'
OWNER='aa000c8ba3d351e877404e9830fbf77ba303fcb02c70923e3a8ae3cb2aeee775'
TARGET=0x43ee30;SIZE=9964;TABLE=0x4413a4;CODE_END=0x4413a2;ENTRIES=94
TOOLS={'scripts/report-anm-execute-table.py':'0ef3bf11bf8d608f3b668441f0f3f5690153cabf66d0a721138c62c3bc3a1599','scripts/target_identity.py':'c7fefbe43382a0f45e0d30a90b60d938d9622f62e746caf50a3851c98709d663','scripts/linked_image.py':'58583c6241e0979288b0c4ce0a0e55f1ae2a178762d93743475d05518eff0354'}
OBJECTS={'source.ltcg.obj': 'ddf2b3e46e204180835e243c36b131a0c0e8e7ff058536c8ea397effd421dc5b', 'support-00-RandomMath.ltcg.obj': '3375e8497aca5363e027b57011e34d80bde93ffbaf112bfd6a2406e445cb53eb', 'support-01-AnmVmCreate.ltcg.obj': '8b5aa10a6d7a5dce667310b2d48f1c7392aee52e19e5ba694dce7e4bc4ccb870', 'support-02-AnmVmId.ltcg.obj': '3b927927c71bc11af98930b6e106776aa8a8a58416ac38ddcbfe1af61266b8fb'}
LEDGER='19d7e4eb1aab50f97afba28cd7a5c6ed529a771f18bd9ccf4997aee6a1954350'
ARTIFACTS={'image':'0d354f972c1a554aa98ad0b6ccf28c860d54f757fbbe521594e5213d98c72dff','map':'5405075777d29b689c75652c16cb249997674fe3c2b3b5865db8755b7d3a2ef9','pdb':'cd26cff46b2e14feecf04dbd7baf3e4f14340fad265ec4454e2247fc2b3b9698'}
def require(v,m):
 if not v:raise ValueError(m)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def load(name,path):
 spec=importlib.util.spec_from_file_location(name,path);module=importlib.util.module_from_spec(spec);previous=sys.modules.get(name);sys.modules[name]=module
 try:spec.loader.exec_module(module)
 except BaseException:
  if previous is None:sys.modules.pop(name,None)
  else:sys.modules[name]=previous
  raise
 return module
def gate(m):
 require(m=={'execution_ready':True,'acceptance_authority':'none','baseline_run':RUN,'baseline_runner_sha256':RUNNER,'baseline_manifest_sha256':PRODUCER,'scope':'physical-span-size-only','output_limit_bytes':20480},'manifest scope drift')
def target_boundary(path):
 with path.open() as f:rows=[r for r in csv.DictReader(f) if int(r['address'],0)==TARGET]
 require(len(rows)==1,'target boundary ambiguity');r=rows[0]
 require(int(r['size'])==SIZE and int(r['span_end'],0)==TARGET+SIZE-1 and r['proposed_name']=='AnmRenderManagerView::ExecuteScript','target boundary drift')
def artifact_identity(image,meta,pdb):
 cv=image.codeview_record();require(meta['timestamp']==image.timestamp and meta['image_base']==image.image_base,'map identity drift');require(cv['guid']==pdb['guid'] and cv['age']==pdb['pdb_age'],'PDB identity drift')
class BoundedImage:
 def __init__(self,image,start,size):self.image=image;self.start=start;self.size=size
 def read_address(self,address,size):
  require(type(address)is int and type(size)is int and size>0 and self.start<=address and address+size<=self.start+self.size,'read outside complete owner')
  return self.image.read_address(address,size)
def source_gate(table,source,header):
 enums,cases,problems=table.parse_source(source,header);require(not problems,'source enum coverage')
 implicit=set(range(-1,93))-{enums[n] for n in cases};allowed={enums.get('ANM_OP_NOP'),enums.get('ANM_OP_INTERRUPT_LABEL')}
 require(not (implicit-allowed),'implicit coverage drift');return {v:k for k,v in enums.items()}
def summarize(target_table,candidate_table,candidate_start,candidate_size,candidate_table_start,names):
 require(len(target_table)==len(candidate_table)==ENTRIES,'table count')
 require(TABLE+ENTRIES*4==TARGET+SIZE,'target table extent')
 require(candidate_start<=candidate_table_start and candidate_table_start+ENTRIES*4==candidate_start+candidate_size,'candidate table extent')
 require(all(type(x)is int and TARGET<=x<=CODE_END for x in target_table),'target destinations')
 require(all(type(x)is int and candidate_start<=x<candidate_table_start for x in candidate_table),'candidate destinations')
 def groups(values):
  d={}
  for op,dest in zip(range(-1,93),values):d.setdefault(dest,[]).append(op)
  return sorted(d.items())
 a=groups(target_table);b=groups(candidate_table)
 require(len(a)==len(b) and [ops for _,ops in a]==[ops for _,ops in b],'physical partition/order mismatch')
 rows=[]
 for i,((left,ops),(right,_)) in enumerate(zip(a,b)):
  ls=(a[i+1][0] if i+1<len(a) else TABLE)-left;rs=(b[i+1][0] if i+1<len(b) else candidate_table_start)-right
  require(ls>0 and rs>0,'nonpositive span')
  rows.append({'opcodes':ops,'target_physical_span_size':ls,'baseline_physical_span_size':rs,'signed_size_delta':rs-ls,'equal_size':ls==rs})
 rows.sort(key=lambda row:row['opcodes'])
 return {'entry_count':ENTRIES,'target_distinct_group_count':len(a),'baseline_distinct_group_count':len(b),'physical_group_partition_matches':True,'physical_order_matches':True,'table_extent_contained':True,'table_ends_at_owner_end':True,'target_pre_table_span_size':TABLE-TARGET,'baseline_pre_table_span_size':candidate_table_start-candidate_start,'pre_table_span_size_delta':candidate_table_start-candidate_start-(TABLE-TARGET),'equal_sized_physical_span_count':sum(r['equal_size'] for r in rows),'unequal_sized_physical_span_count':sum(not r['equal_size'] for r in rows),'rows':rows}
def measure(repo,m):
 global PHASE
 gate(m);PHASE='producer_validation';run=repo/RUN
 for p in (repo,run):
  for q in (p,*p.parents):require(not q.is_symlink(),'symlink directory')
 require(sha(run/'runner.py')==RUNNER and sha(run/'manifest.json')==PRODUCER,'frozen producer drift')
 r=load('frozen_baseline_runner',run/'runner.py');producer=json.loads((run/'manifest.json').read_text());tree=run/'candidate';build=tree/r.OUTPUT
 def inputs():
  r.postcheck(repo,tree,producer)
  require(sha(r.safe(repo,'config/functions.csv'))==LEDGER,'target ledger drift')
  for rel,digest in TOOLS.items():require(sha(r.safe(repo,rel))==digest,'tool drift')
 inputs();target_boundary(repo/'config/functions.csv')
 paths={k:build/('source.'+ext) for k,ext in [('image','exe'),('map','map'),('pdb','pdb')]}
 def artifact_gate():
  for k,p in paths.items():require(sha(r.safe(build,p.name))==ARTIFACTS[k],'artifact drift')
  for name,digest in OBJECTS.items():require(sha(r.safe(build,name))==digest,'object drift')
 artifact_gate();PHASE='parser_import'
 sys.path.insert(0,str(repo/'scripts'));li=load('linked_image',repo/'scripts/linked_image.py');ti=load('target_identity',repo/'scripts/target_identity.py');table=load('bounded_anm_table',repo/'scripts/report-anm-execute-table.py')
 li.verify_capstone(tomllib.loads((tree/'config/tools.lock.toml').read_text())['capstone'])
 require((table.FUNCTION_ADDRESS,table.CODE_END_ADDRESS,table.JUMP_TABLE_ADDRESS,table.JUMP_TABLE_ENTRIES,table.FIRST_OPCODE,table.LAST_OPCODE)==(TARGET,CODE_END,TABLE,ENTRIES,-1,92),'table constants drift')
 PHASE='owner_validation';image=li.PEImage(paths['image']);maps=li.map_publics(paths['map'])+li.map_statics(paths['map']);pdb=li.pdb_section_contributions(paths['pdb']);artifact_identity(image,li.map_metadata(paths['map']),pdb)
 owner=r.measure(li,image,maps,pdb,build,r.OWNERS['execute']);require(owner['size']==9948 and owner['sha256']==OWNER,'baseline owner drift')
 row=next(x for x in maps if x['symbol']==r.OWNERS['execute']['symbol']);start=row['address'];bounded=BoundedImage(image,start,9948)
 target_path=r.safe(tree,'resources/th10.exe');observed,problems=ti.verify_target(target_path);require(not problems and observed['sha256']==producer['target_sha256'],'target identity');target_data=target_path.read_bytes();target_owner=ti.pe_bytes_at(target_data,TARGET,SIZE);require(len(target_owner)==SIZE,'target extent')
 require(target_owner[TABLE-TARGET-1]==0x90,'target alignment')
 names=source_gate(table,tree/'src/AnmManager.cpp',tree/'src/AnmManager.hpp')
 PHASE='bounded_table_measurement';target_table=struct.unpack('<94I',target_owner[TABLE-TARGET:]);candidate_table_start=table.candidate_table_address(bounded,start,9948);candidate_table=struct.unpack('<94I',bounded.read_address(candidate_table_start,376))
 summary=summarize(target_table,candidate_table,start,9948,candidate_table_start,names)
 PHASE='postcheck';inputs();artifact_gate()
 return {'state':'completed','acceptance_authority':'none','diagnostic_only':True,'new_exact_credit':0,'cold_link_calls':0,'runtime_executed':False,'normalization_tested':False,'complete_owner_verified':True,'source_coverage_verified':True,'target_identity_verified':True,'decoder_identity_verified':True,'candidate_table_discovery_is_syntactic':True,'candidate_cfg_reachability_proved':False,'candidate_full_decode_proved':False,'postrun_pins_valid':True,'baseline_owner_size':9948,'baseline_owner_sha256':OWNER,'target_owner_size':SIZE,'target_owner_sha256':hashlib.sha256(target_owner).hexdigest(),'target_sha256':producer['target_sha256'],'artifact_sha256':ARTIFACTS,'tool_sha256':TOOLS,'source_cpp_sha256':producer['source_files']['src/AnmManager.cpp'],'source_header_sha256':producer['source_files']['src/AnmManager.hpp'],'measurement':summary}
def main():
 a=argparse.ArgumentParser();a.add_argument('--repo',type=P,required=True);a.add_argument('--expected-script-sha256',required=True);a.add_argument('--expected-manifest-sha256',required=True);v=a.parse_args();script=P(__file__).resolve();manifest=script.with_name('manifest.json')
 require(sha(script)==v.expected_script_sha256 and sha(manifest)==v.expected_manifest_sha256,'packet drift');require(v.repo.is_absolute(),'absolute repo required')
 os.nice(10);cpus=os.sched_getaffinity(0);os.sched_setaffinity(0,{min(cpus)});resource.setrlimit(resource.RLIMIT_CPU,(30,30))
 result=json.dumps(measure(v.repo,json.loads(manifest.read_text())),sort_keys=True,separators=(',',':'));require(len(result.encode())<=20480,'output bound');print(result)
if __name__=='__main__':
 try:main()
 except Exception as e:print(json.dumps({'state':'failed','error_class':type(e).__name__,'phase':PHASE,'acceptance_authority':'none'}));sys.exit(1)
