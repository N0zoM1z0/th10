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
    0x00441EF0: (30, 'game polar-to-Cartesian vector update', 0x0043EE30, 'incoming'),
    0x00449630: (21, 'game resource handle release', 0x0040E770, 'incoming'),
    0x00449AA0: (59, 'game scheduler node reset', 0x00438AD0, 'incoming'),
    0x00449E50: (82, 'game scheduler node cleanup', 0x00438880, 'incoming'),
    0x00449ED0: (74, 'game scheduler node allocation', 0x00402230, 'incoming'),
    0x00449F60: (114, 'game scheduler node removal', 0x00401260, 'incoming'),
    0x0044A000: (43, 'game sprite task node insertion', 0x0043C970, 'incoming'),
    0x0044A030: (43, 'game sprite task node insertion', 0x0043C970, 'incoming'),
    0x0044A190: (833, 'game joystick state and button mapping', 0x0044A5F0, 'incoming'),
    0x0044B080: (75, 'game keyboard state reset', 0x00438AD0, 'incoming'),
    0x0044B740: (76, 'game hint-file write and close', 0x0041A200, 'incoming'),
    0x0044B810: (201, 'game shared log formatting', 0x00401110, 'incoming'),
    0x0044B8E0: (201, 'game shared log formatting with state flag', 0x00438AD0, 'incoming'),
    0x0044BE20: (66, 'game timer state push', 0x00422C80, 'incoming'),
    0x0044BE70: (47, 'game timer state pop', 0x00422C80, 'incoming'),
    0x0044BEA0: (128, 'game timer advance and wrap', 0x00422C80, 'incoming'),
    0x0044C150: (101, 'game worker-thread cleanup', 0x0040B3A0, 'incoming'),
    0x0044C1C0: (56, 'game worker-thread launch', 0x0040BD80, 'incoming'),
    0x0044C2A0: (171, 'game moving-object position update', 0x0040DC80, 'incoming'),
    0x0044C5D0: (30, 'game polar-to-Cartesian vector update', 0x0040DC80, 'incoming'),
    0x0044C8F0: (702, 'game sound stream object setup', 0x0043D690, 'incoming'),
    0x0044CBF0: (651, 'game sound stream object setup', 0x0043D950, 'incoming'),
    0x0044CF20: (338, 'game sound stream state cleanup', 0x0043DDF0, 'incoming'),
    0x0044D110: (483, 'game sound stream read and refill', 0x0043DDF0, 'incoming'),
    0x0044D440: (145, 'game sound stream playback transition', 0x0043DDF0, 'incoming'),
    0x0044D4E0: (106, 'game sound stream volume update', 0x0043DDF0, 'incoming'),
    0x0044D550: (82, 'game sound stream stop', 0x0043DAB0, 'incoming'),
    0x0044D5B0: (29, 'game sound stream flag update', 0x0043DDF0, 'incoming'),
    0x0044D5D0: (41, 'game sound stream playback state query', 0x0043DDF0, 'incoming'),
    0x0044DAD0: (213, 'game sound stream restart', 0x0043DDF0, 'incoming'),
    0x0044DCA0: (78, 'game sound filename stream offset update', 0x0043DDF0, 'incoming'),
    0x0044CE80: (118, 'game sound stream aggregate construction', 0x0044D110, 'outgoing'),
    0x0044D300: (98, 'game sound stream cursor query', 0x0044D440, 'incoming'),
    0x0044D370: (103, 'game sound stream buffer selection', 0x0044D440, 'incoming'),
    0x0044D600: (61, 'game sound stream buffer stop', 0x0044D440, 'incoming'),
    0x0044D6A0: (109, 'game sound stream object construction', 0x0044C8F0, 'incoming'),
    0x0044D850: (637, 'game sound stream refill callback', 0x0044D110, 'outgoing'),
    0x0044DBF0: (173, 'game sound stream source file open', 0x0044C8F0, 'incoming'),
    0x0044DD40: (194, 'game sound stream read cursor update', 0x0044D110, 'incoming'),
    0x0044DE10: (202, 'game sound stream sample copy', 0x0044D110, 'incoming'),
    0x0043CD30: (786, 'game sound device and WAV resource setup', 0x0043CCF0, 'incoming'),
    0x0043D390: (684, 'game WAV format and DirectSound buffer setup', 0x0043CD30, 'incoming'),
    0x0043D250: (71, 'game WAV chunk scan', 0x0043D390, 'incoming'),
    0x0043CB00: (127, 'game sprite task-node cleanup', 0x00449F60, 'outgoing'),
    0x0044C7E0: (260, 'game DirectSound buffer creation', 0x0043CD30, 'incoming'),
    0x00401D20: (60, 'game manager resource attachment', 0x0040E770, 'incoming'),
    0x00404D40: (92, 'six-component game interpolation difference', 0x004049A0, 'incoming'),
    0x00404DA0: (101, 'six-component game interpolation scaling', 0x004049A0, 'incoming'),
    0x00404E10: (92, 'six-component game interpolation sum', 0x004049A0, 'incoming'),
    0x00405040: (94, 'ANM timed command record assignment', 0x00403C80, 'incoming'),
    0x00405300: (60, 'game manager resource attachment', 0x00402850, 'incoming'),
    0x00405410: (57, 'game frame-timer state update', 0x00402850, 'incoming'),
    0x00405B60: (42, 'game effect-coordinate clamp', 0x0041AFD0, 'incoming'),
    0x00408750: (30, 'game polar-to-Cartesian vector update', 0x00406D90, 'incoming'),
    0x00409D90: (46, 'game score increment and cap', 0x0040DC80, 'incoming'),
    0x00409E20: (28, 'game object flag update', 0x00417870, 'incoming'),
    0x0040AC90: (37, 'game state transition value assignment', 0x00418190, 'incoming'),
    0x0040AD20: (33, 'game frame-timer wrap update', 0x004236F0, 'incoming'),
    0x0040B9F0: (111, 'game renderer state update', 0x0040BD20, 'outgoing'),
    0x00412E70: (46, 'game score or gauge decrement and cap', 0x00425730, 'incoming'),
    0x00412ED0: (16, 'game state timer assignment', 0x0040E770, 'incoming'),
    0x00413270: (30, 'game polar-to-Cartesian vector update', 0x0040C9D0, 'incoming'),
    0x00417010: (41, 'game encoded text buffer transform', 0x0040BD80, 'incoming'),
    0x00417800: (58, 'game frame-timer reset', 0x00417870, 'incoming'),
    0x004180E0: (107, 'game render object allocation', 0x004218D0, 'incoming'),
    0x00418A90: (20, 'game stage object index advance and cap', 0x00417C80, 'incoming'),
    0x00419040: (79, 'game stage linked-list cleanup', 0x00418EE0, 'incoming'),
    0x004198C0: (98, 'game resource token lookup', 0x00419960, 'incoming'),
    0x00419F40: (260, 'game hint-file line parsing', 0x00419960, 'incoming'),
    0x0041A050: (75, 'game hint-file key and value split', 0x00419960, 'incoming'),
    0x0041A0A0: (114, 'game hint-file whitespace trim', 0x00419960, 'incoming'),
    0x0041A120: (220, 'game hint-file entry allocation', 0x00424650, 'incoming'),
    0x0041AB10: (52, 'game object linked-list insertion', 0x00419960, 'incoming'),
    0x0041BE80: (46, 'game score increment and cap', 0x0041AFD0, 'incoming'),
    0x0041BEB0: (30, 'game polar-to-Cartesian vector update', 0x0041AFD0, 'incoming'),
    0x0041C7D0: (44, 'game object list flag propagation', 0x0040E770, 'incoming'),
    0x0041C850: (44, 'game object list event propagation', 0x0040E770, 'incoming'),
    0x0041F800: (30, 'game polar-to-Cartesian vector update', 0x0041CFD0, 'incoming'),
    0x004203C0: (48, 'game MIDI timer cleanup', 0x00420270, 'incoming'),
    0x00420C00: (41, 'game dummy sound-cue setup', 0x00417870, 'incoming'),
    0x00420EA0: (118, 'game worker-thread launch under lock', 0x0044C150, 'outgoing'),
    0x004216F0: (465, 'game global scene-state initialization', 0x004201B0, 'incoming'),
    0x00421C50: (25, 'game input critical-section release', 0x004218D0, 'incoming'),
    0x00421D20: (21, 'game DirectInput COM release', 0x0043B8D0, 'incoming'),
    0x00421F60: (28, 'game stage parameter state transfer', 0x004236F0, 'incoming'),
    0x0042A930: (59, 'game frame-timer assignment', 0x00428F60, 'incoming'),
    0x0042ACB0: (274, 'TH10 replay header initialization', 0x0042AE60, 'incoming'),
    0x0042ADD0: (142, 'TH10 replay payload initialization', 0x0042AE60, 'incoming'),
    0x0042CD50: (96, 'game manager and worker allocation', 0x004218D0, 'incoming'),
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
