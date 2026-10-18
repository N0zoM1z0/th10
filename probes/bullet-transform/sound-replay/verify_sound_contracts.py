#!/usr/bin/env python3
"""Recursive bounded owner CFG preflight; no target bytes, sites or trace output."""
import argparse,csv,hashlib,importlib.util,json,resource,signal,subprocess,sys,tomllib
from pathlib import Path
sys.dont_write_bytecode=True
HEAD='75536f63312688610002392067ab8604cd33844b'
PINS={'target':'2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040','config/functions.csv':'19d7e4eb1aab50f97afba28cd7a5c6ed529a771f18bd9ccf4997aee6a1954350','config/tools.lock.toml':'24d77ab218e86ea8b888418922a31db9a3cf30f54bb73450a04967a4a501db9f','scripts/linked_image.py':'58583c6241e0979288b0c4ce0a0e55f1ae2a178762d93743475d05518eff0354','src/BulletTransform.cpp':'72aa46821fdae7f35482a30fcfe8abbd48b075a75489f593d922e9ad2ab131d1','src/BulletManager.hpp':'3eb4b14e48456c4cec49a851e7c4ede0b7c38ef71c40a36ccd5321da622701c1'}
START=0x406d90; SIZE=1454; OWNER_HASH='7febc838c4339dd1c8f53ae2cc12348f54b7a81ebcb2139fcf5d825da6028f9d'
class GateError(Exception):pass
def need(v,s):
 if not v:raise GateError(s)
def sha(b):return hashlib.sha256(b).hexdigest()
def partial_collect(data,md,entries,capture=None,resolved=None):
 from capstone import CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET,CS_GRP_INT,CS_GRP_IRET
 from capstone.x86 import X86_OP_IMM,X86_OP_REG,X86_OP_MEM
 need(len(data)==SIZE,'owner_size')
 work=[START]; by={}; covered={}; calls=set(); ic=ib=eb=edges=0; returns=set()
 while work:
  pc=work.pop()
  if pc in by:continue
  need(START<=pc<START+SIZE,'fallthrough_outside_owner')
  need(pc not in covered,'overlapping_decode')
  seq=list(md.disasm(data[pc-START:min(SIZE,pc-START+15)],pc,count=1))
  need(len(seq)==1,'reachable_decode_failure');i=seq[0]
  need(i.address==pc and 0<i.size<=15 and pc+i.size<=START+SIZE,'decode_bounds')
  need(all(p not in covered for p in range(pc,pc+i.size)),'overlapping_decode')
  by[pc]=i
  for p in range(pc,pc+i.size):covered[p]=pc
  need(len(by)<=1454,'instruction_bound')
  ctrl=any(i.group(g) for g in (CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET))
  if ctrl:need(all(v==0 for v in i.prefix),'control_prefix')
  need(not i.group(CS_GRP_INT) and not i.group(CS_GRP_IRET) and i.mnemonic not in ('hlt','ud2','sysenter','sysexit','syscall','sysret'),'unsupported_control')
  if i.group(CS_GRP_RET):
   need(i.mnemonic=='ret' and len(i.operands)<=1,'return_shape')
   need(not i.operands or (i.operands[0].type==X86_OP_IMM and i.operands[0].size==4),'return_operand')
   n=i.operands[0].imm if i.operands else 0;need(0<=n<=32 and n%4==0,'cleanup_bound');returns.add(n);continue
  if i.group(CS_GRP_CALL):
   need(i.mnemonic=='call' and len(i.operands)==1,'call_shape');o=i.operands[0]
   if o.type==X86_OP_IMM:
    need(o.size==4 and 0<=o.imm<=0xffffffff,'direct_call_width_or_target')
    need(not START<=o.imm<START+SIZE,'internal_call_requires_return_model');calls.add(o.imm)
   else:need(o.type in (X86_OP_REG,X86_OP_MEM) and o.size==4,'call_width');ic+=1
  if i.group(CS_GRP_JUMP):
   need(len(i.operands)==1,'branch_shape');o=i.operands[0]
   if o.type==X86_OP_IMM:
    need(o.size==4 and 0<=o.imm<=0xffffffff,'direct_branch_width_or_target')
    if START<=o.imm<START+SIZE:work.append(o.imm);edges+=1
    else:eb+=1
   else:
    need(o.type in (X86_OP_REG,X86_OP_MEM) and o.size==4,'branch_width');ib+=1
    if resolved is not None:
     need(i.address==resolved[0],'unexpected_indirect_dispatch');work.extend(resolved[1]);edges+=len(resolved[1])
   if i.mnemonic=='jmp':continue
  work.append(pc+i.size);edges+=1
 need(len(calls)<=32 and len(returns)<=8,'output_count_bound')
 identities=[]
 for a in sorted(calls):
  r=entries.get(a);need(r is not None,'callee_ledger_identity')
  name=r['proposed_name'];source=r['source_file'];need(len(name)<=160 and len(source)<=100,'identity_bound')
  identities.append({'ledger_address':hex(a),'name':name,'source':source})
 dispatch=dispatch_scope(by,md,resolved)
 if capture is not None:capture.update(by)
 return {'dispatch_scope':dispatch,'status':'PREFLIGHT_ONLY','reachable_instruction_count':len(by),'reachable_byte_count':len(covered),'unclassified_owner_byte_count':SIZE-len(covered),'edge_count':edges,'indirect_call_count':ic,'indirect_branch_count':ib,'external_branch_count':eb,'return_cleanup_bytes':sorted(returns),'cfg_complete_under_opaque_returning_calls':not(ib or eb),'direct_callee_identities_unordered':identities,'spawn_pattern_direct_call_present':0x4073e0 in calls,'source_hypothesis':{'descriptor_bulletType_s16_at_0':'zero','descriptor_color_s16_at_2':'sign_extend_packed_bits_8_15','descriptor_aimMode_u16_at_1F8':'zero','descriptor_transformStartIndex_i32_at_208':'packed_bits_0_7'},'hypothesis_status':'UNPROVEN; no descriptor dataflow evaluated','calls':'opaque returning edges only; no callee body read or clobber assumption used for semantic claims','cfg_model':'immutable owner bytes; ordinary normal fallthrough for non-control instructions; opaque calls return','exactness_credit':'none'}
def dispatch_scope(by,md,resolved=None):
 """Propose minimal table spans from bounded owner instruction metadata only."""
 from capstone import CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET
 from capstone.x86 import X86_OP_IMM,X86_OP_REG,X86_OP_MEM
 branches=[i for i in by.values() if i.group(CS_GRP_JUMP) and i.operands[0].type!=X86_OP_IMM]
 need(len(branches)==1,'one_indirect_dispatch_required');jump=branches[0];o=jump.operands[0]
 need(jump.mnemonic=='jmp' and o.type==X86_OP_MEM and o.size==4 and not o.mem.base and o.mem.index and o.mem.scale==4 and not o.mem.segment,'dispatch_indexed_dword_shape')
 pred={a:set() for a in by}
 for i in by.values():
  if i.group(CS_GRP_RET):continue
  if i.group(CS_GRP_JUMP):
   if i.operands[0].type==X86_OP_IMM and i.operands[0].imm in pred:pred[i.operands[0].imm].add(i.address)
   elif resolved is not None and i.address==resolved[0]:
    for target in resolved[1]:
     need(target in pred,'resolved_target_missing');pred[target].add(i.address)
   if i.mnemonic=='jmp':continue
  if i.address+i.size in pred:pred[i.address+i.size].add(i.address)
 def previous(i):
  need(len(pred[i.address])==1,'dispatch_prefix_known_predecessor_count');q=by[next(iter(pred[i.address]))]
  need(q.address+q.size==i.address and not any(q.prefix),'dispatch_prefix_contiguity_or_prefix');return q
 def ordinary(r):return md.reg_name(r) in ('eax','ebx','ecx','edx','esi','edi','ebp','esp')
 def writes(i):return {md.reg_name(r) for r in i.regs_access()[1]}
 def reg(o):
  need(o.type==X86_OP_REG and o.size==4 and ordinary(o.reg),'dispatch_register_width');return o.reg
 need(ordinary(o.mem.index) and writes(jump)==set() and getattr(o,'access',0)==1,'dispatch_jump_access');index=o.mem.index;table=o.mem.disp&0xffffffff;need(table+4<=0x100000000,'dispatch_table_cell_wrap');before=previous(jump);remap=None
 if before.mnemonic=='movzx':
  z=before.operands;need(len(z)==2 and reg(z[0])==index,'dispatch_remap_destination');m=z[1]
  need(m.type==X86_OP_MEM and m.size==1 and not m.mem.segment and bool(m.mem.base)^bool(m.mem.index) and (not m.mem.index or m.mem.scale==1),'dispatch_remap_shape')
  need(ordinary(m.mem.base or m.mem.index) and getattr(m,'access',0)==1 and writes(before)=={md.reg_name(index)},'dispatch_remap_access');remap=m.mem.disp&0xffffffff;index=m.mem.base or m.mem.index;before=previous(before)
 need(before.mnemonic=='ja' and len(before.operands)==1 and before.operands[0].type==X86_OP_IMM and before.operands[0].size==4,'dispatch_unsigned_guard_shape');need(writes(before)==set() and not before.address<=before.operands[0].imm<jump.address+jump.size,'dispatch_guard_bypass_or_write');compare=previous(before)
 need(compare.mnemonic=='cmp' and len(compare.operands)==2 and reg(compare.operands[0])==index and compare.operands[1].type==X86_OP_IMM and compare.operands[1].size==4,'dispatch_guard_compare_shape')
 need(writes(compare)=={'eflags'},'dispatch_compare_writes');maximum=compare.operands[1].imm;need(0<=maximum<=256,'dispatch_guard_source_bound')
 # A remap can only bound the number of bytes to inspect. Its values are not
 # read here and cannot yet establish the dword-table extent.
 proposals=[{'role':'byte_remap','candidate_address':hex(remap),'candidate_size':maximum+1}] if remap is not None else [{'role':'direct_destination_table','candidate_address':hex(table),'candidate_size':4*(maximum+1)}]
 need(all(int(r['candidate_address'],16)+r['candidate_size']<=0x100000000 for r in proposals),'dispatch_proposed_range_wrap')
 return {'status':'METADATA_ONLY_SCOPE_PROPOSAL','source_predicate':'unsigned normalized selector <= fixed maximum','normalized_selector_maximum':maximum,'two_level_byte_remap':remap is not None,'candidate_auxiliary_ranges':proposals,'destination_table_candidate_address':hex(table),'destination_table_extent':'UNRESOLVED_UNTIL_REMAP_READ' if remap is not None else 'bounded_by_unsigned_guard','known_direct_cfg_prefix_no_bypass':True,'whole_cfg_prefix_no_bypass':'UNPROVEN; unresolved indirect edges may enter prefix','selector_kind_origin':'UNPROVEN','auxiliary_data_read':False,'semantic_credit':'none'}
def executable_span(data,start,size):
 import struct
 def u16(p):return struct.unpack_from('<H',data,p)[0]
 def u32(p):return struct.unpack_from('<I',data,p)[0]
 need(data[:2]==b'MZ','dos_signature');pe=u32(0x3c);need(data[pe:pe+4]==b'PE\0\0','pe_signature')
 coff=pe+4;opt=coff+20;need(u16(coff)==0x14c and u16(opt)==0x10b,'pe_x86')
 base=u32(opt+28);need(base==0x400000,'image_base');count=u16(coff+2);need(0<count<=96,'section_count');sec=opt+u16(coff+16);matches=[]
 for n in range(count):
  p=sec+n*40;vs=u32(p+8);rva=u32(p+12);rs=u32(p+16);raw=u32(p+20);flags=u32(p+36)
  need(base+rva+max(vs,rs)<=0x100000000 and raw+rs<=len(data),'section_bounds')
  if base+rva<start+size and start<base+rva+max(vs,rs):matches.append((vs,rva,rs,raw,flags))
 need(len(matches)==1,'owner_section_overlap');vs,rva,rs,raw,flags=matches[0];offset=start-base-rva
 need(flags&0x20000020==0x20000020 and 0<=offset and offset+size<=vs and offset+size<=rs,'owner_executable_raw_virtual_extent')
 return raw+offset
def executable_owner(data):return executable_span(data,START,SIZE)
def cfg_collect(data,md,entries,image,capture):
 import struct
 from capstone import CS_GRP_JUMP
 from capstone.x86 import X86_OP_IMM
 partial={};pre=partial_collect(data,md,entries,partial);scope=pre['dispatch_scope']
 need(scope['normalized_selector_maximum']==127 and scope['candidate_auxiliary_ranges']==[{'role':'byte_remap','candidate_address':'0x407354','candidate_size':128}] and scope['destination_table_candidate_address']=='0x407340','authorized_dispatch_metadata')
 def auxiliary(address,size):
  need((address==0x407354 and size==128) or (address==0x407340 and 4<=size<=20 and size%4==0),'authorized_auxiliary_span')
  need(all(not(b<address+size and address<=int(r['span_end'],0)) for b,r in entries.items()),'auxiliary_ledger_overlap')
  offset=executable_span(image.data,address,size);raw=image.read_address(address,size)
  need(len(raw)==size and raw==image.data[offset:offset+size],'auxiliary_mapping');return raw
 remap=auxiliary(0x407354,128);maximum=max(remap)
 if maximum>4:
  return {'status':'UNRESOLVED_REMAP_EXCEEDS_AUTHORIZED_POINTER_SPAN','remap_sha256':sha(remap),'remap_size':128,'pointer_table_read':False,'semantic_output_suppressed':True}
 pointers=auxiliary(0x407340,4*(maximum+1));destinations=[struct.unpack_from('<I',pointers,n*4)[0] for n in range(maximum+1)]
 targets={destinations[n] for n in remap};need(all(START<=n<START+SIZE for n in targets),'resolved_target_outside_owner')
 indirect=[i.address for i in partial.values() if i.group(CS_GRP_JUMP) and i.operands[0].type!=X86_OP_IMM];need(len(indirect)==1,'one_dispatch')
 complete={};result=partial_collect(data,md,entries,complete,(indirect[0],targets))
 need(result['indirect_branch_count']==1 and result['external_branch_count']==0,'closure_control_shape')
 result['status']='BOUNDED_REACHABLE_CFG_CLOSED';result['cfg_complete_under_opaque_returning_calls']=True
 result['dispatch_scope']={'status':'BOUNDED_UNSIGNED_GUARD_AND_TABLE_CLOSURE','normalized_selector_maximum':127,'remap_size':128,'remap_sha256':sha(remap),'pointer_table_size':len(pointers),'pointer_table_sha256':sha(pointers),'distinct_dispatch_destination_count':len(targets),'selected_remap_targets_inside_owner_and_instruction_boundaries':True,'unused_pointer_slots':'not decoded or validated as targets','complete_cfg_guard_no_bypass':True,'selector_kind_origin':'UNPROVEN','tables_immutable_premise':True,'ordered_table_values_exported':False}
 result['hypothesis_status']='UNPROVEN; CFG closure only, no descriptor dataflow evaluated'
 capture.update({'instructions':complete,'resolved':(indirect[0],targets),'table_bytes':dict(list(enumerate(remap,0x407354))+list(enumerate(pointers,0x407340)))})
 return result
SOUND_TARGETS={0x43dc90:('EnemySoundQueueView::QueueSoundSample',123),0x43dd10:('EnemySoundQueueView::QueueSoundCue',149)}
def sound_caller_scope(by,resolved,md,entries):
 from capstone import CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET,CS_GRP_INT,CS_GRP_IRET
 from capstone.x86 import X86_OP_IMM,X86_OP_REG,X86_OP_MEM,X86_GRP_FPU
 for address,(name,size) in SOUND_TARGETS.items():
  row=entries.get(address);need(row is not None and row['proposed_name']==name and int(row['size'])==size and int(row['span_end'],0)==address+size-1 and row['source_file']=='src/EnemyEclDispatcher.cpp','sound_ledger_identity')
 pred={a:set() for a in by}
 for q in by.values():
  if q.group(CS_GRP_RET):continue
  destinations=set()
  if q.group(CS_GRP_JUMP):
   if q.operands[0].type==X86_OP_IMM:destinations.add(q.operands[0].imm)
   else:need(q.address==resolved[0],'sound_dispatch');destinations.update(resolved[1])
  if not(q.group(CS_GRP_JUMP) and q.mnemonic=='jmp'):destinations.add(q.address+q.size)
  for target in destinations:need(target in pred,'sound_cfg_edge');pred[target].add(q.address)
 def category(q):
  family=q.mnemonic if q.mnemonic in ('mov','lea','push','pop','add','sub','cmp','test','and','or','xor','movzx','movsx') else 'x87' if q.group(X86_GRP_FPU) else 'other'
  def classes(rs):
   out={}
   for r in rs:
    n=md.reg_name(r);c='stack_pointer' if n=='esp' else 'gpr32' if n in ('eax','ebx','ecx','edx','esi','edi','ebp') else 'flags' if n=='eflags' else 'x87_status' if n=='fpsw' else 'other'
    out[c]=out.get(c,0)+1
   return tuple(sorted(out.items()))
  types={X86_OP_REG:'register',X86_OP_MEM:'memory',X86_OP_IMM:'immediate'};reads,writes=q.regs_access()
  return (family,tuple(q.prefix),q.addr_size,tuple(types.get(o.type,'other') for o in q.operands),tuple(o.size for o in q.operands),classes(reads),classes(writes))
 rows=[]
 for target,(name,size) in sorted(SOUND_TARGETS.items()):
  calls=[q for q in by.values() if q.group(CS_GRP_CALL) and q.mnemonic=='call' and len(q.operands)==1 and q.operands[0].type==X86_OP_IMM and q.operands[0].imm==target]
  counts={};stops={};successors={};observed=0
  for call in calls:
   current=call.address;seen=set();stop='eight_instruction_budget'
   for unused in range(8):
    if not pred[current]:stop='entry_boundary';break
    if len(pred[current])!=1:stop='merge';break
    previous=next(iter(pred[current]));need(previous not in seen,'sound_predecessor_cycle');seen.add(previous);q=by[previous]
    if q.group(CS_GRP_CALL):stop='opaque_call';break
    if any(q.group(g) for g in (CS_GRP_JUMP,CS_GRP_RET,CS_GRP_INT,CS_GRP_IRET)) or q.mnemonic in ('hlt','ud2','syscall','sysret','sysenter','sysexit'):stop='control';break
    need(q.address+q.size==current,'sound_predecessor_fallthrough');key=category(q);counts[key]=counts.get(key,0)+1;observed+=1;current=previous
   stops[stop]=stops.get(stop,0)+1
   at=call.address+call.size;need(at in by,'sound_successor_boundary');q=by[at];o=q.operands
   # Syntactic successor bucket only: no cleanup/callee-return theorem.
   bucket='other'
   if q.mnemonic=='add' and not any(q.prefix) and q.addr_size==4 and len(o)==2 and o[0].type==X86_OP_REG and o[0].size==4 and md.reg_name(o[0].reg)=='esp' and o[1].type==X86_OP_IMM and o[1].size==4:
    bucket='add_esp_4' if o[1].imm==4 else 'add_esp_8' if o[1].imm==8 else 'add_esp_other_immediate'
   elif q.mnemonic=='pop':bucket='pop_candidate'
   elif q.group(CS_GRP_JUMP):bucket='branch'
   elif q.group(CS_GRP_RET):bucket='return'
   successors[bucket]=successors.get(bucket,0)+1
  rows.append({'target_identity':name,'target_size':size,'direct_call_count':len(calls),'inspected_predecessor_instruction_occurrences':observed,'stopping_categories_unordered':stops,'successor_shape_counts_unordered':successors,'predecessor_categories_unordered':[{'family':k[0],'prefix':list(k[1]),'address_size':k[2],'operand_types':list(k[3]),'operand_widths':list(k[4]),'read_register_classes':dict(k[5]),'write_register_classes':dict(k[6]),'count':v} for k,v in sorted(counts.items())]})
 return {'status':'BOUNDED_SOUND_CALLER_METADATA','sound_target_groups':rows,'limits':['metadata only: at most eight ordinary predecessor instructions per identified call, stopping before merge/control/opaque call','aggregate categories have no callsite addresses or ordered instruction sequence','successor buckets are syntactic; RET4, stack cleanup, actual callee clobbers, receiver and formal arguments are not proved','historical private ABI hypotheses remain caller-unproved; no ordinary-thiscall assumption','only closed owner and already authorized dispatch-table reads; no sound callee or constant reads'],'source_correction_authorized':False,'exactness_credit':'none'}

def cursor_write_role(by,resolved,md,observations,entries,preserve_out=None,fixed_offset=0x45c):
 from capstone import CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET
 from capstone.x86 import X86_OP_REG,X86_OP_IMM,X86_OP_MEM
 need(len(observations)<=8,'write_role_observation_bound')
 pred={a:set() for a in by}
 for q in by.values():
  targets=set()
  if q.group(CS_GRP_RET):continue
  if q.group(CS_GRP_JUMP):
   if q.operands[0].type==X86_OP_IMM:targets.add(q.operands[0].imm)
   else:need(q.address==resolved[0],'write_role_dispatch');targets.update(resolved[1])
  if not(q.group(CS_GRP_JUMP) and q.mnemonic=='jmp'):targets.add(q.address+q.size)
  for target in targets:need(target in pred,'write_role_edge');pred[target].add(q.address)
 need(not pred[START],'receiver_entry_has_predecessor')
 alias={'al':'eax','ah':'eax','ax':'eax','bl':'ebx','bh':'ebx','bx':'ebx','cl':'ecx','ch':'ecx','cx':'ecx','dl':'edx','dh':'edx','dx':'edx','si':'esi','di':'edi','bp':'ebp','sp':'esp'}
 def parent(r):
  n=md.reg_name(r);return alias.get(n,n)
 def origin(at,name,transparent_calls):
  work=[(at,name)];seen=set();terminals=0;blocked=False;calls=set()
  while work:
   at,name=work.pop()
   if (at,name) in seen:continue
   seen.add((at,name));need(len(seen)<=SIZE*8,'receiver_origin_bound')
   if at==START:
    terminals+=1;blocked|=name!='ecx';continue
   if not pred[at]:blocked=True;continue
   for previous in pred[at]:
    q=by[previous];o=q.operands
    if q.group(CS_GRP_CALL):
     need(len(o)==1 and o[0].type==X86_OP_IMM,'receiver_indirect_call');calls.add(o[0].imm)
     if not transparent_calls or name not in ('ebx','esi','edi','ebp'):blocked=True;continue
     work.append((previous,name));continue
    written={parent(r) for r in q.regs_access()[1]}
    if name not in written:work.append((previous,name));continue
    copy=q.mnemonic=='mov' and not any(q.prefix) and len(o)==2 and o[0].type==X86_OP_REG and o[1].type==X86_OP_REG and o[0].size==o[1].size==4 and parent(o[0].reg)==name and o[0].access==2 and o[1].access==1 and {md.reg_name(r) for r in q.regs_access()[1]}=={md.reg_name(o[0].reg)}
    if copy:work.append((previous,md.reg_name(o[1].reg)))
    else:blocked=True
  return not blocked and terminals>0,calls
 rows=[];call_obligations=set()
 for i,address,size in observations:
  o=i.operands
  fixed=bool(o and o[0].type==X86_OP_MEM and o[0].size==4 and size==4 and o[0].mem.base and not o[0].mem.index and not o[0].mem.segment and o[0].mem.disp==fixed_offset and o[0].access in (2,3))
  strict=conditional=False
  if fixed:
   strict,calls=origin(i.address,md.reg_name(o[0].mem.base),False)
   conditional,more=origin(i.address,md.reg_name(o[0].mem.base),True);call_obligations.update(calls|more)
   if conditional and preserve_out is not None:preserve_out.add(i.address)
  rows.append({'fixed_runtime_cursor_dword_destination':fixed,'increment_dword_shape':fixed and i.mnemonic=='inc','base_proven_entry_ECX_without_call_assumptions':strict,'base_matches_entry_ECX_under_normal_nonvolatile_register_premise':conditional})
 identities=[]
 for a in sorted(call_obligations):
  need(a in entries,'receiver_call_identity');r=entries[a];identities.append({'ledger_address':hex(a),'name':r['proposed_name'],'source':r['source_file']})
 return {'status':'FIXED_CURSOR_WRITE_PROVENANCE_DIAGNOSTIC','possible_alias_write_count':len(observations),'write_role_checks_unordered':sorted(rows,key=lambda r:tuple(r.values())),'opaque_call_preservation_obligations_unordered':identities,'normal_ABI_premise':'opaque calls preserve only EBX/ESI/EDI/EBP values; volatile values and memory provenance are not preserved; actual private helper preservation remains unverified','unconditional_descriptor_cache_havoc_unchanged':True,'runtime_object_private_frame_disjointness':'not inferred; requires proven origin plus explicit normal valid-object/frame premise','source_correction_authorized':False}

def prove_sound_corridor(by,pred,md,call):
 from capstone import CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET,CS_GRP_INT,CS_GRP_IRET
 from capstone.x86 import X86_OP_IMM,X86_OP_REG,X86_OP_MEM
 gprs={'eax','ebx','ecx','edx','esi','edi','ebp'};current=call.address;back=[];seen=set()
 for unused in range(9):
  if len(pred[current])!=1:break
  previous=next(iter(pred[current]));need(previous not in seen,'sound_role_cycle');seen.add(previous);q=by[previous]
  if any(q.group(g) for g in (CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET,CS_GRP_INT,CS_GRP_IRET)):break
  need(q.address+q.size==current,'sound_role_fallthrough');back.append(q);need(len(back)<=8,'sound_role_budget');current=previous
 registers={n:('incoming',n) for n in gprs};words=[];loads={}
 def reg(o):need(o.type==X86_OP_REG and o.size==4 and md.reg_name(o.reg) in gprs,'sound_role_gpr');return md.reg_name(o.reg)
 for q in reversed(back):
  need(not any(q.prefix) and q.addr_size==4 and q.bytes and q.bytes[0] not in (0xf0,0xf2,0xf3,0x66,0x67,0x26,0x2e,0x36,0x3e,0x64,0x65),'sound_role_prefix');o=q.operands;reads,writes=({md.reg_name(r) for r in rs} for rs in q.regs_access())
  if q.mnemonic=='mov':
   need(len(o)==2 and o[0].access==2 and o[1].size==4,'sound_role_move_form');dest=reg(o[0]);source=o[1]
   if source.type==X86_OP_IMM:value=('constant',source.imm&0xffffffff);expected=set()
   elif source.type==X86_OP_REG:need(source.access==1,'sound_role_move_read');n=reg(source);value=registers[n];expected={n}
   else:
    need(source.type==X86_OP_MEM and source.access==1 and not source.mem.segment and source.mem.base and not source.mem.index,'sound_role_memory_source')
    base=md.reg_name(source.mem.base);need(base in gprs,'sound_role_memory_base');value=('load',q.address,registers[base],source.mem.disp);loads[value]=(q,source);expected={base}
   need(reads==expected and writes=={dest},'sound_role_move_effects');registers[dest]=value
  elif q.mnemonic=='push':
   need(len(o)==1 and o[0].size==4 and not words,'sound_role_single_push');source=o[0]
   if source.type==X86_OP_IMM:need(source.access==0,'sound_role_push_immediate_access');value=('constant',source.imm&0xffffffff);expected={'esp'}
   else:need(source.type==X86_OP_REG and source.access==1,'sound_role_push_source');n=reg(source);value=registers[n];expected={'esp',n}
   need(reads==expected and writes=={'esp'},'sound_role_push_effects');words.append(value)
  else:raise GateError('sound_role_unsupported_preparation')
 need(len(words)==1,'sound_role_one_stack_word')
 return registers,words[0],loads,len(back)

def sound_caller_roles(by,resolved,md,entries):
 from capstone import CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET
 from capstone.x86 import X86_OP_IMM,X86_OP_MEM
 from types import SimpleNamespace
 metadata=sound_caller_scope(by,resolved,md,entries);pred={a:set() for a in by}
 for q in by.values():
  if q.group(CS_GRP_RET):continue
  targets=set()
  if q.group(CS_GRP_JUMP):
   if q.operands[0].type==X86_OP_IMM:targets.add(q.operands[0].imm)
   else:need(q.address==resolved[0],'sound_roles_dispatch');targets.update(resolved[1])
  if not(q.group(CS_GRP_JUMP) and q.mnemonic=='jmp'):targets.add(q.address+q.size)
  for target in targets:need(target in pred,'sound_roles_edge');pred[target].add(q.address)
 rows=[];failures={}
 for call in by.values():
  if not(call.group(CS_GRP_CALL) and call.operands[0].type==X86_OP_IMM and call.operands[0].imm in SOUND_TARGETS):continue
  target=call.operands[0].imm
  try:
   regs,word,loads,n=prove_sound_corridor(by,pred,md,call)
   if target==0x43dc90:
    rows.append({'target_identity':SOUND_TARGETS[target][0],'receiver_carrier_is_direct_sound_storage':regs['ecx']==('constant',0x492590),'pre_call_stack_word_is_integer_zero':word==('constant',0),'edi_unchanged_from_corridor_entry':regs['edi']==('incoming','edi'),'modeled_preparation_instruction_count':n})
   else:
    sid=regs['ebx'];id_read=sid in loads and sid[3]==8;x_read=word in loads and word[3]==0x3b4;strict=conditional=False
    if x_read:
     q,source=loads[word];observation=SimpleNamespace(address=q.address,mnemonic='source_role_observation',operands=[SimpleNamespace(type=X86_OP_MEM,size=4,access=2,mem=source.mem)])
     origin=cursor_write_role(by,resolved,md,[(observation,None,4)],entries,fixed_offset=0x3b4)['write_role_checks_unordered'][0]
     strict=origin['base_proven_entry_ECX_without_call_assumptions'];conditional=origin['base_matches_entry_ECX_under_normal_nonvolatile_register_premise']
    rows.append({'target_identity':SOUND_TARGETS[target][0],'receiver_carrier_is_direct_sound_storage':regs['esi']==('constant',0x492590),'ebx_from_actual_dword_read_with_syntactic_offset8':id_read,'pre_call_stack_word_from_actual_dword_read_with_offset3B4':x_read,'stack_word_source_is_bullet_X_without_call_assumptions':strict,'stack_word_source_is_bullet_X_conditional_nonvolatile_preservation':conditional,'modeled_preparation_instruction_count':n})
  except GateError as e:failures[str(e)]=failures.get(str(e),0)+1
 return {'status':'PARTIAL_CONDITIONAL_SOUND_CALLER_CARRIERS','identified_call_count':sum(r['direct_call_count'] for r in metadata['sound_target_groups']),'modeled_call_count':len(rows),'unresolved_categories':failures,'fixed_role_predicates_unordered':sorted(rows,key=lambda r:json.dumps(r,sort_keys=True)),'premises':['immutable closed owner code and validated dispatch tables; ordinary flat32 execution','valid nonwrapping private stack; pushed cell and near-CALL return cell are disjoint from all observed input memory, receiver objects and immutable code/tables','all observed input memory is readable and bullet objects are valid; source memory remains stable across each modeled preparation corridor; no asynchronous/volatile mutation','conditional bullet-root provenance assumes opaque calls preserve ordinary nonvolatile GPR values only; no opaque-call memory preservation'],'limits':['carrier predicates are fixed source hypotheses, not fresh proof of formal callee consumption or private ABI','Sample EDI is only unchanged from corridor entry; its sound-field origin and signed/index guard remain unresolved','Cue offset8 read is not yet current-record payload identity; source reads have no imposed ordering beyond actual dependence','near CALL pushes a return cell below the already prepared word; callee RET4, clobbers and post-return stack balance remain unproved','call-to-action assignment, initialization ordering and live cursor continuation remain unresolved'],'source_correction_authorized':False,'exactness_credit':'none'}

def prove_record_int0_address(by,pred,resolved,md,entries,source_load,earlier_guard_start):
 from capstone import CS_GRP_CALL,CS_GRP_JUMP
 from capstone.x86 import X86_OP_REG,X86_OP_MEM,X86_OP_IMM
 from types import SimpleNamespace
 gprs={'eax','ebx','ecx','edx','esi','edi','ebp','esp'}
 aliases={'ax':'eax','ah':'eax','al':'eax','bx':'ebx','bh':'ebx','bl':'ebx','cx':'ecx','ch':'ecx','cl':'ecx','dx':'edx','dh':'edx','dl':'edx','si':'esi','di':'edi','bp':'ebp','sp':'esp'}
 def parent(r):n=md.reg_name(r);return aliases.get(n,n)
 def effects(q):return tuple({md.reg_name(r) for r in rs} for rs in q.regs_access())
 def basic(q):need(not any(q.prefix) and q.addr_size==4 and q.bytes and q.bytes[0] not in (0xf0,0xf2,0xf3,0x66,0x67,0x26,0x2e,0x36,0x3e,0x64,0x65),'affine_prefix_address')
 def reg(o):return o.type==X86_OP_REG and o.size==4 and md.reg_name(o.reg) in gprs
 def address_regs(m):
  need(not m.segment and m.scale in (1,2,4,8),'affine_address_form')
  names={md.reg_name(r) for r in (m.base,m.index) if r};need(names<=gprs,'affine_address_regs');return names
 receiver_cache={}
 def receiver(at,register):
  key=(at,register)
  if key not in receiver_cache:
   memory=SimpleNamespace(base=register,index=0,scale=1,segment=0,disp=0x45c)
   observation=SimpleNamespace(address=at,mnemonic='queried_register_origin',operands=[SimpleNamespace(type=X86_OP_MEM,size=4,access=2,mem=memory)])
   r=cursor_write_role(by,resolved,md,[(observation,None,4)],entries)
   receiver_cache[key]=r['write_role_checks_unordered'][0]['base_matches_entry_ECX_under_normal_nonvolatile_register_premise']
  return receiver_cache[key]
 # Affine tuple: receiver coefficient, cursor coefficient, constant, cursor-load site.
 def add(a,b):
  sites={x[3] for x in (a,b) if x[3] is not None};need(len(sites)<=1,'affine_different_cursor_loads')
  return tuple((a[i]+b[i])&0xffffffff for i in range(3))+(next(iter(sites)) if sites else None,)
 def scale(a,n):return tuple((a[i]*n)&0xffffffff for i in range(3))+(a[3],)
 cache={};active=set();visits=0;dependency_edges=[]
 def expr(at,register):
  nonlocal visits
  name=md.reg_name(register);need(name in gprs,'affine_query_register');key=(at,register)
  if key in cache:return cache[key]
  need(key not in active and len(cache)+len(active)<48,'affine_query_cycle_or_budget');active.add(key)
  if receiver(at,register):value=(1,0,0,None)
  else:
   work=[at];seen=set();values=[]
   while work:
    current=work.pop()
    if current in seen:continue
    seen.add(current);visits+=1;need(visits<=4096,'affine_visit_budget')
    need(current!=START and pred[current],'affine_unknown_entry')
    for previous in pred[current]:
     q=by[previous];edge=[current,previous,False];dependency_edges.append(edge)
     if q.group(CS_GRP_CALL):
      need(name in ('ebx','esi','edi','ebp'),'affine_volatile_call');work.append(previous);continue
     if name not in {parent(r) for r in q.regs_access()[1]}:work.append(previous);continue
     basic(q);o=q.operands;need(len(o)==2 and reg(o[0]) and o[0].reg==register and o[0].access==2 and q.mnemonic in ('mov','lea'),'affine_definition_shape')
     reads,writes=effects(q);need(writes=={name},'affine_definition_writes')
     if q.mnemonic=='mov' and reg(o[1]):
      need(o[1].access==1 and reads=={md.reg_name(o[1].reg)},'affine_move_reads');values.append(expr(previous,o[1].reg))
     elif q.mnemonic=='lea' and o[1].type==X86_OP_MEM and o[1].size==4:
      names=address_regs(o[1].mem);need(o[1].access==1 and reads==names,'affine_lea_reads');values.append(memory_expr(previous,o[1].mem))
     elif q.mnemonic=='mov' and o[1].type==X86_OP_MEM and o[1].size==4:
      m=o[1].mem;names=address_regs(m);need(o[1].access==1 and reads==names and m.base and not m.index and m.disp==0x45c and receiver(previous,m.base),'affine_cursor_load');edge[2]=True;values.append((0,1,0,previous))
     else:need(False,'affine_unsupported_definition')
   need(values and all(x==values[0] for x in values),'affine_join_disagreement');value=values[0]
  active.remove(key);cache[key]=value;return value
 def memory_expr(at,m):
  address_regs(m);value=(0,0,m.disp&0xffffffff,None)
  if m.base:value=add(value,expr(at,m.base))
  if m.index:value=add(value,scale(expr(at,m.index),m.scale))
  return value
 need(len(source_load.operands)==1 and source_load.operands[0].type==X86_OP_MEM,'affine_source_operand')
 value=memory_expr(source_load.address,source_load.operands[0].mem)
 need(value[:3]==(1,24,0x46c) and value[3] is not None,'affine_expected_record_int0')
 cursor_site=value[3]
 # A static load PC is not a dynamic value token. Every non-receiver dependency
 # must terminate at this one event, never cross it or loop back around it.
 need(all(a!=cursor_site and (b!=cursor_site or leaf) for a,b,leaf in dependency_edges),'affine_crossed_cursor_event')
 def acyclic(edges,label):
  graph={}
  for a,b in edges:graph.setdefault(a,set()).add(b)
  grey=set();black=set()
  def visit(at):
   need(at not in grey,label)
   if at in black:return
   grey.add(at)
   for before in graph.get(at,()):visit(before)
   grey.remove(at);black.add(at)
  for at in graph:visit(at)
 acyclic([(a,b) for a,b,leaf in dependency_edges],'affine_cyclic_dependency_slice')
 fresh=True;reason=None;checked=0;freshness_edges=[];value0_moves=0
 try:
  work=[earlier_guard_start];seen=set();hits=0
  while work:
   current=work.pop()
   if current in seen:continue
   seen.add(current);need(len(seen)<=128,'freshness_node_budget')
   need(current!=START and pred[current],'freshness_entry_bypass')
   for previous in pred[current]:
    freshness_edges.append((current,previous))
    if previous==cursor_site:hits+=1;continue
    q=by[previous];checked+=1;need(checked<=512,'freshness_edge_budget');basic(q)
    need(not q.group(CS_GRP_CALL),'freshness_opaque_call')
    reads,writes=effects(q);o=q.operands
    if q.group(CS_GRP_JUMP):need(not writes and q.mnemonic!='ljmp','freshness_jump_effects')
    elif q.mnemonic=='nop':need(not writes,'freshness_nop_effects')
    elif q.mnemonic=='dec':
     need(len(o)==1 and reg(o[0]) and o[0].access==3 and reads=={md.reg_name(o[0].reg)} and writes=={md.reg_name(o[0].reg),'eflags'},'freshness_dec_register_effects')
    elif q.mnemonic=='mov' and len(o)==2 and o[0].type==X86_OP_MEM:
     m=o[0].mem;names=address_regs(m)
     need(o[0].size==4 and o[0].access==2 and m.base and not m.index and m.disp in (0x43c,0x65c) and receiver(previous,m.base),'freshness_disjoint_fixed_field_move')
     need(reg(o[1]) and o[1].access==1,'freshness_flags_move_source')
     need(reads==names|{md.reg_name(o[1].reg)} and not writes,'freshness_flags_move_effects')
     value0_moves+=int(m.disp==0x65c)
    elif q.mnemonic=='or' and len(o)==2 and o[0].type==X86_OP_MEM:
     m=o[0].mem;names=address_regs(m)
     need(o[0].size==4 and o[0].access==3 and m.base and not m.index and m.disp==0x43c and receiver(previous,m.base),'freshness_disjoint_flags_store')
     need((reg(o[1]) and o[1].access==1) or (o[1].type==X86_OP_IMM and o[1].size==4),'freshness_flags_source')
     expected=names|({md.reg_name(o[1].reg)} if o[1].type==X86_OP_REG else set());need(reads==expected and writes=={'eflags'},'freshness_flags_effects')
    else:
     need(q.mnemonic in ('mov','movzx','movsx','lea','cmp','test','and','or','xor','add','sub','shl','shr','sar','imul') and len(o) in (2,3),'freshness_unsupported_opcode')
     comparison=q.mnemonic in ('cmp','test');move=q.mnemonic in ('mov','movzx','movsx','lea')
     if comparison:need(writes=={'eflags'},'freshness_compare_effects')
     else:need(reg(o[0]) and writes==({md.reg_name(o[0].reg)} if move else {md.reg_name(o[0].reg),'eflags'}),'freshness_register_effects')
     for index,operand in enumerate(o):
      if operand.type==X86_OP_IMM:continue
      if index==0 and not comparison:need(operand.type==X86_OP_REG and operand.access in ((2,) if move or q.mnemonic=='imul' else (3,)),'freshness_destination_access')
      else:need(operand.type in (X86_OP_REG,X86_OP_MEM) and operand.access==1,'freshness_source_access')
      if operand.type==X86_OP_MEM:address_regs(operand.mem)
    work.append(previous)
  need(hits>0,'freshness_no_cursor_boundary')
  acyclic(freshness_edges,'freshness_cyclic_load_to_guard_slice')
 except GateError as e:fresh=False;reason=str(e)
 return {'affine_address_is_receiver_plus_464_plus_24_times_cursor_load_plus_8_mod32':True,'cursor_memory_unchanged_from_load_to_earlier_guard':fresh,'proven_receiver_vector_value0_move_occurrences':value0_moves,'freshness_unresolved_category':reason,'freshness_checked_instruction_occurrences':checked,'live_record_int0_identity_conditional':fresh,'premises':['pinned immutable owner/tables; ordinary normal execution and flat32 addressing','normal nonvolatile-register preservation by opaque calls is assumed for receiver/register origins; no helper memory preservation','valid parent object with disjoint +43C flags, +45C cursor, +65C vector value0 and record int0 fields; cursor index in [0,17] and selected record readable; address arithmetic does not wrap out of the object','no asynchronous or volatile/device-memory cursor mutation'],'limits':['affine coefficients are first established modulo2^32; valid in-object indexing is a separate domain premise','dispatch/action-domain reachability is not proved','if freshness is unresolved the affine identity refers only to the identified prior cursor load'],'source_correction_authorized':False,'exactness_credit':'none'}

def sample_field_gates(by,pred,resolved,md,entries,call):
 from capstone import CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET,CS_GRP_INT,CS_GRP_IRET
 from capstone.x86 import X86_OP_MEM,X86_OP_REG,X86_OP_IMM
 from types import SimpleNamespace
 gprs={'eax','ebx','ecx','edx','esi','edi','ebp'};back=[];at=call.address;seen=set();stop='sixteen_instruction_budget'
 for unused in range(16):
  if len(pred[at])!=1:stop='entry_or_merge';break
  prev=next(iter(pred[at]));need(prev not in seen,'sample_slice_cycle');seen.add(prev);q=by[prev]
  if q.group(CS_GRP_CALL):stop='opaque_call';break
  if q.group(CS_GRP_RET) or q.group(CS_GRP_INT) or q.group(CS_GRP_IRET):stop='unsupported_control';break
  back.append(q);at=prev
 chain=list(reversed(back));rows={'sound_id_from_live_receiver458_conditional':False,'signed_sound_nonnegative_gate_proven':False,'live_cursor_nonzero_gate_proven':False,'bounded_predecessor_instruction_count':len(chain),'slice_boundary':stop,'unresolved_predicates':{}}
 def effects(q):return tuple({md.reg_name(r) for r in rs} for rs in q.regs_access())
 def basic(q):need(not any(q.prefix) and q.addr_size==4 and q.bytes and q.bytes[0] not in (0xf0,0xf2,0xf3,0x66,0x67,0x26,0x2e,0x36,0x3e,0x64,0x65),'sample_prefix')
 def reg(o):return o.type==X86_OP_REG and o.size==4 and md.reg_name(o.reg) in gprs
 def mem(o):return o.type==X86_OP_MEM and o.size==4 and not o.mem.segment and o.mem.base and not o.mem.index and md.reg_name(o.mem.base) in gprs
 def reads_of(o):return {md.reg_name(o.reg)} if o.type==X86_OP_REG else {md.reg_name(o.mem.base)} if mem(o) else set()
 def root(q,o,offset):
  if not(mem(o) and o.mem.disp==offset):return False
  observation=SimpleNamespace(address=q.address,mnemonic='fixed_field_read',operands=[SimpleNamespace(type=X86_OP_MEM,size=4,access=2,mem=o.mem)])
  return cursor_write_role(by,resolved,md,[(observation,None,4)],entries,fixed_offset=offset)['write_role_checks_unordered'][0]['base_matches_entry_ECX_under_normal_nonvolatile_register_premise']
 def safe(q):
  basic(q);o=q.operands;r,w=effects(q)
  if q.group(CS_GRP_JUMP):need(q.mnemonic in ('je','jne','jz','jnz','js','jns','jl','jge','jmp') and len(o)==1 and o[0].type==X86_OP_IMM and o[0].size==4 and not w and r==(set() if q.mnemonic=='jmp' else {'eflags'}),'sample_branch_effects');return
  if q.mnemonic=='mov':
   need(len(o)==2 and reg(o[0]) and o[0].access==2 and o[1].size==4 and (o[1].type==X86_OP_IMM or ((reg(o[1]) or mem(o[1])) and o[1].access==1)) and r==reads_of(o[1]) and w=={md.reg_name(o[0].reg)},'sample_move_effects');return
  if q.mnemonic=='push':
   need(len(o)==1 and o[0].size==4 and ((o[0].type==X86_OP_IMM and o[0].access==0) or (reg(o[0]) and o[0].access==1)) and r==reads_of(o[0])|{'esp'} and w=={'esp'},'sample_push_effects');return
  if q.mnemonic in ('cmp','test'):
   need(len(o)==2 and (reg(o[0]) or mem(o[0])) and o[0].access==1 and o[1].size==4 and (o[1].type==X86_OP_IMM or (reg(o[1]) and o[1].access==1)) and r==reads_of(o[0])|reads_of(o[1]) and w=={'eflags'},'sample_compare_effects');return
  raise GateError('sample_unmodeled_effect')
 def stable_suffix(index,register=None):
  for q in chain[index+1:]:
   safe(q)
   if register is not None:need(register not in effects(q)[1],'sample_guard_register_changed')
 def defining_load(before,name,offset):
  for index in range(before-1,-1,-1):
   q=chain[index]
   if name not in effects(q)[1]:continue
   basic(q);o=q.operands
   if q.mnemonic=='mov' and len(o)==2 and reg(o[0]) and md.reg_name(o[0].reg)==name and root(q,o[1],offset):safe(q);return index
   raise GateError('sample_field_definition_not_exact_load')
  raise GateError('sample_field_load_outside_slice')
 try:
  index=defining_load(len(chain),'edi',0x458);stable_suffix(index,'edi');rows['sound_id_from_live_receiver458_conditional']=True
 except GateError as e:rows['unresolved_predicates']['sound_id']=str(e)
 sign_found=cursor_found=False
 for index,q in enumerate(chain):
  if not q.group(CS_GRP_JUMP) or q.mnemonic=='jmp' or index==0:continue
  compare=chain[index-1];o=compare.operands;next_at=chain[index+1].address if index+1<len(chain) else call.address
  try:
   safe(q);safe(compare);need(compare.address+compare.size==q.address and pred[q.address]=={compare.address},'sample_flag_producer')
   need(q.operands[0].imm!=q.address+q.size and next_at in (q.operands[0].imm,q.address+q.size),'sample_selected_branch_edge');taken=next_at==q.operands[0].imm
   zero_compare=compare.mnemonic=='cmp' and len(o)==2 and o[1].type==X86_OP_IMM and o[1].imm==0
   self_test=compare.mnemonic=='test' and len(o)==2 and reg(o[0]) and reg(o[1]) and o[0].reg==o[1].reg
   need(zero_compare or self_test,'sample_zero_comparison')
   if q.mnemonic in ('js','jns','jl','jge') and reg(o[0]) and md.reg_name(o[0].reg)=='edi':
    sign_found=True;stable_suffix(index,'edi');need(taken==(q.mnemonic in ('jns','jge')),'sample_signed_edge_is_not_nonnegative');rows['signed_sound_nonnegative_gate_proven']=True
   if q.mnemonic in ('je','jne','jz','jnz'):
    if mem(o[0]):cursor=root(compare,o[0],0x45c)
    elif reg(o[0]):
     try:origin=defining_load(index-1,md.reg_name(o[0].reg),0x45c);stable_suffix(origin);cursor=True
     except GateError:cursor=False
    else:cursor=False
    if cursor:
     cursor_found=True;stable_suffix(index);need(taken==(q.mnemonic in ('jne','jnz')),'sample_cursor_edge_is_not_nonzero');rows['live_cursor_nonzero_gate_proven']=True
  except GateError as e:
   key='signed_gate' if q.mnemonic in ('js','jns','jl','jge') else 'cursor_gate';rows['unresolved_predicates'][key]=str(e)
 if not rows['signed_sound_nonnegative_gate_proven']:rows['unresolved_predicates'].setdefault('signed_gate','sample_signed_guard_not_identified')
 if not rows['live_cursor_nonzero_gate_proven']:rows['unresolved_predicates'].setdefault('cursor_gate','sample_cursor_guard_not_identified')
 return rows

def sound_field_contracts(by,resolved,md,entries):
 from capstone import CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET
 from capstone.x86 import X86_OP_IMM,X86_OP_MEM
 from types import SimpleNamespace
 base=sound_caller_roles(by,resolved,md,entries);pred={a:set() for a in by}
 for q in by.values():
  if q.group(CS_GRP_RET):continue
  targets=set()
  if q.group(CS_GRP_JUMP):
   if q.operands[0].type==X86_OP_IMM:targets.add(q.operands[0].imm)
   else:need(q.address==resolved[0],'sound_fields_dispatch');targets.update(resolved[1])
  if not(q.group(CS_GRP_JUMP) and q.mnemonic=='jmp'):targets.add(q.address+q.size)
  for target in targets:need(target in pred,'sound_fields_edge');pred[target].add(q.address)
 samples=[];cues=[];extra_premises=[]
 for call in by.values():
  if not(call.group(CS_GRP_CALL) and call.operands[0].type==X86_OP_IMM):continue
  target=call.operands[0].imm
  if target==0x43dc90:
   try:samples.append(sample_field_gates(by,pred,resolved,md,entries,call))
   except GateError as e:samples.append({'unresolved_category':str(e),'sound_id_from_live_receiver458_conditional':False,'signed_sound_nonnegative_gate_proven':False,'live_cursor_nonzero_gate_proven':False})
  elif target==0x43dd10:
   try:
    regs,word,loads,n=prove_sound_corridor(by,pred,md,call);need(regs['ebx'] in loads,'cue_id_actual_load')
    q,source=loads[regs['ebx']];observation=SimpleNamespace(address=q.address,operands=[SimpleNamespace(type=X86_OP_MEM,size=4,access=1,mem=source.mem)])
    proof=prove_record_int0_address(by,pred,resolved,md,entries,observation,q.address)
    cues.append({'record_int0_affine_identity_proven':True,'live_current_record_int0_identity_conditional':proof['live_record_int0_identity_conditional'],'freshness_unresolved_category':proof['freshness_unresolved_category'],'freshness_checked_instruction_occurrences':proof['freshness_checked_instruction_occurrences']});extra_premises.extend(proof['premises'])
   except GateError as e:cues.append({'record_int0_affine_identity_proven':False,'live_current_record_int0_identity_conditional':False,'unresolved_category':str(e)})
 base['status']='PARTIAL_CONDITIONAL_SOUND_FIELDS_AND_GATES';base['sample_field_gate_predicates_unordered']=sorted(samples,key=lambda r:json.dumps(r,sort_keys=True));base['cue_record_predicates_unordered']=sorted(cues,key=lambda r:json.dumps(r,sort_keys=True));base['premises']=list(dict.fromkeys(base['premises']+extra_premises))
 base['limits']=['fixed carrier/field/gate predicates are separate; false/unresolved entries are not source contradictions','Sample sound loads and guards are proved only on the unique bounded predecessor chain, with no opaque-call memory preservation','Cue live-record identity is conditional on valid bounded cursor/object and the reported freshness proof','action assignment, initialization ordering, formal callee consumption, RET4/clobbers and post-return cursor behavior remain unproved']
 return base

def action_entry_witness(by,md,goal,kind,table_bytes):
 """One bounded, concrete integer-domain path; pointers retain disjoint roots."""
 from capstone.x86 import X86_OP_REG,X86_OP_MEM,X86_OP_IMM
 U=0xffffffff;gprs={'eax','ebx','ecx','edx','esi','edi','ebp','esp'}
 regs={n:('unknown',n) for n in gprs};regs.update(ecx=('object',0),esp=('stack',0))
 need(kind in (0x10,0x20,0x20000),'action_witness_kind')
 index=0 if kind==0x20000 else 1;record=0x464+24*index
 stack={0:('return',0)};fields={0x45c:index,0x43c:0,0x458:7,0x3b4:0xc2f78000,record:0x3f800000,record+8:7,record+0x10:kind,record+0x14:0};flags={};seen=set();pc=START
 def num(x):need(type(x)==int,'witness_unknown_numeric');return x
 def plus(a,b):
  if type(a)==int and type(b)==int:return (a+b)&U
  if type(b)!=int:a,b=b,a
  need(type(b)==int and isinstance(a,tuple) and a[0] in ('object','stack'),'witness_pointer_arithmetic')
  signed=b if b<0x80000000 else b-0x100000000
  off=a[1]+signed;need(-4096<=off<=4096,'witness_pointer_offset');return (a[0],off)
 def name(o):need(o.type==X86_OP_REG and o.size==4 and md.reg_name(o.reg) in gprs,'witness_register');return md.reg_name(o.reg)
 def address(o):
  need(o.type==X86_OP_MEM and not o.mem.segment and o.mem.scale in (1,2,4,8),'witness_address')
  m=o.mem;a=regs[md.reg_name(m.base)] if m.base else 0
  if m.index:a=plus(a,(num(regs[md.reg_name(m.index)])*m.scale)&U)
  return plus(a,m.disp&U)
 def read(o):
  if o.type==X86_OP_REG:return regs[name(o)]
  if o.type==X86_OP_IMM:need(o.size==4,'witness_immediate_width');return o.imm&U
  need(o.type==X86_OP_MEM and o.access==1,'witness_memory_read');a=address(o)
  if type(a)==int:
   need(o.size in (1,4) and all(a+j in table_bytes for j in range(o.size)),'witness_table_bounds')
   return sum(table_bytes[a+j]<<(8*j) for j in range(o.size))
  need(o.size==4,'witness_object_stack_width')
  if a[0]=='stack':need(a[1] in stack,'witness_uninitialized_stack');return stack[a[1]]
  need(a[0]=='object' and a[1] in fields,'witness_unknown_object_read');return fields[a[1]]
 def write(o,value):
  if o.type==X86_OP_REG:regs[name(o)]=value;return
  need(o.type==X86_OP_MEM and o.size==4 and o.access==2,'witness_memory_write');a=address(o)
  need(isinstance(a,tuple),'witness_absolute_write')
  if a[0]=='stack':
   need(-4096<=a[1]<=-4 and a[1]%4==0,'witness_stack_return_overlap');stack[a[1]]=value
  else:need(a in (('object',0x43c),('object',0x65c)),'witness_object_write_alias');fields[a[1]]=num(value)
 def operands_reads(ops,read_dest=True):
  out=set()
  for k,o in enumerate(ops):
   if o.type==X86_OP_REG and (k or read_dest):out.add(md.reg_name(o.reg))
   elif o.type==X86_OP_MEM:out.update(md.reg_name(r) for r in (o.mem.base,o.mem.index) if r)
  return out
 def arithmetic(op,a,b,oldcf=None):
  a=num(a);b=num(b);r={'add':lambda:a+b,'sub':lambda:a-b,'and':lambda:a&b,'or':lambda:a|b,'xor':lambda:a^b}[op]()&U
  f={'zf':r==0,'sf':bool(r&0x80000000),'pf':(r&255).bit_count()%2==0}
  if op=='add':f.update(cf=a+b>U,of=bool((~(a^b)&(a^r))&0x80000000))
  elif op=='sub':f.update(cf=a<b,of=bool(((a^b)&(a^r))&0x80000000))
  else:f.update(cf=False,of=False)
  if oldcf is not None:f['cf']=oldcf
  return r,f
 def branch(m):
  aliases={'jz':'je','jnz':'jne','jc':'jb','jnae':'jb','jnc':'jae','jnb':'jae','jna':'jbe','jnbe':'ja','jpe':'jp','jpo':'jnp','jnge':'jl','jnl':'jge','jng':'jle','jnle':'jg'};m=aliases.get(m,m)
  required={'je':['zf'],'jne':['zf'],'jb':['cf'],'jae':['cf'],'jbe':['cf','zf'],'ja':['cf','zf'],'js':['sf'],'jns':['sf'],'jo':['of'],'jno':['of'],'jp':['pf'],'jnp':['pf'],'jl':['sf','of'],'jge':['sf','of'],'jle':['zf','sf','of'],'jg':['zf','sf','of']}
  need(m in required and all(flags.get(k) is not None for k in required[m]),'witness_unknown_branch_flags')
  f=lambda k:flags.get(k)
  return {'je':lambda:f('zf'),'jne':lambda:not f('zf'),'jb':lambda:f('cf'),'jae':lambda:not f('cf'),'jbe':lambda:f('cf') or f('zf'),'ja':lambda:not f('cf') and not f('zf'),'js':lambda:f('sf'),'jns':lambda:not f('sf'),'jo':lambda:f('of'),'jno':lambda:not f('of'),'jp':lambda:f('pf'),'jnp':lambda:not f('pf'),'jl':lambda:f('sf')!=f('of'),'jge':lambda:f('sf')==f('of'),'jle':lambda:f('zf') or f('sf')!=f('of'),'jg':lambda:not f('zf') and f('sf')==f('of')}[m]()
 while pc!=goal:
  need(pc in by and pc not in seen and len(seen)<96,'witness_path_boundary_or_budget');seen.add(pc);q=by[pc];o=q.operands;m=q.mnemonic
  need(not any(q.prefix) and q.addr_size==4 and q.bytes and q.bytes[0] not in (0xf0,0xf2,0xf3,0x66,0x67,0x26,0x2e,0x36,0x3e,0x64,0x65),'witness_prefix')
  reads,writes=({md.reg_name(r) for r in x} for x in q.regs_access());nxt=pc+q.size
  if m in ('push','pop'):
   need(len(o)==1 and o[0].type==X86_OP_REG and o[0].size==4,'witness_stack_form');n=name(o[0]);need(n!='esp','witness_stack_esp_operand')
   need(regs['esp'][0]=='stack','witness_stack_root');off=regs['esp'][1]
   if m=='push':
    need(o[0].access==1 and reads=={'esp',n} and writes=={'esp'},'witness_push_effects');off-=4;need(-4096<=off<=-4 and off%4==0,'witness_stack_range');stack[off]=regs[n];regs['esp']=('stack',off)
   else:
    need(o[0].access==2 and reads=={'esp'} and writes=={'esp',n} and off in stack and -4096<=off<0 and off%4==0,'witness_pop_effects');regs[n]=stack[off];regs['esp']=('stack',off+4)
  elif m in ('mov','movzx','lea'):
   need(len(o)==2 and o[0].access==2,'witness_move_form')
   expected={name(o[0])} if o[0].type==X86_OP_REG else set()
   need(writes==expected and reads==operands_reads(o,False),'witness_move_effects')
   if m=='lea':need(o[0].type==X86_OP_REG and o[1].type==X86_OP_MEM and o[1].size==4 and o[1].access==1,'witness_lea_form');value=address(o[1])
   else:
    need(o[0].size==4 and ((m=='mov' and o[1].size==4) or (m=='movzx' and o[1].type==X86_OP_MEM and o[1].size==1)),'witness_move_width');value=read(o[1])
   write(o[0],value)
  elif m=='or' and len(o)==2 and o[0].type==X86_OP_MEM:
   need(o[0].size==4 and o[0].access==3 and o[1].type==X86_OP_IMM and o[1].size==4,'witness_fixed_flags_or_form')
   need(address(o[0])==('object',0x43c),'witness_fixed_flags_or_receiver_field')
   need(reads==operands_reads(o) and writes=={'eflags'},'witness_fixed_flags_or_effects')
   fields[0x43c],flags=arithmetic('or',fields[0x43c],read(o[1]))
  elif m in ('cmp','test','add','sub','and','or','xor'):
   need(len(o)==2 and o[0].size==o[1].size==4,'witness_integer_width');compare=m in ('cmp','test')
   need(o[0].access==(1 if compare else 3) and (o[1].type==X86_OP_IMM or o[1].access==1),'witness_integer_access')
   need(reads==operands_reads(o) and writes==({'eflags'} if compare else {'eflags',name(o[0])}),'witness_integer_effects')
   a=read(o[0]) if o[0].type!=X86_OP_MEM else read(o[0]);b=read(o[1]);operation={'cmp':'sub','test':'and'}.get(m,m)
   if m=='and' and o[0].type==X86_OP_REG and name(o[0])=='esp' and o[1].type==X86_OP_IMM and b==0xfffffff8 and isinstance(a,tuple) and a[0]=='stack':
    offset=(a[1]//8)*8;need(-4096<=offset<=0,'witness_aligned_stack_bounds');value=('stack',offset);flags={'cf':False,'of':False}
   elif m in ('add','sub') and isinstance(a,tuple) and a[0] in ('object','stack'):
    need(type(b)==int,'witness_pointer_delta');value=plus(a,b if m=='add' else (-b)&U);flags={}
   elif m=='xor' and o[0].type==o[1].type==X86_OP_REG and o[0].reg==o[1].reg:value,flags=arithmetic('xor',0,0)
   else:value,flags=arithmetic(operation,a,b)
   if not compare:write(o[0],value)
  elif m in ('inc','dec'):
   need(len(o)==1 and o[0].access==3 and reads=={name(o[0])} and writes=={name(o[0]),'eflags'},'witness_unary_effects')
   cf=flags.get('cf');value,flags=arithmetic('add' if m=='inc' else 'sub',read(o[0]),1);flags['cf']=cf;write(o[0],value)
  elif m=='jmp':
   need(len(o)==1 and o[0].size==4 and not writes and reads==operands_reads(o),'witness_jump_effects');nxt=num(read(o[0]))
  elif m.startswith('j'):
   need(len(o)==1 and o[0].type==X86_OP_IMM and o[0].size==4 and reads=={'eflags'} and not writes,'witness_branch_effects')
   if branch(m):nxt=o[0].imm
  elif m=='nop':need(not o and not reads and not writes,'witness_nop')
  else:raise GateError('witness_unsupported_operation')
  pc=nxt
 need(goal in by,'witness_goal_boundary');q=by[goal];o=q.operands
 need(not any(q.prefix) and q.addr_size==4,'action_goal_prefix')
 if kind==0x20000:
  need(q.mnemonic=='call' and len(o)==1 and o[0].type==X86_OP_IMM and o[0].size==4 and o[0].imm==0x43dd10,'action_cue_goal')
 else:
  need(q.mnemonic=='or' and len(o)==2 and o[0].type==X86_OP_MEM and o[0].size==4 and o[0].access==3 and o[1].type==X86_OP_IMM and o[1].size==4 and o[1].imm==kind,'action_initializer_form')
  reads,writes=({md.reg_name(r) for r in x} for x in q.regs_access())
  need(address(o[0])==('object',0x43c) and reads==operands_reads(o) and writes=={'eflags'},'action_initializer_receiver_effects')
 return {'owner_entry_endpoint_reached':True,'instruction_count':len(seen)}

def action_predecessors(by,resolved):
 from capstone import CS_GRP_RET,CS_GRP_JUMP
 from capstone.x86 import X86_OP_IMM
 pred={a:set() for a in by}
 for q in by.values():
  if q.group(CS_GRP_RET):continue
  targets=set()
  if q.group(CS_GRP_JUMP):
   if q.operands[0].type==X86_OP_IMM:targets.add(q.operands[0].imm)
   else:need(resolved and q.address==resolved[0],'action_dispatch');targets.update(resolved[1])
  if not(q.group(CS_GRP_JUMP) and q.mnemonic=='jmp'):targets.add(q.address+q.size)
  for t in targets:need(t in pred,'action_edge_boundary');pred[t].add(q.address)
 return pred

def mandatory_initializer(pred,call,initializer):
 """All backward paths terminate at this event; reject cycles and entry bypass."""
 active=set();done=set();edges=0
 def visit(a):
  nonlocal edges
  if a==initializer:return
  need(a!=START and a in pred and pred[a],'action_initializer_bypass')
  need(a not in active,'action_initializer_cycle')
  if a in done:return
  need(len(done)+len(active)<96,'action_region_budget');active.add(a)
  for b in pred[a]:
   edges+=1;need(edges<=384,'action_region_edge_budget');visit(b)
  active.remove(a);done.add(a)
 visit(call)
 return len(done)

def assignment_coverage(proven_site_sets,all_sample_sites):
 need(len(proven_site_sets)==2,'action_assignment_two_kinds')
 union=set.union(*proven_site_sets)
 need(union<=all_sample_sites,'action_assignment_unknown_site')
 return {'proven_kind_site_sets_disjoint':not bool(proven_site_sets[0]&proven_site_sets[1]),'proven_kind_site_union_covers_all_sample_sites':union==all_sample_sites,'proven_kind_site_union_count':len(union),'total_sample_site_count':len(all_sample_sites),'each_kind_has_exactly_one_proven_site':all(len(s)==1 for s in proven_site_sets)}

def sound_action_layer(by,resolved,md,entries,tables):
 from capstone.x86 import X86_OP_IMM,X86_OP_MEM
 prior=sound_field_contracts(by,resolved,md,entries);pred=action_predecessors(by,resolved)
 out={'status':'PARTIAL_SOUND_ACTION_ENTRY_CONTRACTS','prior_sample_fields':prior['sample_field_gate_predicates_unordered'],'prior_cue_record':prior['cue_record_predicates_unordered'],'cue_entry_witnesses':[],'sample_action_regions':[],
 'premises':['immutable pinned owner and tables; ordinary normal execution and normal opaque-call return edges for topological ordering only','valid receiver object and valid record index; stable chosen input memory; no asynchronous mutation','private valid 8-byte-aligned entry stack disjoint from receiver, source records, code and tables; opaque return cell','Cue witness: entry ECX receiver, cursor0, activeflags0, record0 kind0x20000/allow0/int0=7/float0=1.0, transformSound7, position.x=-123.75','Sample initializer witnesses: entry ECX receiver, cursor1, activeflags0, record1 kind0x10 or0x20/allow0/int0=7/float0=1.0, transformSound7, position.x=-123.75'],
 'limits':['Sample mandatory initializer-instruction-before-call establishes action-region assignment only; its receiver role is proved on the explicit entry witness, not universally at every dynamic execution; call occurrence, full initialization and helper memory preservation remain unproved','Cue witness stops before the actual call; callee consumption/private ABI, RET4, clobbers and continuation remain unproved','general all-index/all-state reachability remains unproved; prior field proofs retain their separate conditional premises'], 'prior_field_premises':prior['premises'],'exactness_credit':'none'}
 samples=[q for q in by.values() if q.mnemonic=='call' and len(q.operands)==1 and q.operands[0].type==X86_OP_IMM and q.operands[0].imm==0x43dc90]
 cues=[q for q in by.values() if q.mnemonic=='call' and len(q.operands)==1 and q.operands[0].type==X86_OP_IMM and q.operands[0].imm==0x43dd10]
 for q in cues:
  try:out['cue_entry_witnesses'].append(action_entry_witness(by,md,q.address,0x20000,tables))
  except GateError as e:out['cue_entry_witnesses'].append({'owner_entry_endpoint_reached':False,'unresolved_category':str(e)})
 proven_site_sets=[]
 for kind in (0x10,0x20):
  proven_sites=set()
  item={'kind':hex(kind),'initializer_entry_reached':False,'sample_call_sites_requiring_initializer_count':0,'unresolved_categories':[]};found=[]
  for q in by.values():
   o=q.operands
   if not(q.mnemonic=='or' and len(o)==2 and o[0].type==X86_OP_MEM and o[0].mem.disp==0x43c and o[1].type==X86_OP_IMM and o[1].imm==kind):continue
   try:found.append((q.address,action_entry_witness(by,md,q.address,kind,tables)))
   except GateError as e:item['unresolved_categories'].append(str(e))
  if len(found)==1:
   init,witness=found[0];item['initializer_entry_reached']=True;item['entry_instruction_count']=witness['instruction_count']
   for q in samples:
    try:mandatory_initializer(pred,q.address,init);proven_sites.add(q.address);item['sample_call_sites_requiring_initializer_count']+=1
    except GateError as e:item['unresolved_categories'].append(str(e))
  else:item['unresolved_categories'].append('action_initializer_not_unique_reached')
  item['unresolved_categories']=sorted(set(item['unresolved_categories']));out['sample_action_regions'].append(item);proven_site_sets.append(proven_sites)
 out['sample_assignment_coverage']=assignment_coverage(proven_site_sets,{q.address for q in samples})
 return out

def collect(data,md,entries,image):
 capture={};r=cfg_collect(data,md,entries,image,capture)
 need(r['status']=='BOUNDED_REACHABLE_CFG_CLOSED' and r['reachable_byte_count']==SIZE,'sound_requires_complete_owner')
 d=r['dispatch_scope'];need(d['pointer_table_sha256']=='9339bb47a5e46ed5dc8f9b8be47018e240ad41129e230e75e865aadcbb908bfd' and d['remap_sha256']=='a82463843ab4bcf47c8611e41926084c1b55330f6675fac171965ad29e4e3717','sound_table_pins')
 out=sound_action_layer(capture['instructions'],capture['resolved'],md,entries,capture['table_bytes']);out['table_sha256']={'pointers':d['pointer_table_sha256'],'remap':d['remap_sha256']};return out

def main():
 need(sys.flags.optimize==0,'optimized_python');resource.setrlimit(resource.RLIMIT_CPU,(15,15));resource.setrlimit(resource.RLIMIT_AS,(768*1024*1024,768*1024*1024));signal.alarm(25)
 p=argparse.ArgumentParser();p.add_argument('--root',type=Path,required=True);p.add_argument('--target',type=Path,required=True);a=p.parse_args();root=a.root.resolve();need(a.target.resolve()==root/'resources/th10.exe','target_path')
 def git(*args):return subprocess.check_output(['git','-C',str(root),*args],text=True,timeout=5).strip()
 need(git('rev-parse','HEAD')==HEAD and git('status','--porcelain','--untracked-files=all')=='','repository_pin')
 for n,h in PINS.items():need(sha((a.target if n=='target' else root/n).read_bytes())==h,'file_pin')
 header=(root/'src/BulletManager.hpp').read_text()
 need('struct BulletTransformPayloadView\n{\n    float float0;\n    float float1;\n    int int0;\n    int int1;\n};' in header and '(sizeof(BulletTransformPayloadView) == 0x10)' in header and '(sizeof(BulletTransformRecordView) == 0x18)' in header and '(offsetof(BulletRuntimeView, transformSound) == 0x458)' in header and '(offsetof(BulletRuntimeView, transformIndex) == 0x45c)' in header,'sound_source_field_layout')
 rows=list(csv.DictReader((root/'config/functions.csv').open(newline='')));entries={int(r['address'],0):r for r in rows};need(len(entries)==len(rows),'duplicate_owners')
 r=entries.get(START);need(r is not None and int(r['size'])==SIZE and int(r['span_end'],0)==START+SIZE-1,'owner_extent')
 need(all(b==START or not(b<START+SIZE and START<=int(q['span_end'],0)) for b,q in entries.items()),'owner_overlap')
 need(r['source_file']=='src/BulletTransform.cpp' and r['status']=='unclassified' and r['owner']=='authored','owner_source_pin')
 need(entries[0x4073e0]['proposed_name']=='BulletManagerView::SpawnBulletPattern','spawn_identity')
 spec=importlib.util.spec_from_file_location('linked_image',root/'scripts/linked_image.py');m=importlib.util.module_from_spec(spec);sys.modules[spec.name]=m;spec.loader.exec_module(m);m.verify_capstone(tomllib.loads((root/'config/tools.lock.toml').read_text())['capstone'])
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 image=m.PEImage(a.target);need(image.image_base==0x400000 and sha(image.data)==PINS['target'],'loaded_image_pin');raw=executable_owner(image.data);data=image.read_address(START,SIZE);need(data==image.data[raw:raw+SIZE] and sha(data)==OWNER_HASH,'owner_hash');md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;result=collect(data,md,entries,image)
 need(git('rev-parse','HEAD')==HEAD and git('status','--porcelain','--untracked-files=all')=='','repository_changed');result.update({'owner':hex(START),'size':SIZE,'owner_sha256':sha(data),'target_sha256':PINS['target'],'collector_sha256':sha(Path(__file__).read_bytes()),'head':HEAD});out=json.dumps(result,sort_keys=True);need(len(out)<8000,'output_bound');print(out)
if __name__=='__main__':
 try:main()
 except GateError as e:print(json.dumps({'status':'UNRESOLVED','category':str(e),'semantic_output_suppressed':True}));sys.exit(2)
 except Exception:print(json.dumps({'status':'UNRESOLVED','category':'input_or_decoder_failure','semantic_output_suppressed':True}));sys.exit(2)
