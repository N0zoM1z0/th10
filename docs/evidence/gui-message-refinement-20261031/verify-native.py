#!/usr/bin/env python3
"""Reproduce the complete GUI message candidate; this grants no exact credit."""

import argparse
import hashlib
import json
from pathlib import Path
import runpy

ROOT = Path(__file__).resolve().parents[3]
PREVIOUS = ROOT / 'docs/evidence/gui-message-run-context-20261030'
REVIEW = runpy.run_path(str(PREVIOUS / 'verify-native.py'))


def return_slots(image, address, prefix_size, calls):
    decoder = REVIEW['Cs'](REVIEW['CS_ARCH_X86'], REVIEW['CS_MODE_32'])
    decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(address, prefix_size), address))
    assert sum(i.size for i in instructions) == prefix_size
    result = {symbol: [] for symbol in calls.values()}
    for index, instruction in enumerate(instructions):
        if instruction.mnemonic != 'call' or instruction.operands[0].type != REVIEW['CS_OP_IMM']:
            continue
        symbol = calls.get(instruction.operands[0].imm)
        if symbol is None:
            continue
        # Each real private sret call is preceded by its EDI stack-slot address.
        setup = next(i for i in reversed(instructions[max(0, index - 12):index])
                     if i.mnemonic == 'lea' and i.op_str.startswith('edi, [esp'))
        between = instructions[instructions.index(setup) + 1:index]
        assert all('edi' not in [i.reg_name(r) for r in i.regs_access()[1]] for i in between)
        result[symbol].append({'call_offset': instruction.address - address,
                               'output': setup.op_str})
    return result


def verify(directory, proof):
    previous_path = PREVIOUS / 'results.json'
    assert hashlib.sha256(previous_path.read_bytes()).hexdigest() == proof['previous_proof_sha256']
    previous = json.loads(previous_path.read_text())
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == proof['target_sha256']
    REVIEW['verify_capstone'](REVIEW['tomllib'].loads(
        (ROOT / 'config/tools.lock.toml').read_text())['capstone'])
    REVIEW['check_target']({**previous, 'native_reproduction': proof['native_reproduction']})
    target = REVIEW['PEImage'](ROOT / 'resources/th10.exe')
    assert target.directories[5] == (0, 0)
    actual = REVIEW['inventory'](directory)
    assert actual == proof['native_reproduction'], 'complete native receipt differs'
    roles = {r['symbol']: r for r in previous['target_call_roles']}
    for symbol, role in roles.items():
        assert actual['direct_call_counts'][symbol] == role['native_count']
    symbols = proof['hidden_return_slots']['target']
    target_calls = {int(roles[s]['target'], 0): s for s in symbols}
    assert return_slots(target, 0x415E90, 4306, target_calls) == symbols
    image = REVIEW['PEImage'](directory / 'source.exe')
    owners = REVIEW['linked_functions'](directory / 'source.exe', directory / 'source.map',
                                       directory / 'source.pdb')['functions']
    owner = next(f for f in owners if f['symbol'] == '?Run@GuiMessageVmView@@SGHPAU1@@Z')
    address = int(owner['address'], 0)
    fields = REVIEW['linked_code_fields'](image, REVIEW['map_publics'](directory / 'source.map'),
                                          address, owner['size'])['fields']
    native_calls = {f['candidate_target']: s for f in fields for s in symbols
                    if f['type'] == 'REL32' and s in f['candidate_symbols']}
    slots = return_slots(image, address, actual['code_and_alignment_bytes'], native_calls)
    assert slots == proof['hidden_return_slots']['native']
    assert sum(len(v) for v in slots.values()) == 54
    for symbol in symbols:
        assert [s['output'] for s in slots[symbol]] == [s['output'] for s in symbols[symbol]]
    return actual


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', type=Path, required=True)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / 'build')
    proof = json.loads(Path(__file__).with_name('results.json').read_text())
    actual = verify(directory, proof)
    print(json.dumps({'result': 'native-candidate-reproduced-nonexact',
                      'candidate_size': actual['complete_owned_bytes'], 'target_size': 4480,
                      'native_fields': len(actual['field_identities']),
                      'native_local_branches': len(actual['fixed_local_branches']),
                      'matching_hidden_return_slots': 54, 'exact_credit': 0}))


if __name__ == '__main__':
    main()
