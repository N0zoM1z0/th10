#!/usr/bin/env python3
"""Rebuild one isolated complete source proposal; no canonical acceptance."""
import argparse,sys,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'scripts'))
from ltcg_link import cold_link,tool_environment
from linked_image import PEImage,linked_functions
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--trial',choices=('first','model','identity'),required=True);p.add_argument('--output-dir',required=True);a=p.parse_args()
out=(ROOT/a.output_dir).resolve();assert out.is_relative_to(ROOT/'build');assert not out.exists(), 'preserve prior artifacts; choose a fresh output directory'
assert PEImage(ROOT/'resources/th10.exe').sha256=='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
linker,env=tool_environment();result=cold_link(Path(__file__).parent/'proposals'/(a.trial+'.cpp'),out,'?PopupNumberDrawCallback@@YIHPAX@Z',linker,env,profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/Isrc','/GL','/GS','/EHs'],support_sources=[ROOT/'src/AnmManager.cpp'])
(out/'proposal-link.json').write_text(json.dumps(result,indent=2,default=str));functions=linked_functions(out/'source.exe',out/'source.map',out/'source.pdb');(out/'proposal-functions.json').write_text(json.dumps(functions,indent=2,default=str));print(json.dumps([x for x in functions['functions'] if 'Popup' in x['symbol']],indent=2))
