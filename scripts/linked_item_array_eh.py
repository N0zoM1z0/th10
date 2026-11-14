"""Target-bound VC7.1 EH handlers for one-state member-array destructors.

Covers the ItemManager and ReplayManager destructors. The reviewed one-state
vector-destruction action, original iterator/dtor identities, immutable
metadata and complete generated PDB contributor (action plus 10-byte dispatch)
must agree. This binds the address, not runtime EH semantics or extra codegen.
"""
from pathlib import Path
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_EAX, X86_REG_EBP
from linked_image import (LinkedImageError, map_statics, pdb_section_contributions)

PARENTS = {
    '??1ItemManagerView@@QAE@XZ': dict(
        target_parent=0x0041ADF0, size=216, offset=9, handler=0x0046541C,
        destructor='??1ItemRecordView@@QAE@XZ', target_destructor=0x0041AD60,
        count=0x896, stride=0x3F0, member=0x14),
    '??1ReplayManager@@QAE@XZ': dict(
        target_parent=0x004294A0, size=367, offset=3, handler=0x00465268,
        destructor='??1ReplayStageState@@QAE@XZ', target_destructor=0x0042AC60,
        count=8, stride=0x24, member=0xA0),
}
PARENT = '??1ItemManagerView@@QAE@XZ'
HANDLER = '__ehhandler$' + PARENT
TARGET_PARENT = PARENTS[PARENT]['target_parent']
TARGET_HANDLER = PARENTS[PARENT]['handler']
TARGET_FRAME = 0x0046276D
TARGET_DESTRUCTOR = PARENTS[PARENT]['target_destructor']
TARGET_ITERATOR = 0x004525FF
DESTRUCTOR = PARENTS[PARENT]['destructor']
ITERATOR = '??_M@YGXPAXIHP6EX0@Z@Z'


def require(ok, why):
    if not ok:
        raise LinkedImageError('item-array EH: ' + why)


def item_array_eh_selector(unit, field):
    if 'item_array_eh_handler' not in field:
        return False
    require(field['item_array_eh_handler'] is True, 'explicit true opt-in required')
    spec = PARENTS.get(unit.get('symbol'))
    require(spec is not None, 'unsupported parent')
    require(unit.get('artifact_kind') == 'linked-pe'
            and unit.get('target_address') == spec['target_parent']
            and unit.get('size') == spec['size']
            and unit.get('compare_size',spec['size']) == spec['size'], 'full reviewed parent required')
    require(field.get('type') == 'DIR32' and field.get('offset') == spec['offset']
            and field.get('target') == spec['handler']
            and field.get('symbol') == '__ehhandler$' + unit['symbol']
            and int(field.get('width',4)) == 4
            and not field.get('local',False) and field.get('addend',0) == 0,
            'complete independent handler DIR32 required')
    require(not any(k.endswith('_eh_handler') and k != 'item_array_eh_handler' for k in field)
            and not any(k.startswith('static_') for k in field),
            'cannot mix handler identity selectors')
    return True


def section_range(image, address, size, code):
    spans = [s for s in image.sections if image.image_base+s.rva <= address
             and address+size <= image.image_base+s.rva+s.virtual_size]
    require(len(spans) == 1, 'whole code/metadata span must occupy one section')
    flags = spans[0].characteristics
    require(bool(flags & 0x40000000) and not bool(flags & 0x80000000)
            and bool(flags & 0x20000000) == code,
            'immutable readable code/metadata section identity differs')


def address_for(publics, aliases, symbol):
    value = aliases.get(symbol,symbol)
    rows = {int(x['address']) for x in publics if x['symbol'] == value}
    require(len(rows) == 1, 'callee/runtime symbol absent or ambiguous: '+symbol)
    return next(iter(rows))


def imm(inst, value):
    return len(inst.operands) == 1 and inst.operands[0].type == X86_OP_IMM \
           and inst.operands[0].imm == value


def protocol(image, handler, frame, destructor, iterator, spec):
    decode=Cs(CS_ARCH_X86,CS_MODE_32);decode.detail=True
    dispatch=list(decode.disasm(image.read_address(handler,10),handler))
    require(len(dispatch)==2 and [x.size for x in dispatch]==[5,5]
            and [x.mnemonic for x in dispatch]==['mov','jmp']
            and len(dispatch[0].operands)==2
            and dispatch[0].operands[0].type==X86_OP_REG
            and dispatch[0].operands[0].reg==X86_REG_EAX
            and dispatch[0].operands[1].type==X86_OP_IMM
            and imm(dispatch[1],frame), '10-byte CxxFrameHandler dispatch differs')
    info=dispatch[0].operands[1].imm
    section_range(image,info,28,False)
    header=struct.unpack('<7I',image.read_address(info,28))
    require((header[0],header[1],*header[3:]) == (0x19930520,1,0,0,0,0),
            'one-state 28-byte FuncInfo identity differs')
    unwind=header[2]
    section_range(image,unwind,8,False)
    predecessor,action=struct.unpack('<iI',image.read_address(unwind,8))
    length=handler-action
    require(predecessor==-1 and 0 < length <= 32,
            'unwind edge does not name an action directly before the dispatch')
    section_range(image,action,length+10,True)
    steps=list(decode.disasm(image.read_address(action,length),action))
    require(len(steps)==8 and sum(x.size for x in steps)==length
            and [x.mnemonic for x in steps]==['push','push','push','mov','add','push','call','ret'],
            'complete vector cleanup instruction shape differs')
    dtor,count,stride,load,member,push,call,ret=steps
    require(imm(dtor,destructor) and imm(count,spec['count']) and imm(stride,spec['stride']),
            'actual destructor/element-count/stride differs')
    require(len(load.operands)==2 and load.operands[0].type==X86_OP_REG
            and load.operands[0].reg==X86_REG_EAX
            and load.operands[1].type==X86_OP_MEM
            and load.operands[1].mem.base==X86_REG_EBP
            and load.operands[1].mem.index==0
            and load.operands[1].mem.disp==4,
            'saved-this load differs')
    require(len(member.operands)==2 and member.operands[0].type==X86_OP_REG
            and member.operands[0].reg==X86_REG_EAX
            and member.operands[1].type==X86_OP_IMM
            and member.operands[1].imm==spec['member']
            and len(push.operands)==1 and push.operands[0].type==X86_OP_REG
            and push.operands[0].reg==X86_REG_EAX
            and imm(call,iterator) and len(ret.operands)==0,
            'array base/iterator/RET differs')
    return {'handler':hex(handler),'action':hex(action),'func_info':hex(info),
            'unwind_map':hex(unwind),'action_bytes':length,'handler_bytes':10,
            'func_info_bytes':28,'unwind_states':1,'saved_this':4,
            'member_offset':spec['member'],'element_count':spec['count'],
            'element_stride':spec['stride'],
            'destructor':hex(destructor),'iterator':hex(iterator)}


def item_array_eh_target(unit, field, directory, image, publics, aliases,
                           target, parent, observed_address):
    require(item_array_eh_selector(unit,field), 'unsupported handler binding')
    spec=PARENTS[unit['symbol']]
    handler_symbol='__ehhandler$'+unit['symbol']
    require(parent.get('symbol')==unit['symbol'], 'parent PDB symbol differs')
    rows=[r for r in map_statics(directory/'source.map') if r['symbol']==handler_symbol]
    require(len(rows)==1,'parent handler MAP static must be unique')
    row=rows[0];address=int(row['address'])
    require(address==observed_address and row['flags']==['f','CIL','library:','CIL']
            and row['object']=='module', 'static handler address/producer differs')
    require(sum(x['section_number']==row['section_number'] and
                x['section_offset']==row['section_offset']
                for x in map_statics(directory/'source.map')+publics)==1,
            'handler static is aliased')
    section=image.section(int(row['section_number']))
    require(image.image_base+section.rva+int(row['section_offset'])==address,
            'static section-offset binding differs')
    destructor=address_for(publics,aliases,spec['destructor'])
    iterator=address_for(publics,aliases,ITERATOR)
    frame=address_for(publics,aliases,'___CxxFrameHandler')
    candidate=protocol(image,address,frame,destructor,iterator,spec)
    original=protocol(target,spec['handler'],TARGET_FRAME,spec['target_destructor'],
                      TARGET_ITERATOR,spec)
    require(candidate['action_bytes']==original['action_bytes'],
            'generated action length differs from the original')
    pdb=pdb_section_contributions(directory/'source.pdb')
    matches=[c for c in pdb['contributions'] if c['section_number']==row['section_number']
             and c['section_offset']<=row['section_offset']
             and row['section_offset']+10<=c['section_offset']+c['size']]
    require(len(matches)==1,'handler has no unique generated PDB contribution')
    c=matches[0]
    length=candidate['action_bytes']
    require(c['section_offset']+length==row['section_offset'] and c['size']==length+10
            and int(c['characteristics']) & 0x20000020 == 0x20000020,
            'full action+dispatch PDB contribution required')
    require(c['module_index']==parent['pdb_module_index']
            and str(c['object']).lower()==str(parent['pdb_object']).lower(),
            'generated helper must share parent source PDB object')
    modules=[m for m in pdb['modules'] if m['index']==c['module_index']]
    require(len(modules)==1 and str(modules[0]['object']).lower()==str(parent['pdb_object']).lower(),
            'handler PDB module differs')
    return {'parent':unit['symbol'],'handler_symbol':handler_symbol,'pdb_module_index':c['module_index'],
            'candidate':candidate,'target':original,
            'authority':'strict address/metadata binding, not generated-code or runtime EH credit'}
