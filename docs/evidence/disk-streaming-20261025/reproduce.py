#!/usr/bin/env python3
"""Prepare/cold-link isolated disk-streaming hypotheses; no acceptance authority."""
import argparse,hashlib,json,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'scripts'))
from ltcg_link import cold_link,tool_environment
from linked_image import PEImage,linked_functions
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--trial',choices=['first','constructor'],required=True)
p.add_argument('--model-dir',required=True)
p.add_argument('--output-dir')
p.add_argument('--prepare-only',action='store_true')
a=p.parse_args()
base=(ROOT/a.model_dir).resolve()
assert base.is_relative_to(ROOT/'.analysis') and not base.exists()
inputs=json.loads((HERE/'source-inputs.json').read_text())
assert all(hashlib.sha256((ROOT/n).read_bytes()).hexdigest()==h for n,h in inputs.items())
assert PEImage(ROOT/'resources/th10.exe').sha256=='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
base.mkdir(parents=True)
for n in inputs:
    (base/Path(n).name).write_text((ROOT/n).read_text())
for patch in ['first-model.patch']+(['constructor-model.patch'] if a.trial=='constructor' else []):
    subprocess.run(['patch','--batch','--fuzz=0','-p0','-i',str(HERE/patch)],cwd=base,check=True)
if a.prepare_only:sys.exit(0)
assert a.output_dir
out=(ROOT/a.output_dir).resolve()
assert out.is_relative_to(ROOT/'build') and not out.exists()
profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/Isrc','/GL','/GS','/EHsc']
def clean(v):
    if isinstance(v,dict):return {str(k):clean(x) for k,x in v.items()}
    if isinstance(v,list):return [clean(x) for x in v]
    return str(v) if isinstance(v,Path) else v
linker,env=tool_environment()
r=cold_link(base/'Sound.cpp',out,'?OpenDiskBgm@SoundPlayerView@@QAEHPBD@Z',linker,env,profile=profile,support_sources=[base/'ZWave.cpp'])
(out/'link.json').write_text(json.dumps(clean(r),indent=2))
(out/'functions.json').write_text(json.dumps(linked_functions(out/'source.exe',out/'source.map',out/'source.pdb'),indent=2))
assert all(hashlib.sha256((ROOT/n).read_bytes()).hexdigest()==h for n,h in inputs.items())
print('Diagnostic image built; whole-owner and all-field replay still required.')
