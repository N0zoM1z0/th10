"""Authored synthetic fixtures only; no target instruction bytes."""
import sys,struct,importlib.util,json
sys.dont_write_bytecode=True
from pathlib import Path
import capstone
assert capstone.__version__=='5.0.6'
p=Path(__file__).parent;spec=importlib.util.spec_from_file_location('v',p/'verify_vector_nan.py');v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True;S=v.START
entries={0x426660:{'proposed_name':'EnemyAngleFromPlayer','source_file':'src/Enemy.cpp'}}
def fixture(source=4,result=0x660,push=b'\x53'):
 b=bytearray();labels={};fix=[]
 def emit(h):b.extend(bytes.fromhex(h))
 def jump(op,label):emit(op);fix.append((len(b),label));b.append(0)
 emit('89ce8b865c0400008d04408d9cc664040000')
 emit('838e3c040000108b0389865c060000')
 emit('d943');b.append(source);emit('d85b08dfe0f6c441');jump('74','fallback')
 emit('d943');b.append(source);emit('d85b08dfe0f6c441');jump('75','alternate')
 emit('89f18d86b4030000e8');b.extend(struct.pack('<i',0x426660-(S+len(b)+4)));jump('eb','store')
 labels['alternate']=len(b);emit('d943');b.append(source)
 labels['store']=len(b);b.extend(push);emit('d99e');b.extend(struct.pack('<i',result));emit('c3')
 labels['fallback']=len(b);emit('d986e4030000d99e');b.extend(struct.pack('<i',result));emit('c3')
 for at,label in fix:b[at]=(labels[label]-at-1)&255
 return {q.address:q for q in md.disasm(bytes(b),S)}
r=v.vector_stages(fixture(),(0,set()),md,entries,{})
assert r['concrete_entry_to_nan_result_store_proven'],r
assert r['live_payload_float1_identity_proven'] and r['proven_receiver_vector_value0_move_occurrences']==1
assert any('four-byte pushed cell is disjoint' in x for x in r['premises'])
for source,result,push in ((0,0x660,b'\x53'),(8,0x660,b'\x53'),(4,0x6c8,b'\x53'),(4,0x660,b'\x53\x53')):
 r=v.vector_stages(fixture(source,result,push),(0,set()),md,entries,{})
 assert not r['concrete_entry_to_nan_result_store_proven']
# Focused final entry-field and flag controls.
def run(raw,kind=0x10):
 code=bytes.fromhex(raw+'d901c3');by={q.address:q for q in md.disasm(code,S)};return v.entry_witness(by,md,S+len(code)-3,kind,{})
positive=('83893c04000010','8b816404000089815c060000','83893c040000107403','83893c040000107203','83893c040000107003','83893c040000107803','83893c040000107a03')
for raw in positive:assert run(raw)['reached_earlier_vector_guard_from_owner_entry']
negative=('83895c04000010','83895c06000010','6683893c04000010','838f3c04000010','89815c060000','8b816404000089815d060000','668b81640400006689815c060000','83893c040000107503')
for raw in negative:
 try:run(raw)
 except v.GateError:pass
 else:raise AssertionError(raw)
print(json.dumps({'status':'PASS','full_protected_push_vector_composition':1,'composed_role_offset_preparation_negatives':4,'entry_field_flag_positives':7,'entry_field_flag_negatives':8,'target_bytes_used':False}))
