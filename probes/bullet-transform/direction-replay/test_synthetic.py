import sys,importlib.util,json
sys.dont_write_bytecode=True
from pathlib import Path
p=Path(__file__).parent;# Use Capstone 5.0.6 from the explicit test environment.
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
spec=importlib.util.spec_from_file_location('v',p/'verify_direction_nan.py');v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
import capstone
assert capstone.__version__=='5.0.6'
md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;S=v.START
# Authored entry: save opaque register; receiver copy; read index; establish record;
# reject active state; accept one of the three fixed kinds; enter FLD guard.
def fixture(kindchecks=True):
 # simpler exact LEA record = receiver + index*8 + 0x464 after index *=3
 code=bytearray(bytes.fromhex('558bec538bf18b865c0400008d04408d9cc6640400008b863c04000085c0'))
 branches=[]
 def j(op):branches.append((len(code),op));code.extend(bytes([op,0]))
 j(0x75) # JNE failure
 code.extend(bytes.fromhex('8b431083f840'));j(0x74)
 code.extend(bytes.fromhex('3d80000000'));j(0x74)
 code.extend(bytes.fromhex('3d00010000'));j(0x75)
 goal=len(code);code.extend(bytes.fromhex('d903c3'));fail=len(code);code.extend(b'\xc3')
 for at,op in branches:code[at+1]=(goal if op==0x74 else fail)-(at+2)
 return bytes(code),S+goal

def run(code,goal,kind=0x40,tables={}):
 by={q.address:q for q in md.disasm(code,S)};return v.entry_witness(by,md,goal,kind,tables)
code,goal=fixture()
for kind in (0x40,0x80,0x100):assert run(code,goal,kind)['reached_earlier_direction_guard_from_owner_entry']
negative=0
def reject(code,goal,kind=0x40,tables={}):
 global negative
 try:run(code,goal,kind,tables)
 except v.GateError:negative+=1;return
 raise AssertionError('accepted negative')
reject(code,goal,0x20) # wrong kind
bad=bytearray(code);bad[code.index(bytes.fromhex('85c0'))+2]=0x74;reject(bytes(bad),goal) # reversed zero branch
reject(bytes.fromhex('85c07502d903c3'),S+4) # stale unknown initial register
reject(bytes.fromhex('7502d903c3'),S+2) # unknown entry flags
reject(bytes.fromhex('890424d903'),S+3) # overwrite opaque return cell
reject(bytes.fromhex('89815c040000d903'),S+6) # cursor alias store
reject(bytes.fromhex('89c88b0001c0d903'),S+6) # unknown object read
reject(bytes.fromhex('ebfec3d903'),S+3) # repeated PC
reject(code,goal-1) # wrong guard boundary
# exact table byte read, selected pointer load and indirect jump
prefix=bytes.fromhex('b8000000000fb68054734000ff248540734000');g=S+len(prefix)
tables={0x407354:0};tables.update({0x407340+j:(g>>(8*j))&255 for j in range(4)})
assert run(prefix+bytes.fromhex('d901'),g,tables=tables)['reached_earlier_direction_guard_from_owner_entry']
bad=bytes.fromhex('b880000000')+prefix[5:];reject(bad+bytes.fromhex('d901'),g,tables=tables)
# DEC preserves CF; arithmetic ZF and signed flags drive exactly selected edges.
for prefix in ('31c04083f8017503','b80000008083e8017d03','31c0487203'):
 raw=bytes.fromhex(prefix);assert run(raw+bytes.fromhex('d90190c3'),S+len(raw))['reached_earlier_direction_guard_from_owner_entry']
# Unknown CF stays unknown across DEC, even though its other flags are known.
reject(bytes.fromhex('b801000000487202d901c3'),S+8)
reject(bytes.fromhex('b8400000005083ec02518b442402d901'),S+14) # unaligned PUSH overlaps saved dword
reject(bytes.fromhex('5083ec025bd901'),S+5) # unaligned POP
print(json.dumps({'status':'PASS','three_fixed_kind_witnesses':3,'table_positive':1,'flag_positive':3,'negative_cases':negative,'target_bytes_used':False}))

def run(raw):
 code=bytes.fromhex(raw+'d901');by={q.address:q for q in md.disasm(code,S)};return v.entry_witness(by,md,S+len(code)-2,0x40,{})
for raw in ('83e4f8','5083e4f8','b8400000005083e4f88b44240483f8407503','83e4f87203','83e4f87003'):
 assert run(raw)['reached_earlier_direction_guard_from_owner_entry']
negative=0
for raw in ('83e4f0','83e4fc','89e083e0f8','83e1f8','83e4f87403','83e4f87a03','83e4f87803','b8400000005083ec02518b442402','83e4f8890424','81ec001000005083e4f8'):
 try:run(raw)
 except v.GateError:negative+=1
 else:raise AssertionError(raw)
print(json.dumps({'status':'PASS','alignment_positive_cases':5,'negative_cases':negative,'target_bytes_used':False}))
