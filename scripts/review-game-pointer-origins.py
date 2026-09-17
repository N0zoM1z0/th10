"""Review two game worker callbacks passed by authored owners as target pointers."""

import argparse
import csv
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'target-ida-game-worker-pointer-2026-09-17'
# Callback, size, immediate-pointer site, reviewed authored caller, IDA role.
CALLBACKS = {
    0x0043CCF0: (57, 0x00438C08, 0x00438AD0, 'game sound setup thread entry'),
    0x0043E3A0: (189, 0x0043D704, 0x0043D690, 'game sound stream worker entry'),
}


def rows(name):
    with (ROOT / 'config' / name).open(newline='', encoding='utf-8') as stream:
        return list(csv.DictReader(stream))


def write_changed(name, before, after):
    path = ROOT / 'config' / name
    lines = path.read_text(encoding='utf-8').splitlines(keepends=True)
    if len(lines) != len(before) + 1 or len(before) != len(after):
        raise ValueError(f'ledger row count changed: {name}')
    with path.open('w', newline='', encoding='utf-8') as stream:
        writer = csv.DictWriter(stream, fieldnames=list(before[0]), lineterminator='\n')
        stream.write(lines[0])
        for old, new, raw in zip(before, after, lines[1:]):
            if old['address'] != new['address']:
                raise ValueError(f'ledger row order changed: {name}')
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
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    functions = rows('functions.csv')
    origins = rows('function-origins.csv')
    revised_functions = [row.copy() for row in functions]
    revised_origins = [row.copy() for row in origins]
    by_function = {int(row['address'], 0): row for row in revised_functions}
    by_origin = {int(row['address'], 0): row for row in revised_origins}
    for address, (size, site, caller, behavior) in CALLBACKS.items():
        row = by_function[address]
        origin = by_origin[address]
        parent = by_function[caller]
        parent_origin = by_origin[caller]
        if (int(row['size']) != size or not caller <= site < caller + int(parent['size'])
                or parent_origin['origin'] != 'authored_game'
                or parent_origin['disposition'] != 'authored'
                or (origin['disposition'] != 'review'
                    and origin['evidence_id'] != EVIDENCE_ID)
                or row['source_file'] or row['owner'] not in ('', 'authored')):
            raise ValueError(f'game pointer ledger conflict: {address:#x}')
        code = pe_bytes_at(image, address, size)
        instructions = list(decoder.disasm(code, address))
        if (sum(item.size for item in instructions) != size
                or instructions[-1].mnemonic != 'ret'):
            raise ValueError(f'game pointer target extent changed: {address:#x}')
        instruction = next(decoder.disasm(pe_bytes_at(image, site, 8), site))
        if address not in (operand.imm for operand in instruction.operands
                           if operand.type == X86_OP_IMM):
            raise ValueError(f'game callback pointer changed: {site:#x}')
        if args.apply:
            origin.update(origin='authored_game', subsystem='GameUnassigned',
                          disposition='authored', confidence='medium',
                          evidence_id=EVIDENCE_ID)
            row['owner'] = 'authored'
            if EVIDENCE_ID not in row['notes']:
                note = (f'Complete target decode; reviewed authored '
                        f'0x{caller:08X} passes this entry pointer at '
                        f'0x{site:08X}. IDA behavior review shows {behavior} '
                        f'({EVIDENCE_ID}). Source unit and exactness remain unknown.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    if args.apply:
        write_changed('functions.csv', functions, revised_functions)
        write_changed('function-origins.csv', origins, revised_origins)
    print(f'{len(CALLBACKS)} target-pointer-backed game worker origins')


if __name__ == '__main__':
    main()
