"""Synthetic whole-contribution owner tests. No executable/PDB inputs."""
import hashlib,importlib.util,pathlib,types,unittest
P=pathlib.Path;sp=importlib.util.spec_from_file_location('runner',P(__file__).with_name('run.py'));r=importlib.util.module_from_spec(sp);sp.loader.exec_module(r)
class Owners(unittest.TestCase):
 def setUp(self):
  self.spec=r.OWNERS['execute'];obj=self.spec['object'];self.maps=[dict(symbol=self.spec['symbol'],object=obj,section_number=1,section_offset=0,address=4096,flags='f')]
  self.cs=[dict(section_number=1,section_offset=0,size=9964,module_index=1,module='producer',object='/fake/build/'+obj,characteristics=3)]
  self.modules=[dict(index=1,module='producer',object='/fake/build/'+obj)]
  self.section=types.SimpleNamespace(rva=0,characteristics=3,virtual_size=16384)
  self.read=lambda a,b,n:b'X'*(n-4)+b'TABL'
 def result(self):
  def path(p):
   p=str(p)
   if not p.startswith('/'):raise ValueError('unrelated module path unsupported')
   return p.lower()
  image=types.SimpleNamespace(image_base=4096,section=lambda n:self.section,read_section=lambda *a:self.read(*a))
  l=types.SimpleNamespace(_object_path=path,_object_basename=lambda p:str(p).split('/')[-1].lower(),IMAGE_SCN_CNT_CODE=1,IMAGE_SCN_MEM_EXECUTE=2)
  return r.measure(l,image,self.maps,dict(modules=self.modules,contributions=self.cs),P('/fake/build'),self.spec)
 def test_full_extent_includes_table(self):
  out=self.result();self.assertEqual(out['size'],9964);self.assertEqual(out['sha256'],hashlib.sha256(b'X'*9960+b'TABL').hexdigest())
 def test_size_mismatch_never_cropped(self):
  self.cs[0]['size']=10020;out=self.result();self.assertEqual(out['size'],10020);self.assertFalse(out['size_equal']);self.assertEqual(out['sha256'],hashlib.sha256(b'X'*10016+b'TABL').hexdigest())
 def test_unrelated_unusable_module_paths(self):
  self.modules.extend([dict(index=2,module='linker',object=''),dict(index=3,module='library',object='relative.obj')]);self.assertTrue(self.result()['complete'])
 def test_attribution_mutations(self):
  changes=[lambda:self.maps.append(dict(self.maps[0],symbol='alias')),lambda:self.maps.clear(),lambda:self.cs.append(dict(self.cs[0])),lambda:self.cs[0].update(object='/wrong/'+self.spec['object']),lambda:self.modules[0].update(object='/wrong/'+self.spec['object']),lambda:self.modules.append(dict(self.modules[0])),lambda:self.cs[0].update(module_index=2),lambda:self.cs[0].update(module='wrong'),lambda:self.maps[0].update(object='other.obj'),lambda:self.cs[0].update(characteristics=1),lambda:setattr(self.section,'characteristics',1),lambda:setattr(self.section,'virtual_size',9963),lambda:self.maps[0].update(address=0),lambda:self.maps.append(dict(self.maps[0],symbol='inside',section_offset=100)),lambda:self.cs.append(dict(self.cs[0],section_offset=100)),lambda:setattr(self,'read',lambda *a:b'X')]
  for i,change in enumerate(changes):
   self.setUp();change()
   with self.subTest(i=i),self.assertRaises((AssertionError,ValueError)):self.result()
if __name__=='__main__':unittest.main()
