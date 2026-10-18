"""Synthetic-only localization and inherited inventory/ownership tests."""
import importlib.util
import json
from pathlib import Path
import struct
import unittest
ROOT=Path(__file__).parent

def load(name,path):
 s=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(s);s.loader.exec_module(m);return m
p=load('localization',ROOT/'localization.py');old=load('inventory_tests',ROOT/'test_inventory.py');old.p=p;old.old.p=p

class Inventory(old.Inventory):
 def test_schema_bounded(self):
  r=p.blank();r['opcode_interval_difference_counts']=[{'opcode':i,'difference_count':139} for i in range(59)]
  self.assertLess(len(json.dumps(r,separators=(',',':')))+1,p.OUTPUT_LIMIT)
  self.assertTrue(r['opcode_intervals_lexical_conditional_only']);self.assertFalse(r['opcode_semantic_ownership_proved'])
  self.assertEqual(set(r['opcode_interval_difference_counts'][0]),{'opcode','difference_count'})

class Localization(unittest.TestCase):
 def setUp(self):
  self.a=bytearray(7020);self.b=bytearray(7020)
  self.origin=0x11223300;self.target_origin=0x44556600
  self.offsets=[1000+i*80 for i in range(60)]
  # Reverse source selector correspondence; physical-order export would fail sorting expectations.
  selectors=bytes(list(reversed(range(59)))+[59]*29)
  for buf,origin in ((self.a,self.origin),(self.b,self.target_origin)):
   struct.pack_into('<60I',buf,6692,*[origin+i for i in self.offsets]);buf[-88:]=selectors
  self.fields=set(range(516));self.canonical=[dict(offset=i,width=4) for i in range(0,516,4)]
  self.a[:468]=bytes([1])*468;self.a[600:739]=bytes([1])*139
  self.records=[(i,1,(),None) for i in range(6692)]
 def result(self):
  r=p.raw_inventory(self.a,self.b);r.update(p.classify(self.a,self.b,self.fields))
  return p.localize(self.a,self.b,self.origin,self.target_origin,self.records,self.records,self.fields,self.canonical,r)
 def test_initial_and_reconciliation(self):
  r=self.result();self.assertTrue(r['localization_valid']);self.assertEqual(r['initial_span_outside_candidate_fields_difference_count'],139)
  self.assertEqual(sum(x['difference_count'] for x in r['opcode_interval_difference_counts']),0)
 def test_default_counted_once_includes_final_alignment(self):
  self.a[600:739]=bytes(139);self.a[6553:6692]=bytes([1])*139
  r=self.result();self.assertEqual(r['shared_default_span_outside_candidate_fields_difference_count'],139)
  self.assertEqual(sum(x['difference_count'] for x in r['opcode_interval_difference_counts']),0)
 def test_opcode_sort_not_physical_order(self):
  self.a[600:739]=bytes(139);self.a[1000:1139]=bytes([1])*139
  r=self.result();rows=r['opcode_interval_difference_counts'];self.assertEqual([x['opcode'] for x in rows],list(range(59)))
  self.assertEqual(rows[58]['difference_count'],80);self.assertEqual(rows[57]['difference_count'],59)
  self.assertEqual(sum(x['difference_count'] for x in rows),139)
 def test_empty_initial(self):
  for buf,origin in ((self.a,self.origin),(self.b,self.target_origin)):struct.pack_into('<I',buf,6692,origin)
  r=self.result();self.assertEqual(r['initial_span_outside_candidate_fields_difference_count'],0);self.assertEqual(r['opcode_interval_difference_counts'][58]['difference_count'],139)
 def test_bad_selector_permutation(self):
  for value in (57,59,60):
   self.setUp();self.a[-88]=value;self.b[-88]=value
   with self.assertRaises(AssertionError):self.result()
 def test_misaligned_boundaries(self):
  self.records=[r for r in self.records if r[0]!=1000]
  with self.assertRaises(AssertionError):self.result()
 def test_different_relative_boundaries(self):
  struct.pack_into('<I',self.a,6692,self.origin+1001)
  with self.assertRaises(AssertionError):self.result()
 def test_crossing_field(self):
  self.canonical.append(dict(offset=999,width=4))
  with self.assertRaises(AssertionError):self.result()
 def test_changed_totals_fail(self):
  self.a[800]=1
  with self.assertRaises(AssertionError):self.result()
 def test_global_postfailure_clear(self):
  r=p.blank();r.update(self.result());p.invalidate(r)
  self.assertFalse(r['localization_valid']);self.assertEqual(r['opcode_interval_difference_counts'],[])
  self.assertEqual(r['initial_span_outside_candidate_fields_difference_count'],0)
 def test_head_repinned_baseline_preserved(self):
  self.assertEqual(p.HEAD,'65e3fd3c127c7efe190ed9ac2cd9b82d31bd2725')
  self.assertEqual(p.BASELINE_HEAD,'a4edda20e434cd40ee99ea3b6b68673a80c3e3be')
  s=(ROOT/'localization.py').read_text();self.assertIn("manifest['base_commit']==BASELINE_HEAD",s)
  self.assertEqual(p.PINS,old.load('prior',ROOT/'inventory.py').PINS)
 def test_output_cap_and_no_native_details(self):
  r=p.blank();r.update(self.result());data=json.dumps(r,separators=(',',':'))
  self.assertLess(len(data)+1,8192)
  self.assertTrue(all(set(x)=={'opcode','difference_count'} for x in r['opcode_interval_difference_counts']))
  self.assertEqual(len(r['opcode_interval_difference_counts']),59)

if __name__=='__main__':
 classes=(Inventory,Localization,old.MainSafety,old.Fields,old.Decode,old.Tables,old.old.Owners,old.old.Aliases,old.old.Tables)
 suite=unittest.TestSuite([unittest.defaultTestLoader.loadTestsFromTestCase(c) for c in classes])
 result=unittest.TextTestRunner(verbosity=2).run(suite);raise SystemExit(not result.wasSuccessful())
