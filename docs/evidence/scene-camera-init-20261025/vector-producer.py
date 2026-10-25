from pathlib import Path
import sys,json,hashlib,os,struct
root=Path.cwd();sys.path.insert(0,str(root/'scripts'))
import ltcg_link as l
from linked_image import PEImage,linked_functions,linked_code_fields,map_publics
from capstone import *
assert os.getpriority(os.PRIO_PROCESS,0)>=10 and len(os.sched_getaffinity(0))==1
out=root/'build/scene-camera-init-vector-20261025';assert not out.exists()
source=root/'.analysis/scene-camera-init-20261025/vector/SceneCamera.cpp'
assert l.sha256(root/'resources/th10.exe')=='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
inputs=[source,source.parent/"StageScript.hpp",*root.glob('src/*'),*root.glob('docs/evidence/d3d-adapter-format/*'),root/'scripts/ltcg_link.py',root/'scripts/linked_image.py',root/'config/tools.lock.toml']
frozen={str(p.relative_to(root)):l.sha256(p) for p in inputs if p.is_file()}
out.mkdir();(out/'frozen-inputs.json').write_text(json.dumps(frozen,indent=2)+'\n')
linker,env=l.tool_environment();r=l.cold_link(source,out,'?MainInitializeSceneCameras@@YAXXZ',linker,env,profile=[*l.LTCG_PROFILE,'/GS'])
(out/'native.json').write_text(json.dumps(r,indent=2,default=str)+'\n')
f=linked_functions(out/'source.exe',out/'source.map',out/'source.pdb');(out/'functions.json').write_text(json.dumps(f,indent=2)+'\n')
rows=[x for x in f['functions'] if x['symbol']=='?MainInitializeSceneCameras@@YAXXZ'];assert len(rows)==1
row=rows[0];im=PEImage(out/'source.exe');target=PEImage(root/'resources/th10.exe');pubs=map_publics(out/'source.map');address=int(row['address'],16);size=row['size']
d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True
result={'scope':'complete-owner diagnostic; no acceptance authority','target_start':0x4216f0,'target_size':465,'candidate':row}
for name,image,start,n,ps in [('target',target,0x4216f0,465,[]),('candidate',im,address,size,pubs)]:
 ins=list(d.disasm(image.read_address(start,n),start));assert sum(i.size for i in ins)==n
 (out/(name+'.asm')).write_text('\n'.join('%08x %s %s %s'%(i.address,i.bytes.hex(),i.mnemonic,i.op_str) for i in ins)+'\n')
 result[name+'_fields']=linked_code_fields(image,ps,start,n)
 result[name+'_locals']=[{'offset':i.address-start,'destination':i.operands[0].imm-start} for i in ins if (i.group(CS_GRP_JUMP) or i.group(CS_GRP_CALL)) and i.operands[0].type==2 and start<=i.operands[0].imm<start+n]
result['verdict']='structural-rejection' if size!=465 else 'full-binding-review-required'
assert all(l.sha256(root/p)==h for p,h in frozen.items())
(out/'comparison.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'size':size,'target':465,'verdict':result['verdict'],'fields':len(result['candidate_fields']['fields'])}))
