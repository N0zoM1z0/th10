"""Review target VC7.1 scalar deleting-destructor wrappers."""

import argparse
import csv
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_GRP_CALL, CS_MODE_32
from capstone.x86 import X86_OP_IMM

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'target-vc71-scalar-deleting-destructor-2026-09-17'
WRAPPERS = {
    0x0044C100: (36, 0x0044C150, 'game worker object'),
    0x0044CF00: (30, 0x0044D080, 'sound stream aggregate'),
    0x0044D710: (30, 0x0044D730, 'derived sound stream object'),
}


def read_rows(name):
    path = ROOT / 'config' / name
    lines = path.read_text(encoding='utf-8').splitlines(keepends=True)
    return path, lines, list(csv.DictReader(lines))


def write_rows(path, lines, old_rows, new_rows):
    with path.open('w', newline='', encoding='utf-8') as stream:
        writer = csv.DictWriter(stream, fieldnames=list(old_rows[0]), lineterminator='\n')
        stream.write(lines[0])
        for old, new, raw in zip(old_rows, new_rows, lines[1:]):
            if old['address'] != new['address']:
                raise ValueError(f'ledger row order changed: {old["address"]}')
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
    function_path, function_lines, functions = read_rows('functions.csv')
    origin_path, origin_lines, origins = read_rows('function-origins.csv')
    new_functions = [row.copy() for row in functions]
    new_origins = [row.copy() for row in origins]
    by_function = {int(row['address'], 0): row for row in new_functions}
    by_origin = {int(row['address'], 0): row for row in new_origins}
    for address, (size, destructor, role) in WRAPPERS.items():
        row, origin = by_function[address], by_origin[address]
        if (int(row['size']) != size or row['source_file']
                or row['owner'] not in ('', 'compiler')
                or (origin['disposition'] != 'review'
                    and origin['evidence_id'] != EVIDENCE_ID)):
            raise ValueError(f'deleting destructor ledger conflict: {address:#x}')
        instructions = list(decoder.disasm(pe_bytes_at(image, address, size), address))
        if sum(item.size for item in instructions) != size:
            raise ValueError(f'deleting destructor body changed: {address:#x}')
        calls = {item.operands[0].imm for item in instructions
                 if item.group(CS_GRP_CALL) and item.operands
                 and item.operands[0].type == X86_OP_IMM}
        tests_flag = any(item.mnemonic == 'test'
                         and item.op_str == 'byte ptr [esp + 8], 1'
                         for item in instructions)
        if (destructor not in calls or 0x004524A1 not in calls
                or not tests_flag or instructions[-1].mnemonic != 'ret'
                or instructions[-1].op_str != '4'):
            raise ValueError(f'scalar deleting-destructor pattern changed: {address:#x}')
        if args.apply:
            origin.update(origin='compiler_generated', subsystem='GameCxx',
                          disposition='exclude', confidence='high',
                          evidence_id=EVIDENCE_ID)
            row['owner'] = 'compiler'
            if EVIDENCE_ID not in row['notes']:
                note = (f'Complete VC7.1 scalar deleting-destructor wrapper for '
                        f'{role}: calls 0x{destructor:08X}, checks the deletion '
                        f'flag and conditionally frees the object before RET 4 '
                        f'({EVIDENCE_ID}). Underlying destructor origin is '
                        f'separate and remains unknown.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    if args.apply:
        write_rows(function_path, function_lines, functions, new_functions)
        write_rows(origin_path, origin_lines, origins, new_origins)
    print(f'{len(WRAPPERS)} VC7.1 scalar deleting-destructor origins reviewed')


if __name__ == '__main__':
    main()
