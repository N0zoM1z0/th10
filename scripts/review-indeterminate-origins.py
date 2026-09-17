"""Record individually inspected TH10 entries whose source origin remains unknown.

These groups follow target extent review plus IDA body/xref inspection. The
labels describe the unresolved attribution question; none claims authorship.
"""

import argparse
import csv
from pathlib import Path

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'target-ida-origin-indeterminate-2026-09-18'

# C++ object setup/teardown is observed, but source-defined lifecycle bodies,
# generated member initialization, and linker outlined bodies cannot yet be
# separated at these individual extents.
LIFECYCLE = {
    0x402160, 0x405D00, 0x405DE0, 0x408910, 0x40B9D0, 0x40CF40,
    0x413810, 0x4175E0, 0x418150, 0x418D00, 0x418D40, 0x41AD60,
    0x41C030, 0x41C5B0, 0x41C680, 0x41F850, 0x421F00, 0x4220E0,
    0x4247C0, 0x4247D0, 0x4247E0, 0x425090, 0x42B430, 0x42B6F0,
    0x42C920, 0x42CDB0, 0x435690, 0x4356C0, 0x436640, 0x436680,
    0x43C870, 0x449A90, 0x44D080, 0x44D730,
}

# IDA shows a call adapter, callback, short forwarder, or private outline.
# The actual original source declaration versus generated adapter is unknown.
ADAPTER = {
    0x40678E, 0x409270, 0x40CF60, 0x40CF90, 0x412A00, 0x412A10,
    0x412A60, 0x412A70, 0x412A80, 0x412AA0, 0x412DB0, 0x4198A0,
    0x422A90, 0x426500, 0x426510, 0x427950, 0x427960, 0x427AD0,
    0x427AE0, 0x42A3D0, 0x42A3E0, 0x42A430, 0x42C730, 0x42C750,
    0x42D2E0, 0x42D2F0,
}

# IDA shows executable game, graphics, or system behavior, but no independent
# provenance strong enough to decide source-written game code versus a linked
# helper/optimizer contribution. Mere game semantics is not source ownership.
BEHAVIOR = {
    0x4050D0, 0x4088C0, 0x40B250, 0x412720, 0x41C330,
    0x41D2C0, 0x41E4D0, 0x420DD0, 0x421B90, 0x427C50, 0x427C70,
    0x428AD0, 0x428C20, 0x428DD0, 0x42C8C0,
    0x436570, 0x437750, 0x44BA80,
}

# The physical extent is reviewed, but IDA does not provide a usable owner
# reference or independent decompilation for these entries.
UNANCHORED = {
    0x424EB0, 0x424EC0, 0x4285F0, 0x429A30, 0x429B30,
    0x4356D0, 0x4452E0, 0x4458A0, 0x44BA30, 0x44BD10,
    0x427D80, 0x44C130,
}

GROUPS = {
    'lifecycle': LIFECYCLE,
    'adapter': ADAPTER,
    'behavior': BEHAVIOR,
    'unanchored': UNANCHORED,
}
REASONS = {
    'lifecycle': 'IDA shows C++ object initialization or teardown; source-authored body versus compiler-generated lifecycle contribution remains unresolved',
    'adapter': 'IDA shows a short adapter, callback, forwarder, or outlined helper; original source declaration versus compiler-generated boundary remains unresolved',
    'behavior': 'IDA shows executable behavior, but target references do not establish whether this extent came from game source, a linked helper, or optimizer outlining',
    'unanchored': 'the physical target extent is reviewed, but IDA lacks an independent entry anchor or usable function body for source attribution',
}

# One target/IDA observation per candidate. These are deliberately short:
# ownership conclusions above remain open even when behavior is clear.
OBSERVED = {
    0x402160: 'vector constructor iterators and sprite-array reset',
    0x4050D0: 'private 64-bit arithmetic helper called from ECL/ANM code',
    0x405D00: 'large object field initialization',
    0x405DE0: 'conditional release of an object buffer',
    0x40678E: 'interior entry within IDA function 0x406770',
    0x4088C0: 'loop over game object state',
    0x408910: 'vector construction followed by global object publication',
    0x409270: 'single call forwarder to 0x4091C0',
    0x40B250: 'object data copy helper without reviewed source owner',
    0x40B9D0: 'conditional object free sequence',
    0x40CF40: 'fixed-size state zeroing and field initialization',
    0x40CF60: 'private ECL state accessor',
    0x40CF90: 'private ECL state accessor called repeatedly by dispatcher',
    0x412720: 'register-bound state helper called by ECL dispatcher',
    0x412A00: 'call forwarder to 0x44FDB0',
    0x412A10: 'ECL argument lookup with virtual fallback',
    0x412A60: 'call forwarder to 0x44FE40',
    0x412A70: 'call forwarder to 0x450070',
    0x412A80: 'small ECL helper whose IDA decompilation fails',
    0x412AA0: 'call forwarder to 0x44FF80',
    0x412DB0: 'private register-bound helper called by ECL dispatcher',
    0x413810: 'multiple vector constructor iterators over sprite records',
    0x4175E0: 'object setup called by reviewed game routine',
    0x418150: 'conditional object free sequence',
    0x418D00: 'zeroes and initializes 0x88-byte object region',
    0x418D40: 'zeroes and initializes 0x1A8-byte object region',
    0x4198A0: 'call forwarder to 0x419650',
    0x41AD60: 'conditional object buffer release',
    0x41C030: 'base object initialization called by derived initializers',
    0x41C330: 'object update routine without independently identified owner',
    0x41C5B0: 'derived object setup installs vtable 0x46DA60',
    0x41C680: 'derived object setup installs vtable 0x46DA10',
    0x41D2C0: 'virtual motion integration uses frame scale and atan2',
    0x41E4D0: 'virtual rotated collision test uses sine and cosine',
    0x41F850: 'object initialization installs vtable 0x4703E4',
    0x420DD0: 'game debug/version string parser with no IDA entry xref',
    0x421B90: 'three-vector floating-point calculation with no IDA entry xref',
    0x421F00: 'object field initialization and nested constructor call',
    0x4220E0: 'object flags and fields initialized before game use',
    0x422A90: 'call forwarder to 0x4223F0',
    0x4247C0: 'seven-byte Player-related field initialization',
    0x4247D0: 'ten-byte Player-related field initialization',
    0x4247E0: 'seven-byte Player-related field initialization',
    0x424EB0: 'three-byte zero-return stub without IDA entry xref',
    0x424EC0: 'three-byte zero-return stub without IDA entry xref',
    0x425090: 'conditional Player-region free sequence',
    0x426500: 'Player callback forwarder to 0x425730',
    0x426510: 'Player callback forwarder to 0x426360',
    0x427950: 'Player wrapper calls 0x427960 then returns zero',
    0x427960: 'Player callback body reached through wrapper 0x427950',
    0x427AD0: 'Player wrapper calls 0x427AE0 then returns zero',
    0x427AE0: 'Player callback body reached through wrapper 0x427AD0',
    0x427C50: 'floor wrapper called from Player update',
    0x427C70: 'object comparison called from two game routines',
    0x427D80: 'IDA decompilation fails and no entry xref is reported',
    0x4285F0: 'CC-delimited entry absent from IDA function inventory',
    0x428AD0: 'data-referenced Player-region helper',
    0x428C20: 'data-referenced Player-region helper',
    0x428DD0: 'floating-point helper called from ANM executor',
    0x429A30: 'CC-delimited entry absent from IDA function inventory',
    0x429B30: 'CC-delimited entry absent from IDA function inventory',
    0x42A3D0: 'forwarder to 0x4297D0 held as a function pointer',
    0x42A3E0: 'state adapter held as a function pointer',
    0x42A430: 'input flag adapter held as a function pointer',
    0x42B430: 'object setup called from reviewed game routine',
    0x42B6F0: 'target constructor-shaped extent without IDA decompilation',
    0x42C730: 'short adapter calls 0x449250',
    0x42C750: 'short adapter calls 0x449250 from multiple callers',
    0x42C8C0: 'game object state helper called from input update',
    0x42C920: 'object initializer installs vtable 0x46ECF0',
    0x42CDB0: 'conditional object free sequence',
    0x42D2E0: 'forwarder to 0x42CDF0 held as a function pointer',
    0x42D2F0: 'short object state adapter held as a function pointer',
    0x435690: 'seven-byte vtable restoration body',
    0x4356C0: 'seven-byte vtable initialization body',
    0x4356D0: 'short lifecycle-shaped extent with no usable IDA decompilation',
    0x436570: 'CreateProcessA helper with no IDA entry xref',
    0x436640: 'initializes fields in a small process-related object',
    0x436680: 'tears down a process-related object',
    0x437750: 'GDI bitmap drawing/transform body with no IDA entry xref',
    0x43C870: 'conditional free wrapper held as a function pointer',
    0x4452E0: 'ten-byte state accessor with no IDA entry xref',
    0x4458A0: 'seven-byte state accessor with no IDA entry xref',
    0x449A90: 'zeroes two object fields',
    0x44BA30: 'CC-delimited entry absent from IDA function inventory',
    0x44BA80: 'state helper with no IDA entry xref',
    0x44BD10: 'CC-delimited entry absent from IDA function inventory',
    0x44C130: 'IDA folds this address into unrelated 0x40BA90 decompilation',
    0x44D080: 'object destruction body called by deleting destructor',
    0x44D730: 'sets vtable then calls destruction body 0x44D080',
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
    functions = rows('functions.csv')
    origins = rows('function-origins.csv')
    boundaries = rows('function-boundaries.csv')
    revised_functions = [row.copy() for row in functions]
    revised_origins = [row.copy() for row in origins]
    by_function = {int(row['address'], 0): row for row in revised_functions}
    by_origin = {int(row['address'], 0): row for row in revised_origins}
    by_boundary = {int(row['address'], 0): row for row in boundaries}
    if sum(map(len, GROUPS.values())) != len(set().union(*GROUPS.values())):
        raise ValueError('indeterminate review groups overlap')
    reviewed = set().union(*GROUPS.values())
    if reviewed != set(OBSERVED):
        raise ValueError(f'individual observations differ: missing={sorted(reviewed-set(OBSERVED))}, extra={sorted(set(OBSERVED)-reviewed)}')
    pending = {address for address, row in by_origin.items()
               if row['disposition'] == 'review'}
    already = {address for address, row in by_origin.items()
               if row['disposition'] == 'indeterminate'
               and row['evidence_id'] == EVIDENCE_ID}
    if reviewed != pending | already:
        raise ValueError(f'indeterminate manifest differs: missing={sorted(pending-reviewed)}, extra={sorted(reviewed-(pending|already))}')
    for group, addresses in GROUPS.items():
        for address in sorted(addresses):
            row = by_function[address]
            origin = by_origin[address]
            boundary = by_boundary[address]
            size = int(row['size'])
            if (boundary['state'] != 'reviewed' or origin['origin'] != 'unknown'
                    or origin['disposition'] not in ('review', 'indeterminate')
                    or row['status'] == 'exact' or row['owner'] not in ('', 'unknown')
                    or len(pe_bytes_at(image, address, size)) != size):
                raise ValueError(f'indeterminate origin ledger conflict: {address:#x}')
            if args.apply:
                origin.update(disposition='indeterminate', confidence='unknown',
                              evidence_id=EVIDENCE_ID)
                if EVIDENCE_ID not in row['notes']:
                    note = (f'origin indeterminate: {OBSERVED[address]}; '
                            f'{REASONS[group]} '
                            f'({EVIDENCE_ID}; reviewed target 0x{address:08X}, '
                            f'{size} bytes).')
                    row['notes'] = (row['notes'] + ' ' + note).strip()
    if args.apply:
        write_changed('functions.csv', functions, revised_functions)
        write_changed('function-origins.csv', origins, revised_origins)
    print(f'{len(reviewed)} individually grouped origin-indeterminate entries')


if __name__ == '__main__':
    main()
