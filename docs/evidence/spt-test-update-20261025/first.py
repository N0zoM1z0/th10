from pathlib import Path
import sys,json,hashlib,os,subprocess
root=Path.cwd();sys.path.insert(0,str(root/'scripts'))
import ltcg_link as l
from linked_image import linked_functions
p=root/'.analysis/spt-test-update-20261025';out=root/'build/spt-test-update-first-20261025';candidate=p/'candidate'
assert not out.exists();assert os.getpriority(os.PRIO_PROCESS,0)>=10;assert len(os.sched_getaffinity(0))==1
inputs=[root/x for x in subprocess.check_output(['git','ls-files','-z']).decode().split('\0') if x]+list(candidate.glob('*'))+list(root.glob('docs/evidence/d3d-adapter-format/*'))
frozen={str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs if p.is_file()};out.mkdir();(out/'frozen-inputs.json').write_text(json.dumps(frozen,indent=2)+'\n')
try:
 linker,env=l.tool_environment()
 r=l.cold_link(candidate/'SptTest.cpp',out,'?SptTestUpdateCallback@@YIHPAX@Z',linker,env,profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/I'+str(candidate.relative_to(root)),'/Isrc','/GL','/GS','/EHs'],support_sources=[candidate/(n+'.cpp') for n in ['AnmManager','Chain','ScriptViewerWorker','ScriptViewer','FrontEnd','Main','EclVm','Enemy']])
 (out/'native.json').write_text(json.dumps(r,indent=2,default=str)+'\n')
 f=linked_functions(out/'source.exe',out/'source.map',out/'source.pdb');(out/'functions.json').write_text(json.dumps(f,indent=2)+'\n')
 print(json.dumps([x for x in f['functions'] if any(k in x['symbol'] for k in ['SptTest','Run@EclVmHost','FindSubroutine@','Move@FrontEnd','IsRepeated@','SetNextGameMode@'])],indent=2))
finally:
 for path,digest in frozen.items():assert hashlib.sha256((root/path).read_bytes()).hexdigest()==digest,path
 (out/'freeze-pass.txt').write_text('all frozen inputs unchanged\n')
