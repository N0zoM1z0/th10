#!/usr/bin/env python3
"""Check complete nonexact GUI message bytes, call order and observed code events."""

import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import runpy

ROOT = Path(__file__).resolve().parents[3]
ORIGINAL = ROOT / 'docs/evidence/gui-message-run-context-20261030'
REVIEW = runpy.run_path(str(ORIGINAL / 'verify-native.py'))
RETURN_SLOTS = runpy.run_path(str(
    ROOT / 'docs/evidence/gui-message-refinement-20261031/verify-native.py'))['return_slots']


def code_events(image, address, prefix, tables, calls, score_counter):
    decoder = REVIEW['Cs'](REVIEW['CS_ARCH_X86'], REVIEW['CS_MODE_32'])
    decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(address, prefix), address))
    assert sum(i.size for i in instructions) == prefix
    assert any(i.mnemonic == 'sub' and i.op_str == 'esp, 0xd8'
               for i in instructions[:8])
    start = tables[1]['entry_offsets'][6]
    end = tables[0]['entry_offsets'][10]
    stage7 = [i for i in instructions if start <= i.address - address < end]
    assert stage7[0].mnemonic == 'cmp' and stage7[0].op_str == 'dword ptr [ebp], 1'
    gate = next(i for i in stage7 if i.group(REVIEW['CS_GRP_JUMP']))
    assert gate.mnemonic == 'jge'
    operands = [i.operands[1].imm for i in stage7 if i.mnemonic == 'add'
                and i.operands[1].type == REVIEW['CS_OP_IMM']]
    scripts = [i.operands[1].imm for i in stage7 if i.mnemonic == 'mov'
               and i.op_str.startswith('ebx, ') and i.operands[1].type == REVIEW['CS_OP_IMM']]
    flag_updates = [i.op_str for i in instructions if i.mnemonic == 'or'
                    and i.op_str == 'dword ptr [eax + 0x9eb4], 0x20']
    for index, instruction in enumerate(instructions):
        if instruction.mnemonic != 'or' or instruction.op_str not in flag_updates:
            continue
        following = instructions[index + 1]
        assert following.mnemonic == 'mov'
        operand = following.operands[1]
        assert (operand.type == REVIEW['CS_OP_MEM'] and operand.mem.base == 0
                and operand.mem.index == 0 and operand.mem.disp == score_counter)
    factory = '?StageCreateTransitionEffect@@YGPAXHHHIII@Z'
    setups = []
    for index, instruction in enumerate(instructions):
        if (instruction.mnemonic == 'call'
                and instruction.operands[0].type == REVIEW['CS_OP_IMM']
                and calls[instruction.operands[0].imm] == factory):
            setups.append([f'{i.mnemonic} {i.op_str}' for i in instructions[index - 6:index]])
    assert operands == [0x44, 0x4d, 0x44, 0x4d, 0x2e, 0x37, 0x2e, 0x37]
    assert scripts == [0x21, 0x24, 0x21, 0x24, 0x18, 0x1b, 0x18, 0x1b]
    assert len(flag_updates) == 2
    assert setups == [['push 0', 'push 0', 'push 0', 'push 0x78', 'push 5', 'mov ebx, 0x31']]
    return {'stage7_gate': gate.mnemonic, 'stage7_operand_offsets': operands,
            'stage7_script_ids': scripts, 'flag_updates': flag_updates,
            'flag_before_score_reads': 2,
            'transition_factory_setup': setups}


def verify(directory, proof):
    previous_path = ORIGINAL / 'results.json'
    assert hashlib.sha256(previous_path.read_bytes()).hexdigest() == proof['target_review_sha256']
    previous = json.loads(previous_path.read_text())
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == proof['target_sha256']
    REVIEW['verify_capstone'](REVIEW['tomllib'].loads(
        (ROOT / 'config/tools.lock.toml').read_text())['capstone'])
    REVIEW['check_target']({**previous, 'native_reproduction': proof['native_reproduction']})
    target = REVIEW['PEImage'](ROOT / 'resources/th10.exe')
    assert target.directories[5] == (0, 0)
    actual = REVIEW['inventory'](directory)
    assert actual == proof['native_reproduction'], 'complete native receipt differs'
    roles = previous['target_call_roles']
    target_calls = {int(r['target'], 0): r['symbol'] for r in roles}
    expected_counts = {r['symbol']: r['target_count'] for r in roles}
    assert actual['direct_call_counts'] == expected_counts
    expected_order = [target_calls[int(c['target'], 0)]
                      for c in previous['target_review']['direct_calls']]
    native_order = [f['destination']['symbols'][0] for f in actual['field_identities']
                    if f['type'] == 'REL32']
    assert native_order == expected_order == proof['direct_call_role_order']
    assert len(native_order) == 150
    assert Counter(native_order) == expected_counts
    image = REVIEW['PEImage'](directory / 'source.exe')
    functions = REVIEW['linked_functions'](directory / 'source.exe', directory / 'source.map',
                                           directory / 'source.pdb')['functions']
    owner = next(f for f in functions if f['symbol'] == '?Run@GuiMessageVmView@@SGHPAU1@@Z')
    address = int(owner['address'], 0)
    fields = REVIEW['linked_code_fields'](image, REVIEW['map_publics'](directory / 'source.map'),
                                          address, owner['size'])['fields']
    native_calls = {}
    for field in fields:
        if field['type'] != 'REL32':
            continue
        assert field['instruction'] == 'call'
        names = [s for s in field['candidate_symbols'] if not s.startswith('_th10_')]
        assert len(names) == 1
        destination = field['candidate_target']
        native_calls[destination] = names[0]
        section = next(s for s in image.sections if image.image_base + s.rva <= destination
                       < image.image_base + s.rva + s.virtual_size)
        assert section.characteristics & 0x20000000, 'direct callee is a data anchor'
        assert any(f['symbol'] == names[0] and int(f['address'], 0) == destination
                   and f['size'] > 0 for f in functions), 'direct callee has no PDB-owned body'
    assert len(native_calls) == 24
    slot_symbols = proof['hidden_return_slots']['target']
    target_slots = RETURN_SLOTS(target, 0x415E90, 4306,
                                {a: s for a, s in target_calls.items() if s in slot_symbols})
    native_slots = RETURN_SLOTS(image, address, actual['code_and_alignment_bytes'],
                                {a: s for a, s in native_calls.items() if s in slot_symbols})
    assert target_slots == proof['hidden_return_slots']['target']
    assert native_slots == proof['hidden_return_slots']['native']
    assert sum(len(v) for v in native_slots.values()) == 54
    for symbol in slot_symbols:
        assert [v['output'] for v in target_slots[symbol]] == [v['output'] for v in native_slots[symbol]]
    counters = {f['candidate_target'] for f in fields
                if '?g_GuiCounter4C4C@@3HA' in f['candidate_symbols']}
    assert len(counters) == 1
    expected_events = code_events(target, 0x415E90, 4308,
                                  previous['target_review']['tables'], target_calls, 0x474C4C)
    events = code_events(image, address, actual['code_and_alignment_bytes'],
                         actual['tables'], native_calls, next(iter(counters)))
    assert events == expected_events == proof['reviewed_code_events']
    assert proof['exact_credit'] == 0 and proof['status'] == 'complete-native-candidate-nonexact'
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
                      'ordered_target_callees': 150, 'matching_hidden_return_slots': 54,
                      'target_transition_call_abi': True, 'exact_credit': 0}))


if __name__ == '__main__':
    main()
