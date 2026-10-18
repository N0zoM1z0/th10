#!/usr/bin/env python3
"""Final A006/A008 bounded structure, protected-stack preparation and return syntax; no ABI theorem."""
import argparse, hashlib, importlib.util, json, os, pathlib, resource, signal, subprocess, sys, tomllib
if not __debug__:
    raise RuntimeError('optimized Python refused')
sys.dont_write_bytecode = True
P = pathlib.Path
HEAD = 'e534a6f6c77115017e613721353308a41cde9b6a'
ROOT = '/home/pentester/coding/codex_ida/th10-reconstruction/th10'
OWNER_SIZE = 1612
OWNER_SHA = '5c5f98eb1bd41a6b3c591b3f012d7a5926277c4e436004454649b85e61cc1463'
HELPER_SYMBOL = '?BulletResolveTransformAngle@@YAMPAUBulletRuntimeView@@M@Z'
SPEC = {'symbol': '?AdvanceTransformProgram@BulletRuntimeView@@QAEXXZ', 'object': 'support-01-BulletTransform.normal.obj', 'target_size': 1454}
ALIAS_PINS = {
    'link-aliases.rsp': '7af855ebef42efc8d424897afd358bcfff13b212aacdec1f4f9339920ad30e56',
    'link-anchors.c': '97d1508d5b5d2d5e0511e73e05adb49178e3424d162f13343896db92c3f2561b',
}

class GateError(ValueError):
    pass

def need(value, category):
    if not value:
        raise GateError(category)

def sha_bytes(data):
    return hashlib.sha256(data).hexdigest()

def regular(path):
    need(path.is_absolute(), 'absolute_path_required')
    need(all(not x.is_symlink() for x in (path, *path.parents)), 'symlink_input')
    need(path.is_file() and path.stat().st_size <= 64 * 1024 * 1024, 'file_extent')
    return path

def sha(path):
    return sha_bytes(regular(path).read_bytes())

# Complete PDB/MAP attribution predicates copied from the pinned A005 runner.
def predicates(l,image,maps,pdb,build,spec,metadata_only=False):
 def check(fn):
  try:return bool(fn())
  except (ValueError,KeyError,TypeError,IndexError,AssertionError):return False
 out={}
 expected=l._object_path(build/spec['object'])
 out['all_module_object_paths_normalizable']=check(lambda:all(l._object_path(x['object']) for x in pdb['modules']))
 modules=[x for x in pdb['modules'] if l._object_basename(x['object'])==spec['object'].lower()]
 out['basename_prefilter_unique_module']=len(modules)==1
 out['prefilter_module_exact_path']=len(modules)==1 and check(lambda:l._object_path(modules[0]['object'])==expected)
 rows=[x for x in maps if x['symbol']==spec['symbol']]
 out['unique_requested_symbol']=len(rows)==1
 if len(rows)!=1:return out
 row=rows[0];start=(row['section_number'],row['section_offset'])
 out['unique_same_start_symbol']=sum((x['section_number'],x['section_offset'])==start for x in maps)==1
 cs=[x for x in pdb['contributions'] if (x['section_number'],x['section_offset'])==start]
 out['unique_same_start_contribution']=len(cs)==1
 if len(cs)!=1:return out
 c=cs[0];size=c['size'];section=image.section(start[0]);required=l.IMAGE_SCN_CNT_CODE|l.IMAGE_SCN_MEM_EXECUTE
 out.update(positive_contribution_size=type(size) is int and size>0,
  contribution_exact_object_path=check(lambda:l._object_path(c['object'])==expected),
  contribution_module_index_equal=len(modules)==1 and c['module_index']==modules[0]['index'],
  contribution_module_name_equal=len(modules)==1 and c['module']==modules[0]['module'],
  map_object_basename_equal=l._object_basename(row['object'])==spec['object'].lower(),
  map_object_fullpath_if_present=not any(x in str(row['object']) for x in ('/','\\',':')) or check(lambda:l._object_path(row['object'])==expected),
  function_symbol_flag='f' in row['flags'],
  contribution_code_executable=c['characteristics']&required==required,
  section_code_executable=section.characteristics&required==required,
  virtual_extent_covered=0<=start[1] and start[1]+size<=section.virtual_size,
  no_interior_function_symbol=not any(x['section_number']==start[0] and start[1]<x['section_offset']<start[1]+size and 'f' in x['flags'] for x in maps),
  map_address_agrees=row['address']==image.image_base+section.rva+start[1],
  no_overlapping_contribution=not any(x is not c and x['section_number']==start[0] and x['size']>0 and x['section_offset']<start[1]+size and start[1]<x['section_offset']+x['size'] for x in pdb['contributions']),
  raw_extent_covered=check(lambda:(0 <= start[1] and start[1]+size <= section.raw_size and 0 <= section.raw_offset and section.raw_offset+start[1]+size <= len(image.data)) if metadata_only else len(image.read_section(start[0],start[1],size))==size))
 return out

def measure(l,image,maps,pdb,build,spec):
 gates=predicates(l,image,maps,pdb,build,spec)
 expected={'basename_prefilter_unique_module','prefilter_module_exact_path','unique_requested_symbol','unique_same_start_symbol','unique_same_start_contribution','positive_contribution_size','contribution_exact_object_path','contribution_module_index_equal','contribution_module_name_equal','map_object_basename_equal','map_object_fullpath_if_present','function_symbol_flag','contribution_code_executable','section_code_executable','virtual_extent_covered','no_interior_function_symbol','map_address_agrees','no_overlapping_contribution','raw_extent_covered'}
 assert set(gates)==expected|{'all_module_object_paths_normalizable'}
 assert all(gates[k] for k in expected),'complete owner attribution failed'
 row=next(x for x in maps if x['symbol']==spec['symbol'])
 c=next(x for x in pdb['contributions'] if (x['section_number'],x['section_offset'])==(row['section_number'],row['section_offset']))
 payload=image.read_section(c['section_number'],c['section_offset'],c['size'])
 return {'complete':True,'size':c['size'],'sha256':hashlib.sha256(payload).hexdigest(),'target_size':spec['target_size'],'size_equal':c['size']==spec['target_size'],'all_required_attribution_gates':True}

# Finite normal-flow allowlist. Unknown instructions are unresolved, even when
# Capstone omits a control group. No semantic/dataflow equivalence is inferred.
NORMAL = frozenset(('mov movzx movsx lea push pop add adc sub sbb and or xor cmp test not neg inc dec shl shr sar sal rol ror rcl rcr shld shrd mul imul div idiv cdq cwde cbw xchg nop wait cld sahf lahf stc clc cmc '
    'fld fild fld1 fldz fldpi fldl2e fldl2t fldlg2 fldln2 fst fstp fist fistp fisttp '
    'fadd faddp fiadd fsub fsubp fsubr fsubrp fisub fisubr fmul fmulp fimul fdiv fdivp fdivr fdivrp fidiv fidivr '
    'fcom fcomp fcompp fucom fucomp fucompp fcomi fcomip fucomi fucomip ftst fxam fxch fchs fabs fsqrt fsin fcos fsincos fpatan fptan frndint fscale fprem fprem1 f2xm1 fyl2x fyl2xp1 '
    'fnstsw fnstcw fldcw fnclex ffree fincstp fdecstp '
    'movsb movsw movsd stosb stosw stosd lodsb lodsw lodsd '
    'seta setae setb setbe sete setne setg setge setl setle seto setno sets setns setp setnp '
    'cmova cmovae cmovb cmovbe cmove cmovne cmovg cmovge cmovl cmovle cmovo cmovno cmovs cmovns cmovp cmovnp').split())
JCC = frozenset('ja jae jb jbe je jne jg jge jl jle jo jno js jns jp jnp'.split())
GPR = frozenset('eax ebx ecx edx esi edi ebp esp'.split())
WRITES = GPR | frozenset('ax al ah bx bl bh cx cl ch dx dl dh si di bp sp eflags fpsw fpcw'.split()) | frozenset('st(%d)' % i for i in range(8))


def ordinary(md, i):
    from capstone import CS_GRP_BRANCH_RELATIVE, CS_GRP_INT, CS_GRP_IRET, CS_GRP_PRIVILEGE
    need(not any(i.group(g) for g in (CS_GRP_BRANCH_RELATIVE, CS_GRP_INT, CS_GRP_IRET, CS_GRP_PRIVILEGE)), 'unsupported_control_group')
    prefixes = tuple(i.prefix)
    if prefixes[0] == 0xf3:
        need(prefixes == (0xf3, 0, 0, 0) and i.mnemonic in ('rep movsb', 'rep movsd', 'rep stosb', 'rep stosd'), 'unsupported_repeat_prefix')
    else:
        need(i.mnemonic in NORMAL, 'unsupported_normal_instruction')
        need(prefixes == (0, 0, 0, 0) or (prefixes == (0, 0, 0x66, 0) and i.mnemonic in NORMAL and any(o.size == 2 for o in i.operands)), 'unsupported_normal_prefix')
    _, written = i.regs_access()
    need(all(md.reg_name(r) in WRITES for r in written), 'unsupported_normal_write_effect')


def proposal(md, nodes, edges, site, start, end, section_info, capture=None):
    """Instruction metadata only. The callback classifies sections, never reads bytes."""
    from capstone import CS_AC_READ
    from capstone.x86 import X86_OP_IMM, X86_OP_REG, X86_OP_MEM
    pred = {a: set() for a in nodes}
    for at, successors in edges.items():
        for successor in successors:
            need(successor in pred, 'missing_direct_successor')
            pred[successor].add(at)
    # The function-entry edge counts as a predecessor, so a loop back into an
    # instruction at entry cannot accidentally establish unique dominance.
    pred[start].add(None)
    def previous(at):
        need(len(pred[at]) == 1 and None not in pred[at], 'known_direct_prefix_bypass')
        before = nodes[next(iter(pred[at]))]
        need(before.address + before.size == at and not any(before.prefix), 'noncontiguous_or_prefixed_dispatch_chain')
        return before
    def reg(op):
        need(op.type == X86_OP_REG and op.size == 4 and md.reg_name(op.reg) in GPR, 'dispatch_register_width')
        return op.reg
    def writes(i):
        return {md.reg_name(r) for r in i.regs_access()[1]}
    jump = nodes[site]
    need(not any(jump.prefix) and jump.mnemonic == 'jmp' and len(jump.operands) == 1, 'dispatch_jump_shape')
    op = jump.operands[0]
    need(op.type == X86_OP_MEM and op.size == 4 and op.access == CS_AC_READ and not op.mem.base and op.mem.index and op.mem.scale == 4 and not op.mem.segment, 'dispatch_memory_shape')
    need(md.reg_name(op.mem.index) in GPR and writes(jump) == set(), 'dispatch_jump_effect')
    index = op.mem.index
    table = op.mem.disp & 0xffffffff
    need(table + 4 <= 0x100000000, 'destination_table_cell_wrap')
    before = previous(site)
    remap = None
    if before.mnemonic == 'movzx':
        need(len(before.operands) == 2 and reg(before.operands[0]) == index and writes(before) == {md.reg_name(index)}, 'remap_destination_or_effect')
        mem = before.operands[1]
        need(mem.type == X86_OP_MEM and mem.size == 1 and mem.access == CS_AC_READ and not mem.mem.segment and bool(mem.mem.base) != bool(mem.mem.index) and (not mem.mem.index or mem.mem.scale == 1), 'remap_memory_shape')
        index = mem.mem.base or mem.mem.index
        need(md.reg_name(index) in GPR, 'remap_index_register')
        remap = mem.mem.disp & 0xffffffff
        before = previous(before.address)
    need(before.mnemonic == 'ja' and len(before.operands) == 1 and before.operands[0].type == X86_OP_IMM and before.operands[0].size == 4 and writes(before) == set(), 'unsigned_guard_shape_or_effect')
    guard = before
    compare = previous(guard.address)
    need(compare.mnemonic == 'cmp' and len(compare.operands) == 2 and reg(compare.operands[0]) == index and compare.operands[1].type == X86_OP_IMM and compare.operands[1].size == 4 and writes(compare) == {'eflags'}, 'guard_compare_shape_or_effect')
    default = guard.operands[0].imm
    need(not compare.address <= default < site + jump.size, 'default_enters_dispatch_prefix')
    maximum = compare.operands[1].imm
    need(0 <= maximum <= (127 if remap is not None else 31), 'dispatch_range_bound')
    address = remap if remap is not None else table
    span = maximum + 1 if remap is not None else 4 * (maximum + 1)
    need(address + span <= 0x100000000, 'proposed_table_span_wrap')
    metadata = section_info(address, span)
    need(type(metadata) is dict and all(type(v) is bool for v in metadata.values()), 'metadata_output_type')
    metadata.update({
        'wholly_inside_complete_owner': start <= address and address + span <= end,
        'disjoint_from_complete_owner': address + span <= start or end <= address,
        'disjoint_from_known_reachable_instructions': all(address + span <= a or a + i.size <= address for a, i in nodes.items()),
    })
    if capture is not None:
        capture.update(remap_address=remap, pointer_address=table, dispatch_site=site, compare_site=compare.address, guard_site=guard.address)
    return {
        'status': 'METADATA_ONLY_RANGE_PROPOSAL',
        'required_table_kind': 'byte_remap' if remap is not None else 'destination_pointer_table',
        'required_span_bytes': span,
        'pointer_table_extent_known': remap is None,
        'span_classification': metadata,
        'known_direct_prefix_has_no_bypass': True,
        'complete_cfg_guard_dominance_proved': False,
        'table_contents_inspected': False,
        'table_contents_sha256_exported': False,
    }


def preflight(md, code, start, section_info, capture=None, resolved=None, forbidden=frozenset(), propose_indirect=True):
    from capstone import CS_GRP_CALL, CS_GRP_JUMP, CS_GRP_RET, CS_GRP_INT, CS_GRP_IRET, CS_GRP_PRIVILEGE
    from capstone.x86 import X86_OP_IMM, X86_OP_REG, X86_OP_MEM
    need(type(code) is bytes and 0 < len(code) <= OWNER_SIZE and 0 <= start < start + len(code) <= 0x100000000, 'owner_decode_extent')
    end = start + len(code)
    nodes, edges, covered = {}, {}, set()
    pending, indirect = [start], []
    calls = returns = 0
    while pending:
        at = pending.pop()
        if at in nodes:
            continue
        need(start <= at < end, 'successor_outside_owner')
        need(at not in forbidden, 'control_flow_enters_table')
        need(at not in covered, 'overlapping_decode')
        found = list(md.disasm(code[at-start:min(len(code), at-start+15)], at, count=1))
        need(len(found) == 1, 'reachable_decode_failure')
        i = found[0]
        need(i.address == at and 0 < i.size <= 15 and at + i.size <= end, 'instruction_decode_extent')
        need(bytes(i.bytes) == code[at-start:at-start+i.size], 'decoder_bytes_disagree')
        # Capstone reports one prefix per group; inspect the raw prefix run too
        # so repeated/hidden prefixes cannot be silently collapsed.
        raw_prefixes = []
        for value in i.bytes:
            if value not in (0xf0, 0xf2, 0xf3, 0x2e, 0x36, 0x3e, 0x26, 0x64, 0x65, 0x66, 0x67):
                break
            raw_prefixes.append(value)
        need(raw_prefixes in ([], [0x66], [0xf3]), 'unsupported_raw_prefix_sequence')
        need(raw_prefixes == [v for v in i.prefix if v], 'decoder_prefix_disagree')
        cells = set(range(at, at + i.size))
        need(not cells & covered, 'overlapping_decode')
        need(not cells & forbidden, 'instruction_overlaps_table')
        covered |= cells
        nodes[at] = i
        need(len(nodes) <= OWNER_SIZE, 'instruction_count_bound')
        control = [i.group(g) for g in (CS_GRP_CALL, CS_GRP_JUMP, CS_GRP_RET)]
        need(sum(control) <= 1, 'ambiguous_control_group')
        need(not any(i.group(g) for g in (CS_GRP_INT, CS_GRP_IRET, CS_GRP_PRIVILEGE)), 'unsupported_control_group')
        successors = []
        if any(control):
            need(not any(i.prefix), 'control_prefix')
        if i.group(CS_GRP_RET):
            need(i.mnemonic == 'ret' and (not i.operands or len(i.operands) == 1 and i.operands[0].type == X86_OP_IMM and i.operands[0].size == 4 and 0 <= i.operands[0].imm <= 32 and i.operands[0].imm % 4 == 0), 'return_shape')
            need({md.reg_name(r) for r in i.regs_access()[1]} == {'esp'}, 'return_write_effect')
            returns += 1
        elif i.group(CS_GRP_JUMP):
            need(i.mnemonic in JCC | {'jmp'} and len(i.operands) == 1, 'unsupported_branch_shape')
            need(not i.regs_access()[1], 'branch_write_effect')
            op = i.operands[0]
            if op.type == X86_OP_IMM:
                need(op.size == 4 and start <= op.imm < end, 'direct_branch_outside_owner_or_width')
                successors.append(op.imm)
                if i.mnemonic != 'jmp':
                    successors.append(at + i.size)
            else:
                need(i.mnemonic == 'jmp' and op.type in (X86_OP_REG, X86_OP_MEM) and op.size == 4, 'indirect_branch_shape')
                indirect.append(at)
                need(len(indirect) <= 1, 'unresolved_dispatch_count_bound')
                if resolved is not None:
                    need(at == resolved[0], 'additional_indirect_dispatch')
                    need(type(resolved[1]) is frozenset and 0 < len(resolved[1]) <= 5, 'resolved_target_set_bound')
                    successors.extend(sorted(resolved[1]))
        else:
            if i.group(CS_GRP_CALL):
                need(i.mnemonic == 'call' and len(i.operands) == 1, 'call_shape')
                op = i.operands[0]
                need(op.size == 4 and op.type in (X86_OP_IMM, X86_OP_REG, X86_OP_MEM), 'call_operand_shape')
                if op.type == X86_OP_IMM:
                    need(0 <= op.imm <= 0xffffffff and not start <= op.imm < end, 'internal_call_or_target_width')
                need({md.reg_name(r) for r in i.regs_access()[1]} == {'esp'}, 'call_write_effect')
                calls += 1
            else:
                ordinary(md, i)
            successors.append(at + i.size)
        need(all(start <= target < end for target in successors), 'fallthrough_outside_owner')
        need(all(target not in forbidden for target in successors), 'control_flow_enters_table')
        edges[at] = successors
        pending.extend(successors)
    if resolved is not None:
        need(indirect == [resolved[0]], 'resolved_dispatch_not_recovered')
    unresolved = len(indirect) if resolved is None else 0
    result = {
        'status': 'UNRESOLVED_INDIRECT_DISPATCH' if unresolved else ('CFG_CLOSED' if resolved is not None else 'DIRECT_CFG_CLOSED'),
        'owner_byte_count': len(code),
        'reachable_instruction_count': len(nodes),
        'reachable_instruction_byte_count': len(covered),
        'unclassified_owner_byte_count': len(code) - len(covered),
        'direct_cfg_edge_count': sum(len(v) for v in edges.values()),
        'opaque_returning_call_count': calls,
        'return_instruction_count': returns,
        'unresolved_indirect_branch_count': unresolved,
        'full_owner_cfg_closed_under_model': not unresolved,
        'all_owner_bytes_reachable_instructions': not unresolved and len(covered) == len(code),
        'unclassified_bytes_are_not_assumed_data_or_fill': True,
        'immutable_owner_and_opaque_normally_returning_calls_assumed': True,
        'dynamic_reachability_proved': False,
        'table_contents_inspected': False,
        'exactness_credit': 'none',
    }
    if capture is not None:
        capture.update(nodes=nodes, edges=edges, covered=covered, indirect=indirect)
    if indirect and resolved is None and propose_indirect:
        try:
            result['table_range_proposal'] = proposal(md, nodes, edges, indirect[0], start, end, section_info, capture)
        except GateError as exc:
            result['table_range_proposal'] = {'status': 'UNRESOLVED', 'category': str(exc), 'table_contents_inspected': False}
    return result




def close_owner(md, code, start, section_info, read_table, require_native_preflight=False, capture=None):
    import struct
    before = {}
    initial = preflight(md, code, start, section_info, before)
    need(initial['status'] == 'UNRESOLVED_INDIRECT_DISPATCH' and initial['unresolved_indirect_branch_count'] == 1, 'authorized_preflight_dispatch')
    scope = initial.get('table_range_proposal', {})
    need(scope.get('status') == 'METADATA_ONLY_RANGE_PROPOSAL' and scope.get('required_table_kind') == 'byte_remap' and scope.get('required_span_bytes') == 128 and scope.get('pointer_table_extent_known') is False, 'authorized_exact_remap_proposal')
    expected_metadata = {
        'unique_mapped_virtual_section': True, 'raw_span_covered': True,
        'readable_section': True, 'executable_section': True,
        'disjoint_from_other_pdb_contributions': True,
        'wholly_inside_complete_owner': True, 'disjoint_from_complete_owner': False,
        'disjoint_from_known_reachable_instructions': True,
    }
    need(scope['span_classification'] == expected_metadata, 'authorized_remap_classification')
    if require_native_preflight:
        need(len(code) == OWNER_SIZE and sha_bytes(code) == OWNER_SHA, 'complete_owner_closure_pin')
    end = start + len(code)
    remap_address = before['remap_address']
    pointer_address = before['pointer_address']
    remap_cells = set(range(remap_address, remap_address + 128))
    need(start <= remap_address and remap_address + 128 <= end and not remap_cells & before['covered'], 'remap_owner_or_code_overlap')
    remap = read_table(remap_address, 128)
    need(type(remap) is bytes and len(remap) == 128 and remap == code[remap_address-start:remap_address-start+128], 'remap_read_mapping')
    maximum = max(remap)
    pointer_size = 4 * (maximum + 1)
    base = {
        'owner_byte_count': len(code),
        'remap_byte_count': 128,
        'remap_sha256': sha_bytes(remap),
        'required_pointer_table_byte_count': pointer_size,
        'remap_contents_inspected': True,
        'pointer_table_contents_inspected': False,
        'full_owner_cfg_closed_under_model': False,
        'exactness_credit': 'none',
        'immutable_code_and_table_contents_assumed': True,
        'opaque_normally_returning_calls_assumed': True,
        'normal_valid_nonexception_memory_access_assumed': True,
        'dynamic_reachability_proved': False,
        'unclassified_bytes_are_not_assumed_data_or_fill': True,
    }
    if pointer_size > 20:
        return {**base, 'status': 'UNRESOLVED_POINTER_SPAN_EXCEEDS_AUTHORIZED_BOUND', 'pointer_table_byte_count': 0, 'classified_table_byte_count': 128, 'known_instruction_byte_count': len(before['covered']), 'unclassified_owner_byte_count': len(code)-len(before['covered'])-128, 'additional_pointer_range_proposal_only': True}
    need(4 <= pointer_size <= 20 and pointer_size % 4 == 0, 'pointer_extent_bound')
    pointer_cells = set(range(pointer_address, pointer_address + pointer_size))
    need(start <= pointer_address and pointer_address + pointer_size <= end and not pointer_cells & (remap_cells | before['covered']), 'pointer_owner_remap_or_code_overlap')
    need(section_info(pointer_address, pointer_size) == {key: value for key, value in expected_metadata.items() if key not in {'wholly_inside_complete_owner', 'disjoint_from_complete_owner', 'disjoint_from_known_reachable_instructions'}}, 'pointer_section_or_contribution_ambiguity')
    pointer_bytes = read_table(pointer_address, pointer_size)
    need(type(pointer_bytes) is bytes and len(pointer_bytes) == pointer_size and pointer_bytes == code[pointer_address-start:pointer_address-start+pointer_size], 'pointer_read_mapping')
    # Slots not selected by any remap entry are data only. They are not
    # decoded or accepted as executable destinations.
    pointers = struct.unpack('<%dI' % (maximum + 1), pointer_bytes)
    targets = frozenset(pointers[index] for index in remap)
    tables = frozenset(remap_cells | pointer_cells)
    need(all(start <= target < end and target not in tables for target in targets), 'selected_table_target_outside_code_extent')
    after = {}
    completed = preflight(md, code, start, section_info, after, (before['dispatch_site'], targets), tables)
    need(completed['full_owner_cfg_closed_under_model'] and completed['unresolved_indirect_branch_count'] == 0, 'complete_cfg_not_closed')
    # Re-run the exact recognizer against all direct and recovered table
    # edges. Its unique contiguous predecessors establish the whole-CFG
    # no-bypass condition for compare/guard/remap/jump under this model.
    final_capture = {}
    final_scope = proposal(md, after['nodes'], after['edges'], before['dispatch_site'], start, end, section_info, final_capture)
    need(final_scope == scope and final_capture == {key: before[key] for key in final_capture}, 'complete_cfg_dispatch_contract_drift')
    need(all(target in after['nodes'] for successors in after['edges'].values() for target in successors), 'complete_cfg_successor_boundary')
    need(not after['covered'] & tables, 'complete_cfg_code_table_overlap')
    remaining = set(range(start, end)) - after['covered'] - tables
    need(len(after['covered']) + len(tables) + len(remaining) == len(code), 'complete_owner_accounting')
    classified_code = bytes(code[at-start] for at in sorted(after['covered']))
    unclassified = bytes(code[at-start] for at in sorted(remaining))
    if capture is not None:
        capture.update(nodes=after['nodes'], edges=after['edges'], covered=after['covered'], tables=tables, remaining=remaining)
    return {
        **base,
        'status': 'BOUNDED_COMPLETE_OWNER_CFG_CLOSED',
        'pointer_table_contents_inspected': True,
        'pointer_table_byte_count': pointer_size,
        'pointer_table_sha256': sha_bytes(pointer_bytes),
        'classified_table_byte_count': len(tables),
        'selected_dispatch_destination_count': len(targets),
        'unused_pointer_slots_are_not_execution_targets': True,
        'full_owner_cfg_closed_under_model': True,
        'complete_cfg_guard_has_no_bypass': True,
        'all_selected_targets_are_in_owner_instruction_starts': True,
        'complete_cfg_code_and_tables_disjoint': True,
        'instruction_bytes_form_contiguous_owner_prefix': after['covered'] == set(range(start, start + len(after['covered']))),
        'table_bytes_form_single_contiguous_trailing_suffix': tables == frozenset(range(end - len(tables), end)),
        'pointer_table_precedes_remap': pointer_address + pointer_size <= remap_address,
        'tables_end_at_owner_end': max(tables) + 1 == end,
        'layout_predicates_are_accounting_only': True,
        'reachable_instruction_count': completed['reachable_instruction_count'],
        'reachable_instruction_byte_count': len(after['covered']),
        'reachable_instruction_bytes_sha256': sha_bytes(classified_code),
        'cfg_edge_count': completed['direct_cfg_edge_count'],
        'opaque_returning_call_count': completed['opaque_returning_call_count'],
        'return_instruction_count': completed['return_instruction_count'],
        'unclassified_owner_byte_count': len(remaining),
        'unclassified_owner_bytes_sha256': sha_bytes(unclassified),
        'owner_byte_accounting_complete': True,
        'all_owner_bytes_classified': not remaining,
        'fill_bytes_classified': 0,
    }


def section_metadata(image, pdb, owner_contribution, address, span):
    # Reject ambiguous virtual/raw section mappings without reading the span.
    need(0 <= address < address + span <= 0x100000000, 'section_span_wrap')
    overlaps = [s for s in image.sections if image.image_base+s.rva < address+span and address < image.image_base+s.rva+max(s.virtual_size, s.raw_size)]
    selected = overlaps[0] if len(overlaps) == 1 else None
    virtual = selected is not None and image.image_base+selected.rva <= address and address+span <= image.image_base+selected.rva+selected.virtual_size
    raw = selected is not None and image.image_base+selected.rva <= address and address+span <= image.image_base+selected.rva+selected.raw_size
    other_overlap = any(x is not owner_contribution and x['size'] > 0 and image.image_base+image.section(x['section_number']).rva+x['section_offset'] < address+span and address < image.image_base+image.section(x['section_number']).rva+x['section_offset']+x['size'] for x in pdb['contributions'])
    return {
        'unique_mapped_virtual_section': virtual,
        'raw_span_covered': raw,
        'readable_section': selected is not None and bool(selected.characteristics & 0x40000000),
        'executable_section': selected is not None and bool(selected.characteristics & 0x20000000),
        'disjoint_from_other_pdb_contributions': not other_overlap,
    }



def normal_coff_symbols(data):
    """Read bounded COFF headers, symbol names and section metadata, not code bodies."""
    import struct
    need(type(data) is bytes and 20 <= len(data) <= 64*1024*1024, 'coff_file_bound')
    machine, count, _, symbols_at, symbol_count, optional, _ = struct.unpack_from('<HHIIIHH', data)
    need(machine == 0x14c and optional == 0 and 0 < count <= 4096 and 0 < symbol_count <= 65536, 'normal_coff_header')
    need(20 + count*40 <= symbols_at and symbols_at + symbol_count*18 + 4 <= len(data), 'coff_metadata_extent')
    sections = []
    for index in range(count):
        fields = struct.unpack_from('<8sIIIIIIHHI', data, 20+40*index)
        size, raw, reloc, reloc_count, flags = fields[3], fields[4], fields[5], fields[7], fields[9]
        need((not size or (raw == 0 and flags & 0x80 and not flags & 0x20) or 20+count*40 <= raw and raw+size <= symbols_at) and (not reloc_count or 20+count*40 <= reloc and reloc+10*reloc_count <= symbols_at), 'coff_section_metadata_extent')
        sections.append({'size': size, 'raw_offset': raw, 'flags': flags})
    strings_at = symbols_at + 18*symbol_count
    strings_size = struct.unpack_from('<I', data, strings_at)[0]
    need(4 <= strings_size <= 4*1024*1024 and strings_at+strings_size == len(data), 'coff_string_table_extent')
    symbols = []
    cursor = 0
    while cursor < symbol_count:
        raw, value, section, kind, storage, auxiliary = struct.unpack_from('<8sIhHBB', data, symbols_at+18*cursor)
        need(-2 <= section <= count and cursor+1+auxiliary <= symbol_count, 'coff_symbol_metadata_extent')
        if raw[:4] == b'\0'*4:
            offset = struct.unpack_from('<I', raw, 4)[0]
            need(4 <= offset < strings_size, 'coff_symbol_string_offset')
            first = strings_at+offset
            last = data.find(b'\0', first, min(strings_at+strings_size, first+1025))
            need(last >= first, 'coff_symbol_string_termination')
            name = data[first:last].decode('ascii')
        else:
            name = raw.split(b'\0',1)[0].decode('ascii')
        need(0 < len(name) <= 1024, 'coff_symbol_name_bound')
        symbols.append({'name':name,'value':value,'section':section,'type':kind,'storage':storage})
        cursor += 1+auxiliary
    need(cursor == symbol_count, 'coff_complete_symbol_table')
    return {'sections':sections, 'symbols':symbols}


def helper_absence_metadata(module, image, maps, pdb, build, caller, coff, map_text):
    """Qualified named-owner absence; no claim of an exhaustive named PDB index."""
    need(HELPER_SYMBOL not in map_text and not any(row['symbol'] == HELPER_SYMBOL for row in maps), 'helper_map_symbol_present')
    need(not any(row['name'] == HELPER_SYMBOL for row in coff['symbols']), 'helper_coff_symbol_present')
    expected = module._object_path(build / SPEC['object'])
    producer = [x for x in pdb['modules'] if module._object_basename(x['object']) == SPEC['object'].lower()]
    need(len(producer) == 1 and module._object_path(producer[0]['object']) == expected, 'absence_producer_identity')
    selected = [x for x in pdb['contributions'] if module._object_basename(x['object']) == SPEC['object'].lower() and x['size'] > 0 and x['characteristics'] & module.IMAGE_SCN_CNT_CODE]
    need(0 < len(selected) <= 64 and any(x is caller for x in selected), 'absence_pdb_contribution_set')
    for contribution in selected:
        need(contribution['characteristics'] & module.IMAGE_SCN_MEM_EXECUTE, 'absence_nonexecutable_code_contribution')
        need(module._object_path(contribution['object']) == expected and contribution['module_index'] == producer[0]['index'] and contribution['module'] == producer[0]['module'], 'absence_pdb_producer_identity')
        position = (contribution['section_number'], contribution['section_offset'])
        rows = [x for x in maps if (x['section_number'], x['section_offset']) == position]
        need(len(rows) == 1 and 'f' in rows[0]['flags'] and rows[0]['symbol'] != HELPER_SYMBOL, 'absence_unattributed_or_aliased_pdb_owner')
        row = rows[0]
        need(module._object_basename(row['object']) == SPEC['object'].lower(), 'absence_map_producer')
        if any(mark in str(row['object']) for mark in ('/', '\\', ':')):
            need(module._object_path(row['object']) == expected, 'absence_map_fullpath')
        section = image.section(contribution['section_number'])
        address = image.image_base + section.rva + contribution['section_offset']
        need(row['address'] == address and all(section_metadata(image,pdb,contribution,address,contribution['size']).values()), 'absence_pdb_mapping_or_overlap')
        need(not any(x['section_number'] == contribution['section_number'] and contribution['section_offset'] < x['section_offset'] < contribution['section_offset']+contribution['size'] and 'f' in x['flags'] for x in maps), 'absence_interior_function_symbol')
    caller_symbols = [x for x in coff['symbols'] if x['name'] == SPEC['symbol']]
    need(len(caller_symbols) == 1, 'caller_coff_symbol_unique')
    symbol = caller_symbols[0]
    need(symbol['type'] == 0x20 and symbol['storage'] == 2 and symbol['value'] == 0 and 1 <= symbol['section'] <= len(coff['sections']), 'caller_coff_function_identity')
    section = coff['sections'][symbol['section']-1]
    need(section['flags'] & 0x20001020 == 0x20001020 and section['size'] == caller['size'], 'caller_coff_complete_owner_metadata')
    need(sum(x['section'] == symbol['section'] and x['type'] == 0x20 for x in coff['symbols']) == 1, 'caller_coff_shared_function_section')
    return {'status':'QUALIFIED_EXACT_NAMED_HELPER_OWNER_ABSENT', 'exact_helper_map_symbol_count':0, 'exact_helper_normal_coff_symbol_count':0, 'attributable_exact_helper_map_pdb_owner_count':0, 'same_normal_object_code_contribution_count':len(selected), 'all_same_object_code_contributions_uniquely_map_attributed':True, 'caller_complete_normal_coff_owner_metadata_agrees':True, 'full_named_pdb_symbol_index_parsed':False, 'general_semantic_helper_absence_proved':False, 'corresponding_named_callee_absence_proved':False, 'callee_identity_census_still_required':True, 'callee_bodies_inspected':False, 'caller_and_helper_sizes_not_combined':True, 'exactness_credit':'none'}


def alias_scope_proposal(module, image, maps, aliases, anchors, capture=None, expected_count=203):
    """Validate grammar and propose hashes; unpinned names are not callee identity."""
    import re
    need(type(aliases) is bytes and type(anchors) is bytes and 0 < len(aliases) <= 65536 and 0 < len(anchors) <= 65536, 'alias_source_extent')
    text = aliases.decode('ascii')
    need(text.endswith('\n') and '\r' not in text, 'alias_source_line_endings')
    need(type(expected_count) is int and expected_count in (202, 203), 'alias_expected_count')
    bindings = {}
    for index, line in enumerate(text.splitlines()):
        match = re.fullmatch(r'/alternatename:([A-Za-z0-9_?$@.]{1,240})=(_th10_ltcg_probe_anchor_[0-9]{4})',line)
        need(match is not None, 'alias_line_grammar')
        symbol, anchor = match.groups()
        need(index < 256 and symbol not in bindings and anchor == '_th10_ltcg_probe_anchor_%04d' % index, 'alias_bijection_and_numbering')
        bindings[symbol] = anchor
    need(len(bindings) == expected_count and list(bindings) == sorted(bindings), 'alias_fixed_count_and_order')
    expected = '/* Diagnostic-only, non-runnable unresolved-symbol address anchors. */\n'+''.join('unsigned char '+anchor[1:]+'[4] = {0};\n' for anchor in bindings.values())
    need(anchors.decode('ascii') == expected, 'anchor_source_exact_generator_grammar')
    starts = set()
    for anchor in bindings.values():
        rows = [x for x in maps if x['symbol'] == anchor]
        need(len(rows) == 1, 'anchor_map_identity')
        row = rows[0]
        section = image.section(row['section_number'])
        need('f' not in row['flags'] and module._object_basename(row['object']) == 'link-anchors.obj' and not section.characteristics & module.IMAGE_SCN_MEM_EXECUTE and bool(section.characteristics & 0x40000000), 'anchor_data_object_identity')
        need(row['address'] == image.image_base+section.rva+row['section_offset'] and 0 <= row['section_offset'] and row['section_offset']+4 <= section.virtual_size, 'anchor_metadata_mapping')
        need(row['address'] not in starts, 'anchor_address_alias');starts.add(row['address'])
    if capture is not None:
        capture.update(bindings=bindings)
    return {'status':'GENERATED_ALIAS_HASH_SCOPE_PROPOSAL', 'alias_sha256':sha_bytes(aliases), 'anchor_source_sha256':sha_bytes(anchors), 'binding_count':expected_count, 'fixed_generator_grammar_validated':True, 'numbered_anchor_map_identities_validated':True, 'source_callee_attribution_performed':False, 'requires_independent_exact_source_hash_pin_before_census':True, 'anchor_data_bytes_read':False, 'anchor_is_callable_implementation':False, 'exactness_credit':'none'}


def raw_callee_metadata(module, image, nodes, maps):
    """Only literal pinned-MAP names. Generated source aliases stay unresolved."""
    import re
    from capstone import CS_GRP_CALL
    from capstone.x86 import X86_OP_IMM
    direct = indirect = unresolved = anchored = 0
    counts = {}
    for instruction in nodes.values():
        if not instruction.group(CS_GRP_CALL):continue
        operand = instruction.operands[0]
        if operand.type != X86_OP_IMM:
            indirect += 1;continue
        direct += 1
        rows = [x for x in maps if x['address'] == operand.imm]
        if len(rows) != 1:
            unresolved += 1;continue
        name = rows[0]['symbol']
        if re.fullmatch(r'_th10_ltcg_probe_anchor_[0-9]{4}',name):
            anchored += 1;continue
        row = rows[0]
        section = image.section(row['section_number'])
        good = (re.fullmatch(r'[A-Za-z0-9_?$@.]{1,240}',name) is not None and sum(other['symbol'] == name for other in maps) == 1 and 'f' in row['flags'] and section.characteristics & (module.IMAGE_SCN_CNT_CODE | module.IMAGE_SCN_MEM_EXECUTE) == (module.IMAGE_SCN_CNT_CODE | module.IMAGE_SCN_MEM_EXECUTE) and row['address'] == image.image_base+section.rva+row['section_offset'] and 0 <= row['section_offset'] < min(section.virtual_size,section.raw_size))
        if not good:
            unresolved += 1;continue
        counts[name] = counts.get(name,0)+1
    need(direct+indirect <= 64 and len(counts) <= 32, 'callee_census_bound')
    return {'status':'PARTIAL_LITERAL_MAP_CENSUS', 'direct_call_instruction_count':direct, 'indirect_call_instruction_count':indirect, 'unresolved_or_aliased_direct_call_count':unresolved, 'generated_anchor_direct_call_count':anchored, 'literal_map_symbol_call_counts_unordered':dict(sorted(counts.items())), 'anchor_source_names_resolved':False, 'no_call_order_or_sites_exported':True, 'callee_bodies_inspected':False, 'exactness_credit':'none'}


def pinned_callee_census(module, image, nodes, maps, aliases, anchors):
    """Unordered source identities, never source bodies or dynamic call claims."""
    import re
    from capstone import CS_GRP_CALL
    from capstone.x86 import X86_OP_IMM
    need(sha_bytes(aliases) == ALIAS_PINS['link-aliases.rsp'] and sha_bytes(anchors) == ALIAS_PINS['link-anchors.c'], 'generated_alias_source_pin')
    parsed = {}
    alias_scope_proposal(module, image, maps, aliases, anchors, parsed)
    bindings = parsed['bindings']
    by_anchor = {anchor: symbol for symbol, anchor in bindings.items()}
    need(not set(bindings) & set(by_anchor), 'alias_source_anchor_name_collision')
    anchor_rows = {row['symbol']: row for row in maps if row['symbol'] in by_anchor}
    anchor_ranges = sorted((row['address'], row['address']+4) for row in anchor_rows.values())
    need(all(left[1] <= right[0] for left, right in zip(anchor_ranges, anchor_ranges[1:])), 'anchor_virtual_extent_overlap')
    direct = indirect = unresolved = 0
    literal_counts, generated_counts = {}, {}
    for instruction in nodes.values():
        if not instruction.group(CS_GRP_CALL):
            continue
        operand = instruction.operands[0]
        if operand.type != X86_OP_IMM:
            indirect += 1
            continue
        direct += 1
        rows = [row for row in maps if row['address'] == operand.imm]
        named_anchors = [row for row in rows if row['symbol'] in by_anchor]
        if named_anchors:
            # A generated alias is accepted only as the exact declared pair.
            # Anchor-only, third-name, duplicate and contradictory rows remain
            # unresolved, even if one row superficially looks usable.
            anchor = named_anchors[0]
            source = by_anchor[anchor['symbol']]
            good = (len(named_anchors) == 1 and len(rows) == 2
                    and {row['symbol'] for row in rows} == {source, anchor['symbol']}
                    and sum(row['symbol'] == source for row in maps) == 1
                    and all(all(row[key] == anchor[key] for key in ('section_number', 'section_offset', 'flags', 'object')) for row in rows))
            if good:
                generated_counts[source] = generated_counts.get(source, 0) + 1
            else:
                unresolved += 1
            continue
        if len(rows) != 1:
            unresolved += 1
            continue
        row = rows[0]
        name = row['symbol']
        section = image.section(row['section_number'])
        good = (re.fullmatch(r'[A-Za-z0-9_?$@.]{1,240}', name) is not None
                and sum(other['symbol'] == name for other in maps) == 1
                and 'f' in row['flags']
                and section.characteristics & (module.IMAGE_SCN_CNT_CODE | module.IMAGE_SCN_MEM_EXECUTE) == (module.IMAGE_SCN_CNT_CODE | module.IMAGE_SCN_MEM_EXECUTE)
                and row['address'] == image.image_base+section.rva+row['section_offset']
                and 0 <= row['section_offset'] < min(section.virtual_size, section.raw_size)
                and not any(begin <= row['address'] < end for begin, end in anchor_ranges))
        if not good:
            unresolved += 1
            continue
        literal_counts[name] = literal_counts.get(name, 0) + 1
    need(direct+indirect <= 64 and len(literal_counts)+len(generated_counts) <= 32, 'callee_census_bound')
    need(not set(literal_counts) & set(generated_counts), 'mixed_literal_and_generated_identity')
    resolved = sum(literal_counts.values()) + sum(generated_counts.values())
    need(resolved+unresolved == direct, 'callee_census_accounting')
    complete = not unresolved and not indirect
    helper_count = literal_counts.get(HELPER_SYMBOL, 0) + generated_counts.get(HELPER_SYMBOL, 0)
    return {
        'status': 'COMPLETE_BOUNDED_NAMED_DIRECT_CALLEE_CENSUS' if complete else 'PARTIAL_BOUNDED_NAMED_DIRECT_CALLEE_CENSUS',
        'direct_call_instruction_count': direct,
        'indirect_call_instruction_count': indirect,
        'resolved_direct_call_instruction_count': resolved,
        'unresolved_direct_call_instruction_count': unresolved,
        'literal_map_symbol_call_counts_unordered': dict(sorted(literal_counts.items())),
        'generated_alias_source_symbol_call_counts_unordered': dict(sorted(generated_counts.items())),
        'generated_alias_direct_call_count': sum(generated_counts.values()),
        'all_recovered_calls_direct_and_named': complete,
        'exact_named_helper_recovered_direct_call_count': helper_count if complete else None,
        'exact_named_helper_absent_from_recovered_calls': complete and helper_count == 0,
        'all_generated_alias_source_hashes_pinned': True,
        'accepted_generated_calls_require_exact_declared_map_pairs': True,
        'generated_anchor_is_callable_implementation': False,
        'anchor_data_bytes_read': False,
        'callee_bodies_inspected': False,
        'no_call_order_or_sites_exported': True,
        'dynamic_call_behavior_proved': False,
        'exactness_credit': 'none',
    }


def gap_metadata(start, size, closed, result):
    """Bounded proposal only. Do not decode or inspect any leftover contents."""
    nodes, edges = closed['nodes'], closed['edges']
    covered, tables, remaining = closed['covered'], closed['tables'], closed['remaining']
    owner = set(range(start, start+size))
    need(result.get('full_owner_cfg_closed_under_model') and result.get('complete_cfg_guard_has_no_bypass'), 'gap_requires_closed_cfg')
    need(covered | tables | remaining == owner and not covered & tables and not covered & remaining and not tables & remaining, 'gap_partition')
    need(0 < len(remaining) <= 3 and result['unclassified_owner_byte_count'] == len(remaining), 'gap_extent_bound')
    need(set(nodes) == set(edges) and all(target in nodes for targets in edges.values() for target in targets), 'gap_closed_successor_set')
    need(covered == {at for instruction in nodes.values() for at in range(instruction.address, instruction.address+instruction.size)}, 'gap_instruction_coverage')
    begin, end = min(remaining), max(remaining)+1
    contiguous = remaining == set(range(begin, end))
    predecessors = [instruction for instruction in nodes.values() if instruction.address+instruction.size == begin]
    terminating_predecessor = len(predecessors) == 1 and predecessors[0].mnemonic in ('ret', 'jmp') and begin not in edges[predecessors[0].address]
    return {
        'status': 'BOUNDED_UNCLASSIFIED_GAP_METADATA',
        'byte_count': len(remaining),
        'sha256': result['unclassified_owner_bytes_sha256'],
        'single_contiguous_span': contiguous,
        'between_contiguous_instruction_prefix_and_table_suffix': contiguous and covered == set(range(start, begin)) and tables == frozenset(range(end, start+size)),
        'preceded_by_recovered_unconditional_control_terminator': terminating_predecessor,
        'no_recovered_cfg_edge_enters_span': not any(target in remaining for targets in edges.values() for target in targets),
        'wholly_inside_complete_owner': remaining <= owner,
        'contents_interpreted': False,
        'new_image_read_range_opened': False,
        'fill_classification_performed': False,
        'remains_unclassified': True,
        'exactness_credit': 'none',
    }


CONFIGS = {'a006': {'run': '.analysis/bullet1454-native-baseline-20261018-a006', 'artifacts': {'source.exe': '0c088ba2713a559cccacbe9c94bec8da385226952c8bb11a2cc5a52f561b93e4', 'source.map': 'f1cae3e21b663a7c6dc852bb60ccbbbde911ed39f0b989cf6b847dc55eac460e', 'source.pdb': 'dbcc5b40358bf070e923150b55bb31043142eca1cdea0a5be2ad8d64de1c4a66', 'source.ltcg.obj': 'b449f121e347497494198692fb5596a435a44871cffa6442f444de23b908ba35', 'support-04-EnemyEclDispatcher.ltcg.obj': '50d6d1e4f24b3807a1554993c2ae1d7447a3db61d10ece19b391fe7ed0815f55', 'support-01-BulletTransform.normal.obj': 'b9799dc2fbf84b15869d0758bc656f650439154a7149a01b2741b5eb89677046'}, 'static': {'manifest.json': '62c65f42d5d3b62803025712b41012f15576a86e81bee8bdb82c211567840328', 'runner.py': '3b839cd2ea2421b73b0b9ff98663924894f73aa196a56e21df3c8f298d51bee6', 'candidate/scripts/linked_image.py': '58583c6241e0979288b0c4ce0a0e55f1ae2a178762d93743475d05518eff0354', 'candidate/config/tools.lock.toml': '24d77ab218e86ea8b888418922a31db9a3cf30f54bb73450a04967a4a501db9f', 'candidate/src/BulletTransform.cpp': 'e698e713a6f3f7e069970090e09c515ce9d64b3011bdfdd73ca046ea82b65f9e', 'candidate/src/BulletRuntime.cpp': 'fd80dbabfa90524f4300eeaf7347e59febe5710589192416da2a3a71b6ac2def', 'candidate/src/EnemySoundQueue.hpp': '1fdc23149fca106bd00413a7acbdb3bc39849364a1ff0807e41c66d1365574e8', 'candidate/src/EnemyEclDispatcher.cpp': '9fae2594e13b7e399e7a7b27a3117495f4a54afd08907df9dcc668d5b00a1e30'}, 'owners': {'runtime_guard': {'spec': {'object': 'source.ltcg.obj', 'symbol': '?BulletUpdateRuntime@@YGHPAUBulletRuntimeView@@@Z', 'target_address': 4219456, 'target_size': 885}, 'size': 885, 'sha256': 'd1fe8601b9f3e2d742b5f35a99e88e63a5d7096cb23a28dc92830c1aece47853'}}, 'alias_count': 203, 'alias_pins': {'link-aliases.rsp': '7af855ebef42efc8d424897afd358bcfff13b212aacdec1f4f9339920ad30e56', 'link-anchors.c': '97d1508d5b5d2d5e0511e73e05adb49178e3424d162f13343896db92c3f2561b'}, 'advance_producer': 'support-01-BulletTransform.normal.obj'}, 'a008': {'run': '.analysis/bullet1454-native-baseline-20261018-a008', 'artifacts': {'source.exe': '80f99df4560bf675be3819fd94d171f5b3f0fe4aca65763c96007756cd95181e', 'source.map': '200e4fe739632f6ee303de22fdf7c3757ffa20aaff41b59e69937d52e5864c9c', 'source.pdb': '6adc835248ba699c0ab62ff5808a052222859eb8d8da6fe8f1506e03187dac38', 'source.ltcg.obj': '4d698796e91e88700eb7dd302c8655958689d5f683dc4396ac175d7ccc66f7d0', 'support-01-BulletTransform.ltcg.obj': 'be3d9ebef81edcc064f3a516f949aaead9f3a2d2867040c2eaa60254c9e706de', 'support-04-EnemyEclDispatcher.ltcg.obj': 'bfe86004f42021b0bfea727606fc2ff1b731118aadf1aa268eca6e8fa9593794'}, 'static': {'manifest.json': '0bf27bd36cf55044bcd12a38f00fa3b3b3698654ebba3532aed95121135114d8', 'runner.py': 'cb12e386279dcd9b35dddc44cd4ae814759c15ea51cb84ee4be417d70d610700', 'candidate/scripts/linked_image.py': '58583c6241e0979288b0c4ce0a0e55f1ae2a178762d93743475d05518eff0354', 'candidate/config/tools.lock.toml': '24d77ab218e86ea8b888418922a31db9a3cf30f54bb73450a04967a4a501db9f', 'candidate/src/BulletTransform.cpp': '824eb6a3a14fe140168ed1146f4aff7ece5bbc914639934ae35850f0ecdacccc', 'candidate/src/BulletRuntime.cpp': 'fd80dbabfa90524f4300eeaf7347e59febe5710589192416da2a3a71b6ac2def', 'candidate/src/EnemySoundQueue.hpp': '1fdc23149fca106bd00413a7acbdb3bc39849364a1ff0807e41c66d1365574e8', 'candidate/src/EnemyEclDispatcher.cpp': '9fae2594e13b7e399e7a7b27a3117495f4a54afd08907df9dcc668d5b00a1e30'}, 'owners': {'runtime_guard': {'spec': {'object': 'source.ltcg.obj', 'symbol': '?BulletUpdateRuntime@@YGHPAUBulletRuntimeView@@@Z', 'target_address': 4219456, 'target_size': 885}, 'size': 884, 'sha256': 'e9c0048eae824cd8bacc047c7160c3cea92f573f202625f0287f6f17a725b132'}, 'transform': {'spec': {'object': 'support-01-BulletTransform.ltcg.obj', 'symbol': '?AdvanceTransformProgram@BulletRuntimeView@@QAEXXZ', 'target_address': 4222352, 'target_size': 1454}, 'size': 1612, 'sha256': '5c5f98eb1bd41a6b3c591b3f012d7a5926277c4e436004454649b85e61cc1463'}}, 'alias_count': 202, 'advance_producer': 'support-01-BulletTransform.ltcg.obj', 'conditional_alias_pins': {'link-aliases.rsp': '7782bdc5b0240576f7342184f983353dd0c5b855e72f4c6fc67c8773a1db44bd', 'link-anchors.c': 'adf2976791eb8b9558ac347e9d22b5f881adca62967a762d573651303e992119'}, 'alias_pins': {}}}


def selected_owner(module, image, maps, pdb, build, owner):
    """All complete-owner attribution, expected extent and mapping gates precede slicing."""
    spec = owner['spec']
    gates = predicates(module, image, maps, pdb, build, spec, metadata_only=True)
    expected = {'basename_prefilter_unique_module','prefilter_module_exact_path','unique_requested_symbol','unique_same_start_symbol','unique_same_start_contribution','positive_contribution_size','contribution_exact_object_path','contribution_module_index_equal','contribution_module_name_equal','map_object_basename_equal','map_object_fullpath_if_present','function_symbol_flag','contribution_code_executable','section_code_executable','virtual_extent_covered','no_interior_function_symbol','map_address_agrees','no_overlapping_contribution','raw_extent_covered'}
    need(set(gates) == expected | {'all_module_object_paths_normalizable'} and all(gates[key] for key in expected), 'whole_owner_attribution_metadata')
    row = next(row for row in maps if row['symbol'] == spec['symbol'])
    contribution = next(item for item in pdb['contributions'] if (item['section_number'], item['section_offset']) == (row['section_number'], row['section_offset']))
    need(type(owner['size']) is int and 0 < owner['size'] <= OWNER_SIZE and contribution['size'] == owner['size'], 'authorized_whole_owner_size')
    need(all(section_metadata(image, pdb, contribution, row['address'], owner['size']).values()), 'whole_owner_mapping_metadata')
    code = image.read_address(row['address'], owner['size'])
    need(type(code) is bytes and len(code) == owner['size'] and sha_bytes(code) == owner['sha256'], 'authorized_whole_owner_hash')
    return row['address'], code, contribution


def aggregate_control_counts(nodes):
    from capstone import CS_GRP_CALL, CS_GRP_JUMP, CS_GRP_RET
    return {
        'call_instruction_count': sum(instruction.group(CS_GRP_CALL) for instruction in nodes.values()),
        'return_instruction_count': sum(instruction.group(CS_GRP_RET) for instruction in nodes.values()),
        'conditional_branch_instruction_count': sum(instruction.group(CS_GRP_JUMP) and instruction.mnemonic in JCC for instruction in nodes.values()),
        'unconditional_branch_instruction_count': sum(instruction.group(CS_GRP_JUMP) and instruction.mnemonic == 'jmp' for instruction in nodes.values()),
        'instruction_order_exported': False,
    }


def diagnose_owner(md, role, code, start, section_info):
    closed = {}
    if role == 'runtime_guard':
        result = preflight(md, code, start, section_info, closed, propose_indirect=False)
        remaining = set(range(start, start+len(code))) - closed['covered']
        result.update({
            'reachable_instruction_bytes_sha256': sha_bytes(bytes(code[at-start] for at in sorted(closed['covered']))),
            'unclassified_owner_bytes_sha256': sha_bytes(bytes(code[at-start] for at in sorted(remaining))),
            'owner_byte_accounting_complete': len(closed['covered']) + len(remaining) == len(code),
            'classified_table_byte_count': 0,
            'fill_bytes_classified': 0,
            'runtime_auxiliary_table_reads_permitted': False,
        })
    else:
        need(role == 'transform', 'owner_role_scope')
        def read_table(address, span):
            need(start <= address and address+span <= start+len(code), 'table_read_owner_extent')
            return code[address-start:address-start+span]
        result = close_owner(md, code, start, section_info, read_table, require_native_preflight=True, capture=closed)
    if result.get('full_owner_cfg_closed_under_model'):
        result['aggregate_control_counts'] = aggregate_control_counts(closed['nodes'])
    return result, closed


CUE_SYMBOL = '?QueueSoundCue@EnemySoundQueueView@@QAEXHM@Z'
SOUND_OWNER_SYMBOL = '?g_MainSoundOwner@@3PAEA'
CUE_PRODUCER = 'support-04-EnemyEclDispatcher.ltcg.obj'


def cue_corridor(nodes, edges, call):
    from capstone import CS_GRP_CALL, CS_GRP_JUMP, CS_GRP_RET
    need(set(nodes) == set(edges) and call.address in nodes, 'cue_closed_node_set')
    predecessors = {address: set() for address in nodes}
    for address, targets in edges.items():
        for target in targets:
            need(target in nodes, 'cue_closed_edge_set')
            predecessors[target].add(address)
    predecessors[min(nodes)].add(None)
    current, back, seen = call.address, [], set()
    while True:
        previous = predecessors[current]
        if None in previous:
            stop = 'owner_entry_boundary';break
        if len(previous) != 1:
            stop = 'merge_or_missing_predecessor';break
        address = next(iter(previous))
        need(address not in seen, 'cue_predecessor_cycle');seen.add(address)
        instruction = nodes[address]
        if instruction.group(CS_GRP_CALL):
            stop = 'opaque_call_boundary';break
        if instruction.group(CS_GRP_JUMP) or instruction.group(CS_GRP_RET):
            stop = 'control_boundary';break
        need(instruction.address+instruction.size == current and edges[address] == [current], 'cue_noncontiguous_predecessor')
        if len(back) == 8:
            stop = 'eight_instruction_budget';break
        back.append(instruction);current = address
    return list(reversed(back)), stop


def cue_metadata(md, corridor, stop, successor):
    from capstone import CS_GRP_JUMP, CS_GRP_RET
    from capstone.x86 import X86_OP_REG, X86_OP_IMM
    counts = {}
    for instruction in corridor:
        family = instruction.mnemonic if instruction.mnemonic in ('mov','lea','push','pop','add','sub','xor','fld','fstp') else 'other_ordinary'
        counts[family] = counts.get(family, 0)+1
    following = 'other'
    if successor.mnemonic == 'add' and not any(successor.prefix) and len(successor.operands) == 2:
        left,right = successor.operands
        if left.type == X86_OP_REG and left.size == 4 and md.reg_name(left.reg) == 'esp' and right.type == X86_OP_IMM and right.size == 4:
            following = 'stack_add_4' if right.imm == 4 else 'stack_add_8' if right.imm == 8 else 'other_stack_add'
    elif successor.mnemonic == 'pop':following = 'pop_shape'
    elif successor.group(CS_GRP_JUMP):following = 'branch_shape'
    elif successor.group(CS_GRP_RET):following = 'return_shape'
    return {'preparation_instruction_count':len(corridor), 'predecessor_families_unordered':dict(sorted(counts.items())), 'stopping_category':stop, 'immediate_successor_syntactic_category':following, 'successor_is_not_cleanup_proof':True, 'instruction_order_exported':False}


def model_cue_corridor(md, corridor, stop, receiver_address):
    """Fixed conditional caller predicates; never formal callee consumption or ABI."""
    from capstone.x86 import X86_OP_REG, X86_OP_IMM, X86_OP_MEM
    need(corridor and stop != 'eight_instruction_budget', 'cue_preparation_budget_or_empty')
    names = {'eax','ebx','ecx','edx','esi','edi','ebp'}
    registers = {name:('incoming',name) for name in names}
    stack, floating = {}, []
    delta = 0
    def register(operand):
        need(operand.type == X86_OP_REG and operand.size == 4 and md.reg_name(operand.reg) in names, 'cue_model_gpr')
        return md.reg_name(operand.reg)
    def memory(operand):
        need(operand.type == X86_OP_MEM and operand.size == 4 and not operand.mem.segment and not operand.mem.index and operand.mem.base, 'cue_model_memory_form')
        base = md.reg_name(operand.mem.base)
        need(base in names | {'esp'}, 'cue_model_memory_base')
        return base, operand.mem.disp
    def source(operand):
        need(operand.size == 4, 'cue_model_source_width')
        if operand.type == X86_OP_IMM:
            need(operand.access == 0, 'cue_model_immediate_access')
            return ('constant',operand.imm & 0xffffffff), set()
        if operand.type == X86_OP_REG:
            name = register(operand);need(operand.access == 1, 'cue_model_register_read')
            return registers[name], {name}
        need(operand.access == 1, 'cue_model_memory_read')
        base, displacement = memory(operand)
        if base == 'esp':
            need(displacement % 4 == 0 and -32 <= delta+displacement <= 32, 'cue_model_stack_read_bound')
            return stack.get(delta+displacement, ('unknown_stack',delta+displacement)), {'esp'}
        return ('load32',registers[base],displacement), {base}
    def stack_destination(operand):
        base, displacement = memory(operand)
        need(base == 'esp' and displacement in (0,4,8,12) and delta+displacement in stack, 'cue_model_private_stack_destination')
        return delta+displacement
    for instruction in corridor:
        operands = instruction.operands
        need(not any(instruction.prefix) and instruction.addr_size == 4 and instruction.bytes[0] not in (0xf0,0xf2,0xf3,0x66,0x67,0x26,0x2e,0x36,0x3e,0x64,0x65), 'cue_model_prefix')
        reads, writes = ({md.reg_name(reg) for reg in values} for values in instruction.regs_access())
        expected_reads, expected_writes = set(), set()
        if instruction.mnemonic == 'mov':
            need(len(operands) == 2 and operands[0].access == 2, 'cue_model_mov_shape')
            value, expected_reads = source(operands[1])
            if operands[0].type == X86_OP_REG:
                destination = register(operands[0]);registers[destination] = value;expected_writes = {destination}
            else:
                destination = stack_destination(operands[0]);stack[destination] = value;expected_reads |= {'esp'}
        elif instruction.mnemonic == 'lea':
            need(len(operands) == 2 and operands[0].access == 2 and operands[1].type == X86_OP_MEM and operands[1].size == 4 and operands[1].access == 1 and not operands[1].mem.segment and not operands[1].mem.index, 'cue_model_lea_shape')
            destination = register(operands[0]);mem = operands[1].mem
            if mem.base:
                base = md.reg_name(mem.base);need(base in names, 'cue_model_lea_base');expected_reads = {base}
                value = registers[base] if mem.disp == 0 else ('address_sum',registers[base],mem.disp)
            else:value = ('constant',mem.disp & 0xffffffff)
            registers[destination] = value;expected_writes = {destination}
        elif instruction.mnemonic == 'push':
            need(len(operands) == 1, 'cue_model_push_shape')
            value, expected_reads = source(operands[0]);expected_reads |= {'esp'};expected_writes = {'esp'}
            delta -= 4;need(delta >= -32, 'cue_model_stack_bound');stack[delta] = value
        elif instruction.mnemonic in ('add','sub'):
            need(len(operands) == 2 and operands[0].type == X86_OP_REG and operands[0].size == 4 and operands[0].access == 3 and md.reg_name(operands[0].reg) == 'esp' and operands[1].type == X86_OP_IMM and operands[1].size == 4 and operands[1].access == 0 and operands[1].imm in (4,8,12,16), 'cue_model_stack_adjust_shape')
            amount = operands[1].imm
            if instruction.mnemonic == 'sub':
                for offset in range(delta-amount,delta,4):stack[offset] = ('reserved_unknown',)
                delta -= amount
            else:
                for offset in range(delta,delta+amount,4):stack.pop(offset,None)
                delta += amount
            need(-32 <= delta <= 32, 'cue_model_stack_bound');expected_reads = {'esp'};expected_writes = {'esp','eflags'}
        elif instruction.mnemonic == 'xor':
            need(len(operands) == 2 and operands[0].access == 3 and operands[1].access == 1 and register(operands[0]) == register(operands[1]), 'cue_model_xor_shape')
            name = register(operands[0]);registers[name] = ('constant',0);expected_reads = {name};expected_writes = {name,'eflags'}
        elif instruction.mnemonic == 'fld':
            need(len(operands) == 1 and operands[0].type == X86_OP_MEM, 'cue_model_fld_shape')
            value, expected_reads = source(operands[0]);floating.append(('f32_read_value',value));expected_writes = {'fpsw'}
        elif instruction.mnemonic == 'fstp':
            # Pinned Capstone5.0.6 reports this memory operand access as READ.
            # Its explicit FSTP shape is modeled as a private-frame write only.
            need(len(operands) == 1 and operands[0].access == 1 and floating, 'cue_model_fstp_shape')
            stack[stack_destination(operands[0])] = floating.pop();expected_reads = {'esp'};expected_writes = {'fpsw'}
        elif instruction.mnemonic == 'nop':
            need(not operands, 'cue_model_nop_shape')
        else:
            raise GateError('cue_model_unsupported_preparation')
        need(reads == expected_reads and writes == expected_writes, 'cue_model_effect_mismatch')
    need(not floating, 'cue_model_unconsumed_x87_value')
    words = []
    for offset in range(delta,delta+16,4):
        if offset not in stack:break
        words.append(stack[offset])
    need(1 <= len(words) <= 2, 'cue_model_prepared_word_count')
    def field(value):
        if value[0] == 'f32_read_value':value = value[1]
        return len(value) == 3 and value[0] == 'load32' and value[1][0] == 'incoming' and value[2] == 0x3b4
    receiver_ecx = registers['ecx'] == ('constant',receiver_address)
    receiver_esi = registers['esi'] == ('constant',receiver_address)
    id_ebx = registers['ebx'] == ('constant',28)
    one_field = len(words) == 1 and field(words[0])
    two_id_field = len(words) == 2 and words[0] == ('constant',28) and field(words[1])
    return {
        'status':'FIXED_CONDITIONAL_CALLER_CARRIER_PREDICATES',
        'modeled_preparation_instruction_count':len(corridor),
        'prepared_top_stack_word_count':len(words),
        'ecx_is_sound_owner_diagnostic_symbol_address':receiver_ecx,
        'esi_is_sound_owner_diagnostic_symbol_address':receiver_esi,
        'ebx_is_source_sound_id_28':id_ebx,
        'one_prepared_word_originates_from_incoming_base_field_3b4':one_field,
        'two_prepared_words_are_id28_then_incoming_base_field_3b4':two_id_field,
        'conditional_esi_ebx_one_word_pattern':receiver_esi and id_ebx and one_field,
        'conditional_ecx_two_word_pattern':receiver_ecx and two_id_field,
        'bullet_identity_of_incoming_base_proved':False,
        'floating_bitwise_equivalence_proved':False,
        'callee_consumption_or_stack_cleanup_proved':False,
        'real_sound_owner_storage_or_runtime_semantics_proved':False,
        'abi_or_causal_mechanism_proved':False,
    }


def cue_preparation(module, image, maps, pdb, build, md, closed, aliases, anchors, config):
    from capstone import CS_GRP_CALL
    from capstone.x86 import X86_OP_IMM
    spec = {'symbol':CUE_SYMBOL,'object':CUE_PRODUCER}
    gates = predicates(module,image,maps,pdb,build,spec,metadata_only=True)
    need(all(value for key,value in gates.items() if key != 'all_module_object_paths_normalizable'), 'cue_callee_metadata_identity')
    rows = [row for row in maps if row['symbol'] == CUE_SYMBOL]
    need(len(rows) == 1, 'cue_callee_unique_symbol')
    calls = [instruction for instruction in closed['nodes'].values() if instruction.group(CS_GRP_CALL) and instruction.operands[0].type == X86_OP_IMM and instruction.operands[0].imm == rows[0]['address']]
    need(len(calls) == 1, 'cue_unique_recovered_direct_call')
    call = calls[0]
    corridor, stop = cue_corridor(closed['nodes'],closed['edges'],call)
    need(closed['edges'][call.address] == [call.address+call.size] and call.address+call.size in closed['nodes'], 'cue_normal_return_successor')
    result = {'status':'BOUNDED_CUE_CALLER_PREPARATION', 'unique_cue_callee_map_pdb_producer_identity':True, 'unique_recovered_direct_call':True, 'metadata':cue_metadata(md,corridor,stop,closed['nodes'][call.address+call.size]), 'callee_body_bytes_read':False, 'anchor_data_bytes_read':False, 'new_owner_ranges_opened':False, 'exactness_credit':'none'}
    try:
        need(config['alias_pins'].get('link-aliases.rsp') == sha_bytes(aliases) and config['alias_pins'].get('link-anchors.c') == sha_bytes(anchors), 'cue_receiver_alias_hash_pin')
        parsed = {};alias_scope_proposal(module,image,maps,aliases,anchors,parsed,expected_count=config['alias_count'])
        need(SOUND_OWNER_SYMBOL in parsed['bindings'], 'cue_receiver_source_binding')
        anchor_name = parsed['bindings'][SOUND_OWNER_SYMBOL]
        anchor_rows = [row for row in maps if row['symbol'] == anchor_name]
        need(len(anchor_rows) == 1, 'cue_receiver_anchor_unique');anchor = anchor_rows[0]
        paired = [row for row in maps if row['address'] == anchor['address']]
        need(len(paired) == 2 and {row['symbol'] for row in paired} == {SOUND_OWNER_SYMBOL,anchor_name} and sum(row['symbol'] == SOUND_OWNER_SYMBOL for row in maps) == 1, 'cue_receiver_exact_declared_pair')
        need(all(all(row[key] == anchor[key] for key in ('section_number','section_offset','flags','object')) for row in paired), 'cue_receiver_pair_metadata')
        result['fixed_predicates'] = model_cue_corridor(md,corridor,stop,anchor['address'])
        result['receiver_identity_is_diagnostic_anchor_only'] = True
    except GateError as exc:
        result['fixed_predicates'] = {'status':'UNRESOLVED', 'category':str(exc), 'fixed_caller_carrier_predicates_proved':False}
    result['conditional_premises'] = ['closed immutable caller and ordinary flat32 execution; opaque calls normally return', 'valid nonwrapping private stack disjoint from all input memory and diagnostic symbol storage', 'stable readable source memory; valid x87 stack with sufficient free capacity for every modeled FLD and no stack faults; ordinary nontrapping floating processing within the corridor', 'field3b4 is bullet X only if its corridor-entry base denotes the intended bullet; that identity is not proved']
    result['limits'] = ['no callee body, formal argument consumption, return cleanup, or callee clobber proof', 'diagnostic anchor identity is not real sound storage or runnable behavior', 'fixed caller patterns do not establish an ABI or explain complete owner differences', 'no raw bytes, addresses, register values, ordered instructions or traces exported']
    return result


ADVANCE_SYMBOL = '?AdvanceTransformProgram@BulletRuntimeView@@QAEXXZ'


def advance_transfer(md, instruction):
    """Recognize only complete, non-stack dword GPR copies; unknown effects fail."""
    from capstone.x86 import X86_OP_REG, X86_OP_MEM
    names = GPR - {'esp'}
    need(not any(instruction.prefix) and instruction.addr_size == 4 and instruction.bytes[0] not in (0xf0,0xf2,0xf3,0x66,0x67,0x26,0x2e,0x36,0x3e,0x64,0x65), 'advance_transfer_prefix')
    operands = instruction.operands
    reads, writes = ({md.reg_name(reg) for reg in values} for values in instruction.regs_access())
    if instruction.mnemonic == 'nop':
        need(not operands and reads == writes == set(), 'advance_nop_effect')
        return None
    need(len(operands) == 2 and operands[0].type == X86_OP_REG and operands[0].size == 4 and operands[0].access == 2, 'advance_transfer_destination')
    destination = md.reg_name(operands[0].reg)
    need(destination in names, 'advance_transfer_destination_register')
    if instruction.mnemonic == 'mov':
        need(operands[1].type == X86_OP_REG and operands[1].size == 4 and operands[1].access == 1, 'advance_mov_source')
        source = md.reg_name(operands[1].reg)
    elif instruction.mnemonic == 'lea':
        need(operands[1].type == X86_OP_MEM and operands[1].size == 4 and operands[1].access == 1, 'advance_lea_source')
        memory = operands[1].mem
        need(memory.base and not memory.index and not memory.segment and memory.disp == 0, 'advance_lea_zero_offset_copy')
        source = md.reg_name(memory.base)
    else:
        raise GateError('advance_transfer_unsupported_instruction')
    need(source in names and reads == {source} and writes == {destination}, 'advance_transfer_effect')
    return destination, source


def advance_transfer_metadata(md, corridor):
    counts, other = {}, 0
    for instruction in corridor:
        try:
            pair = advance_transfer(md,instruction)
            if pair is not None:
                key = pair[0]+'_from_'+pair[1]
                counts[key] = counts.get(key,0)+1
        except GateError:
            other += 1
    return {'recognized_register_copy_counts_unordered':dict(sorted(counts.items())), 'instructions_outside_copy_grammar':other, 'copy_counts_do_not_prove_live_values':True}


def model_advance_corridor(md, corridor, stop):
    need(corridor and stop != 'eight_instruction_budget', 'advance_preparation_budget_or_empty')
    names = GPR - {'esp'}
    origins = {name:name for name in names}
    for instruction in corridor:
        pair = advance_transfer(md,instruction)
        if pair is not None:origins[pair[0]] = origins[pair[1]]
    return {
        'status':'FIXED_CONDITIONAL_CORRIDOR_ENTRY_CARRIER_PREDICATES',
        'modeled_preparation_instruction_count':len(corridor),
        'ecx_equals_corridor_entry_esi':origins['ecx'] == 'esi',
        'eax_equals_corridor_entry_esi':origins['eax'] == 'esi',
        'esi_equals_corridor_entry_esi':origins['esi'] == 'esi',
        'ecx_equals_corridor_entry_edi':origins['ecx'] == 'edi',
        'eax_equals_corridor_entry_edi':origins['eax'] == 'edi',
        'no_stack_effect_within_modeled_corridor':True,
        'corridor_entry_is_not_function_entry':True,
        'bullet_identity_or_formal_parameter_role_proved':False,
        'callee_consumption_or_stack_cleanup_proved':False,
        'abi_or_complete_owner_difference_cause_proved':False,
    }


def model_advance_stack_corridor(md, corridor, stop):
    """Exactly two merge-delimited instructions; private-stack GPR pushes only."""
    from capstone.x86 import X86_OP_REG
    need(len(corridor) == 2 and stop == 'merge_or_missing_predecessor', 'advance_exact_two_instruction_merge_corridor')
    names = GPR - {'esp'}
    origins = {name:name for name in names}
    pushed = []
    for instruction in corridor:
        if instruction.mnemonic != 'push':
            pair = advance_transfer(md,instruction)
            if pair is not None:origins[pair[0]] = origins[pair[1]]
            continue
        need(not any(instruction.prefix) and instruction.addr_size == 4 and instruction.bytes[0] not in (0xf0,0xf2,0xf3,0x66,0x67,0x26,0x2e,0x36,0x3e,0x64,0x65), 'advance_push_prefix')
        operands = instruction.operands
        need(len(operands) == 1 and operands[0].type == X86_OP_REG and operands[0].size == 4 and operands[0].access == 1, 'advance_push_dword_register_shape')
        name = md.reg_name(operands[0].reg)
        need(name in names, 'advance_push_non_esp_register')
        reads,writes = ({md.reg_name(reg) for reg in values} for values in instruction.regs_access())
        need(reads == {name,'esp'} and writes == {'esp'}, 'advance_push_exact_effect')
        pushed.append(origins[name])
    need(1 <= len(pushed) <= 2, 'advance_protected_stack_word_count')
    words = list(reversed(pushed))
    return {
        'status':'FIXED_CONDITIONAL_TWO_INSTRUCTION_PRIVATE_STACK_PREDICATES',
        'modeled_preparation_instruction_count':2,
        'prepared_top_stack_word_count':len(words),
        'ecx_equals_corridor_entry_ebp':origins['ecx'] == 'ebp',
        'ecx_equals_corridor_entry_esi':origins['ecx'] == 'esi',
        'eax_equals_corridor_entry_esi':origins['eax'] == 'esi',
        'esi_equals_corridor_entry_esi':origins['esi'] == 'esi',
        'ecx_equals_corridor_entry_edi':origins['ecx'] == 'edi',
        'eax_equals_corridor_entry_edi':origins['eax'] == 'edi',
        'one_prepared_word_equals_corridor_entry_ebp':len(words) == 1 and words[0] == 'ebp',
        'two_prepared_words_both_equal_corridor_entry_ebp':len(words) == 2 and all(word == 'ebp' for word in words),
        'top_prepared_word_equals_corridor_entry_ebp':words[0] == 'ebp',
        'corridor_entry_is_not_function_entry':True,
        'merged_predecessor_state_is_not_resolved':True,
        'bullet_identity_or_formal_parameter_role_proved':False,
        'callee_consumption_or_stack_cleanup_proved':False,
        'abi_or_complete_owner_difference_cause_proved':False,
    }


def transform_return_syntax(md, closed, result):
    """One exact near-RET syntax category from the already closed whole owner."""
    from capstone import CS_GRP_RET
    from capstone.x86 import X86_OP_IMM
    need(result.get('full_owner_cfg_closed_under_model') is True and result.get('complete_cfg_guard_has_no_bypass') is True, 'transform_return_requires_closed_guarded_owner')
    need(set(closed['nodes']) == set(closed['edges']) and all(target in closed['nodes'] for targets in closed['edges'].values() for target in targets), 'transform_return_closed_cfg')
    returns = [instruction for instruction in closed['nodes'].values() if instruction.group(CS_GRP_RET)]
    need(len(returns) == 1, 'transform_unique_return_count')
    instruction = returns[0]
    need(instruction.mnemonic == 'ret' and not any(instruction.prefix) and instruction.addr_size == 4 and closed['edges'][instruction.address] == [], 'transform_near_return_shape')
    reads,writes = ({md.reg_name(reg) for reg in values} for values in instruction.regs_access())
    need(reads == {'esp'} and writes == {'esp'}, 'transform_near_return_effect')
    if not instruction.operands:
        need(bytes(instruction.bytes) == b'\xc3', 'transform_plain_return_encoding')
        category = 'near_return_without_immediate'
    else:
        need(len(instruction.operands) == 1, 'transform_return_operand_count')
        operand = instruction.operands[0]
        need(operand.type == X86_OP_IMM and operand.size == 4 and operand.access == 0 and operand.imm in range(0,33,4), 'transform_return_immediate_bound')
        need(bytes(instruction.bytes) == b'\xc2'+int(operand.imm).to_bytes(2,'little'), 'transform_immediate_return_encoding')
        category = 'near_return_explicit_additional_'+str(operand.imm)+'_bytes'
    return {
        'status':'UNORDERED_EXACT_NEAR_RETURN_SYNTAX_CENSUS',
        'return_instruction_count':1,
        'return_syntax_counts_unordered':{category:1},
        'already_authorized_whole_owner_only':True,
        'return_instruction_offsets_exported':False,
        'instruction_order_exported':False,
        'stack_balance_argument_consumption_or_cleanup_theorem_proved':False,
        'callee_saved_register_or_abi_proof':False,
        'complete_owner_difference_cause_proved':False,
        'exactness_credit':'none',
    }


def advance_preparation(module, image, maps, pdb, build, md, closed, producer):
    from capstone import CS_GRP_CALL
    from capstone.x86 import X86_OP_IMM
    need(producer in ('support-01-BulletTransform.normal.obj','support-01-BulletTransform.ltcg.obj'), 'advance_producer_scope')
    gates = predicates(module,image,maps,pdb,build,{'symbol':ADVANCE_SYMBOL,'object':producer},metadata_only=True)
    need(all(value for key,value in gates.items() if key != 'all_module_object_paths_normalizable'), 'advance_callee_metadata_identity')
    rows = [row for row in maps if row['symbol'] == ADVANCE_SYMBOL]
    need(len(rows) == 1, 'advance_callee_unique_symbol')
    calls = [instruction for instruction in closed['nodes'].values() if instruction.group(CS_GRP_CALL) and instruction.operands[0].type == X86_OP_IMM and instruction.operands[0].imm == rows[0]['address']]
    need(len(calls) == 1, 'advance_unique_recovered_direct_call')
    call = calls[0]
    corridor,stop = cue_corridor(closed['nodes'],closed['edges'],call)
    need(closed['edges'][call.address] == [call.address+call.size] and call.address+call.size in closed['nodes'], 'advance_normal_return_successor')
    result = {
        'status':'BOUNDED_ADVANCE_CALLER_PREPARATION',
        'unique_advance_callee_map_pdb_producer_identity':True,
        'unique_recovered_direct_call':True,
        'metadata':dict(cue_metadata(md,corridor,stop,closed['nodes'][call.address+call.size]),**advance_transfer_metadata(md,corridor)),
        'callee_body_read_by_preparation_analysis':False,
        'new_owner_ranges_opened_by_preparation_analysis':False,
        'exactness_credit':'none',
    }
    try:
        need(corridor and len({address for address,targets in closed['edges'].items() if corridor[0].address in targets}) >= 2, 'advance_requires_actual_merge_boundary')
        result['fixed_predicates'] = model_advance_stack_corridor(md,corridor,stop)
    except GateError as exc:
        result['fixed_predicates'] = {'status':'UNRESOLVED','category':str(exc),'fixed_caller_carrier_predicates_proved':False}
    result['conditional_premises'] = ['closed immutable caller and ordinary flat32 execution; opaque calls normally return', 'the exact two-instruction corridor is entered at its merged CFG predecessor boundary', 'valid writable nonwrapping eight-byte private stack disjoint from immutable code, tables, input memory and diagnostic storage']
    result['limits'] = ['register origins denote corridor-entry values only, never function-entry or bullet identity', 'partial or implicit writes and every unsupported instruction suppress the full carrier model', 'false fixed predicates mean not established under the finite model', 'unordered syntactic copy counts alone do not establish surviving values or formal arguments', 'no callee body, consumption, cleanup, ABI, complete owner-difference cause or instruction order is proved']
    return result


def main():
    resource.setrlimit(resource.RLIMIT_CPU, (30, 30))
    resource.setrlimit(resource.RLIMIT_AS, (1024**3, 1024**3))
    def timeout(*args):
        raise GateError('wall_limit')
    signal.signal(signal.SIGALRM, timeout)
    signal.alarm(60)
    parser = argparse.ArgumentParser()
    parser.add_argument('--repo', type=P, required=True)
    args = parser.parse_args()
    repo = P(os.path.abspath(args.repo))
    need(str(repo) == ROOT and all(not path.is_symlink() for path in (repo, *repo.parents)), 'repository_path')
    def git(*args):
        return subprocess.check_output(['git', '-C', str(repo), *args], timeout=5)
    def inputs():
        need(git('rev-parse', 'HEAD').decode().strip() == HEAD and not git('status', '--porcelain=v1', '--untracked-files=all'), 'production_repository_pin')
        need(sha(repo / 'src/BulletTransform.cpp') == '72aa46821fdae7f35482a30fcfe8abbd48b075a75489f593d922e9ad2ab131d1', 'production_source_pin')
        for config in CONFIGS.values():
            run = repo / config['run']
            need(subprocess.run(['git', '-C', str(repo), 'check-ignore', '-q', str(run)], timeout=5).returncode == 0, 'analysis_directory_not_ignored')
            build = run / 'candidate/build/match-linked/BulletRuntimeUpdate'
            for name, digest in config['artifacts'].items():
                need(sha(build / name) == digest, 'native_artifact_pin')
            for name, digest in config['static'].items():
                need(sha(run / name) == digest, 'source_or_tool_pin')
            for name, digest in config['alias_pins'].items():
                need(sha(build / name) == digest, 'existing_alias_pin')
    inputs()
    os.nice(10)
    output = {}
    for tag, config in CONFIGS.items():
        run = repo / config['run']
        tree = run / 'candidate'
        build = tree / 'build/match-linked/BulletRuntimeUpdate'
        manifest = json.loads(regular(run / 'manifest.json').read_text())
        need(all(manifest['owners'][role] == owner['spec'] for role, owner in config['owners'].items()), 'owner_spec_pin')
        imported = importlib.util.spec_from_file_location('linked_image_' + tag, tree / 'scripts/linked_image.py')
        module = importlib.util.module_from_spec(imported)
        sys.modules[imported.name] = module
        imported.loader.exec_module(module)
        image = module.PEImage(build / 'source.exe')
        maps = module.map_publics(build / 'source.map') + module.map_statics(build / 'source.map')
        pdb = module.pdb_section_contributions(build / 'source.pdb')
        map_identity = module.map_metadata(build / 'source.map')
        codeview = image.codeview_record()
        need(map_identity['timestamp'] == image.timestamp and map_identity['image_base'] == image.image_base, 'pe_coff_map_identity')
        need(codeview['guid'] == pdb['guid'] and codeview['age'] == pdb['pdb_age'], 'pe_pdb_identity')
        module.verify_capstone(tomllib.loads(regular(tree / 'config/tools.lock.toml').read_text())['capstone'])
        from capstone import Cs, CS_ARCH_X86, CS_MODE_32
        md = Cs(CS_ARCH_X86, CS_MODE_32)
        md.detail = True
        artifact_result = {'artifact_sha256': config['artifacts'], 'owners': {}}
        runtime_closed = None
        for role, owner in config['owners'].items():
            try:
                start, code, contribution = selected_owner(module, image, maps, pdb, build, owner)
                def section_info(address, span):
                    return section_metadata(image, pdb, contribution, address, span)
                result, closed = diagnose_owner(md, role, code, start, section_info)
                result.update(complete_owner_sha256=owner['sha256'], owner_attribution_complete=True, complete_owner_extent_checked_before_read=True)
                if result.get('full_owner_cfg_closed_under_model'):
                    result['literal_direct_callee_census'] = raw_callee_metadata(module, image, closed['nodes'], maps)
                    if role == 'runtime_guard':runtime_closed = closed
                    elif role == 'transform':result['return_syntax_census'] = transform_return_syntax(md,closed,result)
                else:
                    result['callee_census_suppressed_until_closed_cfg'] = True
                artifact_result['owners'][role] = result
            except GateError as exc:
                artifact_result['owners'][role] = {'status':'UNRESOLVED', 'category':str(exc), 'semantic_output_suppressed':True, 'exactness_credit':'none'}
        try:
            alias_file, anchor_file = regular(build / 'link-aliases.rsp'), regular(build / 'link-anchors.c')
            need(alias_file.stat().st_size <= 65536 and anchor_file.stat().st_size <= 65536, 'alias_source_extent')
            aliases, anchors = alias_file.read_bytes(), anchor_file.read_bytes()
            alias_result = alias_scope_proposal(module, image, maps, aliases, anchors, expected_count=config['alias_count'])
            alias_result['previously_observed_source_hashes_pinned'] = bool(config['alias_pins'])
            need(sha(alias_file) == sha_bytes(aliases) and sha(anchor_file) == sha_bytes(anchors), 'alias_prerequisite_input_drift')
            artifact_result['alias_hash_prerequisite'] = alias_result
            need(runtime_closed is not None, 'cue_requires_closed_runtime')
            cue_config = dict(config, alias_pins=config.get('conditional_alias_pins',config['alias_pins']))
            alias_result['matches_approved_receiver_source_hashes'] = all(cue_config['alias_pins'].get(name) == sha_bytes(data) for name,data in [('link-aliases.rsp',aliases),('link-anchors.c',anchors)])
            artifact_result['cue_preparation'] = cue_preparation(module, image, maps, pdb, build, md, runtime_closed, aliases, anchors, cue_config)
        except GateError as exc:
            artifact_result['cue_preparation'] = {'status':'UNRESOLVED', 'category':str(exc), 'fixed_caller_carrier_predicates_proved':False}
        try:
            need(runtime_closed is not None, 'advance_requires_closed_runtime')
            artifact_result['advance_preparation'] = advance_preparation(module,image,maps,pdb,build,md,runtime_closed,config['advance_producer'])
        except GateError as exc:
            artifact_result['advance_preparation'] = {'status':'UNRESOLVED','category':str(exc),'fixed_caller_carrier_predicates_proved':False}
        output[tag] = artifact_result
    inputs()
    output.update(state='complete', collector_sha256=sha(P(__file__).absolute()), head=HEAD, postrun_pins_valid=True, runtime_executed=False, builds_executed=0, target_body_read=False, abi_adaptation_proved=False, instruction_order_exported=False, acceptance_authority='none', exactness_credit='none')
    encoded = json.dumps(output, sort_keys=True)
    need(len(encoded) <= 24000, 'output_bound')
    print(encoded)

if __name__ == '__main__':
    try:
        main()
    except GateError as exc:
        print(json.dumps({'state':'unresolved','category':str(exc),'semantic_output_suppressed':True,'exactness_credit':'none'}))
        sys.exit(2)
    except BaseException:
        print(json.dumps({'state':'unresolved','category':'input_or_decoder_failure','semantic_output_suppressed':True,'exactness_credit':'none'}))
        sys.exit(2)
