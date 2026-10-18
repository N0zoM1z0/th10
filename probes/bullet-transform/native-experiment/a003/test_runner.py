import copy,importlib.util,json,pathlib,subprocess,sys,tempfile,types,unittest
from unittest.mock import patch,Mock
P=pathlib.Path;ROOT=P(__file__).parent
sp=importlib.util.spec_from_file_location('runner',ROOT/'run.py');r=importlib.util.module_from_spec(sp);sp.loader.exec_module(r)
class Tests(unittest.TestCase):
 def manifest(self):
  m=json.loads((ROOT/'manifest.proposed.json').read_text());m['execution_ready']=True;return m
 def exact(self):return {'result':'exact','size':885,'compared_size':885,'matched_bytes':885,'matched_compared_bytes':885,'normalization_complete':True,'raw_equal':False,'image_sha256':'a'*64}
 def test_overlay_scope_and_candidate_pins(self):
  m=self.manifest();r.manifest_gate(m);base=r.input_pins(m);candidate=r.candidate_pins(m)
  self.assertEqual({k for k in base if base[k]!=candidate[k]},{'src/BulletTransform.cpp'});self.assertEqual(base['src/BulletTransform.cpp'],r.BASE_TRANSFORM_SHA)
  for overlays in ({},{'src/BulletRuntime.cpp':next(iter(r.OVERLAYS.values()))},{**r.OVERLAYS,'src/Extra.cpp':'a'*64},{'src/BulletTransform.cpp':'a'*64}):
   x=copy.deepcopy(m);x['overlays']=overlays
   with self.subTest(overlays=overlays),self.assertRaises(ValueError):r.manifest_gate(x)
 def test_overlay_inventory_rejects_mutations(self):
  for mode in ('extra','missing','changed','symlink'):
   with self.subTest(mode=mode),tempfile.TemporaryDirectory() as t:
    root=P(t);(root/'src').mkdir();p=root/'src/BulletTransform.cpp';p.write_bytes((ROOT/'src/BulletTransform.cpp').read_bytes());r.validate_overlay(root,self.manifest())
    if mode=='extra':(root/'src/Extra.cpp').write_bytes(b'')
    if mode=='missing':p.unlink()
    if mode=='changed':p.write_bytes(p.read_bytes()+b' ')
    if mode=='symlink':
     payload=root/'payload';payload.write_bytes(p.read_bytes());p.unlink();p.symlink_to(payload)
    with self.assertRaises(ValueError):r.validate_overlay(root,self.manifest())
 def test_git_operations_have_timeouts(self):
  with patch.object(r.subprocess,'check_output',return_value=b'') as read:
   r.git(P('/fake'),'status');self.assertEqual(read.call_args.kwargs['timeout'],10)
  with tempfile.TemporaryDirectory() as t:
   root=P(t);repo=root/'repo';packet=root/'packet';repo.mkdir();packet.mkdir()
   with patch.object(r.subprocess,'run',return_value=types.SimpleNamespace(returncode=0)) as check:
    r.scope(repo,packet,repo/'.analysis'/'run');self.assertEqual(check.call_args.kwargs['timeout'],10)
 def test_manifest_and_scope_mutations(self):
  m=self.manifest();r.manifest_gate(m)
  for key,value in [('execution_ready',False),('base_commit','a'*40),('cold_link_calls',2),('owners',{}),('source_changes',False)]:
   x=copy.deepcopy(m);x[key]=value
   with self.subTest(key=key),self.assertRaises(ValueError):r.manifest_gate(x)
  for key,value in [('normal_support_sources',[]),('support_sources',list(reversed(r.SUPPORT))),('profile',r.PROFILE[:-1]),('link_profile',[]),('entry_symbol','bad')]:
   x=copy.deepcopy(m);x['canonical_unit'][key]=value
   with self.subTest(key=key),self.assertRaises(ValueError):r.manifest_gate(x)
 def test_build_input_identity_unchanged(self):
  old=json.loads((ROOT.parent/'bullet1454-native-plan/manifest.proposed.json').read_text());new=self.manifest()
  for key in ('base_commit','source_files','canonical_config_sha256','canonical_unit','source_include_closure','owners','tool_pins_from_prior_verified_packet','additional_remote_pins','target_sha256','guard_replay'):
   with self.subTest(key=key):self.assertEqual(new[key],old[key])
 def test_inherited_as_limit_fails_before_build(self):
  with tempfile.TemporaryDirectory() as t:
   run=P(t)
   with patch.object(r,'validate'),patch.object(r,'postcheck'),patch.object(r.os,'nice'),patch.object(r.resource,'getrlimit',return_value=(1024,1024)),patch.object(r.resource,'setrlimit') as change,patch.object(r,'copy_inputs') as copy_inputs,patch.object(r.traceback,'print_exc'):
    self.assertEqual(r.worker(P('/fake/repo'),run,self.manifest()),1);change.assert_not_called();copy_inputs.assert_not_called()
   out=r.status(run,'worker-receipt.json');self.assertEqual(out['stage'],'resource_setup');self.assertEqual(out['cold_link_calls'],0);self.assertEqual(out['error_class'],'ValueError')
 def test_resource_policy_exact(self):
  self.assertEqual(r.RESOURCE_POLICY,{'cpu_seconds':120,'wall_seconds':150,'nice_increment':10,'cpu_affinity_count':1,'address_space':'inherit_unlimited_without_setrlimit'})
  m=self.manifest();m['resource_policy']['cpu_seconds']=121
  with self.assertRaises(ValueError):r.manifest_gate(m)
  import ast
  tree=ast.parse((ROOT/'run.py').read_text());calls=[n for n in ast.walk(tree) if isinstance(n,ast.Call) and isinstance(n.func,ast.Attribute) and n.func.attr=='setrlimit']
  self.assertEqual(len(calls),1);self.assertEqual(calls[0].args[0].attr,'RLIMIT_CPU')
 def test_env_case_insensitive(self):
  r.env_gate({'PATH':'x'})
  for key in ['CL','cl','_CL_','_cl_','Cl']:
   with self.assertRaises(ValueError):r.env_gate({key:''})
 def test_comparator_excludes_sensitive_fields(self):
  v=self.exact();v.update(first_differences=[{'linked':222,'target':111}],candidate_address='SECRET',error='SECRET',linkages=['SECRET'])
  out=r.comparator_summary(v,0);self.assertNotIn('SECRET',json.dumps(out));self.assertNotIn('first_differences',out)
  for key,value in [('matched_bytes',884),('normalization_complete',False),('size',1454)]:
   x=dict(v);x[key]=value
   with self.assertRaises(ValueError):r.comparator_summary(x,0)
  with self.assertRaises(ValueError):r.comparator_summary(v,1)
 def test_copies_raw_inputs_and_rejects_drift(self):
  with tempfile.TemporaryDirectory() as t:
   root=P(t);repo=root/'repo';repo.mkdir();m=self.manifest();pins={}
   for rel in r.input_pins(m):
    p=repo/rel;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(b'raw\r\n\x00'+rel.encode());pins[rel]=r.sha(p)
   m['source_files']={k:pins[k] for k in m['source_files']};m['tool_pins_from_prior_verified_packet']={k:pins[k] for k in m['tool_pins_from_prior_verified_packet']};m['additional_remote_pins']={k:pins[k] for k in m['additional_remote_pins']};m['canonical_config_sha256']=pins['config/match-units.toml'];m['target_sha256']=pins['resources/th10.exe']
   overlay=root/'overlay';(overlay/'src').mkdir(parents=True);(overlay/'src/BulletTransform.cpp').write_bytes((ROOT/'src/BulletTransform.cpp').read_bytes());r.copy_inputs(repo,root/'candidate',m,overlay)
   for rel,digest in pins.items():self.assertEqual(r.sha(root/'candidate'/rel),r.OVERLAYS.get(rel,digest));self.assertEqual(r.sha(repo/rel),digest)
   with patch.object(r,'validate') as validate_base:
    r.postcheck(repo,root/'candidate',m);validate_base.assert_called_once_with(repo,m)
   changed=root/'candidate/src/AnmManager.cpp';saved=changed.read_bytes();changed.write_bytes(b'candidate drift')
   with patch.object(r,'validate'),self.assertRaises(ValueError):r.postcheck(repo,root/'candidate',m)
   changed.write_bytes(saved);extra=root/'candidate/src/Extra.cpp';extra.write_bytes(b'')
   with patch.object(r,'validate'),self.assertRaises(ValueError):r.postcheck(repo,root/'candidate',m)
   extra.unlink();frozen=overlay/'src/BulletTransform.cpp';saved=frozen.read_bytes();frozen.write_bytes(b'frozen overlay drift')
   with patch.object(r,'validate'),self.assertRaises(ValueError):r.postcheck(repo,root/'candidate',m)
   frozen.write_bytes(saved)
   (repo/'src/BulletTransform.cpp').write_bytes(b'drift')
   with self.assertRaises(ValueError):r.copy_inputs(repo,root/'another',m,overlay)
 def test_mock_worker_one_fixed_call(self):
  with tempfile.TemporaryDirectory() as t:
   run=P(t);tree=run/'candidate';build=tree/'build/match-linked/BulletRuntimeUpdate';m=self.manifest();record={}
   def copy_inputs(*a):
    build.mkdir(parents=True);(tree/'scripts').mkdir();(tree/'scripts/ltcg_link.py').touch();(tree/'scripts/linked_image.py').touch()
   def cold(*args):
    record['args']=args;paths={k:build/('source.'+ext) for k,ext in [('image','exe'),('map','map'),('pdb','pdb')]}
    for p in paths.values():p.write_bytes(b'synthetic fixture, not PE')
    obj=build/'source.ltcg.obj';obj.write_bytes(b'fixture');return dict(paths,objects=[obj],unresolved_symbols=[])
   l=types.SimpleNamespace(__file__=str(tree/'scripts/ltcg_link.py'),ROOT=tree,LTCG_PROFILE=r.PROFILE[:-1],LINK_PROFILE=r.LINK,HARNESS_KIND='unresolved-symbol-data-anchors-v1',tool_environment=lambda:(P('/fake/link.exe'),{}),cold_link=Mock(side_effect=cold))
   li=types.SimpleNamespace(__file__=str(tree/'scripts/linked_image.py'),linked_functions=lambda *a:None,PEImage=lambda *a:None,map_publics=lambda *a:[],map_statics=lambda *a:[],pdb_section_contributions=lambda *a:{})
   def compare(*args,**kwargs):
    v=self.exact();v['image_sha256']=r.sha(build/'source.exe');kwargs['stdout'].write(json.dumps(v).encode());return types.SimpleNamespace(returncode=0)
   measure=lambda li,image,maps,pdb,build,spec:dict(complete=True,size=spec['target_size'],sha256='a'*64,target_size=spec['target_size'],size_equal=True,all_required_attribution_gates=True)
   with patch.object(r,'validate'),patch.object(r,'postcheck'),patch.object(r,'copy_inputs',side_effect=copy_inputs),patch.object(r.os,'nice'),patch.object(r.resource,'getrlimit',return_value=(-1,-1)),patch.object(r.resource,'setrlimit') as set_limit,patch.object(r.os,'sched_setaffinity'),patch.object(r.importlib,'import_module',side_effect=lambda name:l if name=='ltcg_link' else li),patch.object(r,'measure',side_effect=measure),patch.object(r.subprocess,'run',side_effect=compare):
    self.assertEqual(r.worker(P('/fake/repo'),run,m),0)
   args=record['args'];set_limit.assert_called_once_with(r.resource.RLIMIT_CPU,(120,120));self.assertEqual(l.cold_link.call_count,1);self.assertEqual(args[0],tree/'src/BulletRuntime.cpp');self.assertEqual(args[1],build);self.assertEqual(args[5],r.PROFILE);self.assertEqual(args[6],[tree/x for x in r.SUPPORT]);self.assertEqual(args[7],[tree/'src/BulletTransform.cpp']);self.assertTrue(r.status(run,'worker-receipt.json')['baseline_valid']);self.assertFalse((run/'receipt.json').exists())
 def test_timeout_group_cleanup_and_no_retry(self):
  with tempfile.TemporaryDirectory() as t:
   root=P(t);packet=root/'packet';packet.mkdir();(packet/'run.py').write_bytes(b'fixture');(packet/'manifest.proposed.json').write_text(json.dumps(self.manifest()));(packet/'src').mkdir();(packet/'src/BulletTransform.cpp').write_bytes((ROOT/'src/BulletTransform.cpp').read_bytes());run=root/'run'
   process=Mock(pid=12345,returncode=-9);process.wait.side_effect=[subprocess.TimeoutExpired('fixture',150),None,None]
   with patch.object(r,'validate'),patch.object(r,'clean'),patch.object(r,'postcheck'),patch.object(r.subprocess,'Popen',return_value=process),patch.object(r.os,'killpg') as kill:
    code=r.start(root,packet,run,self.manifest(),r.sha(packet/'run.py'),r.sha(packet/'manifest.proposed.json'));self.assertEqual(code,1);self.assertEqual(kill.call_count,2);self.assertEqual(r.status(run)['state'],'timed_out')
    with self.assertRaises(FileExistsError):r.start(root,packet,run,self.manifest(),r.sha(packet/'run.py'),r.sha(packet/'manifest.proposed.json'))
   self.assertTrue((run/'exit.json').exists())
 def test_supervisor_failure_paths_durable(self):
  for mode in ('copy_failure','spawn_failure','sigterm','contradictory_exit'):
   with self.subTest(mode=mode),tempfile.TemporaryDirectory() as t:
    root=P(t);packet=root/'packet';packet.mkdir();(packet/'run.py').write_bytes(b'fixture');(packet/'manifest.proposed.json').write_text(json.dumps(self.manifest()));(packet/'src').mkdir();(packet/'src/BulletTransform.cpp').write_bytes((ROOT/'src/BulletTransform.cpp').read_bytes());run=root/'run'
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
   root=P(t);packet=root/'packet';packet.mkdir();(packet/'run.py').write_bytes(b'fixture');(packet/'manifest.proposed.json').write_text(json.dumps(self.manifest()));(packet/'src').mkdir();(packet/'src/BulletTransform.cpp').write_bytes((ROOT/'src/BulletTransform.cpp').read_bytes());run=root/'run';process=Mock(pid=12345,returncode=1);calls=[]
   def wait(*args,**kwargs):
    calls.append(1)
    if len(calls)==1:r.write(run/'worker-receipt.json',{'state':'failed','stage':'cold_link','error_class':'ValueError','cold_link_calls':1,'runtime_executed':False,'transform_exactness_tested':False})
    return 1
   process.wait.side_effect=wait
   with patch.object(r,'validate'),patch.object(r,'clean'),patch.object(r,'postcheck'),patch.object(r.subprocess,'Popen',return_value=process),patch.object(r.os,'killpg'):
    self.assertEqual(r.start(root,packet,run,self.manifest(),r.sha(packet/'run.py'),r.sha(packet/'manifest.proposed.json')),1)
   original=json.loads((run/'worker-receipt.json').read_text());out=r.status(run);self.assertEqual(out['error_class'],'ValueError');self.assertEqual(out['stage'],'cold_link');self.assertEqual(out['cold_link_calls'],1);self.assertNotIn('runner_sha256',original)
 def test_worker_and_supervisor_errors_both_survive(self):
  for mode in ('interrupted','cleanup_failed'):
   with self.subTest(mode=mode),tempfile.TemporaryDirectory() as t:
    root=P(t);packet=root/'packet';packet.mkdir();(packet/'run.py').write_bytes(b'fixture');(packet/'manifest.proposed.json').write_text(json.dumps(self.manifest()));(packet/'src').mkdir();(packet/'src/BulletTransform.cpp').write_bytes((ROOT/'src/BulletTransform.cpp').read_bytes());run=root/'run';process=Mock(pid=12345,returncode=1)
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
   root=P(t);r.write(root/'receipt.json',{'state':'completed','guard':dict(self.exact(),exit_ok=True,first_differences=['secret'])})
   with self.assertRaises(ValueError):r.status(root)
 def test_optimized_runner_refused(self):
  p=subprocess.run([sys.executable,'-O',str(ROOT/'run.py'),'--help'],capture_output=True);self.assertNotEqual(p.returncode,0)
if __name__=='__main__':unittest.main()
