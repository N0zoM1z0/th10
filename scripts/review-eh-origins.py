"""Classify unwind actions reached from target VC7.1 C++ FuncInfo maps."""

import argparse
import csv
from pathlib import Path
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_32

from target_identity import parse_pe, pe_bytes_at, resolve_target, verify_target

ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'target-cxx-eh-unwind-map-2026-09-17'
FUNC_INFO_MAGIC = 0x19930520


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


def unwind_actions(image):
    pe = parse_pe(image)
    section = next(item for item in pe['sections'] if item['name'] == '.rdata')
    data = image[section['raw_offset']:section['raw_offset'] + section['raw_size']]
    start = pe['image_base'] + section['rva']
    stop = start + len(data)
    actions = {}
    info_count = 0
    for offset in range(0, len(data) - 12, 4):
        if struct.unpack_from('<I', data, offset)[0] != FUNC_INFO_MAGIC:
            continue
        max_state, table = struct.unpack_from('<II', data, offset + 4)
        if not (0 < max_state < 100 and start <= table and table + max_state * 8 <= stop):
            continue
        entries = [struct.unpack('<iI', pe_bytes_at(image, table + index * 8, 8))
                   for index in range(max_state)]
        if not all(-1 <= state < max_state for state, _ in entries):
            continue
        info_count += 1
        for index, (_, action) in enumerate(entries):
            if action:
                actions.setdefault(action, []).append((start + offset, table + index * 8))
    return info_count, actions


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply', action='store_true')
    args = parser.parse_args()
    target_path = resolve_target()
    _, problems = verify_target(target_path)
    if problems:
        raise ValueError(problems)
    image = target_path.read_bytes()
    info_count, actions = unwind_actions(image)
    functions = read_rows('functions.csv')
    origins = read_rows('function-origins.csv')
    by_origin = {row['address']: row for row in origins}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    reviewed = []
    for row in functions:
        if not row['current_name'].startswith('Unwind@'):
            continue
        address = int(row['address'], 0)
        references = actions.get(address)
        if not references:
            raise ValueError(f'Unwind entry lacks target FuncInfo map: {row["address"]}')
        size = int(row['size'])
        code = pe_bytes_at(image, address, size)
        instructions = list(decoder.disasm(code, address))
        if sum(instruction.size for instruction in instructions) != size or (
                instructions[-1].mnemonic not in ('ret', 'jmp')):
            raise ValueError(f'Unwind action is not a complete local body: {row["address"]}')
        origin = by_origin[row['address']]
        if origin['disposition'] not in ('review', 'exclude') or (
                origin['disposition'] == 'exclude' and origin['evidence_id'] != EVIDENCE_ID):
            raise ValueError(f'Unwind origin has competing evidence: {row["address"]}')
        reviewed.append(row['address'])
        if args.apply:
            origin.update(origin='compiler_generated', subsystem='CxxEH',
                          disposition='exclude', confidence='high', evidence_id=EVIDENCE_ID)
            if row['owner'] not in ('', 'compiler_generated'):
                raise ValueError(f'refusing to replace function owner: {row["address"]}')
            row.update(module='CxxEH', status='excluded', owner='compiler_generated')
            if EVIDENCE_ID not in row['notes']:
                info, table_entry = references[0]
                note = (f'VC7.1 C++ FuncInfo 0x{info:08X} unwind-map entry '
                        f'0x{table_entry:08X} points to this generated cleanup action; '
                        f'complete target code ends in {instructions[-1].mnemonic} ({EVIDENCE_ID}).')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    if len(reviewed) != 62:
        raise ValueError(f'expected 62 current unwind actions, found {len(reviewed)}')
    if args.apply:
        write_rows('function-origins.csv', origins)
        write_rows('functions.csv', functions)
    print(f'{info_count} VC7.1 FuncInfo records; {len(reviewed)} unwind actions reviewed')


if __name__ == '__main__':
    main()
