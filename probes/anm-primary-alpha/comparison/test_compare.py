import os,ast,subprocess,sys,types,copy,csv,importlib.util,json,pathlib,tempfile,unittest
from unittest.mock import patch
import compare as c
P=pathlib.Path;ROOT=P(__file__).parent
class Tests(unittest.TestCase):
 def test_optimized_python_refused(self):
  done=subprocess.run([sys.executable,'-O',str(ROOT/'compare.py')],capture_output=True,text=True)
  self.assertEqual(done.returncode,1);self.assertEqual(json.loads(done.stdout)['error_class'],'OptimizedPythonDisabled')
 def test_no_all_owner_enumeration(self):
  tree=ast.parse((ROOT/'compare.py').read_text())
  self.assertFalse(any(isinstance(n,ast.Attribute) and n.attr=='linked_functions' for n in ast.walk(tree)))
 def test_artifact_identity_mismatches(self):
  image=types.SimpleNamespace(timestamp=1,image_base=4096,codeview_record=lambda:{'guid':'g','age':2});meta={'timestamp':1,'image_base':4096};pdb={'guid':'g','pdb_age':2};c.artifact_identity(image,meta,pdb)
  for key,value in [('timestamp',2),('image_base',8192)]:
   bad=dict(meta);bad[key]=value
   with self.assertRaises(ValueError):c.artifact_identity(image,bad,pdb)
  for key,value in [('guid','h'),('pdb_age',3)]:
   bad=dict(pdb);bad[key]=value
   with self.assertRaises(ValueError):c.artifact_identity(image,meta,bad)
 def test_real_pinned_parser_import_without_native_inputs(self):
  path=P(os.environ['TH10_LINKED_IMAGE'])
  self.assertEqual(c.sha(path),'58583c6241e0979288b0c4ce0a0e55f1ae2a178762d93743475d05518eff0354')
  name='host_regression_linked_image';module=c.load(name,path)
  self.assertIs(sys.modules[name],module);self.assertTrue(callable(module.PEImage));sys.modules.pop(name)
 def test_failed_import_removes_or_restores_registration(self):
  with tempfile.TemporaryDirectory() as t:
   p=P(t)/'bad.py';p.write_text('raise RuntimeError("synthetic")');name='host_failed_import'
   with self.assertRaises(RuntimeError):c.load(name,p)
   self.assertNotIn(name,sys.modules)
   sentinel=types.SimpleNamespace();sys.modules[name]=sentinel
   with self.assertRaises(RuntimeError):c.load(name,p)
   self.assertIs(sys.modules[name],sentinel);sys.modules.pop(name)
 def test_scope_and_pin_mutations(self):
  m=json.loads((ROOT/'manifest.json').read_text());m['execution_ready']=True;c.gate(m)
  for key,v in [('execution_ready',False),('size',129),('target_address',0x442301),('runner_sha256','a'*64),('manifest_sha256','a'*64),('candidate_run','../other'),('acceptance_authority','exact')]:
   bad=copy.deepcopy(m);bad[key]=v
   with self.assertRaises(ValueError):c.gate(bad)
 def test_raw_full130_only(self):
  result=c.raw_summary(b'a'*130,b'b'*130);self.assertEqual(result['raw_matched_bytes'],0);self.assertEqual(result['raw_mismatched_bytes'],130)
  self.assertTrue(c.raw_summary(b'a'*130,b'a'*130)['raw_equal'])
  for a,b in [(129,130),(130,129),(131,131),(0,0)]:
   with self.assertRaises(ValueError):c.raw_summary(b'a'*a,b'a'*b)
 def test_inventory_excludes_native_fields(self):
  r={'normalization_complete':True,'decoded_bytes':130,'fields':[{'type':'DIR32','width':4,'offset':12,'candidate_target':123,'candidate_symbols':['SECRET'],'instruction':'SECRET'}]}
  self.assertNotIn('SECRET',json.dumps(c.inventory(r)));self.assertEqual(c.inventory(r)['field_bytes'],4)
 def test_invalid_inventory_fails(self):
  base={'normalization_complete':True,'decoded_bytes':130,'fields':[]}
  for key,value in [('normalization_complete',False),('decoded_bytes',129),('fields',[{'type':'OTHER','width':4,'offset':0}]),('fields',[{'type':'DIR32','width':4,'offset':128}]),('fields',[{'type':'DIR32','width':4,'offset':-1}]),('fields',[{'type':'DIR32','width':2,'offset':1}]),('fields',[{'type':'DIR32','width':4,'offset':1}]*2)]:
   r=dict(base);r[key]=value
   with self.assertRaises(ValueError):c.inventory(r)
 def test_target_boundary_unique_and_exact(self):
  with tempfile.TemporaryDirectory() as t:
   p=P(t)/'f.csv';header='address,size,span_end,proposed_name\n';row='0x00442300,130,0x00442381,AnmVmView::StartPrimaryAlphaInterpolation\n';p.write_text(header+row);c.target_boundary(p)
   for rows in [row+row,row.replace(',130,',',129,'),row.replace('442381','442382'),'']:
    p.write_text(header+rows)
    with self.assertRaises(ValueError):c.target_boundary(p)
 def test_hash_drift_fails_before_import_or_native_read(self):
  with tempfile.TemporaryDirectory() as t:
   root=P(t);m=json.loads((ROOT/'manifest.json').read_text());m['execution_ready']=True;run=root/m['candidate_run'];run.mkdir(parents=True);(run/'runner.py').write_text('drift')
   with patch.object(c,'load') as load:
    with self.assertRaises(ValueError):c.compare(root,m)
    load.assert_not_called()
if __name__=='__main__':unittest.main()
