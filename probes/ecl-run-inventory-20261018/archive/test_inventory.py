"""Entirely synthetic inventory tests; no real target/candidate native inputs."""
import contextlib
import io
from unittest import mock
import importlib.util
import json
from pathlib import Path
import struct
import types
import unittest
ROOT=Path(__file__).parent

def load(name,path):
 s=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(s);s.loader.exec_module(m);return m
p=load('inventory',ROOT/'inventory.py')
old=load('oldtests',Path('/workspace/shared/ecl-vm-reference-review/verifier/test_op82_baseline_order.py'));old.p=p

class Inventory(unittest.TestCase):
 def test_partition(self):
  a=bytes(7020);b=bytearray(a)
  for i in (0,6691,6692,6931,6932,7019):b[i]=1
  r=p.raw_inventory(a,b)
  self.assertEqual([r['raw_'+x+'_difference_count'] for x in ('owner','prefix','table','selector')],[6,2,2,2])
 def test_short_owner(self):
  for n in (0,7019,7021):
   with self.assertRaises(AssertionError):p.raw_inventory(bytes(n),bytes(7020))
 def test_field_classification(self):
  a=bytes(7020);b=bytearray(a);b[4]=1;b[8]=2;b[7019]=3
  r=p.classify(a,b,{4,5,6,7})
  self.assertEqual(r['prefix_difference_inside_recognized_candidate_fields'],1)
  self.assertEqual(r['prefix_difference_outside_recognized_candidate_fields'],1)
  for fields,inside,outside in ((set(),0,2),(set(range(6692)),2,0)):
   r=p.classify(a,b,fields)
   self.assertEqual(r['prefix_difference_inside_recognized_candidate_fields'],inside)
   self.assertEqual(r['prefix_difference_outside_recognized_candidate_fields'],outside)
 def test_bad_field_positions(self):
  for f in ({-1},{6692},{True}):
   with self.assertRaises(AssertionError):p.classify(bytes(7020),bytes(7020),f)
 def test_global_failure_clears_all_measurements(self):
  r=p.blank();r.update({k:1 for k in p.MEASURE_COUNTS});r.update(raw_inventory_valid=True,structural_triage_valid=True,relative_table_comparison_valid=True)
  p.invalidate(r)
  self.assertTrue(all(r[k]==0 for k in p.MEASURE_COUNTS));self.assertFalse(r['raw_inventory_valid']);self.assertFalse(r['structural_triage_valid'])
 def test_schema_bounded(self):
  r=p.blank();self.assertLess(len(json.dumps(r)),p.OUTPUT_LIMIT)
  self.assertTrue(all(type(v) in (bool,int) or (type(v) is str and len(v)==64 and set(v)<=set('0123456789abcdef')) for v in r.values()))
  self.assertFalse(any(x in k for k in r for x in ('address','offset','instruction','register','trace')))
  self.assertTrue(r['assumed_prefix_partition']);self.assertTrue(r['linear_decode_only'])
  self.assertFalse(r['residual_is_semantic_discrepancy_proved']);self.assertFalse(r['target_linkage_exhaustiveness_proved'])

class MainSafety(unittest.TestCase):
 def test_any_late_failure_suppresses_counts_and_native_text(self):
  def fail(root,target,r):
   r.update(p.raw_inventory(bytes(7020),bytes([1])*7020))
   r.update(structural_triage_valid=True,relative_table_comparison_valid=True,
     prefix_difference_outside_recognized_candidate_fields=6692)
   print('forbidden native-derived text')
   raise AssertionError('forbidden native-derived error')
  out=io.StringIO()
  with mock.patch.object(p,'inspect',fail),mock.patch.object(p,'limit_resources',lambda:None),mock.patch.object(p.sys,'argv',['inventory','/synthetic','/synthetic']),contextlib.redirect_stdout(out):
   status=p.main()
  result=json.loads(out.getvalue());self.assertEqual(status,1)
  self.assertNotIn('forbidden',out.getvalue())
  self.assertTrue(all(result[k]==0 for k in p.MEASURE_COUNTS))
  self.assertFalse(result['raw_inventory_valid']);self.assertFalse(result['structural_triage_valid']);self.assertFalse(result['relative_table_comparison_valid'])
 def test_global_guard_must_pin_target_owner(self):
  source=(ROOT/'inventory.py').read_text()
  self.assertIn("assert sha(target_owner)=='b802e20de41d15a9bd59d3b7110f99840e56e5d40f27a2cda159b883d9fe3667'",source)
  self.assertLess(source.index("assert repo_clean(root);r['repository_post_valid']=True"),source.index('    r.update(result)'))
  self.assertIn("li.verify_capstone(lock['capstone']);r['post_pins_valid']=True",source)


class Fields(unittest.TestCase):
 def setUp(self):
  self.origin=4096
  self.records=[(0,5,((1,4),),None),(5,5,((6,4),),(6,4,99999))]
  self.relocs=[4097]
  self.fields=[dict(offset=1,width=4,type='DIR32',instruction_end=None),dict(offset=6,width=4,type='REL32',instruction_end=10)]
 def result(self):return p.recognized_fields(types.SimpleNamespace(base_relocations=lambda:self.relocs),self.origin,self.records,dict(decoded_bytes=6692,fields=self.fields))
 def test_valid(self):self.assertEqual(self.result(),set(range(1,5))|set(range(6,10)))
 def test_missing_or_extra_field(self):
  self.fields.pop()
  with self.assertRaises(AssertionError):self.result()
  self.setUp();self.fields.append(dict(self.fields[0]))
  with self.assertRaises(AssertionError):self.result()
 def test_bad_field_shapes(self):
  for change in ({'offset':-1},{'offset':6691},{'width':0},{'width':5},{'offset':2},{'type':'REL32'},{'instruction_end':4},{'offset':True}):
   self.setUp();self.fields[0].update(change)
   with self.subTest(change=change),self.assertRaises(AssertionError):self.result()
 def test_bad_relocations(self):
  for relocations in ([4095],[4096+6691],[4097,4097],[4098],[4102]):
   self.setUp();self.relocs=relocations
   with self.subTest(case=relocations),self.assertRaises(AssertionError):self.result()
 def test_internal_branch_not_field(self):
  self.records[1]=(5,5,((6,4),),(6,4,self.origin+10));self.fields.pop()
  self.assertEqual(self.result(),set(range(1,5)))
 def test_external_branch_overlap_rejected(self):
  self.records[0]=(0,5,((1,4),),(1,4,99999))
  with self.assertRaises(AssertionError):self.result()
 def test_incomplete_decode_rejected(self):
  with self.assertRaises(AssertionError):p.recognized_fields(types.SimpleNamespace(base_relocations=lambda:[]),0,[],dict(decoded_bytes=6691,fields=[]))

class Decode(unittest.TestCase):
 def ins(self,off,size=1,**kw):
  d=dict(address=4096+off,size=size,imm_offset=0,imm_size=0,disp_offset=0,disp_size=0,operands=[],group=lambda n:False);d.update(kw);return types.SimpleNamespace(**d)
 def run_decode(self,items):return p.decode_prefix(types.SimpleNamespace(disasm=lambda *a:iter(items)),bytes(6692),4096,1,2,3)
 def test_complete(self):self.assertEqual(len(self.run_decode([self.ins(i) for i in range(6692)])),6692)
 def test_failures(self):
  for items in ([],[self.ins(1)],[self.ins(0,0)],[self.ins(0,16)],[self.ins(0,1,imm_offset=1,imm_size=1)], [self.ins(i) for i in range(6693)]):
   with self.assertRaises(AssertionError):self.run_decode(items)
 def test_operand_and_control(self):
  first=self.ins(0,5,imm_offset=1,imm_size=4,group=lambda n:n==1,operands=[types.SimpleNamespace(type=3,imm=99999)])
  r=self.run_decode([first]+[self.ins(i) for i in range(5,6692)])
  self.assertEqual(r[0],(0,5,((1,4),),(1,4,99999)))

class Tables(unittest.TestCase):
 def setUp(self):
  self.a=bytearray(7020);self.b=bytearray(7020);self.oa=4096;self.ob=8192
  for buf,origin in ((self.a,self.oa),(self.b,self.ob)):struct.pack_into('<60I',buf,6692,*[origin+i*10 for i in range(60)])
  self.records=[(i,1,(),None) for i in range(6692)]
 def result(self):return p.compare_tables(self.a,self.b,self.oa,self.ob,self.records,self.records)
 def test_relocated_origins_equal(self):self.assertTrue(self.result()['relative_destinations_all_equal'])
 def test_one_changed(self):
  struct.pack_into('<I',self.a,6692,self.oa+1)
  self.assertEqual(self.result()['relative_destination_difference_count'],1)
 def test_bad_destinations(self):
  for offset in (-1,10,6690,6692,7020):
   self.setUp();struct.pack_into('<I',self.a,6692,self.oa+offset)
   with self.subTest(offset=offset),self.assertRaises(AssertionError):self.result()
 def test_not_instruction_start(self):
  self.records=self.records[1:]
  with self.assertRaises(AssertionError):self.result()
 def test_selector_mismatch(self):
  self.a[-1]=1
  with self.assertRaises(AssertionError):self.result()

if __name__=='__main__':
 suite=unittest.TestSuite([unittest.defaultTestLoader.loadTestsFromTestCase(c) for c in (Inventory,MainSafety,Fields,Decode,Tables,old.Owners,old.Aliases,old.Tables)])
 result=unittest.TextTestRunner(verbosity=2).run(suite);raise SystemExit(not result.wasSuccessful())
