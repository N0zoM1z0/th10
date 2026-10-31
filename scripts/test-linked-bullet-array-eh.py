#!/usr/bin/env python3
"""Target-backed negative controls for the one permitted Bullet array EH binding.

No compiler/linker invocation. The fixture uses the authentic reviewed TH10
handler and a verified local linked PE/PDB, with independent in-memory edits.
An EH handler's address may match while its metadata is semantically wrong;
each mutation must be rejected before any exact-owner credit is possible.
"""
from __future__ import annotations

import copy
import runpy
import struct
from pathlib import Path
from dataclasses import replace
from unittest.mock import patch

import linked_bullet_array_eh as eh
from linked_image import LinkedImageError, PEImage, linked_functions, map_publics, map_statics, pdb_section_contributions

ROOT = Path(__file__).resolve().parents[1]
PRODUCT = ROOT / 'build/match-linked/ScriptViewerBulletCleanup'


class MutableImage:
    def __init__(self, image: PEImage):
        self.raw = image
        self.image_base = image.image_base
        self.sections = copy.deepcopy(image.sections)
        self.changes: dict[int, int] = {}

    def section(self, number):
        return self.sections[number - 1]

    def read_address(self, address, size):
        raw = bytearray(self.raw.read_address(address, size))
        for addr, value in self.changes.items():
            if address <= addr < address + size:
                raw[addr - address] = value
        return bytes(raw)

    def write(self, address, payload):
        # Require that every modified byte exists within the original PE.
        self.raw.read_address(address, len(payload))
        for off, byte in enumerate(payload):
            self.changes[address + off] = byte

    def word(self, address, value):
        self.write(address, struct.pack('<I', value & 0xFFFFFFFF))


def fixture():
    unit = copy.deepcopy(runpy.run_path(str(ROOT / 'scripts/build-match-unit.py'))['load']()['units']['script-viewer-bullet-destructor'])
    field = next(f for f in unit['linkages'] if f.get('bullet_array_eh_handler'))
    functions = linked_functions(PRODUCT/'source.exe', PRODUCT/'source.map', PRODUCT/'source.pdb')['functions']
    parent, = [r for r in functions if r['symbol'] == eh.PARENT]
    statics = map_statics(PRODUCT/'source.map')
    pdb = pdb_section_contributions(PRODUCT/'source.pdb')
    publics = map_publics(PRODUCT/'source.map')
    aliases = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))['anchor_aliases'](PRODUCT/'link-aliases.rsp')
    handler, = [r for r in statics if r['symbol'] == eh.HANDLER]
    return dict(unit=unit, field=field, parent=parent, statics=statics,
                pdb=pdb, publics=publics, aliases=aliases,
                address=handler['address'],
                candidate=MutableImage(PEImage(PRODUCT/'source.exe')),
                target=MutableImage(PEImage(ROOT/'resources/th10.exe')))


def bind(f):
    with patch.object(eh, 'map_statics', return_value=f['statics']), \
         patch.object(eh, 'pdb_section_contributions', return_value=f['pdb']):
        return eh.bullet_array_eh_target(f['unit'], f['field'], PRODUCT,
                                       f['candidate'], f['publics'],
                                       f['aliases'], f['target'], f['parent'],
                                       f['address'])


def main():
    f = fixture()
    proof = bind(f)
    assert proof['candidate']['action_bytes'] == 28
    assert proof['candidate']['handler_bytes'] == 10
    assert proof['candidate']['element_count'] == 0x7D1
    assert proof['candidate']['element_stride'] == 0x7F0
    assert proof['target']['member_offset'] == 0x60
    assert eh.bullet_array_eh_selector(f['unit'], {}) is False
    raw = runpy.run_path(str(ROOT/'scripts/compare-linked-function.py'))['expected_fields'](f['unit'])
    assert [x['offset'] for x in raw if x.get('bullet_array_eh_handler')] == [9]
    runpy.run_path(str(ROOT/'scripts/build-match-unit.py'))['validate_fields']('targeted-bullet', f['unit'], 236)
    # Changing unrelated rdata must not be mistaken for a metadata failure.
    other = fixture()
    other['target'].word(0x4715E8, 0xAABBCCDD)
    bind(other)

    address = f['address']; action = address - 28
    candidate_info = int(proof['candidate']['func_info'], 16)
    candidate_unwind = int(proof['candidate']['unwind_map'], 16)
    target_info = int(proof['target']['func_info'], 16)
    target_action = eh.TARGET_HANDLER - 28
    target_unwind = int(proof['target']['unwind_map'], 16)

    cases = [
        ('false-opt-in', lambda q: q['field'].update(bullet_array_eh_handler=False)),
        ('mixed-handler', lambda q: q['field'].update(stage_array_eh_handler=True)),
        ('mixed-static', lambda q: q['field'].update(static_source='src/AnmManager.cpp')),
        ('wrong-parent', lambda q: q['unit'].update(symbol='?Other@@YAXXZ')),
        ('partial-parent', lambda q: q['unit'].update(compare_size=235)),
        ('wrong-target-owner', lambda q: q['unit'].update(target_address=eh.TARGET_PARENT+4)),
        ('bad-field-offset', lambda q: q['field'].update(offset=8)),
        ('wrong-handler-address', lambda q: q['field'].update(target=eh.TARGET_HANDLER+1)),
        ('duplicate-map', lambda q: q['statics'].append(copy.deepcopy(q['statics'][next(i for i,r in enumerate(q['statics']) if r['symbol']==eh.HANDLER)]))),
        ('candidate-operand-interior', lambda q: q.update(address=q['address']+1)),
        ('wrong-map-producer', lambda q: next(r for r in q['statics'] if r['symbol']==eh.HANDLER).update(object='other')),
        ('other-parent-pdb', lambda q: q['parent'].update(pdb_module_index=-1)),
        ('wrong-pdb-region-length', lambda q: next(c for c in q['pdb']['contributions'] if c['section_offset']+28==q['address']-q['candidate'].image_base-0x1000).update(size=37)),
        ('mutable-target-info', lambda q: q['target'].sections.__setitem__(1, replace(q['target'].sections[1],characteristics=0xC0000040))),
        ('target-handler-magic', lambda q: q['target'].write(eh.TARGET_HANDLER,b'\x90')),
        ('candidate-handler-opcode', lambda q: q['candidate'].write(address,b'\x90')),
        ('candidate-info-magic', lambda q: q['candidate'].word(candidate_info,0x19930521)),
        ('target-state-count', lambda q: q['target'].word(target_info+4,2)),
        ('target-unwind-predecessor', lambda q: q['target'].word(target_unwind,0)),
        ('candidate-unwind-action', lambda q: q['candidate'].word(candidate_unwind+4,action+1)),
        ('target-destructor-address', lambda q: q['target'].word(target_action+1,eh.TARGET_DESTRUCTOR+4)),
        ('target-element-count', lambda q: q['target'].word(target_action+6,0x7D2)),
        ('target-element-stride', lambda q: q['target'].word(target_action+11,0x7E0)),
        ('candidate-saved-this', lambda q: q['candidate'].write(action+18,b'\x08')),
        ('target-array-member-offset', lambda q: q['target'].write(target_action+19,b'\x64')),
        ('candidate-iterator-displacement', lambda q: q['candidate'].word(action+23,0)),
        ('target-frame-handler', lambda q: q['target'].word(eh.TARGET_HANDLER+6,0)),
        ('unknown-callee-symbol', lambda q: q['aliases'].update({eh.ITERATOR:'nonexistent'})),
    ]
    checked = 0
    for label, mutation in cases:
        q = fixture()
        try:
            mutation(q)
            bind(q)
        except (LinkedImageError, ValueError, IndexError, AssertionError) as exc:
            assert str(exc), label
        else:
            raise AssertionError('Bullet EH negative control accepted: '+label)
        checked += 1
    print(f'bullet-array EH: 1 native/target positive, 1 unrelated-byte control, {checked} rejects OK')


if __name__ == '__main__':
    main()
