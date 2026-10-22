"""Strict address binding for VC7.1's one-allocation C++ unwind protocol.

This is an opt-in DIR32 binding, not generated-code or runtime credit. The
parent's named handler must share its PDB object. The complete 21-byte generated
region, one-state FuncInfo and saved-storage deletion protocol are checked in
both images. Other EH shapes remain unsupported. No caller or bytes are patched.
"""
from pathlib import Path
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM, X86_REG_EAX, X86_REG_EBP, X86_REG_ECX
from linked_image import LinkedImageError, map_statics, pdb_section_contributions

TARGET_FRAME_HANDLER = 0x0046276D
TARGET_DELETE = 0x004524A1


def require(condition, message):
    if not condition: raise LinkedImageError('allocation EH: ' + message)


def allocation_eh_selector(unit, field):
    if 'allocation_eh_handler' not in field: return False
    require(field['allocation_eh_handler'] is True, 'explicit true opt-in required')
    require(unit.get('artifact_kind') == 'linked-pe', 'linked-pe required')
    require(field.get('type') == 'DIR32' and not field.get('local', False)
            and field.get('addend', 0) == 0
            and 'stack_file_eh_handler' not in field
            and not any(k.startswith('static_') for k in field), 'separate DIR32 entry required')
    require(field.get('symbol') == '__ehhandler$' + str(unit.get('symbol')), 'parent symbol differs')
    return True


def address_for(publics, aliases, semantic):
    symbol = aliases.get(semantic, semantic)
    addresses = {int(row['address']) for row in publics if row['symbol'] == symbol}
    require(len(addresses) == 1, 'runtime symbol is absent or ambiguous: ' + semantic)
    return next(iter(addresses))


def section_range(image, address, size, code):
    spans = [s for s in image.sections if image.image_base+s.rva <= address
             and address+size <= image.image_base+s.rva+s.virtual_size]
    require(len(spans) == 1, 'range is outside a unique section')
    flags = spans[0].characteristics
    require(flags & 0x40000000 and not flags & 0x80000000, 'immutable readable section required')
    require(bool(flags & 0x20000000) == code, 'code/data section role differs')


def protocol(image, handler, frame_handler, deletion):
    section_range(image, handler-11, 21, True)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32); decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(handler, 10), handler))
    require(len(instructions) == 2 and [i.size for i in instructions] == [5,5]
            and instructions[0].mnemonic == 'mov'
            and instructions[0].operands[0].reg == X86_REG_EAX
            and instructions[0].operands[1].type == X86_OP_IMM
            and instructions[1].mnemonic == 'jmp'
            and instructions[1].operands[0].type == X86_OP_IMM
            and instructions[1].operands[0].imm == frame_handler, 'handler/frame-handler protocol differs')
    info = instructions[0].operands[1].imm
    section_range(image, info, 28, False)
    header = struct.unpack('<7I', image.read_address(info,28))
    require((header[0],header[1],*header[3:]) ==
            (0x19930520,1,0,0,0,0), 'FuncInfo is not the supported one-state shape')
    section_range(image, header[2], 8, False)
    state, action = struct.unpack('<iI',image.read_address(header[2],8))
    require(state == -1 and action == handler-11, 'unwind state/action differs')
    action_code = list(decoder.disasm(image.read_address(action,11),action))
    require(len(action_code) == 5 and [i.size for i in action_code] == [3,1,5,1,1]
            and [i.mnemonic for i in action_code] == ['mov','push','call','pop','ret'], 'cleanup instruction shape differs')
    load,push,call,pop,ret = action_code
    require(load.operands[0].reg == X86_REG_EAX and load.operands[1].type == X86_OP_MEM
            and load.operands[1].mem.base == X86_REG_EBP and load.operands[1].mem.index == 0
            and load.operands[1].mem.disp == -0x10
            and push.operands[0].reg == X86_REG_EAX
            and call.operands[0].type == X86_OP_IMM and call.operands[0].imm == deletion
            and pop.operands[0].reg == X86_REG_ECX and not ret.operands,
            'saved-storage/scalar-delete protocol differs')
    return {'handler':hex(handler),'unwind_action':hex(action),'func_info':hex(info),
            'unwind_map':hex(header[2]),'saved_storage_offset':-16,
            'region_bytes':21,'unwind_states':1,'func_info_bytes':28}


def allocation_eh_target(unit, field, directory, image, publics, aliases,
                         target, parent, observed_address):
    require(allocation_eh_selector(unit,field), 'missing selector')
    require(parent.get('symbol') == unit.get('symbol'), 'PDB parent symbol differs')
    statics = map_statics(directory/'source.map')
    rows = [r for r in statics if r['symbol'] == field['symbol']]
    require(len(rows) == 1, 'named MAP handler is absent or ambiguous')
    row = rows[0]; address = int(row['address'])
    require(sum(r['section_number'] == row['section_number'] and r['section_offset'] == row['section_offset'] for r in statics+publics) == 1, 'MAP handler start is aliased')
    require(address == observed_address, 'operand does not name the parent handler')
    require('f' in row['flags'] and row['flags'][1:] == ['CIL','library:','CIL']
            and row['object'] == 'module', 'MAP generated-handler producer differs')
    section = image.section(int(row['section_number']))
    require(image.image_base+section.rva+int(row['section_offset']) == address, 'MAP location differs')
    pdb = pdb_section_contributions(directory/'source.pdb')
    matches = [r for r in pdb['contributions'] if r['section_number'] == row['section_number']
               and r['section_offset'] <= row['section_offset']
               and row['section_offset']+10 <= r['section_offset']+r['size']]
    require(len(matches) == 1, 'generated region lacks a unique PDB contribution')
    contribution = matches[0]
    require(int(contribution['characteristics']) & 0x20000020 == 0x20000020, 'PDB generated region is not code')
    require(contribution['section_offset']+11 == row['section_offset'] and contribution['size'] == 21,
            'generated contribution is not the complete 21-byte region')
    require(contribution['module_index'] == parent['pdb_module_index']
            and str(contribution['object']).lower() == str(parent['pdb_object']).lower(), 'handler leaves its parent PDB object')
    modules = [m for m in pdb['modules'] if m['index'] == contribution['module_index']]
    require(len(modules) == 1 and str(modules[0]['object']).lower() == str(parent['pdb_object']).lower(),
            'parent PDB module differs')
    candidate_protocol = protocol(image,address,address_for(publics,aliases,'___CxxFrameHandler'),
                                  address_for(publics,aliases,'??3@YAXPAX@Z'))
    target_protocol = protocol(target,int(field['target']),TARGET_FRAME_HANDLER,TARGET_DELETE)
    return {'symbol':field['symbol'],'parent_symbol':unit['symbol'],
            'pdb_module_index':contribution['module_index'],'candidate':candidate_protocol,
            'target':target_protocol,'authority':'address binding only; no generated-code/runtime credit'}
