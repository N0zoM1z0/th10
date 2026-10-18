import copy,importlib.util,json,pathlib,subprocess,sys,tempfile,types,unittest
from unittest.mock import patch,Mock
P=pathlib.Path;ROOT=P(__file__).parent
sp=importlib.util.spec_from_file_location('runner',ROOT/'run.py');r=importlib.util.module_from_spec(sp);sp.loader.exec_module(r)
class Tests(unittest.TestCase):
 def manifest(self):
  m=json.loads((ROOT/'manifest.proposed.json').read_text());m['execution_ready']=True;return m
 def exact(self,size=273):return {'result':'exact','size':size,'compared_size':size,'matched_bytes':size,'matched_compared_bytes':size,'normalization_complete':True,'raw_equal':True,'image_sha256':'a'*64}
 def test_git_operations_have_timeouts(self):
  with patch.object(r.subprocess,'check_output',return_value=b'') as read:
   r.git(P('/fake'),'status');self.assertEqual(read.call_args.kwargs['timeout'],10)
  with tempfile.TemporaryDirectory() as t:
   root=P(t);repo=root/'repo';packet=root/'packet';repo.mkdir();packet.mkdir()
   with patch.object(r.subprocess,'run',return_value=types.SimpleNamespace(returncode=0)) as check:
    r.scope(repo,packet,repo/'.analysis'/'run');self.assertEqual(check.call_args.kwargs['timeout'],10)
 def test_manifest_and_scope_mutations(self):
  m=self.manifest();r.manifest_gate(m)
  for key,value in [('execution_ready',False),('base_commit','a'*40),('cold_link_calls',2),('owners',{}),('source_changes',True)]:
   x=copy.deepcopy(m);x[key]=value
   with self.subTest(key=key),self.assertRaises(ValueError):r.manifest_gate(x)
  for key,value in [('support_sources',list(reversed(r.SUPPORT))),('profile',r.PROFILE[:-1]),('link_profile',[]),('entry_symbol','bad')]:
   x=copy.deepcopy(m);x['graph'][key]=value
   with self.subTest(key=key),self.assertRaises(ValueError):r.manifest_gate(x)
 def test_resource_policy_exact(self):
  self.assertEqual(r.RESOURCE_POLICY,{'cpu_seconds':120,'wall_seconds':150,'nice_increment':10,'cpu_affinity_count':1,'address_space':'inherit_unlimited_without_setrlimit'})
  m=self.manifest();m['resource_policy']['cpu_seconds']=121
  with self.assertRaises(ValueError):r.manifest_gate(m)
  import ast
  tree=ast.parse((ROOT/'run.py').read_text());calls=[n for n in ast.walk(tree) if isinstance(n,ast.Call) and isinstance(n.func,ast.Attribute) and n.func.attr=='setrlimit']
  self.assertEqual(len(calls),1);self.assertEqual(calls[0].args[0].attr,'RLIMIT_CPU')
 def test_env_case_insensitive(self):
  r.env_gate({'PATH':'x'})
  for key in ['CL','cl','_CL_','_cl_','Cl','WINE','WINESERVER','WINELOADER']:
   with self.assertRaises(ValueError):r.env_gate({key:''})
 def test_comparator_excludes_sensitive_fields(self):
  v=self.exact();v.update(first_differences=[{'linked':222,'target':111}],candidate_address='SECRET',error='SECRET',linkages=['SECRET'])
  out=r.comparator_summary(v,0,273);self.assertNotIn('SECRET',json.dumps(out));self.assertNotIn('first_differences',out)
  for key,value in [('matched_bytes',884),('normalization_complete',False),('size',1454)]:
   x=dict(v);x[key]=value
   with self.assertRaises(ValueError):r.comparator_summary(x,0,273)
  with self.assertRaises(ValueError):r.comparator_summary(v,1,273)
 def test_copies_raw_inputs_and_rejects_drift(self):
  with tempfile.TemporaryDirectory() as t:
   root=P(t);repo=root/'repo';repo.mkdir();m=self.manifest();pins={}
   for rel in r.input_pins(m):
    p=repo/rel;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(b'raw\r\n\x00'+rel.encode());pins[rel]=r.sha(p)
   m['source_files']={k:pins[k] for k in m['source_files']};m['tool_pins']={k:pins[k] for k in m['tool_pins']};m['canonical_config_sha256']=pins['config/match-units.toml'];m['target_sha256']=pins['resources/th10.exe']
   r.copy_inputs(repo,root/'candidate',m)
   for rel,digest in pins.items():self.assertEqual(r.sha(root/'candidate'/rel),digest)
   (repo/'src/AnmManager.cpp').write_bytes(b'drift')
   with self.assertRaises(ValueError):r.copy_inputs(repo,root/'another',m)
 def mock_worker(self,mode):
  with tempfile.TemporaryDirectory() as t:
   run=P(t);tree=run/'candidate';build=tree/'build/match-linked/AnmSetupVertexBuffer';m=self.manifest();record={}
   def copy_inputs(*a):
    build.mkdir(parents=True);(tree/'scripts').mkdir();(tree/'scripts/ltcg_link.py').touch();(tree/'scripts/linked_image.py').touch()
   def cold(*args):
    record['args']=args;paths={k:build/('source.'+ext) for k,ext in [('image','exe'),('map','map'),('pdb','pdb')]}
    for p in paths.values():p.write_bytes(b'synthetic fixture, not PE')
    obj=build/'source.ltcg.obj';obj.write_bytes(b'fixture');return dict(paths,objects=[obj],unresolved_symbols=[])
   l=types.SimpleNamespace(__file__=str(tree/'scripts/ltcg_link.py'),ROOT=tree,LTCG_PROFILE=r.PROFILE[:-1],LINK_PROFILE=r.LINK,HARNESS_KIND='unresolved-symbol-data-anchors-v1',tool_environment=lambda:(P('/fake/link.exe'),{}),cold_link=Mock(side_effect=cold),windows_path=lambda *a:None)
   li=types.SimpleNamespace(__file__=str(tree/'scripts/linked_image.py'),linked_functions=lambda *a:None,PEImage=lambda *a:None,map_publics=lambda *a:[],map_statics=lambda *a:[],pdb_section_contributions=lambda *a:{})
   def compare(*args,**kwargs):
    v=self.exact(r.GUARDS[args[0][3]]);v['image_sha256']=r.sha(build/'source.exe');kwargs['stdout'].write(json.dumps(v).encode());return types.SimpleNamespace(returncode=0)
   measure=lambda li,image,maps,pdb,build,spec:dict(complete=True,size=spec['target_size'],sha256='a'*64,target_size=spec['target_size'],size_equal=True,all_required_attribution_gates=True)
   with patch.object(r.traceback,'print_exc'),patch.object(r,'checked_windows_path',side_effect=ValueError('winepath failure') if mode=='winepath' else None),patch.object(r,'validate'),patch.object(r,'postcheck',side_effect=ValueError('postcheck failure') if mode=='postcheck' else None),patch.object(r,'copy_inputs',side_effect=copy_inputs),patch.object(r.os,'nice'),patch.object(r.resource,'getrlimit',return_value=(-1,-1)),patch.object(r.resource,'setrlimit') as set_limit,patch.object(r.os,'sched_setaffinity'),patch.object(r.importlib,'import_module',side_effect=lambda name:l if name=='ltcg_link' else li),patch.object(r,'measure',side_effect=measure),patch.object(r.subprocess,'run',side_effect=compare):
    self.assertEqual(r.worker(P('/fake/repo'),run,m),0 if mode=='success' else 1)
   if mode=='winepath':
    self.assertEqual(l.cold_link.call_count,0);self.assertEqual(r.status(run,'worker-receipt.json')['state'],'failed');return
   if mode=='postcheck':
    self.assertEqual(l.cold_link.call_count,1);self.assertFalse(r.status(run,'worker-receipt.json')['baseline_valid']);return
   args=record['args'];set_limit.assert_called_once_with(r.resource.RLIMIT_CPU,(120,120));self.assertEqual(l.cold_link.call_count,1);self.assertEqual(args[0],tree/'src/AnmManager.cpp');self.assertEqual(args[1],build);self.assertEqual(args[5],r.PROFILE);self.assertEqual(args[6],[tree/x for x in r.SUPPORT]);self.assertEqual(args[7],[]);self.assertTrue(r.status(run,'worker-receipt.json')['baseline_valid']);self.assertFalse((run/'receipt.json').exists())
 def test_mock_worker_one_fixed_call(self):self.mock_worker('success')
 def test_worker_postcheck_clears_baseline(self):self.mock_worker('postcheck')
 def test_winepath_preflight_before_compile(self):self.mock_worker('winepath')
 def test_timeout_group_cleanup_and_no_retry(self):
  with tempfile.TemporaryDirectory() as t:
   root=P(t);packet=root/'packet';packet.mkdir();(packet/'run.py').write_bytes(b'fixture');(packet/'manifest.proposed.json').write_text(json.dumps(self.manifest()));run=root/'run'
   process=Mock(pid=12345,returncode=-9);process.wait.side_effect=[subprocess.TimeoutExpired('fixture',150),None,None]
   with patch.object(r,'validate'),patch.object(r,'clean'),patch.object(r,'postcheck'),patch.object(r.subprocess,'Popen',return_value=process),patch.object(r.os,'killpg') as kill:
    code=r.start(root,packet,run,self.manifest(),r.sha(packet/'run.py'),r.sha(packet/'manifest.proposed.json'));self.assertEqual(code,1);self.assertEqual(kill.call_count,2);self.assertEqual(r.status(run)['state'],'timed_out')
    with self.assertRaises(FileExistsError):r.start(root,packet,run,self.manifest(),r.sha(packet/'run.py'),r.sha(packet/'manifest.proposed.json'))
   self.assertTrue((run/'exit.json').exists())
 def test_supervisor_failure_paths_durable(self):
  for mode in ('copy_failure','spawn_failure','sigterm','contradictory_exit'):
   with self.subTest(mode=mode),tempfile.TemporaryDirectory() as t:
    root=P(t);packet=root/'packet';packet.mkdir();(packet/'run.py').write_bytes(b'fixture');(packet/'manifest.proposed.json').write_text(json.dumps(self.manifest()));run=root/'run'
    process=Mock(pid=12345,returncode=1 if mode=='contradictory_exit' else -15)
    def first_wait(*args,**kwargs):
     if mode=='sigterm':r.signal.getsignal(r.signal.SIGTERM)(r.signal.SIGTERM,None)
     if mode=='contradictory_exit':r.write(run/'worker-receipt.json',{'state':'completed','production_clean':True,'postrun_pins_valid':True})
    process.wait.side_effect=[first_wait,None,None]
    # Mock side_effect sequence returns callables rather than calling them; use a dispatcher.
    calls=[]
    def wait(*args,**kwargs):
     calls.append(1)
     if len(calls)==1:return first_wait(*args,**kwargs)
     return process.returncode
    process.wait.side_effect=wait
    popen=Mock(side_effect=OSError('synthetic spawn failure')) if mode=='spawn_failure' else Mock(return_value=process)
    real_copy=r.shutil.copyfile
    def copy_file(*args,**kwargs):
     if mode=='copy_failure':raise OSError('synthetic copy failure')
     return real_copy(*args,**kwargs)
    with patch.object(r,'validate'),patch.object(r,'clean'),patch.object(r,'postcheck'),patch.object(r.subprocess,'Popen',popen),patch.object(r.os,'killpg') as kill,patch.object(r.shutil,'copyfile',side_effect=copy_file):
     self.assertEqual(r.start(root,packet,run,self.manifest(),r.sha(packet/'run.py'),r.sha(packet/'manifest.proposed.json')),1)
     self.assertEqual(r.status(run)['state'],'failed');self.assertTrue((run/'exit.json').exists())
     if mode in ('sigterm','contradictory_exit'):self.assertEqual(kill.call_count,2)
     with self.assertRaises(FileExistsError):r.start(root,packet,run,self.manifest(),r.sha(packet/'run.py'),r.sha(packet/'manifest.proposed.json'))
 def test_failed_worker_receipt_preserved(self):
  with tempfile.TemporaryDirectory() as t:
   root=P(t);packet=root/'packet';packet.mkdir();(packet/'run.py').write_bytes(b'fixture');(packet/'manifest.proposed.json').write_text(json.dumps(self.manifest()));run=root/'run';process=Mock(pid=12345,returncode=1);calls=[]
   def wait(*args,**kwargs):
    calls.append(1)
    if len(calls)==1:r.write(run/'worker-receipt.json',{'state':'failed','stage':'cold_link','error_class':'ValueError','cold_link_calls':1,'runtime_executed':False,'owner_exactness_tested':False})
    return 1
   process.wait.side_effect=wait
   with patch.object(r,'validate'),patch.object(r,'clean'),patch.object(r,'postcheck'),patch.object(r.subprocess,'Popen',return_value=process),patch.object(r.os,'killpg'):
    self.assertEqual(r.start(root,packet,run,self.manifest(),r.sha(packet/'run.py'),r.sha(packet/'manifest.proposed.json')),1)
   original=json.loads((run/'worker-receipt.json').read_text());out=r.status(run);self.assertEqual(out['error_class'],'ValueError');self.assertEqual(out['stage'],'cold_link');self.assertEqual(out['cold_link_calls'],1);self.assertNotIn('runner_sha256',original)
 def test_worker_and_supervisor_errors_both_survive(self):
  for mode in ('interrupted','cleanup_failed'):
   with self.subTest(mode=mode),tempfile.TemporaryDirectory() as t:
    root=P(t);packet=root/'packet';packet.mkdir();(packet/'run.py').write_bytes(b'fixture');(packet/'manifest.proposed.json').write_text(json.dumps(self.manifest()));run=root/'run';process=Mock(pid=12345,returncode=1)
    def wait(*args,**kwargs):
     r.write(run/'worker-receipt.json',{'state':'failed','stage':'cold_link','error_class':'ValueError','cold_link_calls':1})
     if mode=='interrupted':raise InterruptedError('synthetic supervisor interruption')
     return 1
    process.wait.side_effect=wait
    with patch.object(r,'validate'),patch.object(r,'clean'),patch.object(r,'postcheck'),patch.object(r.subprocess,'Popen',return_value=process),patch.object(r,'terminate_group',side_effect=OSError('synthetic cleanup error') if mode=='cleanup_failed' else None):
     self.assertEqual(r.start(root,packet,run,self.manifest(),r.sha(packet/'run.py'),r.sha(packet/'manifest.proposed.json')),1)
    out=r.status(run);self.assertEqual(out['error_class'],'ValueError')
    if mode=='interrupted':self.assertEqual(out['supervisor_error_class'],'InterruptedError')
    else:self.assertEqual(out['cleanup_error_class'],'OSError');self.assertFalse(out['process_group_terminated'])
 def test_status_rejects_payload_in_guard(self):
  with tempfile.TemporaryDirectory() as t:
   root=P(t);r.write(root/'receipt.json',{'state':'completed','guards':{'anm-vm-initialize-private':dict(self.exact(),exit_ok=True,first_differences=['secret'])}})
   with self.assertRaises(ValueError):r.status(root)
 def test_supervisor_failures_clear_valid_worker_receipt(self):
  for mode in ('clean','postcheck','cleanup','timeout'):
   with self.subTest(mode=mode),tempfile.TemporaryDirectory() as t:
    root=P(t);packet=root/'packet';packet.mkdir();(packet/'run.py').write_bytes(b'fixture');(packet/'manifest.proposed.json').write_text(json.dumps(self.manifest()));run=root/'run'
    process=Mock(pid=12345,returncode=0);calls=[]
    def wait(*args,**kwargs):
     calls.append(1)
     if len(calls)==1:
      r.write(run/'worker-receipt.json',{'state':'completed','baseline_valid':True})
      if mode=='timeout':raise subprocess.TimeoutExpired('fixture',150)
     return 0
    process.wait.side_effect=wait
    with patch.object(r,'validate'),patch.object(r,'clean',side_effect=ValueError('dirty') if mode=='clean' else None),patch.object(r,'postcheck',side_effect=ValueError('pin drift') if mode=='postcheck' else None),patch.object(r,'terminate_group',side_effect=OSError('cleanup failure') if mode=='cleanup' else None),patch.object(r.subprocess,'Popen',return_value=process):
     self.assertEqual(r.start(root,packet,run,self.manifest(),r.sha(packet/'run.py'),r.sha(packet/'manifest.proposed.json')),1)
    out=r.status(run);self.assertEqual(out['state'],'timed_out' if mode=='timeout' else 'failed');self.assertFalse(out['baseline_valid'])
 def test_optimized_runner_refused(self):
  p=subprocess.run([sys.executable,'-O',str(ROOT/'run.py'),'--help'],capture_output=True);self.assertNotEqual(p.returncode,0)
if __name__=='__main__':unittest.main()
