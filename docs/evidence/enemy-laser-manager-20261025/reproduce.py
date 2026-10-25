#!/usr/bin/env python3
"""Reproduce one complete isolated laser-manager hypothesis; no acceptance shortcut."""
from pathlib import Path
import argparse,sys,subprocess,hashlib,json,os
ROOT=Path(__file__).resolve().parents[3];PACKET=Path(__file__).resolve().parent
p=argparse.ArgumentParser();p.add_argument('--trial',choices=['first','publication','literal'],required=True);p.add_argument('--output-dir',required=True);p.add_argument('--prepare-only',action='store_true');a=p.parse_args()
out=(ROOT/a.output_dir).resolve();assert out.is_relative_to(ROOT/'build') and not out.exists()
assert hashlib.sha256((ROOT/'resources/th10.exe').read_bytes()).hexdigest()=='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
base='2b0f3f5acb405036f78cc214f9d975123c5673fe';src=ROOT/'.analysis'/('laser-manager-replay-'+out.name);assert not src.exists();src.mkdir()
for name in subprocess.check_output(['git','ls-tree','-r','--name-only',base,'src'],cwd=ROOT).decode().splitlines():
 (src/Path(name).name).write_bytes(subprocess.check_output(['git','show',base+':'+name],cwd=ROOT))
for name in ['EnemyLaser.cpp','EnemyLaser.hpp']:
 f=src/name;f.write_text(f.read_text())
subprocess.run(['patch','--batch','-p1','-i',str(PACKET/(a.trial+'.patch'))],cwd=src,check=True)
if a.prepare_only:print(src);sys.exit(0)
assert os.getpriority(os.PRIO_PROCESS,0)>=10 and len(os.sched_getaffinity(0))==1
sys.path.insert(0,str(ROOT/'scripts'));from ltcg_link import cold_link,tool_environment
from linked_image import linked_functions
names=['EnemyEclDispatcher','EclVm','AnmManager','AnmVmCreate','EnemyLaser','Enemy','AnmVmId','RandomMath','AnmInt3Scale','ItemManager','Chain','GameErrorContext']
profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/I'+str(src),'/GL','/GS'];override={src/'AnmInt3Scale.cpp':profile+['/Ob1']}
def clean(v):
 if isinstance(v,dict):return {str(k):clean(x) for k,x in v.items()}
 if isinstance(v,(list,tuple)):return [clean(x) for x in v]
 return str(v) if isinstance(v,Path) else v
json.dumps(clean(override))
linker,env=tool_environment();r=cold_link(src/'EnemyLaserManager.cpp',out,'?GameCreateOwner41C290@@YAPAXXZ',linker,env,profile=profile,support_sources=[src/(n+'.cpp') for n in names],support_profiles=override)
(out/'native.json').write_text(json.dumps(clean(r),indent=2)+'\n');(out/'functions.json').write_text(json.dumps(clean(linked_functions(out/'source.exe',out/'source.map',out/'source.pdb')),indent=2)+'\n')
