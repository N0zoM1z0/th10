"""Replay direct target entry references found during IDA boundary review.

The pinned target instruction at each reference is decoded independently.
The entry, extent, dense decode, and lack of overlap were separately checked
against the IDA MCP function inventory and the existing target boundary audit.
"""

import argparse
import csv
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_CALL, CS_GRP_JUMP
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

from target_identity import pe_bytes_at, resolve_target, verify_target

ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'ida-direct-entry-target-decode-2026-09-17'
ENTRY_REFS = {
    '0x00402720': '0x403052',
    '0x00402850': '0x403061',
    '0x00402CA0': '0x403071',
    '0x00405750': '0x405843',
    '0x004066E0': '0x4066c7',
    '0x00408D60': '0x409223',
    '0x0040A940': '0x40abf3',
    '0x0040B9F0': '0x40ba73',
    '0x0040C3C0': '0x40b4f4',
    '0x00413350': '0x420207',
    '0x00415800': '0x415af1',
    '0x00417870': '0x417c76',
    '0x00418190': '0x4187c1',
    '0x0041AFD0': '0x41ba29',
    '0x0041C330': '0x41c4c1',
    '0x0041FDD0': '0x41fef1',
    '0x00420100': '0x4201b2',
    '0x004216F0': '0x4201cb',
    '0x00421F00': '0x4658f6',
    '0x00422660': '0x422aa1',
    '0x0042B780': '0x42b9b3',
    '0x0042C360': '0x42c593',
    '0x0042CAA0': '0x42c9f7',
    '0x00437A00': '0x420260',
    '0x0043AAD0': '0x43acad',
    '0x0043B8D0': '0x43ba9f',
    '0x0043CD30': '0x43ccfb',
    '0x00457589': '0x4576a1',
    '0x00463D3B': '0x463bb5',
    '0x00463D8A': '0x463ce7',
    '0x00464EA0': '0x46502e',
    '0x00464F15': '0x463c1c',
    '0x00464F2C': '0x463c7a',
    '0x00464F88': '0x463bf6',
    '0x0040678E': '0x406777',
    '0x0042B6F0': '0x42b9a2',
    '0x0045F715': '0x459c5f',
    '0x00452E1C': '0x452ec7',
    '0x004535BD': '0x45365e',
    '0x00457547': '0x45377e',
    '0x00458E74': '0x458eae',
    '0x0045A245': '0x4556c3',
    '0x00459972': '0x454c9e',
    '0x00462428': '0x438273',
    '0x004624E5': '0x4382b1',
}
MATH_FRAGMENT = ('0x00463C0D', '0x00463BFB', 0xAE)
IDA_OMITTED_ENTRIES = {'0x0040678E', '0x0042B6F0', '0x0045F715'}
FOCUSED_TERMINALS = {
    '0x004387C2': ('call', 0x462AFB),
    '0x00452E1C': ('call_mem', 0x4661AC),
    '0x004535BD': ('call_mem', 0x4661AC),
    '0x00457547': ('call_mem', 0x46614C),
    '0x00458E74': ('call_mem', 0x46614C),
    '0x0045A245': ('int3', None),
    '0x00459972': ('ret', None),
    '0x00462428': ('call', 0x462AFB),
    '0x004624E5': ('call', 0x462AFB),
}
DATA_ENTRY = ('0x004387C2', 0x47140C)
NESTED_HELPERS = (
    ('0x00452422', '0x00452475', '0x00452465'),
    ('0x00456BE8', '0x00456E03', '0x00456C95'),
    ('0x00457395', '0x004574FD', '0x004574A7'),
    ('0x00457629', '0x004576D8', '0x004576BB'),
    ('0x0045794D', '0x00457A13', '0x004579C1'),
    ('0x00459C27', '0x00459CAE', '0x00459CA1'),
    ('0x00459E8E', '0x00459F15', '0x00459F08'),
    ('0x00460028', '0x00460163', '0x00460146'),
    ('0x00461361', '0x004613E8', '0x004613DB'),
    ('0x0046218A', '0x00462201', '0x004621F4'),
)
SHARED_MATH_ENTRY = ('0x00463D83', ('0x00463D7B', '0x00463EFC'), '0x00463D8A')


def read_rows(name):
    with (ROOT / 'config' / name).open(newline='', encoding='utf-8') as stream:
        return list(csv.DictReader(stream))


def write_rows(name, rows):
    path = ROOT / 'config' / name
    prior_lines = path.read_text(encoding='utf-8').splitlines(keepends=True)
    prior_rows = list(csv.DictReader(prior_lines))
    if len(prior_lines) != len(rows) + 1 or len(prior_rows) != len(rows):
        raise ValueError(f'CSV physical rows changed unexpectedly: {path}')
    with path.open('w', newline='', encoding='utf-8') as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), lineterminator='\n')
        stream.write(prior_lines[0])
        for old, new, raw in zip(prior_rows, rows, prior_lines[1:]):
            if old['address'] != new['address']:
                raise ValueError(f'CSV row order changed unexpectedly: {path}')
            if old == new:
                stream.write(raw)
            else:
                writer.writerow(new)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply', action='store_true')
    args = parser.parse_args()
    target_path = resolve_target()
    _, problems = verify_target(target_path)
    if problems:
        raise ValueError(problems)
    image = target_path.read_bytes()
    functions = read_rows('functions.csv')
    boundaries = read_rows('function-boundaries.csv')
    by_address = {row['address']: row for row in functions}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    for address, reference in ENTRY_REFS.items():
        function = by_address[address]
        start, end = int(address, 0), int(function['span_end'], 0)
        ref = int(reference, 0)
        if start <= ref <= end:
            raise ValueError(f'entry reference is internal: {address}')
        if any(other['address'] != address and
               int(other['address'], 0) <= end and int(other['span_end'], 0) >= start
               for other in functions):
            raise ValueError(f'candidate overlaps another extent: {address}')
        instruction = next(decoder.disasm(pe_bytes_at(image, ref, 16), ref, count=1), None)
        if instruction is None or not (instruction.group(CS_GRP_CALL) or instruction.group(CS_GRP_JUMP)) or (
                len(instruction.operands) != 1 or instruction.operands[0].type != X86_OP_IMM or
                instruction.operands[0].imm != start):
            raise ValueError(f'entry reference is not a direct call/jump: {reference} -> {address}')
    data_entry, data_reference = DATA_ENTRY
    if int.from_bytes(pe_bytes_at(image, data_reference, 4), 'little') != int(data_entry, 0):
        raise ValueError('catch-handler metadata no longer points to its entry')
    for address, (terminal_kind, expected) in FOCUSED_TERMINALS.items():
        start = int(address, 0)
        size = int(by_address[address]['size'])
        instructions = list(decoder.disasm(pe_bytes_at(image, start, size), start))
        if sum(instruction.size for instruction in instructions) != size:
            raise ValueError(f'focused boundary is not a dense body: {address}')
        last = instructions[-1]
        if terminal_kind == 'call' and not (last.mnemonic == 'call' and
                len(last.operands) == 1 and last.operands[0].type == X86_OP_IMM and
                last.operands[0].imm == expected):
            raise ValueError(f'wrong terminal call at {address}')
        if terminal_kind == 'call_mem' and not (last.mnemonic == 'call' and
                len(last.operands) == 1 and last.operands[0].type == X86_OP_MEM and
                last.operands[0].mem.disp == expected):
            raise ValueError(f'wrong terminal import call at {address}')
        if terminal_kind in ('ret', 'int3') and last.mnemonic != terminal_kind:
            raise ValueError(f'wrong terminal instruction at {address}')
    # Ghidra's 5,022-byte span merges the acos entry with independent math
    # functions and a remote shared tail. IDA identifies the first 0xAE-byte
    # contiguous chunk; the target independently has a direct entry call and
    # a complete instruction stream ending in RET at that chunk boundary.
    fragment_address, fragment_reference, fragment_size = MATH_FRAGMENT
    fragment = by_address[fragment_address]
    start, ref = int(fragment_address, 0), int(fragment_reference, 0)
    caller = next(decoder.disasm(pe_bytes_at(image, ref, 16), ref, count=1), None)
    if caller is None or caller.mnemonic != 'call' or len(caller.operands) != 1 or (
            caller.operands[0].type != X86_OP_IMM or caller.operands[0].imm != start):
        raise ValueError('acos fragment has no independent direct entry call')
    instructions = list(decoder.disasm(pe_bytes_at(image, start, fragment_size), start))
    if sum(instruction.size for instruction in instructions) != fragment_size or (
            instructions[-1].mnemonic != 'ret'):
        raise ValueError('acos contiguous fragment does not end in RET')
    old_size = int(fragment['size'])
    if old_size not in (5022, fragment_size):
        raise ValueError(f'unexpected old acos fragment size: {old_size}')
    fragment['size'] = str(fragment_size)
    fragment['span_end'] = f'0x{start + fragment_size - 1:08X}'
    fragment_note = ('IDA MCP identifies the 174-byte contiguous acos entry; target direct '
                     'call at 0x00463BFB and dense decode through RET at 0x00463CBA confirm '
                     'its local extent. Shared exit chunks at 0x00464F9E and later are '
                     'noncontiguous; the old 5,022-byte span merged independent CRT functions.')
    if EVIDENCE_ID not in fragment['notes']:
        fragment['notes'] = (fragment['notes'] + ' ' + fragment_note + ' ' + EVIDENCE_ID).strip()
    nested_notes = {}
    for parent_address, child_address, reference in NESTED_HELPERS:
        parent, child = by_address[parent_address], by_address[child_address]
        parent_start, parent_end = int(parent_address, 0), int(parent['span_end'], 0)
        child_start, child_end = int(child_address, 0), int(child['span_end'], 0)
        ref = int(reference, 0)
        if not (parent_start <= ref < child_start <= child_end <= parent_end):
            raise ValueError(f'helper is not nested in its calling owner: {child_address}')
        call = next(decoder.disasm(pe_bytes_at(image, ref, 16), ref, count=1), None)
        if call is None or call.mnemonic != 'call' or len(call.operands) != 1 or (
                call.operands[0].type != X86_OP_IMM or call.operands[0].imm != child_start):
            raise ValueError(f'owner does not call its local helper: {reference}')
        for address, size in ((parent_start, int(parent['size'])),
                              (child_start, int(child['size']))):
            instructions = list(decoder.disasm(pe_bytes_at(image, address, size), address))
            if sum(instruction.size for instruction in instructions) != size or (
                    instructions[-1].mnemonic != 'ret'):
                raise ValueError(f'nested owner/helper is not a complete RET body: {address:#x}')
        nested_notes[parent_address] = (f'Contains local RET helper {child_address}; target call '
                                        f'at {reference} and IDA parent extent confirm intentional overlap')
        nested_notes[child_address] = (f'Local RET helper inside {parent_address}; target call '
                                       f'at {reference} confirms nested entry; IDA does not claim a separate owner')
    shared_address, shared_callers, next_entry = SHARED_MATH_ENTRY
    shared_start = int(shared_address, 0)
    shared_size = int(by_address[shared_address]['size'])
    if shared_size != 7 or shared_start + shared_size != int(next_entry, 0):
        raise ValueError('shared CRT math entry has unexpected extent')
    shared_instructions = list(decoder.disasm(pe_bytes_at(image, shared_start, shared_size), shared_start))
    if len(shared_instructions) != 1 or shared_instructions[0].mnemonic != 'and' or (
            shared_instructions[0].size != shared_size):
        raise ValueError('shared CRT math entry is no longer the one-instruction prefix')
    for caller_address in shared_callers:
        caller_start = int(caller_address, 0)
        call = next(decoder.disasm(pe_bytes_at(image, caller_start, 16), caller_start, count=1), None)
        if call is None or call.mnemonic != 'call' or len(call.operands) != 1 or (
                call.operands[0].type != X86_OP_IMM or call.operands[0].imm != shared_start):
            raise ValueError(f'CRT math entry lacks caller {caller_address}')
    nested_notes[shared_address] = ('Shared CRT math entry prefix called at '
                                    f'{shared_callers[0]} and {shared_callers[1]}; one '
                                    f'7-byte AND instruction falls through to {next_entry}')
    promoted = 0
    for row in boundaries:
        if row['address'] == fragment_address:
            row['span_end'] = fragment['span_end']
            if row['state'] != 'reviewed':
                row.update(state='reviewed', confidence='high',
                           evidence_id=row['evidence_id'] + '+' + EVIDENCE_ID,
                           notes=row['notes'] + '; ' + fragment_note)
                promoted += 1
            continue
        if row['address'] in nested_notes:
            if row['state'] != 'reviewed':
                if row['state'] != 'needs_review' or 'overlapping-extents' not in row['notes']:
                    raise ValueError(f'nested entry lacks prior overlap review: {row["address"]}')
                row.update(state='reviewed', confidence='high',
                           evidence_id=row['evidence_id'] + '+' + EVIDENCE_ID,
                           notes=row['notes'] + '; ' + nested_notes[row['address']])
                promoted += 1
            continue
        if row['address'] == data_entry and row['state'] != 'reviewed':
            if row['state'] != 'needs_review':
                raise ValueError('catch-handler entry lacks prior review state')
            row.update(state='reviewed', confidence='high',
                       evidence_id=row['evidence_id'] + '+' + EVIDENCE_ID,
                       notes=row['notes'] + '; target metadata pointer at 0x0047140C identifies the entry; complete local body ends in a C++ throw call')
            promoted += 1
            continue
        reference = ENTRY_REFS.get(row['address'])
        if reference is None or row['state'] == 'reviewed':
            continue
        if row['state'] == 'provisional' and row['notes'].startswith('Complete dense decode'):
            pass
        elif row['state'] == 'needs_review' and row['notes'].startswith('overlapping-extents'):
            start = int(row['address'], 0)
            size = int(by_address[row['address']]['size'])
            instructions = list(decoder.disasm(pe_bytes_at(image, start, size), start))
            if sum(instruction.size for instruction in instructions) != size or (
                    instructions[-1].mnemonic not in ('ret', 'jmp')):
                raise ValueError(f'former overlap lacks complete local code: {row["address"]}')
        elif row['state'] == 'needs_review' and row['address'] in FOCUSED_TERMINALS:
            pass
        else:
            raise ValueError(f'candidate lacks complete prior decode: {row["address"]}')
        ida_note = ('IDA MCP does not define this separate entry; target local extent is '
                    'independently dense through its terminal instruction'
                    if row['address'] in IDA_OMITTED_ENTRIES else
                    'independent IDA MCP extent agrees')
        row.update(state='reviewed', confidence='high',
                   evidence_id=row['evidence_id'] + '+' + EVIDENCE_ID,
                   notes=row['notes'] + f'; direct target call/jump at {reference} to entry; {ida_note}')
        promoted += 1
    if args.apply:
        write_rows('functions.csv', functions)
        write_rows('function-boundaries.csv', boundaries)
    count = len(ENTRY_REFS) + 1 + len(NESTED_HELPERS) + len(shared_callers) + 1
    print(f'{count} target entry references verified; {promoted} boundary reviews pending write' if not args.apply
          else f'{count} target entry references verified; {promoted} boundaries promoted')


if __name__ == '__main__':
    main()
