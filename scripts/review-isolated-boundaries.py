"""Review isolated TH10 code extents lacking an observed entry reference.

The eleven IDA extents below were observed from the hash-matched IDA MCP
database. Four additional extents are bounded by target code/padding islands.
All fifteen are independently re-decoded from the pinned target before write.
They are physical-boundary observations; origin and source remain separate.
"""

import argparse
import csv
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32

from target_identity import pe_bytes_at, resolve_target, verify_target

ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'ida-isolated-target-boundary-2026-09-17'
IDA_EXTENTS = {
    '0x0040B110': 148,
    '0x00420DD0': 205,
    '0x00421B90': 77,
    '0x00436570': 201,
    '0x00437750': 645,
    '0x0043BFA0': 501,
    '0x0044BA80': 155,
    '0x0044CE80': 118,
    '0x00454CF6': 27,
    '0x0046077D': 27,
    '0x00462D9E': 31,
}
STRUCTURAL_EXTENTS = {
    '0x00427D80': 21,
    '0x004285F0': 55,
    '0x0044BD10': 260,
    '0x0045D3D6': 12,
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
    boundaries = read_rows('function-boundaries.csv')
    by_address = {row['address']: row for row in functions}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    for address, expected_size in {**IDA_EXTENTS, **STRUCTURAL_EXTENTS}.items():
        function = by_address[address]
        start, size = int(address, 0), int(function['size'])
        if size != expected_size or int(function['span_end'], 0) != start + size - 1:
            raise ValueError(f'extent changed: {address}')
        if any(other['address'] != address and
               int(other['address'], 0) <= start + size - 1 and
               int(other['span_end'], 0) >= start for other in functions):
            raise ValueError(f'isolated extent now overlaps: {address}')
        instructions = list(decoder.disasm(pe_bytes_at(image, start, size), start))
        if sum(instruction.size for instruction in instructions) != size or (
                instructions[-1].mnemonic not in ('ret', 'jmp')):
            raise ValueError(f'incomplete target code extent: {address}')
    # Check the four target-local separators that supplement IDA's omission.
    if pe_bytes_at(image, 0x427D7F, 1) != b'\xCC' or pe_bytes_at(image, 0x427D95, 1) != b'\xCC':
        raise ValueError('0x427D80 is no longer CC-delimited')
    if pe_bytes_at(image, 0x4285EC, 4) != b'\xCC' * 4 or pe_bytes_at(image, 0x428627, 9) != b'\xCC' * 9:
        raise ValueError('0x4285F0 is no longer CC-delimited')
    if pe_bytes_at(image, 0x44BD0F, 1) != b'\xC3' or pe_bytes_at(image, 0x44BE14, 12) != b'\xCC' * 12:
        raise ValueError('0x44BD10 has lost its preceding RET or following padding')
    if pe_bytes_at(image, 0x45D3D5, 1) != b'\xC3' or pe_bytes_at(image, 0x45D3E1, 1) != b'\xC3':
        raise ValueError('0x45D3D6 no longer fills the neighboring RET-delimited gap')
    promoted = 0
    for row in boundaries:
        address = row['address']
        if address not in IDA_EXTENTS and address not in STRUCTURAL_EXTENTS:
            continue
        if row['state'] == 'reviewed':
            continue
        if row['state'] != 'provisional' or not row['notes'].startswith('Complete dense decode'):
            raise ValueError(f'prior provisional evidence changed: {address}')
        detail = ('independent hash-matched IDA MCP function extent agrees' if address in IDA_EXTENTS
                  else 'target-local RET/CC separator and neighboring code establish the contiguous extent')
        row.update(state='reviewed', confidence='medium',
                   evidence_id=row['evidence_id'] + '+' + EVIDENCE_ID,
                   notes=row['notes'] + '; ' + detail + '; no independent entry reference observed')
        promoted += 1
    if args.apply:
        write_rows('function-boundaries.csv', boundaries)
    print(f'15 isolated target extents verified; {promoted} boundaries promoted' if args.apply
          else f'15 isolated target extents verified; {promoted} boundary reviews pending write')


if __name__ == '__main__':
    main()
