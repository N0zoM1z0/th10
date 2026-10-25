from pathlib import Path
import sys,json,tomllib,struct,hashlib
sys.path.insert(0,'scripts');from linked_image import PEImage,map_publics,linked_code_fields,linked_functions
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_GRP_CALL,CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM,X86_OP_MEM
assert hashlib.sha256(Path('resources/th10.exe').read_bytes()).hexdigest()=='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
p=Path('.analysis/laser-manager-20261025');b=Path(sys.argv[1]);t=PEImage('resources/th10.exe');c=PEImage(b/'source.exe');pub=map_publics(b/'source.map');fn=linked_functions(b/'source.exe',b/'source.map',b/'source.pdb')['functions'];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
units=tomllib.loads(Path('config/match-units.toml').read_text())['units'];by_symbol={x['symbol']:int(x['address']) for x in pub}
main='?g_MainSupervisorView@@3UMainSupervisorView@@A';glob='?g_EnemyLaserManager@@3PAUEnemyLaserManagerView@@A';base='??0EnemyLaserConstructionBaseView@@QAE@XZ';ini='?Initialize@EnemyLaserManagerStorageView@@QAEHXZ';cleanup='?GameCleanupLinked41C1C0@@YGXPAUGameChainOwnerView@@@Z';create='?GameCreateOwner41C290@@YAPAXXZ'
roles={}
def role(sym,target,addend=0):roles[(sym,addend)]=target+addend
for u in units.values():
 for f in u.get('linkages',[]):
  if f.get('symbol') and 'target' in f:role(f['symbol'],int(f['target']),int(f.get('addend',0)))
for sym,tar in [(glob,0x47781c),(base,0x41c030),(ini,0x41c120),(cleanup,0x41c1c0),(create,0x41c290),('?EnemyLaserUpdateCallback@@YIHPAX@Z',0x41c480),('?EnemyLaserDrawCallback@@YIHPAX@Z',0x41c4e0),('??_7EnemyLaserConstructionBaseView@@6B@',0x46dab0),('?g_AnmGameSpeed@@3MA',0x476f78)]:role(sym,tar)
role(main,0x491c28,0x64c);role(main,0x491c28,0x6f4)
# These literal identities are checked against actual target bytes below.
literal_checks=[]
initfn=next(x for x in fn if x['symbol']==ini)
for tar,offset in [(0x46cd88,7),(0x46cd54,32)]:
 addr=struct.unpack('<I',c.read_address(int(initfn['address'],0)+offset,4))[0]
 candidates=[sym for sym,val in by_symbol.items() if sym.startswith('??_C@') and val==addr]
 assert len(candidates)==1,(hex(tar),candidates)
 def literal(img,a):
  n=0
  while img.read_address(a+n,1)!=b'\0':n+=1
  return img.read_address(a,n+1)
 expected=literal(t,tar);actual=literal(c,addr)
 literal_checks.append({'target':tar,'symbol':candidates[0],'expected_hex':expected.hex(),'candidate_hex':actual.hex(),'equal':expected==actual})
 role(candidates[0],tar)

def inventory(img,addr,size,publics=None):
 code=img.read_address(addr,size);ins=list(md.disasm(code,addr));assert sum(x.size for x in ins)==size
 if publics is not None:r=linked_code_fields(img,publics,addr,size)['fields']
 else:
  r=[]
  for i in ins:
   ctrl=i.group(CS_GRP_CALL) or i.group(CS_GRP_JUMP)
   for o in i.operands:
    if o.type==X86_OP_IMM and ctrl:
     dst=int(o.imm)&0xffffffff
     if not addr<=dst<addr+size:r.append({'offset':i.address-addr+i.imm_offset,'width':i.imm_size,'type':'REL'+str(i.imm_size*8),'candidate_target':dst,'instruction_end':i.address-addr+i.size})
    elif o.type==X86_OP_IMM and 0x400000<=o.imm<0x49c000 and i.imm_size==4:r.append({'offset':i.address-addr+i.imm_offset,'width':4,'type':'DIR32','candidate_target':int(o.imm),'instruction_end':None})
    elif o.type==X86_OP_MEM and not o.mem.base and not o.mem.index and 0x400000<=o.mem.disp<0x49c000 and i.disp_size==4:r.append({'offset':i.address-addr+i.disp_offset,'width':4,'type':'DIR32','candidate_target':int(o.mem.disp),'instruction_end':None})
  r.sort(key=lambda x:x['offset'])
 local=[{'offset':i.address-addr,'size':i.size,'target_offset':int(i.operands[0].imm)-addr} for i in ins if (i.group(CS_GRP_JUMP) or i.group(CS_GRP_CALL)) and i.operands and i.operands[0].type==X86_OP_IMM and addr<=i.operands[0].imm<addr+size]
 return r,local

def compare(name,sym,ta,ts,explicit=None):
 f=next(x for x in fn if x['symbol']==sym);ca=int(f['address'],0);cs=f['size'];tf,tl=inventory(t,ta,ts);cf,cl=inventory(c,ca,cs,pub);raw=bytearray(c.read_address(ca,cs));bindings=[]
 for field in cf:
  dst=field['candidate_target'];matches=[(s,a,v) for (s,a),v in roles.items() if s in by_symbol and by_symbol[s]+a==dst]
  if explicit is not None:
   row=next((x for x in explicit if x['offset']==field['offset'] and x['type']==field['type']),None)
   if row is not None:matches=[(row['symbol'],int(row.get('addend',0)),int(row['target'])+int(row.get('addend',0)))]
  # Overlapping diagnostic anchors are not identities: prefer exact symbols, except explicit canonical base/addend.
  exact=[x for x in matches if x[1]==0 and x[0] in field.get('candidate_symbols',[])];matches=exact or matches
  targets=set(x[2] for x in matches)
  assert len(targets)==1,(name,field,matches)
  dest=next(iter(targets));assert any(x['candidate_target']==dest and x['type']==field['type'] for x in tf),(name,field,dest)
  for s,a,v in matches:assert by_symbol[s]+a==dst
  val=dest if field['type']=='DIR32' else (dest-(ta+field['instruction_end']))&0xffffffff
  assert field['width']==4;struct.pack_into('<I',raw,field['offset'],val)
  bindings.append({**field,'source_roles':[{'symbol':s,'addend':a,'target':v} for s,a,v in matches],'replay_target':dest})
 target=t.read_address(ta,ts);diff=[{'offset':i,'target':a,'candidate':bb} for i,(a,bb) in enumerate(zip(target,raw)) if a!=bb]
 shapes=[(x['offset'],x['type'],x['width']) for x in tf]==[(x['offset'],x['type'],x['width']) for x in cf]
 if ts==cs and shapes and not diff:
  assert [(x['offset'],x['type'],x['candidate_target']) for x in tf]==[(x['offset'],x['type'],x['replay_target']) for x in bindings]
 return {'symbol':sym,'target_address':ta,'target_size':ts,'candidate_size':cs,'fields_shape_equal':shapes,'target_fields':tf,'candidate_fields':bindings,'target_locals':tl,'candidate_locals':cl,'ordinary_differences':diff,'literal_checks':literal_checks if name=='Initialize' else [],'exact':ts==cs and shapes and not diff and tl==cl and (name!='Initialize' or all(x['equal'] for x in literal_checks))}
results={}
for n,s,a,z in [('Initialize',ini,0x41c120,146),('Cleanup',cleanup,0x41c1c0,195),('Create',create,0x41c290,91),('base',base,0x41c030,98)]:results[n]=compare(n,s,a,z)
for name in ['enemy-laser-type0-constructor','enemy-laser-type1-constructor','anm-manager-preload-anm','decomphelp-game-error-log','decomphelp-chain-unregister','chain-add-calc','chain-add-draw']:
 u=units[name];results[name]=compare(name,u['symbol'],u['target_address'],u['size'],u['linkages'])
(p/(b.name+'-comparison.json')).write_text(json.dumps(results,indent=2)+'\n');print(json.dumps({n:{k:r[k] for k in ['target_size','candidate_size','fields_shape_equal','exact','ordinary_differences']} for n,r in results.items()},indent=2))
