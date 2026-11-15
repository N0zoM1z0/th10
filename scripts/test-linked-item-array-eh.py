#!/usr/bin/env python3
"""Target-backed negative controls for the one permitted Item array EH binding.

No compiler/linker invocation. The fixture uses the authentic reviewed TH10
handler and a verified local linked PE/PDB, with independent in-memory edits.
An EH handler's address may match while its metadata is semantically wrong;
each mutation must be rejected before any exact-owner credit is possible.
"""
from __future__ import annotations

import copy
import functools
import runpy
import struct
from pathlib import Path
from dataclasses import replace
from unittest.mock import patch

import linked_item_array_eh as eh
from linked_image import LinkedImageError, PEImage, linked_functions, map_publics, map_statics, pdb_section_contributions

ROOT = Path(__file__).resolve().parents[1]
PRODUCT = ROOT / 'build/match-linked/ItemManagerDestructor'
REPLAY_PRODUCT = ROOT / 'build/match-linked/ClaudeReplayInitLoadScript'


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


@functools.lru_cache(maxsize=None)
def _inputs(name, product):
    """Parse the manifest, map, PDB and images once; cases mutate deep copies."""
    unit = runpy.run_path(str(ROOT / 'scripts/build-match-unit.py'))['load']()['units'][name]
    functions = linked_functions(product/'source.exe', product/'source.map', product/'source.pdb')['functions']
    aliases = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))['anchor_aliases'](product/'link-aliases.rsp')
    return (unit, functions, map_statics(product/'source.map'), pdb_section_contributions(product/'source.pdb'),
            map_publics(product/'source.map'), aliases, PEImage(product/'source.exe'), PEImage(ROOT/'resources/th10.exe'))


def fixture(name='item-manager-destructor-0041adf0', product=PRODUCT):
    unit, functions, statics, pdb, publics, aliases, candidate, target = (
        copy.deepcopy(v) if i < 6 else v for i, v in enumerate(_inputs(name, product)))
    field = next(f for f in unit['linkages'] if f.get('item_array_eh_handler'))
    parent, = [r for r in functions if r['symbol'] == unit['symbol']]
    handler, = [r for r in statics if r['symbol'] == '__ehhandler$' + unit['symbol']]
    return dict(unit=unit, field=field, parent=parent, statics=statics,
                pdb=pdb, publics=publics, aliases=aliases, product=product,
                address=handler['address'],
                candidate=MutableImage(candidate),
                target=MutableImage(target))


def bind(f):
    with patch.object(eh, 'map_statics', return_value=f['statics']), \
         patch.object(eh, 'pdb_section_contributions', return_value=f['pdb']):
        return eh.item_array_eh_target(f['unit'], f['field'], f['product'],
                                       f['candidate'], f['publics'],
                                       f['aliases'], f['target'], f['parent'],
                                       f['address'])


def main():
    f = fixture()
    proof = bind(f)
    assert proof['candidate']['action_bytes'] == 28
    assert proof['candidate']['handler_bytes'] == 10
    assert proof['candidate']['element_count'] == 0x896
    assert proof['candidate']['element_stride'] == 0x3F0
    assert proof['target']['member_offset'] == 0x14
    assert eh.item_array_eh_selector(f['unit'], {}) is False
    raw = runpy.run_path(str(ROOT/'scripts/compare-linked-function.py'))['expected_fields'](f['unit'])
    assert [x['offset'] for x in raw if x.get('item_array_eh_handler')] == [9]
    runpy.run_path(str(ROOT/'scripts/build-match-unit.py'))['validate_fields']('targeted-item', f['unit'], 216)
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
        ('false-opt-in', lambda q: q['field'].update(item_array_eh_handler=False)),
        ('mixed-handler', lambda q: q['field'].update(stage_array_eh_handler=True)),
        ('mixed-static', lambda q: q['field'].update(static_source='src/AnmManager.cpp')),
        ('wrong-parent', lambda q: q['unit'].update(symbol='?Other@@YAXXZ')),
        ('partial-parent', lambda q: q['unit'].update(compare_size=215)),
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
        ('target-element-count', lambda q: q['target'].word(target_action+6,0x897)),
        ('target-element-stride', lambda q: q['target'].word(target_action+11,0x3E0)),
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
            raise AssertionError('Item EH negative control accepted: '+label)
        checked += 1
    replay = lambda: fixture('replay-manager-destructor', REPLAY_PRODUCT)
    r = replay()
    proof = bind(r)
    assert proof['candidate']['action_bytes'] == proof['target']['action_bytes'] == 24
    assert proof['target']['element_count'] == 8 and proof['target']['element_stride'] == 0x24
    assert proof['target']['member_offset'] == 0xA0
    spec = eh.PARENTS['??1ReplayManager@@QAE@XZ']
    target_action = spec['handler'] - 24
    replay_cases = [
        ('replay-wrong-field-offset', lambda q: q['field'].update(offset=9)),
        ('replay-target-element-count', lambda q: q['target'].write(target_action+6, b'\x09')),
        ('replay-target-element-stride', lambda q: q['target'].write(target_action+8, b'\x28')),
        ('replay-target-member-offset', lambda q: q['target'].word(target_action+14, 0xA4)),
        ('replay-target-destructor', lambda q: q['target'].word(target_action+1, spec['target_destructor']+1)),
        ('replay-candidate-unwind-action', lambda q: q['candidate'].word(int(bind(replay())['candidate']['unwind_map'], 16)+4, q['address']-23)),
    ]
    for label, mutation in replay_cases:
        q = replay()
        try:
            mutation(q)
            bind(q)
        except (LinkedImageError, ValueError, IndexError, AssertionError) as exc:
            assert str(exc), label
        else:
            raise AssertionError('Replay EH negative control accepted: '+label)
        checked += 1
    print(f'item-array EH: 2 native/target positives, 1 unrelated-byte control, {checked} rejects OK')


if __name__ == '__main__':
    main()
