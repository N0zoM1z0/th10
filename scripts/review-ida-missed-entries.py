"""Add IDA-missed ledger entries after target pointer and extent replay."""

import argparse
import csv
import io
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'target-ida-missed-pointer-boundary-2026-09-17'
# Address, complete IDA extent, target instruction carrying the entry pointer.
ENTRIES = (
    (0x004201B0, 186, 0x004204B7),
    (0x0043BA90, 83, 0x00420222),
    (0x0043BB30, 98, 0x0043BA66),
    (0x0043CCF0, 57, 0x00438C08),
)


def read_lines(name):
    path = ROOT / 'config' / name
    lines = path.read_text(encoding='utf-8').splitlines(keepends=True)
    rows = list(csv.DictReader(lines))
    if len(lines) != len(rows) + 1:
        raise ValueError(f'CSV physical rows changed unexpectedly: {path}')
    return path, lines, rows


def serialized_row(row, columns):
    stream = io.StringIO()
    csv.DictWriter(stream, fieldnames=columns, lineterminator='\n').writerow(row)
    return stream.getvalue()


def add_rows(name, new_rows, apply):
    path, lines, old_rows = read_lines(name)
    columns = lines[0].rstrip('\n').split(',')
    existing = {int(row['address'], 0) for row in old_rows}
    if len(existing) != len(old_rows):
        raise ValueError(f'duplicate ledger address in {name}')
    for row in new_rows:
        address = int(row['address'], 0)
        if address in existing:
            old = next(item for item in old_rows if int(item['address'], 0) == address)
            stable_fields = ({'address', 'size', 'span_end', 'current_name', 'evidence'}
                             if name == 'functions.csv' else
                             {'address', 'span_end', 'state', 'confidence',
                              'evidence_id', 'notes'}
                             if name == 'function-boundaries.csv' else
                             {'address'})
            if any(old[field] != row[field] for field in stable_fields):
                raise ValueError(f'ledger entry changed at {address:#x} in {name}')
    if not apply:
        return
    added = {int(row['address'], 0): row for row in new_rows if int(row['address'], 0) not in existing}
    result = [lines[0]]
    old_items = ((int(row['address'], 0), raw) for row, raw in zip(old_rows, lines[1:]))
    all_items = list(old_items) + [(address, serialized_row(row, columns))
                                   for address, row in added.items()]
    for _, raw in sorted(all_items, key=lambda item: item[0]):
        result.append(raw)
    path.write_text(''.join(result), encoding='utf-8')


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
    _, _, functions = read_lines('functions.csv')
    old_extents = [(int(row['address'], 0), int(row['span_end'], 0))
                   for row in functions]
    function_rows, boundary_rows, origin_rows = [], [], []
    for address, size, pointer_site in ENTRIES:
        end = address + size - 1
        if any(start <= end and address <= finish and start != address
               for start, finish in old_extents):
            raise ValueError(f'new entry overlaps tracked extent: {address:#x}')
        instructions = list(decoder.disasm(pe_bytes_at(image, address, size), address))
        if (sum(item.size for item in instructions) != size
                or instructions[-1].mnemonic != 'ret'):
            raise ValueError(f'new entry extent changed: {address:#x}')
        pointer = next(decoder.disasm(pe_bytes_at(image, pointer_site, 8), pointer_site))
        if address not in (operand.imm for operand in pointer.operands
                           if operand.type == X86_OP_IMM):
            raise ValueError(f'entry pointer changed at {pointer_site:#x}')
        label = f'0x{address:08X}'
        span_end = f'0x{end:08X}'
        note = (f'Complete target dense decode ends in RET; independent '
                f'{pointer.mnemonic} pointer at 0x{pointer_site:08X}; '
                f'hash-attested IDA contiguous extent agrees; no tracked overlap.')
        function_rows.append(dict(
            address=label, size=str(size), span_end=span_end,
            current_name=f'raw_{address:08X}', proposed_name='', module='',
            status='unclassified', match_percent='0.00', calling_convention='',
            signature='', is_thunk='false', source_file='',
            evidence=EVIDENCE_ID, owner='',
            notes=f'{note} Authored versus compiler/library origin remains under review.'))
        boundary_rows.append(dict(address=label, span_end=span_end,
                                  state='reviewed', confidence='high',
                                  evidence_id=EVIDENCE_ID, notes=note))
        origin_rows.append(dict(address=label, origin='unknown', subsystem='',
                                disposition='review', confidence='unknown',
                                evidence_id=EVIDENCE_ID))
    add_rows('functions.csv', function_rows, args.apply)
    add_rows('function-boundaries.csv', boundary_rows, args.apply)
    add_rows('function-origins.csv', origin_rows, args.apply)
    print(f'{len(ENTRIES)} target-pointer-backed IDA-missed boundaries; '
          f'{sum(size for _, size, _ in ENTRIES)} target bytes')


if __name__ == '__main__':
    main()
