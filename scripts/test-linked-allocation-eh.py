#!/usr/bin/env python3
"""Target-independent synthetic rejection tests for the narrow allocation-EH lane.

These model generic i386 compiler metadata and code, not a TH10 byte fixture or
runtime. Native/canonical cold comparisons provide the independent integration
check. No compiler execution, private input or generated-code credit is needed.
"""
import copy, runpy, struct
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch
from linked_image import LinkedImageError
import linked_allocation_eh as eh


class Image:
    image_base=0
    def __init__(self,handler,frame,delete):
        self.sections=[SimpleNamespace(number=1,rva=0x1000,virtual_size=0x1000,characteristics=0x60000020),
                       SimpleNamespace(number=2,rva=0x3000,virtual_size=0x24,characteristics=0x40000040)]
        action=handler-11
        cleanup=b'\x8b\x45\xf0\x50\xe8'+struct.pack('<i',delete-(action+9))+b'\x59\xc3'
        dispatch=b'\xb8'+struct.pack('<I',0x3008)+b'\xe9'+struct.pack('<i',frame-(handler+10))
        self.memory={action:bytearray(cleanup+dispatch),0x3000:bytearray(struct.pack('<iI7I',-1,action,0x19930520,1,0x3000,0,0,0,0))}
    def section(self,number):return self.sections[number-1]
    def read_address(self,address,size):
        for start,raw in self.memory.items():
            if start<=address and address+size<=start+len(raw):return bytes(raw[address-start:address-start+size])
        raise LinkedImageError('synthetic memory is unmapped')
    def write(self,address,data):
        for start,raw in self.memory.items():
            if start<=address and address+len(data)<=start+len(raw):raw[address-start:address-start+len(data)]=data;return
        raise AssertionError('test mutation leaves memory')


def fixture():
    symbol='?Open@ReplayFixture@@SGPAXPBD@Z'
    unit={'artifact_kind':'linked-pe','symbol':symbol}
    field={'type':'DIR32','symbol':'__ehhandler$'+symbol,'target':0x120b,'allocation_eh_handler':True}
    obj='Z:\\fake\\support-04-ReplayFixture.ltcg.obj'
    parent={'pdb_module_index':13,'pdb_object':obj,'symbol':symbol}
    statics=[{'symbol':field['symbol'],'section_number':1,'section_offset':0x10b,'address':0x110b,
              'flags':['f','CIL','library:','CIL'],'object':'module'}]
    pdb={'contributions':[{'section_number':1,'section_offset':0x100,'size':21,'module_index':13,
                          'object':obj,'characteristics':0x60000020}],
         'modules':[{'index':13,'object':obj}]}
    publics=[{'symbol':'frame-anchor','address':0x1800,'section_number':1,'section_offset':0x800},{'symbol':'delete-anchor','address':0x1810,'section_number':1,'section_offset':0x810}]
    aliases={'___CxxFrameHandler':'frame-anchor','??3@YAXPAX@Z':'delete-anchor'}
    return dict(unit=unit,field=field,parent=parent,statics=statics,pdb=pdb,publics=publics,aliases=aliases,
                native=Image(0x110b,0x1800,0x1810),target=Image(0x120b,eh.TARGET_FRAME_HANDLER,eh.TARGET_DELETE),address=0x110b)


def bind(f):
    with patch.object(eh,'map_statics',return_value=f['statics']),patch.object(eh,'pdb_section_contributions',return_value=f['pdb']):
        return eh.allocation_eh_target(f['unit'],f['field'],Path('build/synthetic'),f['native'],
            f['publics'],f['aliases'],f['target'],f['parent'],f['address'])


def set32(f,side,address,value):f[side].write(address,struct.pack('<I',value))


def main():
    f=fixture();result=bind(f);assert result['candidate']['region_bytes']==21 and result['target']['unwind_states']==1
    assert eh.allocation_eh_selector(f['unit'],{'type':'DIR32','symbol':'ordinary'}) is False
    for neighbor in [0,0x17,0xffffffff]:
        f=fixture()
        for side in ['native','target']:
            f[side].sections[1].virtual_size=0x28
            f[side].memory[0x3000].extend(struct.pack('<I',neighbor))
        bind(f)

    # The explicitly declared slot is an equality constraint, not a mask.
    f=fixture();f['field']['allocation_eh_storage_offset']=-0x48
    expected_fields=runpy.run_path(str(Path(__file__).with_name('compare-linked-function.py')))['expected_fields']
    f['field']=expected_fields({**f['unit'],'linkages':[{**f['field'],'offset':3}]})[0]
    assert f['field']['allocation_eh_storage_offset']==-0x48
    for side in ['native','target']:
        f[side].write((0x1100 if side=='native' else 0x1200)+2,b'\xb8')
    result=bind(f)
    assert result['candidate']['saved_storage_offset']==result['target']['saved_storage_offset']==-0x48
    for side in ['native','target']:
        wrong=copy.deepcopy(f)
        wrong[side].write((0x1100 if side=='native' else 0x1200)+2,b'\xb4')
        try:bind(wrong)
        except LinkedImageError as error:assert 'saved-storage/scalar-delete' in str(error)
        else:raise AssertionError('accepted a different declared storage slot in '+side)

    controls=[
        ('no-opt-in',lambda f:f['field'].pop('allocation_eh_handler'),'missing selector'),
        ('false-opt-in',lambda f:f['field'].update(allocation_eh_handler=False),'explicit true'),
        ('offset-without-selector',lambda f:(f['field'].pop('allocation_eh_handler'),f['field'].update(allocation_eh_storage_offset=-0x48)),'storage offset requires'),
        ('positive-slot',lambda f:f['field'].update(allocation_eh_storage_offset=4),'aligned negative disp8'),
        ('unaligned-slot',lambda f:f['field'].update(allocation_eh_storage_offset=-0x47),'aligned negative disp8'),
        ('wide-slot',lambda f:f['field'].update(allocation_eh_storage_offset=-0x84),'aligned negative disp8'),
        ('boolean-slot',lambda f:f['field'].update(allocation_eh_storage_offset=False),'aligned negative disp8'),
        ('wrong-declared-slot',lambda f:f['field'].update(allocation_eh_storage_offset=-0x48),'saved-storage/scalar-delete'),
        ('wrong-artifact',lambda f:f['unit'].update(artifact_kind='coff'),'linked-pe'),
        ('wrong-field-kind',lambda f:f['field'].update(type='REL32'),'separate DIR32'),
        ('local-field',lambda f:f['field'].update(local=True),'separate DIR32'),
        ('added-offset',lambda f:f['field'].update(addend=1),'separate DIR32'),
        ('mixed-selector',lambda f:f['field'].update(static_source='src/other.cpp'),'separate DIR32'),
        ('mixed-stack-EH',lambda f:f['field'].update(stack_file_eh_handler=True),'separate DIR32'),
        ('wrong-parent',lambda f:f['unit'].update(symbol='?Other@@YAXXZ'),'parent symbol'),
        ('wrong-pdb-parent',lambda f:f['parent'].update(symbol='?Other@@YAXXZ'),'PDB parent symbol'),
        ('absent-map',lambda f:f['statics'].clear(),'named MAP'),
        ('duplicate-map',lambda f:f['statics'].append(copy.deepcopy(f['statics'][0])),'named MAP'),
        ('aliased-map',lambda f:f['statics'].append({**f['statics'][0],'symbol':'other'}),'start is aliased'),
        ('operand-interior',lambda f:f.update(address=0x110c),'operand does not'),
        ('wrong-map-producer',lambda f:f['statics'][0].update(object='other.obj'),'MAP generated-handler'),
        ('wrong-map-location',lambda f:f['statics'][0].update(section_offset=0x10c),'MAP location'),
        ('absent-pdb',lambda f:f['pdb']['contributions'].clear(),'unique PDB'),
        ('overlapping-pdb',lambda f:f['pdb']['contributions'].append(copy.deepcopy(f['pdb']['contributions'][0])),'unique PDB'),
        ('truncated-pdb',lambda f:f['pdb']['contributions'][0].update(size=20),'unique PDB'),
        ('oversized-pdb',lambda f:f['pdb']['contributions'][0].update(size=22),'complete 21-byte'),
        ('data-pdb',lambda f:f['pdb']['contributions'][0].update(characteristics=0x40000040),'PDB generated region is not code'),
        ('wrong-parent-module',lambda f:f['parent'].update(pdb_module_index=14),'parent PDB object'),
        ('wrong-contribution-object',lambda f:f['pdb']['contributions'][0].update(object='wrong.obj'),'parent PDB object'),
        ('wrong-module-object',lambda f:f['pdb']['modules'][0].update(object='wrong.obj'),'parent PDB module'),
        ('mutable-code',lambda f:setattr(f['native'].sections[0],'characteristics',0xe0000020),'immutable readable'),
        ('mutable-metadata',lambda f:setattr(f['target'].sections[1],'characteristics',0xc0000040),'immutable readable'),
        ('missing-runtime',lambda f:f['publics'].clear(),'runtime symbol'),
        ('ambiguous-runtime',lambda f:f['publics'].append({'symbol':'frame-anchor','address':0x1900,'section_number':1,'section_offset':0x900}),'runtime symbol'),
        ('wrong-native-frame',lambda f:set32(f,'native',0x1111,0x1900-(0x110b+10)),'handler/frame-handler'),
        ('wrong-native-magic',lambda f:set32(f,'native',0x3008,0x19930521),'FuncInfo'),
        ('wrong-target-states',lambda f:set32(f,'target',0x300c,2),'FuncInfo'),
        ('target-try-map',lambda f:set32(f,'target',0x3014,1),'FuncInfo'),
        ('wrong-next-state',lambda f:set32(f,'native',0x3000,0),'unwind state/action'),
        ('unrelated-action',lambda f:set32(f,'native',0x3004,0x1101),'unwind state/action'),
        ('wrong-storage-slot',lambda f:f['native'].write(0x1102,b'\xec'),'saved-storage/scalar-delete'),
        ('wrong-delete',lambda f:set32(f,'target',0x1205,eh.TARGET_DELETE+1-(0x1200+9)),'saved-storage/scalar-delete'),
        ('short-FuncInfo',lambda f:setattr(f['native'].sections[1],'virtual_size',0x23),'range is outside'),
    ]
    for name,mutate,message in controls:
        f=fixture();mutate(f)
        try:bind(f)
        except LinkedImageError as error:
            assert message in str(error),(name,str(error))
        else:raise AssertionError('accepted '+name)
    print(f'Allocation EH synthetic5 positives (section end/neighbors/declared slot) and {len(controls)+2} rejection controls pass')

if __name__=='__main__':main()
