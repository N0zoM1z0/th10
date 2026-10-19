#!/usr/bin/env python3
"""Audit four Ending creation contracts in native code; grants no exact credit.

This deliberately checks historical-compiler output as well as the target.
C++17 host evaluation order cannot prove the destination was read after a call
in the reconstructed VC7.1 image. Complete-owner exactness is a separate Oracle.
It accepts the observed EBP receiver and18-entry table shape; it is not a
general semantic equivalence checker.
"""
from pathlib import Path
import argparse
import hashlib
import json
import struct
import tomllib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_EBP
from linked_image import PEImage, linked_functions, verify_capstone

ROOT = Path(__file__).resolve().parents[1]
RUN_SYMBOL = '?Run@EndingTextVmView@@QAGHXZ'
CREATE_SYMBOL = '?CreateVmVariant0@AnmLoadedView@@QAE?AUAnmVmIdView@@HI@Z'
SECONDARY_SYMBOL = '?CreateVmVariant1@AnmLoadedView@@QAE?AUAnmVmIdView@@HI@Z'


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


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--directory', type=Path, required=True,
                        help='linked carrier with source.exe, source.map and source.pdb')
    parser.add_argument('--json', type=Path)
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
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=2)+'\n')
    print('PASS' if result['passed'] else 'FAIL',
          'native Ending primary-list and post-creation destination contracts')
    return 0 if result['passed'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
