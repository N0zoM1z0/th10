#!/usr/bin/env python3
"""Synthetic address/metadata controls for the named two-sentinel EH protocol."""
import copy
import runpy
import struct
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch
from linked_image import LinkedImageError
import linked_chain_member_eh as eh


class Image:
    image_base = 0
    def __init__(self, handler, frame, destructor):
        self.sections = [SimpleNamespace(rva=0x1000, virtual_size=0x1000, characteristics=0x60000020),
                         SimpleNamespace(rva=0x3000, virtual_size=0x2c, characteristics=0x40000040)]
        first, second = handler-19, handler-11
        actions = b'\x8b\x45\x04\xe9'+struct.pack('<i', destructor-first-8)
        actions += b'\x8b\x45\x04\x83\xc0\x24\xe9'+struct.pack('<i', destructor-second-11)
        dispatch = b'\xb8'+struct.pack('<I', 0x3010)+b'\xe9'+struct.pack('<i', frame-handler-10)
        member = b'\x33\xc9\x89\x48\x08\x89\x48\x0c\x89\x48\x10\xc3'
        self.memory = {first:bytearray(actions+dispatch), destructor:bytearray(member),
                       0x3000:bytearray(struct.pack('<iIiI7I', -1, first, 0, second,
                                      0x19930520, 2, 0x3000, 0, 0, 0, 0))}
    def section(self, number): return self.sections[number-1]
    def read_address(self, address, size):
        for start, data in self.memory.items():
            if start <= address and address+size <= start+len(data):
                return bytes(data[address-start:address-start+size])
        raise LinkedImageError('synthetic unmapped bytes')
    def write(self, address, data):
        for start, dest in self.memory.items():
            if start <= address and address+len(data) <= start+len(dest):
                dest[address-start:address-start+len(data)] = data
                return
        raise AssertionError('mutation leaves fixture')


def fixture():
    obj = 'Z:\\synthetic\\source.ltcg.obj'
    unit = dict(artifact_kind='linked-pe', source='src/Chain.cpp', symbol=eh.PARENT_SYMBOL)
    field = dict(type='DIR32', offset=9, symbol='__ehhandler$'+eh.PARENT_SYMBOL,
                 target=0x1213, chain_member_eh_handler=True)
    parent = dict(symbol=eh.PARENT_SYMBOL, pdb_module_index=13, pdb_object=obj,
                  map_object='source.ltcg.obj')
    statics = [dict(symbol=field['symbol'], section_number=1, section_offset=0x113,
                   address=0x1113, flags=['f','CIL','library:','CIL'], object='module')]
    pdb = dict(contributions=[dict(section_number=1, section_offset=0x100, size=29,
               module_index=13, object=obj, characteristics=0x60000020)],
               modules=[dict(index=13, object=obj)])
    publics = [dict(symbol='frame-anchor', address=0x1800, section_number=1, section_offset=0x800),
               dict(symbol=eh.DESTRUCTOR_SYMBOL, address=0x1820, section_number=1, section_offset=0x820)]
    functions = [dict(symbol=eh.DESTRUCTOR_SYMBOL, address='0x1820', size=12,
                     map_object='source.ltcg.obj', pdb_module_index=13, pdb_object=obj)]
    return dict(unit=unit, field=field, parent=parent, statics=statics, pdb=pdb,
                publics=publics, aliases={'___CxxFrameHandler':'frame-anchor'},
                functions=functions, links=dict(normalization_complete=True, fields=[]),
                native=Image(0x1113,0x1800,0x1820), target=Image(0x1213,0x1900,0x1810), address=0x1113)


def bind(f):
    with patch.object(eh,'TARGET_FRAME_HANDLER',0x1900), patch.object(eh,'TARGET_DESTRUCTOR',0x1810), \
         patch.object(eh,'map_statics',return_value=f['statics']), \
         patch.object(eh,'pdb_section_contributions',return_value=f['pdb']), \
         patch.object(eh,'linked_functions',return_value={'functions':f['functions']}), \
         patch.object(eh,'linked_code_fields',return_value=f['links']):
        return eh.chain_member_eh_target(f['unit'],f['field'],Path('build/synthetic'),
                   f['native'],f['publics'],f['aliases'],f['target'],f['parent'],f['address'])


def word(f, side, address, value): f[side].write(address, struct.pack('<I',value))


def main():
    f=fixture();result=bind(f)
    assert result['candidate']['unwind_states']==2 and result['cleanup_proof']['bytes']==12
    assert eh.chain_member_eh_selector(f['unit'],{}) is False
    unit={**f['unit'], 'linkages':[f['field']]}
    parser=runpy.run_path('scripts/compare-linked-function.py')['expected_fields']
    assert parser(unit)[0]['chain_member_eh_handler'] is True
    runpy.run_path('scripts/build-match-unit.py')['validate_fields']('synthetic',unit,103)
    for neighbor in [0, 0x37, 0xffffffff]:
        q=fixture()
        for side in ['native','target']:
            q[side].sections[1].virtual_size=0x30
            q[side].memory[0x3000].extend(struct.pack('<I',neighbor))
        bind(q)
    cases=[
        ('missing',lambda q:q['field'].pop('chain_member_eh_handler')),
        ('false',lambda q:q['field'].update(chain_member_eh_handler=False)),
        ('other-parent',lambda q:q['unit'].update(symbol='?Other@@YAXXZ')),
        ('coff',lambda q:q['unit'].update(artifact_kind='coff')),
        ('rel32',lambda q:q['field'].update(type='REL32')),
        ('offset',lambda q:q['field'].update(offset=3)),
        ('local',lambda q:q['field'].update(local=True)),
        ('addend',lambda q:q['field'].update(addend=1)),
        ('mixed-worker',lambda q:q['field'].update(worker_member_eh_handler=True)),
        ('mixed-static',lambda q:q['field'].update(static_source='src/a.cpp')),
        ('duplicate-handler',lambda q:q['statics'].append(copy.deepcopy(q['statics'][0]))),
        ('aliased-entry',lambda q:q['statics'].append({**q['statics'][0],'symbol':'alias'})),
        ('interior-entry',lambda q:q.update(address=0x1114)),
        ('foreign-module',lambda q:q['pdb']['contributions'][0].update(module_index=14)),
        ('region-tail',lambda q:q['pdb']['contributions'][0].update(size=30)),
        ('region-data',lambda q:q['pdb']['contributions'][0].update(characteristics=0x40000040)),
        ('mutable-code',lambda q:setattr(q['native'].sections[0],'characteristics',0xe0000020)),
        ('mutable-info',lambda q:setattr(q['target'].sections[1],'characteristics',0xc0000040)),
        ('one-state',lambda q:word(q,'native',0x3014,1)),
        ('wrong-magic',lambda q:word(q,'target',0x3010,0x19930521)),
        ('try-map',lambda q:word(q,'native',0x301c,1)),
        ('wrong-predecessor',lambda q:word(q,'target',0x3008,0xffffffff)),
        ('wrong-action',lambda q:word(q,'native',0x300c,0x1100)),
        ('wrong-saved-this',lambda q:q['native'].write(0x1102,b'\x08')),
        ('wrong-member',lambda q:q['target'].write(0x120d,b'\x28')),
        ('wrong-tail',lambda q:word(q,'native',0x1104,0x1821-0x1108)),
        ('frame-tail',lambda q:word(q,'target',0x1219,0x1901-0x121d)),
        ('anchored-destructor',lambda q:q['aliases'].update({eh.DESTRUCTOR_SYMBOL:'fake'})),
        ('foreign-destructor',lambda q:q['functions'][0].update(pdb_module_index=14)),
        ('short-destructor',lambda q:q['functions'][0].update(size=11)),
        ('destructor-interior',lambda q:q['publics'][1].update(address=0x1821)),
        ('destructor-field',lambda q:q['links']['fields'].append(dict(offset=1,type='DIR32',width=4))),
        ('destructor-byte',lambda q:q['native'].write(0x1824,b'\x0c')),
    ]
    for label,change in cases:
        q=fixture();change(q)
        try: bind(q)
        except LinkedImageError: continue
        raise AssertionError('accepted '+label)
    print('chain-member EH guards passed: positive/tail controls and',len(cases),'targeted rejections')


if __name__=='__main__': main()
