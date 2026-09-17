"""Review authored origins with independent target call and game-global evidence."""

import argparse
from collections import Counter
from collections import defaultdict
import csv
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_GRP_CALL, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'target-authored-call-and-globals-2026-09-17'
NEIGHBOR_EVIDENCE_ID = 'target-authored-neighbor-and-global-2026-09-17'
GAME_GLOBAL_START = 0x00491000
GAME_GLOBAL_END = 0x00493000
GAME_CODE_END = 0x00452000
EXPECTED_COUNT = 46
EXPECTED_NEIGHBOR_COUNT = 77


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


def inspect_body(decoder, image, address, size, entries):
    code = pe_bytes_at(image, address, size)
    instructions = list(decoder.disasm(code, address))
    if sum(instruction.size for instruction in instructions) != size:
        return None
    globals_used = set()
    direct_calls = set()
    for instruction in instructions:
        for operand in instruction.operands:
            value = None
            if operand.type == X86_OP_IMM:
                value = operand.imm
            elif (operand.type == X86_OP_MEM and operand.mem.base == 0
                  and operand.mem.index == 0):
                value = operand.mem.disp
            if value is not None and GAME_GLOBAL_START <= value < GAME_GLOBAL_END:
                globals_used.add(value)
        if (instruction.group(CS_GRP_CALL) and instruction.operands
                and instruction.operands[0].type == X86_OP_IMM
                and instruction.operands[0].imm in entries):
            direct_calls.add(instruction.operands[0].imm)
    return globals_used, direct_calls


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
    origins = read_rows('function-origins.csv')
    by_origin = {row['address']: row for row in origins}
    entries = {int(row['address'], 0) for row in functions}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    evidence = {}
    for row in functions:
        address = int(row['address'], 0)
        evidence[address] = inspect_body(decoder, image, address, int(row['size']), entries)

    # Never use this pass's own classifications as seeds on a later replay.
    seeds = {int(row['address'], 0) for row in origins
             if row['origin'] == 'authored_game' and row['disposition'] == 'authored'
             and row['evidence_id'] not in (EVIDENCE_ID, NEIGHBOR_EVIDENCE_ID)}
    excluded = {int(row['address'], 0) for row in origins
                if row['disposition'] == 'exclude'}
    use_count = Counter(value for address in seeds if evidence[address]
                        for value in evidence[address][0])
    excluded_globals = {value for address in excluded if evidence[address]
                        for value in evidence[address][0]}
    incoming = defaultdict(set)
    for caller, facts in evidence.items():
        if facts:
            for callee in facts[1]:
                incoming[callee].add(caller)
    reviewed = []
    for row in functions:
        address = int(row['address'], 0)
        origin = by_origin[row['address']]
        if origin['disposition'] != 'review' and origin['evidence_id'] != EVIDENCE_ID:
            continue
        facts = evidence[address]
        if address >= GAME_CODE_END or int(row['size']) < 64 or not facts:
            continue
        shared = sorted(value for value in facts[0]
                        if use_count[value] >= 2 and value not in excluded_globals)
        authored_calls = sorted(facts[1] & seeds)
        if len(shared) < 2 or not authored_calls:
            continue
        if row['source_file'] or row['owner'] not in ('', 'authored'):
            raise ValueError(f'candidate already has a competing source/owner: {row["address"]}')
        reviewed.append((row['address'], int(row['size'])))
        if args.apply:
            origin.update(origin='authored_game', subsystem='GameUnassigned',
                          disposition='authored', confidence='medium',
                          evidence_id=EVIDENCE_ID)
            row['owner'] = 'authored'
            if EVIDENCE_ID not in row['notes']:
                note = (f'Complete target decode; directly calls reviewed authored '
                        f'0x{authored_calls[0]:08X} and references shared game globals '
                        f'0x{shared[0]:08X}, 0x{shared[1]:08X} '
                        f'({EVIDENCE_ID}). Subsystem and source unit remain unknown.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    if len(reviewed) != EXPECTED_COUNT:
        raise ValueError(f'expected {EXPECTED_COUNT} conservative game candidates, '
                         f'found {len(reviewed)}')
    neighbors = []
    for row in functions:
        address = int(row['address'], 0)
        origin = by_origin[row['address']]
        if (origin['disposition'] != 'review'
                and origin['evidence_id'] != NEIGHBOR_EVIDENCE_ID):
            continue
        facts = evidence[address]
        if address >= GAME_CODE_END or int(row['size']) < 32 or not facts:
            continue
        shared = sorted(value for value in facts[0]
                        if use_count[value] >= 2 and value not in excluded_globals)
        authored_calls = sorted(facts[1] & seeds)
        authored_callers = sorted(incoming[address] & seeds)
        if not shared or not (authored_calls or authored_callers):
            continue
        if row['source_file'] or row['owner'] not in ('', 'authored'):
            raise ValueError(f'candidate already has a competing source/owner: {row["address"]}')
        neighbors.append((row['address'], int(row['size'])))
        if args.apply:
            origin.update(origin='authored_game', subsystem='GameUnassigned',
                          disposition='authored', confidence='medium',
                          evidence_id=NEIGHBOR_EVIDENCE_ID)
            row['owner'] = 'authored'
            if NEIGHBOR_EVIDENCE_ID not in row['notes']:
                if authored_calls:
                    edge = f'directly calls reviewed authored 0x{authored_calls[0]:08X}'
                else:
                    edge = f'is directly called by reviewed authored 0x{authored_callers[0]:08X}'
                note = (f'Complete target decode; {edge} and references shared '
                        f'game global 0x{shared[0]:08X} '
                        f'({NEIGHBOR_EVIDENCE_ID}). Subsystem and source unit remain unknown.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    if len(neighbors) != EXPECTED_NEIGHBOR_COUNT:
        raise ValueError(f'expected {EXPECTED_NEIGHBOR_COUNT} neighbor candidates, '
                         f'found {len(neighbors)}')
    if args.apply:
        write_rows('function-origins.csv', origins)
        write_rows('functions.csv', functions)
    print(f'{len(reviewed)} hash-attested authored game origins reviewed; '
          f'{sum(size for _, size in reviewed)} target bytes')
    print(f'{len(neighbors)} neighbor-and-global authored origins reviewed; '
          f'{sum(size for _, size in neighbors)} target bytes')


if __name__ == '__main__':
    main()
