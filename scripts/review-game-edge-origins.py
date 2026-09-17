"""Review game origins using fixed authored call edges and target data references."""

import argparse
from collections import Counter, defaultdict
import csv
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_GRP_CALL, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

from target_identity import parse_pe, pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
CALL_EVIDENCE_ID = 'target-authored-call-sandwich-2026-09-17'
DATA_EVIDENCE_ID = 'target-authored-data-and-edge-2026-09-17'
GAME_CODE_END = 0x00452000
COMMON_FLOAT_ONE = 0x00476F78
EXPECTED_CALLS = 11
EXPECTED_DATA = 18


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


def inspect_body(decoder, image, address, size, entries, data_start, data_end):
    instructions = list(decoder.disasm(pe_bytes_at(image, address, size), address))
    if sum(instruction.size for instruction in instructions) != size:
        return None
    data_refs = set()
    calls = set()
    for instruction in instructions:
        for operand in instruction.operands:
            value = None
            if operand.type == X86_OP_IMM:
                value = operand.imm
            elif (operand.type == X86_OP_MEM and operand.mem.base == 0
                  and operand.mem.index == 0):
                value = operand.mem.disp
            if value is not None and data_start <= value < data_end:
                data_refs.add(value)
        if (instruction.group(CS_GRP_CALL) and instruction.operands
                and instruction.operands[0].type == X86_OP_IMM
                and instruction.operands[0].imm in entries):
            calls.add(instruction.operands[0].imm)
    return data_refs, calls


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply', action='store_true')
    args = parser.parse_args()
    target_path = resolve_target()
    _, problems = verify_target(target_path)
    if problems:
        raise ValueError(problems)
    image = target_path.read_bytes()
    pe = parse_pe(image)
    data = next(section for section in pe['sections'] if section['name'] == '.data')
    data_start = pe['image_base'] + data['rva']
    data_end = data_start + data['virtual_size']
    if pe_bytes_at(image, COMMON_FLOAT_ONE, 4) != b'\x00\x00\x80\x3f':
        raise ValueError('common float-one constant changed')
    functions = read_rows('functions.csv')
    origins = read_rows('function-origins.csv')
    by_origin = {row['address']: row for row in origins}
    entries = {int(row['address'], 0) for row in functions}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    facts = {}
    incoming = defaultdict(set)
    for row in functions:
        address = int(row['address'], 0)
        item = inspect_body(decoder, image, address, int(row['size']), entries,
                            data_start, data_end)
        facts[address] = item
        if item:
            for callee in item[1]:
                incoming[callee].add(address)
    # The earlier 320 reviewed authored owners are a fixed, non-circular seed.
    seeds = {int(row['address'], 0) for row in origins
             if row['origin'] == 'authored_game' and row['disposition'] == 'authored'
             and not row['evidence_id'].startswith('target-authored-')}
    excluded = {int(row['address'], 0) for row in origins
                if row['disposition'] == 'exclude'}
    use_count = Counter(value for address in seeds if facts[address]
                        for value in facts[address][0])
    excluded_data = {value for address in excluded if facts[address]
                     for value in facts[address][0]}
    excluded_data.add(COMMON_FLOAT_ONE)
    call_reviews = []
    data_reviews = []
    for row in functions:
        address = int(row['address'], 0)
        size = int(row['size'])
        origin = by_origin[row['address']]
        if (origin['disposition'] != 'review'
                and origin['evidence_id'] not in (CALL_EVIDENCE_ID, DATA_EVIDENCE_ID)):
            continue
        item = facts[address]
        if address >= GAME_CODE_END or size < 32 or not item:
            continue
        callees = sorted(item[1] & seeds)
        callers = sorted(incoming[address] & seeds)
        shared = sorted(value for value in item[0]
                        if use_count[value] >= 2 and value not in excluded_data)
        if size >= 64 and callees and callers:
            evidence_id = CALL_EVIDENCE_ID
            call_reviews.append((row['address'], size))
            note = (f'Complete target decode; reviewed authored 0x{callers[0]:08X} '
                    f'directly calls this entry, which directly calls reviewed authored '
                    f'0x{callees[0]:08X} ({evidence_id}).')
        elif shared and (callees or callers):
            evidence_id = DATA_EVIDENCE_ID
            data_reviews.append((row['address'], size))
            edge = (f'calls reviewed authored 0x{callees[0]:08X}' if callees else
                    f'is called by reviewed authored 0x{callers[0]:08X}')
            note = (f'Complete target decode; {edge} and references target .data '
                    f'address 0x{shared[0]:08X}, used by at least two earlier '
                    f'reviewed authored bodies and no excluded body ({evidence_id}).')
        else:
            continue
        if row['source_file'] or row['owner'] not in ('', 'authored'):
            raise ValueError(f'candidate already has competing source/owner: {row["address"]}')
        if args.apply:
            origin.update(origin='authored_game', subsystem='GameUnassigned',
                          disposition='authored', confidence='medium',
                          evidence_id=evidence_id)
            row['owner'] = 'authored'
            if evidence_id not in row['notes']:
                row['notes'] = (row['notes'] + ' ' + note +
                                ' Subsystem and source unit remain unknown.').strip()
    if len(call_reviews) != EXPECTED_CALLS or len(data_reviews) != EXPECTED_DATA:
        raise ValueError(f'expected {EXPECTED_CALLS}/{EXPECTED_DATA} call/data '
                         f'origins; found {len(call_reviews)}/{len(data_reviews)}')
    if args.apply:
        write_rows('function-origins.csv', origins)
        write_rows('functions.csv', functions)
    print(f'{len(call_reviews)} call-sandwich origins, '
          f'{sum(size for _, size in call_reviews)} target bytes')
    print(f'{len(data_reviews)} data-and-edge origins, '
          f'{sum(size for _, size in data_reviews)} target bytes')


if __name__ == '__main__':
    main()
