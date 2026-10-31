#!/usr/bin/env python3
"""Reproduce the complete nonexact GUI drawing owner and bound read-order gains."""

import argparse
import hashlib
import json
from pathlib import Path
import runpy

ROOT = Path(__file__).resolve().parents[3]
OLD = ROOT / 'docs/evidence/gui-draw-context-20261030'
REVIEW = runpy.run_path(str(OLD / 'verify-native.py'))
from capstone import CS_OP_MEM


def observations(directory, proof):
    REVIEW['verify_capstone'](REVIEW['tomllib'].loads(
        (ROOT / 'config/tools.lock.toml').read_text())['capstone'])
    old_path = OLD / 'results.json'
    assert hashlib.sha256(old_path.read_bytes()).hexdigest() == proof['previous_proof_sha256']
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == proof['target_sha256']
    target = REVIEW['PEImage'](ROOT / 'resources/th10.exe')
    image = REVIEW['PEImage'](directory / 'source.exe')
    publics = REVIEW['map_publics'](directory / 'source.map')
    names = {p['symbol']: p['address'] for p in publics}
    address = names[proof['owner']['symbol']]
    native = REVIEW['inventory'](directory, {'owner': proof['owner']})
    fields = REVIEW['linked_code_fields'](image, publics, address, 724)
    assert fields['normalization_complete'] and len(fields['fields']) == 33
    roles = json.loads(old_path.read_text())['field_role_review']['roles']
    decoder = REVIEW['Cs'](REVIEW['CS_ARCH_X86'], REVIEW['CS_MODE_32'])
    decoder.detail = True
    instructions = list(decoder.disasm(target.read_address(0x415800, 724), 0x415800))
    assert len(instructions) == 173 and sum(i.size for i in instructions) == 724
    target_fields, branches = [], []
    def mapped(value):
        return any(target.image_base + s.rva <= value <
                   target.image_base + s.rva + s.virtual_size for s in target.sections)
    for i in instructions:
        offset = i.address - 0x415800
        if i.group(REVIEW['CS_GRP_JUMP']):
            assert i.operands[0].type == REVIEW['CS_OP_IMM']
            branches.append(dict(offset=offset, destination=i.operands[0].imm - 0x415800,
                                 mnemonic=i.mnemonic))
        elif i.mnemonic == 'call':
            assert i.operands[0].type == REVIEW['CS_OP_IMM']
            target_fields.append((offset + i.imm_offset, 'REL32'))
        elif i.disp_size == 4 and any(o.type == CS_OP_MEM and o.mem.base == 0
                                     and mapped(o.mem.disp) for o in i.operands):
            target_fields.append((offset + i.disp_offset, 'DIR32'))
        elif i.imm_size == 4 and any(o.type == REVIEW['CS_OP_IMM'] and mapped(o.imm)
                                    for o in i.operands):
            target_fields.append((offset + i.imm_offset, 'DIR32'))
    assert target_fields == [(r['target_offset'], r['type']) for r in roles]
    assert len(branches) == 23 and branches == native['fixed_local_branches']
    assert native['returns'] == [{'offset': 721, 'operand': '4'}]
    assert native['complete_owned_bytes'] == 724 and native['decoded_instructions'] == 173
    target_code = bytearray(target.read_address(0x415800, 724))
    native_code = bytearray(image.read_address(address, 724))
    functions = REVIEW['linked_functions'](directory / 'source.exe', directory / 'source.map',
                                           directory / 'source.pdb')['functions']
    bindings = []
    for field, role in zip(fields['fields'], roles):
        assert field['type'] == role['type'] and field['width'] == role['width'] == 4
        symbol, target_address = role['symbol'], int(role['target'], 0)
        to, no = role['target_offset'], field['offset']
        word = int.from_bytes(target_code[to:to + 4], 'little', signed=role['type'] == 'REL32')
        assert (0x415800 + to + 4 + word if role['type'] == 'REL32' else word) == target_address
        assert field['candidate_target'] == names[symbol]
        if symbol.startswith('__real@'):
            assert image.read_address(names[symbol], 4) == target.read_address(target_address, 4)
        if role['type'] == 'REL32':
            assert any(f['symbol'] == symbol and int(f['address'], 0) == names[symbol]
                       and f['size'] > 0 for f in functions)
        target_code[to:to + 4] = bytes(4)
        native_code[no:no + 4] = bytes(4)
        bindings.append({**role, 'native_offset': no})
    regions = []
    for start, end in ((0, 0xd6), (0xf0, 724)):
        assert target_code[start:end] == native_code[start:end]
        regions.append(dict(start=start, size=end - start,
                            normalized_sha256=hashlib.sha256(target_code[start:end]).hexdigest()))
    misplaced = [r for r in bindings if r['target_offset'] != r['native_offset']]
    assert len(misplaced) == 1 and misplaced[0]['target_offset'] == 0xec
    assert misplaced[0]['native_offset'] == 0xe8
    # The unresolved interval contains exactly the same four instructions;
    # only the address calculation moves across the two stores and FADD.
    target_middle = list(decoder.disasm(target_code[0xd6:0xf0], 0))
    native_middle = list(decoder.disasm(native_code[0xd6:0xf0], 0))
    assert len(target_middle) == len(native_middle) == 4
    assert target_middle[0].mnemonic == native_middle[-1].mnemonic == 'lea'
    assert [bytes(i.bytes) for i in native_middle] == [
        bytes(i.bytes) for i in target_middle[1:] + target_middle[:1]]
    assert target_code != native_code
    return dict(native_reproduction=native, field_bindings=bindings, bound_regions=regions,
                misplaced_fields=misplaced, exact_credit=0)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', required=True, type=Path)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / 'build')
    proof = json.loads(Path(__file__).with_name('results.json').read_text())
    actual = observations(directory, proof)
    assert all(actual[k] == proof[k] for k in actual)
    print(json.dumps(dict(result='complete-native-candidate-nonexact', size=724,
                          fields=33, local_destinations=23, misplaced_fields=1, exact_credit=0)))


if __name__ == '__main__':
    main()
