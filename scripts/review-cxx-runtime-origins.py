"""Review VC7.1 standard C++ exception bodies and EH member-call adapters."""

import argparse
import csv
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_GRP_CALL, CS_MODE_32
from capstone.x86 import X86_OP_IMM

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
CXX_EVIDENCE_ID = 'target-vc71-standard-exception-family-2026-09-17'
ADAPTER_EVIDENCE_ID = 'target-vc71-eh-member-adapter-2026-09-17'
CXX_BODIES = {
    0x00462301: (38, 'std::string constructor'),
    0x00462327: (60, 'std::logic_error constructor'),
    0x00462371: (29, 'std::logic_error destructor'),
    0x0046238E: (28, 'std::logic_error deleting destructor'),
    0x004623C2: (28, 'std::length_error deleting destructor'),
    0x004623DE: (11, 'std::length_error vtable destructor'),
    0x00462401: (28, 'std::out_of_range deleting destructor'),
    0x0046241D: (11, 'std::out_of_range vtable destructor'),
    0x00462428: (63, 'std::string invalid-position throw helper'),
    0x004624C3: (34, 'std::string constructor'),
    0x004624E5: (63, 'std::string excessive-length throw helper'),
    0x00462B7B: (28, 'type_info deleting destructor'),
    0x00462D00: (11, 'bad_cast vtable destructor'),
    0x00462D3C: (11, 'bad_typeid vtable destructor'),
    0x00462D77: (11, 'bad_typeid vtable destructor'),
    0x00462D82: (28, 'base exception deleting destructor'),
    0x00462DBD: (28, 'bad_cast deleting destructor'),
    0x00462DD9: (28, 'bad_typeid deleting destructor'),
    0x00462DF5: (28, 'bad_typeid deleting destructor'),
}
# Verified direct call sites inside already reviewed VC7.1 EH library bodies.
MEMBER_ADAPTER_SITES = {
    0x00462706: (0x00462FA1, 0x00462F7C),
    0x0046270D: (0x004634F3, 0x00463399),
    0x00462714: (0x004634E2, 0x00463399),
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
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    for address, (size, behavior) in CXX_BODIES.items():
        row = by_function[address]
        origin = by_origin[address]
        if int(row['size']) != size or (origin['disposition'] != 'review'
                                         and origin['evidence_id'] != CXX_EVIDENCE_ID):
            raise ValueError(f'C++ runtime ledger conflict: {address:#x}')
        instructions = list(decoder.disasm(pe_bytes_at(image, address, size), address))
        if sum(instruction.size for instruction in instructions) != size:
            raise ValueError(f'C++ runtime body does not completely decode: {address:#x}')
        if args.apply:
            if row['owner'] not in ('', 'library'):
                raise ValueError(f'refusing to replace C++ runtime owner: {address:#x}')
            origin.update(origin='library', subsystem='CxxRuntime',
                          disposition='exclude', confidence='medium',
                          evidence_id=CXX_EVIDENCE_ID)
            row.update(module='CxxRuntime', status='excluded', owner='library')
            if CXX_EVIDENCE_ID not in row['notes']:
                note = (f'Complete target code; IDA and target vtable/call review '
                        f'identifies a {behavior} in the VC7.1 standard C++ '
                        f'exception family ({CXX_EVIDENCE_ID}). Exact CRT member '
                        f'and source unit remain unknown.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    for address, (site, caller) in MEMBER_ADAPTER_SITES.items():
        row = by_function[address]
        origin = by_origin[address]
        caller_row = by_function[caller]
        if (int(row['size']) != 7 or by_origin[caller]['origin'] != 'library'
                or not caller <= site < caller + int(caller_row['size'])
                or (origin['disposition'] != 'review'
                    and origin['evidence_id'] != ADAPTER_EVIDENCE_ID)):
            raise ValueError(f'EH adapter ledger conflict: {address:#x}')
        instructions = list(decoder.disasm(pe_bytes_at(image, address, 7), address))
        if [(item.mnemonic, item.op_str) for item in instructions] != [
                ('pop', 'eax'), ('pop', 'ecx'),
                ('xchg', 'dword ptr [esp], eax'), ('jmp', 'eax')]:
            raise ValueError(f'EH adapter instructions changed: {address:#x}')
        call = next(decoder.disasm(pe_bytes_at(image, site, 8), site))
        if (not call.group(CS_GRP_CALL) or not call.operands
                or call.operands[0].type != X86_OP_IMM
                or call.operands[0].imm != address):
            raise ValueError(f'EH adapter caller changed: {site:#x}')
        if args.apply:
            if row['owner'] not in ('', 'library'):
                raise ValueError(f'refusing to replace EH adapter owner: {address:#x}')
            origin.update(origin='library', subsystem='CxxEH',
                          disposition='exclude', confidence='medium',
                          evidence_id=ADAPTER_EVIDENCE_ID)
            row.update(module='CxxEH', status='excluded', owner='library')
            if ADAPTER_EVIDENCE_ID not in row['notes']:
                note = (f'Seven-byte member-call adapter directly called by '
                        f'reviewed CRT EH body 0x{caller:08X} at 0x{site:08X} '
                        f'({ADAPTER_EVIDENCE_ID}). Exact object provenance remains unknown.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    if args.apply:
        write_rows('function-origins.csv', origins)
        write_rows('functions.csv', functions)
    print(f'{len(CXX_BODIES)} C++ standard-library bodies and '
          f'{len(MEMBER_ADAPTER_SITES)} EH member-call adapters reviewed')


if __name__ == '__main__':
    main()
