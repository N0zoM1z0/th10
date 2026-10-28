#!/usr/bin/env python3
"""Audit four Ending creation contracts in native code; grants no exact credit.

This deliberately checks historical-compiler output as well as the target.
C++17 host evaluation order cannot prove the destination was read after a call
in the reconstructed VC7.1 image. Complete-owner exactness is a separate Oracle.
It accepts the observed EBP receiver and18-entry table shape; it is not a
general semantic equivalence checker. Optional initializer checks cover the
selection destination and post-allocation header sampling.
"""
from pathlib import Path
import argparse
import hashlib
import json
import struct
import tomllib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_CALL, CS_GRP_JUMP
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_EBP
from linked_image import PEImage, linked_functions, map_publics, verify_capstone, linked_code_fields

ROOT = Path(__file__).resolve().parents[1]
RUN_SYMBOL = '?Run@EndingTextVmView@@QAGHXZ'
CREATE_SYMBOL = '?CreateVmVariant0@AnmLoadedView@@QAE?AUAnmVmIdView@@HI@Z'
SECONDARY_SYMBOL = '?CreateVmVariant1@AnmLoadedView@@QAE?AUAnmVmIdView@@HI@Z'
INITIALIZE_SYMBOL = '?Initialize@EndingView@@QAEHXZ'
TEXT_CONSTRUCTOR = '??0EndingTextVmView@@QAE@PAE@Z'
SCREEN_CREATOR = '?CreateVmAtScreenVariant0@AnmLoadedView@@QAE?AUAnmVmIdView@@HPBUAnmFloat3View@@@Z'


def mapped_section(image, address):
    """Recognize section addresses independently of readable file bytes.

    BSS symbols are address identities even though read_address deliberately
    refuses to manufacture their initializer bytes.
    """
    rva = address - image.image_base
    for section in image.sections:
        span = section.virtual_size or section.raw_size
        if section.rva <= rva < section.rva + span:
            return section
    return None


def inventory_run(image, address, size):
    """Inventory complete code, fixed local destinations and the owned table.

    Syntactic address recognition is diagnostic; it is not semantic binding
    or permission to normalize a changed local offset.
    """
    table = address + size - 18 * 4
    words = struct.unpack('<18I', image.read_address(table, 72))
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(address, size - 72), address))
    if sum(i.size for i in instructions) != size - 72:
        raise ValueError('complete Run code/alignment does not densely decode')
    starts = {i.address for i in instructions}
    if not all(word in starts for word in words):
        raise ValueError('Run table destination is not an owned instruction start')
    fields, locals_, unbacked = [], [], []
    dispatchers = 0
    def field(offset, width, kind, destination):
        if width != 4:
            raise ValueError('unsupported linked field width in complete Run')
        row = dict(offset=offset, width=width, type=kind, destination=hex(destination))
        fields.append(row)
        return row
    for i in instructions:
        control = i.group(CS_GRP_CALL) or i.group(CS_GRP_JUMP)
        if control and i.operands and i.operands[0].type == X86_OP_IMM:
            destination = i.operands[0].imm & 0xffffffff
            if address <= destination < table:
                if destination not in starts:
                    raise ValueError('Run local branch leaves instruction boundaries')
                locals_.append(dict(offset=i.address - address,
                                    destination_offset=destination - address))
            else:
                field(i.address - address + i.imm_offset, i.imm_size, 'REL32', destination)
        for op in i.operands:
            if op.type == X86_OP_MEM and not op.mem.base:
                destination = op.mem.disp & 0xffffffff
                if mapped_section(image, destination) is not None:
                    field(i.address - address + i.disp_offset, i.disp_size, 'DIR32', destination)
                    if i.mnemonic == 'jmp' and op.mem.index:
                        if destination != table or op.mem.scale != 4:
                            raise ValueError('Run dispatcher does not select the complete trailing table')
                        dispatchers += 1
            elif op.type == X86_OP_IMM and not control:
                destination = op.imm & 0xffffffff
                section = mapped_section(image, destination)
                if section is not None:
                    row = field(i.address - address + i.imm_offset, i.imm_size, 'DIR32', destination)
                    if destination - image.image_base - section.rva >= section.raw_size:
                        unbacked.append(dict(row, section=section.name))
    if dispatchers != 1:
        raise ValueError('Run lacks one complete indexed dispatcher')
    for index, destination in enumerate(words):
        field(table - address + index * 4, 4, 'DIR32', destination)
    keys = [(f['offset'], f['width'], f['type']) for f in fields]
    if len(set(keys)) != len(keys):
        raise ValueError('duplicate linked field in Run inventory')
    return dict(size=size, code_and_alignment_size=size - 72, table_words=18,
                fields=sorted(fields, key=lambda f: f['offset']),
                linked_field_count=len(fields), local_branches=locals_,
                table_local_offsets=[word - address for word in words],
                virtual_only_immediate_fields=unbacked)


def register_load(instruction, base, displacement):
    operands = instruction.operands
    if (instruction.mnemonic == 'mov' and len(operands) == 2
            and operands[0].type == X86_OP_REG and operands[1].type == X86_OP_MEM
            and operands[1].mem.base == base and not operands[1].mem.index
            and operands[1].mem.disp == displacement):
        return operands[0].reg
    return None


def late_destination(instructions, call):
    for pc_index in range(call + 1, len(instructions)):
        pc_register = register_load(instructions[pc_index], X86_REG_EBP, 0x54)
        if pc_register is None:
            continue
        for index in range(pc_index + 1, len(instructions)):
            operand_register = register_load(instructions[index], pc_register, 4)
            if operand_register is None:
                _, written = instructions[index].regs_access()
                if pc_register in written:
                    break
                continue
            for store in instructions[index + 1:]:
                operands = store.operands
                if (store.mnemonic == 'mov' and len(operands) == 2
                        and operands[0].type == X86_OP_MEM
                        and operands[0].mem.base == X86_REG_EBP
                        and operands[0].mem.index == operand_register
                        and operands[0].mem.scale == 4
                        and operands[0].mem.disp == 0x90):
                    return dict(cursor_load=hex(instructions[pc_index].address),
                                operand_load=hex(instructions[index].address),
                                destination_store=hex(store.address))
                _, written = store.regs_access()
                if operand_register in written:
                    break
            break
    return None


def inspect(image, address, size, primary, secondary):
    table = address + size - 18 * 4
    destinations = struct.unpack('<18I', image.read_address(table, 18 * 4))
    if not all(address <= dest < table for dest in destinations):
        raise ValueError('18-entry trailing table leaves the Run code extent')
    labels = sorted(set(destinations)) + [table]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    records = []
    for opcode in [8, 15, 16, 17]:
        start = destinations[opcode]
        end = labels[labels.index(start) + 1]
        instructions = list(decoder.disasm(image.read_address(start, end - start), start))
        if sum(item.size for item in instructions) != end - start:
            raise ValueError('creation case is not densely decoded')
        calls = [(index, item) for index, item in enumerate(instructions)
                 if item.mnemonic == 'call' and len(item.operands) == 1
                 and item.operands[0].type == X86_OP_IMM
                 and item.operands[0].imm in [primary, secondary]]
        if len(calls) != 1:
            raise ValueError('creation case lacks one uniquely bound creator call')
        index, call = calls[0]
        live = late_destination(instructions, index)
        records.append(dict(opcode=opcode, creator_call=hex(call.address),
                            uses_primary_creator=call.operands[0].imm == primary,
                            destination_loaded_after_call=live is not None,
                            destination=live))
    return dict(records=records, passed=all(row['uses_primary_creator'] and
                row['destination_loaded_after_call'] for row in records))


def inspect_initialize(image, address, size, creator, allocator, constructor):
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(address, size), address))
    if sum(item.size for item in instructions) != size:
        raise ValueError('initializer is not densely decoded')
    def calls(destination):
        return [index for index, item in enumerate(instructions)
                if item.mnemonic == 'call' and len(item.operands) == 1
                and item.operands[0].type == X86_OP_IMM
                and item.operands[0].imm == destination]
    creation_calls, constructor_calls = calls(creator), calls(constructor)
    if len(creation_calls) != 1 or len(constructor_calls) != 1:
        raise ValueError('initializer lacks unique selection/constructor calls')
    call = creation_calls[0]
    selection = None
    for index, item in enumerate(instructions[:call]):
        operands = item.operands
        if (item.mnemonic != 'lea' or len(operands) != 2
                or operands[0].type != X86_OP_REG or operands[1].type != X86_OP_MEM
                or operands[1].mem.disp != 0x89a4 or operands[1].mem.index):
            continue
        register = operands[0].reg
        for position in range(index + 1, len(instructions)):
            later = instructions[position]
            if (position > call and later.mnemonic == 'mov' and later.operands
                    and later.operands[0].type == X86_OP_MEM
                    and later.operands[0].mem.base == register
                    and not later.operands[0].mem.index and later.operands[0].mem.disp == 0):
                selection = dict(address_capture=hex(item.address),
                                 creator_call=hex(instructions[call].address),
                                 destination_store=hex(later.address))
                break
            _, written = later.regs_access()
            if register in written:
                break
        if selection:
            break
    ctor = constructor_calls[0]
    allocation_calls = [index for index in calls(allocator) if index < ctor]
    if not allocation_calls:
        raise ValueError('text construction lacks its preceding allocation')
    allocation = allocation_calls[-1]
    header = None
    for index in range(allocation + 1, ctor):
        register = register_load(instructions[index], X86_REG_EBP, 0x14)
        if register is None:
            continue
        for later in instructions[index + 1:ctor]:
            if register_load(later, register, 4) is not None:
                header = dict(allocation_call=hex(instructions[allocation].address),
                              buffer_load=hex(instructions[index].address),
                              header_load=hex(later.address),
                              constructor_call=hex(instructions[ctor].address))
                break
            _, written = later.regs_access()
            if register in written:
                break
        if header:
            break
    return dict(selection_destination_captured_before_call=selection is not None,
                header_read_after_allocation=header is not None,
                selection=selection, header=header,
                passed=selection is not None and header is not None)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--directory', type=Path, required=True,
                        help='linked carrier with source.exe, source.map and source.pdb')
    parser.add_argument('--json', type=Path)
    parser.add_argument('--include-initialize', action='store_true',
                        help='also audit selection-owner and allocation/header sampling')
    parser.add_argument('--inventory', action='store_true',
                        help='inventory all Run fields and fixed locals, including BSS addresses')
    args = parser.parse_args()
    with (ROOT/'config/tools.lock.toml').open('rb') as stream:
        verify_capstone(tomllib.load(stream)['capstone'])
    with (ROOT/'config/target.toml').open('rb') as stream:
        expected = tomllib.load(stream)['target']
    path = ROOT/'resources/th10.exe'
    data = path.read_bytes()
    if len(data) != expected['size'] or hashlib.sha256(data).hexdigest() != expected['sha256']:
        raise ValueError('wrong TH10 target')
    target = inspect(PEImage(path), 0x40bd80, 1568, 0x448d00, 0x448f60)
    if not target['passed']:
        raise ValueError('target no longer proves the declared creation contracts')
    directory = args.directory.resolve()
    def artifact_hashes():
        return {name: hashlib.sha256((directory/name).read_bytes()).hexdigest()
                for name in ['source.exe', 'source.map', 'source.pdb']}
    hashes = artifact_hashes()
    functions = linked_functions(directory/'source.exe', directory/'source.map',
                                 directory/'source.pdb')['functions']
    def owner(symbol):
        rows = [row for row in functions if row['symbol'] == symbol]
        if len(rows) != 1:
            raise ValueError('no unique PE/MAP/PDB-bound owner: ' + symbol)
        return rows[0]
    run, primary, secondary = [owner(name) for name in
                               [RUN_SYMBOL, CREATE_SYMBOL, SECONDARY_SYMBOL]]
    result = inspect(PEImage(directory/'source.exe'), int(run['address'], 0),
                     run['size'], int(primary['address'], 0), int(secondary['address'], 0))
    if artifact_hashes() != hashes:
        raise ValueError('native carrier changed during the audit')
    report = dict(scope=__doc__, target_sha256=expected['sha256'], target=target,
                  native=result, run_owner=run, passed=result['passed'],
                  artifact_sha256=hashes,
                  exactness_authority='none; complete-owner canonical replay is separate')
    if args.inventory:
        native_image = PEImage(directory/'source.exe')
        native_inventory = inventory_run(native_image, int(run['address'], 0), run['size'])
        linked = linked_code_fields(native_image, map_publics(directory/'source.map'),
                                    int(run['address'], 0), run['size'])
        keys = lambda rows: {(r['offset'], r['width'], r['type']) for r in rows}
        if not linked['normalization_complete'] or keys(linked['fields']) != keys(native_inventory['fields']):
            raise ValueError('complete native syntax and authoritative linked fields disagree')
        report['inventory'] = dict(target=inventory_run(PEImage(path), 0x40bd80, 1568),
                                   native=native_inventory,
                                   native_linked_field_coverage=True,
                                   authority='inventory only; no parent exactness or semantic binding')
        if artifact_hashes() != hashes:
            raise ValueError('native carrier changed during complete inventory')
    if args.include_initialize:
        target_init = inspect_initialize(PEImage(path), 0x40b560, 584,
                                         0x448d50, 0x452493, 0x40ba90)
        if not target_init['passed']:
            raise ValueError('target does not prove initializer sampling contracts')
        init, screen, text = [owner(name) for name in
                              [INITIALIZE_SYMBOL, SCREEN_CREATOR, TEXT_CONSTRUCTOR]]
        allocators = [row for row in map_publics(directory/'source.map')
                      if row['symbol'] == '??2@YAPAXI@Z']
        if len(allocators) != 1:
            raise ValueError('no unique allocator identity')
        native_init = inspect_initialize(PEImage(directory/'source.exe'),
                          int(init['address'], 0), init['size'],
                          int(screen['address'], 0), allocators[0]['address'],
                          int(text['address'], 0))
        if artifact_hashes() != hashes:
            raise ValueError('native carrier changed during initializer audit')
        report.update(initialize_target=target_init, initialize_native=native_init,
                      initialize_owner=init,
                      passed=report['passed'] and native_init['passed'])
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=2)+'\n')
    print('PASS' if result['passed'] else 'FAIL',
          'native Ending primary-list and post-creation destination contracts')
    if args.include_initialize:
        print('PASS' if native_init['passed'] else 'FAIL',
              'native Ending selection-owner and allocation/header contracts')
    if args.inventory:
        print('PASS complete Run inventory; target', report['inventory']['target']['linked_field_count'],
              'native', native_inventory['linked_field_count'],
              'fields; no complete-owner exactness claim')
    return 0 if report['passed'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
