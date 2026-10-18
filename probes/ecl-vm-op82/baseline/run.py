#!/usr/bin/env python3
"""One reviewed isolated canonical baseline. Start never retries a claimed run."""
import argparse,hashlib,importlib,json,os,pathlib,re,resource,shutil,signal,subprocess,sys,time,tomllib,traceback
sys.dont_write_bytecode=True
P=pathlib.Path
HEAD='a4edda20e434cd40ee99ea3b6b68673a80c3e3be'
SUPPORT=['src/Enemy.cpp']
PROFILE=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/Isrc','/GL']
LINK=['/ltcg','/nodefaultlib','/subsystem:console','/incremental:no','/fixed:no','/base:0x00400000','/debug','/debugtype:cv','/opt:noref','/opt:noicf','/safeseh:no']
ENTRY='?EnemyRuntimeUpdate@@YGHPAUEnemyRuntimeView@@@Z'
OWNERS={'run':{'symbol':'?Run@EclVmContext@@QAEHM@Z','object':'source.ltcg.obj','target_address':0x44e1a0,'target_size':7020}}
GUARDS={'ecl-vm-host-run':146,'ecl-vm-spawn-thread':142}
OUTPUT='build/match-linked/EclVmHostRunEnemy'
MSVC='/home/pentester/coding/codex_ida/th10-reconstruction/th10/.tools/msvc710-sp1'
WINE='/home/pentester/coding/codex_ida/th10-reconstruction/th10/.tools/wineprefix-msvc710-sp1'
WALL=150
RESOURCE_POLICY={'cpu_seconds':120,'wall_seconds':150,'nice_increment':10,'cpu_affinity_count':1,'address_space':'inherit_unlimited_without_setrlimit'}

def require(value,message):
 if not value:raise ValueError(message)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def safe(root,rel):
 r=P(rel);require(r.parts and not r.is_absolute() and '..' not in r.parts,'unsafe path');p=root
 for part in r.parts:
  p=p/part;require(not p.is_symlink(),'symlink input')
 require(p.is_file(),'missing file');return p
def no_symlinks(p):
 require(p.is_absolute(),'absolute path required')
 for q in (p,*p.parents):require(not q.is_symlink(),'symlink directory')
def write(p,value):
 tmp=p.with_suffix(p.suffix+'.tmp');payload=(json.dumps(value,sort_keys=True,indent=2)+'\n').encode()
 with tmp.open('wb') as stream:stream.write(payload);stream.flush();os.fsync(stream.fileno())
 os.replace(tmp,p)
 fd=os.open(str(p.parent),os.O_RDONLY)
 try:os.fsync(fd)
 finally:os.close(fd)
def git(repo,*args):return subprocess.check_output(['git','-C',str(repo),*args],timeout=10)
def clean(repo):
 require(git(repo,'rev-parse','HEAD').decode().strip()==HEAD,'HEAD drift')
 require(not git(repo,'status','--porcelain=v1','--untracked-files=all'),'production dirty')
def env_gate(environment):
 require(not any(k.upper() in {'CL','_CL_','WINE','WINESERVER','WINELOADER'} for k in environment),'hidden compiler or Wine overrides')
def input_pins(m):
 result=dict(m['source_files'])
 for key,value in m['tool_pins'].items():require(key not in result,'duplicate pin');result[key]=value
 result['config/match-units.toml']=m['canonical_config_sha256'];result['resources/th10.exe']=m['target_sha256'];return result

def manifest_gate(m):
 require(m['execution_ready'] is True,'packet not execution-ready')
 require(m['resource_policy']==RESOURCE_POLICY,'resource policy drift')
 require(m['base_commit']==HEAD and len(m['source_files'])==83,'wrong source identity')
 require(m['owners']==OWNERS and m['cold_link_calls']==1 and not m.get('sweeps',False) and not m.get('source_changes',False) and not m.get('overlays',{}) and not m['runtime_execution'],'scope drift')
 require(m['acceptance_authority']=='none','acceptance authority')
 require(m['python_dependencies']=={'capstone':'5.0.6'} and m['outer_timeout_seconds']==240,'dependency or outer budget drift')
 u=m['graph'];require(u['source']=='src/EclVm.cpp' and u['support_sources']==SUPPORT and m['normal_support_sources']==[],'graph drift')
 require(u['profile']==PROFILE and u['link_profile']==LINK and u['harness']=='unresolved-symbol-data-anchors-v1','profile drift')
 require(u['entry_symbol']==ENTRY and u['output_dir']==OUTPUT,'entry/output drift')
 require(set(m['guards'])==set(GUARDS),'guard set drift')
 for name,size in GUARDS.items():
  guard=m['guards'][name]
  require(guard['size']==size and all(guard.get(k)==v for k,v in u.items()),'guard recipe drift')
 expected={'scripts/ltcg_link.py','scripts/linked_image.py','scripts/compile-probe.sh','scripts/run-headless-wine.sh','config/tools.lock.toml','scripts/compare-linked-function.py','config/target.toml'}
 require(set(m['tool_pins'])==expected,'driver pin set drift')
 for rel,digest in input_pins(m).items():
  require(re.fullmatch(r'[A-Za-z0-9_./-]+',rel) is not None and re.fullmatch('[0-9a-f]{64}',digest) is not None,'malformed pin')
 require(all(k.startswith('src/') for k in m['source_files']),'source path outside src')

def dependency_gate():
 import capstone
 require(capstone.__version__=='5.0.6','capstone module version drift')

def validate(repo,m):
 manifest_gate(m);clean(repo);env_gate(os.environ);dependency_gate()
 tracked=git(repo,'ls-files','-z','--','src').decode().split('\0');tracked=[x for x in tracked if x]
 require(len(tracked)==83 and set(tracked)==set(m['source_files']),'tracked source set drift')
 for rel,digest in input_pins(m).items():require(sha(safe(repo,rel))==digest,'input pin drift: '+rel)
 units=tomllib.loads(safe(repo,'config/match-units.toml').read_text())['units']
 for name in GUARDS:require(units[name]==m['guards'][name],'canonical unit drift')
 verify_closure(repo,m)
 for root in (P(MSVC),P(WINE)):require(root.is_dir(),'tool environment missing')

def verify_closure(root,m):
 seen=set();edges={};external=set();todo=['src/EclVm.cpp',*SUPPORT]
 while todo:
  rel=todo.pop()
  if rel in seen:continue
  seen.add(rel);edges[rel]=[]
  for line in safe(root,rel).read_bytes().splitlines():
   if not re.match(rb'^\s*#\s*include\b',line):continue
   match=re.match(rb'^\s*#\s*include\s*([<"])([^>"\r\n]+)[>"]',line)
   require(match is not None,'nonliteral include');name=match[2].decode('ascii')
   choices=[str(P(rel).parent/name),str(P('src')/name)]
   found=next((x for x in choices if x in m['source_files']),None)
   if found:edges[rel].append(found);todo.append(found)
   else:require(match[1]==b'<','unresolved quoted include');external.add(name)
 actual={'files':{x:m['source_files'][x] for x in sorted(seen)},'edges':{k:sorted(set(v)) for k,v in sorted(edges.items())},'external_toolchain_headers':sorted(external)}
 require(all(m['source_include_closure'][k]==v for k,v in actual.items()),'include closure drift')

def copy_inputs(repo,tree,m):
 tree.mkdir()
 for rel,digest in input_pins(m).items():
  src=safe(repo,rel);require(sha(src)==digest,'copy source drift');dst=tree/rel;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(src,dst)
  if rel.endswith('.sh'):dst.chmod(0o755)
  require(sha(dst)==digest,'copy changed raw bytes')

def postcheck(repo,tree,m):
 validate(repo,m)
 for rel,digest in input_pins(m).items():require(sha(safe(tree,rel))==digest,'candidate input drift')
 actual={str(p.relative_to(tree)) for p in (tree/'src').rglob('*') if p.is_file()}
 require(actual==set(m['source_files']),'candidate source set drift')
 verify_closure(tree,m)

def comparator_summary(value,returncode,size):
 require(type(value) is dict and value.get('result') in {'exact','mismatch','error'},'invalid comparator result')
 out={'result':value['result']}
 for key in ('size','compared_size','matched_bytes','matched_compared_bytes'):
  if key in value:require(type(value[key]) is int and 0<=value[key]<=10000000,'invalid comparator integer');out[key]=value[key]
 for key in ('normalization_complete','raw_equal'):
  if key in value:require(type(value[key]) is bool,'invalid comparator boolean');out[key]=value[key]
 if 'image_sha256' in value:require(re.fullmatch('[0-9a-f]{64}',value['image_sha256']) is not None,'invalid comparator hash');out['image_sha256']=value['image_sha256']
 require(type(returncode) is int,'invalid comparator exit')
 out['exit_ok']=returncode==0
 if value['result']=='exact':require(returncode==0 and out.get('size')==size and out.get('compared_size')==size and out.get('matched_bytes')==size and out.get('matched_compared_bytes')==size and out.get('normalization_complete') is True,'incomplete exact guard')
 return out

# Corrected same-start attribution predicates; no size-based selection.
def predicates(l,image,maps,pdb,build,spec):
 def check(fn):
  try:return bool(fn())
  except (ValueError,KeyError,TypeError,IndexError,AssertionError):return False
 out={}
 expected=l._object_path(build/spec['object'])
 out['all_module_object_paths_normalizable']=check(lambda:all(l._object_path(x['object']) for x in pdb['modules']))
 modules=[x for x in pdb['modules'] if l._object_basename(x['object'])==spec['object'].lower()]
 out['basename_prefilter_unique_module']=len(modules)==1
 out['prefilter_module_exact_path']=len(modules)==1 and check(lambda:l._object_path(modules[0]['object'])==expected)
 rows=[x for x in maps if x['symbol']==spec['symbol']]
 out['unique_requested_symbol']=len(rows)==1
 if len(rows)!=1:return out
 row=rows[0];start=(row['section_number'],row['section_offset'])
 out['unique_same_start_symbol']=sum((x['section_number'],x['section_offset'])==start for x in maps)==1
 cs=[x for x in pdb['contributions'] if (x['section_number'],x['section_offset'])==start]
 out['unique_same_start_contribution']=len(cs)==1
 if len(cs)!=1:return out
 c=cs[0];size=c['size'];section=image.section(start[0]);required=l.IMAGE_SCN_CNT_CODE|l.IMAGE_SCN_MEM_EXECUTE
 out.update(positive_contribution_size=type(size) is int and size>0,
  contribution_exact_object_path=check(lambda:l._object_path(c['object'])==expected),
  contribution_module_index_equal=len(modules)==1 and c['module_index']==modules[0]['index'],
  contribution_module_name_equal=len(modules)==1 and c['module']==modules[0]['module'],
  map_object_basename_equal=l._object_basename(row['object'])==spec['object'].lower(),
  map_object_fullpath_if_present=not any(x in str(row['object']) for x in ('/','\\',':')) or check(lambda:l._object_path(row['object'])==expected),
  function_symbol_flag='f' in row['flags'],
  contribution_code_executable=c['characteristics']&required==required,
  section_code_executable=section.characteristics&required==required,
  virtual_extent_covered=0<=start[1] and start[1]+size<=section.virtual_size,
  no_interior_function_symbol=not any(x['section_number']==start[0] and start[1]<x['section_offset']<start[1]+size and 'f' in x['flags'] for x in maps),
  map_address_agrees=row['address']==image.image_base+section.rva+start[1],
  no_overlapping_contribution=not any(x is not c and x['section_number']==start[0] and x['size']>0 and x['section_offset']<start[1]+size and start[1]<x['section_offset']+x['size'] for x in pdb['contributions']),
  raw_extent_covered=check(lambda:len(image.read_section(start[0],start[1],size))==size))
 return out

def measure(l,image,maps,pdb,build,spec):
 gates=predicates(l,image,maps,pdb,build,spec)
 expected={'basename_prefilter_unique_module','prefilter_module_exact_path','unique_requested_symbol','unique_same_start_symbol','unique_same_start_contribution','positive_contribution_size','contribution_exact_object_path','contribution_module_index_equal','contribution_module_name_equal','map_object_basename_equal','map_object_fullpath_if_present','function_symbol_flag','contribution_code_executable','section_code_executable','virtual_extent_covered','no_interior_function_symbol','map_address_agrees','no_overlapping_contribution','raw_extent_covered'}
 require(set(gates)==expected|{'all_module_object_paths_normalizable'},'incomplete attribution predicates')
 require(all(gates[k] for k in expected),'complete owner attribution failed')
 row=next(x for x in maps if x['symbol']==spec['symbol'])
 c=next(x for x in pdb['contributions'] if (x['section_number'],x['section_offset'])==(row['section_number'],row['section_offset']))
 payload=image.read_section(c['section_number'],c['section_offset'],c['size'])
 return {'complete':True,'size':c['size'],'sha256':hashlib.sha256(payload).hexdigest(),'target_size':spec['target_size'],'size_equal':c['size']==spec['target_size'],'all_required_attribution_gates':True}

def validate_windows_path_output(raw,path):
 require(type(raw) is bytes and 0<len(raw)<=8192,'winepath output size')
 value=raw.decode('ascii')
 if value.endswith('\r\n'):value=value[:-2]
 elif value.endswith('\n'):value=value[:-1]
 expected='Z:'+str(path.resolve()).replace('/','\\')
 require(value.lower()==expected.lower() and not any(ord(c)<32 for c in value),'winepath empty or wrong path')
 return value

def checked_windows_path(path,environment):
 require(shutil.which('winepath',path=environment.get('PATH'))=='/usr/bin/winepath','PATH winepath identity')
 require(not any(k in os.environ for k in ('WINE','WINESERVER','WINELOADER')),'Wine override')
 wp=P('/usr/bin/winepath');require(str(wp.resolve())=='/usr/lib/wine/wineapploader' and sha(wp.resolve())=='3e3fa1050f95b39cf90e21f03e6e8cda4415a480db696719ef137e3c7a86bb54','winepath identity')
 # Keep wrapper argv0; bound output on disk instead of exposing it.
 import tempfile
 with tempfile.TemporaryFile() as output:
  done=subprocess.run([str(wp),'-w',str(path.resolve())],env=environment,stdout=output,stderr=subprocess.DEVNULL,timeout=5)
  output.seek(0);raw=output.read(8193)
 require(done.returncode==0 and len(raw)<=8192,'winepath failure')
 return validate_windows_path_output(raw,path)


def worker(repo,run,m):
 started=time.monotonic();tree=run/'candidate';out={'state':'failed','acceptance_authority':'none','runtime_executed':False,'owner_exactness_tested':False,'base_commit':HEAD,'cold_link_calls':0,'stage':'validation','production_clean':False,'postrun_pins_valid':False}
 try:
  validate(repo,m);out['stage']='resource_setup';os.nice(10)
  require(resource.getrlimit(resource.RLIMIT_AS)==(-1,-1),'inherited virtual-address cap')
  resource.setrlimit(resource.RLIMIT_CPU,(120,120));out['resource_policy']=dict(RESOURCE_POLICY)
  if hasattr(os,'sched_getaffinity'):
   cpus=os.sched_getaffinity(0);require(bool(cpus),'no CPU');os.sched_setaffinity(0,{min(cpus)})
  out['stage']='input_copy';copy_inputs(repo,tree,m)
  out['stage']='driver_import'
  os.environ['TH10_MSVC71_ROOT']=MSVC;os.environ['TH10_WINEPREFIX']=WINE;env_gate(os.environ)
  sys.path.insert(0,str(tree/'scripts'));l=importlib.import_module('ltcg_link');li=importlib.import_module('linked_image')
  require(P(l.__file__).resolve()==(tree/'scripts/ltcg_link.py').resolve() and P(li.__file__).resolve()==(tree/'scripts/linked_image.py').resolve(),'import path drift')
  require(l.ROOT==tree and l.LTCG_PROFILE==PROFILE and l.LINK_PROFILE==LINK and l.HARNESS_KIND=='unresolved-symbol-data-anchors-v1','driver constants drift')
  li.verify_capstone(tomllib.loads(safe(tree,'config/tools.lock.toml').read_text())['capstone'])
  out['stage']='tool_environment'
  linker,environment=l.tool_environment();env_gate(environment);env_gate(os.environ)
  checked_windows_path(tree,environment)
  write(run/'phase.json',{'state':'building','pid':os.getpid()})
  out['cold_link_calls']=1;out['stage']='cold_link'
  previous_windows_path=l.windows_path
  try:
   l.windows_path=checked_windows_path
   built=l.cold_link(tree/'src/EclVm.cpp',tree/OUTPUT,ENTRY,linker,environment,list(PROFILE),[tree/x for x in SUPPORT],[])
  finally:l.windows_path=previous_windows_path
  out['build_success']=True;out['artifact_sha256']={k:sha(P(built[k])) for k in ('image','map','pdb')};out['object_sha256']={P(x).name:sha(P(x)) for x in built['objects']};out['unresolved_count']=len(built['unresolved_symbols'])
  out['stage']='owner_measurement'
  li.linked_functions(built['image'],built['map'],built['pdb'])
  image=li.PEImage(built['image']);maps=li.map_publics(built['map'])+li.map_statics(built['map']);pdb=li.pdb_section_contributions(built['pdb']);build=P(built['image']).parent
  out['owners']={}
  for name,spec in OWNERS.items():
   try:out['owners'][name]=measure(li,image,maps,pdb,build,spec)
   except (AssertionError,ValueError,KeyError,TypeError,IndexError):out['owners'][name]={'complete':False,'target_size':spec['target_size'],'attribution_failed':True}
  out['stage']='guard_replay'
  out['guards']={}
  for name,size in GUARDS.items():
   private=run/(name+'-private.json')
   with private.open('xb') as log:
    compared=subprocess.run([sys.executable,str(tree/'scripts/compare-linked-function.py'),'--unit',name,'--json'],cwd=tree,stdout=log,stderr=subprocess.STDOUT,env=environment)
   require(private.stat().st_size<=4*1024*1024,'comparator output oversized')
   guard=comparator_summary(json.loads(private.read_text()),compared.returncode,size)
   require(guard.get('image_sha256')==out['artifact_sha256']['image'],'guard used different or missing image')
   out['guards'][name]=guard
   require(all(sha(P(built[k]))==out['artifact_sha256'][k] for k in ('image','map','pdb')),'artifact drift after replay')
  out['baseline_valid']=all(g['result']=='exact' for g in out['guards'].values()) and all(v['complete'] for v in out['owners'].values())
  out['state']='completed';out['stage']='complete'
 except BaseException as exc:
  traceback.print_exc();out['error_class']=type(exc).__name__
 finally:
  try:postcheck(repo,tree,m);out['postrun_pins_valid']=True;out['production_clean']=True
  except BaseException as exc:out.update(state='failed',baseline_valid=False,postcheck_error_class=type(exc).__name__)
  out['elapsed_seconds']=round(time.monotonic()-started,3);write(run/'worker-receipt.json',out)
 return 0 if out['state']=='completed' else 1

def scope(repo,packet,run):
 for p in (repo,packet,run):no_symlinks(p)
 require(repo.is_dir() and packet.is_dir(),'missing repo/packet')
 require(run.is_relative_to(repo/'.analysis') and run!=repo/'.analysis','run must be below .analysis')
 require(not packet.is_relative_to(run) and not run.is_relative_to(packet),'packet overlaps run')
 require(subprocess.run(['git','-C',str(repo),'check-ignore','-q',str(run)],timeout=10).returncode==0,'run not ignored')

def status(run,filename='receipt.json'):
 require(filename in {'receipt.json','worker-receipt.json'},'invalid receipt selector')
 p=run/filename
 if not p.exists():
  out={'state':'pending_or_interrupted','restart_allowed':False}
  if (run/'phase.json').exists():
   q=safe(run,'phase.json');require(q.stat().st_size<=4096,'phase oversized');v=json.loads(q.read_text());require(v.get('state') in {'claimed','building'} and type(v.get('pid')) is int,'phase invalid');out['phase']={'state':v['state'],'pid':v['pid']}
  return out
 p=safe(run,filename);require(p.stat().st_size<=65536,'receipt oversized');v=json.loads(p.read_text());require(v.get('state') in {'completed','failed','timed_out'},'receipt state invalid')
 out={'state':v['state'],'restart_allowed':False,'acceptance_authority':'none'}
 if 'resource_policy' in v:
  require(v['resource_policy']==RESOURCE_POLICY,'receipt resource policy drift');out['resource_policy']=dict(RESOURCE_POLICY)
 if 'stage' in v:
  require(v['stage'] in {'validation','resource_setup','input_copy','driver_import','tool_environment','cold_link','owner_measurement','guard_replay','complete'},'bad stage');out['stage']=v['stage']
 for k in ('runtime_executed','owner_exactness_tested','production_clean','postrun_pins_valid','build_success','baseline_valid','process_group_terminated'):
  if k in v:require(type(v[k]) is bool,'bad status bool');out[k]=v[k]
 for k in ('cold_link_calls','unresolved_count'):
  if k in v:require(type(v[k]) is int and 0<=v[k]<10000000,'bad status count');out[k]=v[k]
 for k in ('elapsed_seconds',):
  if k in v:require(type(v[k]) in (float,int) and 0<=v[k]<1000000,'bad status duration');out[k]=v[k]
 for k in ('error_class','postcheck_error_class','supervisor_error_class','cleanup_error_class'):
  if k in v:require(re.fullmatch('[A-Za-z]{1,80}',v[k]) is not None,'bad error class');out[k]=v[k]
 for k in ('base_commit','runner_sha256','manifest_sha256'):
  if k in v:require(re.fullmatch('[0-9a-f]{40}|[0-9a-f]{64}',v[k]) is not None,'bad identity');out[k]=v[k]
 for k in ('artifact_sha256','object_sha256'):
  if k in v:
   require(type(v[k]) is dict and len(v[k])<=10,'bad artifact hashes')
   require(all(re.fullmatch('[A-Za-z0-9_.-]{1,100}',a) and re.fullmatch('[0-9a-f]{64}',b) for a,b in v[k].items()),'bad artifact pin');out[k]=v[k]
 if 'guards' in v:
  require(type(v['guards']) is dict and set(v['guards'])<=set(GUARDS),'bad guards');out['guards']={}
  for name,guard in v['guards'].items():
   allowed={'result','size','compared_size','matched_bytes','matched_compared_bytes','image_sha256','normalization_complete','raw_equal','exit_ok'}
   require(set(guard)<=allowed and type(guard.get('exit_ok')) is bool,'bad guard fields')
   out['guards'][name]=comparator_summary(guard,0 if guard['exit_ok'] else 1,GUARDS[name])
 if 'owners' in v:
  require(set(v['owners'])==set(OWNERS),'owner set drift');out['owners']={}
  for name,row in v['owners'].items():
   allowed={'complete','size','sha256','target_size','size_equal','all_required_attribution_gates','attribution_failed'};require(set(row)<=allowed and type(row.get('complete')) is bool and row.get('target_size')==OWNERS[name]['target_size'],'bad owner schema')
   if row['complete']:
    require(type(row.get('size')) is int and 0<row['size']<10000000 and re.fullmatch('[0-9a-f]{64}',row.get('sha256','')) is not None and row.get('all_required_attribution_gates') is True and row.get('size_equal')==(row['size']==row['target_size']),'bad complete owner')
   else:require(set(row)=={'complete','target_size','attribution_failed'} and row['attribution_failed'] is True,'bad failed owner')
   out['owners'][name]=row
 return out

def terminate_group(process):
 try:os.killpg(process.pid,signal.SIGTERM)
 except ProcessLookupError:pass
 try:process.wait(timeout=3)
 except subprocess.TimeoutExpired:pass
 # Kill the group even if its leader exited, to remove remaining descendants.
 try:os.killpg(process.pid,signal.SIGKILL)
 except ProcessLookupError:pass
 process.wait(timeout=3)

def start(repo,packet,run,m,runner_hash,manifest_hash):
 validate(repo,m);run.mkdir(parents=True,exist_ok=False)
 started=time.monotonic();process=None;timed_out=False;error=None;cleanup_error=None;out=None
 previous={sig:signal.getsignal(sig) for sig in (signal.SIGTERM,signal.SIGINT)}
 def interrupted(signum,frame):raise InterruptedError('supervisor interrupted')
 try:
  for sig in previous:signal.signal(sig,interrupted)
  write(run/'phase.json',{'state':'claimed','pid':os.getpid(),'runner_sha256':runner_hash,'manifest_sha256':manifest_hash})
  shutil.copyfile(packet/'manifest.proposed.json',run/'manifest.json');shutil.copyfile(packet/'run.py',run/'runner.py')
  require(sha(run/'runner.py')==runner_hash and sha(run/'manifest.json')==manifest_hash,'freeze copy drift')
  with (run/'private.log').open('xb') as log:
   process=subprocess.Popen([sys.executable,str(run/'runner.py'),'worker','--repo',str(repo),'--packet',str(packet),'--run',str(run),'--expected-runner-sha256',runner_hash,'--expected-manifest-sha256',manifest_hash],stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
   try:process.wait(timeout=WALL)
   except subprocess.TimeoutExpired:timed_out=True
 except BaseException as exc:error=type(exc).__name__
 finally:
  # A second termination signal must not interrupt descendant cleanup/receipt writes.
  for sig in previous:signal.signal(sig,signal.SIG_IGN)
  try:
   if process is not None:
    try:terminate_group(process)
    except BaseException as exc:cleanup_error=type(exc).__name__
   if (run/'worker-receipt.json').is_file():
    try:
     candidate=status(run,'worker-receipt.json');out=candidate
     if timed_out:out['state']='timed_out'
     elif error is not None or cleanup_error is not None or process is None or process.returncode!=0 or candidate['state']!='completed':out['state']='failed'
    except BaseException as exc:error=type(exc).__name__
   if out is None:
    out={'state':'timed_out' if timed_out else 'failed','acceptance_authority':'none','runtime_executed':False,'owner_exactness_tested':False,'elapsed_seconds':round(time.monotonic()-started,3)}
    if error:out['error_class']=error
    if cleanup_error:out['cleanup_error_class']=cleanup_error
   if error:out['supervisor_error_class']=error
   if cleanup_error:out['cleanup_error_class']=cleanup_error
   out.update(runner_sha256=runner_hash,manifest_sha256=manifest_hash,process_group_terminated=process is not None and cleanup_error is None)
   try:clean(repo);out['production_clean']=True
   except BaseException:out['production_clean']=False;out['state']='failed'
   try:postcheck(repo,run/'candidate',m);out['postrun_pins_valid']=True
   except BaseException as exc:out.update(postrun_pins_valid=False,postcheck_error_class=type(exc).__name__);out['state']='failed' if not timed_out else out['state']
   if out['state']!='completed':out['baseline_valid']=False
   write(run/'receipt.json',out)
   write(run/'exit.json',{'exit_code':0 if out['state']=='completed' else 1,'worker_returncode':process.returncode if process is not None else None})
  finally:
   for sig,handler in previous.items():signal.signal(sig,handler)
 return 0 if out['state']=='completed' else 1

def main():
 ap=argparse.ArgumentParser();ap.add_argument('action',choices=['start','status','worker']);ap.add_argument('--repo',type=P,required=True);ap.add_argument('--packet',type=P,required=True);ap.add_argument('--run',type=P,required=True);ap.add_argument('--expected-runner-sha256');ap.add_argument('--expected-manifest-sha256');a=ap.parse_args()
 repo=P(os.path.abspath(a.repo));packet=P(os.path.abspath(a.packet));run=P(os.path.abspath(a.run));scope(repo,packet,run)
 if a.action=='status':print(json.dumps(status(run),sort_keys=True));return 0
 require(a.expected_runner_sha256 and a.expected_manifest_sha256,'reviewed hashes required')
 script=safe(run,'runner.py') if a.action=='worker' else safe(packet,'run.py')
 manifest=safe(run,'manifest.json') if a.action=='worker' else safe(packet,'manifest.proposed.json')
 require(P(__file__).resolve()==script.resolve(),'unexpected runner location');require(sha(script)==a.expected_runner_sha256 and sha(manifest)==a.expected_manifest_sha256,'reviewed packet drift');m=json.loads(manifest.read_text())
 if a.action=='worker':
  with (run/'worker-claimed').open('xb') as marker:marker.write(b'one-shot\n')
  require(not (run/'receipt.json').exists(),'already terminal');claim=json.loads(safe(run,'phase.json').read_text());require(claim['state']=='claimed' and claim['runner_sha256']==a.expected_runner_sha256 and claim['manifest_sha256']==a.expected_manifest_sha256,'claim drift');return worker(repo,run,m)
 code=start(repo,packet,run,m,a.expected_runner_sha256,a.expected_manifest_sha256);print(json.dumps(status(run),sort_keys=True));return code
if __name__=='__main__':
 if not __debug__:raise RuntimeError('optimized Python disabled owner attribution checks')
 try:sys.exit(main())
 except BaseException as exc:
  if isinstance(exc,SystemExit):raise
  print(json.dumps({'state':'failed','error_class':type(exc).__name__,'restart_allowed':False}));sys.exit(1)
