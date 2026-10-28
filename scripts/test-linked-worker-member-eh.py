#!/usr/bin/env python3
"""Synthetic i386 metadata/ownership controls for one worker-member EH shape.

All code/data addresses are synthetic. Native cold proofs separately establish
TH10 integration; these fixtures grant no generated-code or runtime credit.
"""
import copy
import struct
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch
from linked_image import LinkedImageError
import linked_worker_member_eh as eh


class Image:
    image_base = 0
    def __init__(self, handler, frame, destructor, stop, vtable):
        self.sections = [SimpleNamespace(rva=0x1000, virtual_size=0x1000, characteristics=0x60000020),
                         SimpleNamespace(rva=0x3000, virtual_size=0x24, characteristics=0x40000040)]
        action = handler - 14
        cleanup = b'\x8b\x4d\x04\x81\xc1' + struct.pack('<I', 0xd0) + b'\xe9' + struct.pack('<i', destructor-handler)
        dispatch = b'\xb8' + struct.pack('<I', 0x3008) + b'\xe9' + struct.pack('<i', frame-handler-10)
        # Generic virtual destructor: restore vptr, call Stop, restore register.
        dtor = b'\x56\x8b\xf1\xc7\x01' + struct.pack('<I', vtable) + b'\xe8' + struct.pack('<i', stop-destructor-14) + b'\x5e\xc3'
        self.memory = {action:bytearray(cleanup+dispatch), destructor:bytearray(dtor),
                       0x3000:bytearray(struct.pack('<iI7I', -1, action, 0x19930520, 1, 0x3000, 0, 0, 0, 0))}
    def section(self, number): return self.sections[number-1]
    def read_address(self, address, size):
        for start, data in self.memory.items():
            if start <= address and address+size <= start+len(data):
                return bytes(data[address-start:address-start+size])
        raise LinkedImageError('synthetic unmapped bytes')
    def write(self, address, data):
        for start, target in self.memory.items():
            if start <= address and address+len(data) <= start+len(target):
                target[address-start:address-start+len(data)] = data
                return
        raise AssertionError('mutation leaves fixture')


def fixture():
    obj = 'Z:\\synthetic\\source.ltcg.obj'
    unit = dict(artifact_kind='linked-pe', source='src/Ending.cpp',
                support_sources=['src/ScriptViewerWorker.cpp'], symbol=eh.PARENT_SYMBOL)
    field = dict(type='DIR32', offset=3, symbol='__ehhandler$'+eh.PARENT_SYMBOL,
                 target=0x120e, worker_member_eh_handler=True)
    parent = dict(symbol=eh.PARENT_SYMBOL, pdb_module_index=13, pdb_object=obj)
    statics = [dict(symbol=field['symbol'], section_number=1, section_offset=0x10e,
                   address=0x110e, flags=['f','CIL','library:','CIL'], object='module')]
    pdb = dict(contributions=[dict(section_number=1, section_offset=0x100, size=24,
               module_index=13, object=obj, characteristics=0x60000020)], modules=[dict(index=13, object=obj)])
    publics = [dict(symbol='frame-anchor', address=0x1800, section_number=1, section_offset=0x800),
               dict(symbol=eh.DESTRUCTOR_SYMBOL, address=0x1820, section_number=1, section_offset=0x820),
               dict(symbol=eh.STOP_SYMBOL, address=0x1860, section_number=1, section_offset=0x860),
               dict(symbol=eh.VTABLE_SYMBOL, address=0x3600, section_number=2, section_offset=0x600)]
    functions = [dict(symbol=eh.DESTRUCTOR_SYMBOL, address='0x1820', size=16, map_object='support-00-ScriptViewerWorker.ltcg.obj'),
                 dict(symbol=eh.STOP_SYMBOL, address='0x1860', size=101, map_object='support-00-ScriptViewerWorker.ltcg.obj')]
    links = dict(normalization_complete=True, fields=[dict(offset=5,width=4,type='DIR32'),dict(offset=10,width=4,type='REL32')])
    return dict(unit=unit,field=field,parent=parent,statics=statics,pdb=pdb,publics=publics,
                aliases={'___CxxFrameHandler':'frame-anchor'}, functions=functions, links=links,
                native=Image(0x110e,0x1800,0x1820,0x1860,0x3600),
                target=Image(0x120e,0x1900,0x1810,0x1850,0x3500), address=0x110e)


def bind(f):
    with patch.object(eh,'TARGET_FRAME_HANDLER',0x1900), patch.object(eh,'TARGET_DESTRUCTOR',0x1810), \
         patch.object(eh,'TARGET_STOP',0x1850), patch.object(eh,'TARGET_VTABLE',0x3500), \
         patch.object(eh,'map_statics',return_value=f['statics']), \
         patch.object(eh,'pdb_section_contributions',return_value=f['pdb']), \
         patch.object(eh,'linked_functions',return_value={'functions':f['functions']}), \
         patch.object(eh,'linked_code_fields',return_value=f['links']):
        return eh.worker_member_eh_target(f['unit'],f['field'],Path('build/synthetic'),
                  f['native'],f['publics'],f['aliases'],f['target'],f['parent'],f['address'])


def word(f,side,address,value): f[side].write(address,struct.pack('<I',value))


def main():
    result = bind(fixture())
    assert result['candidate']['region_bytes'] == 24 and result['cleanup_proof']['bytes'] == 16
    assert eh.worker_member_eh_selector(fixture()['unit'], {'type':'DIR32'}) is False
    for neighbor in [0,0x17,0xffffffff]:
        f=fixture()
        for side in ['native','target']:
            f[side].sections[1].virtual_size=0x28
            f[side].memory[0x3000].extend(struct.pack('<I',neighbor))
        bind(f)
    controls=[
        ('missing', lambda f:f['field'].pop('worker_member_eh_handler'), 'missing selector'),
        ('false', lambda f:f['field'].update(worker_member_eh_handler=False), 'explicit true'),
        ('wrong-parent', lambda f:f['unit'].update(symbol='?Other@@YAXXZ'), 'unsupported parent'),
        ('COFF', lambda f:f['unit'].update(artifact_kind='coff'), 'linked-pe'),
        ('REL32', lambda f:f['field'].update(type='REL32'), 'separate DIR32'),
        ('local', lambda f:f['field'].update(local=True), 'separate DIR32'),
        ('addend', lambda f:f['field'].update(addend=1), 'separate DIR32'),
        ('width', lambda f:f['field'].update(width=2), 'separate DIR32'),
        ('offset', lambda f:f['field'].update(offset=4), 'separate DIR32'),
        ('mixed-allocation', lambda f:f['field'].update(allocation_eh_handler=True), 'separate DIR32'),
        ('mixed-file', lambda f:f['field'].update(stack_file_eh_handler=True), 'separate DIR32'),
        ('mixed-static', lambda f:f['field'].update(static_source='src/a.cpp'), 'separate DIR32'),
        ('map-alias', lambda f:f['statics'].append({**f['statics'][0],'symbol':'alias'}), 'start is aliased'),
        ('map-duplicate', lambda f:f['statics'].append(copy.deepcopy(f['statics'][0])), 'named MAP'),
        ('interior-handler', lambda f:f.update(address=0x110f), 'operand does not'),
        ('different-module', lambda f:f['parent'].update(pdb_module_index=14), 'parent PDB object'),
        ('different-object', lambda f:f['pdb']['contributions'][0].update(object='wrong.obj'), 'parent PDB object'),
        ('long-region', lambda f:f['pdb']['contributions'][0].update(size=25), 'complete 24-byte'),
        ('short-region', lambda f:f['pdb']['contributions'][0].update(size=23), 'unique PDB'),
        ('data-region', lambda f:f['pdb']['contributions'][0].update(characteristics=0x40000040), 'not code'),
        ('mutable-code', lambda f:setattr(f['native'].sections[0],'characteristics',0xe0000020), 'immutable'),
        ('mutable-metadata', lambda f:setattr(f['target'].sections[1],'characteristics',0xc0000040), 'immutable'),
        ('two-states', lambda f:word(f,'native',0x300c,2), 'FuncInfo'),
        ('wrong-state', lambda f:word(f,'target',0x3000,0), 'state/action'),
        ('other-action', lambda f:word(f,'native',0x3004,0x1101), 'state/action'),
        ('short-info', lambda f:setattr(f['native'].sections[1],'virtual_size',0x23), 'range is outside'),
        ('wrong-saved-this', lambda f:f['target'].write(0x1202,b'\x08'), 'saved-this/member'),
        ('wrong-member', lambda f:word(f,'native',0x1105,0xd4), 'saved-this/member'),
        ('CALL-cleanup', lambda f:f['native'].write(0x1109,b'\xe8'), 'instruction shape'),
        ('wrong-tail', lambda f:word(f,'target',0x120a,0x1811-0x120e), 'saved-this/member'),
        ('short-dtor', lambda f:f['functions'][0].update(size=15), 'qualified16-byte'),
        ('wrong-dtor-object', lambda f:f['functions'][0].update(map_object='other.obj'), 'qualified16-byte'),
        ('short-Stop', lambda f:f['functions'][1].update(size=100), 'qualified101-byte'),
        ('anchor-dtor', lambda f:f['aliases'].update({eh.DESTRUCTOR_SYMBOL:'fake'}), 'destructor cannot'),
        ('anchor-Stop', lambda f:f['aliases'].update({eh.STOP_SYMBOL:'fake'}), 'runtime symbol'),
        ('anchor-vtable', lambda f:f['aliases'].update({eh.VTABLE_SYMBOL:'fake'}), 'cannot be diagnostic'),
        ('ordinary-dtor-byte', lambda f:f['native'].write(0x182e,b'\x5f'), 'complete destructor bytes'),
        ('extra-dtor-field', lambda f:f['links']['fields'].append(dict(offset=0,width=4,type='DIR32')), 'field inventory'),
        ('no-worker-source', lambda f:f['unit'].update(support_sources=[]), 'one ScriptViewerWorker'),
    ]
    for name,mutate,message in controls:
        f=fixture();mutate(f)
        try:bind(f)
        except LinkedImageError as error: assert message in str(error),(name,str(error))
        else:raise AssertionError('accepted '+name)
    print(f'Worker-member EH:4 synthetic positives and{len(controls)} strict rejection controls pass')


if __name__=='__main__': main()
