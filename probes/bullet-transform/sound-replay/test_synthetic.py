"""Authored Capstone fixtures, no target instruction bytes."""
import sys,struct,importlib.util,json,copy
from pathlib import Path
p=Path(__file__).parent;sys.path.insert(0,str(p/'synthetic-deps'))
import capstone
assert capstone.__version__=='5.0.6'
spec=importlib.util.spec_from_file_location('v',p/'verify_sound_contracts.py');v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True;S=v.START
entries={a:{'proposed_name':name,'size':str(n),'span_end':hex(a+n-1),'source_file':'src/EnemyEclDispatcher.cpp'} for a,(name,n) in v.SOUND_TARGETS.items()}
def call(b,target):b.append(0xe8);b.extend(struct.pack('<i',target-(S+len(b)+4)))
def run(b,rows=entries):return v.sound_caller_scope({q.address:q for q in md.disasm(bytes(b),S)},(0,set()),md,rows)
b=bytearray.fromhex('6a008bbed8040000b990254900');call(b,0x43dc90);b.extend(bytes.fromhex('83c4046a00'));call(b,0x43dc90);b.extend(bytes.fromhex('5356'));call(b,0x43dd10);b.extend(b'\xc3')
r=run(b);assert [x['direct_call_count'] for x in r['sound_target_groups']]==[2,1]
assert r['sound_target_groups'][0]['successor_shape_counts_unordered']['add_esp_4']==1
assert r['sound_target_groups'][0]['stopping_categories_unordered']['opaque_call']==1
# No source-implied count assumption: missing sound sites simply have zero counts.
r=run(b'\xc3');assert all(x['direct_call_count']==0 for x in r['sound_target_groups'])
# A diamond merges at the call and stops before inspecting either predecessor path.
b=bytearray.fromhex('750390eb0190');call(b,0x43dc90);b+=b'\xc3';r=run(b)
assert r['sound_target_groups'][0]['stopping_categories_unordered']=={'merge':1}
# A sole jump predecessor is a control boundary, never walked backward through.
b=bytearray.fromhex('eb0090');call(b,0x43dc90);b+=b'\xc3';r=run(b);assert r['sound_target_groups'][0]['stopping_categories_unordered']=={'control':1}
b=bytearray(b'\x90'*9);call(b,0x43dc90);b+=b'\xc3';r=run(b);assert r['sound_target_groups'][0]['inspected_predecessor_instruction_occurrences']==8
negative=0
for field,value in [('size','124'),('span_end','0x43dd0b'),('proposed_name','wrong'),('source_file','wrong')]:
 rows=copy.deepcopy(entries);rows[0x43dc90][field]=value
 try:run(b,rows)
 except v.GateError:negative+=1
 else:raise AssertionError('bad ledger accepted')
try:run(b[:-1])
except v.GateError:negative+=1
else:raise AssertionError('missing call continuation accepted')
print(json.dumps({'status':'PASS','metadata_positive_cases':5,'negative_cases':negative,'target_bytes_used':False}))

def fixture(prep,target=0x43dc90):
 b=bytearray.fromhex('89cdeb00'+prep);b.append(0xe8);b.extend(struct.pack('<i',target-(S+len(b)+4)));b+=b'\xc3';return list(md.disasm(bytes(b),S))
def run(q):return v.sound_caller_roles({i.address:i for i in q},(0,set()),md,entries)
r=run(fixture('6a00b990254900'));row=r['fixed_role_predicates_unordered'][0];assert row['receiver_carrier_is_direct_sound_storage'] and row['pre_call_stack_word_is_integer_zero'] and row['edi_unchanged_from_corridor_entry']
cue='8b5e088b85b403000050be90254900'
r=run(fixture(cue,0x43dd10));row=r['fixed_role_predicates_unordered'][0]
assert row['receiver_carrier_is_direct_sound_storage'] and row['ebx_from_actual_dword_read_with_syntactic_offset8'] and row['stack_word_source_is_bullet_X_without_call_assumptions']
# Fixed value and source predicates must go false rather than silently reclassify arguments.
for prep,key in [('6a01b990254900','pre_call_stack_word_is_integer_zero'),('6a00b994254900','receiver_carrier_is_direct_sound_storage'),('6a00b99025490031ff','edi_unchanged_from_corridor_entry')]:
 r=run(fixture(prep))
 if prep.endswith('31ff'):assert r['modeled_call_count']==0
 else:assert not r['fixed_role_predicates_unordered'][0][key]
for prep,key in [('8b5e0c8b85b403000050be90254900','ebx_from_actual_dword_read_with_syntactic_offset8'),('8b5e088b85b803000050be90254900','pre_call_stack_word_from_actual_dword_read_with_offset3B4'),('8b5e088b86b403000050be90254900','stack_word_source_is_bullet_X_conditional_nonvolatile_preservation')]:
 assert not run(fixture(prep,0x43dd10))['fixed_role_predicates_unordered'][0][key]
negative=0
for prep in ('6a006a00b990254900','666a00b990254900','ff30b990254900','54b990254900','89066a00b990254900'):
 assert run(fixture(prep))['modeled_call_count']==0;negative+=1
# Use actual enum for an extra hidden write.
from capstone.x86 import X86_REG_EDI
q=fixture('6a00b990254900');move=next(i for i in q if i.mnemonic=='mov' and i.operands[1].type==2)
class Clobber:
 def __getattr__(self,k):return getattr(move,k)
 def regs_access(self):return move.regs_access()[0],move.regs_access()[1]+[X86_REG_EDI]
q[q.index(move)]=Clobber();assert run(q)['modeled_call_count']==0;negative+=1
print(json.dumps({'status':'PASS','sample_and_cue_positive':2,'fixed_predicate_controls':6,'unsupported_effect_negatives':negative,'target_bytes_used':False}))

def sample(sound=0x458,cursor=0x45c,sign=0x78,zero=0x74,extra=b''):
 b=bytearray.fromhex('89cd83bd');b.extend(struct.pack('<i',cursor));b.append(0);zi=len(b);b.extend(bytes([zero,0]));b.extend(bytes.fromhex('8bbd'));b.extend(struct.pack('<i',sound));b.extend(extra);b.extend(bytes.fromhex('85ff'));si=len(b);b.extend(bytes([sign,0]));b.extend(bytes.fromhex('6a00b990254900e8'));b.extend(struct.pack('<i',0x43dc90-(S+len(b)+4)));end=len(b);b.append(0xc3);b[zi+1]=end-zi-2;b[si+1]=end-si-2;return bytes(b)
def run(blob):return v.sound_field_contracts({q.address:q for q in md.disasm(blob,S)},(0,set()),md,entries)
r=run(sample());row=r['sample_field_gate_predicates_unordered'][0]
assert row['sound_id_from_live_receiver458_conditional'] and row['signed_sound_nonnegative_gate_proven'] and row['live_cursor_nonzero_gate_proven'],row
for kwargs,key in [({'sound':0x45c},'sound_id_from_live_receiver458_conditional'),({'cursor':0x458},'live_cursor_nonzero_gate_proven'),({'sign':0x79},'signed_sound_nonnegative_gate_proven'),({'zero':0x75},'live_cursor_nonzero_gate_proven'),({'extra':bytes.fromhex('898558040000')},'sound_id_from_live_receiver458_conditional'),({'extra':bytes.fromhex('bf01000000')},'sound_id_from_live_receiver458_conditional')]:
 assert not run(sample(**kwargs))['sample_field_gate_predicates_unordered'][0][key]
# A branch bypass into preparation creates a merge and cannot inherit prior gates.
base=sample();push=next(q.address-S for q in md.disasm(base,S) if q.mnemonic=='push');b=bytearray(bytes.fromhex('7500')+base);b[1]=push
# relocate the synthetic direct CALL after prepending two bytes
for q in md.disasm(base,S):
 if q.mnemonic=='call':off=q.address-S+2;b[off+1:off+5]=struct.pack('<i',0x43dc90-(S+off+5))
r=run(bytes(b));row=r['sample_field_gate_predicates_unordered'][0];assert not row['signed_sound_nonnegative_gate_proven'] and not row['live_cursor_nonzero_gate_proven']
# Actual Cue id-load pointer has the same bounded affine/current-cursor proof.
def cue(offset=8,extra=b''):
 b=bytearray.fromhex('89cd8b855c0400008d04408d9cc564040000');b.extend(extra);b.extend(bytes.fromhex('eb008b5b'));b.append(offset);b.extend(bytes.fromhex('8b85b403000050be90254900e8'));b.extend(struct.pack('<i',0x43dd10-(S+len(b)+4)));b.append(0xc3);return bytes(b)
r=run(cue());row=r['cue_record_predicates_unordered'][0];assert row['record_int0_affine_identity_proven'] and row['live_current_record_int0_identity_conditional'],row
assert not run(cue(12))['cue_record_predicates_unordered'][0]['record_int0_affine_identity_proven']
r=run(cue(extra=bytes.fromhex('89855c040000')));row=r['cue_record_predicates_unordered'][0];assert row['record_int0_affine_identity_proven'] and not row['live_current_record_int0_identity_conditional']
print(json.dumps({'status':'PASS','sample_live_field_signed_cursor_positive':1,'sample_field_gate_or_bypass_negatives':7,'cue_affine_freshness_positive':1,'cue_offset_and_freshness_controls':2,'target_bytes_used':False}))

def decode(b):return {q.address:q for q in md.disasm(b,S)}
def witness(b,kind):
 by=decode(b);goal=max(by);return v.action_entry_witness(by,md,goal,kind,{})
def reject(fn):
 try:fn()
 except v.GateError:return
 raise AssertionError('accepted negative')
# Exact rooted initializer endpoint, including a nonzero cursor witness field read.
for kind in (0x10,0x20):
 b=bytes.fromhex('8b815c04000083f801750a81893c040000')+struct.pack('<I',kind)
 # Simpler uninterrupted endpoint, plus explicit cursor check tested below.
 b=bytes.fromhex('89cd8b855c040000818d3c040000')+struct.pack('<I',kind)
 assert witness(b,kind)['owner_entry_endpoint_reached']
reject(lambda:witness(bytes.fromhex('818b3c04000010000000'),0x10)) # unknown receiver
reject(lambda:witness(bytes.fromhex('81893d04000010000000'),0x10)) # partial alias
reject(lambda:witness(bytes.fromhex('6681893c0400001000'),0x10)) # width/prefix
reject(lambda:witness(bytes.fromhex('81893c04000020000000'),0x10)) # wrong kind
reject(lambda:witness(bytes.fromhex('81893c04000010000000'),0x40)) # unsupported witness
# Concrete Cue input snapshots and endpoint; no callee execution.
prefix=bytes.fromhex('8b996c0400008b81b403000050be90254900')
b=prefix+b'\xe8'+struct.pack('<i',0x43dd10-(S+len(prefix)+5))
assert witness(b,0x20000)['owner_entry_endpoint_reached']
reject(lambda:witness(prefix+b'\xe8'+struct.pack('<i',0x43dc90-(S+len(prefix)+5)),0x20000))
reject(lambda:witness(bytes.fromhex('83c301')+b,0x20000)) # this actually carries unknown only, no consumption
# Unknown flags, opaque call, stale entry loop, unknown input each stop closed.
for prefix in (bytes.fromhex('7500'), bytes.fromhex('e800000000'),bytes.fromhex('ebfe'),bytes.fromhex('8b8110040000')):
 reject(lambda prefix=prefix:witness(prefix+b,0x20000))
# Topological cut accepts converging paths and opaque-call nodes, never bypass/cycle.
pred={S:set(),S+1:{S},S+2:{S+1},S+3:{S+1},S+4:{S+2,S+3}}
assert v.mandatory_initializer(pred,S+4,S+1)==3
bad={k:set(x) for k,x in pred.items()};bad[S+3].add(S);reject(lambda:v.mandatory_initializer(bad,S+4,S+1))
bad={k:set(x) for k,x in pred.items()};bad[S+2].add(S+4);reject(lambda:v.mandatory_initializer(bad,S+4,S+1))
reject(lambda:v.mandatory_initializer({S+4:set()},S+4,S+1))
# A complete wrapper has independent Cue success and explicit unresolved Sample rows.
by=decode(b+b'\xc3');out=v.sound_action_layer(by,(0,set()),md,entries,{})
assert out['cue_entry_witnesses'][0]['owner_entry_endpoint_reached']
assert all(not r['initializer_entry_reached'] for r in out['sample_action_regions'])
assert len(json.dumps(out))<6500,len(json.dumps(out))
print(json.dumps({'status':'PASS','action_entry_positives':3,'entry_negatives':11,'topological_positive':1,'topological_negatives':3,'full_wrapper_positive':1,'target_bytes_used':False}))
# Complete Sample composition: the exact initializer dominates the call while
# the field/gate proof remains independently evaluated after initialization.
for kind in (0x10,0x20):
 raw=bytearray(bytes.fromhex('81893c040000')+struct.pack('<I',kind)+sample())
 for q in md.disasm(raw,S):
  if q.mnemonic=='call':
   off=q.address-S;raw[off+1:off+5]=struct.pack('<i',0x43dc90-(q.address+5))
 out=v.sound_action_layer(decode(bytes(raw)),(0,set()),md,entries,{})
 row=next(r for r in out['sample_action_regions'] if r['kind']==hex(kind))
 assert row['initializer_entry_reached'] and row['sample_call_sites_requiring_initializer_count']==1,row
 assert out['prior_sample_fields'][0]['signed_sound_nonnegative_gate_proven']
 assert len(json.dumps(out))<6500,len(json.dumps(out))
print(json.dumps({'status':'PASS','full_sample_compositions':2}))

assert v.assignment_coverage([{1},{2}],{1,2})['proven_kind_site_sets_disjoint']
assert v.assignment_coverage([{1},{2}],{1,2})['proven_kind_site_union_covers_all_sample_sites']
r=v.assignment_coverage([{1},{1}],{1,2});assert not r['proven_kind_site_sets_disjoint'] and not r['proven_kind_site_union_covers_all_sample_sites']
r=v.assignment_coverage([{1},set()],{1,2});assert not r['each_kind_has_exactly_one_proven_site'] and not r['proven_kind_site_union_covers_all_sample_sites']
reject(lambda:v.assignment_coverage([{1},{3}],{1,2}))
print(json.dumps({'status':'PASS','coverage_positive':1,'coverage_controls':3}))
