"""Replay selected authored origins after individual target and IDA behavior review."""

import argparse
import csv
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_GRP_CALL, CS_MODE_32
from capstone.x86 import X86_OP_IMM

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'target-ida-reviewed-game-behavior-2026-09-17'
EARLY_EVIDENCE_ID = 'target-ida-reviewed-early-game-2026-09-17'
MIDDLE_EVIDENCE_ID = 'target-ida-reviewed-middle-game-2026-09-17'
LATE_EVIDENCE_ID = 'target-ida-reviewed-late-game-2026-09-17'
AUDIO_RENDER_EVIDENCE_ID = 'target-ida-reviewed-audio-render-2026-09-17'
# These cues are individually reviewed observations, not proposed source names.
BEHAVIORS = {
    0x00402230: (522, 'game manager state and ANM resource setup'),
    0x00403C80: (1806, 'ANM command dispatch over timed script records'),
    0x00406D90: (1454, 'ANM VM timed script loop and sprite actions'),
    0x00407560: (305, 'ANM VM position interpolation'),
    0x00407780: (338, 'ANM VM interpolation and game sound action'),
    0x004078E0: (335, 'ANM VM interpolation and game sound action'),
    0x00419650: (571, 'stage object position relative to player state'),
    0x00419960: (1498, 'game-owned file and stage resource handling'),
    0x0041A200: (2247, 'game hint-file text generation'),
    0x00421FA0: (316, 'score and stage-indexed state update'),
    0x00422C80: (1338, 'stage controller state dispatch'),
    0x00423570: (377, 'stage transition using game difficulty globals'),
    0x004267F0: (471, 'player-relative option position computation'),
    0x00428B10: (266, 'player option and shot state update'),
    0x0042A6A0: (356, 'game difficulty-indexed option state'),
    0x0042B030: (423, 'TH10 replay header and payload parsing'),
    0x0043D7D0: (369, 'game sound cue name and buffer update'),
    0x0043D950: (298, 'game sound manager state and playback'),
    0x0044A4E0: (264, 'game controller state through DirectInput'),
    0x0044A5F0: (850, 'keyboard state and game input transition update'),
}
EARLY_BEHAVIORS = {
    0x00402640: (151, 'game manager object allocation and initialization'),
    0x00403090: (1984, 'game collision geometry using transformed bounds'),
    0x00403990: (154, 'ANM script record traversal'),
    0x00404450: (214, 'ANM VM flag changes across manager slots'),
    0x00404530: (119, 'game sound and ANM manager state'),
    0x004045B0: (92, 'ANM manager state flag reset'),
    0x004049A0: (688, 'ANM interpolation state update'),
    0x00404ED0: (72, 'ANM interpolation record advance'),
    0x00405500: (81, 'frame-timed game state update'),
    0x004055C0: (84, 'game scheduler registration'),
    0x00405620: (137, 'game loader synchronization cleanup'),
    0x004056B0: (85, 'game loader object allocation'),
    0x00405AC0: (124, 'effect state and game manager access'),
    0x00405BE0: (158, 'ANM script state reset'),
    0x00406060: (182, 'game projectile pool allocation'),
    0x00406160: (109, 'playfield bounds test'),
    0x004061D0: (109, 'playfield bounds test'),
    0x004065C0: (249, 'projectile manager initialization'),
    0x004074B0: (164, 'sprite position and timed state update'),
    0x004076A0: (209, 'ANM interpolation update'),
    0x00408660: (80, 'game angle normalization'),
    0x004086B0: (91, 'game playfield culling'),
    0x004089C0: (211, 'game scheduler registration'),
    0x00408C90: (136, 'ANM manager object allocation'),
    0x0040AC20: (109, 'game object state initialization'),
    0x0040AF00: (137, 'game object synchronization cleanup'),
    0x0040AF90: (86, 'small game state object allocation'),
    0x0040B3A0: (192, 'game renderer manager setup'),
    0x0040B480: (106, 'game resource path and file access'),
    0x0040B940: (99, 'small game state object allocation'),
    0x0040BD20: (93, 'game renderer state check'),
    0x0040CC70: (145, 'ANM VM array state initialization'),
}
MIDDLE_BEHAVIORS = {
    0x00413350: (151, 'game object allocation and initialization'),
    0x00413450: (84, 'game object resource cleanup'),
    0x00414830: (136, 'game manager allocation'),
    0x00415DB0: (124, 'stage object allocation and lookup'),
    0x00417770: (66, 'game object linked-list cleanup'),
    0x00418810: (64, 'stage position and camera globals'),
    0x00418A00: (65, 'stage object state update'),
    0x00418B80: (73, 'stage timer and state update'),
    0x00418C40: (125, 'stage object state initialization'),
    0x00418E30: (131, 'game scheduler registration'),
    0x00418EE0: (346, 'game loader resource and synchronization cleanup'),
    0x00419090: (67, 'stage object allocation'),
    0x0041AD90: (84, 'game scheduler registration'),
    0x0041ADF0: (216, 'game object resource and synchronization cleanup'),
    0x0041AED0: (185, 'game projectile pool allocation'),
    0x0041C1C0: (195, 'game object resource and synchronization cleanup'),
    0x0041C290: (91, 'game object allocation'),
    0x0041C510: (153, 'game object event queue update'),
    0x0041C800: (76, 'game object state transfer'),
    0x0041F7A0: (91, 'playfield bounds test'),
    0x0041FAC0: (140, 'game scheduler registration'),
    0x0041FD00: (69, 'game object allocation'),
}
LATE_BEHAVIORS = {
    0x00420A90: (120, 'game resource filename transformation'),
    0x00420B10: (73, 'game sound and stage state transition'),
    0x004218D0: (572, 'game stage state update'),
    0x00421E00: (225, 'game object cleanup'),
    0x00422150: (142, 'game scheduler registration'),
    0x00422360: (67, 'game controller allocation'),
    0x004223F0: (179, 'game controller state dispatch'),
    0x00423510: (89, 'stage state transition'),
    0x00424650: (104, 'difficulty-indexed game resource access'),
    0x00426610: (67, 'player-relative angle computation'),
    0x00426660: (67, 'player-relative angle computation'),
    0x00427B50: (181, 'player option slot traversal'),
    0x00428160: (105, 'player power to option-count conversion'),
    0x00428D70: (91, 'playfield bounds test'),
    0x00428E10: (83, 'player position conversion'),
    0x0042A8A0: (85, 'game state buffer write'),
    0x0042AA50: (139, 'game controller allocation'),
    0x0042AB20: (93, 'game subsystem node cleanup'),
    0x0042AE60: (132, 'game controller initialization'),
    0x0042B570: (232, 'game object resource cleanup'),
    0x0042B660: (71, 'game object allocation'),
    0x0042B9C0: (167, 'frame-indexed game state update'),
    0x0042C5C0: (83, 'game controller state assignment'),
    0x0042C620: (80, 'game controller state assignment'),
    0x0042C850: (69, 'game controller flag query'),
}
AUDIO_RENDER_BEHAVIORS = {
    0x00437D10: (149, 'game GDI font and bitmap cleanup'),
    0x00437DB0: (554, 'GDI glyph rendering into the game texture'),
    0x00437FE0: (509, 'GDI glyph rendering with alternate color path'),
    0x00438880: (103, 'game window state initialization'),
    0x0043A910: (156, 'game MIDI output object cleanup'),
    0x0043AE20: (129, 'game MIDI output reset'),
    0x0043AEB0: (79, 'game MIDI output header release'),
    0x0043BC40: (135, 'game D3D draw-state update'),
    0x0043C2C0: (68, 'game D3D draw wrapper'),
    0x0043C410: (68, 'game D3D draw wrapper'),
    0x0043C500: (74, 'game D3D draw wrapper'),
    0x0043C8B0: (82, 'game sprite state allocation'),
    0x0043CB80: (88, 'game script random-state update'),
    0x0043CC40: (175, 'game sound worker cleanup'),
    0x0043D120: (301, 'game sound manager cleanup'),
    0x0043D2A0: (227, 'game sound filename lookup'),
    0x0043D690: (241, 'game sound stream thread setup'),
    0x0043DAB0: (176, 'game sound stream thread cleanup'),
    0x0043DC90: (123, 'game sound cue queue update'),
    0x0043DD10: (149, 'game sound cue queue with timing'),
    0x0043E460: (104, 'game sound filename slot assignment'),
}
DIRECT_XREF_SITES = {
    0x00421FA0: (0x00432DB9, 0x00432CB0),
    0x0044A4E0: (0x0042F6B6, 0x0042F540),
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
    seeds = {address for address, row in by_origin.items()
             if row['origin'] == 'authored_game' and row['disposition'] == 'authored'
             and row['evidence_id'] not in (
                 EVIDENCE_ID, EARLY_EVIDENCE_ID, MIDDLE_EVIDENCE_ID,
                 LATE_EVIDENCE_ID, AUDIO_RENDER_EVIDENCE_ID)}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    incoming = {address: set() for address in
                BEHAVIORS | EARLY_BEHAVIORS | MIDDLE_BEHAVIORS |
                LATE_BEHAVIORS | AUDIO_RENDER_BEHAVIORS}
    calls = {}
    for row in functions:
        address = int(row['address'], 0)
        code = pe_bytes_at(image, address, int(row['size']))
        instructions = list(decoder.disasm(code, address))
        if sum(instruction.size for instruction in instructions) != int(row['size']):
            continue
        callees = {instruction.operands[0].imm for instruction in instructions
                   if instruction.group(CS_GRP_CALL) and instruction.operands
                   and instruction.operands[0].type == X86_OP_IMM}
        calls[address] = callees
        if address in seeds:
            for callee in callees & incoming.keys():
                incoming[callee].add(address)
    # These two callers contain switch tables, so their whole ledger spans are
    # not linearly decodable. Verify the actual call instruction at each site.
    for callee, (site, caller) in DIRECT_XREF_SITES.items():
        caller_row = by_function[caller]
        if (caller not in seeds or not caller <= site < caller + int(caller_row['size'])):
            raise ValueError(f'direct xref source is not reviewed authored: {site:#x}')
        instruction = next(decoder.disasm(pe_bytes_at(image, site, 8), site))
        if (not instruction.group(CS_GRP_CALL) or not instruction.operands
                or instruction.operands[0].type != X86_OP_IMM
                or instruction.operands[0].imm != callee):
            raise ValueError(f'direct xref changed: {site:#x}')
        incoming[callee].add(caller)
    for address, (size, behavior) in (
            BEHAVIORS | EARLY_BEHAVIORS | MIDDLE_BEHAVIORS |
            LATE_BEHAVIORS | AUDIO_RENDER_BEHAVIORS).items():
        evidence_id = (EVIDENCE_ID if address in BEHAVIORS else
                       EARLY_EVIDENCE_ID if address in EARLY_BEHAVIORS else
                       MIDDLE_EVIDENCE_ID if address in MIDDLE_BEHAVIORS else
                       LATE_EVIDENCE_ID if address in LATE_BEHAVIORS else
                       AUDIO_RENDER_EVIDENCE_ID)
        row = by_function[address]
        origin = by_origin[address]
        if int(row['size']) != size or (origin['disposition'] != 'review'
                                         and origin['evidence_id'] != evidence_id):
            raise ValueError(f'behavior ledger conflict: {address:#x}')
        if address not in calls:
            raise ValueError(f'behavior body does not completely decode: {address:#x}')
        outgoing = sorted(calls[address] & seeds)
        inbound = sorted(incoming[address])
        if not outgoing and not inbound:
            raise ValueError(f'behavior lacks independent authored call edge: {address:#x}')
        if row['source_file'] or row['owner'] not in ('', 'authored'):
            raise ValueError(f'behavior has competing source/owner: {address:#x}')
        if args.apply:
            origin.update(origin='authored_game', subsystem='GameUnassigned',
                          disposition='authored', confidence='medium',
                          evidence_id=evidence_id)
            row['owner'] = 'authored'
            if evidence_id not in row['notes']:
                edge = (f'directly calls reviewed authored 0x{outgoing[0]:08X}'
                        if outgoing else
                        f'is directly called by reviewed authored 0x{inbound[0]:08X}')
                note = (f'Complete target decode; {edge}. IDA behavior review '
                        f'shows {behavior} ({evidence_id}). Original owner and '
                        f'source unit remain unknown.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    if args.apply:
        write_rows('function-origins.csv', origins)
        write_rows('functions.csv', functions)
    print(f'{len(BEHAVIORS)} individually reviewed game-behavior origins; '
          f'{sum(size for size, _ in BEHAVIORS.values())} target bytes')
    print(f'{len(EARLY_BEHAVIORS)} individually reviewed early-game origins; '
          f'{sum(size for size, _ in EARLY_BEHAVIORS.values())} target bytes')
    print(f'{len(MIDDLE_BEHAVIORS)} individually reviewed middle-game origins; '
          f'{sum(size for size, _ in MIDDLE_BEHAVIORS.values())} target bytes')
    print(f'{len(LATE_BEHAVIORS)} individually reviewed late-game origins; '
          f'{sum(size for size, _ in LATE_BEHAVIORS.values())} target bytes')
    print(f'{len(AUDIO_RENDER_BEHAVIORS)} individually reviewed audio/render origins; '
          f'{sum(size for size, _ in AUDIO_RENDER_BEHAVIORS.values())} target bytes')


if __name__ == '__main__':
    main()
