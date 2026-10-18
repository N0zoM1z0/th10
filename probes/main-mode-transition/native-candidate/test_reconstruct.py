#!/usr/bin/env python3
"""Source-only reconstruction controls. No native target execution."""
import argparse,importlib.util,json,tempfile,shutil,sys
from pathlib import Path
sys.dont_write_bytecode=True
if sys.flags.optimize:raise SystemExit('optimized Python is unsupported')
root=Path(__file__).resolve().parent;p=argparse.ArgumentParser();p.add_argument('--base-tree',required=True,type=Path);p.add_argument('--a003-tree',required=True,type=Path);a=p.parse_args()
spec=importlib.util.spec_from_file_location('reconstruct',root/'reconstruct.py');r=importlib.util.module_from_spec(spec);spec.loader.exec_module(r);m=json.loads((root/'manifest.json').read_text())
with tempfile.TemporaryDirectory(prefix='mode-source-tests-') as td:
 temp=Path(td);out=temp/'valid';r.reconstruct(a.base_tree,a.a003_tree,out)
 assert len(m['overlays'])==7
 for rel,digest in m['overlays'].items():assert r.sha((out/rel).read_bytes())==digest
 def reject(base,old,dest):
  try:r.reconstruct(base,old,dest)
  except ValueError:return
  raise AssertionError('accepted invalid reconstruction')
 reject(a.base_tree,a.a003_tree,out)
 for group,tree in (('a003_inputs',a.a003_tree),('production_inputs',a.base_tree)):
  bad=temp/group;(bad/'src').mkdir(parents=True)
  for rel in m[group]:shutil.copyfile(tree/rel,bad/rel)
  first=next(iter(m[group]));(bad/first).write_bytes((bad/first).read_bytes()+b' ')
  destination=temp/(group+'-out');reject(bad if group=='production_inputs' else a.base_tree,bad if group=='a003_inputs' else a.a003_tree,destination);assert not destination.exists()
 clone=temp/'packet';shutil.copytree(root,clone,ignore=shutil.ignore_patterns('__pycache__'));r.ROOT=clone
 patch=clone/'patches/Main.cpp.patch';patch.write_bytes(patch.read_bytes()+b'bad\n');dest=temp/'bad-patch-out';reject(a.base_tree,a.a003_tree,dest);assert not dest.exists()
print(json.dumps({'status':'PASS','seven_overlays_reconstructed':True,'negative_controls':4,'target_or_native_execution':False}))
