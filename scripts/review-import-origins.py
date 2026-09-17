"""Classify target-owned FF 25 jump thunks by the pinned PE import table."""

import argparse
import csv
from pathlib import Path
import struct

from target_identity import parse_pe, pe_bytes_at, resolve_target, verify_target

ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'pe32-iat-jump-thunk-2026-09-17'


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


def c_string(image, address):
    data = pe_bytes_at(image, address, 256)
    stop = data.find(b'\0')
    if stop < 0:
        raise ValueError(f'unterminated PE import string at {address:#x}')
    return data[:stop].decode('ascii')


def import_slots(image):
    pe = parse_pe(image)
    pe_offset = struct.unpack_from('<I', image, 0x3C)[0]
    optional = pe_offset + 24
    import_rva, import_size = struct.unpack_from('<II', image, optional + 96 + 8)
    if not import_rva or not import_size:
        raise ValueError('target has no import directory')
    base = pe['image_base']
    slots = {}
    for descriptor_index in range(256):
        address = base + import_rva + descriptor_index * 20
        original, _, _, name_rva, first_rva = struct.unpack(
            '<IIIII', pe_bytes_at(image, address, 20))
        if not any((original, name_rva, first_rva)):
            break
        if not name_rva or not first_rva:
            raise ValueError(f'invalid import descriptor at {address:#x}')
        module = c_string(image, base + name_rva)
        lookup_rva = original or first_rva
        for index in range(1024):
            entry = struct.unpack('<I', pe_bytes_at(image, base + lookup_rva + index * 4, 4))[0]
            if not entry:
                break
            if entry & 0x80000000:
                symbol = f'Ordinal_{entry & 0xffff}'
            else:
                symbol = c_string(image, base + entry + 2)
            slot = base + first_rva + index * 4
            if slot in slots:
                raise ValueError(f'duplicate IAT slot {slot:#x}')
            slots[slot] = (module, symbol)
        else:
            raise ValueError(f'import descriptor has too many symbols: {module}')
    else:
        raise ValueError('too many import descriptors')
    return slots


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply', action='store_true')
    args = parser.parse_args()
    target_path = resolve_target()
    _, problems = verify_target(target_path)
    if problems:
        raise ValueError(problems)
    image = target_path.read_bytes()
    slots = import_slots(image)
    functions = read_rows('functions.csv')
    origins = read_rows('function-origins.csv')
    by_origin = {row['address']: row for row in origins}
    matches = []
    for row in functions:
        origin = by_origin[row['address']]
        if origin['disposition'] != 'review' and origin['evidence_id'] != EVIDENCE_ID:
            continue
        if int(row['size']) != 6:
            continue
        code = pe_bytes_at(image, int(row['address'], 0), 6)
        if code[:2] != b'\xff\x25':
            continue
        slot = struct.unpack_from('<I', code, 2)[0]
        if slot not in slots:
            continue
        module, symbol = slots[slot]
        expected = f'{module}::{symbol}'
        if row['current_name'].lower() != expected.lower():
            raise ValueError(f'import thunk name disagrees with target IAT: {row["address"]}')
        matches.append((row['address'], slot, module, symbol))
        if args.apply:
            if row['owner'] not in ('', 'import_thunk'):
                raise ValueError(f'refusing to replace function owner: {row["address"]}')
            origin.update(origin='import_thunk', subsystem='PEImport',
                          disposition='exclude', confidence='high', evidence_id=EVIDENCE_ID)
            row.update(module='PEImport', status='excluded', owner='import_thunk')
            if EVIDENCE_ID not in row['notes']:
                note = (f'Target FF 25 jump reaches PE IAT slot 0x{slot:08X} for '
                        f'{module}::{symbol}; import thunk, no authored body ({EVIDENCE_ID}).')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    if args.apply:
        write_rows('function-origins.csv', origins)
        write_rows('functions.csv', functions)
    print(f'{len(matches)} hash-attested PE import thunks reviewed')


if __name__ == '__main__':
    main()
