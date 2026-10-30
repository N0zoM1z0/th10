#!/usr/bin/env python3
"""Reproduce the complete nonexact MusicRoom owner, including its four-way table."""

import argparse
import hashlib
import json
from pathlib import Path
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_OP_IMM, CS_OP_MEM
from linked_image import PEImage, linked_code_fields, linked_functions, map_publics, map_statics, verify_capstone


def string_identity(image, address):
    data = bytearray()
    for offset in range(512):
        value = image.read_address(address + offset, 1)
        data += value
        if value == b'\0':
            return {'size': len(data), 'sha256': hashlib.sha256(data).hexdigest()}
    raise AssertionError('unterminated native source literal')


def inventory(directory):
    image = PEImage(directory / 'source.exe')
    functions = linked_functions(directory / 'source.exe', directory / 'source.map',
                                 directory / 'source.pdb')['functions']
    owners = [f for f in functions if f['symbol'] == '?FrontEndUpdateMusicRoom@@YGHPAUFrontEndControllerView@@@Z']
    assert len(owners) == 1
    owner = owners[0]
    assert owner['map_object'] == 'support-10-FrontEnd.ltcg.obj'
    assert owner['extent_source'] == 'pdb-section-contribution'
    address, size = int(owner['address'], 0), owner['size']
    raw = image.read_address(address, size)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw[:-16], address))
    assert sum(i.size for i in instructions) == size - 16
    starts = {i.address - address for i in instructions}
    switches = [i for i in instructions if i.mnemonic == 'jmp' and i.operands[0].type == CS_OP_MEM]
    assert len(switches) == 1
    table_offset = switches[0].operands[0].mem.disp - address
    assert table_offset == size - 16
    table = [int.from_bytes(raw[p:p + 4], 'little') - address for p in range(table_offset, size, 4)]
    assert all(destination in starts for destination in table)
    publics = map_publics(directory / 'source.map')
    all_symbols = publics + map_statics(directory / 'source.map')
    input_base = next(p['address'] for p in publics if p['symbol'] == '?g_FrontEndInput@@3UFrontEndInputView@@A')
    relocations = set(image.base_relocations())
    fields = linked_code_fields(image, all_symbols, address, size)
    assert fields['normalization_complete']
    normalized = bytearray(raw)
    identities = []
    for field in fields['fields']:
        offset, width, destination = field['offset'], field['width'], field['candidate_target']
        normalized[offset:offset + width] = b'\0' * width
        identity = {'offset': offset, 'width': width, 'type': field['type']}
        if address <= destination < address + size:
            identity['destination'] = {'local_offset': destination - address}
        else:
            symbols = sorted(s for s in field['candidate_symbols'] if not s.startswith('_th10_'))
            if symbols:
                identity['destination'] = {'symbols': symbols}
            elif destination == input_base + 6:
                identity['destination'] = {'symbol': '?g_FrontEndInput@@3UFrontEndInputView@@A', 'addend': 6}
            else:
                section = next(s for s in image.sections if
                               image.image_base + s.rva <= destination < image.image_base + s.rva + s.virtual_size)
                if section.name == '.rdata':
                    assert image.read_address(destination, 5) == b'No.%d'
                    identity['destination'] = {'source_literal': 'music-locked-title',
                                               **string_identity(image, destination)}
                else:
                    assert section.name == '.data'
                    assert all(destination + n * 4 in relocations for n in range(8))
                    pointers = [int.from_bytes(image.read_address(destination + n * 4, 4), 'little') for n in range(8)]
                    identity['destination'] = {'source_pointer_table': 'music-warning-lines',
                                               'strings': [string_identity(image, p) for p in pointers]}
        identities.append(identity)
    branches = []
    for instruction in instructions:
        if not instruction.group(CS_GRP_JUMP):
            continue
        if instruction.operands[0].type != CS_OP_IMM:
            assert instruction.address == switches[0].address
            continue
        destination = instruction.operands[0].imm - address
        assert destination in starts
        branches.append({'offset': instruction.address - address, 'destination': destination,
                         'mnemonic': instruction.mnemonic})
    return {'complete_owned_bytes': size, 'code_and_alignment_bytes': size - 16,
            'decoded_instructions': len(instructions),
            'normalized_sha256': hashlib.sha256(normalized).hexdigest(),
            'field_identities': identities, 'fixed_local_branches': branches,
            'table': {'offset': table_offset, 'entry_offsets': table},
            'returns': [{'offset': i.address - address, 'operand': i.op_str}
                        for i in instructions if i.mnemonic == 'ret']}



def check_target(proof):
    image = PEImage(ROOT / 'resources/th10.exe')
    review = proof['target_review']
    address = int(review['address'], 0)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    code = image.read_address(address, review['code_bytes'])
    instructions = list(decoder.disasm(code, address))
    assert sum(i.size for i in instructions) == 2778
    assert len(instructions) == review['instruction_count'] == 740
    assert hashlib.sha256(code).hexdigest() == review['code_sha256']
    starts = {i.address - address for i in instructions}
    switches = [i for i in instructions if i.mnemonic == 'jmp' and i.operands[0].type == CS_OP_MEM]
    assert len(switches) == 1 and switches[0].operands[0].mem.disp == 0x4349CC
    entries = [int.from_bytes(image.read_address(0x4349CC + n * 4, 4), 'little') - address for n in range(4)]
    assert entries == review['table']['entry_offsets'] and all(e in starts for e in entries)
    assert image.read_address(0x4349DC, 4) == b'\xcc' * 4
    assert (ROOT / 'resources/th10.exe').read_bytes().count((0x4349CC).to_bytes(4, 'little')) == 1
    branches = [{'offset': i.address - address, 'destination': i.operands[0].imm - address,
                 'mnemonic': i.mnemonic} for i in instructions
                if i.group(CS_GRP_JUMP) and i.operands[0].type == CS_OP_IMM]
    assert branches == review['fixed_local_branches'] and all(b['destination'] in starts for b in branches)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', type=Path, required=True)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / 'build')
    proof = json.loads(Path(__file__).with_name('results.json').read_text())
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == proof['target_sha256']
    verify_capstone(tomllib.loads((ROOT / 'config/tools.lock.toml').read_text())['capstone'])
    check_target(proof)
    actual = inventory(directory)
    assert actual == proof['native_reproduction'], 'native receipt differs'
    print(json.dumps({'result': 'native-candidate-reproduced-nonexact',
                      'candidate_size': actual['complete_owned_bytes'], 'target_size': 2796,
                      'field_count': len(actual['field_identities']),
                      'fixed_local_branches': len(actual['fixed_local_branches']),
                      'table_entries': 4, 'exact_credit': 0}))


if __name__ == '__main__':
    main()
