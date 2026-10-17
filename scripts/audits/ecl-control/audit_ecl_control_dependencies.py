#!/usr/bin/env python3
"""Read-only pinned ECL structural gate and bounded semantic observations.
No target bytes, instruction text, addresses or register traces are emitted.
Counts and local motifs do not prove semantic equivalence. No files are written.
"""
import argparse
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import sys
import tomllib

PINS = {
    'target': '2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040',
    'config/functions.csv': '19d7e4eb1aab50f97afba28cd7a5c6ed529a771f18bd9ccf4997aee6a1954350',
    'config/match-units.toml': '8811679fb37ab220216a8926a9ce46b8637335e47cea7cd8d5d8a78b6004cc88',
    'config/tools.lock.toml': '24d77ab218e86ea8b888418922a31db9a3cf30f54bb73450a04967a4a501db9f',
    'scripts/linked_image.py': '58583c6241e0979288b0c4ce0a0e55f1ae2a178762d93743475d05518eff0354',
    'src/EclVm.cpp': '8f236b6d2bdfa73bbf8747b16e934eb62264d2d54a811ac5f5cec789a1457efb',
    'src/EclVm.hpp': 'a01e19c6bb2659e258444cd57018947fe6f5444522b75d2b5777e13f55ffe551',
}
OWNERS = {'Run': (0x44E1A0,7020,0x44FBC2),
          'StartSubroutine': (0x44DF70,550,0x44E196),
          'Pop': (0x450610,122,0x45068A),
          'LeaveFrame': (0x4506D0,41,0x4506F9)}
JT, ST, END, DEFAULT = 0x44FBC4,0x44FCB4,0x44FD0C,0x44FB68
class GateError(Exception): pass

def need(condition, label):
    if not condition: raise GateError(label)

def digest(data): return hashlib.sha256(data).hexdigest()

def check_pin(data, expected, label):
    need(digest(data)==expected, 'pin_mismatch_'+label)

def check_owner(name, start, size, end, rows):
    matches=[r for r in rows if int(r['address'],0)==start]
    need(len(matches)==1, 'owner_identity_'+name)
    row=matches[0]
    need(start < end <= start+size, 'code_owner_partition_'+name)
    need(int(row['size'])==size and int(row['span_end'],0)==start+size-1, 'owner_extent_'+name)
    need(row['proposed_name'].endswith('::'+name),'owner_role_'+name)
    for other in rows:
        a=int(other['address'],0);b=int(other['span_end'],0)+1
        need(a==start or not(a<start+size and start<b),'owner_overlap_'+name)

def check_partitions():
    start,size,code_end=OWNERS['Run']
    need(start < code_end and code_end+2==JT and JT+60*4==ST and ST+88==END and END==start+size and END+4==0x44FD10,'run_partition')

def check_dense(insns, start, end):
    cursor = start
    for ins in insns:
        need(ins.address == cursor and ins.size > 0, 'decode_coverage')
        cursor += ins.size
    need(cursor == end, 'decode_extent')

def check_tables(selectors, jumps, starts, source_values):
    need(len(selectors)==88 and len(jumps)==60, 'table_extent')
    need(len(set(jumps))==60, 'table_uniqueness')
    need(all(s < 60 for s in selectors), 'selector_range')
    need(set(selectors)==set(range(60)), 'selector_slot_coverage')
    need(all(j in starts for j in jumps), 'table_instruction_boundary')
    need([i for i,j in enumerate(jumps) if j==DEFAULT]==[59], 'default_slot')
    active = {i for i,s in enumerate(selectors) if jumps[s]!=DEFAULT}
    need(active == source_values and len(active)==59, 'source_case_coverage')

def check_branch_destination(dest, starts):
    need(dest in starts, 'branch_instruction_boundary')

def self_test():
    from types import SimpleNamespace as I
    passed=0
    def rejected(fn):
        nonlocal passed
        try: fn()
        except GateError: passed+=1; return
        raise AssertionError('negative gate accepted')
    seq=[I(address=100,size=2),I(address=102,size=1)]
    check_dense(seq,100,103)
    rejected(lambda:check_dense(seq,100,104))
    rejected(lambda:check_branch_destination(101,{100,102}))
    jumps=list(range(100,159))+[DEFAULT]
    selectors=bytes(list(range(60))+[59]*28)
    starts=set(jumps)
    check_tables(selectors,jumps,starts,set(range(59)))
    bad=bytearray(selectors); bad[0]=60
    rejected(lambda:check_tables(bad,jumps,starts,set(range(59))))
    badj=list(jumps);badj[0]=99
    rejected(lambda:check_tables(selectors,badj,starts,set(range(59))))
    rejected(lambda:check_pin(b'wrong',PINS['target'],'target'))
    rows=[{'address':'100','size':'10','span_end':'109','proposed_name':'C::X'}]
    check_owner('X',100,10,108,rows)
    rejected(lambda:check_owner('X',100,11,108,rows))
    rejected(lambda:check_owner('X',100,10,111,rows))
    overlap=rows+[{'address':'105','size':'3','span_end':'107','proposed_name':'C::Y'}]
    rejected(lambda:check_owner('X',100,10,108,overlap))
    check_partitions()
    print(json.dumps({'self_test':'PASS','negative_gate_cases':passed}))

"""Bounded syntactic control/data-dependency facts; no assumed helper ABI.
No target bytes, assembly, addresses or private registers are emitted.
"""
def analyze_dependencies(decoded,jumps,selectors):
    from capstone import CS_GRP_JUMP,CS_GRP_CALL,CS_GRP_RET
    from capstone.x86 import X86_OP_IMM,X86_OP_REG,X86_OP_MEM
    code={i.address:i for i in decoded['Run']}
    owner_roles={seq[0].address:name for name,seq in decoded.items()}
    common=jumps[selectors[0]]
    first_case=min(jumps)
    def rn(i,r):return i.reg_name(r)
    def writes_reg(i,name):
        _,wr=i.regs_access()
        return any(rn(i,r) in ({'eax','ax','al','ah'} if name=='eax' else {name}) for r in wr)
    def eax(o,i):return o.type==X86_OP_REG and rn(i,o.reg)=='eax'
    def basic(p):
        out=[]
        for _ in range(128):
            if p not in code:return None
            i=code[p];out.append(i);p+=i.size
            if i.group(CS_GRP_JUMP) or i.group(CS_GRP_RET):return out
        return None
    def immediate_result_predicate(seq):
        if not seq or seq[-1].mnemonic not in ('je','jz','jne','jnz'):return None
        positions=[n for n,i in enumerate(seq) if i.group(CS_GRP_CALL)]
        if len(positions)!=1:return None
        ci=positions[0];cmpi=None;constant=None
        for k in range(ci+1,len(seq)-1):
            i=seq[k];ops=i.operands
            if i.mnemonic=='test' and len(ops)==2 and eax(ops[0],i) and eax(ops[1],i):cmpi=k;constant=0
            elif i.mnemonic=='cmp' and len(ops)==2 and eax(ops[0],i) and ops[1].type==X86_OP_IMM:cmpi=k;constant=ops[1].imm
            else:
                if writes_reg(i,'eax'):return None
                if cmpi is not None and writes_reg(i,'eflags'):return None
        if cmpi is None:return None
        call=seq[ci]
        role=owner_roles.get(call.operands[0].imm,'other_known_owner') if call.operands[0].type==X86_OP_IMM else 'indirect_call'
        return {'role':role,'constant':constant,'equal_taken':seq[-1].mnemonic in ('je','jz')}
    def tail(p):
        # Only an explicit whitelist can establish a constant return with no
        # direct instruction/time stores. Calls and other effects abort proof.
        seen=set();value=None;count=0;reloads=0
        while count<64 and p not in seen:
            if p==common:return {'category':'common_nop_entry','return_constant':None}
            if p<first_case:return {'category':'pre_dispatch_code','return_constant':None,'gpr_reload_count':reloads}
            seen.add(p);i=code.get(p)
            if i is None:return {'category':'UNRESOLVED'}
            count+=1;m=i.mnemonic;ops=i.operands
            if i.group(CS_GRP_RET):return {'category':'return_without_explicit_nonstack_store','return_constant':value}
            if m=='jmp' and ops[0].type==X86_OP_IMM:p=ops[0].imm;continue
            if i.group(CS_GRP_JUMP) or i.group(CS_GRP_CALL):return {'category':'UNRESOLVED','stop_category':'conditional_branch' if i.group(CS_GRP_JUMP) else 'call'}
            if m=='xor' and len(ops)==2 and eax(ops[0],i) and eax(ops[1],i):value=0
            elif m=='or' and len(ops)==2 and eax(ops[0],i) and ops[1].type==X86_OP_IMM and ops[1].imm in (-1,0xffffffff):value=-1
            elif m=='mov' and len(ops)==2 and eax(ops[0],i) and ops[1].type==X86_OP_IMM:value=ops[1].imm
            elif m=='pop' and len(ops)==1 and ops[0].type==X86_OP_REG:
                if eax(ops[0],i):value=None
            elif m in ('add','sub') and len(ops)==2 and ops[0].type==X86_OP_REG and rn(i,ops[0].reg)=='esp' and ops[1].type==X86_OP_IMM:pass
            elif m=='mov' and len(ops)==2 and ops[0].type==X86_OP_REG and rn(i,ops[0].reg)=='esp' and ops[1].type==X86_OP_REG and rn(i,ops[1].reg)=='ebp':pass
            elif m in ('mov','lea') and len(ops)==2 and ops[0].type==X86_OP_REG and ops[0].size==4 and rn(i,ops[0].reg)!='esp' and ops[1].type==X86_OP_MEM:
                reloads+=1
                if eax(ops[0],i):value=None
            elif m in ('nop','leave'):pass
            else:return {'category':'UNRESOLVED','stop_category':'floating_operation' if m.startswith('f') else 'comparison' if m in ('cmp','test') else 'other_operation'}
            p+=i.size
        return {'category':'UNRESOLVED'}
    def slot(o):
        if o.type!=X86_OP_MEM:return None
        m=o.mem;return (m.base,m.index,m.scale,m.disp,o.size,getattr(m,'segment',0))
    def common_block():
        p=common;seen=set();hops=0
        while p not in seen and hops<=4:
            seen.add(p);seq=basic(p)
            if not seq:return None,hops
            if len(seq)==1 and seq[0].mnemonic=='jmp' and seq[0].operands[0].type==X86_OP_IMM:
                p=seq[0].operands[0].imm;hops+=1;continue
            return seq,hops
        return None,hops
    def dependency_chain():
        seq,hops=common_block()
        if not seq:return {'established':False,'failure_category':'missing_bounded_block'}
        regs={};generation=0;loads=0;sizes=0;adds=0;stores=0
        unknown=('unknown',)
        gprs={'eax','ebx','ecx','edx','esi','edi','ebp','esp'}
        aliases={'al':'eax','ah':'eax','ax':'eax','bl':'ebx','bh':'ebx','bx':'ebx','cl':'ecx','ch':'ecx','cx':'ecx','dl':'edx','dh':'edx','dx':'edx','si':'esi','di':'edi','sp':'esp','bp':'ebp'}
        def reg(i,r):
            name=rn(i,r)
            if name not in gprs:return unknown
            return regs.get(name,('entry_register',name))
        def summation(a,b):
            if a==unknown or b==unknown:return unknown
            terms=[];number=0
            for v in (a,b):
                if isinstance(v,int):number=(number+v)&0xffffffff
                elif v[0]=='sum':terms.extend(v[1]);number=(number+v[2])&0xffffffff
                else:terms.append(v)
            terms=tuple(sorted(terms,key=repr))
            if not terms:return number
            if len(terms)==1 and number==0:return terms[0]
            return ('sum',terms,number)
        def effective(i,o):
            m=o.mem
            if getattr(m,'segment',0):return unknown
            v=reg(i,m.base) if m.base else 0
            if m.index:
                if m.scale!=1:return unknown
                v=summation(v,reg(i,m.index))
            return summation(v,m.disp)
        def components(v):
            return (v[1],v[2]) if isinstance(v,tuple) and v[0]=='sum' else ((v,),0)
        def load_plus_six(address):
            terms,number=components(address)
            if number==6 and len(terms)==1 and isinstance(terms[0],tuple) and terms[0][0]=='load32':return terms[0]
            return None
        def advanced(v):
            terms,number=components(v)
            if number or len(terms)!=2:return None
            for ptr,sz in (terms,terms[::-1]):
                if isinstance(ptr,tuple) and ptr[0]=='load32' and sz==('size16',ptr):return ptr
            return None
        def fail(category):return {'established':False,'failure_category':category,'dword_load_count':loads,'size_origin_count':sizes,'add_count':adds,'store_count':stores}
        for i in seq:
            m=i.mnemonic;o=i.operands
            if i.group(CS_GRP_JUMP) or i.group(CS_GRP_RET):return fail('control_boundary_before_matching_store')
            if i.group(CS_GRP_CALL):return fail('call_barrier')
            if m=='nop':continue
            if m not in ('mov','movzx','add','lea'):
                # Unsupported operations are not assumed harmless, even if the
                # decoder reports no obvious memory write. Invalidate and stop.
                generation+=1
                _,written=i.regs_access()
                for r in written:
                    name=rn(i,r);regs[aliases.get(name,name)]=unknown
                return fail('unsupported_'+('floating_operation' if m.startswith('f') else 'comparison' if m in ('cmp','test') else 'other_operation'))
            if len(o)!=2:return fail('unsupported_operand_shape')
            dst,src=o
            if dst.type==X86_OP_MEM:
                if m!='mov':return fail('unsupported_memory_update')
                address=effective(i,dst);value=reg(i,src.reg) if src.type==X86_OP_REG and src.size==4 else unknown
                ptr=advanced(value);stores+=1
                if dst.size==4 and address!=unknown and ptr is not None and ptr[1]==address and ptr[2]==generation:
                    return {'established':True,'scope':'generic_same_slot_unsigned_size_pointer_advance',
                            'dword_load_count':loads,'size_origin_count':sizes,'add_count':adds,'store_count':stores,
                            'intervening_store_count':generation,'context_cursor_identity':'UNRESOLVED'}
                # Any other memory store might alias every earlier loaded cell.
                generation+=1
                return fail('intervening_or_nonmatching_store')
            if dst.type!=X86_OP_REG or dst.size!=4 or rn(i,dst.reg) not in gprs:return fail('partial_or_unsupported_destination')
            name=rn(i,dst.reg)
            if m=='mov':
                if src.type==X86_OP_REG and src.size==4 and rn(i,src.reg) in gprs:value=reg(i,src.reg)
                elif src.type==X86_OP_IMM:value=src.imm&0xffffffff
                elif src.type==X86_OP_MEM and src.size==4:
                    address=effective(i,src)
                    if address==unknown:return fail('unsupported_load_address')
                    value=('load32',address,generation);loads+=1
                else:return fail('unsupported_move_width')
            elif m=='movzx':
                if src.type!=X86_OP_MEM or src.size!=2:return fail('unsupported_zeroextension')
                address=effective(i,src)
                if address==unknown:return fail('unsupported_size_address')
                ptr=load_plus_six(address)
                if ptr is not None and ptr[2]==generation:value=('size16',ptr);sizes+=1
                else:value=unknown
            elif m=='lea':
                if src.type!=X86_OP_MEM:return fail('unsupported_lea')
                value=effective(i,src)
                if value==unknown:return fail('unsupported_lea_address')
            else:
                if src.type==X86_OP_REG and src.size==4 and rn(i,src.reg) in gprs:right=reg(i,src.reg)
                elif src.type==X86_OP_IMM:right=src.imm&0xffffffff
                else:return fail('unsupported_add_source')
                value=summation(reg(i,dst.reg),right);adds+=1
            regs[name]=value
        return fail('no_matching_store')
    def common_shape():
        seq,hops=common_block()
        if not seq:return {'status':'UNRESOLVED','jump_prefix_hops':hops}
        counts={'dword_memory_loads':0,'dword_memory_stores':0,'zeroextend_word_loads':0,'register_adds':0,'memory_adds':0,'register_moves':0}
        for i in seq:
            o=i.operands
            if i.mnemonic=='mov' and len(o)==2:
                if o[0].type==X86_OP_REG and o[1].type==X86_OP_MEM and o[1].size==4:counts['dword_memory_loads']+=1
                if o[0].type==X86_OP_MEM and o[1].type==X86_OP_REG and o[0].size==4:counts['dword_memory_stores']+=1
                if all(v.type==X86_OP_REG for v in o):counts['register_moves']+=1
            if i.mnemonic=='movzx' and len(o)==2 and o[1].type==X86_OP_MEM and o[1].size==2:counts['zeroextend_word_loads']+=1
            if i.mnemonic=='add' and len(o)==2:
                if all(v.type==X86_OP_REG for v in o):counts['register_adds']+=1
                if o[0].type==X86_OP_MEM:counts['memory_adds']+=1
        return {'jump_prefix_hops':hops,'instruction_count':len(seq),'role_shape_counts':counts}
    result={'common_advance':{'strict_straight_line_dependency':dependency_chain(),'bounded_shape':common_shape(),
                             'context_cursor_slot_identity':'UNRESOLVED'},
            'normal_execution_only':True,'faulting_accesses_modeled':False,
            'helper_register_contracts':'UNRESOLVED',
            'difficulty_skip_to_common_advance':'UNRESOLVED'}
    for name,entry in [('CALL',jumps[selectors[11]]),('DEFAULT',jumps[59])]:
        seq=basic(entry);pred=immediate_result_predicate(seq)
        if pred is None:result[name]={'status':'UNRESOLVED'};continue
        last=seq[-1]
        if last.operands[0].type!=X86_OP_IMM:result[name]={'status':'UNRESOLVED'};continue
        taken=tail(last.operands[0].imm);fall=tail(last.address+last.size)
        result[name]={'status':'LOCAL_CONTROL_DEPENDENCY_ESTABLISHED',
                      'call_role':pred['role'],'comparison_constant':pred['constant'],
                      'equal_successor':taken if pred['equal_taken'] else fall,
                      'unequal_successor':fall if pred['equal_taken'] else taken,
                      'receiver_arguments_and_prefix_time_gate':'UNRESOLVED'}
    # RETURN: extract only its first block's one named helper call; subsequent
    # fields/argument pointers require helper-preservation evidence not assumed.
    seq=basic(jumps[selectors[10]])
    roles=[]
    if seq:
        for i in seq:
            if i.group(CS_GRP_CALL) and i.operands[0].type==X86_OP_IMM:roles.append(owner_roles.get(i.operands[0].imm,'other_known_owner'))
    result['RETURN']={'status':'UNRESOLVED','first_block_direct_call_roles':roles}
    return result


def main():
    need(sys.flags.optimize == 0, 'optimized_python_not_permitted')
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root',type=Path)
    parser.add_argument('--target',type=Path)
    parser.add_argument('--self-test',action='store_true')
    args=parser.parse_args()
    if args.self_test: self_test();return
    need(args.root is not None and args.target is not None,'explicit_paths_required')
    root=args.root.resolve(); target=args.target.resolve()
    # Verify trusted code before importing, and all inputs before decoding.
    for name,pin in PINS.items():
        path=target if name=='target' else root/name
        check_pin(path.read_bytes(),pin,name)
    spec=importlib.util.spec_from_file_location('linked_image',root/'scripts/linked_image.py')
    linked=importlib.util.module_from_spec(spec);sys.modules[spec.name]=linked;spec.loader.exec_module(linked)
    lock=tomllib.loads((root/'config/tools.lock.toml').read_text())
    decoder_identity=linked.verify_capstone(lock['capstone'])
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_GRP_CALL, CS_GRP_RET
    from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
    image=linked.PEImage(target)
    need(image.image_base==0x400000,'image_base')
    rows=list(csv.DictReader((root/'config/functions.csv').open(newline='')))
    rowmap={int(r['address'],0):r for r in rows}
    need(len(rowmap)==len(rows),'duplicate_owners')
    check_partitions()
    for name,(start,size,end) in OWNERS.items():
        check_owner(name,start,size,end,rows)
    source=(root/'src/EclVm.cpp').read_text()
    pairs=re.findall(r'^\s*(ECL_VM_[A-Z0-9_]+)\s*=\s*(0x[0-9A-Fa-f]+),',source,re.M)
    enums={n:int(v,16) for n,v in pairs}
    cases=re.findall(r'^\s*case\s+(ECL_VM_[A-Z0-9_]+):',source,re.M)
    need(len(pairs)==len(enums)==len(set(enums.values())),'enum_duplicates')
    need(len(cases)==len(set(cases)) and set(cases)==set(enums),'enum_case_identity')
    md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
    decoded={};allstarts=set()
    for name,(start,size,end) in OWNERS.items():
        insns=list(md.disasm(image.read_address(start,end-start),start))
        check_dense(insns,start,end)
        decoded[name]=insns;allstarts.update(i.address for i in insns)
    starts={i.address for i in decoded['Run']}
    jumps=struct.unpack('<60I',image.read_address(JT,240))
    selectors=image.read_address(ST,88)
    check_tables(selectors,jumps,starts,set(enums.values()))
    need(image.read_address(0x44FBC2,2)==b'\x8b\xff','alignment')
    need(image.read_address(END,4)==b'\xcc'*4,'following_padding')
    need(0x44FD10 in rowmap,'next_owner')
    direct_branch_count=0;direct_call_count=0;indirect_call_count=0;switch_count=0
    for name,insns in decoded.items():
        ownstarts={i.address for i in insns}
        for ins in insns:
            if ins.group(CS_GRP_JUMP):
                if ins.operands[0].type==X86_OP_IMM:
                    check_branch_destination(ins.operands[0].imm,ownstarts)
                    direct_branch_count+=1
                else:
                    # The only admitted indirect transfer is the validated Run table.
                    op=ins.operands[0]
                    need(name=='Run' and ins.mnemonic=='jmp' and op.type==X86_OP_MEM and op.mem.disp==JT and op.mem.scale==4 and op.mem.base==0 and op.mem.index!=0,'unknown_indirect_branch')
                    switch_count+=1
            elif ins.group(CS_GRP_CALL):
                op=ins.operands[0]
                if op.type==X86_OP_IMM:
                    # Direct helper destinations must be known owner entries, not interiors.
                    need(op.imm in rowmap,'unknown_direct_call_owner')
                    direct_call_count+=1
                else: indirect_call_count+=1
    need(switch_count==1,'switch_count')
    # Collect only first-basic-block observations at reviewed case table roots.
    # These facts intentionally do not claim path completeness or field identity.
    byaddr={i.address:i for i in decoded['Run']}
    role_by_address={v[0]:k for k,v in OWNERS.items()}
    def first_block(root_address):
        seq=[];p=root_address
        for _ in range(128):
            need(p in byaddr,'case_decode_boundary')
            i=byaddr[p];seq.append(i);p+=i.size
            if i.group(CS_GRP_JUMP) or i.group(CS_GRP_RET): return seq
        raise GateError('case_block_bound')
    def observations(seq):
        calls=[];indirect=0;minus_one_comparisons=0;zero_pushes=0
        for i in seq:
            if i.group(CS_GRP_CALL):
                op=i.operands[0]
                if op.type==X86_OP_IMM:calls.append(role_by_address.get(op.imm,'other_known_owner'))
                else:indirect+=1
            if i.mnemonic=='cmp' and any(o.type==X86_OP_IMM and o.imm==-1 for o in i.operands):minus_one_comparisons+=1
            if i.mnemonic=='push' and i.operands[0].type==X86_OP_IMM and i.operands[0].imm==0:zero_pushes+=1
        term=seq[-1]
        return {'instruction_count':len(seq),'direct_call_roles':calls,'indirect_call_count':indirect,
                'minus_one_literal_comparison_count':minus_one_comparisons,'zero_immediate_push_count':zero_pushes,
                'terminator_category':('conditional_branch' if term.group(CS_GRP_JUMP) and term.mnemonic!='jmp' else 'jump' if term.group(CS_GRP_JUMP) else 'return')}
    questions={}
    for name,opcode in [('CALL',11),('RETURN',10),('DEFAULT',2)]:
        address=DEFAULT if name=='DEFAULT' else jumps[selectors[opcode]]
        questions[name]={'status':'UNRESOLVED','first_block_observations':observations(first_block(address)),
                         'reason':'First-block call/literal observations do not prove receiver, path effects, or callback-sensitive dataflow.'}
    questions['DIFFICULTY_AND_COMMON_ADVANCE']={'status':'UNRESOLVED','reason':'Full field-base and path dataflow proof required; structural gates alone are insufficient.'}
    questions['START_SUBROUTINE_CALLBACK_RELOADS']={'status':'UNRESOLVED','reason':'Requires cross-callback alias/dataflow proof; no allocation differences interpreted.'}
    report={'schema':1,'audit':'read_only_source_semantics_preflight','gates':'PASS','pins':PINS,
            'decoder_version':decoder_identity['version'],
            'counts':{'owners':4,'decoded_instructions':sum(map(len,decoded.values())),
                      'direct_branches':direct_branch_count,'direct_calls':direct_call_count,
                      'indirect_calls':indirect_call_count,'selector_entries':88,'jump_entries':60,'active_cases':59},
            'questions':questions,'routing_dependency_facts':analyze_dependencies(decoded,jumps,selectors),'source_change_justified':False}
    print(json.dumps(report,sort_keys=True))

if __name__=='__main__':
    try: main()
    except GateError as e:
        print(json.dumps({'gates':'FAIL','category':str(e),'semantic_output_suppressed':True}));sys.exit(2)
    except Exception:
        # Never emit exception representations containing target-derived instruction data.
        print(json.dumps({'gates':'FAIL','category':'input_or_decoder_failure','semantic_output_suppressed':True}));sys.exit(2)
