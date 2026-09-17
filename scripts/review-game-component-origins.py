"""Replay individually inspected game-component origins through fixed call edges."""

import argparse
import csv
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_GRP_CALL, CS_MODE_32
from capstone.x86 import X86_OP_IMM

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'target-ida-reviewed-game-component-2026-09-17'
# Each entry was inspected in IDA; its anchor is an independently reviewed
# authored owner outside this campaign. Direction is relative to the entry.
COMPONENTS = {
    0x004349E0: (59, 'game text token scan', 0x00433EF0, 'incoming'),
    0x00434A20: (92, 'game text token copy', 0x00433EF0, 'incoming'),
    0x00434A80: (206, 'game render object state assignment', 0x00433EF0, 'incoming'),
    0x00436A30: (107, 'GDI glyph bitmap cleanup', 0x00437D10, 'incoming'),
    0x00436CA0: (72, 'GDI glyph bitmap color fill', 0x00437FE0, 'incoming'),
    0x00436DA0: (946, 'GDI glyph bitmap color conversion', 0x00437DB0, 'incoming'),
    0x00437160: (157, 'GDI glyph bitmap alpha inversion', 0x00437DB0, 'incoming'),
    0x0043AA60: (107, 'game MIDI track buffer cleanup', 0x0043A910, 'incoming'),
    0x0043B110: (1323, 'game MIDI event dispatch', 0x0043AEB0, 'outgoing'),
    0x0043C970: (357, 'game sprite rendering state setup', 0x0043C8B0, 'incoming'),
    0x0043D790: (55, 'game sound filename stream lookup', 0x0043D2A0, 'outgoing'),
    0x00436AF0: (421, 'GDI glyph bitmap creation', 0x00436A30, 'outgoing'),
    0x00437A00: (781, 'GDI game font and bitmap setup', 0x00436AF0, 'outgoing'),
    0x0043A730: (26, 'MIDI variable-length quantity decode', 0x0043B110, 'incoming'),
    0x0043AAD0: (448, 'MIDI track header decode', 0x0043AA60, 'outgoing'),
    0x0043AF30: (473, 'MIDI timed event runner', 0x0043B110, 'outgoing'),
    0x0043ACE0: (132, 'MIDI track cursor reset', 0x0043AF30, 'incoming'),
    0x0043B7A0: (140, 'MIDI channel volume update', 0x0043AF30, 'incoming'),
}


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


def direct_calls(decoder, image, address, size):
    instructions = list(decoder.disasm(pe_bytes_at(image, address, size), address))
    if sum(instruction.size for instruction in instructions) != size:
        raise ValueError(f'body does not completely decode: {address:#x}')
    return {instruction.operands[0].imm for instruction in instructions
            if instruction.group(CS_GRP_CALL) and instruction.operands
            and instruction.operands[0].type == X86_OP_IMM}


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
    by_function = {int(row['address'], 0): row for row in functions}
    by_origin = {int(row['address'], 0): row for row in origins}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    reviewed_anchors = {address for address, origin in by_origin.items()
                        if origin['disposition'] == 'authored'
                        and origin['origin'] == 'authored_game'
                        and address not in COMPONENTS}
    for address, (size, behavior, anchor, direction) in COMPONENTS.items():
        row = by_function[address]
        origin = by_origin[address]
        anchor_row = by_function[anchor]
        if (int(row['size']) != size or anchor not in reviewed_anchors
                or (origin['disposition'] != 'review'
                    and origin['evidence_id'] != EVIDENCE_ID)
                or row['source_file'] or row['owner'] not in ('', 'authored')):
            raise ValueError(f'game component ledger conflict: {address:#x}')
        calls = direct_calls(decoder, image, address, size)
        anchor_calls = direct_calls(decoder, image, anchor, int(anchor_row['size']))
        if ((direction == 'outgoing' and anchor not in calls)
                or (direction == 'incoming' and address not in anchor_calls)):
            raise ValueError(f'game component direct call changed: {address:#x}')
        if args.apply:
            origin.update(origin='authored_game', subsystem='GameUnassigned',
                          disposition='authored', confidence='medium',
                          evidence_id=EVIDENCE_ID)
            row['owner'] = 'authored'
            if EVIDENCE_ID not in row['notes']:
                edge = ('calls' if direction == 'outgoing' else 'is called by')
                note = (f'Complete target decode; directly {edge} reviewed authored '
                        f'0x{anchor:08X}. IDA behavior review shows {behavior} '
                        f'({EVIDENCE_ID}). Original source unit and exact '
                        f'codegen remain unknown.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
        reviewed_anchors.add(address)
    if args.apply:
        write_rows('function-origins.csv', origins)
        write_rows('functions.csv', functions)
    print(f'{len(COMPONENTS)} individually reviewed game-component origins; '
          f'{sum(size for size, *_ in COMPONENTS.values())} target bytes')


if __name__ == '__main__':
    main()
