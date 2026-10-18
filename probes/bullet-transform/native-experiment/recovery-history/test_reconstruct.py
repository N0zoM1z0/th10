import copy,importlib.util,json,pathlib,tempfile,unittest,os
P=pathlib.Path;s=importlib.util.spec_from_file_location('r',P(__file__).with_name('reconstruct.py'));r=importlib.util.module_from_spec(s);s.loader.exec_module(r)
ROOT=P(os.environ.get('CHECKPOINT_REPO',P(__file__).resolve().parents[4]));BASE=ROOT/'probes/bullet-transform/native-experiment/a003';SPEC=json.loads(P(__file__).with_name('recipes.json').read_text())
class Tests(unittest.TestCase):
 def inputs(self):
  # Test fixture only: ordinary checkout may use CRLF; production reconstruction stays raw-strict.
  fixture=(ROOT/'src/BulletTransform.cpp').read_bytes().replace(b'\r\n',b'\n');self.assertNotIn(b'\r',fixture)
  return [(BASE/'run.py').read_bytes(),(BASE/'manifest.proposed.json').read_bytes(),fixture]
 def test_exact_recipes(self):
  for tag in ('a004','r001'):
   out=r.reconstruct(*self.inputs(),SPEC,tag);self.assertEqual(set(out),{'run.py','manifest.proposed.json','src/BulletTransform.cpp'});self.assertTrue(json.loads(out['manifest.proposed.json'])['execution_ready'])
 def test_raw_crlf_overlay_rejected(self):
  inputs=self.inputs();inputs[2]=inputs[2].replace(b'\n',b'\r\n')
  with self.assertRaises(ValueError):r.reconstruct(*inputs,SPEC,'r001')
 def test_input_drift(self):
  for i in range(3):
   a=self.inputs();a[i]+=b'x'
   with self.assertRaises(ValueError):r.reconstruct(*a,SPEC,'r001')
 def test_splice_drift(self):
  for mode in ('missing','extra','wrong'):
   x=copy.deepcopy(SPEC);ed=x['recipes']['r001']['runner_splices']
   if mode=='missing':ed.pop()
   if mode=='extra':ed.append(ed[-1])
   if mode=='wrong':ed[0]['replacement']+='x'
   with self.assertRaises(ValueError):r.reconstruct(*self.inputs(),x,'r001')
 def test_no_overwrite(self):
  with tempfile.TemporaryDirectory() as t:
   p=P(t)/'packet';r.materialize(p,{'run.py':b'fixture'})
   with self.assertRaises(ValueError):r.materialize(p,{'run.py':b'changed'})
   self.assertEqual((p/'run.py').read_bytes(),b'fixture')
if __name__=='__main__':unittest.main()
