#!/usr/bin/env python3
"""Replay the four complete audio command owners and their fixed destinations."""

import argparse
import hashlib
import json
from pathlib import Path
import runpy
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_OP_IMM
from linked_image import PEImage, linked_functions, verify_capstone


def inventory(image, address, size):
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(address, size), address))
    assert sum(i.size for i in instructions) == size
    starts = {i.address for i in instructions}
    branches = []
    for instruction in instructions:
        if instruction.group(CS_GRP_JUMP):
            assert instruction.operands[0].type == CS_OP_IMM
            assert instruction.operands[0].imm in starts
            branches.append({'offset': instruction.address - address,
                             'destination': instruction.operands[0].imm - address,
                             'mnemonic': instruction.mnemonic})
    return {'instruction_count': len(instructions), 'fixed_local_branches': branches,
            'returns': [{'offset': i.address - address, 'operand': i.op_str}
                        for i in instructions if i.mnemonic == 'ret']}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', type=Path, required=True)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / 'build')
    proof = json.loads(Path(__file__).with_name('results.json').read_text())
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == proof['target_sha256']
    verify_capstone(tomllib.loads((ROOT / 'config/tools.lock.toml').read_text())['capstone'])
    target, native = PEImage(ROOT / 'resources/th10.exe'), PEImage(directory / 'source.exe')
    functions = linked_functions(directory / 'source.exe', directory / 'source.map',
                                 directory / 'source.pdb')['functions']
    manifest = tomllib.loads((ROOT / 'config/match-units.toml').read_text())
    compare = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))['compare_unit']
    matched = fields = 0
    for owner in proof['owners']:
        unit = manifest['units'][owner['unit']]
        unit['output_dir'] = str(directory.relative_to(ROOT))
        result = compare(owner['unit'], _unit_manifest=manifest)
        assert result['result'] == 'exact'
        candidates = [f for f in functions if f['symbol'] == unit['symbol']]
        assert len(candidates) == 1 and candidates[0]['size'] == unit['size']
        actual = inventory(native, int(candidates[0]['address'], 0), unit['size'])
        expected = inventory(target, unit['target_address'], unit['size'])
        assert actual == expected
        assert all(expected[k] == owner['target_review'][k] for k in expected)
        matched += unit['size']
        fields += len(result['linkages'])
    assert matched == 381 and fields == 22
    print(json.dumps({'result': 'exact', 'complete_owners': 4, 'matched_bytes': matched,
                      'bound_fields': fields, 'fixed_local_branches': 9}))


if __name__ == '__main__':
    main()
