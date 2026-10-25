#!/usr/bin/env python3
"""Cold-build a complete isolated shot-creation proposal. Diagnostic, no acceptance."""
import argparse,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'scripts'))
from ltcg_link import cold_link,tool_environment
from linked_image import PEImage,linked_functions
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--trial',choices=['first','while','counted','motion'],required=True);p.add_argument('--output-dir',required=True);a=p.parse_args()
out=(ROOT/a.output_dir).resolve()
assert out.is_relative_to(ROOT/'build') and not out.exists()
assert PEImage(ROOT/'resources/th10.exe').sha256=='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/Isrc','/GL']
support=[ROOT/'src'/x for x in ['AnmManager.cpp','AnmVmCreate.cpp','Enemy.cpp','EnemyEclDispatcher.cpp']]
def clean(value):
    if isinstance(value,dict):return {str(k):clean(v) for k,v in value.items()}
    if isinstance(value,list):return [clean(v) for v in value]
    return str(value) if isinstance(value,Path) else value
linker,env=tool_environment()
r=cold_link(Path(__file__).parent/'proposals'/(a.trial+'.cpp'),out,'?PlayerFireShots@@YAHPAUPlayer@@H@Z',linker,env,profile=profile,support_sources=support,support_profiles={ROOT/'src/EnemyEclDispatcher.cpp':profile+['/GS']})
(out/'link.json').write_text(json.dumps(clean(r),indent=2))
f=linked_functions(out/'source.exe',out/'source.map',out/'source.pdb')
(out/'functions.json').write_text(json.dumps(f,indent=2))
print(json.dumps([x for x in f['functions'] if 'PlayerCreateShot@@' in x['symbol'] or 'PlayerFireShots@@' in x['symbol']],indent=2))
