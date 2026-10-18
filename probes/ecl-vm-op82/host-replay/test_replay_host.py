"""Portable adapter controls using synthetic text-only inputs; no native target."""
import importlib.util,json,pathlib,subprocess,sys,tempfile,unittest
from unittest.mock import patch
P=pathlib.Path;HERE=P(__file__).parent
sp=importlib.util.spec_from_file_location('replay_adapter',HERE/'replay_host.py');r=importlib.util.module_from_spec(sp);sp.loader.exec_module(r)
class Adapter(unittest.TestCase):
 def fixture(self,folder):
  root=folder/'source root with spaces';archive=folder/'archive with spaces';adapter=folder/'adapter';adapter.mkdir();root.mkdir();archive.mkdir()
  sources={}
  for rel in r.SOURCES:
   p=root/rel;p.parent.mkdir(exist_ok=True);p.write_bytes(b'synthetic\r\n'+rel.encode());sources[rel]=r.sha(p)
  files={
   'baseline/manifest.proposed.json':json.dumps({'base_commit':r.HEAD,'source_files':sources}),
   'destination-first-hypothesis/manifest.json':'{}',
   'destination-first-hypothesis/destination-first.patch':'synthetic patch\n',
   'destination-first-hypothesis/test_contract.py':"from pathlib import Path\nimport unittest\nSNAPSHOT=None\nclass T(unittest.TestCase):\n def test_source(self):self.assertTrue((SNAPSHOT/'EclVm.cpp').is_file())\nif __name__=='__main__':raise RuntimeError('must not run main')\n",
   'destination-first-hypothesis/compiled_contract.py':"BASE=None\ndef execute():\n assert BASE.name=='EclVm.cpp'\n assert ' ' in str(BASE)\n return {'all_passed':True,'host_only':True}\nif __name__=='__main__':raise RuntimeError('must not run main')\n"}
  pins={}
  for rel,data in files.items():
   p=archive/rel;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(data);pins[rel]=r.sha(p)
  (archive/'destination-first-hypothesis/compiled-contract-result.json').write_text('{"original":true}\n')
  (adapter/'adapter-pins.json').write_text(json.dumps(pins));return root,archive,adapter
 def test_spaces_snapshot_and_no_main_or_result_overwrite(self):
  with tempfile.TemporaryDirectory() as t:
   root,archive,adapter=self.fixture(P(t))
   with patch.object(r,'HERE',adapter):out=r.replay(root,archive,True)
   self.assertEqual(out['semantic_tests'],1);self.assertTrue(out['archived_result_unchanged']);self.assertEqual(out['source_mode'],'explicit-four-file-pinned-snapshot')
 def test_source_and_frozen_script_drift_prevent_import(self):
  for rel,which in [('src/Enemy.cpp','source'),('destination-first-hypothesis/compiled_contract.py','archive')]:
   with tempfile.TemporaryDirectory() as t:
    root,archive,adapter=self.fixture(P(t));((root if which=='source' else archive)/rel).write_text('drift')
    with patch.object(r,'HERE',adapter),patch.object(r,'load') as load,self.assertRaises(ValueError):r.replay(root,archive,True)
    load.assert_not_called()
 def test_symlink_file_and_ancestor_rejected(self):
  for mode in ('file','ancestor','archive'):
   with tempfile.TemporaryDirectory() as t:
    folder=P(t);root,archive,adapter=self.fixture(folder)
    if mode=='file':
     p=root/'src/EclVm.cpp';saved=folder/'saved';p.rename(saved);p.symlink_to(saved)
    elif mode=='ancestor':
     link=folder/'link';link.symlink_to(root,target_is_directory=True);root=link
    else:
     link=folder/'link';link.symlink_to(archive,target_is_directory=True);archive=link
    with patch.object(r,'HERE',adapter),self.assertRaises(ValueError):r.validate(root,archive,True)
 def test_repository_mode_requires_clean_exact_head(self):
  for outputs,ok in [([r.HEAD.encode(),b''],True),([b'0'*40],False),([r.HEAD.encode(),b' M source'],False)]:
   with tempfile.TemporaryDirectory() as t:
    root,archive,adapter=self.fixture(P(t))
    with patch.object(r,'HERE',adapter),patch.object(r.subprocess,'check_output',side_effect=outputs) as git:
     if ok:r.validate(root,archive,False,r.HEAD)
     else:
      with self.assertRaises(ValueError):r.validate(root,archive,False,r.HEAD)
     self.assertTrue(all(c.kwargs['timeout']==10 for c in git.call_args_list))
 def test_optimized_python_rejected_before_help(self):
  result=subprocess.run([sys.executable,'-O',str(HERE/'replay_host.py'),'--help'],capture_output=True,text=True)
  self.assertNotEqual(result.returncode,0);self.assertIn('optimized Python rejected',result.stderr)
 def test_source_mutation_during_execution_rejected(self):
  with tempfile.TemporaryDirectory() as t:
   root,archive,adapter=self.fixture(P(t));original=r.load
   def load(path,name):
    mod=original(path,name)
    if name.endswith('compiled_contract'):
     def mutate():
      (root/'src/Enemy.cpp').write_text('drift');return {}
     mod.execute=mutate
    return mod
   with patch.object(r,'HERE',adapter),patch.object(r,'load',side_effect=load),self.assertRaises(ValueError):r.replay(root,archive,True)
if __name__=='__main__':unittest.main()
