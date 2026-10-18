"""Synthetic COFF structures only; no native artifact reads."""
import copy,importlib.util,json,pathlib,struct,unittest
P=pathlib.Path;sp=importlib.util.spec_from_file_location('collector',P(__file__).with_name('collect.py'));c=importlib.util.module_from_spec(sp);sp.loader.exec_module(c)
class Tests(unittest.TestCase):
 def setUp(self):
  self.data=bytearray(1900);self.sections=[dict(flags=0x20001020,size=1476,raw_offset=100,reloc_offset=1700,reloc_count=3),dict(flags=0x20001020,size=68,raw_offset=1600,reloc_offset=1800,reloc_count=0)]
  self.symbols={0:dict(name=c.SYMBOLS['transform'],section=1,value=0,type=0x20,storage=2),1:dict(name=c.SYMBOLS['helper'],section=2,value=0,type=0x20,storage=3),2:dict(name='external',section=0,value=0,type=0,storage=2)}
  self.rows=[dict(symbol=c.SYMBOLS[k],section_number=i,section_offset=0,size=c.SIZES[k],extent_source='single-function-comdat-section') for i,k in enumerate(('transform','helper'),1)]
  for n,(offset,index,kind) in enumerate([(16,1,0x14),(48,1,0x14),(80,2,6)]):struct.pack_into('<IIH',self.data,1700+n*10,offset,index,kind)
  self.data[115]=self.data[147]=0xE8
 def result(self):return c.analyze(self.data,self.sections,self.symbols,self.rows)
 def test_two_callform_encodings(self):
  out=self.result();self.assertEqual(out['helper_named_rel32_e8_callform_count'],2);self.assertTrue(out['helper_rel32_e8_form_present']);self.assertEqual(out['whole_coff_owners']['transform']['size'],1476)
  self.assertNotIn('offset',json.dumps(out));self.assertNotIn('section_number',json.dumps(out))
 def test_e8_in_data_limit_is_explicit(self):
  # The structures intentionally provide no instruction-boundary oracle.
  # An E8-looking byte in embedded data produces the same form evidence.
  self.data[110:115]=b'DATA:'
  out=self.result();self.assertTrue(out['helper_rel32_e8_form_present']);self.assertFalse(out['instruction_boundary_proved']);self.assertFalse(out['control_flow_proved'])
 def test_named_noncall_not_claimed(self):
  self.data[115]=self.data[147]=0xE9;out=self.result();self.assertEqual(out['helper_named_other_reference_count'],2);self.assertFalse(out['helper_rel32_e8_form_present'])
 def test_nonzero_addend_not_entry(self):
  struct.pack_into('<i',self.data,116,1);self.assertEqual(self.result()['helper_named_rel32_e8_callform_count'],1)
 def test_present_helper_without_references(self):
  struct.pack_into('<I',self.data,1704,2);struct.pack_into('<I',self.data,1714,2);out=self.result();self.assertEqual(out['helper_named_reference_count'],0);self.assertFalse(out['helper_rel32_e8_form_present'])
 def test_section_alias_resolution(self):
  self.symbols[3]=dict(name='.text$helper',section=2,value=0,type=0,storage=3);struct.pack_into('<I',self.data,1704,3);out=self.result();self.assertEqual(out['helper_section_alias_rel32_e8_callform_count'],1);self.assertEqual(out['helper_named_rel32_e8_callform_count'],1)
 def test_bounds_and_identity_mutations(self):
  changes=[lambda:self.rows[0].update(size=1454),lambda:self.sections[0].update(size=1454),lambda:self.symbols[1].update(storage=2),lambda:self.symbols[0].update(value=1),lambda:self.sections[0].update(flags=0x20),lambda:self.symbols.update({3:dict(self.symbols[0],name='alias')}),lambda:struct.pack_into('<I',self.data,1704,99),lambda:struct.pack_into('<I',self.data,1710,16),lambda:struct.pack_into('<I',self.data,1700,1474),lambda:self.sections[1].update(raw_offset=1500),lambda:self.sections[0].update(reloc_count=10001)]
  for i,change in enumerate(changes):
   self.setUp();change()
   with self.subTest(i=i),self.assertRaises(ValueError):self.result()
if __name__=='__main__':unittest.main()
