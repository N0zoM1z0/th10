#!/usr/bin/env python3
"""Replay the complete spell-panel contracts and independently decode their geometry."""

import argparse
import copy
import hashlib
import json
from pathlib import Path
import runpy
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_OP_IMM, CS_OP_MEM
from linked_image import PEImage, verify_capstone


def geometry(image, address, size):
    raw = image.read_address(address, size)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, address))
    assert sum(i.size for i in instructions) == size
    starts = {i.address - address for i in instructions}
    fields, branches, calls = [], [], []

    def mapped(value):
        return any(image.image_base + s.rva <= value < image.image_base + s.rva + s.virtual_size
                   for s in image.sections)

    for instruction in instructions:
        offset = instruction.address - address
        if instruction.group(CS_GRP_JUMP):
            assert instruction.operands[0].type == CS_OP_IMM
            destination = instruction.operands[0].imm - address
            assert destination in starts
            branches.append({'offset': offset, 'destination': destination,
                             'mnemonic': instruction.mnemonic})
            continue
        if instruction.mnemonic == 'call':
            assert instruction.operands[0].type == CS_OP_IMM
            fields.append({'offset': offset + instruction.imm_offset,
                           'type': 'REL32', 'width': 4})
            calls.append(offset)
        elif instruction.disp_size == 4 and any(
                o.type == CS_OP_MEM and o.mem.base == 0 and mapped(o.mem.disp)
                for o in instruction.operands):
            fields.append({'offset': offset + instruction.disp_offset,
                           'type': 'DIR32', 'width': 4})
        elif instruction.imm_size == 4 and any(
                o.type == CS_OP_IMM and mapped(o.imm) for o in instruction.operands):
            fields.append({'offset': offset + instruction.imm_offset,
                           'type': 'DIR32', 'width': 4})
    normalized = bytearray(raw)
    for field in fields:
        normalized[field['offset']:field['offset'] + 4] = b'\0' * 4
    return {'owned_bytes': size, 'decoded_instructions': len(instructions),
            'fields': fields, 'fixed_local_branches': branches, 'call_offsets': calls,
            'returns': [{'offset': i.address - address, 'operand': i.op_str}
                        for i in instructions if i.mnemonic == 'ret'],
            'normalized_sha256': hashlib.sha256(normalized).hexdigest()}


def verify(directory, proof):
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == proof['target_sha256']
    verify_capstone(tomllib.loads((ROOT / 'config/tools.lock.toml').read_text())['capstone'])
    target = PEImage(ROOT / 'resources/th10.exe')
    assert target.directories[5] == (0, 0)
    manifest = tomllib.loads((ROOT / 'config/match-units.toml').read_text())
    manifest['units'].update(copy.deepcopy(proof['unit_specs']))
    compare = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))['compare_unit']
    image = PEImage(directory / 'source.exe')
    reports = {}
    for name in proof['unit_specs']:
        unit = manifest['units'][name]
        unit['output_dir'] = str(directory)
        report = compare(name, _unit_manifest=manifest)
        assert report['result'] == 'exact' and not report['first_differences']
        assert report['matched_bytes'] == report['size']
        assert report['extent_source'] == 'pdb-section-contribution'
        native = geometry(image, int(report['candidate_address'], 0), report['size'])
        expected = geometry(target, int(report['target_address'], 0), report['size'])
        assert native == expected == proof['complete_geometry'][name]
        wanted = sorted([{'offset': int(f['offset'], 0), 'type': f['type'], 'width': f['width']}
                         for f in report['linkages']], key=lambda f: f['offset'])
        assert native['fields'] == wanted
        for field in unit['linkages']:
            symbol = field.get('symbol', '')
            if symbol.startswith('__real@'):
                bits = bytes.fromhex(symbol.removeprefix('__real@'))[::-1]
                assert target.read_address(field['target'], len(bits)) == bits
        reports[name] = report
    assert sum(r['size'] for r in reports.values()) == 1599
    assert sum(len(r['linkages']) for r in reports.values()) == 47
    return reports


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', type=Path, required=True)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / 'build')
    proof = json.loads(Path(__file__).with_name('results.json').read_text())
    reports = verify(directory, proof)
    print(json.dumps({'result': 'exact', 'new_authored_bytes': 1057,
                      'new_indeterminate_origin_bytes': 10,
                      'same_image_contracts': len(reports), 'complete_bytes': 1599,
                      'bound_fields': 47}))


if __name__ == '__main__':
    main()
