"""Synthetic baseline ownership/alias/table fixtures; no actual native inputs."""
import importlib.util
from pathlib import Path
import struct
import types
import unittest
ROOT=Path(__file__).parent
def load(name,path):
 s=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(s);s.loader.exec_module(m);return m
previous=load('order_tests',ROOT/'test_op82_local_order.py')
p=load('baseline_order',ROOT/'op82_baseline_order.py')
previous.p=p;previous.base.p=p
oldowner=load('baseline_owner_tests',Path('/workspace/shared/ecl-run-baseline-plan-v2/test_owner.py'))

class Owners(oldowner.Owners):
 def setUp(self):
  super().setUp();self.section.raw_offset=0;self.section.raw_size=16384
 def result(self):
  def path(x):
   x=str(x)
   if not x.startswith('/'):raise ValueError()
   return x.lower()
  image=types.SimpleNamespace(image_base=4096,data=bytes(16384),section=lambda n:self.section,read_section=lambda *a:self.read(*a))
  l=types.SimpleNamespace(_object_path=path,_object_basename=lambda x:str(x).split('/')[-1].lower(),IMAGE_SCN_CNT_CODE=1,IMAGE_SCN_MEM_EXECUTE=2)
  address,data=p.strict_owner(l,image,self.maps,dict(modules=self.modules,contributions=self.cs),Path('/fake/build'),self.spec['symbol'])
  return {'size':len(data),'sha256':p.sha(data),'complete':True,'size_equal':len(data)==7020}
 def test_helper_metadata_no_body_read(self):
  def forbidden(*args):raise RuntimeError('helper bytes requested')
  image=types.SimpleNamespace(image_base=4096,data=bytes(16384),section=lambda n:self.section,read_section=forbidden)
  l=types.SimpleNamespace(_object_path=lambda x:str(x).lower(),_object_basename=lambda x:str(x).split('/')[-1].lower(),IMAGE_SCN_CNT_CODE=1,IMAGE_SCN_MEM_EXECUTE=2)
  address,size=p.strict_owner(l,image,self.maps,dict(modules=self.modules,contributions=self.cs),Path('/fake/build'),self.spec['symbol'],False)
  self.assertEqual((address,size),(4096,7020))
  self.section.raw_size=7019
  with self.assertRaises(AssertionError):p.strict_owner(l,image,self.maps,dict(modules=self.modules,contributions=self.cs),Path('/fake/build'),self.spec['symbol'],False)
 def test_raw_file_extent_rejected(self):
  self.section.raw_offset=16380
  with self.assertRaises(AssertionError):self.result()

class Aliases(unittest.TestCase):
 def grammar(self):
  names=sorted(['?EnemyWrapAngle@@YGMM@Z']+['symbol%03d'%i for i in range(73)])
  rows=[(name,'_th10_ltcg_probe_anchor_%04d'%i) for i,name in enumerate(names)]
  alias=''.join('/alternatename:%s=%s\n'%row for row in rows).encode()
  source=('/* Diagnostic-only, non-runnable unresolved-symbol address anchors. */\n'+''.join('unsigned char %s[4] = {0};\n'%a[1:] for _,a in rows)).encode()
  return alias,source,dict(rows)['?EnemyWrapAngle@@YGMM@Z']
 def test_exact_grammar(self):
  a,s,name=self.grammar();self.assertEqual(p.alias_binding(a,s),name)
 def test_mutated_grammar(self):
  a,s,_=self.grammar()
  for aa,ss in [(a.replace(b'/alternatename:',b'/other:',1),s),(a.replace(b'_0000',b'_9999',1),s),(a,s.replace(b'[4]',b'[8]',1)),(a,s+b' '),(a.replace(b'EnemyWrapAngle',b'Different'),s),(a+b'/alternatename:extra=_th10_ltcg_probe_anchor_0074\n',s)]:
   with self.assertRaises(AssertionError):p.alias_binding(aa,ss)
 def setUp(self):
  self.alias='_th10_ltcg_probe_anchor_0000';self.symbol='?EnemyWrapAngle@@YGMM@Z';self.obj='/fake/build/link-anchors.obj'
  self.maps=[dict(symbol=n,address=4100,section_number=1,section_offset=4,flags='',object='link-anchors.obj') for n in (self.symbol,self.alias)]
  self.modules=[dict(index=1,module='anchors',object=self.obj)]
  self.cs=[dict(section_number=1,section_offset=0,size=16,module_index=1,module='anchors',object=self.obj,characteristics=0xC0000080)]
  # Deliberately zero raw backing: diagnostic zero-fill mapping is sufficient.
  self.section=types.SimpleNamespace(rva=0,virtual_size=16,raw_size=0,characteristics=0xC0000080)
 def address(self):
  l=types.SimpleNamespace(_object_path=lambda x:str(x).lower(),_object_basename=lambda x:str(x).split('/')[-1].lower())
  image=types.SimpleNamespace(image_base=4096,section=lambda n:self.section)
  return p.anchor_address(l,image,self.maps,dict(modules=self.modules,contributions=self.cs),Path('/fake/build'),self.alias)
 def test_zero_fill_valid_pair(self):self.assertEqual(self.address(),4100)
 def test_pair_and_contribution_mutations(self):
  changes=[lambda:self.maps.pop(),lambda:self.maps.append(dict(self.maps[0],symbol='extra_alias')),lambda:self.maps[1].update(address=4104),lambda:self.maps[1].update(flags='f'),lambda:self.maps[1].update(object='wrong.obj'),lambda:self.maps[0].update(section_offset=0),lambda:self.modules.append(dict(self.modules[0])),lambda:self.modules[0].update(object='/other/link-anchors.obj'),lambda:self.cs[0].update(module_index=2),lambda:self.cs[0].update(module='other'),lambda:self.cs[0].update(object='/other/link-anchors.obj'),lambda:self.cs[0].update(size=7),lambda:self.cs.append(dict(self.cs[0])),lambda:self.cs.append(dict(self.cs[0],section_offset=6,size=1)),lambda:self.cs[0].update(characteristics=0x20000020),lambda:setattr(self.section,'characteristics',0x20000020),lambda:setattr(self.section,'virtual_size',7),lambda:self.maps.append(dict(self.maps[0],symbol='interior',address=4102,section_offset=6))]
  for i,change in enumerate(changes):
   self.setUp();change()
   with self.subTest(i=i),self.assertRaises(AssertionError):self.address()

class Tables(unittest.TestCase):
 def test_relocated_same_owner_tables(self):
  fixture=previous.base.TableTests();owner,source,jt,st=fixture.data();target=bytes(owner);origin=0x500000
  addresses=struct.unpack_from('<60I',owner,jt)
  struct.pack_into('<60I',owner,jt,*[a-previous.B+origin for a in addresses])
  table,entry,end,sel,jump=p.baseline_tables(owner,origin,target)
  self.assertEqual(sel,origin+st);self.assertEqual(jump,origin+jt);self.assertGreater(end,entry)
 def test_selector_drift_rejected(self):
  fixture=previous.base.TableTests();owner,_,_,st=fixture.data();target=bytes(owner);owner[st+82]=59
  with self.assertRaises(AssertionError):p.baseline_tables(owner,previous.B,target)
 def test_table_outside_owner_rejected(self):
  fixture=previous.base.TableTests();owner,_,jt,_=fixture.data();target=bytes(owner);struct.pack_into('<I',owner,jt,previous.B+7020)
  with self.assertRaises(AssertionError):p.baseline_tables(owner,previous.B,target)

if __name__=='__main__':
 suite=unittest.TestSuite([unittest.defaultTestLoader.loadTestsFromModule(previous.base),unittest.defaultTestLoader.loadTestsFromTestCase(previous.OrderTests),unittest.defaultTestLoader.loadTestsFromTestCase(Owners),unittest.defaultTestLoader.loadTestsFromTestCase(Aliases),unittest.defaultTestLoader.loadTestsFromTestCase(Tables)])
 result=unittest.TextTestRunner().run(suite);raise SystemExit(not result.wasSuccessful())
