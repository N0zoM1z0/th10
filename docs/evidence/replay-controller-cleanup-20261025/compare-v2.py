import sys,json,tomllib,importlib.util,hashlib,copy
from pathlib import Path
sys.path.insert(0,'scripts')
from linked_image import *
from capstone import *
from capstone.x86_const import X86_OP_IMM
from ltcg_link import LTCG_PROFILE,LINK_PROFILE,HARNESS_KIND
model=sys.argv[1] if len(sys.argv)>1 else 'first'
p=Path('build/replay-controller-cleanup-'+model+'-20261025');cand='.analysis/gpt-dots/replay-controller-cleanup-20261025/candidate/'
spec=importlib.util.spec_from_file_location('compare','scripts/compare-linked-function.py');c=importlib.util.module_from_spec(spec);spec.loader.exec_module(c)
canonical=tomllib.loads(Path('config/match-units.toml').read_text());im=PEImage(p/'source.exe');tar=c.verified_target();pubs=map_publics(p/'source.map');fs=linked_functions(p/'source.exe',p/'source.map',p/'source.pdb')['functions']
base=dict(artifact_kind='linked-pe',source=cand+'GameManager.cpp',pdb_source=cand+'GameManager.cpp',support_sources=['src/'+n for n in ['Hint.cpp','Chain.cpp','AnmManager.cpp','FileSystem.cpp','GameStatistics.cpp','Lzss.cpp','ReplayManager.cpp']],output_dir=str(p),entry_symbol='?GameManagerCleanup@@YGXPAUGameManager@@@Z',profile=LTCG_PROFILE,harness=HARNESS_KIND,link_profile=LINK_PROFILE)
if model=='replay-eh':base['support_profiles']={'src/ReplayManager.cpp':LTCG_PROFILE+['/GS','/EHs']}
links=[]
def field(offset,kind,symbol,target,addend=0):
 links.append(dict(offset=offset,type=kind,symbol=symbol,target=target,addend=addend))
field(7,'DIR32','__imp__EnterCriticalSection@4',0x4660b0)
field(14,'DIR32','__imp__LeaveCriticalSection@4',0x4660b4)
for o in [27,79]:field(o,'DIR32','?g_Chain@th10@@3PAUChain@1@A',0x491be4)
for o in [34,56,86,108]:field(o,'DIR32','?g_MainSupervisorView@@3UMainSupervisorView@@A',0x491c28,0x64c)
for o in [42,64,94,116]:field(o,'DIR32','?g_MainSupervisorView@@3UMainSupervisorView@@A',0x491c28,0x6f4)
for o in [51,103]:field(o,'REL32','?UnregisterElem@Chain@th10@@SIXPAUChainElem@2@PAU12@@Z',0x449f60)
field(143,'REL32','??1ReplayManager@@QAE@XZ',0x4294a0)
field(149,'REL32','??3@YAXPAX@Z',0x4524a1)
field(178,'DIR32','?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A',0x491c10)
field(303,'DIR32','?g_GameOwner477830@@3PAUGameChainOwnerView@@A',0x477830)
links.sort(key=lambda x:x['offset'])
u=dict(base,symbol='?GameCleanupController422220@@YGXPAUGameChainOwnerView@@@Z',functions=['GameCleanupController422220'],size=311,target_address=0x422220,linkages=links)
m={'target_sha256':canonical['target_sha256'],'units':{'cleanup':u}}
(p/'comparison-manifest-v2.json').write_text(json.dumps(m,indent=2)+'\n')
d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True
def inv(image,address,size,publics):
 raw=image.read_address(address,size);ins=list(d.disasm(raw,address));assert sum(i.size for i in ins)==size
 starts={i.address for i in ins};local=[]
 for i in ins:
  if i.group(CS_GRP_JUMP) and i.operands[0].type==X86_OP_IMM:
   dest=i.operands[0].imm
   if address<=dest<address+size:
    assert dest in starts;local.append(dict(offset=i.address-address,width=i.size,destination=dest-address))
 return dict(size=size,sha256=hashlib.sha256(raw).hexdigest(),fields=linked_code_fields(image,publics,address,size)['fields'],locals=local,instructions=[dict(offset=i.address-address,bytes=i.bytes.hex(),mnemonic=i.mnemonic,operands=i.op_str) for i in ins])
f=next(f for f in fs if f['symbol']==u['symbol'])
inventory={'target':inv(tar,0x422220,311,[]),'candidate':inv(im,int(f['address'],16),f['size'],pubs)}
inventory['target']['explicit_field_bindings']=links
assert len(links)==18 and len(inventory['target']['locals'])==16
# Target stripped relocations do not erase the absolute field obligations above.
for link in links:
 o=link['offset'];raw=tar.read_address(0x422220+o,4);val=int.from_bytes(raw,'little')
 expected=link['target']+link['addend']
 if link['type']=='REL32':val=(0x422220+o+4+int.from_bytes(raw,'little',signed=True))&0xffffffff
 assert val==expected,(link,val)
candidate_bindings=[]
for fieldrow in inventory['candidate']['fields']:
 matches=[]
 for link in links:
  addresses={x['address'] for x in pubs if x['symbol']==link['symbol']}
  assert len(addresses)==1,(link,addresses)
  if next(iter(addresses))+link['addend']==fieldrow['candidate_target'] and link['type']==fieldrow['type']:
   matches.append(link)
 identities={(x['symbol'],x['target'],x['addend'],x['type']) for x in matches}
 assert len(identities)==1,(fieldrow,matches)
 l=copy.deepcopy(matches[0]);l['offset']=fieldrow['offset'];l['candidate_value']=fieldrow['candidate_target'];l['candidate_base']=l['candidate_value']-l['addend']
 candidate_bindings.append(l)
assert len(candidate_bindings)==18 and len({x['offset'] for x in candidate_bindings})==18
inventory['candidate']['explicit_field_bindings']=candidate_bindings
(p/'inventory-v2.json').write_text(json.dumps(inventory,indent=2)+'\n')
try:r=c.compare_unit('cleanup',_unit_manifest=m)
except Exception as e:r=dict(result='rejected',error=str(e))
(p/'comparison-v2.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r,indent=2))
# Same-image existing peers only; original canonical artifacts are never rebuilt here.
guards={}
for name in ['anm-manager-find-vm','anm-manager-mark-vm-for-deletion','replay-manager-destroy']:
 if name not in canonical['units']:continue
 unit=copy.deepcopy(canonical['units'][name]);orig=unit.get('pdb_source',unit['source']);unit.update(base);unit['pdb_source']=orig;unit['linkages']=unit.get('linkages',unit.pop('relocations',[]));unit.pop('object',None)
 m['units'][name]=unit
 try:guards[name]=c.compare_unit(name,_unit_manifest=m)
 except Exception as e:guards[name]=dict(result='rejected',error=str(e))
 print(name,guards[name].get('result'),guards[name].get('error',''))
(p/'guard-manifest-v2.json').write_text(json.dumps(m,indent=2)+'\n')
(p/'guards-v2.json').write_text(json.dumps(guards,indent=2)+'\n')
