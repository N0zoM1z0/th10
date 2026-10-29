"""Bind the reviewed two-sentinel Chain destructor EH entry, without runtime credit."""
from pathlib import Path
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_EAX, X86_REG_EBP
from linked_image import (LinkedImageError, map_statics, pdb_section_contributions,
                          linked_functions, linked_code_fields)

PARENT_SYMBOL = '??1Chain@th10@@QAE@XZ'
DESTRUCTOR_SYMBOL = '??1ChainElem@th10@@QAE@XZ'
TARGET_FRAME_HANDLER = 0x0046276D
TARGET_DESTRUCTOR = 0x00449A90


def require(condition, message):
    if not condition:
        raise LinkedImageError('chain-member EH: ' + message)


def chain_member_eh_selector(unit, field):
    if 'chain_member_eh_handler' not in field:
        return False
    require(field['chain_member_eh_handler'] is True, 'explicit true opt-in required')
    require(unit.get('artifact_kind') == 'linked-pe', 'linked-pe required')
    require(unit.get('symbol') == PARENT_SYMBOL, 'unsupported parent')
    require(field.get('type') == 'DIR32' and not field.get('local', False)
            and field.get('addend', 0) == 0 and int(field.get('width', 4)) == 4
            and int(field.get('offset', -1)) == 9
            and not any(k in field for k in ['allocation_eh_handler',
                        'stack_file_eh_handler', 'worker_member_eh_handler'])
            and not any(k.startswith('static_') for k in field),
            'separate DIR32 handler entry required')
    require(field.get('symbol') == '__ehhandler$' + PARENT_SYMBOL, 'parent symbol differs')
    return True


def address_for(publics, aliases, semantic):
    symbol = aliases.get(semantic, semantic)
    rows = {int(r['address']) for r in publics if r['symbol'] == symbol}
    require(len(rows) == 1, 'symbol is absent or ambiguous: ' + semantic)
    return next(iter(rows))


def section_range(image, address, size, code):
    spans = [s for s in image.sections if image.image_base+s.rva <= address
             and address+size <= image.image_base+s.rva+s.virtual_size]
    require(len(spans) == 1, 'range leaves a unique section')
    flags = spans[0].characteristics
    require(flags & 0x40000000 and not flags & 0x80000000,
            'immutable readable section required')
    require(bool(flags & 0x20000000) == code, 'section code/data role differs')


def protocol(image, handler, frame, destructor):
    section_range(image, handler-19, 29, True)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32); decoder.detail = True
    dispatch = list(decoder.disasm(image.read_address(handler, 10), handler))
    require(len(dispatch) == 2 and [i.size for i in dispatch] == [5, 5]
            and [i.mnemonic for i in dispatch] == ['mov', 'jmp']
            and dispatch[0].operands[0].type == X86_OP_REG
            and dispatch[0].operands[0].reg == X86_REG_EAX
            and dispatch[0].operands[1].type == X86_OP_IMM
            and dispatch[1].operands[0].type == X86_OP_IMM
            and dispatch[1].operands[0].imm == frame, 'handler/frame protocol differs')
    info = dispatch[0].operands[1].imm
    section_range(image, info, 28, False)
    words = struct.unpack('<7I', image.read_address(info, 28))
    require((words[0], words[1], *words[3:]) == (0x19930520, 2, 0, 0, 0, 0),
            'FuncInfo is not the two-state28-byte shape')
    section_range(image, words[2], 16, False)
    rows = struct.unpack('<iIiI', image.read_address(words[2], 16))
    require(rows == (-1, handler-19, 0, handler-11), 'unwind state/action edges differ')
    for action, size, member in [(handler-19, 8, 0), (handler-11, 11, 0x24)]:
        code = list(decoder.disasm(image.read_address(action, size), action))
        require(len(code) == (3 if member else 2)
                and [i.size for i in code] == ([3, 3, 5] if member else [3, 5])
                and [i.mnemonic for i in code] == (['mov', 'add', 'jmp'] if member
                                                  else ['mov', 'jmp']),
                'cleanup instruction shape differs')
        load, tail = code[0], code[-1]
        require(load.operands[0].type == X86_OP_REG
                and load.operands[0].reg == X86_REG_EAX
                and load.operands[1].type == X86_OP_MEM
                and load.operands[1].mem.base == X86_REG_EBP
                and load.operands[1].mem.index == 0 and load.operands[1].mem.disp == 4
                and tail.operands[0].type == X86_OP_IMM
                and tail.operands[0].imm == destructor, 'saved-this/destructor tail differs')
        if member:
            require(code[1].operands[0].type == X86_OP_REG
                    and code[1].operands[0].reg == X86_REG_EAX
                    and code[1].operands[1].type == X86_OP_IMM
                    and code[1].operands[1].imm == member, 'sentinel member offset differs')
    return dict(handler=hex(handler), func_info=hex(info), unwind_map=hex(words[2]),
                region_bytes=29, func_info_bytes=28, unwind_states=2,
                saved_this_offset=4, member_offsets=[0, 0x24])


def destructor_identity(unit, directory, image, publics, aliases, target, parent):
    require(DESTRUCTOR_SYMBOL not in aliases, 'destructor cannot be a diagnostic anchor')
    sources = [str(unit['source']), *map(str, unit.get('support_sources', []))]
    indices = [i for i, s in enumerate(sources) if s == 'src/Chain.cpp']
    require(len(indices) == 1, 'one Chain source required')
    index = indices[0]
    kind = 'normal' if sources[index] in unit.get('normal_support_sources', []) else 'ltcg'
    obj = 'source.ltcg.obj' if index == 0 else f'support-{index-1:02d}-Chain.{kind}.obj'
    require(parent.get('map_object') == obj, 'parent leaves its Chain source')
    rows = [r for r in linked_functions(directory/'source.exe', directory/'source.map',
                                       directory/'source.pdb')['functions']
            if r['symbol'] == DESTRUCTOR_SYMBOL]
    require(len(rows) == 1 and rows[0]['size'] == 12 and rows[0]['map_object'] == obj
            and rows[0]['pdb_module_index'] == parent['pdb_module_index']
            and str(rows[0]['pdb_object']).lower() == str(parent['pdb_object']).lower(),
            'complete qualified12-byte member destructor required')
    address = address_for(publics, aliases, DESTRUCTOR_SYMBOL)
    require(address == int(rows[0]['address'], 0), 'destructor public/PDB entry differs')
    section_range(image, address, 12, True)
    section_range(target, TARGET_DESTRUCTOR, 12, True)
    fields = linked_code_fields(image, publics, address, 12)
    require(fields['normalization_complete'] and not fields['fields'],
            'member destructor must have zero link fields')
    require(image.read_address(address, 12) == target.read_address(TARGET_DESTRUCTOR, 12),
            'complete member destructor bytes differ')
    return address, dict(symbol=DESTRUCTOR_SYMBOL, bytes=12, fields=0, result='exact')


def chain_member_eh_target(unit, field, directory, image, publics, aliases,
                           target, parent, observed_address):
    require(chain_member_eh_selector(unit, field), 'missing selector')
    require(parent.get('symbol') == PARENT_SYMBOL, 'PDB parent differs')
    statics = map_statics(directory/'source.map')
    rows = [r for r in statics if r['symbol'] == field['symbol']]
    require(len(rows) == 1, 'named MAP handler is absent or ambiguous')
    row = rows[0]; address = int(row['address'])
    require(sum(r['section_number'] == row['section_number'] and
                r['section_offset'] == row['section_offset'] for r in statics+publics) == 1,
            'MAP handler start is aliased')
    require(address == observed_address, 'operand does not name the parent handler')
    require(row['flags'] == ['f', 'CIL', 'library:', 'CIL'] and row['object'] == 'module',
            'MAP generated-handler producer differs')
    section = image.section(int(row['section_number']))
    require(image.image_base+section.rva+int(row['section_offset']) == address,
            'MAP location differs')
    pdb = pdb_section_contributions(directory/'source.pdb')
    regions = [r for r in pdb['contributions'] if r['section_number'] == row['section_number']
               and r['section_offset'] <= row['section_offset']
               and row['section_offset']+10 <= r['section_offset']+r['size']]
    require(len(regions) == 1, 'generated region lacks a unique PDB contribution')
    region = regions[0]
    require(region['section_offset']+19 == row['section_offset'] and region['size'] == 29
            and int(region['characteristics']) & 0x20000020 == 0x20000020,
            'complete29-byte code region required')
    require(region['module_index'] == parent['pdb_module_index']
            and str(region['object']).lower() == str(parent['pdb_object']).lower(),
            'handler leaves its parent PDB object')
    modules = [r for r in pdb['modules'] if r['index'] == region['module_index']]
    require(len(modules) == 1 and str(modules[0]['object']).lower() ==
            str(parent['pdb_object']).lower(), 'parent PDB module differs')
    destructor, proof = destructor_identity(unit, directory, image, publics, aliases, target, parent)
    native = protocol(image, address, address_for(publics, aliases, '___CxxFrameHandler'), destructor)
    original = protocol(target, int(field['target']), TARGET_FRAME_HANDLER, TARGET_DESTRUCTOR)
    return dict(symbol=field['symbol'], parent_symbol=PARENT_SYMBOL,
                pdb_module_index=region['module_index'], cleanup_proof=proof,
                candidate=native, target=original,
                authority='handler address binding only; no generated-code or runtime credit')
