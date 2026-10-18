"""Synthetic decoder/table tests only. Never opens a target or invokes inspect."""
import importlib.util
from pathlib import Path
import struct
import sys
import types
import unittest

spec=importlib.util.spec_from_file_location('probe',Path(__file__).with_name('op82_metadata.py'))
p=importlib.util.module_from_spec(spec);spec.loader.exec_module(p)
cs=types.ModuleType('capstone')
for i,name in enumerate(('CALL','RET','JUMP','INT','IRET','PRIVILEGE'),1):setattr(cs,'CS_GRP_'+name,i)
x=types.ModuleType('capstone.x86');x.X86_OP_IMM=1;x.X86_OP_MEM=2
sys.modules['capstone']=cs;sys.modules['capstone.x86']=x
BASE=p.CONSTANTS['FUNCTION_ADDRESS']

def imm(n):return types.SimpleNamespace(type=1,imm=n)
class Ins:
 def __init__(self,addr,size=1,group=0,op=None,mnemonic='nop'):
  self.address=addr;self.size=size;self.g=group;self.operands=[] if op is None else [op];self.mnemonic=mnemonic
 def group(self,n):return n==self.g
class Decoder:
 def __init__(self,items):self.items={i.address:i for i in items}
 def disasm(self,raw,addr,count=1):
  if addr in self.items:yield self.items[addr]

class ScanTests(unittest.TestCase):
 def scan(self,items,end=BASE+10,kind='case'):
  r=p.blank();p.scan(Decoder(items),bytes(7020),BASE,end,kind,r);return r
 def test_return_nonvacuous(self):
  r=self.scan([Ins(BASE,group=cs.CS_GRP_RET)]);self.assertTrue(r['case_completed']);self.assertFalse(r['order_resolved'])
 def test_missing_decode(self):
  r=self.scan([]);self.assertTrue(r['case_decode_invalid']);self.assertFalse(r['case_completed'])
 def test_outbound_stops(self):
  r=self.scan([Ins(BASE,group=cs.CS_GRP_JUMP,op=imm(BASE+100),mnemonic='jmp')]);self.assertEqual(r['case_instruction_count'],1);self.assertTrue(r['case_all_outbound_same_destination']);self.assertFalse(r['continuation_valid'])
 def test_conditional_both_edges(self):
  r=self.scan([Ins(BASE,group=cs.CS_GRP_JUMP,op=imm(BASE+100),mnemonic='jne'),Ins(BASE+1,group=cs.CS_GRP_RET)])
  self.assertEqual(r['case_edge_count'],2);self.assertEqual(r['case_return_exit_count'],1)
 def test_loop_rejected(self):
  r=self.scan([Ins(BASE,group=cs.CS_GRP_JUMP,op=imm(BASE),mnemonic='jmp')]);self.assertTrue(r['case_loop_detected']);self.assertFalse(r['case_completed'])
 def test_unknown_call_stops(self):
  r=self.scan([Ins(BASE,group=cs.CS_GRP_CALL,op=imm(123))]);self.assertEqual(r['case_unknown_call_count'],1);self.assertFalse(r['case_completed'])
 def test_known_calls_counts_without_order_proof(self):
  items=[Ins(BASE+i,group=cs.CS_GRP_CALL,op=imm(a)) for i,a in enumerate((0x450070,0x44FE40,0x44BC70))]+[Ins(BASE+3,group=cs.CS_GRP_RET)]
  r=self.scan(items);self.assertTrue(r['case_completed']);self.assertEqual(r['case_read_call_count'],1);self.assertFalse(r['destination_before_read_on_all_paths'])
 def test_indirect_stop(self):
  r=self.scan([Ins(BASE,group=cs.CS_GRP_JUMP)]);self.assertEqual(r['case_indirect_exit_count'],1);self.assertFalse(r['dispatch_association_valid'])
 def test_privileged_rejected(self):
  self.assertTrue(self.scan([Ins(BASE,group=cs.CS_GRP_PRIVILEGE)])['case_decode_invalid'])
 def test_ud2_and_transaction_terminators_rejected(self):
  for name in ('ud2','ud0','ud1','hlt','syscall','sysenter','xbegin','xabort'):
   with self.subTest(name=name):self.assertTrue(self.scan([Ins(BASE,mnemonic=name)])['case_decode_invalid'])
 def test_xbegin_jump_group_rejected_before_branch(self):
  r=self.scan([Ins(BASE,group=cs.CS_GRP_JUMP,op=imm(BASE+100),mnemonic='xbegin')])
  self.assertTrue(r['case_decode_invalid']);self.assertFalse(r['case_completed']);self.assertEqual(r['case_edge_count'],0)
 def test_cross_boundary_rejected(self):
  self.assertTrue(self.scan([Ins(BASE,size=11)])['case_decode_invalid'])
 def test_overlap_rejected(self):
  r=self.scan([Ins(BASE,size=2,group=cs.CS_GRP_JUMP,op=imm(BASE+1),mnemonic='jne'),Ins(BASE+1,group=cs.CS_GRP_RET),Ins(BASE+2,group=cs.CS_GRP_RET)])
  self.assertTrue(r['case_decode_invalid'])
 def test_instruction_budget(self):
  r=self.scan([Ins(BASE+i) for i in range(129)],BASE+129);self.assertTrue(r['case_budget_exhausted']);self.assertEqual(r['case_instruction_count'],128)
 def test_prefix_separate_budget(self):
  r=self.scan([Ins(BASE+i) for i in range(257)],BASE+257,'prefix');self.assertTrue(r['prefix_budget_exhausted']);self.assertEqual(r['prefix_instruction_count'],256)
 def test_edge_budget(self):
  old=p.BUDGETS['case'];p.BUDGETS['case']=(128,1,256)
  try:r=self.scan([Ins(BASE,group=cs.CS_GRP_JUMP,op=imm(BASE+100),mnemonic='jne')]);self.assertTrue(r['case_budget_exhausted'])
  finally:p.BUDGETS['case']=old
 def test_state_budget(self):
  old=p.BUDGETS['case'];p.BUDGETS['case']=(128,256,1)
  try:r=self.scan([Ins(BASE),Ins(BASE+1,group=cs.CS_GRP_RET)]);self.assertTrue(r['case_budget_exhausted'])
  finally:p.BUDGETS['case']=old
 def test_nonvacuous_same_destination(self):
  r=self.scan([Ins(BASE,group=cs.CS_GRP_RET)]);self.assertFalse(r['case_all_outbound_same_destination'])
 def test_fixed_output_types(self):
  r=p.blank();self.assertTrue(all(type(v) in (bool,int,str) for v in r.values()));self.assertTrue(all(len(v)==64 for v in r.values() if type(v)==str))

class TableTests(unittest.TestCase):
 def data(self):
  # Synthetic topology only, not target instructions/bytes.
  buf=bytearray(7020);jt=p.CONSTANTS['JUMP_TABLE_ADDRESS']-BASE;st=p.CONSTANTS['SELECTOR_TABLE_ADDRESS']-BASE
  tab=[BASE+100+i*5 for i in range(59)]+[p.CONSTANTS['DEFAULT_DESTINATION']]
  struct.pack_into('<60I',buf,jt,*tab)
  active=list(range(58))+[82]
  for n in range(88):buf[st+n]=active.index(n) if n in active else 59
  source='\n'.join('ECL_VM_X%d = 0x%02X,\ncase ECL_VM_X%d:'%(n,n,n) for n in active)
  return buf,source,jt,st
 def test_valid_shape(self):
  b,s,_,_=self.data();table,entry,end=p.table_metadata(b,s);self.assertGreater(end,entry);self.assertEqual(len(table),60)
 def test_selector_oob(self):
  b,s,_,st=self.data();b[st]=60
  with self.assertRaises(AssertionError):p.table_metadata(b,s)
 def test_table_destination_outside_code(self):
  b,s,jt,_=self.data();struct.pack_into('<I',b,jt,p.CONSTANTS['JUMP_TABLE_ADDRESS'])
  with self.assertRaises(AssertionError):p.table_metadata(b,s)
 def test_duplicate_target(self):
  b,s,jt,_=self.data();b[jt:jt+4]=b[jt+4:jt+8]
  with self.assertRaises(AssertionError):p.table_metadata(b,s)
 def test_missing_case(self):
  b,s,_,_=self.data()
  with self.assertRaises(AssertionError):p.table_metadata(b,s.replace('case ECL_VM_X82:',''))
 def test_wrong_owner_size(self):
  b,s,_,_=self.data()
  with self.assertRaises(AssertionError):p.table_metadata(b[:-1],s)

class ImportTests(unittest.TestCase):
 def test_failed_import_rolls_back_existing_module(self):
  name='_op82_import_rollback';old=types.ModuleType(name);sys.modules[name]=old
  try:
   with self.assertRaises(RuntimeError):p.load_module(name,Path('synthetic.py'),b'raise RuntimeError()')
   self.assertIs(sys.modules[name],old)
  finally:sys.modules.pop(name,None)
 def test_failed_import_removes_new_registration(self):
  name='_op82_import_absent';self.assertNotIn(name,sys.modules)
  with self.assertRaises(RuntimeError):p.load_module(name,Path('synthetic.py'),b'raise RuntimeError()')
  self.assertNotIn(name,sys.modules)
 def test_actual_pinned_imports_and_cleanup(self):
  root=Path(__file__).resolve().parents[1]/'dependencies'
  names=('linked_image','target_identity','_op82_report_import_test')
  absent=object();old={n:sys.modules.get(n,absent) for n in names}
  try:
   mods=[]
   for name,relative in zip(names,('scripts/linked_image.py','scripts/target_identity.py','scripts/report-ecl-vm-table.py')):
    data=(root/relative).read_bytes();self.assertEqual(p.sha(data),p.PINS[relative])
    mod=p.load_module(name,root/relative,data);self.assertIs(sys.modules[name],mod);mods.append(mod)
   # Instantiates only an authored dataclass, never PEImage or target accessors.
   section=mods[0].PESection(1,'.synthetic',0,0,0,0,0)
   self.assertEqual(section.name,'.synthetic')
   self.assertTrue(callable(mods[1].verify_target))
   self.assertEqual(mods[2].FUNCTION_ADDRESS,p.CONSTANTS['FUNCTION_ADDRESS'])
  finally:
   for name in names:
    if old[name] is absent:sys.modules.pop(name,None)
    else:sys.modules[name]=old[name]
  for name in names:
   self.assertIs(sys.modules.get(name,absent),old[name])

class MappingTests(unittest.TestCase):
 def data(self,sections=1):
  buf=bytearray(12000);buf[:2]=b'MZ';struct.pack_into('<I',buf,0x3c,0x80);buf[0x80:0x84]=b'PE\0\0'
  struct.pack_into('<HH',buf,0x84,0x14c,sections);struct.pack_into('<H',buf,0x94,96)
  struct.pack_into('<H',buf,0x98,0x10b);struct.pack_into('<I',buf,0x98+28,0x400000)
  section=0x98+96;rva=BASE-0x400000
  for i in range(sections):
   off=section+i*40;struct.pack_into('<IIII',buf,off+8,8000,rva,8000,1000);struct.pack_into('<I',buf,off+36,0x20000020)
  return buf,section
 def test_valid_code_mapping(self):
  b,_=self.data();self.assertEqual(len(p.owner_slice(b)),7020)
 def test_wrong_machine(self):
  b,_=self.data();struct.pack_into('<H',b,0x84,0x8664)
  with self.assertRaises(ValueError):p.owner_slice(b)
 def test_wrong_image_base(self):
  b,_=self.data();struct.pack_into('<I',b,0x98+28,0x500000)
  with self.assertRaises(ValueError):p.owner_slice(b)
 def test_non_executable(self):
  b,s=self.data();struct.pack_into('<I',b,s+36,0x20)
  with self.assertRaises(ValueError):p.owner_slice(b)
 def test_non_code(self):
  b,s=self.data();struct.pack_into('<I',b,s+36,0x20000000)
  with self.assertRaises(ValueError):p.owner_slice(b)
 def test_short_virtual_extent(self):
  b,s=self.data();struct.pack_into('<I',b,s+8,7019)
  with self.assertRaises(ValueError):p.owner_slice(b)
 def test_short_raw_extent(self):
  b,s=self.data();struct.pack_into('<I',b,s+16,7019)
  with self.assertRaises(ValueError):p.owner_slice(b)
 def test_ambiguous_sections(self):
  b,_=self.data(sections=2)
  with self.assertRaises(ValueError):p.owner_slice(b)
 def test_file_extent(self):
  b,s=self.data();struct.pack_into('<I',b,s+20,10000)
  with self.assertRaises(ValueError):p.owner_slice(b)
 def test_schema_under_output_ceiling(self):
  import json
  self.assertLess(len(json.dumps(p.blank()).encode())+1,p.OUTPUT_LIMIT)

if __name__=='__main__':unittest.main()
