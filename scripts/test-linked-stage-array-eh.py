#!/usr/bin/env python3
"""Synthetic address/metadata controls for the two Stage array EH bindings."""
import copy
import runpy
import struct
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

from linked_image import LinkedImageError
import linked_stage_array_eh as eh


class Image:
    image_base = 0

    def __init__(self, handler, frame, destructor, iterator, free, arrays, base, data):
        self.sections = [SimpleNamespace(rva=base, virtual_size=0x100000, characteristics=0x60000020),
                         SimpleNamespace(rva=data, virtual_size=0x100, characteristics=0x40000040)]
        start = handler - 27 * len(arrays)
        code = bytearray()
        unwind = bytearray()
        for index, (member, count) in enumerate(arrays):
            action = start + len(code)
            code += b'\x68' + struct.pack('<I', destructor) + b'\x6a' + bytes([count])
            code += b'\x68' + struct.pack('<I', 0x3ac) + b'\x8b\x45\x04\x05'
            code += struct.pack('<I', member) + b'\x50\xe8'
            code += struct.pack('<i', iterator - action - 26) + b'\xc3'
            unwind += struct.pack('<iI', index - 1, action)
        code += b'\xb8' + struct.pack('<I', data + 0x20)
        code += b'\xe9' + struct.pack('<i', frame - handler - 10)
        info = bytearray(0x100)
        info[:len(unwind)] = unwind
        struct.pack_into('<7I', info, 0x20, 0x19930520, len(arrays), data, 0, 0, 0, 0)
        # A synthetic dense34-byte callee with the same single-field inventory.
        cleanup = b'\x90' * 14 + b'\xe8' + struct.pack('<i', free - destructor - 19)
        cleanup += b'\x90' * 14 + b'\xc3'
        self.memory = {start: code, data: info, destructor: bytearray(cleanup)}

    def section(self, number):
        return self.sections[number - 1]

    def read_address(self, address, size):
        for start, data in self.memory.items():
            if start <= address and address + size <= start + len(data):
                return bytes(data[address - start:address - start + size])
        raise LinkedImageError('synthetic unmapped range')

    def write(self, address, value):
        for start, data in self.memory.items():
            if start <= address and address + len(value) <= start + len(data):
                data[address - start:address - start + len(value)] = value
                return
        raise AssertionError('control leaves fixture')


def fixture(symbol):
    spec = eh.PARENTS[symbol]
    arrays = spec['arrays']
    lead = 27 * len(arrays)
    address = 0x111000 + lead
    stage_object = 'Z:\\synthetic\\support-00-StageLifecycle.ltcg.obj'
    vm_object = 'Z:\\synthetic\\support-01-AnmManager.ltcg.obj'
    unit = dict(artifact_kind='linked-pe', source='src/StageScript.cpp',
                support_sources=['src/StageLifecycle.cpp', 'src/AnmManager.cpp'],
                symbol=symbol, target_address=spec['entry'], size=spec['size'])
    field = dict(offset=spec['offset'], type='DIR32', symbol='__ehhandler$' + symbol,
                 target=spec['handler'], stage_array_eh_handler=True)
    parent = dict(symbol=symbol, size=spec['size'], map_object='support-00-StageLifecycle.ltcg.obj',
                  pdb_module_index=13, pdb_object=stage_object)
    statics = [dict(symbol=field['symbol'], section_number=1, section_offset=address - 0x100000,
                   address=address, flags=['f', 'CIL', 'library:', 'CIL'], object='module')]
    pdb = dict(contributions=[dict(section_number=1, section_offset=0x11000, size=lead + 10,
               module_index=13, object=stage_object, characteristics=0x60000020)],
               modules=[dict(index=13, object=stage_object), dict(index=14, object=vm_object)])
    publics = [dict(symbol=s, address=a, section_number=1, section_offset=a - 0x100000)
               for s, a in [('frame', 0x101800), ('iterator', 0x101820),
                            (eh.DESTRUCTOR_SYMBOL, 0x101840), ('free', 0x1018a0)]]
    aliases = {'___CxxFrameHandler': 'frame', eh.ITERATOR_SYMBOL: 'iterator', '_free': 'free'}
    functions = [dict(symbol=eh.DESTRUCTOR_SYMBOL, address='0x101840', size=34,
                     map_object='support-01-AnmManager.ltcg.obj',
                     pdb_module_index=14, pdb_object=vm_object)]
    native = Image(address, 0x101800, 0x101840, 0x101820, 0x1018a0, arrays, 0x100000, 0x300000)
    target = Image(spec['handler'], eh.TARGET_FRAME_HANDLER, eh.TARGET_DESTRUCTOR,
                   eh.TARGET_ITERATOR, eh.TARGET_FREE, arrays, 0x400000, 0x600000)
    links = dict(normalization_complete=True, decoded_bytes=34,
                 fields=[dict(offset=15, width=4, type='REL32')])
    region_fields = [dict(offset=27 * i + offset, width=4, type=kind)
                     for i in range(len(arrays))
                     for offset, kind in [(1, 'DIR32'), (22, 'REL32')]]
    region_fields += [dict(offset=lead + 1, width=4, type='DIR32'),
                      dict(offset=lead + 6, width=4, type='REL32')]
    region_links = dict(normalization_complete=True, decoded_bytes=lead + 10, fields=region_fields)
    return dict(unit=unit, field=field, parent=parent, statics=statics, pdb=pdb,
                publics=publics, aliases=aliases, functions=functions, links=links,
                native=native, target=target, address=address, region_links=region_links,
                target_start=spec['handler'] - lead)


def bind(f):
    def fields(image, publics, address, size):
        return f['links'] if size == 34 else f['region_links']
    with patch.object(eh, 'map_statics', return_value=f['statics']), \
         patch.object(eh, 'pdb_section_contributions', return_value=f['pdb']), \
         patch.object(eh, 'linked_functions', return_value={'functions': f['functions']}), \
         patch.object(eh, 'linked_code_fields', side_effect=fields):
        return eh.stage_array_eh_target(f['unit'], f['field'], Path('build/synthetic'),
                   f['native'], f['publics'], f['aliases'], f['target'], f['parent'], f['address'])


def word(f, side, address, value):
    f[side].write(address, struct.pack('<I', value & 0xffffffff))


def main():
    checks = 0
    for symbol, spec in eh.PARENTS.items():
        f = fixture(symbol)
        result = bind(f)
        assert result['candidate']['unwind_states'] == len(spec['arrays'])
        assert result['cleanup_proof'] == dict(bytes=34, fields=1, result='exact')
        assert eh.stage_array_eh_selector(f['unit'], {}) is False
        unit = {**f['unit'], 'linkages': [f['field']]}
        assert runpy.run_path('scripts/compare-linked-function.py')['expected_fields'](unit)[0]['stage_array_eh_handler']
        runpy.run_path('scripts/build-match-unit.py')['validate_fields']('synthetic', unit, unit['size'])
        for neighbor in [0, 0x37, 0xffffffff]:
            q = fixture(symbol)
            word(q, 'target', 0x60003c, neighbor)
            bind(q)
        cases = [
            ('false-opt-in', lambda q: q['field'].update(stage_array_eh_handler=False)),
            ('other-parent', lambda q: q['unit'].update(symbol='?Other@@YAXXZ')),
            ('parent-size', lambda q: q['unit'].update(size=spec['size'] - 1)),
            ('partial-compare', lambda q: q['unit'].update(compare_size=spec['size'] - 1)),
            ('field-offset', lambda q: q['field'].update(offset=spec['offset'] + 1)),
            ('target-handler', lambda q: q['field'].update(target=spec['handler'] + 1)),
            ('local', lambda q: q['field'].update(local=True)),
            ('mixed-eh', lambda q: q['field'].update(allocation_eh_handler=True)),
            ('mixed-static', lambda q: q['field'].update(static_source='src/StageLifecycle.cpp')),
            ('duplicate-map', lambda q: q['statics'].append(copy.deepcopy(q['statics'][0]))),
            ('interior-operand', lambda q: q.update(address=q['address'] + 1)),
            ('foreign-parent', lambda q: q['parent'].update(map_object='other.obj')),
            ('foreign-module', lambda q: q['pdb']['contributions'][0].update(module_index=12)),
            ('region-size', lambda q: q['pdb']['contributions'][0].update(size=spec['size'])),
            ('extra-region-field', lambda q: q['region_links']['fields'].append(dict(offset=2, width=4, type='DIR32'))),
            ('partial-region-decode', lambda q: q['region_links'].update(decoded_bytes=1)),
            ('mutable-info', lambda q: setattr(q['target'].sections[1], 'characteristics', 0xc0000040)),
            ('wrong-magic', lambda q: word(q, 'target', 0x600020, 0x19930521)),
            ('wrong-state-count', lambda q: word(q, 'native', 0x300024, 3)),
            ('try-map', lambda q: word(q, 'target', 0x60002c, 1)),
            ('wrong-action', lambda q: word(q, 'native', 0x300004, q['address'])),
            ('wrong-predecessor', lambda q: word(q, 'target', 0x600000, 0)),
            ('array-count', lambda q: q['target'].write(q['target_start'] + 6, b'\x09')),
            ('array-stride', lambda q: word(q, 'target', q['target_start'] + 8, 0x3b0)),
            ('saved-this', lambda q: q['native'].write(0x11100e, b'\x08')),
            ('member-offset', lambda q: word(q, 'target', q['target_start'] + 16, 0x184)),
            ('iterator-entry', lambda q: word(q, 'native', 0x111016, 0x101821 - 0x11101a)),
            ('anchored-destructor', lambda q: q['aliases'].update({eh.DESTRUCTOR_SYMBOL: 'fake'})),
            ('short-destructor', lambda q: q['functions'][0].update(size=33)),
            ('foreign-destructor', lambda q: q['functions'][0].update(pdb_module_index=15)),
            ('destructor-field', lambda q: q['links']['fields'].append(dict(offset=1, width=4, type='DIR32'))),
            ('destructor-byte', lambda q: q['native'].write(0x101840, b'\x91')),
            ('destructor-free', lambda q: word(q, 'native', 0x10184f, 0x1018a1 - 0x101853)),
        ]
        if len(spec['arrays']) == 2:
            cases += [('second-predecessor', lambda q: word(q, 'target', 0x600008, 0xffffffff)),
                      ('second-member', lambda q: word(q, 'target', q['target_start'] + 43, 0x1f0c))]
        for label, change in cases:
            q = fixture(symbol)
            change(q)
            try:
                bind(q)
            except LinkedImageError:
                checks += 1
                continue
            raise AssertionError('accepted ' + label)
    print('Stage array EH guards passed: both protocols, metadata tails and', checks, 'focused rejections')


if __name__ == '__main__':
    main()
