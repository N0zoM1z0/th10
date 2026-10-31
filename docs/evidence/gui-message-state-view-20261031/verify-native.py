#!/usr/bin/env python3
"""Reproduce the complete nonexact GUI owner and bounded target-bound gains."""

import argparse
import hashlib
import json
from pathlib import Path
import runpy

ROOT = Path(__file__).resolve().parents[3]
PREVIOUS = ROOT / 'docs/evidence/gui-message-call-closure-20261031'
PRIOR = runpy.run_path(str(PREVIOUS / 'verify-native.py'))
REVIEW = PRIOR['REVIEW']
STATE = '?g_GameStageState@@3PAEA'
MANAGER = '?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A'


def bound_region(target, image, address, publics, calls, target_offset,
                 native_offset, size, native_advance):
    """Check every byte and explicitly resolve every field in a named region.

    These regions are observations within a nonexact owner, never match units.
    The enclosing owner still requires its complete strict comparison.
    """
    decoder = REVIEW['Cs'](REVIEW['CS_ARCH_X86'], REVIEW['CS_MODE_32'])
    decoder.detail = True
    target_raw = bytearray(target.read_address(0x415E90 + target_offset, size))
    native_raw = bytearray(image.read_address(address + native_offset, size))
    ti = list(decoder.disasm(target_raw, 0x415E90 + target_offset))
    ni = list(decoder.disasm(native_raw, address + native_offset))
    assert sum(i.size for i in ti) == sum(i.size for i in ni) == size
    assert len(ti) == len(ni)
    bindings = []
    data = {0x491C10: (MANAGER, 0)}
    data.update({0x474C40 + n: (STATE, n) for n in (0x28, 0x2c, 0x34, 0x3c)})
    for t, n in zip(ti, ni):
        tpos = t.address - 0x415E90 - target_offset
        npos = n.address - address - native_offset
        assert tpos == npos and t.size == n.size
        field = None
        if t.mnemonic == 'call':
            assert t.operands[0].type == n.operands[0].type == REVIEW['CS_OP_IMM']
            symbol = calls[t.operands[0].imm]
            assert n.operands[0].imm == publics[symbol]
            field = (t.imm_offset, n.imm_offset, t.imm_size, n.imm_size)
            binding = {'symbol': symbol, 'target': hex(t.operands[0].imm)}
        elif t.group(REVIEW['CS_GRP_JUMP']):
            assert t.mnemonic == n.mnemonic == 'jmp'
            assert t.operands[0].type == n.operands[0].type == REVIEW['CS_OP_IMM']
            assert t.operands[0].imm == 0x416F09
            assert n.operands[0].imm == address + native_advance
            field = (t.imm_offset, n.imm_offset, t.imm_size, n.imm_size)
            binding = {'local_target': 0x1079, 'native_destination': native_advance}
        elif t.disp_size == 4 and any(
                o.type == REVIEW['CS_OP_MEM'] and o.mem.base == 0 and o.mem.index == 0
                for o in t.operands):
            tm = next(o.mem for o in t.operands if o.type == REVIEW['CS_OP_MEM']
                      and o.mem.base == 0 and o.mem.index == 0)
            nm = next(o.mem for o in n.operands if o.type == REVIEW['CS_OP_MEM']
                      and o.mem.base == 0 and o.mem.index == 0)
            symbol, addend = data[tm.disp]
            assert nm.disp == publics[symbol] + addend
            field = (t.disp_offset, n.disp_offset, t.disp_size, n.disp_size)
            binding = {'symbol': symbol, 'addend': addend, 'target': hex(tm.disp)}
        if field is not None:
            to, no, tw, nw = field
            assert to == no and tw == nw == 4
            target_raw[tpos + to:tpos + to + tw] = bytes(tw)
            native_raw[npos + no:npos + no + nw] = bytes(nw)
            bindings.append({'instruction_offset': tpos, **binding})
    assert target_raw == native_raw, 'ordinary bytes differ in reviewed region'
    return {'target_offset': target_offset, 'native_offset': native_offset,
            'size': size, 'instructions': len(ti), 'bindings': bindings,
            'normalized_sha256': hashlib.sha256(target_raw).hexdigest()}


def gains(directory, actual, proof):
    image = REVIEW['PEImage'](directory / 'source.exe')
    target = REVIEW['PEImage'](ROOT / 'resources/th10.exe')
    publics = REVIEW['map_publics'](directory / 'source.map')
    publics = {row['symbol']: row['address'] for row in publics}
    functions = REVIEW['linked_functions'](directory / 'source.exe', directory / 'source.map',
                                           directory / 'source.pdb')['functions']
    owner = next(f for f in functions if f['symbol'] == '?Run@GuiMessageVmView@@SGHPAU1@@Z')
    address = int(owner['address'], 0)
    previous = json.loads((ROOT / 'docs/evidence/gui-message-run-context-20261030/results.json').read_text())
    calls = {int(r['target'], 0): r['symbol'] for r in previous['target_call_roles']}
    decoder = REVIEW['Cs'](REVIEW['CS_ARCH_X86'], REVIEW['CS_MODE_32'])
    decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(address,
                                        actual['code_and_alignment_bytes']), address))
    sound = publics['?GameQueueScaledSoundCommand@@YGHM@Z']
    advance = next(i.address + i.size - address for i in instructions
                   if i.mnemonic == 'call' and i.operands[0].type == REVIEW['CS_OP_IMM']
                   and i.operands[0].imm == sound)
    regions = {}
    for name, target_offset, native_offset, size in (
            ('side_0', 0x41f, 0x41f, 82),
            ('side_1', 0x471, 0x471, 88),
            ('second_profile_flag', 0xd80, 0xd7f, 45)):
        regions[name] = bound_region(target, image, address, publics, calls,
                                     target_offset, native_offset, size, advance)
    assert regions == proof['reviewed_regions']
    native_by_offset = {i.address - address: i for i in instructions}
    selection_reads = []
    for t in decoder.disasm(target.read_address(0x415E90, 0xd43), 0x415E90):
        for operand in t.operands:
            if (operand.type != REVIEW['CS_OP_MEM'] or operand.mem.base != 0
                    or operand.mem.index != 0 or operand.mem.disp not in
                    (0x474C68, 0x474C6C, 0x474C74, 0x474C7C)):
                continue
            offset = t.address - 0x415E90
            n = native_by_offset[offset]
            addend = operand.mem.disp - 0x474C40
            assert n.mnemonic == t.mnemonic and n.disp_size == t.disp_size == 4
            assert any(o.type == REVIEW['CS_OP_MEM'] and o.mem.base == 0
                       and o.mem.index == 0 and o.mem.disp == publics[STATE] + addend
                       for o in n.operands)
            selection_reads.append({'offset': offset, 'symbol': STATE, 'addend': addend,
                                    'target': hex(operand.mem.disp)})
    assert selection_reads == proof['selection_reads_before_completion']
    return regions


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', required=True, type=Path)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / 'build')
    proof = json.loads(Path(__file__).with_name('results.json').read_text())
    actual = PRIOR['verify'](directory, proof)
    regions = gains(directory, actual, proof)
    print(json.dumps({'result': 'complete-native-candidate-nonexact',
                      'candidate_size': actual['complete_owned_bytes'], 'target_size': 4480,
                      'fields': len(actual['field_identities']), 'ordered_calls': 150,
                      'hidden_return_slots': 54, 'reviewed_regions': list(regions),
                      'exact_credit': 0}))


if __name__ == '__main__':
    main()
