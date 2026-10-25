#!/usr/bin/env python3
"""Cold-build a complete isolated activation proposal; diagnostic, no acceptance."""
import argparse, hashlib, json, shutil, subprocess, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / 'scripts'))
from ltcg_link import cold_link, tool_environment
from linked_image import PEImage, linked_functions
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('--trial', choices=['first', 'capture'], required=True)
p.add_argument('--output-dir', required=True)
p.add_argument('--prepare-only', action='store_true')
a = p.parse_args()
out = (ROOT / a.output_dir).resolve()
assert out.is_relative_to(ROOT / 'build') and not out.exists()
assert PEImage(ROOT / 'resources/th10.exe').sha256 == '2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
for name, digest in json.loads((PACKET / 'source-inputs.json').read_text()).items():
    assert hashlib.sha256((ROOT / name).read_bytes()).hexdigest() == digest, name
src = ROOT / '.analysis/gpt-dots' / (out.name + '-inputs')
assert not src.exists()
shutil.copytree(ROOT / 'src', src)
# Proposal edits used text-mode writes; normalize only those three existing files.
for name in ['Player.cpp', 'EnemyGameState.hpp', 'ItemManager.cpp']:
    file = src / name
    file.write_text(file.read_text())
subprocess.run(['patch', '-p1', '--batch', '-i', str(PACKET / (a.trial + '.patch'))], cwd=src, check=True)
if a.prepare_only:
    print('Prepared hash-checked isolated proposal:', src)
    sys.exit(0)
profile = ['/TP', '/MT', '/O2', '/Gy', '/GF', '/Oi', '/DNDEBUG', '/I' + str(src), '/GL']
support = [src / x for x in ['ItemManager.cpp', 'AnmManager.cpp', 'AnmVmCreate.cpp', 'EnemyEclDispatcher.cpp']]
linker, env = tool_environment()
r = cold_link(src / 'Player.cpp', out, '?PlayerUpdateCallback@@YIHPAUPlayer@@@Z', linker, env,
    profile=profile, support_sources=support, support_profiles={src / 'EnemyEclDispatcher.cpp': profile + ['/GS']})
def clean(v):
    if isinstance(v, dict): return {str(k): clean(x) for k, x in v.items()}
    if isinstance(v, list): return [clean(x) for x in v]
    return str(v) if isinstance(v, Path) else v
(out / 'link.json').write_text(json.dumps(clean(r), indent=2))
f = linked_functions(out / 'source.exe', out / 'source.map', out / 'source.pdb')
(out / 'functions.json').write_text(json.dumps(f, indent=2))
print(json.dumps([x for x in f['functions'] if any(n in x['symbol'] for n in
    ['PlayerNotifyPowerTransition@@', 'CancelSpellBonusAfterGrace@'])], indent=2))
