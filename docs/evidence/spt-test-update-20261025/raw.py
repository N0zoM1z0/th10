from pathlib import Path
import sys,json,hashlib
sys.path.insert(0,'scripts');from linked_image import PEImage,linked_code_fields
from capstone import *
from capstone.x86_const import *
p=Path('resources/th10.exe');assert hashlib.sha256(p.read_bytes()).hexdigest()=='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
im=PEImage(p);d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True;out={}
for name,a,n in [('update',0x42bfc0,926),('callback',0x42c580,5),('factory',0x450500,89)]:
 b=im.read_address(a,n);ii=list(d.disasm(b,a));assert sum(i.size for i in ii)==n
 fields=[];locals=[]
 for i in ii:
  if (i.group(CS_GRP_JUMP) or i.group(CS_GRP_CALL)) and i.operands[0].type==X86_OP_IMM:
   dest=i.operands[0].imm
   if a<=dest<a+n:locals.append(dict(offset=i.address-a,destination=dest-a,width=i.size))
   else:fields.append(dict(offset=i.address-a+i.imm_offset,type='REL32' if i.imm_size==4 else 'REL8',target=hex(dest)))
  for op in i.operands:
   if op.type==X86_OP_MEM and op.mem.base==0 and op.mem.index==0 and (op.mem.disp>=0x400000 or op.mem.segment==X86_REG_FS):fields.append(dict(offset=i.address-a+i.disp_offset,type='DIR32',target=hex(op.mem.disp)))
   elif op.type==X86_OP_IMM and not(i.group(CS_GRP_CALL) or i.group(CS_GRP_JUMP)) and 0x400000<=op.imm<0x49c000:fields.append(dict(offset=i.address-a+i.imm_offset,type='DIR32',target=hex(op.imm)))
 out[name]=dict(address=hex(a),size=n,sha256=hashlib.sha256(b).hexdigest(),fields=fields,locals=locals,instructions=[dict(offset=i.address-a,bytes=i.bytes.hex(),mnemonic=i.mnemonic,operands=i.op_str) for i in ii],following16=im.read_address(a+n,16).hex())
out['eh_metadata']={hex(a):im.read_address(a,n).hex() for a,n in [(0x4714d0,8),(0x4714d8,28)]}
Path('.analysis/spt-test-update-20261025/target-inventory.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps({k:{z:v[z] for z in ['address','size','fields','locals']} for k,v in out.items() if k!='eh_metadata'},indent=2))
