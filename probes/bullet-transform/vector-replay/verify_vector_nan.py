#!/usr/bin/env python3
"""Recursive bounded owner CFG preflight; no target bytes, sites or trace output."""
import argparse,csv,hashlib,importlib.util,json,resource,signal,subprocess,sys,tomllib
from pathlib import Path
sys.dont_write_bytecode=True
HEAD='1ace9bb5c34811a6d5911864cbe75599b47966f2'
PINS={'target':'2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040','config/functions.csv':'19d7e4eb1aab50f97afba28cd7a5c6ed529a771f18bd9ccf4997aee6a1954350','config/tools.lock.toml':'24d77ab218e86ea8b888418922a31db9a3cf30f54bb73450a04967a4a501db9f','scripts/linked_image.py':'58583c6241e0979288b0c4ce0a0e55f1ae2a178762d93743475d05518eff0354','src/BulletTransform.cpp':'b635d17c1f7bc76d3af8fc7b9fd4453f6c32b0e1f90fd6f718a4e6a7460516c0','src/BulletManager.hpp':'3eb4b14e48456c4cec49a851e7c4ede0b7c38ef71c40a36ccd5321da622701c1'}
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
def unordered_angle_status(by,resolved,md,entries,capture=None):
 from capstone import CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET
 from capstone.x86 import X86_OP_IMM,X86_OP_MEM,X86_OP_REG
 need(entries.get(0x426660,{}).get('proposed_name')=='EnemyAngleFromPlayer','angle_identity')
 pred={a:set() for a in by}
 for q in by.values():
  targets=set()
  if q.group(CS_GRP_RET):continue
  if q.group(CS_GRP_JUMP):
   if q.operands[0].type==X86_OP_IMM:targets.add(q.operands[0].imm)
   else:need(q.address==resolved[0],'unordered_dispatch');targets.update(resolved[1])
  if not(q.group(CS_GRP_JUMP) and q.mnemonic=='jmp'):targets.add(q.address+q.size)
  for t in targets:need(t in pred,'unordered_edge');pred[t].add(q.address)
 need(not pred[START],'unordered_entry_backedge')
 sites=[q for q in by.values() if q.group(CS_GRP_CALL) and len(q.operands)==1 and q.operands[0].type==X86_OP_IMM and q.operands[0].imm==0x426660]
 need(0<len(sites)<=8,'unordered_site_bound')
 gprs={'eax','ebx','ecx','edx','esi','edi','ebp','esp'}
 def previous(q):
  need(len(pred[q.address])==1,'unordered_predecessor_bypass');return by[next(iter(pred[q.address]))]
 def basic(q):need(not any(q.prefix) and q.addr_size==4,'unordered_prefix_address')
 def effects(q):return tuple({md.reg_name(r) for r in rs} for rs in q.regs_access())
 def address_regs(o):
  need(not o.mem.segment and o.mem.scale in (1,2,4,8),'unordered_memory_address')
  names={md.reg_name(r) for r in (o.mem.base,o.mem.index) if r};need(names<=gprs,'unordered_memory_register');return names
 def branch_value(name,z,p,s):
  values={'je':z,'jz':z,'jne':not z,'jnz':not z,'jp':p,'jpe':p,'jnp':not p,'jpo':not p,'ja':not z,'jnbe':not z,'jbe':z,'jna':z,'jb':False,'jc':False,'jnae':False,'jae':True,'jnb':True,'jnc':True,'js':s,'jns':not s,'jo':False,'jno':True,'jg':not z and not s,'jge':not s,'jl':s,'jle':z or s}
  need(name in values,'unordered_branch_condition');return values[name]
 results=[]
 for call in sites:
  basic(call);need(call.mnemonic=='call' and call.operands[0].size==4,'unordered_call_shape')
  after=call;preparations=[]
  for unused in range(2):
   q=previous(after);basic(q);o=q.operands
   need(q.address+q.size==after.address and q.mnemonic in ('mov','lea') and len(o)==2,'unordered_call_preparation')
   need(o[0].type==X86_OP_REG and o[0].size==4 and md.reg_name(o[0].reg) in gprs and o[0].access==2 and o[1].size==4,'unordered_preparation_destination')
   reads,writes=effects(q);need(writes=={md.reg_name(o[0].reg)},'unordered_preparation_writes')
   if o[1].type==X86_OP_MEM:
    need(o[1].access==1,'unordered_preparation_memory_access');expected=address_regs(o[1])
   elif o[1].type==X86_OP_REG:
    need(q.mnemonic=='mov' and o[1].access==1 and md.reg_name(o[1].reg) in gprs,'unordered_preparation_source');expected={md.reg_name(o[1].reg)}
   else:need(q.mnemonic=='mov' and o[1].type==X86_OP_IMM,'unordered_preparation_immediate');expected=set()
   need(reads==expected,'unordered_preparation_reads');preparations.append(q);after=q
  branch=previous(after);basic(branch);o=branch.operands
  need(branch.group(CS_GRP_JUMP) and branch.mnemonic!='jmp' and len(o)==1 and o[0].type==X86_OP_IMM and o[0].size==4,'unordered_branch_shape')
  need(effects(branch)==({'eflags'},set()),'unordered_branch_effects')
  taken=o[0].imm;fall=branch.address+branch.size;need(taken!=fall and after.address in (taken,fall),'unordered_distinct_successors')
  test=previous(branch);basic(test);o=test.operands
  need(test.address+test.size==branch.address and test.mnemonic=='test' and len(o)==2 and o[0].type==X86_OP_REG and md.reg_name(o[0].reg)=='ah' and o[0].size==1 and o[0].access==1 and o[1].type==X86_OP_IMM and o[1].size==1 and 0<=o[1].imm<=255,'unordered_test_ah_imm8')
  need(effects(test)==({'ah'},{'eflags'}),'unordered_test_effects');mask=o[1].imm
  status=previous(test);basic(status);o=status.operands
  need(status.address+status.size==test.address and status.mnemonic in ('fnstsw','fstsw') and len(o)==1 and o[0].type==X86_OP_REG and md.reg_name(o[0].reg)=='ax' and o[0].size==2 and o[0].access==2,'unordered_status_ax')
  need(effects(status)==({'fpsw'},{'ax'}),'unordered_status_effects')
  compare=previous(status);basic(compare);o=compare.operands
  need(compare.address+compare.size==status.address and compare.mnemonic=='fcomp' and len(o)==1 and o[0].type==X86_OP_MEM and o[0].size==4 and o[0].access==1,'unordered_fcomp_m32')
  # Pinned Capstone5.0.6 reports only address GPR reads and no writes for FCOMP.
  # Its implicit x87 status/stack effects are supplied by this exact opcode model,
  # not inferred from absence in regs_access. Every reported extra effect rejects.
  reads,writes=effects(compare);address=address_regs(o[0]);need(reads==address and writes==set(),'unordered_fcomp_effects')
  # Masked-invalid FCOMP unordered result: C3=C2=C0=1. Other AH bits are unknown.
  choices=set()
  for ah in range(256):
   if ah&0x45!=0x45:continue
   value=ah&mask;z=value==0;parity=bin(value).count('1')%2==0;sign=bool(value&0x80)
   choices.add(branch_value(branch.mnemonic,z,parity,sign))
  need(len(choices)==1,'unordered_unknown_status_bits_affect_branch')
  selected=taken if next(iter(choices)) else fall
  results.append(selected==after.address)
  if capture is not None:capture.append((compare,branch,selected,after.address,call))
 return {'status':'CONDITIONAL_UNORDERED_ANGLE_STATUS_BRANCH','direct_angle_call_count':len(sites),'unordered_selects_unique_call_preparation_count':sum(results),'unordered_selects_other_successor_count':len(results)-sum(results),'all_observed_unordered_status_branches_select_angle_call':all(results),'premises':['pinned immutable owner/tables and ordinary normal execution','valid x87 stack and memory operands; masked floating-point exceptions so unordered FCOMP completes and status transfer executes','unordered input to each identified FCOMP is assumed, not proven from requested-angle data'],'limits':['no constant bytes read or threshold identity proved','no requested-value provenance, source-site mapping or complete sentinel semantics established','theorem concerns the unique immediate call-preparation path selected by each observed branch, not eventual reachability through the other successor'],'source_correction_authorized':False,'exactness_credit':'none'}

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

def angle_nan_reload_store(by,resolved,md,entries,selected_call=None):
 from capstone import CS_GRP_JUMP,CS_GRP_RET
 from capstone.x86 import X86_OP_MEM,X86_OP_IMM,X86_OP_REG
 from types import SimpleNamespace
 captured=[];base=unordered_angle_status(by,resolved,md,entries,captured)
 if selected_call is not None:captured=[row for row in captured if row[4].address==selected_call]
 need(captured,'nan_selected_site')
 pred={a:set() for a in by}
 for q in by.values():
  targets=set()
  if q.group(CS_GRP_RET):continue
  if q.group(CS_GRP_JUMP):
   if q.operands[0].type==X86_OP_IMM:targets.add(q.operands[0].imm)
   else:need(q.address==resolved[0],'nan_dispatch');targets.update(resolved[1])
  if not(q.group(CS_GRP_JUMP) and q.mnemonic=='jmp'):targets.add(q.address+q.size)
  for t in targets:need(t in pred,'nan_edge');pred[t].add(q.address)
 gprs={'eax','ebx','ecx','edx','esi','edi','ebp','esp'}
 def effect(q):return tuple({md.reg_name(r) for r in rs} for rs in q.regs_access())
 def memory(o):
  need(o.type==X86_OP_MEM and o.size==4 and not o.mem.segment and o.mem.scale in (1,2,4,8),'nan_memory_form')
  names={md.reg_name(r) for r in (o.mem.base,o.mem.index) if r};need(names<=gprs,'nan_address_registers')
  return (o.mem.base,o.mem.index,o.mem.scale,o.mem.disp),names
 def floating(q,name):
  need(q.mnemonic==name and not any(q.prefix) and q.addr_size==4 and len(q.operands)==1,'nan_float_opcode')
  need(q.bytes and q.bytes[0]==(0xd9 if name in ('fld','fst','fstp') else 0xd8),'nan_float_raw_prefix')
  o=q.operands[0];address,names=memory(o)
  # Capstone5.0.6 labels FLD, FST and FSTP m32 operands READ.
  # Exact FST/FSTP opcode semantics supply the memory write and non-pop/pop distinction.
  need(o.access==1 and effect(q)==(names,{'fpsw'}),'nan_float_decoder_effects')
  return address,names
 rows=[]
 for compare,branch,selected,prep,call in captured:
  need(selected!=prep,'nan_other_successor_required')
  need(len(pred[compare.address])==1,'nan_compare_predecessor')
  initial=by[next(iter(pred[compare.address]))];need(initial.address+initial.size==compare.address,'nan_initial_fallthrough')
  initial_address,initial_regs=floating(initial,'fld');compare_address,compare_regs=memory(compare.operands[0])
  need(selected in by,'nan_alternate_boundary');reload=by[selected];reload_address,reload_regs=floating(reload,'fld')
  # All intervening instructions are the already validated compare/status/TEST/branch chain.
  chain=[];at=compare.address
  while at!=branch.address:
   need(at in by and len(chain)<4,'nan_fixed_chain');q=by[at];chain.append(q);at+=q.size
  chain.append(branch);need(len(chain)==4,'nan_fixed_chain_length')
  aliases={'ax':'eax','ah':'eax','al':'eax'}
  changed={aliases.get(name,name) for q in chain for name in effect(q)[1]}
  matches_compare=reload_address==compare_address and not(compare_regs&changed)
  matches_initial=reload_address==initial_address and not(initial_regs&changed)
  need(matches_compare or matches_initial,'nan_reload_same_stable_address')
  at=reload.address+reload.size;need(at in by,'nan_store_boundary');store=by[at]
  if store.mnemonic=='jmp':
   need(not any(store.prefix) and store.addr_size==4 and len(store.operands)==1 and store.operands[0].type==X86_OP_IMM and store.operands[0].size==4 and effect(store)==(set(),set()),'nan_merge_jump')
   need(store.operands[0].imm in by,'nan_merge_jump_boundary');store=by[store.operands[0].imm]
  stack_preparation=False
  if store.mnemonic=='push':
   need(not any(store.prefix) and store.addr_size==4 and store.bytes and store.bytes[0] not in (0xf0,0xf2,0xf3,0x66,0x67,0x26,0x2e,0x36,0x3e,0x64,0x65),'nan_push_prefix')
   need(len(store.operands)==1,'nan_push_operand_count');operand=store.operands[0]
   need(operand.type==X86_OP_REG and operand.size==4 and operand.access==1 and md.reg_name(operand.reg) in gprs-{'esp'},'nan_push_register')
   need(effect(store)==({'esp',md.reg_name(operand.reg)},{'esp'}),'nan_push_effects')
   stack_preparation=True;at=store.address+store.size;need(at in by,'nan_after_push_boundary');store=by[at]
  arithmetic=False
  if not stack_preparation and store.mnemonic in ('fadd','fmul','fsub','fsubr','fdiv','fdivr'):
   # Exactly one non-popping m32 arithmetic operation preserves NaN class at ST(0).
   # Its opcode model has no memory write; decoder metadata is separately gated.
   floating(store,store.mnemonic);arithmetic=True
   at=store.address+store.size;need(at in by,'nan_arithmetic_store_boundary');store=by[at]
  need(store.mnemonic in ('fst','fstp'),'nan_float_opcode')
  _,store_regs=floating(store,store.mnemonic)
  need(not stack_preparation or 'esp' not in store_regs,'nan_push_destination_address_changed')
  # This path's freshly pushed value is at ST(0) regardless of other merge predecessors.
  o=store.operands[0];need(o.mem.disp in (0x660,0x6c8),'nan_fixed_result_offset')
  observation=SimpleNamespace(address=store.address,mnemonic='x87_result_field_write',operands=[SimpleNamespace(type=X86_OP_MEM,size=4,access=2,mem=o.mem)])
  origin=cursor_write_role(by,resolved,md,[(observation,None,4)],entries,fixed_offset=o.mem.disp)
  receiver=origin['write_role_checks_unordered'][0]['base_matches_entry_ECX_under_normal_nonvolatile_register_premise']
  rows.append({'one_protected_stack_push_before_result_store':stack_preparation,'result_store_preserves_x87_stack_depth':store.mnemonic=='fst','one_modeled_nan_preserving_arithmetic_step':arithmetic,'same_stable_address_as_compare_memory':matches_compare,'same_stable_address_as_preceding_fld':matches_initial,'path_stores_nan_without_angle_call_if_matched_source_is_nan':True,'destination_receiver_origin_conditional':receiver,'vector_value1_role_conditional':receiver and o.mem.disp==0x660,'direction_value1_role_conditional':receiver and o.mem.disp==0x6c8})
 return {'status':'CONDITIONAL_REACHED_COMPARE_NAN_RELOAD_STORE','observed_site_count':len(rows),'site_role_predicates_unordered':sorted(rows,key=lambda r:tuple(r.values())),'premises':base['premises']+['matched readable source memory is unchanged during this straight-line path; no asynchronous or volatile/device-memory mutation','opaque calls preserve EBX/ESI/EDI/EBP values only for conditional receiver-origin roles','if one PUSH preparation is modeled: ESP names a valid nonwrapping private stack and its four-byte pushed cell is disjoint from the valid receiver object, matched float source memory, and immutable code/tables; PUSH changes only ESP and this cell, not x87 state'],'limits':['theorem begins at the identified reached compare and assumes its matched memory source is NaN','requested record/root identity and earlier-guard reachability remain unresolved','NaN classification is preserved, not signaling state or bitwise payload identity','theorem applies only to the selected path through a merge, not all predecessor values','theorem stops at the first result-field store, permitting at most one protected stack PUSH beforehand: FST leaves ST(0) and stack depth intact, FSTP pops once; no subsequent x87-stack or continuation claim'],'complete_requested_nan_behavior_proven':False,'source_correction_authorized':False,'exactness_credit':'none'}

def angle_nan_partial(by,resolved,md,entries):
 captured=[];base=unordered_angle_status(by,resolved,md,entries,captured)
 rows=[];unresolved={};premises=None;limits=None
 for site in captured:
  try:
   result=angle_nan_reload_store(by,resolved,md,entries,site[4].address)
   rows.extend(result['site_role_predicates_unordered']);premises=result['premises'];limits=result['limits']
  except GateError as error:
   category=str(error);need(len(category)<=80 and category.replace('_','').isalnum(),'nan_fixed_failure_category')
   unresolved[category]=unresolved.get(category,0)+1
 return {'status':'PARTIAL_CONDITIONAL_REACHED_COMPARE_NAN_STORES','observed_site_count':len(captured),'verified_site_count':len(rows),'unresolved_site_count':sum(unresolved.values()),'unresolved_fixed_categories':unresolved,'verified_site_role_predicates_unordered':sorted(rows,key=lambda r:tuple(r.values())),'protected_stack_preparation_site_count':sum(r['one_protected_stack_push_before_result_store'] for r in rows),'nonpopping_result_store_site_count':sum(r['result_store_preserves_x87_stack_depth'] for r in rows),'modeled_arithmetic_step_site_count':sum(r['one_modeled_nan_preserving_arithmetic_step'] for r in rows),'premises':premises or base['premises'],'limits':limits or ['all per-site reload/store checks unresolved; no NaN store theorem established'],'complete_requested_nan_behavior_proven':False,'source_correction_authorized':False,'exactness_credit':'none'}

def vector_earlier_guard(by,resolved,md,entries):
 from capstone import CS_GRP_JUMP,CS_GRP_RET
 from capstone.x86 import X86_OP_REG,X86_OP_MEM,X86_OP_IMM
 from types import SimpleNamespace
 captured=[];unordered_angle_status(by,resolved,md,entries,captured);chosen=[]
 for site in captured:
  try:r=angle_nan_reload_store(by,resolved,md,entries,site[4].address)
  except GateError:continue
  if r['site_role_predicates_unordered'][0]['vector_value1_role_conditional']:chosen.append(site)
 need(len(chosen)==1,'guard_unique_verified_vector_site')
 pred={a:set() for a in by}
 for q in by.values():
  targets=set()
  if q.group(CS_GRP_RET):continue
  if q.group(CS_GRP_JUMP):
   if q.operands[0].type==X86_OP_IMM:targets.add(q.operands[0].imm)
   else:need(q.address==resolved[0],'guard_dispatch');targets.update(resolved[1])
  if not(q.group(CS_GRP_JUMP) and q.mnemonic=='jmp'):targets.add(q.address+q.size)
  for t in targets:need(t in pred,'guard_edge');pred[t].add(q.address)
 def prev(q):need(len(pred[q.address])==1,'guard_predecessor_merge');return by[next(iter(pred[q.address]))]
 def basic(q):need(not any(q.prefix) and q.addr_size==4 and q.bytes and q.bytes[0] not in (0xf0,0xf2,0xf3,0x66,0x67,0x26,0x2e,0x36,0x3e,0x64,0x65),'guard_prefix_address')
 def eff(q):return tuple({md.reg_name(r) for r in rs} for rs in q.regs_access())
 def addr(o):
  need(o.type==X86_OP_MEM and o.size==4 and not o.mem.segment and o.mem.scale in (1,2,4,8),'guard_memory_form')
  regs={md.reg_name(r) for r in (o.mem.base,o.mem.index) if r};need(regs<={'eax','ebx','ecx','edx','esi','edi','ebp','esp'},'guard_address_regs')
  return (o.mem.base,o.mem.index,o.mem.scale,o.mem.disp),regs
 def fp(q,name):
  basic(q);need(q.mnemonic==name and len(q.operands)==1 and q.operands[0].access==1,'guard_float_form');a,r=addr(q.operands[0]);need(eff(q)==(r,{'fpsw'}),'guard_float_effects');return a,r
 later_compare=chosen[0][0];later_load=prev(later_compare);later_address,later_regs=fp(later_load,'fld')
 branch=prev(later_load);basic(branch);o=branch.operands
 need(branch.group(CS_GRP_JUMP) and branch.mnemonic in ('je','jz','jne','jnz','jp','jpe','jnp','jpo','ja','jbe','jb','jae') and len(o)==1 and o[0].type==X86_OP_IMM and o[0].size==4 and eff(branch)==({'eflags'},set()),'guard_branch_form')
 taken=o[0].imm;fall=branch.address+branch.size;need(taken!=fall and later_load.address in (taken,fall),'guard_distinct_edges')
 test=prev(branch);basic(test);o=test.operands
 need(test.address+test.size==branch.address and test.mnemonic=='test' and len(o)==2 and o[0].type==X86_OP_REG and md.reg_name(o[0].reg)=='ah' and o[0].size==1 and o[0].access==1 and o[1].type==X86_OP_IMM and o[1].size==1 and 0<=o[1].imm<=255 and eff(test)==({'ah'},{'eflags'}),'guard_test_form');mask=o[1].imm
 status=prev(test);basic(status);o=status.operands
 need(status.address+status.size==test.address and status.mnemonic=='fnstsw' and len(o)==1 and o[0].type==X86_OP_REG and md.reg_name(o[0].reg)=='ax' and o[0].size==2 and o[0].access==2 and eff(status)==({'fpsw'},{'ax'}),'guard_status_form')
 compare=prev(status);basic(compare);o=compare.operands
 need(compare.address+compare.size==status.address and compare.mnemonic=='fcomp' and len(o)==1 and o[0].access==1,'guard_compare_form');compare_address,compare_regs=addr(o[0]);need(eff(compare)==(compare_regs,set()),'guard_compare_effects')
 initial=prev(compare);initial_address,initial_regs=fp(initial,'fld');need(initial.address+initial.size==compare.address,'guard_initial_fallthrough')
 # FNSTSW AX is the only GPR write between these known source reads and the successor.
 match_compare=later_address==compare_address and 'eax' not in compare_regs
 match_initial=later_address==initial_address and 'eax' not in initial_regs
 need(match_compare or match_initial,'guard_shared_stable_nan_source')
 decisions=set()
 for ah in range(256):
  if ah&0x45!=0x45:continue
  value=ah&mask;z=value==0;p=bin(value).count('1')%2==0
  choices={'je':z,'jz':z,'jne':not z,'jnz':not z,'jp':p,'jpe':p,'jnp':not p,'jpo':not p,'ja':not z,'jbe':z,'jb':False,'jae':True};decisions.add(choices[branch.mnemonic])
 need(len(decisions)==1,'guard_unknown_status_bits');selected=taken if next(iter(decisions)) else fall
 permits=selected==later_load.address
 outcome={'selected_path_first_store_resolved':False,'stores_parent_angle_conditional':False,'stores_matched_nan_source':False,'vector_destination_conditional':False}
 failure=None
 if not permits:
  try:
   need(selected in by,'guard_selected_boundary');load=by[selected];load_address,load_regs=fp(load,'fld')
   at=load.address+load.size;need(at in by,'guard_store_boundary');store=by[at]
   if store.mnemonic=='jmp':
    basic(store);need(len(store.operands)==1 and store.operands[0].type==X86_OP_IMM and store.operands[0].size==4 and eff(store)==(set(),set()),'guard_selected_jump');need(store.operands[0].imm in by,'guard_jump_boundary');store=by[store.operands[0].imm]
   fp(store,'fstp');need(store.operands[0].mem.disp==0x660,'guard_vector_store_offset')
   def receiver(q,offset):
    observation=SimpleNamespace(address=q.address,mnemonic='proven_memory_role',operands=[SimpleNamespace(type=X86_OP_MEM,size=4,access=2,mem=q.operands[0].mem)])
    r=cursor_write_role(by,resolved,md,[(observation,None,4)],entries,fixed_offset=offset)
    return r['write_role_checks_unordered'][0]['base_matches_entry_ECX_under_normal_nonvolatile_register_premise']
   destination=receiver(store,0x660)
   outcome={'selected_path_first_store_resolved':True,'stores_parent_angle_conditional':destination and load.operands[0].mem.disp==0x3e4 and receiver(load,0x3e4),'stores_matched_nan_source':load_address==later_address and 'eax' not in load_regs,'vector_destination_conditional':destination}
  except GateError as e:failure=str(e)
 return {'status':'CONDITIONAL_VECTOR_EARLIER_UNORDERED_GUARD','shared_nan_source_matches_earlier_compare_memory':match_compare,'shared_nan_source_matches_earlier_fld':match_initial,'earlier_unordered_guard_permits_later_vector_compare':permits,'earlier_unordered_guard_excludes_later_vector_compare':not permits,'selected_path_outcome':outcome,'selected_path_unresolved_category':failure,'premises':['pinned immutable owner/tables and ordinary normal execution','masked FP exceptions; valid x87 stack and ordinary valid flat32 memory','shared source memory is NaN and stable across the modeled path; no asynchronous or volatile/device-memory mutation','conditional receiver roles assume opaque calls preserve EBX/ESI/EDI/EBP values only'],'limits':['execution must first reach the earlier identified guard; still-earlier dispatch/guards and current-record/requested-root identity remain unproved','no threshold constants read; no NaN payload identity claim','a permitted earlier edge does not by itself compose the later NaN-store theorem; its matched source identity must also agree','a selected-path store is not a statement about other merge predecessors'],'complete_requested_nan_behavior_proven':False,'source_correction_authorized':False,'exactness_credit':'none'}

def vector_source_root_scope(by,resolved,md,entries):
 from capstone import CS_GRP_CALL,CS_GRP_JUMP,CS_GRP_RET
 from capstone.x86 import X86_OP_REG,X86_OP_MEM,X86_OP_IMM
 from types import SimpleNamespace
 guard=vector_earlier_guard(by,resolved,md,entries);result_store_premises=[]
 captured=[];unordered_angle_status(by,resolved,md,entries,captured);chosen=[]
 for site in captured:
  try:r=angle_nan_reload_store(by,resolved,md,entries,site[4].address)
  except GateError:continue
  row=r['site_role_predicates_unordered'][0]
  if row['vector_value1_role_conditional']:chosen.append((site,row));result_store_premises=r['premises']
 need(len(chosen)==1,'root_unique_vector_site');site,row=chosen[0]
 pred={a:set() for a in by}
 for q in by.values():
  targets=set()
  if q.group(CS_GRP_RET):continue
  if q.group(CS_GRP_JUMP):
   if q.operands[0].type==X86_OP_IMM:targets.add(q.operands[0].imm)
   else:need(q.address==resolved[0],'root_dispatch');targets.update(resolved[1])
  if not(q.group(CS_GRP_JUMP) and q.mnemonic=='jmp'):targets.add(q.address+q.size)
  for t in targets:need(t in pred,'root_edge');pred[t].add(q.address)
 need(len(pred[site[0].address])==1,'root_load_predecessor');load=by[next(iter(pred[site[0].address]))];memory=load.operands[0].mem
 aliases={'ax':'eax','ah':'eax','al':'eax','bx':'ebx','bh':'ebx','bl':'ebx','cx':'ecx','ch':'ecx','cl':'ecx','dx':'edx','dh':'edx','dl':'edx','si':'esi','di':'edi','bp':'ebp','sp':'esp'}
 gprs={'eax','ebx','ecx','edx','esi','edi','ebp','esp'}
 def parent(reg):name=md.reg_name(reg);return aliases.get(name,name)
 queries=[(load.address,md.reg_name(reg)) for reg in (memory.base,memory.index) if reg];seen_queries=set();definitions={};boundaries={};visits=0
 def boundary(name):boundaries[name]=boundaries.get(name,0)+1
 while queries:
  at,name=queries.pop()
  if (at,name) in seen_queries:continue
  need(name in gprs and len(seen_queries)<48,'root_query_budget');seen_queries.add((at,name))
  work=[at];seen=set()
  while work:
   current=work.pop()
   if current in seen:continue
   seen.add(current);visits+=1;need(visits<=4096,'root_predecessor_budget')
   if current==START:boundary('owner_entry_root');continue
   need(pred[current],'root_predecessor_gap')
   for previous in pred[current]:
    q=by[previous]
    if q.group(CS_GRP_CALL):boundary('opaque_call');continue
    writes={parent(reg) for reg in q.regs_access()[1]}
    if name not in writes:work.append(previous);continue
    definitions[q.address]=q;o=q.operands
    if not(o and o[0].type==X86_OP_REG and o[0].size==4 and md.reg_name(o[0].reg)==name):boundary('partial_or_implicit_definition');continue
    if q.mnemonic=='mov' and len(o)==2 and o[1].type==X86_OP_REG and o[1].size==4:queries.append((previous,md.reg_name(o[1].reg)))
    elif q.mnemonic=='lea' and len(o)==2 and o[1].type==X86_OP_MEM:
     queries.extend((previous,md.reg_name(reg)) for reg in (o[1].mem.base,o[1].mem.index) if reg)
    elif q.mnemonic in ('add','sub','shl','sal','imul'):
     # Dependency metadata only: even over-approximated old-destination dependence carries no value theorem.
     queries.append((previous,name));queries.extend((previous,md.reg_name(x.reg)) for x in o[1:] if x.type==X86_OP_REG and x.size==4)
    else:boundary('definition_not_recursed')
 counts={};cursor_loads=cursor_receivers=0
 for q in definitions.values():
  o=q.operands;family=q.mnemonic if q.mnemonic in ('mov','lea','add','sub','shl','sal','imul') else 'other_definition'
  key=(family,tuple(q.prefix),q.addr_size,tuple(x.size for x in o));counts[key]=counts.get(key,0)+1
  if q.mnemonic=='mov' and len(o)==2 and o[1].type==X86_OP_MEM and o[1].size==4 and o[1].mem.disp==0x45c:
   cursor_loads+=1
   if not any(q.prefix) and q.addr_size==4 and o[1].access==1:
    observation=SimpleNamespace(address=q.address,mnemonic='cursor_read_origin',operands=[SimpleNamespace(type=X86_OP_MEM,size=4,access=2,mem=o[1].mem)])
    r=cursor_write_role(by,resolved,md,[(observation,None,4)],entries)
    cursor_receivers+=int(r['write_role_checks_unordered'][0]['base_matches_entry_ECX_under_normal_nonvolatile_register_premise'])
 need(len(counts)<=24,'root_category_budget')
 composed=guard['earlier_unordered_guard_permits_later_vector_compare'] and row['same_stable_address_as_preceding_fld'] and (guard['shared_nan_source_matches_earlier_fld'] or guard['shared_nan_source_matches_earlier_compare_memory'])
 return {'status':'VECTOR_NAN_COMPOSITION_AND_ROOT_METADATA','reached_earlier_guard_nan_source_composes_to_vector_nan_store':composed,'requested_source_address_shape':{'has_base':bool(memory.base),'has_index':bool(memory.index),'scale_is_1':memory.scale==1,'displacement_is_record_float1_offset4':memory.disp==4,'displacement_is_transform_array_offset464':memory.disp==0x464},'definition_categories_unordered':[{'family':k[0],'prefix':list(k[1]),'address_size':k[2],'operand_widths':list(k[3]),'count':v} for k,v in sorted(counts.items())],'dependency_query_count':len(seen_queries),'definition_instruction_count':len(definitions),'stopping_categories_aggregated':boundaries,'syntactic_cursor45c_load_count':cursor_loads,'conditional_receiver_cursor_load_count':cursor_receivers,'premises':guard['premises']+result_store_premises,'limits':['composition begins only when execution reaches the earlier guard with the shared memory source NaN','root traversal is dependency metadata only; arithmetic values and live-cursor freshness are not proved','source layout is record float1+4, kind+10; no current-record or dispatch identity inferred','opaque calls stop root metadata traversal, even when receiver-origin subproof uses its explicit nonvolatile premise'],'complete_requested_nan_behavior_proven':False,'source_correction_authorized':False,'exactness_credit':'none'}

def prove_record_address(by,pred,resolved,md,entries,source_load,earlier_guard_start):
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
 need(value[:3]==(1,24,0x468) and value[3] is not None,'affine_expected_record_float1')
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
 return {'affine_address_is_receiver_plus_464_plus_24_times_cursor_load_plus_4_mod32':True,'cursor_memory_unchanged_from_load_to_earlier_guard':fresh,'proven_receiver_vector_value0_move_occurrences':value0_moves,'freshness_unresolved_category':reason,'freshness_checked_instruction_occurrences':checked,'live_record_float1_identity_conditional':fresh,'premises':['pinned immutable owner/tables; ordinary normal execution and flat32 addressing','normal nonvolatile-register preservation by opaque calls is assumed for receiver/register origins; no helper memory preservation','valid parent object with disjoint +43C flags, +45C cursor, +65C vector value0 and record float1 fields; cursor index in [0,17] and selected record readable; address arithmetic does not wrap out of the object','no asynchronous or volatile/device-memory cursor mutation'],'limits':['affine coefficients are first established modulo2^32; valid in-object indexing is a separate domain premise','dispatch/action-domain reachability is not proved','if freshness is unresolved the affine identity refers only to the identified prior cursor load'],'source_correction_authorized':False,'exactness_credit':'none'}

def vector_affine_record(by,resolved,md,entries,witness_capture=None):
 from capstone import CS_GRP_JUMP,CS_GRP_RET
 from capstone.x86 import X86_OP_IMM
 root=vector_source_root_scope(by,resolved,md,entries)
 need(root['reached_earlier_guard_nan_source_composes_to_vector_nan_store'],'affine_requires_composed_nan_path')
 captured=[];unordered_angle_status(by,resolved,md,entries,captured);chosen=[]
 for site in captured:
  try:r=angle_nan_reload_store(by,resolved,md,entries,site[4].address)
  except GateError:continue
  if r['site_role_predicates_unordered'][0]['vector_value1_role_conditional']:chosen.append(site)
 need(len(chosen)==1,'affine_unique_vector')
 pred={a:set() for a in by}
 for q in by.values():
  targets=set()
  if q.group(CS_GRP_RET):continue
  if q.group(CS_GRP_JUMP):
   if q.operands[0].type==X86_OP_IMM:targets.add(q.operands[0].imm)
   else:need(q.address==resolved[0],'affine_dispatch');targets.update(resolved[1])
  if not(q.group(CS_GRP_JUMP) and q.mnemonic=='jmp'):targets.add(q.address+q.size)
  for t in targets:need(t in pred,'affine_edge');pred[t].add(q.address)
 def prev(q):need(len(pred[q.address])==1,'affine_path_predecessor');return by[next(iter(pred[q.address]))]
 load=prev(chosen[0][0]);earlier=load
 for unused in range(5):earlier=prev(earlier)
 if witness_capture is not None:witness_capture['goal']=earlier.address
 result=prove_record_address(by,pred,resolved,md,entries,load,earlier.address)
 result['status']='CONDITIONAL_VECTOR_RECORD_AFFINE_AND_FRESHNESS'
 result['reached_earlier_guard_nan_source_composes_to_vector_nan_store']=True
 result['conditional_payload_float1_nan_store_when_earlier_guard_reached']=result['live_record_float1_identity_conditional']
 result['premises']+=root['premises']+['matched payload source remains stable across the entire composed earlier-guard-to-final-store path']
 result['complete_dispatch_to_requested_nan_behavior_proven']=False
 return result

def entry_witness(by,md,goal,kind,table_bytes):
 """One bounded, concrete integer-domain path; pointers retain disjoint roots."""
 from capstone.x86 import X86_OP_REG,X86_OP_MEM,X86_OP_IMM
 U=0xffffffff;gprs={'eax','ebx','ecx','edx','esi','edi','ebp','esp'}
 regs={n:('unknown',n) for n in gprs};regs.update(ecx=('object',0),esp=('stack',0))
 stack={0:('return',0)};fields={0x45c:0,0x43c:0,0x464:0x3f800000,0x474:kind,0x478:0};flags={};seen=set();pc=START
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
 need(goal in by and by[goal].mnemonic=='fld','witness_goal_guard')
 return {'reached_earlier_vector_guard_from_owner_entry':True,'instruction_count':len(seen)}

def vector_stages(by,resolved,md,entries,tables):
 out={'status':'PARTIAL_VECTOR_NAN_PREFIX_STAGES','earlier_guard_composition_proven':False,'record_affine_proven':False,'live_payload_float1_identity_proven':False,'concrete_entry_to_nan_result_store_proven':False,'unresolved_stages':{},'limits':['first vector result-field store/path-prefix only; no return-state or continuation equivalence','unproved later stages stay unresolved; no new entry-floating effects accepted; +65C freshness write requires exact independently proven receiver role'],'source_correction_authorized':False,'exactness_credit':'none'}
 try:
  root=vector_source_root_scope(by,resolved,md,entries)
  out['earlier_guard_composition_proven']=root['reached_earlier_guard_nan_source_composes_to_vector_nan_store']
  out['premises']=root['premises']
 except GateError as e:out['unresolved_stages']['earlier_guard_composition']=str(e);return out
 try:
  capture={};r=vector_affine_record(by,resolved,md,entries,capture)
  out['record_affine_proven']=r['affine_address_is_receiver_plus_464_plus_24_times_cursor_load_plus_4_mod32']
  out['live_payload_float1_identity_proven']=r['live_record_float1_identity_conditional']
  out['freshness_checked_instruction_occurrences']=r['freshness_checked_instruction_occurrences'];out['proven_receiver_vector_value0_move_occurrences']=r['proven_receiver_vector_value0_move_occurrences']
  out['premises']=r['premises']
  if not out['live_payload_float1_identity_proven']:out['unresolved_stages']['freshness']=r['freshness_unresolved_category'];return out
 except GateError as e:out['unresolved_stages']['affine_record']=str(e);return out
 try:
  witness=entry_witness(by,md,capture['goal'],0x10,tables)
  out['entry_instruction_count']=witness['instruction_count'];out['concrete_entry_to_nan_result_store_proven']=True
 except GateError as e:out['unresolved_stages']['entry_witness']=str(e)
 out['premises']+=['concrete entry witness: valid receiver ECX, cursor0, activeflags0, record0.allowWhileActive0, kind0x10, payload.float0 has finite float32 value1.0/raw bits0x3F800000, stable payload.float1 NaN','valid private 8-byte-aligned entry stack, opaque return cell, disjoint from receiver/source/code/tables; no asynchronous mutation']
 out['limits']=['first vector result-field store/path-prefix only; no owner-return-state or vector continuation equivalence','general all-index/all-state dispatch remains unproved','entry floating operations remain unsupported; only exact flags43C OR/MOV and vector-value0_65C MOV writes are allowed']
 return out

def collect(data,md,entries,image):
 capture={};r=cfg_collect(data,md,entries,image,capture)
 need(r['status']=='BOUNDED_REACHABLE_CFG_CLOSED' and r['reachable_byte_count']==SIZE,'postspawn_closed_cfg')
 d=r['dispatch_scope'];need(d['remap_sha256']=='a82463843ab4bcf47c8611e41926084c1b55330f6675fac171965ad29e4e3717' and d['pointer_table_sha256']=='9339bb47a5e46ed5dc8f9b8be47018e240ad41129e230e75e865aadcbb908bfd','postspawn_table_pins')
 out=vector_stages(capture['instructions'],capture['resolved'],md,entries,capture['table_bytes'])
 out['table_sha256']={'remap':d['remap_sha256'],'pointers':d['pointer_table_sha256']}
 return out
def main():
 need(sys.flags.optimize==0,'optimized_python');resource.setrlimit(resource.RLIMIT_CPU,(15,15));resource.setrlimit(resource.RLIMIT_AS,(768*1024*1024,768*1024*1024));signal.alarm(25)
 p=argparse.ArgumentParser();p.add_argument('--root',type=Path,required=True);p.add_argument('--target',type=Path,required=True);a=p.parse_args();root=a.root.resolve();need(a.target.resolve()==root/'resources/th10.exe','target_path')
 def git(*args):return subprocess.check_output(['git','-C',str(root),*args],text=True,timeout=5).strip()
 need(git('rev-parse','HEAD')==HEAD and git('status','--porcelain','--untracked-files=all')=='','repository_pin')
 for n,h in PINS.items():need(sha((a.target if n=='target' else root/n).read_bytes())==h,'file_pin')
 header=(root/'src/BulletManager.hpp').read_text()
 need('struct BulletTransformPayloadView\n{\n    float float0;\n    float float1;\n    int int0;\n    int int1;\n};' in header and 'struct BulletTransformRecordView\n{\n    BulletTransformPayloadView payload;\n    unsigned int kind;\n    int allowWhileActive;\n};' in header and '(sizeof(BulletTransformPayloadView) == 0x10)' in header and '(sizeof(BulletTransformRecordView) == 0x18)' in header and '(offsetof(BulletTransformRecordView, kind) == 0x10)' in header,'source_record_layout_float1_four_kind10')
 need('(offsetof(BulletRuntimeView, exStates) == 0x614)' in header and '(sizeof(BulletExStateView) == 0x34)' in header and '(offsetof(BulletExStateView, value0) == 0x14)' in header and 0x614+0x34+0x14==0x65c,'source_vector_value0_layout')
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
