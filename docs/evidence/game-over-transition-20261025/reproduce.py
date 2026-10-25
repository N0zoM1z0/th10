#!/usr/bin/env python3
"""Reproduce isolated GameOver source/native graph; no acceptance authority."""
import argparse,hashlib,json,os,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];PACKET=Path(__file__).resolve().parent
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--trial',choices=['first','ending'],required=True);p.add_argument('--output-dir',required=True);p.add_argument('--prepare-only',action='store_true');a=p.parse_args()
out=(ROOT/a.output_dir).resolve();assert out.is_relative_to(ROOT/'build') and not out.exists()
assert hashlib.sha256((ROOT/'resources/th10.exe').read_bytes()).hexdigest()=='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
for n,h in json.loads((PACKET/'source-inputs.json').read_text()).items():assert hashlib.sha256((ROOT/n).read_bytes()).hexdigest()==h,n
src=ROOT/'.analysis/gpt-dots'/(out.name+'-inputs');assert not src.exists();shutil.copytree(ROOT/'src',src)
for name in ['FrontEnd.cpp','ResultMenu.hpp','Player.cpp','Gui.cpp','GameManager.cpp']:
 file=src/name;file.write_text(file.read_text())
subprocess.run(['patch','-p1','--batch','-i',str(PACKET/'proposal.patch')],cwd=src,check=True)
if a.prepare_only:print('Prepared',src);sys.exit(0)
assert os.getpriority(os.PRIO_PROCESS,0)>=10
sys.path.insert(0,str(ROOT/'scripts'));from ltcg_link import cold_link,tool_environment
from linked_image import linked_functions
profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/I'+str(src),'/GL']
support=[src/n for n in ['FrontEnd.cpp','AnmManager.cpp','AnmVmCreate.cpp','AnmVmId.cpp','Sound.cpp']+(['Ending.cpp'] if a.trial=='ending' else [])]
linker,env=tool_environment();r=cold_link(src/'Player.cpp',out,'?PlayerUpdateCallback@@YIHPAUPlayer@@@Z',linker,env,profile=profile,support_sources=support)
def clean(v):
 if isinstance(v,dict):return {str(k):clean(x) for k,x in v.items()}
 if isinstance(v,list):return [clean(x) for x in v]
 return str(v) if isinstance(v,Path) else v
(out/'link.json').write_text(json.dumps(clean(r),indent=2));f=linked_functions(out/'source.exe',out/'source.map',out/'source.pdb');(out/'functions.json').write_text(json.dumps(f,indent=2));print(json.dumps([x for x in f['functions'] if 'GameOverTransition@@' in x['symbol']],indent=2))
