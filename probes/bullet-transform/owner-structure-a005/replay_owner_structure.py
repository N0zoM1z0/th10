#!/usr/bin/env python3
"""Pinned A005 complete-owner structure and direct helper-call metadata replay."""
import argparse, hashlib, importlib.util, json, os, pathlib, resource, signal, subprocess, sys, tomllib
if not __debug__:
    raise RuntimeError('optimized Python refused')
sys.dont_write_bytecode = True
P = pathlib.Path
HEAD = '75536f63312688610002392067ab8604cd33844b'
ROOT = '/home/pentester/coding/codex_ida/th10-reconstruction/th10'
RUN = '.analysis/bullet1454-native-baseline-20261018-a005'
OWNER_SIZE = 1540
OWNER_SHA = '72917a0ae941e4e20a0418c93db0bf9810446934d2acf38c7df03d9bc552fb52'
HELPER_SYMBOL = '?BulletResolveTransformAngle@@YAMPAUBulletRuntimeView@@M@Z'
SPEC = {'symbol': '?AdvanceTransformProgram@BulletRuntimeView@@QAEXXZ', 'object': 'support-01-BulletTransform.normal.obj', 'target_size': 1454}
PINS = {
    'source.exe': '90abfeda00eb3e8e72a47494dd5dc62a09844556262099f51ff985fe41ee1f8f',
    'source.map': 'b871fd5e8cd2a6f905b8ba0a730cb78f43f8bfd40a7a1612ac7c5fe4e349edc8',
    'source.pdb': '889220cc6025e47b95d974c5202e2df72fd70563c24595a891d47f83487b5085',
    'support-01-BulletTransform.normal.obj': '4da6109ef273ee2d6973b55296e44ad5d1ff8c751581ff3279f5f00632665ae0',
}
STATIC_PINS = {
    'manifest.json': '44405c372589be571824b9e51e107462d3b706465ab04137a7411258b5d655b0',
    'runner.py': '30beed535bda0d3f6819bca0fd973c5c7a6e1306f055ecdea1308e4126c28341',
    'candidate/scripts/linked_image.py': '58583c6241e0979288b0c4ce0a0e55f1ae2a178762d93743475d05518eff0354',
    'candidate/config/tools.lock.toml': '24d77ab218e86ea8b888418922a31db9a3cf30f54bb73450a04967a4a501db9f',
    'candidate/src/BulletTransform.cpp': '72aa46821fdae7f35482a30fcfe8abbd48b075a75489f593d922e9ad2ab131d1',
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
def predicates(l,image,maps,pdb,build,spec):
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
  raw_extent_covered=check(lambda:len(image.read_section(start[0],start[1],size))==size))
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


def preflight(md, code, start, section_info, capture=None, resolved=None, forbidden=frozenset()):
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
    if indirect and resolved is None:
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
        for name, value in {'reachable_instruction_byte_count': 1154, 'reachable_instruction_count': 271, 'direct_cfg_edge_count': 304, 'opaque_returning_call_count': 9, 'return_instruction_count': 1, 'unclassified_owner_byte_count': 386}.items():
            need(initial[name] == value, 'prior_preflight_count_drift')
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
        capture.update(nodes=after['nodes'], edges=after['edges'])
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



def helper_owner_metadata(module, image, maps, static_maps, pdb, build, caller_contribution):
    """Join fixed static symbol to complete contribution. Never slice helper bytes."""
    expected_object = module._object_path(build / SPEC['object'])
    rows = [row for row in maps if row['symbol'] == HELPER_SYMBOL]
    static_rows = [row for row in static_maps if row['symbol'] == HELPER_SYMBOL]
    need(len(rows) == len(static_rows) == 1 and rows[0] == static_rows[0], 'helper_unique_static_map_symbol')
    row = rows[0]
    position = (row['section_number'], row['section_offset'])
    need(sum((other['section_number'], other['section_offset']) == position for other in maps) == 1, 'helper_same_start_alias')
    candidates = [x for x in pdb['contributions'] if (x['section_number'], x['section_offset']) == position]
    need(len(candidates) == 1, 'helper_unique_pdb_contribution')
    contribution = candidates[0]
    size = contribution['size']
    need(type(size) is int and 0 < size <= 4096, 'helper_owner_metadata_size_bound')
    modules = [x for x in pdb['modules'] if module._object_basename(x['object']) == SPEC['object'].lower()]
    need(len(modules) == 1, 'helper_unique_producer_module')
    producer = modules[0]
    need(module._object_path(producer['object']) == module._object_path(contribution['object']) == module._object_path(caller_contribution['object']) == expected_object, 'helper_exact_same_normal_object')
    need(contribution['module_index'] == caller_contribution['module_index'] == producer['index'] and contribution['module'] == caller_contribution['module'] == producer['module'], 'helper_producer_module_identity')
    need(module._object_basename(row['object']) == SPEC['object'].lower(), 'helper_map_object_identity')
    if any(mark in str(row['object']) for mark in ('/', '\\', ':')):
        need(module._object_path(row['object']) == expected_object, 'helper_map_full_object_path')
    need('f' in row['flags'], 'helper_map_function_flag')
    section = image.section(row['section_number'])
    required = module.IMAGE_SCN_CNT_CODE | module.IMAGE_SCN_MEM_EXECUTE
    need(contribution['characteristics'] & required == required and section.characteristics & required == required, 'helper_executable_contribution')
    offset = row['section_offset']
    need(0 <= offset and offset + size <= section.virtual_size and offset + size <= section.raw_size, 'helper_complete_virtual_raw_extent')
    need(0 <= section.raw_offset + offset and section.raw_offset + offset + size <= len(image.data), 'helper_raw_file_extent')
    address = image.image_base + section.rva + offset
    need(row['address'] == address, 'helper_map_pe_address')
    need(not any(other['section_number'] == row['section_number'] and offset < other['section_offset'] < offset + size and 'f' in other['flags'] for other in maps), 'helper_interior_function_symbol')
    mapping = section_metadata(image, pdb, contribution, address, size)
    need(all(mapping.values()), 'helper_mapping_or_contribution_overlap')
    caller_start = image.image_base + image.section(caller_contribution['section_number']).rva + caller_contribution['section_offset']
    need(address + size <= caller_start or caller_start + caller_contribution['size'] <= address, 'helper_caller_owner_overlap')
    return address, size


def count_direct_helper_calls(md, nodes, caller_start, caller_size, helper_address, helper_size):
    from capstone import CS_GRP_CALL
    from capstone.x86 import X86_OP_IMM
    count = 0
    for address, instruction in nodes.items():
        need(caller_start <= address and address + instruction.size <= caller_start + caller_size and instruction.address == address, 'call_count_caller_extent')
        if not instruction.group(CS_GRP_CALL):
            continue
        need(instruction.mnemonic == 'call' and not any(instruction.prefix) and len(instruction.operands) == 1, 'call_count_instruction_shape')
        operand = instruction.operands[0]
        if operand.type != X86_OP_IMM:
            continue
        need(operand.size == 4, 'call_count_target_width')
        if helper_address <= operand.imm < helper_address + helper_size:
            need(operand.imm == helper_address, 'call_into_helper_interior')
            count += 1
    need(0 <= count <= 32, 'helper_call_count_bound')
    return {
        'helper_symbol': HELPER_SYMBOL,
        'helper_complete_owner_byte_count': helper_size,
        'helper_owner_attribution_complete': True,
        'same_normal_object_as_caller': True,
        'reachable_direct_call_instruction_count': count,
        'call_instruction_boundaries_and_closed_caller_cfg_proved': True,
        'helper_entry_identity_proved': True,
        'helper_body_sliced_decoded_or_hashed': False,
        'helper_runtime_behavior_proved': False,
        'indirect_calls_are_not_helper_identity_evidence': True,
        'caller_and_helper_sizes_not_combined': True,
        'exactness_credit': 'none',
    }


def main():
    resource.setrlimit(resource.RLIMIT_CPU, (20, 20))
    resource.setrlimit(resource.RLIMIT_AS, (1024**3, 1024**3))
    def timeout(*args):
        raise GateError('wall_limit')
    signal.signal(signal.SIGALRM, timeout)
    signal.alarm(45)
    ap = argparse.ArgumentParser()
    ap.add_argument('--repo', type=P, required=True)
    args = ap.parse_args()
    repo = P(os.path.abspath(args.repo))
    need(str(repo) == ROOT and all(not x.is_symlink() for x in (repo, *repo.parents)), 'repository_path')
    run = repo / RUN
    tree = run / 'candidate'
    build = tree / 'build/match-linked/BulletRuntimeUpdate'
    def git(*args):
        return subprocess.check_output(['git', '-C', str(repo), *args], timeout=5)
    def inputs():
        need(git('rev-parse', 'HEAD').decode().strip() == HEAD and not git('status', '--porcelain=v1', '--untracked-files=all'), 'production_repository_pin')
        need(subprocess.run(['git', '-C', str(repo), 'check-ignore', '-q', str(run)], timeout=5).returncode == 0, 'analysis_directory_not_ignored')
        for name, digest in PINS.items():
            need(sha(build / name) == digest, 'native_artifact_pin')
        for name, digest in STATIC_PINS.items():
            need(sha(run / name) == digest, 'source_or_tool_pin')
    inputs()
    os.nice(10)
    manifest = json.loads((run / 'manifest.json').read_text())
    need(manifest['owners']['transform'] == {**SPEC, 'target_address': 0x406d90}, 'owner_spec_pin')
    script = tree / 'scripts/linked_image.py'
    spec = importlib.util.spec_from_file_location('linked_image', script)
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    image = module.PEImage(build / 'source.exe')
    static_maps = module.map_statics(build / 'source.map')
    maps = module.map_publics(build / 'source.map') + static_maps
    pdb = module.pdb_section_contributions(build / 'source.pdb')
    metadata = module.map_metadata(build / 'source.map')
    codeview = image.codeview_record()
    need(metadata['timestamp'] == image.timestamp and metadata['image_base'] == image.image_base, 'pe_coff_map_identity')
    need(codeview['guid'] == pdb['guid'] and codeview['age'] == pdb['pdb_age'], 'pe_pdb_identity')
    measured = measure(module, image, maps, pdb, build, SPEC)
    need(measured['complete'] and measured['size'] == OWNER_SIZE and measured['sha256'] == OWNER_SHA, 'complete_owner_pin')
    row = next(x for x in maps if x['symbol'] == SPEC['symbol'])
    owner_contribution = next(x for x in pdb['contributions'] if (x['section_number'], x['section_offset']) == (row['section_number'], row['section_offset']))
    start = row['address']
    code = image.read_address(start, OWNER_SIZE)
    need(len(code) == OWNER_SIZE and sha_bytes(code) == OWNER_SHA, 'owner_mapping_hash')
    module.verify_capstone(tomllib.loads((tree / 'config/tools.lock.toml').read_text())['capstone'])
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = True
    def section_info(address, span):
        return section_metadata(image, pdb, owner_contribution, address, span)
    owner_mapping = section_info(start, OWNER_SIZE)
    need(all(owner_mapping.values()), 'complete_owner_mapping_ambiguity')
    def read_table(address, span):
        # close_owner gates each of its only two possible calls before slicing.
        need(start <= address and address+span <= start+OWNER_SIZE, 'table_read_owner_extent')
        return code[address-start:address-start+span]
    closed = {}
    result = close_owner(md, code, start, section_info, read_table, require_native_preflight=True, capture=closed)
    expected_closure = {'status': 'BOUNDED_COMPLETE_OWNER_CFG_CLOSED', 'reachable_instruction_count': 327, 'reachable_instruction_byte_count': 1392, 'classified_table_byte_count': 148, 'remap_byte_count': 128, 'pointer_table_byte_count': 20, 'cfg_edge_count': 369, 'opaque_returning_call_count': 16, 'return_instruction_count': 1, 'selected_dispatch_destination_count': 5, 'unclassified_owner_byte_count': 0, 'all_owner_bytes_classified': True, 'complete_cfg_guard_has_no_bypass': True, 'complete_cfg_code_and_tables_disjoint': True, 'remap_sha256': 'a82463843ab4bcf47c8611e41926084c1b55330f6675fac171965ad29e4e3717', 'pointer_table_sha256': '02eeb51684fbd4d0e5478e11777f311e047f412d40b5386b8ce1b8f56ba2da64', 'reachable_instruction_bytes_sha256': 'd6951457606ce5ed927c1baef862601ff97e4c308d21272f9ccad3d85400c354'}
    need(all(result.get(key) == value for key, value in expected_closure.items()), 'prior_complete_closure_drift')
    helper_address, helper_size = helper_owner_metadata(module, image, maps, static_maps, pdb, build, owner_contribution)
    result['direct_helper_call_metadata'] = count_direct_helper_calls(md, closed['nodes'], start, OWNER_SIZE, helper_address, helper_size)
    inputs()
    result.update({
        'state': 'complete',
        'head': HEAD,
        'collector_sha256': sha(P(__file__).absolute()),
        'artifact_sha256': PINS,
        'complete_owner_sha256': OWNER_SHA,
        'owner_attribution_complete': True,
        'postrun_pins_valid': True,
        'runtime_executed': False,
        'builds_executed': 0,
        'acceptance_authority': 'none',
    })
    output = json.dumps(result, sort_keys=True)
    need(len(output) <= 8000, 'output_bound')
    print(output)

if __name__ == '__main__':
    try:
        main()
    except GateError as exc:
        print(json.dumps({'state': 'unresolved', 'category': str(exc), 'semantic_output_suppressed': True, 'table_contents_inspected': False, 'exactness_credit': 'none'}))
        sys.exit(2)
    except BaseException:
        print(json.dumps({'state': 'unresolved', 'category': 'input_or_decoder_failure', 'semantic_output_suppressed': True, 'table_contents_inspected': False, 'exactness_credit': 'none'}))
        sys.exit(2)
