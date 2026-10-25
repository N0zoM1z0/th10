from pathlib import Path
import sys,json,hashlib,os
root=Path.cwd();sys.path.insert(0,str(root/'scripts'))
import ltcg_link as l
from linked_image import linked_functions
out=root/'build/statistics-load-first-20261025'; candidate=root/'.analysis/gpt-dots/statistics-load-20261025/candidate'
assert not out.exists(), 'preserve original output'
assert os.getpriority(os.PRIO_PROCESS,0)>=10
assert len(os.sched_getaffinity(0))==1
inputs=[*sorted(candidate.glob('*')),*sorted(root.glob('src/*')),root/'scripts/ltcg_link.py',root/'config/tools.lock.toml',*sorted(root.glob('docs/evidence/d3d-adapter-format/*'))]
frozen={str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs if p.is_file()}
out.mkdir(parents=True)
(out/'frozen-inputs.json').write_text(json.dumps(frozen,indent=2)+'\n')
linker,env=l.tool_environment()
try:
 r=l.cold_link(candidate/'GameStatistics.cpp',out,'?CreateStatistics42AF20@@YGPAUGameStatisticsOwner@@XZ',linker,env,profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/I'+str(candidate.relative_to(root)),'/Isrc','/GL','/GS','/EHsc','/Gz'],support_sources=[root/'src/FileSystem.cpp',root/'src/Decompress.cpp'],normal_support_sources=[root/'src/Decompress.cpp'])
 (out/'native.json').write_text(json.dumps(r,indent=2,default=str)+'\n')
 f=linked_functions(out/'source.exe',out/'source.map',out/'source.pdb')
 (out/'functions.json').write_text(json.dumps(f,indent=2)+'\n')
 print(json.dumps([x for x in f['functions'] if any(k in x['symbol'] for k in ['Statistics','Decrypt','Decompress'])],indent=2))
finally:
 for path,digest in frozen.items():assert hashlib.sha256((root/path).read_bytes()).hexdigest()==digest,path
