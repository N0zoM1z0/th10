#!/usr/bin/env python3
"""Full-target boundary and complete-native nonexact guard for 0x422200."""
import hashlib
from pathlib import Path
import sys
import tomllib
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))
from linked_image import PEImage, linked_functions, linked_code_fields, map_publics

with (ROOT / 'config/target.toml').open('rb') as f:
    pinned = tomllib.load(f)['target']['sha256']
assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == pinned
original = PEImage(ROOT / 'resources/th10.exe')
target = original.read_address(0x422200, 28)
assert hashlib.sha256(target).hexdigest() == 'ff7169a1ffda26fb648d1b9021d0a6affadd599732ae8acf918d02ca1872f6d1'
ins = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(target, 0x422200))
assert sum(x.size for x in ins) == 28 and ins[-1].mnemonic == 'ret'
assert original.read_address(0x42221C, 4) == b'\xcc' * 4

artifact = ROOT / 'build/match-linked/ResultMenuLifecycle'
fs = linked_functions(artifact / 'source.exe', artifact / 'source.map', artifact / 'source.pdb')['functions']
f = next(x for x in fs if x['symbol'] == '?ResultMenuMarkTasks422200@@YAXPAUResultMenuView@@@Z')
assert f['extent_source'] == 'pdb-section-contribution'
assert int(f['size']) == 26
native = PEImage(artifact / 'source.exe')
fields = linked_code_fields(native, map_publics(artifact / 'source.map'), int(f['address'], 0), 26)['fields']
assert not fields
print('nonexact source coverage: target28/native26, 0 link fields; exact credit 0')
