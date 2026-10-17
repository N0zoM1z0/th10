"""Synthetic abstract-executor tests; no PE bytes or native compilation."""
import sys,types,copy
cs=types.ModuleType('capstone');cs.CS_GRP_JUMP=1;cs.CS_GRP_CALL=2;cs.CS_GRP_RET=3
x=types.ModuleType('capstone.x86');x.X86_OP_IMM=1;x.X86_OP_MEM=2;x.X86_OP_REG=3
sys.modules['capstone']=cs;sys.modules['capstone.x86']=x
N=types.SimpleNamespace

def R(n):return N(type=3,reg=n,size=4,access=0)
def C(v):return N(type=1,imm=v,size=4,access=0)
def M(b,d=0,size=4):return N(type=2,mem=N(base=b,index=0,scale=1,disp=d,segment=0),size=size,access=0)
class I:
 def __init__(self,a,m,*ops):self.address=a;self.mnemonic=m;self.operands=list(ops);self.size=1
 def reg_name(self,n):return n
 def group(self,g):return g==1 and self.mnemonic.startswith('j') or g==2 and self.mnemonic=='call' or g==3 and self.mnemonic=='ret'
 def regs_access(self):
  writes=['eflags'] if self.mnemonic in ('test','cmp','add','sub','xor','or') else []
  if self.mnemonic in ('mov','movzx','movsx','lea','pop','add','sub','xor','or') and self.operands and self.operands[0].type==3:writes.append(self.operands[0].reg)
  return [],writes

run=[I(1,'mov',R('esi'),R('eax')),I(2,'mov',R('eax'),M('esi',4)),I(3,'test',R('eax'),R('eax')),I(4,'je',C(90)),I(5,'jmp',M(0,500)),
 I(10,'push',C(0)),I(11,'push',R('esi')),I(12,'mov',R('eax'),R('esi')),I(13,'call',C(1000)),I(14,'test',R('eax'),R('eax')),I(15,'jne',C(90)),I(16,'jmp',C(2)),
 I(20,'lea',R('eax'),M('esi',8)),I(21,'call',C(2000)),I(22,'cmp',M('esi',0x1008),C(0)),I(23,'je',C(80)),
 I(24,'lea',R('eax'),M('esi',8)),I(25,'lea',R('edx'),M('esi',4)),I(26,'push',R('edx')),I(27,'mov',R('ecx'),C(4)),I(28,'xor',R('ebx'),R('ebx')),I(29,'call',C(3000)),
 I(30,'lea',R('eax'),M('esi',8)),I(31,'push',R('esi')),I(32,'mov',R('ecx'),C(4)),I(33,'call',C(3000)),I(34,'cmp',M('esi',4),C(0)),I(35,'je',C(90)),I(36,'jmp',C(60)),
 I(40,'mov',R('ecx'),M('esi',0x1014)),I(41,'mov',R('eax'),M('ecx')),I(42,'call',M('eax')),I(43,'cmp',R('eax'),C(-1)),I(44,'je',C(95)),I(45,'jmp',C(60)),
 I(60,'mov',R('eax'),M('esi',4)),I(61,'movzx',R('ecx'),M('eax',6,2)),I(62,'add',R('eax'),R('ecx')),I(63,'mov',M('esi',4),R('eax')),I(64,'jmp',C(2)),
 I(80,'mov',M('esi',4),C(0)),I(81,'jmp',C(90)),I(90,'or',R('eax'),C(-1)),I(91,'ret'),I(95,'xor',R('eax'),R('eax')),I(96,'ret')]
dec={'Run':run,'StartSubroutine':[I(1000,'ret')],'LeaveFrame':[I(2000,'ret')],'Pop':[I(3000,'ret')]}
jumps=[60]*60;jumps[10]=20;jumps[11]=10;jumps[59]=40
sel=list(range(60))+[59]*28
