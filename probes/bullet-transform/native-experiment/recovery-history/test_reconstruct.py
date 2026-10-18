import copy,importlib.util,json,pathlib,tempfile,unittest,os
P=pathlib.Path;s=importlib.util.spec_from_file_location('r',P(__file__).with_name('reconstruct.py'));r=importlib.util.module_from_spec(s);s.loader.exec_module(r)
ROOT=P(os.environ.get('CHECKPOINT_REPO',P(__file__).resolve().parents[4]));BASE=ROOT/'probes/bullet-transform/native-experiment/a003';SPEC=json.loads(P(__file__).with_name('recipes.json').read_text())
class Tests(unittest.TestCase):
 def inputs(self):return [(BASE/'run.py').read_bytes(),(BASE/'manifest.proposed.json').read_bytes(),(BASE/'src/BulletTransform.cpp').read_bytes()]
 def test_exact_recipes(self):
  for tag in ('a004','r001','a005','a006','a007','a008'):
   out=r.reconstruct(*self.inputs(),SPEC,tag);self.assertEqual(set(out),{'run.py','manifest.proposed.json','src/BulletTransform.cpp'});self.assertTrue(json.loads(out['manifest.proposed.json'])['execution_ready'])
   self.assertEqual(r.digest(out['src/BulletTransform.cpp']),'824eb6a3a14fe140168ed1146f4aff7ece5bbc914639934ae35850f0ecdacccc' if tag in ('a007','a008') else 'e698e713a6f3f7e069970090e09c515ce9d64b3011bdfdd73ca046ea82b65f9e' if tag=='a006' else '72aa46821fdae7f35482a30fcfe8abbd48b075a75489f593d922e9ad2ab131d1' if tag=='a005' else 'b635d17c1f7bc76d3af8fc7b9fd4453f6c32b0e1f90fd6f718a4e6a7460516c0')
 def test_raw_crlf_base_rejected(self):
  inputs=self.inputs();inputs[2]=inputs[2].replace(b'\n',b'\r\n')
  with self.assertRaises(ValueError):r.reconstruct(*inputs,SPEC,'r001')
 def test_input_drift(self):
  for i in range(3):
   a=self.inputs();a[i]+=b'x'
   with self.assertRaises(ValueError):r.reconstruct(*a,SPEC,'r001')
 def test_splice_drift(self):
  for key in ('runner_splices','overlay_splices'):
   for mode in ('missing','extra','wrong'):
    x=copy.deepcopy(SPEC);ed=x['recipes']['r001'][key]
    if mode=='missing':ed.pop()
    if mode=='extra':ed.append(ed[-1])
    if mode=='wrong':ed[0]['replacement']+='x'
    with self.assertRaises(ValueError):r.reconstruct(*self.inputs(),x,'r001')
 def test_production_source_not_read(self):
  # Reject any accidental production-source dependency even when it exists.
  from unittest.mock import patch
  original=P.read_bytes
  def read(p):
   if p==ROOT/'src/BulletTransform.cpp':raise AssertionError('production source read')
   return original(p)
  with patch.object(P,'read_bytes',read):
   for tag in ('a004','r001','a005','a006','a007','a008'):r.reconstruct(*self.inputs(),SPEC,tag)
 def test_no_overwrite(self):
  with tempfile.TemporaryDirectory() as t:
   p=P(t)/'packet';r.materialize(p,{'run.py':b'fixture'})
   with self.assertRaises(ValueError):r.materialize(p,{'run.py':b'changed'})
   self.assertEqual((p/'run.py').read_bytes(),b'fixture')
if __name__=='__main__':unittest.main()
