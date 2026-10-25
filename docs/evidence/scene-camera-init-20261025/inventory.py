from pathlib import Path
import sys,json,hashlib,struct
sys.path.insert(0,'scripts')
from linked_image import PEImage
from capstone import *
from capstone.x86_const import *
root=Path.cwd();base=Path('.analysis/scene-camera-init-20261025');target=PEImage('resources/th10.exe')
assert target.sha256=='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True
roles={0:'position.x',4:'position.y',8:'position.z',12:'direction.x',16:'direction.y',20:'direction.z',24:'up.x',28:'up.y',32:'up.z',60:'rocking.x',64:'rocking.y',68:'rocking.z',72:'fov',204:'viewport.x',208:'viewport.y',212:'viewport.width',216:'viewport.height',220:'viewport.minZ',224:'viewport.maxZ',228:'flag'}
bases={'gameplay':0x491e94,'background':0x491d7c};symbols={'gameplay':'?g_SceneGameplayCamera@@3UStageCameraStateView@@A','background':'?g_StagePublishedCamera@@3UStageCameraStateView@@A'}
def scan(image,start,size):
 ins=list(d.disasm(image.read_address(start,size),start));assert sum(i.size for i in ins)==size
 rows=[];locals=[]
 for i in ins:
  for op in i.operands:
   if op.type==X86_OP_MEM and op.mem.base==0 and op.mem.index==0:
    assert i.disp_size==4 and i.mnemonic=='mov' and i.operands[0].type==X86_OP_MEM and op.size==4
    rows.append({'offset':i.address-start+i.disp_offset,'type':'DIR32','width':4,'address':op.mem.disp})
  if i.group(CS_GRP_CALL) or i.group(CS_GRP_JUMP):locals.append(i.address-start)
 assert not locals
 return rows
tr=scan(target,0x4216f0,465)
for f in tr:
 matches=[(r,b) for r,b in bases.items() if f['address']-b in roles];assert len(matches)==1
 r,b=matches[0];f.update(owner=r,symbol=symbols[r],target_base=b,addend=f['address']-b,role=roles[f['address']-b])
assert len(tr)==40
result={'scope':'complete diagnostic inventories only; structural rejects, no masking score or exact claim','target_sha256':target.sha256,'target':tr,'fixed_local_targets':[],'models':{}}
for model in ['first','vector']:
 p=Path('build/scene-camera-init-'+model+'-20261025');report=json.loads((p/'comparison.json').read_text());im=PEImage(p/'source.exe');row=report['candidate'];cr=scan(im,int(row['address'],16),row['size']);assert len(cr)==40
 expected=report['candidate_fields']['fields'];assert [(f['offset'],f['address']) for f in cr]==[(f['offset'],f['candidate_target']) for f in expected]
 # Instruction provenance: position/direction/up constructor copies, rocking,
 # distinct viewport constants and final background rocking stores, reviewed
 # against the complete emitted sequence. Never nearest-symbol alias binding.
 for j,f in enumerate(cr):
  owner=('gameplay' if (j<9 or 14<=j<=24) else 'background') if model=='first' else ('gameplay' if (j<12 or 18<=j<=25) else 'background')
  candidate_base=0x403000 if owner=='gameplay' else 0x403004;addend=f['address']-candidate_base;assert addend in roles
  f.update(owner=owner,symbol=symbols[owner],candidate_base=candidate_base,target_base=bases[owner],addend=addend,target=bases[owner]+addend,role=roles[addend])
 assert sorted((f['owner'],f['addend']) for f in cr)==sorted((f['owner'],f['addend']) for f in tr)
 frozen=json.loads((p/'frozen-inputs.json').read_text());assert all(hashlib.sha256(Path(k).read_bytes()).hexdigest()==v for k,v in frozen.items())
 result['models'][model]={'candidate_size':row['size'],'target_size':465,'result':'structural-rejection','fields':cr,'fixed_local_targets':[],'frozen_inputs_unchanged':len(frozen),'image_sha256':im.sha256}
(base/'role-field-bindings.json').write_text(json.dumps(result,indent=2)+'\n');print('all40 source-role fields per owner and both complete models verified; zero local targets')
