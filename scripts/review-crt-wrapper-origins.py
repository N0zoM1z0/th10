"""Review individually identified thin VC7.1 runtime wrappers."""

import argparse
import csv
import importlib.util
from pathlib import Path
import struct

from capstone import Cs, CS_ARCH_X86, CS_GRP_CALL, CS_GRP_JUMP, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
EVIDENCE_ID = 'target-vc71-runtime-wrapper-2026-09-17'
SPECIAL_EVIDENCE_ID = 'target-vc71-runtime-special-2026-09-17'
CALLBACK_EVIDENCE_ID = 'target-vc71-runtime-callback-helper-2026-09-17'
# Each target-local transfer was individually checked against its CRT behavior.
WRAPPERS = {
    0x00452493: (14, 0x004526DA, 'operator new forwards to runtime allocation'),
    0x004524A1: (5, 0x00452422, 'free tail-forwards to runtime free'),
    0x00452876: (5, 0x004527EE, 'atol tail-forwards to runtime conversion'),
    0x00456AF2: (9, 0x00456357, 'errno accesses the runtime thread record'),
    0x00456AFB: (9, 0x00456357, 'doserrno accesses the runtime thread record'),
    0x00457577: (9, 0x00453C0C, 'lockexit locks CRT slot eight'),
    0x00457580: (9, 0x00453B3F, 'unlockexit unlocks CRT slot eight'),
    0x0045770E: (15, 0x00457629, 'cexit invokes runtime exit cleanup'),
    0x00458EA5: (14, 0x00458E74, 'security cookie check invokes CRT failure path'),
    0x0045FDAF: (9, 0x0045373D, 'floating-point trap invokes CRT fatal exit'),
    0x00463BBA: (10, 0x00463CC0, 'CIfmod enters the CRT math dispatcher'),
}
SPECIALS = {
    0x00452FF0: (289, 'floor SSE path and CRT fallback'),
    0x00456A8C: (56, 'CRT floating-conversion function pointer initialization'),
    0x00459969: (9, 'NLG notification prefix into reviewed CRT body'),
    0x0045BD80: (7, 'strcpy prefix into reviewed CRT body'),
    0x0045ED70: (11, 'x87 status-word runtime helper'),
}
CALLBACK_HELPERS = {
    0x0045631E: (9, 'CRT TLS allocation wrapper'),
    0x00456A8B: (1, 'default no-op thread callback'),
    0x0045E3A0: (3, 'default zero-result math callback'),
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
    def library_parent(site):
        return any(start <= site < start + int(row['size'])
                   and by_origin[start]['origin'] == 'library'
                   for start, row in by_function.items())
    for address, (size, callee, behavior) in WRAPPERS.items():
        row = by_function[address]
        origin = by_origin[address]
        if int(row['size']) != size or (origin['disposition'] not in ('review', 'exclude')
                                         or origin['disposition'] == 'exclude'
                                         and origin['evidence_id'] != EVIDENCE_ID):
            raise ValueError(f'wrapper ledger conflict: {address:#x}')
        callee_origin = by_origin.get(callee)
        if not callee_origin or callee_origin['origin'] != 'library':
            raise ValueError(f'wrapper callee is not reviewed CRT library: {callee:#x}')
        instructions = list(decoder.disasm(pe_bytes_at(image, address, size), address))
        if sum(instruction.size for instruction in instructions) != size:
            raise ValueError(f'wrapper does not completely decode: {address:#x}')
        transfers = [instruction.operands[0].imm for instruction in instructions
                     if (instruction.group(CS_GRP_CALL) or instruction.group(CS_GRP_JUMP))
                     and instruction.operands
                     and instruction.operands[0].type == X86_OP_IMM
                     and not address <= instruction.operands[0].imm < address + size]
        if transfers != [callee]:
            raise ValueError(f'wrapper transfer differs: {address:#x} {transfers}')
        if args.apply:
            if row['owner'] not in ('', 'library'):
                raise ValueError(f'refusing to replace wrapper owner: {address:#x}')
            origin.update(origin='library', subsystem='CRT', disposition='exclude',
                          confidence='medium', evidence_id=EVIDENCE_ID)
            row.update(module='CRT', status='excluded', owner='library')
            if EVIDENCE_ID not in row['notes']:
                note = (f'Complete thin runtime wrapper directly transfers to '
                        f'reviewed CRT 0x{callee:08X}; {behavior} '
                        f'({EVIDENCE_ID}). No exact source identity claimed.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    for address, (size, behavior) in SPECIALS.items():
        row = by_function[address]
        origin = by_origin[address]
        if int(row['size']) != size or (origin['disposition'] not in ('review', 'exclude')
                                         or origin['disposition'] == 'exclude'
                                         and origin['evidence_id'] != SPECIAL_EVIDENCE_ID):
            raise ValueError(f'runtime special ledger conflict: {address:#x}')
        instructions = list(decoder.disasm(pe_bytes_at(image, address, size), address))
        if sum(instruction.size for instruction in instructions) != size:
            raise ValueError(f'runtime special does not completely decode: {address:#x}')
        if address in (0x00459969, 0x0045BD80):
            final = instructions[-1]
            if final.mnemonic != 'jmp' or final.operands[0].type != X86_OP_IMM:
                raise ValueError(f'runtime special lacks final jump: {address:#x}')
            destination = final.operands[0].imm
            if not any(start <= destination < start + int(other['size'])
                       and by_origin[start]['origin'] == 'library'
                       for start, other in by_function.items()):
                raise ValueError(f'runtime special does not enter CRT body: {address:#x}')
        elif address == 0x00452FF0:
            crt_transfers = {instruction.operands[0].imm for instruction in instructions
                         if (instruction.group(CS_GRP_CALL) or instruction.group(CS_GRP_JUMP))
                         and instruction.operands
                         and instruction.operands[0].type == X86_OP_IMM
                         and instruction.operands[0].imm in by_origin
                         and by_origin[instruction.operands[0].imm]['origin'] == 'library'}
            if not {0x00458C39, 0x004588A7} <= crt_transfers:
                raise ValueError('floor CRT fallback targets changed')
        elif address == 0x00456A8C:
            data_slots = {operand.mem.disp for instruction in instructions
                          for operand in instruction.operands
                          if operand.type == X86_OP_MEM and operand.mem.base == 0
                          and operand.mem.index == 0}
            if not set(range(0x004739C0, 0x004739D8, 4)) <= data_slots:
                raise ValueError('CRT conversion pointer table changed')
        elif address == 0x0045ED70:
            if 'fnstsw' not in [instruction.mnemonic for instruction in instructions]:
                raise ValueError('x87 status helper changed')
        if args.apply:
            if row['owner'] not in ('', 'library'):
                raise ValueError(f'refusing to replace runtime special owner: {address:#x}')
            origin.update(origin='library', subsystem='CRT', disposition='exclude',
                          confidence='medium', evidence_id=SPECIAL_EVIDENCE_ID)
            row.update(module='CRT', status='excluded', owner='library')
            if SPECIAL_EVIDENCE_ID not in row['notes']:
                note = (f'Target runtime behavior review: {behavior} '
                        f'({SPECIAL_EVIDENCE_ID}). No exact CRT object identity claimed.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    spec = importlib.util.spec_from_file_location(
        'review_import_origins', ROOT / 'scripts' / 'review-import-origins.py')
    imports = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(imports)
    if imports.import_slots(image).get(0x00466170) != ('KERNEL32.dll', 'TlsAlloc'):
        raise ValueError('CRT TLS allocation import changed')
    for address, (size, behavior) in CALLBACK_HELPERS.items():
        row = by_function[address]
        origin = by_origin[address]
        if int(row['size']) != size or (origin['disposition'] not in ('review', 'exclude')
                                         or origin['disposition'] == 'exclude'
                                         and origin['evidence_id'] != CALLBACK_EVIDENCE_ID):
            raise ValueError(f'CRT callback helper ledger conflict: {address:#x}')
        instructions = list(decoder.disasm(pe_bytes_at(image, address, size), address))
        if sum(item.size for item in instructions) != size:
            raise ValueError(f'CRT callback helper does not decode: {address:#x}')
        if address == 0x0045631E:
            call = instructions[0]
            writer = next(decoder.disasm(pe_bytes_at(image, 0x004565CA, 10), 0x004565CA))
            if (call.mnemonic != 'call' or call.operands[0].type != X86_OP_MEM
                    or call.operands[0].mem.disp != 0x00466170
                    or writer.mnemonic != 'mov' or writer.operands[0].type != X86_OP_MEM
                    or writer.operands[0].mem.disp != 0x00477374
                    or writer.operands[1].type != X86_OP_IMM
                    or writer.operands[1].imm != address
                    or not library_parent(0x004565CA)):
                raise ValueError('CRT TLS wrapper or runtime registration changed')
        elif address == 0x00456A8B:
            if [item.mnemonic for item in instructions] != ['ret']:
                raise ValueError('CRT no-op callback changed')
            for slot in (0x004733AC, 0x004733B0):
                if struct.unpack('<I', pe_bytes_at(image, slot, 4))[0] != address:
                    raise ValueError(f'CRT no-op callback slot changed: {slot:#x}')
            for site in (0x00452E1C, 0x004535BD):
                load = next(decoder.disasm(pe_bytes_at(image, site, 8), site))
                if (load.mnemonic != 'mov' or load.operands[1].type != X86_OP_MEM
                        or load.operands[1].mem.disp != 0x004733B0
                        or not library_parent(site)):
                    raise ValueError(f'CRT thread callback consumer changed: {site:#x}')
        elif address == 0x0045E3A0:
            if [item.mnemonic for item in instructions] != ['xor', 'ret']:
                raise ValueError('CRT zero-result callback changed')
            if struct.unpack('<I', pe_bytes_at(image, 0x00473658, 4))[0] != address:
                raise ValueError('CRT math callback slot changed')
            for site in (0x0045E940, 0x00465161):
                call = next(decoder.disasm(pe_bytes_at(image, site, 8), site))
                if (call.mnemonic != 'call' or call.operands[0].type != X86_OP_IMM
                        or call.operands[0].imm != address or not library_parent(site)):
                    raise ValueError(f'CRT math callback consumer changed: {site:#x}')
        if args.apply:
            if row['owner'] not in ('', 'library'):
                raise ValueError(f'refusing to replace CRT callback helper: {address:#x}')
            origin.update(origin='library', subsystem='CRT', disposition='exclude',
                          confidence='medium', evidence_id=CALLBACK_EVIDENCE_ID)
            row.update(module='CRT', status='excluded', owner='library')
            if CALLBACK_EVIDENCE_ID not in row['notes']:
                note = (f'Target {behavior} is registered or consumed only by '
                        f'reviewed CRT runtime code ({CALLBACK_EVIDENCE_ID}). '
                        f'Exact object provenance remains unknown.')
                row['notes'] = (row['notes'] + ' ' + note).strip()
    if args.apply:
        write_rows('function-origins.csv', origins)
        write_rows('functions.csv', functions)
    print(f'{len(WRAPPERS)} target-bound VC7.1 runtime wrappers reviewed')
    print(f'{len(SPECIALS)} target-bound runtime special entries reviewed')
    print(f'{len(CALLBACK_HELPERS)} target-bound CRT callback helpers reviewed')


if __name__ == '__main__':
    main()
