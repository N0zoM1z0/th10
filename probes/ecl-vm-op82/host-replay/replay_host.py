#!/usr/bin/env python3
"""Explicit pinned source-root adapter; no original __main__ or result-file writes."""
import argparse,hashlib,importlib.util,io,json,os,pathlib,subprocess,sys,unittest
sys.dont_write_bytecode=True
P=pathlib.Path
HERE=P(__file__).absolute().parent
HEAD='a4edda20e434cd40ee99ea3b6b68673a80c3e3be'
SOURCES=('src/EclVm.cpp','src/EclVm.hpp','src/Enemy.cpp','src/EnemyEclDispatcher.cpp')
def require(ok,message):
 if not ok:raise ValueError(message)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def path_gate(path,directory=False):
 p=P(os.path.abspath(path))
 for part in (p,*p.parents):require(not part.is_symlink(),'symlink path rejected')
 require(p.is_dir() if directory else p.is_file(),'missing path')
 return p
def pinned(root,rel,digest):
 require(not P(rel).is_absolute() and '..' not in P(rel).parts,'unsafe relative path')
 p=path_gate(root/rel);require(sha(p)==digest,'raw pin drift: '+rel);return p
def validate(root,archive,snapshot=False,require_head=None):
 require(__debug__,'optimized Python rejected');root=path_gate(root,True);archive=path_gate(archive,True)
 pins=json.loads(path_gate(HERE/'adapter-pins.json').read_text())
 for rel,h in pins.items():pinned(archive,rel,h)
 m=json.loads((archive/'baseline/manifest.proposed.json').read_text());require(m['base_commit']==HEAD,'HEAD manifest drift')
 source_names=SOURCES
 for rel in source_names:pinned(root,rel,m['source_files'][rel])
 if require_head is not None:
  require(len(require_head)==40 and all(c in '0123456789abcdef' for c in require_head),'invalid required HEAD')
  require(subprocess.check_output(['git','-C',str(root),'rev-parse','HEAD'],timeout=10).decode().strip()==require_head,'repository HEAD drift')
  require(not subprocess.check_output(['git','-C',str(root),'status','--porcelain=v1','--untracked-files=all'],timeout=10),'repository dirty')
 return root,archive

def load(path,name):
 spec=importlib.util.spec_from_file_location(name,path);module=importlib.util.module_from_spec(spec);sys.modules[name]=module
 try:spec.loader.exec_module(module)
 except BaseException:sys.modules.pop(name,None);raise
 return module

def replay(root,archive,snapshot=False,require_head=None):
 root,archive=validate(root,archive,snapshot,require_head)
 saved_result=archive/'destination-first-hypothesis/compiled-contract-result.json'
 result_before=sha(path_gate(saved_result)) if saved_result.exists() else None
 contract=load(archive/'destination-first-hypothesis/test_contract.py','ecl_frozen_host_contract')
 compiled=load(archive/'destination-first-hypothesis/compiled_contract.py','ecl_frozen_compiled_contract')
 # The only globals changed in either frozen module are their source input roots.
 contract.SNAPSHOT=root/'src';compiled.BASE=root/'src/EclVm.cpp'
 stream=io.StringIO();suite=unittest.defaultTestLoader.loadTestsFromModule(contract)
 tests=unittest.TextTestRunner(stream=stream,verbosity=0).run(suite)
 require(tests.wasSuccessful(),'frozen semantic contracts failed: '+stream.getvalue())
 report=compiled.execute()
 validate(root,archive,snapshot,require_head)
 require((sha(path_gate(saved_result)) if saved_result.exists() else None)==result_before,'archived result modified')
 return {'scope':'host-only; canceled unbuilt candidate; no VC7.1 or target exactness claim','source_mode':'clean-head-checked-four-file-pinned-root' if require_head else ('explicit-four-file-pinned-snapshot' if snapshot else 'four-file-pinned-source-root'),'semantic_tests':tests.testsRun,'compiled_report':report,'archived_result_unchanged':True,'source_pins_unchanged':True}

def main():
 require(__debug__,'optimized Python rejected')
 ap=argparse.ArgumentParser();ap.add_argument('--repo-root',type=P,required=True);ap.add_argument('--archive-root',type=P,default=HERE.parent);ap.add_argument('--source-snapshot',action='store_true',help='Explicit host-only snapshot: validate only four required raw sources; do not claim Git HEAD identity.')
 ap.add_argument('--require-head',help='Optionally require this exact clean Git HEAD in addition to the four source pins.')
 a=ap.parse_args();print(json.dumps(replay(a.repo_root,a.archive_root,a.source_snapshot,a.require_head),indent=2,sort_keys=True))
if __name__=='__main__':main()
