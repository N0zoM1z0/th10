"""Authored synthetic CFG mutations, no target data or access."""
import importlib.util
from pathlib import Path
import unittest

root=Path(__file__).parent
def load(name,path):
 spec=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
base=load('metadata_tests',root/'test_op82_metadata.py')
p=load('local_probe',root/'op82_local_order.py');base.p=p
base.x.X86_INS_CALL=77
B=base.BASE
ADDR={'read':0x44FE40,'resolve':0x450070,'wrap':0x44BC70}

def call(pc,name,near=True):
 op=base.imm(ADDR[name]);op.size=4
 ins=base.Ins(pc,size=5,group=base.cs.CS_GRP_CALL,op=op,mnemonic='call')
 ins.id=77 if near else 78;ins.bytes=b'\xe8\0\0\0\0'
 return ins

def run(items,end=B+100):
 r=p.blank();seen,g,exits=p.scan(base.Decoder(items),bytes(7020),B,end,'case',r)
 p.prove_local(B,seen,g,exits,r);return r

def linear(seq):
 items=[call(B+5*i,n) for i,n in enumerate(seq)]
 items.append(base.Ins(B+5*len(seq),group=base.cs.CS_GRP_JUMP,op=base.imm(B+1000),mnemonic='jmp'))
 return items

class OrderTests(unittest.TestCase):
 def test_all_six_orders_computed_not_assumed(self):
  import itertools
  for seq in itertools.permutations(ADDR):
   with self.subTest(seq=seq):
    r=run(linear(seq));self.assertTrue(r['local_order_resolved'])
    self.assertEqual(r['local_destination_before_read'],seq.index('resolve')<seq.index('read'))
    self.assertEqual(r['local_read_before_wrap'],seq.index('read')<seq.index('wrap'))
    self.assertFalse(r['order_resolved']);self.assertFalse(r['dispatch_association_valid']);self.assertFalse(r['continuation_valid'])
 def test_missing_call(self):
  self.assertFalse(run(linear(('resolve','read')))['local_order_resolved'])
 def test_duplicate_call(self):
  self.assertFalse(run(linear(('resolve','read','read','wrap')))['local_order_resolved'])
 def test_far_call_rejected(self):
  items=linear(('resolve','read','wrap'));items[0].id=78
  self.assertFalse(run(items)['local_control_valid'])
 def test_wrong_call_opcode(self):
  items=linear(('resolve','read','wrap'));items[0].bytes=b'\x9a\0\0\0\0'
  self.assertFalse(run(items)['local_order_resolved'])
 def test_wrong_immediate_width(self):
  items=linear(('resolve','read','wrap'));items[0].operands[0].size=2
  self.assertFalse(run(items)['local_order_resolved'])
 def test_vacuous_exit_rejected(self):
  self.assertFalse(run([base.Ins(B,group=base.cs.CS_GRP_JUMP,op=base.imm(B+1000),mnemonic='jmp')])['local_order_resolved'])
 def test_return_not_escape(self):
  items=linear(('resolve','read','wrap'));items[-1]=base.Ins(B+15,group=base.cs.CS_GRP_RET)
  self.assertFalse(run(items)['local_order_resolved'])
 def test_fallthrough_not_direct_escape(self):
  items=linear(('resolve','read','wrap'));items[-1]=base.Ins(B+15)
  self.assertFalse(run(items,end=B+16)['local_order_resolved'])
 def branching(self,right):
  items=[base.Ins(B,group=base.cs.CS_GRP_JUMP,op=base.imm(B+30),mnemonic='jne')]
  for offset,seq in ((1,('resolve','read','wrap')),(30,right)):
   items.extend(call(B+offset+5*i,n) for i,n in enumerate(seq))
   items.append(base.Ins(B+offset+5*len(seq),group=base.cs.CS_GRP_JUMP,op=base.imm(B+1000),mnemonic='jmp'))
  return items
 def test_mixed_orders_unresolved(self):
  r=run(self.branching(('read','resolve','wrap')))
  self.assertFalse(r['local_order_resolved']);self.assertTrue(r['local_one_each_every_exit']);self.assertFalse(r['local_destination_before_read']);self.assertFalse(r['local_read_before_destination'])
 def test_uniform_branches_resolved(self):
  r=run(self.branching(('resolve','read','wrap')));self.assertTrue(r['local_order_resolved']);self.assertEqual(r['local_exit_state_count'],2)
 def test_bypass_one_call_rejected(self):
  self.assertFalse(run(self.branching(('read','wrap')))['local_order_resolved'])
 def test_loop_path_rejected(self):
  items=self.branching(('resolve','read','wrap'));items[-1]=base.Ins(B+45,group=base.cs.CS_GRP_JUMP,op=base.imm(B+30),mnemonic='jmp')
  self.assertFalse(run(items)['local_order_resolved'])
 def test_unknown_call_rejected(self):
  items=linear(('resolve','read','wrap'));items[0].operands[0].imm=123
  self.assertFalse(run(items)['local_order_resolved'])
 def test_decoder_failure_rejected(self):
  items=linear(('resolve','read','wrap'));del items[1]
  self.assertFalse(run(items)['local_order_resolved'])
 def test_local_state_budget_exhaustion(self):
  r=p.blank();items=linear(('resolve','read','wrap'))
  seen,g,exits=p.scan(base.Decoder(items),bytes(7020),B,B+100,'case',r)
  r['local_state_count']=256;p.prove_local(B,seen,g,exits,r)
  self.assertTrue(r['local_state_budget_exhausted']);self.assertFalse(r['local_order_resolved'])

if __name__=='__main__':
 suite=unittest.TestSuite((unittest.defaultTestLoader.loadTestsFromModule(base),unittest.defaultTestLoader.loadTestsFromTestCase(OrderTests)))
 result=unittest.TextTestRunner().run(suite)
 raise SystemExit(not result.wasSuccessful())
