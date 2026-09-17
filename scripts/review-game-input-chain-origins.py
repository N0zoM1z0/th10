"""Review the target-linked game input initialization and callback chain."""

import argparse
import csv
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_GRP_CALL, CS_MODE_32
from capstone.x86 import X86_OP_IMM

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'target-ida-game-input-chain-2026-09-17'
# Order is deliberate: an existing authored owner anchors the first entry,
# then each direct call or function pointer anchors the next source body.
CHAIN = (
    (0x004201B0, 186, 0x00413350, 'call_out', 'game initialization and input-thread setup'),
    (0x0043BA90, 83, 0x00420222, 'pointer', 'game input polling thread entry'),
    (0x0043B8D0, 439, 0x0043BA90, 'call_in', 'game DirectInput initialization'),
    (0x0043BB30, 98, 0x0043BA66, 'pointer', 'game DirectInput device callback'),
)
POINTER_OWNERS = {0x0043BA90: 0x004201B0, 0x0043BB30: 0x0043B8D0}


def read_rows(name):
    path = ROOT / 'config' / name
    lines = path.read_text(encoding='utf-8').splitlines(keepends=True)
    rows = list(csv.DictReader(lines))
    if len(lines) != len(rows) + 1:
        raise ValueError(f'CSV physical rows changed: {name}')
    return path, lines, rows


def write_rows(path, lines, before, after):
    with path.open('w', newline='', encoding='utf-8') as stream:
        writer = csv.DictWriter(stream, fieldnames=list(before[0]), lineterminator='\n')
        stream.write(lines[0])
        for old, new, raw in zip(before, after, lines[1:]):
            if old['address'] != new['address']:
                raise ValueError(f'ledger row order changed: {old["address"]}')
            if old == new:
                stream.write(raw)
            else:
                writer.writerow(new)


def decoded(decoder, image, address, size):
    instructions = list(decoder.disasm(pe_bytes_at(image, address, size), address))
    if sum(item.size for item in instructions) != size or instructions[-1].mnemonic != 'ret':
        raise ValueError(f'input-chain body changed: {address:#x}')
    return instructions


def direct_calls(instructions):
    return {item.operands[0].imm for item in instructions
            if item.group(CS_GRP_CALL) and item.operands
            and item.operands[0].type == X86_OP_IMM}


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
    revised_functions = [row.copy() for row in functions]
    revised_origins = [row.copy() for row in origins]
    by_function = {int(row['address'], 0): row for row in revised_functions}
    by_origin = {int(row['address'], 0): row for row in revised_origins}
    reviewed = {address for address, row in by_origin.items()
                if row['origin'] == 'authored_game' and row['disposition'] == 'authored'
                and address not in {item[0] for item in CHAIN}}
    for address, size, anchor, edge, behavior in CHAIN:
        row, origin = by_function[address], by_origin[address]
        if (int(row['size']) != size or row['source_file']
                or row['owner'] not in ('', 'authored')
                or (origin['disposition'] != 'review'
                    and origin['evidence_id'] != EVIDENCE_ID)):
            raise ValueError(f'input-chain ledger conflict: {address:#x}')
        instructions = decoded(decoder, image, address, size)
        if edge == 'call_out':
            if anchor not in reviewed or anchor not in direct_calls(instructions):
                raise ValueError(f'input-chain authored call changed: {address:#x}')
            evidence = f'directly calls reviewed authored 0x{anchor:08X}'
        elif edge == 'call_in':
            if (anchor not in reviewed or address not in direct_calls(decoded(
                    decoder, image, anchor, int(by_function[anchor]['size'])))):
                raise ValueError(f'input-chain inbound call changed: {address:#x}')
            evidence = f'is directly called by reviewed authored 0x{anchor:08X}'
        else:
            owner = POINTER_OWNERS[address]
            if (owner not in reviewed
                    or not owner <= anchor < owner + int(by_function[owner]['size'])):
                raise ValueError(f'input-chain pointer owner changed: {address:#x}')
            instruction = next(decoder.disasm(pe_bytes_at(image, anchor, 8), anchor))
            if address not in (operand.imm for operand in instruction.operands
                               if operand.type == X86_OP_IMM):
                raise ValueError(f'input-chain function pointer changed: {anchor:#x}')
            evidence = f'is passed as a function pointer by reviewed authored 0x{owner:08X} at 0x{anchor:08X}'
        if args.apply:
            origin.update(origin='authored_game', subsystem='GameUnassigned',
                          disposition='authored', confidence='medium',
                          evidence_id=EVIDENCE_ID)
            row['owner'] = 'authored'
            if EVIDENCE_ID not in row['notes']:
                note = (f'Complete target decode; {evidence}. IDA behavior review '
                        f'shows {behavior} ({EVIDENCE_ID}). Source unit and '
                        f'exact codegen remain unknown.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
        reviewed.add(address)
    if args.apply:
        write_rows(function_path, function_lines, functions, revised_functions)
        write_rows(origin_path, origin_lines, origins, revised_origins)
    print(f'{len(CHAIN)} individually reviewed game input-chain origins')


if __name__ == '__main__':
    main()
