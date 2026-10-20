#!/usr/bin/env python3
"""Audit the original TH10 spell-panel extent, callback and sampling operands."""
from collections import Counter
import csv
import json
from pathlib import Path
import struct
import tomllib

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM
from linked_image import verify_capstone
from target_identity import pe_bytes_at, resolve_target, verify_target

ROOT = Path(__file__).resolve().parents[1]


def require(condition, message):
    if not condition:
        raise ValueError(message)


def main():
    path = resolve_target()
    identity, problems = verify_target(path)
    require(not problems, '; '.join(problems))
    with (ROOT / 'config/tools.lock.toml').open('rb') as stream:
        verify_capstone(tomllib.load(stream)['capstone'])
    raw = path.read_bytes()
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True

    def decode(start, size):
        instructions = list(decoder.disasm(pe_bytes_at(raw, start, size), start))
        require(sum(i.size for i in instructions) == size, 'incomplete decode')
        return instructions

    owner = decode(0x408D60, 1057)
    adapter = decode(0x409220, 10)
    require(owner[-1].address == 0x409180 and owner[-1].mnemonic == 'ret', 'owner tail differs')
    starts = {i.address for i in owner}
    require(all(i.operands[0].imm in starts for i in owner
                if i.group(CS_GRP_JUMP) and i.operands[0].type == X86_OP_IMM),
            'branch leaves the owner or enters an instruction interior')
    calls = Counter(i.operands[0].imm for i in owner
                    if i.mnemonic == 'call' and i.operands[0].type == X86_OP_IMM)
    require(calls == {0x43EE30: 5, 0x463B2C: 1, 0x449470: 2,
                      0x43E5A0: 6, 0x449350: 1}, 'call identities/counts differ')
    require([(i.mnemonic, i.op_str) for i in adapter] == [
        ('push', 'edi'), ('mov', 'edi, ecx'), ('call', '0x408d60'),
        ('pop', 'edi'), ('ret', '')], 'callback protocol differs')
    require(pe_bytes_at(raw, 0x409181, 15) == b'\xcc' * 15 and
            pe_bytes_at(raw, 0x40921F, 1) == b'\xcc' and
            pe_bytes_at(raw, 0x40922A, 6) == b'\xcc' * 6, 'neighbor padding differs')
    # Ghidra omits these two six-byte internal alignment instructions.
    for address in (0x408EDA, 0x408F5A):
        alignment = decode(address, 6)
        require(len(alignment) == 1 and alignment[0].mnemonic == 'lea' and
                alignment[0].op_str == 'ebx, [ebx]', 'internal alignment differs')
    registration = decode(0x4089C2, 5)
    require([(i.mnemonic, i.op_str) for i in registration] == [('push', '0x409220')],
            'registration address differs')
    observed = {i.address: (i.mnemonic, i.op_str) for i in owner}
    anchors = {
        0x408D71: ('cmp', 'dword ptr [edi + 0x3738], 0x3c'),
        0x408D86: ('cmp', 'dword ptr [edi + 0x3738], 0x12c'),
        0x408D95: ('test', 'byte ptr [edi + 0x378c], 8'),
        0x408D9E: ('mov', 'ecx, dword ptr [edi + 0x3794]'),
        0x408DDC: ('mov', 'dword ptr [edi + 0x3790], ecx'),
        0x408E5B: ('cmp', 'dword ptr [edi + 0x3738], 0x78'),
        0x408E81: ('fcomp', 'dword ptr [0x470c80]'),
        0x408F07: ('fcomp', 'dword ptr [0x470bf4]'),
        0x408F85: ('mov', 'ecx, dword ptr [edi + 0x3790]'),
        0x409052: ('mov', 'esi, dword ptr [0x47770c]'),
        0x409084: ('mov', 'eax, dword ptr [0x474c68]'),
        0x409089: ('mov', 'edx, dword ptr [0x474c6c]'),
        0x40908F: ('mov', 'ebp, dword ptr [0x47783c]'),
        0x40910B: ('mov', 'eax, dword ptr [edx + 0x10]'),
        0x40910E: ('fld', 'dword ptr [eax + 0x1068]'),
    }
    require(all(observed.get(a) == pair for a, pair in anchors.items()), 'sampling operand differs')
    for address in (0x409058, 0x409070, 0x4090C7, 0x4090DD):
        require(observed.get(address) == ('mov', 'ecx, dword ptr [esi + 0x9ec8]'),
                'counter resource does not reload from the captured GUI')
    for address, value in [(0x470C80, 96.), (0x470BF4, 128.), (0x470C84, .05)]:
        require(pe_bytes_at(raw, address, 4) == struct.pack('<f', value), 'float constant differs')
    bonus_start = decode(0x409889, 29)
    require([(i.mnemonic, i.op_str) for i in bonus_start] == [
        ('cmp', 'eax, 0x5f5e100'), ('mov', 'dword ptr [ebp + 0x3790], eax'),
        ('mov', 'dword ptr [ebp + 0x3794], eax'), ('jl', '0x4098a6'),
        ('mov', 'dword ptr [ebp + 0x3794], 0x5f5e0ff')], 'spell-start bonus stores differ')
    with (ROOT / 'config/function-boundaries.csv').open(newline='') as stream:
        rows = {int(r['address'], 0): r for r in csv.DictReader(stream)}
    for address, size in [(0x408D60, 1057), (0x409220, 10)]:
        require(rows[address]['state'] == 'reviewed' and
                int(rows[address]['span_end'], 0) == address + size - 1, 'boundary ledger differs')
    print(json.dumps({'result': 'passed', 'target': identity,
        'authority': 'target observations only; no exact/runtime credit',
        'owners': [{'address': '0x408d60', 'owned_bytes': 1057,
                    'ghidra_body_bytes': 1045, 'internal_alignment_bytes': 12},
                   {'address': '0x409220', 'owned_bytes': 10, 'origin': 'indeterminate'}],
        'calls': {hex(a): n for a, n in sorted(calls.items())},
        'registration_site': '0x4089c2',
        'bonus_current': 'owner+3790', 'bonus_initial': 'owner+3794',
        'spell_start_caps_only_initial_slot': True}, indent=2))


if __name__ == '__main__':
    main()
