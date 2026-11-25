"""Bind target-reviewed VM-array EH metadata, without runtime credit.

Covers the Stage, spell-panel, GUI, ASCII and startup VMs, the startup
worker, and the ANM manager's pool, embedded VM and layer array.
A full PDB-generated contribution, immutable FuncInfo/unwind map, saved this,
real VM destructor and CRT entries must agree with the original executable.
"""
from pathlib import Path
import struct

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_EAX, X86_REG_EBP, X86_REG_ECX
from linked_image import (LinkedImageError, linked_code_fields, linked_functions,
                          map_statics, pdb_section_contributions)
import linked_worker_member_eh as worker_eh

PARENTS = {
    '??0StageScriptView@@QAE@XZ': dict(entry=0x402160, size=188, offset=9,
                                     handler=0x46556b, arrays=((0x180, 8),)),
    '??1StageScriptView@@QAE@XZ': dict(entry=0x402440, size=499, offset=3,
                                     handler=0x4652d6, arrays=((0x180, 8), (0x1f08, 3))),
    '??1GuiView@@QAE@XZ': dict(entry=0x4145f0, size=574, offset=3,
        handler=0x46574e, arrays=((0x10, 10), (0x24c8, 10), (0x4980, 9),
                                (0x6a8c, 4), (0x793c, 2), (0x8094, 7), (0x9a48, 1)),
        owner_source='src/Gui.cpp'),
    '??1AsciiManagerView@@QAE@XZ': dict(entry=0x401260, size=467, offset=3,
        handler=0x4652f9, arrays=((0x14, 1), (0x3c0, 1)), owner_source='src/AnmManager.cpp'),
    '??1StartupWorkerView@@QAE@XZ': dict(entry=0x41fb50, size=417, offset=3,
        handler=0x465696, arrays=((0x10, 1), (0x30, 1)), worker_offset=0x10,
        owner_source='src/Startup.cpp'),
    '??1ScriptViewerView@@QAE@XZ': dict(entry=0x40a1a0, size=412, offset=9,
        handler=0x4657e9, arrays=((0x10, 1), (0x2c8, 1)), worker_offset=0x10,
        owner_source='src/ScriptViewer.cpp', vm_destructor_source='src/ScriptViewer.cpp'),
    '??0EnemyGameStateView@@QAE@XZ': dict(entry=0x408910, size=176, offset=9,
        handler=0x465514, arrays=((0x10, 2), (0x778, 8)),
        owner_source='src/EnemyGameStateLifecycle.cpp'),
    '??0AnmRenderManagerView@@QAE@XZ': dict(entry=0x445900, size=2315, offset=3,
        handler=0x4657a5, arrays=((0x68, 4096), (0x3ad130, 1), (0x72dae4, 20)),
        owner_source='src/AnmManager.cpp'),
}
DESTRUCTOR_SYMBOL = '??1AnmVmView@@QAE@XZ'
ITERATOR_SYMBOL = '??_M@YGXPAXIHP6EX0@Z@Z'
TARGET_DESTRUCTOR = 0x401ff0
TARGET_ITERATOR = 0x4525ff
TARGET_FRAME_HANDLER = 0x46276d
TARGET_FREE = 0x452422


def require(condition, message):
    if not condition:
        raise LinkedImageError('stage-array EH: ' + message)


def stage_array_eh_selector(unit, field):
    if 'stage_array_eh_handler' not in field:
        return False
    require(field['stage_array_eh_handler'] is True, 'explicit true opt-in required')
    spec = PARENTS.get(unit.get('symbol'))
    require(spec is not None, 'unsupported parent')
    require(unit.get('artifact_kind') == 'linked-pe'
            and unit.get('target_address') == spec['entry']
            and unit.get('size') == spec['size']
            and unit.get('compare_size', spec['size']) == spec['size'], 'parent contract differs')
    require(field.get('type') == 'DIR32' and not field.get('local', False)
            and int(field.get('width', 4)) == 4 and field.get('addend', 0) == 0
            and field.get('offset') == spec['offset']
            and field.get('target') == spec['handler'], 'separate reviewed handler entry required')
    require(not any(key.endswith('_eh_handler') and key != 'stage_array_eh_handler'
                    for key in field)
            and not any(key.startswith('static_') for key in field), 'mixed binding modes')
    require(field.get('symbol') == '__ehhandler$' + unit['symbol'], 'parent symbol differs')
    return True


def address_for(publics, aliases, semantic):
    symbol = aliases.get(semantic, semantic)
    addresses = {int(row['address']) for row in publics if row['symbol'] == symbol}
    require(len(addresses) == 1, 'symbol absent or ambiguous: ' + semantic)
    return next(iter(addresses))


def section_range(image, address, size, code):
    spans = [s for s in image.sections if image.image_base + s.rva <= address
             and address + size <= image.image_base + s.rva + s.virtual_size]
    require(len(spans) == 1, 'range leaves a unique section')
    flags = spans[0].characteristics
    require(flags & 0x40000000 and not flags & 0x80000000, 'immutable readable section required')
    require(bool(flags & 0x20000000) == code, 'section code/data role differs')


def immediate(instruction, value):
    return len(instruction.operands) == 1 and instruction.operands[0].type == X86_OP_IMM \
        and instruction.operands[0].imm == value


def action_size(member, count):
    if count == 1:
        return 11 if 0 <= member <= 0x7f else 14
    return (25 if 0 <= member <= 0x7f else 27) + (3 if count > 0x7f else 0)


def generated_lead(arrays):
    return sum(action_size(member, count) for member, count in arrays)


def protocol(image, handler, frame, destructor, iterator, arrays, member_destructors=None):
    lead = generated_lead(arrays)
    region_size = lead + 10
    start = handler - lead
    section_range(image, start, region_size, True)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    dispatch = list(decoder.disasm(image.read_address(handler, 10), handler))
    require(len(dispatch) == 2 and [i.size for i in dispatch] == [5, 5]
            and [i.mnemonic for i in dispatch] == ['mov', 'jmp']
            and len(dispatch[0].operands) == 2
            and dispatch[0].operands[0].type == X86_OP_REG
            and dispatch[0].operands[0].reg == X86_REG_EAX
            and dispatch[0].operands[1].type == X86_OP_IMM
            and immediate(dispatch[1], frame), 'handler/frame protocol differs')
    info = dispatch[0].operands[1].imm
    section_range(image, info, 28, False)
    words = struct.unpack('<7I', image.read_address(info, 28))
    require((words[0], words[1], *words[3:]) == (0x19930520, len(arrays), 0, 0, 0, 0),
            'FuncInfo shape differs')
    section_range(image, words[2], 8 * len(arrays), False)
    actions = []
    action_offset = 0
    for index, (member, count) in enumerate(arrays):
        size = action_size(member, count)
        predecessor, action = struct.unpack('<iI', image.read_address(words[2] + index * 8, 8))
        require(predecessor == index - 1 and action == start + action_offset,
                'unwind state/action edge differs')
        code = list(decoder.disasm(image.read_address(action, size), action))
        worker_member = member_destructors is not None and member in member_destructors
        if count == 1:
            require([i.size for i in code] == [3, size - 8, 5]
                    and [i.mnemonic for i in code] == ['mov', 'add', 'jmp'],
                    'complete member cleanup shape differs')
            load, add, jump = code
            callee = member_destructors[member] if worker_member else destructor
            require(immediate(jump, callee), 'member destructor differs')
            receiver = X86_REG_ECX
        else:
            count_size = 5 if count > 0x7f else 2
            require([i.size for i in code] == [5, count_size, 5, 3, size - 20 - count_size, 1, 5, 1]
                    and [i.mnemonic for i in code] ==
                    ['push', 'push', 'push', 'mov', 'add', 'push', 'call', 'ret'],
                    'complete array cleanup shape differs')
            dtor, number, stride, load, add, push, call, ret = code
            require(immediate(dtor, destructor) and immediate(number, count)
                    and immediate(stride, 0x3ac), 'array destructor/count/stride differs')
            require(len(push.operands) == 1 and push.operands[0].type == X86_OP_REG
                    and push.operands[0].reg == X86_REG_EAX
                    and immediate(call, iterator) and not ret.operands,
                    'array iterator protocol differs')
            receiver = X86_REG_EAX
        require(len(load.operands) == 2 and load.operands[0].type == X86_OP_REG
                and load.operands[0].reg == receiver
                and load.operands[1].type == X86_OP_MEM
                and load.operands[1].mem.base == X86_REG_EBP
                and load.operands[1].mem.index == 0 and load.operands[1].mem.segment == 0
                and load.operands[1].mem.disp == 4
                and len(add.operands) == 2 and add.operands[0].type == X86_OP_REG
                and add.operands[0].reg == receiver
                and add.operands[1].type == X86_OP_IMM and add.operands[1].imm == member,
                'saved-this/member protocol differs')
        actions.append(dict(state=index, predecessor=predecessor, address=hex(action),
                            member_offset=member, count=count, size=size,
                            **({'destructor': hex(callee)} if worker_member else {'stride': 0x3ac})))
        action_offset += size
    return dict(handler=hex(handler), func_info=hex(info), unwind_map=hex(words[2]),
                unwind_states=len(arrays), region_bytes=region_size, func_info_bytes=28,
                saved_this_offset=4, actions=actions)


def source_object(unit, source):
    sources = [str(unit['source']), *map(str, unit.get('support_sources', []))]
    require(sources.count(source) == 1, 'one declared source required: ' + source)
    index = sources.index(source)
    kind = 'normal' if source in unit.get('normal_support_sources', []) else 'ltcg'
    return 'source.ltcg.obj' if index == 0 else f'support-{index-1:02d}-{Path(source).stem}.{kind}.obj'


def destructor_identity(unit, directory, image, target, publics, aliases):
    require(DESTRUCTOR_SYMBOL not in aliases, 'VM destructor cannot be an anchor')
    source = PARENTS[unit['symbol']].get('vm_destructor_source', 'src/AnmManager.cpp')
    expected = source_object(unit, source)
    rows = [f for f in linked_functions(directory/'source.exe', directory/'source.map',
                                       directory/'source.pdb')['functions']
            if f['symbol'] == DESTRUCTOR_SYMBOL]
    require(len(rows) == 1 and rows[0]['size'] == 34
            and rows[0]['map_object'] == expected, 'complete qualified 34-byte VM destructor required')
    owner = rows[0]
    require(str(owner['pdb_object']).replace('\\', '/').rsplit('/', 1)[-1] == expected,
            'VM destructor leaves its PDB object')
    modules = [m for m in pdb_section_contributions(directory/'source.pdb')['modules']
               if m['index'] == owner['pdb_module_index']]
    require(len(modules) == 1 and str(modules[0]['object']).casefold() ==
            str(owner['pdb_object']).casefold(), 'VM destructor PDB module differs')
    address = address_for(publics, aliases, DESTRUCTOR_SYMBOL)
    require(int(owner['address'], 0) == address, 'VM destructor entry differs')
    section_range(image, address, 34, True)
    section_range(target, TARGET_DESTRUCTOR, 34, True)
    fields = linked_code_fields(image, publics, address, 34)
    require(fields['normalization_complete'] and fields['decoded_bytes'] == 34
            and {(f['offset'], f['width'], f['type']) for f in fields['fields']} == {(15, 4, 'REL32')},
            'VM destructor field inventory differs')
    code = bytearray(image.read_address(address, 34))
    require(address + 19 + struct.unpack_from('<i', code, 15)[0] ==
            address_for(publics, aliases, '_free'), 'VM destructor free binding differs')
    struct.pack_into('<i', code, 15, TARGET_FREE - (TARGET_DESTRUCTOR + 19))
    require(bytes(code) == target.read_address(TARGET_DESTRUCTOR, 34), 'complete VM destructor bytes differ')
    return address, owner


def stage_array_eh_target(unit, field, directory, image, publics, aliases, target,
                          parent, observed_address):
    require(stage_array_eh_selector(unit, field), 'missing selector')
    require(parent.get('symbol') == unit['symbol'] and parent.get('size') == unit['size']
            and parent.get('map_object') == source_object(unit,
                PARENTS[unit['symbol']].get('owner_source', 'src/StageLifecycle.cpp')),
            'parent leaves its declared lifecycle object')
    rows = [r for r in map_statics(directory/'source.map') if r['symbol'] == field['symbol']]
    require(len(rows) == 1, 'named MAP handler absent or ambiguous')
    row = rows[0]
    address = int(row['address'])
    all_rows = map_statics(directory/'source.map') + publics
    require(sum(r['section_number'] == row['section_number'] and
                r['section_offset'] == row['section_offset'] for r in all_rows) == 1,
            'handler entry is aliased')
    require(address == observed_address and row['flags'] == ['f', 'CIL', 'library:', 'CIL']
            and row['object'] == 'module', 'handler address/producer differs')
    section = image.section(int(row['section_number']))
    require(image.image_base + section.rva + int(row['section_offset']) == address,
            'MAP section location differs')
    pdb = pdb_section_contributions(directory/'source.pdb')
    matches = [r for r in pdb['contributions'] if r['section_number'] == row['section_number']
               and r['section_offset'] <= row['section_offset']
               and row['section_offset'] + 10 <= r['section_offset'] + r['size']]
    spec = PARENTS[unit['symbol']]
    lead = generated_lead(spec['arrays'])
    require(len(matches) == 1, 'handler lacks unique PDB contribution')
    contribution = matches[0]
    require(int(contribution['characteristics']) & 0x20000020 == 0x20000020
            and contribution['section_offset'] + lead == row['section_offset']
            and contribution['size'] == lead + 10, 'complete generated region differs')
    require(contribution['module_index'] == parent['pdb_module_index']
            and str(contribution['object']).casefold() == str(parent['pdb_object']).casefold(),
            'handler leaves parent PDB object')
    modules = [m for m in pdb['modules'] if m['index'] == contribution['module_index']]
    require(len(modules) == 1 and str(modules[0]['object']).casefold() ==
            str(parent['pdb_object']).casefold(), 'parent PDB module differs')
    generated_fields = linked_code_fields(image, publics, address - lead, lead + 10)
    expected = set()
    cursor = 0
    for member, count in spec['arrays']:
        size = action_size(member, count)
        if count == 1:
            expected.add((cursor + size - 4, 4, 'REL32'))
        else:
            expected.update(((cursor + 1, 4, 'DIR32'), (cursor + size - 5, 4, 'REL32')))
        cursor += size
    expected.update({(lead + 1, 4, 'DIR32'), (lead + 6, 4, 'REL32')})
    require(generated_fields['normalization_complete']
            and generated_fields['decoded_bytes'] == lead + 10
            and {(f['offset'], f['width'], f['type']) for f in generated_fields['fields']} == expected,
            'generated region field inventory differs')
    destructor, owner = destructor_identity(unit, directory, image, target, publics, aliases)
    worker_offset = spec.get('worker_offset')
    candidate_members = reviewed_members = worker_proof = None
    if worker_offset is not None:
        worker, worker_owner = worker_eh.destructor_identity(unit, directory, publics, aliases)
        worker_proof = dict(owner=worker_owner,
                            **worker_eh.destructor_bytes(image, target, worker, publics, aliases))
        candidate_members = {worker_offset: worker}
        reviewed_members = {worker_offset: worker_eh.TARGET_DESTRUCTOR}
    iterator = None
    if any(count > 1 for _, count in spec['arrays']):
        iterator = address_for(publics, aliases, ITERATOR_SYMBOL)
    candidate = protocol(image, address, address_for(publics, aliases, '___CxxFrameHandler'),
                         destructor, iterator, spec['arrays'], candidate_members)
    reviewed = protocol(target, field['target'], TARGET_FRAME_HANDLER, TARGET_DESTRUCTOR,
                        TARGET_ITERATOR, spec['arrays'], reviewed_members)
    return dict(symbol=field['symbol'], parent_symbol=unit['symbol'],
                pdb_module_index=contribution['module_index'], candidate=candidate, target=reviewed,
                generated_region_fields=len(expected),
                cleanup_owner=owner, cleanup_proof=dict(bytes=34, fields=1, result='exact'),
                **({'worker_cleanup_proof': worker_proof} if worker_proof is not None else {}),
                authority='address binding only; no generated-code or runtime credit')
