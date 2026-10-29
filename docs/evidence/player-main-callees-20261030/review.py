#!/usr/bin/env python3
"""Read-only complete result-owner review against one saved canonical image."""
from __future__ import annotations

import argparse
import copy
import json
from pathlib import Path
import runpy
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM


def review(directory: Path) -> dict:
    manifest = tomllib.loads((ROOT / 'config/match-units.toml').read_text())
    comparator = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))
    compare = comparator['compare_unit']
    target = comparator['verified_target']()
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    results = {}
    for name, count in [('result-menu-open-game-over', 102),
                        ('game-complete-stage-transition', 103)]:
        unit = copy.deepcopy(manifest['units'][name])
        unit['output_dir'] = str(directory)
        manifest['units'][name] = unit
        address, size = unit['target_address'], unit['size']
        instructions = list(decoder.disasm(target.read_address(address, size), address))
        assert len(instructions) == count
        assert sum(i.size for i in instructions) == size == 411
        assert instructions[-1].mnemonic == 'ret'
        assert target.read_address(address + size, 5) == b'\xcc' * 5
        starts = {i.address for i in instructions}
        fields, locals_ = [], []
        for instruction in instructions:
            for operand in instruction.operands:
                if operand.type == X86_OP_IMM:
                    value = operand.imm
                    if instruction.group(CS_GRP_JUMP) and address <= value < address + size:
                        assert value in starts
                        locals_.append({'offset': instruction.address - address,
                                        'destination': value - address})
                    elif 0x400000 <= value < 0x49c000:
                        fields.append((instruction.address - address + instruction.imm_offset,
                                       'REL32' if instruction.mnemonic == 'call' else 'DIR32', value))
                elif (operand.type == X86_OP_MEM and operand.mem.base == 0
                      and operand.mem.index == 0 and 0x400000 <= operand.mem.disp < 0x49c000):
                    fields.append((instruction.address - address + instruction.disp_offset,
                                   'DIR32', operand.mem.disp))
        assert fields == [(f['offset'], f['type'], f['target']) for f in unit['linkages']]
        assert len(fields) == 28 and len(locals_) == 3
        result = compare(name, _unit_manifest=manifest)
        assert result['result'] == 'exact', result
        results[name] = {'target_instruction_count': count, 'fixed_locals': locals_,
                         'complete_comparison': result}
    return results


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--image-dir', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    assert args.output.resolve().is_relative_to((ROOT / '.analysis').resolve())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    result = review(args.image_dir.resolve())
    args.output.write_text(json.dumps(result, indent=2) + '\n')
    print('Two complete owners exact:822 bytes/all56 fields; six fixed locals retained.')


if __name__ == '__main__':
    main()
