#!/usr/bin/env python3
"""Conditional baseline structural call ordering; diagnostic image is non-runnable."""
import ast
import csv
import hashlib
import contextlib
import io
import json
import os
from pathlib import Path
import re
import resource
import signal
import struct
import subprocess
import sys
import tomllib
import types

PINS = {
 'scripts/report-ecl-vm-table.py':'1bbe0eb9632d7c903d8059782b1f23191987ef74cf847dae873008e2789da96e',
 'scripts/target_identity.py':'c7fefbe43382a0f45e0d30a90b60d938d9622f62e746caf50a3851c98709d663',
 'scripts/linked_image.py':'58583c6241e0979288b0c4ce0a0e55f1ae2a178762d93743475d05518eff0354',
 'config/tools.lock.toml':'24d77ab218e86ea8b888418922a31db9a3cf30f54bb73450a04967a4a501db9f',
 'config/target.toml':'385ecdeac9ac89bf9706a7db766ef1ad2d6d33b2f50ee360c72eae3d67932789',
 'config/functions.csv':'19d7e4eb1aab50f97afba28cd7a5c6ed529a771f18bd9ccf4997aee6a1954350',
 'config/match-units.toml':'8811679fb37ab220216a8926a9ce46b8637335e47cea7cd8d5d8a78b6004cc88',
 'src/EclVm.cpp':'8f236b6d2bdfa73bbf8747b16e934eb62264d2d54a811ac5f5cec789a1457efb',
 'src/EclVm.hpp':'a01e19c6bb2659e258444cd57018947fe6f5444522b75d2b5777e13f55ffe551',
}
TARGET_HASH='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
CONSTANTS={'FUNCTION_ADDRESS':0x44E1A0,'CODE_END_ADDRESS':0x44FBC1,
 'ALIGNMENT_ADDRESS':0x44FBC2,'JUMP_TABLE_ADDRESS':0x44FBC4,
 'JUMP_TABLE_ENTRIES':60,'SELECTOR_TABLE_ADDRESS':0x44FCB4,
 'SELECTOR_TABLE_ENTRIES':88,'OWNER_END_ADDRESS':0x44FD0B,
 'NEXT_FUNCTION_ADDRESS':0x44FD10,'DEFAULT_DESTINATION':0x44FB68}
HELPERS={
 'ecl-vm-read-float':('EclVmContext::ReadFloat','?ReadFloat@EclVmContext@@QAEMI@Z',0x44FE40,183),
 'ecl-vm-resolve-float':('EclVmContext::ResolveFloat','?ResolveFloat@EclVmContext@@QAEPAMI@Z',0x450070,93),
 'enemy-wrap-angle':('EnemyWrapAngle','?EnemyWrapAngle@@YGMM@Z',0x44BC70,94)}
BUDGETS={'prefix':(256,512,512),'case':(128,256,256)}
ZERO='0'*64
HEAD='a4edda20e434cd40ee99ea3b6b68673a80c3e3be'
EXPECTED_ROOT='/home/pentester/coding/codex_ida/th10-reconstruction/th10'
EXPECTED_TARGET=EXPECTED_ROOT+'/resources/th10.exe'
OUTPUT_LIMIT=8192
ANALYSIS={'origin':CONSTANTS['FUNCTION_ADDRESS'],'selector':CONSTANTS['SELECTOR_TABLE_ADDRESS'],
          'jump':CONSTANTS['JUMP_TABLE_ADDRESS'],'calls':{0x44FE40:'read',0x450070:'resolve',0x44BC70:'wrap'}}
BASELINE_REL='.analysis/ecl-run-baseline-20261018-a002/candidate/build/match-linked/EclVmHostRunEnemy'
BASELINE_PINS={
 'source.exe':'7ceaba074b38dc02c58cdbfecf7a730aec77876f7cd416b6bfde1e2cb87b84dd',
 'source.map':'3a90b15706065796ebfdd3dace3ff774aafde88168c9f3721c940c475ff6d5e6',
 'source.pdb':'b9f06a62186600215dd6b5c736df90bdfc580e78101b48a8ca0615c09f330897',
 'source.ltcg.obj':'3b66755ab2ba8568e1eda1ea248d225baa853eb114ad2af45b62dc6a33403ded',
 'support-00-Enemy.ltcg.obj':'3c3468497ab4dd7753b7e2b6461c1574e6541dbb6dbc171494e7fa70d4507723',
 'link-aliases.rsp':'4d6adc20f443b92bad96949550bc186c75df361ca99d81c5ec24b20b4f313472',
 'link-anchors.c':'d1c496d458110a55dd22a00bfc09982d71436c446a74e6e2e090a5bd470226b4',
 'link-anchors.obj':'43670fefaf0d7322aba7400004c173d9e5ea49784d72f555d0daebbfd2ad72f8'}
BASELINE_OWNER_HASH='98f613dc9838da5b5879304e255c32409b8a0dbb9141f24a7c6d1cd66e42ce39'
RUN_REL='.analysis/ecl-run-baseline-20261018-a002'
RUN_PINS={'manifest.json':'3756f6c56689cdba2ea9c66c598bacb98aadc8280ce3e2e3732a8416ff56081e',
          'runner.py':'cb83691809cd57f4e6157987fc81dfd5194b5a426c6fa70cb018e4add3b07755'}
PINS['scripts/ltcg_link.py']='3c7728e58d452760baa2ab076c16169e6b8a45fabbc35692b60493f08921f98b'

def sha(data): return hashlib.sha256(data).hexdigest()

def blank():
    r={k:False for k in ('source_pins_valid','helper_identity_valid','constants_valid',
      'decoder_pin_valid','target_identity_valid','whole_owner_valid','table_shape_valid',
      'source_case_coverage_valid','dispatch_association_valid','continuation_valid',
      'order_resolved','destination_before_read_on_all_paths','read_before_destination_on_all_paths')}
    for k in ('scope_valid','resource_bounds_valid','repository_pre_valid','repository_post_valid',
              'post_pins_valid','post_target_valid','owner_mapping_valid'):r[k]=False
    r.update(unresolved=True,metadata_only=True,acceptance_authority_none=True,
             normal_returning_paths_only=True)
    r['metadata_only']=False
    r.update(baseline_image=True,baseline_non_runnable=True,structural_call_labels_only=True,
             hypothetical_anchor_normal_return_assumed=True,wrap_is_diagnostic_anchor=False,
             baseline_artifact_pins_valid=False,baseline_artifact_postpins_valid=False,
             baseline_pdb_identity_valid=False,baseline_owner_attribution_valid=False,
             baseline_helper_owners_valid=False,baseline_alias_provenance_valid=False,
             baseline_table_shape_valid=False,target_owner_sha256=ZERO,
             baseline_image_sha256=ZERO,baseline_map_sha256=ZERO,baseline_pdb_sha256=ZERO,
             baseline_alias_sha256=ZERO,baseline_anchor_source_sha256=ZERO,
             baseline_anchor_object_sha256=ZERO,baseline_wrap_symbol_count_capped=0,
             baseline_anchor_symbol_count_capped=0,baseline_anchor_contribution_count_capped=0)
    r.update(local_order_resolved=False,local_destination_before_read=False,
             local_read_before_destination=False,local_read_before_wrap=False,
             local_wrap_before_read=False,local_reference_consistent=False,
             conditional_entry_assumption=True,full_opcode_semantics_proved=False,
             immutable_code_and_tables_assumed=True,
             ordinary_noncall_completion_assumed=True,
             no_exception_or_nonlocal_transfer_assumed=True,
             infeasible_branches_conservatively_included=True,
             local_state_budget_exhausted=False,local_control_valid=False,
             local_one_each_every_exit=False,local_nonvacuous_exits=False,
             local_state_count=0,local_exit_state_count=0)
    for k in ('target_sha256','owner_sha256','config_sha256','reporter_sha256',
              'verifier_sha256','decoder_lock_sha256'): r[k]=ZERO
    for k in ('owner_size','active_opcode_count','default_opcode_count','failure_count'):r[k]=0
    for p in BUDGETS:
      for k in ('instruction_count','edge_count','state_count','outbound_count',
                'direct_exit_count','fallthrough_exit_count','return_exit_count',
                'indirect_exit_count','unknown_call_count','read_call_count',
                'resolve_call_count','wrap_call_count','selector_read_shape_count',
                'jump_table_shape_count','conditional_branch_count'):
        r[p+'_'+k]=0
      for k in ('completed','budget_exhausted','loop_detected','decode_invalid',
                'all_outbound_same_destination','has_outbound'):
        r[p+'_'+k]=False
    return r

def load_module(name,path,source):
    mod=types.ModuleType(name)
    mod.__file__=str(path)
    absent=object();previous=sys.modules.get(name,absent)
    sys.modules[name]=mod
    try:
      exec(compile(source,str(path),'exec'),mod.__dict__)
    except BaseException:
      if previous is absent:sys.modules.pop(name,None)
      else:sys.modules[name]=previous
      raise
    return mod

def no_symlinks(path):
    path=Path(path)
    if not path.is_absolute():return False
    return all(not part.is_symlink() for part in (path,*path.parents))

def repo_clean(root):
    env=os.environ.copy();env['GIT_OPTIONAL_LOCKS']='0'
    def git(*args):
      cp=subprocess.run(['git','-c','core.fsmonitor=false','-c','core.untrackedCache=false',*args],cwd=root,env=env,stdout=subprocess.PIPE,stderr=subprocess.DEVNULL,timeout=5,check=True)
      if len(cp.stdout)>4096:raise RuntimeError()
      return cp.stdout
    return git('rev-parse','HEAD').decode().strip()==HEAD and git('status','--porcelain=v1','--untracked-files=all')==b''

def limit_resources():
    def timeout(signum,frame):raise RuntimeError()
    signal.signal(signal.SIGALRM,timeout);signal.alarm(58)
    signal.signal(signal.SIGXCPU,timeout)
    os.nice(10)
    cpus=os.sched_getaffinity(0);os.sched_setaffinity(0,{min(cpus)})
    resource.setrlimit(resource.RLIMIT_CPU,(29,30))
    resource.setrlimit(resource.RLIMIT_AS,(768*1024*1024,768*1024*1024))
    resource.setrlimit(resource.RLIMIT_FSIZE,(0,0))
    sys.dont_write_bytecode=True

def owner_slice(data):
    # All reads use the hash-verified immutable buffer; no PEImage file reopen.
    if len(data)<64 or data[:2]!=b'MZ':raise ValueError()
    pe=struct.unpack_from('<I',data,0x3c)[0]
    if data[pe:pe+4]!=b'PE\0\0':raise ValueError()
    machine,n=struct.unpack_from('<HH',data,pe+4)
    optsize=struct.unpack_from('<H',data,pe+20)[0];opt=pe+24
    if machine!=0x14c or optsize<96 or opt+optsize>len(data):raise ValueError()
    if struct.unpack_from('<H',data,opt)[0]!=0x10b or struct.unpack_from('<I',data,opt+28)[0]!=0x400000:raise ValueError()
    rva=CONSTANTS['FUNCTION_ADDRESS']-0x400000;end=rva+7020;candidates=[]
    for i in range(n):
      off=opt+optsize+i*40
      if off+40>len(data):raise ValueError()
      vs,va,rs,ro=struct.unpack_from('<IIII',data,off+8)
      flags=struct.unpack_from('<I',data,off+36)[0]
      if ro+rs>len(data):raise ValueError()
      if va<end and rva<va+max(vs,rs):
        if not (va<=rva and end<=va+vs and end<=va+rs and flags&0x20 and flags&0x20000000):raise ValueError()
        candidates.append((ro+rva-va,7020))
    if len(candidates)!=1:raise ValueError()
    off,size=candidates[0];return data[off:off+size]

def table_metadata(owner,source):
    start=CONSTANTS['FUNCTION_ADDRESS']
    jt=CONSTANTS['JUMP_TABLE_ADDRESS']-start
    st=CONSTANTS['SELECTOR_TABLE_ADDRESS']-start
    table=struct.unpack_from('<60I',owner,jt)
    selectors=owner[st:st+88]
    assert len(owner)==7020 and len(selectors)==88
    assert len(set(table))==60
    assert all(start <= x <= CONSTANTS['CODE_END_ADDRESS'] for x in table)
    assert all(x<60 for x in selectors)
    assert [i for i,x in enumerate(table) if x==CONSTANTS['DEFAULT_DESTINATION']]==[59]
    active={i for i,x in enumerate(selectors) if x!=59}
    assert len(active)==59
    pairs=re.findall(r'^\s*(ECL_VM_[A-Z0-9_]+)\s*=\s*(0x[0-9a-fA-F]+),',source,re.M)
    cases=re.findall(r'^\s*case\s+(ECL_VM_[A-Z0-9_]+):',source,re.M)
    assert len(pairs)==len(cases)==59
    assert len(set(cases))==59 and {x for x,_ in pairs}==set(cases)
    assert {int(x,16) for _,x in pairs}==active
    entry=table[selectors[0x52]]
    assert selectors[0x52]!=59
    end=min([x for x in table if x>entry]+[CONSTANTS['ALIGNMENT_ADDRESS']])
    return table,entry,end

def scan(decoder,owner,start,end,prefix,out):
    """Unordered structural metadata only; never recognizes successful continuation."""
    import capstone as cs
    from capstone.x86 import X86_OP_IMM,X86_OP_MEM
    origin=ANALYSIS['origin']
    limits=BUDGETS[prefix]
    pending=[start]; seen={}; spans=[]; graph={}; outbound=[];exits=[]
    failed=False
    def inc(key): out[prefix+'_'+key]+=1
    def edge(src,dst,kind):
      nonlocal failed
      if out[prefix+'_edge_count']>=limits[1]:
        out[prefix+'_budget_exhausted']=True;failed=True;return
      inc('edge_count')
      if start<=dst<end:
        graph.setdefault(src,[]).append(dst);pending.append(dst)
      else:
        outbound.append(dst);inc('outbound_count');inc(kind+'_exit_count')
        exits.append((src,dst,kind))
    while pending and not failed:
      if out[prefix+'_state_count']>=limits[2]:
        out[prefix+'_budget_exhausted']=True;failed=True;break
      pc=pending.pop();inc('state_count')
      if pc in seen:continue
      if out[prefix+'_instruction_count']>=limits[0]:
        out[prefix+'_budget_exhausted']=True;failed=True;break
      ins=next(decoder.disasm(owner[pc-origin:min(end-origin,pc-origin+15)],pc,count=1),None)
      if ins is None or ins.address!=pc or pc+ins.size>end or any(a<pc<b or pc<a<pc+ins.size for a,b in spans):
        out[prefix+'_decode_invalid']=True;failed=True;break
      seen[pc]=ins;spans.append((pc,pc+ins.size));inc('instruction_count')
      for op in ins.operands:
        if op.type==X86_OP_MEM and op.mem.disp==ANALYSIS['selector'] and op.size==1 and op.mem.scale==1 and (op.mem.index or op.mem.base):
          inc('selector_read_shape_count')
      if ins.mnemonic in ('ud0','ud1','ud2','hlt','int','int1','int3','into','iret','iretd','iretq','syscall','sysenter','sysexit','sysret','sysretq','rsm','xbegin','xabort','xend') or any(ins.group(g) for g in (cs.CS_GRP_INT,cs.CS_GRP_IRET,cs.CS_GRP_PRIVILEGE)):
        out[prefix+'_decode_invalid']=True;failed=True;break
      if ins.group(cs.CS_GRP_CALL):
        target=ins.operands[0].imm if len(ins.operands)==1 and ins.operands[0].type==X86_OP_IMM else None
        names=ANALYSIS['calls']
        if target not in names:
          inc('unknown_call_count');failed=True;break
        inc(names[target]+'_call_count');edge(pc,pc+ins.size,'fallthrough');continue
      if ins.group(cs.CS_GRP_RET):inc('return_exit_count');continue
      if ins.group(cs.CS_GRP_JUMP):
        op=ins.operands[0] if len(ins.operands)==1 else None
        if op is None or op.type!=X86_OP_IMM:
          inc('indirect_exit_count')
          if op and op.type==X86_OP_MEM and op.mem.disp==ANALYSIS['jump'] and op.size==4 and op.mem.scale==4 and op.mem.index and not op.mem.base:
            inc('jump_table_shape_count')
          # It is a stop, never a proven dispatcher or normal exit.
          continue
        edge(pc,op.imm,'direct')
        if ins.mnemonic!='jmp':inc('conditional_branch_count');edge(pc,pc+ins.size,'fallthrough')
        continue
      edge(pc,pc+ins.size,'fallthrough')
    # Reject cycles; converging acyclic edges are allowed.
    colors={}
    def cycle(n):
      if colors.get(n)==1:return True
      if colors.get(n)==2:return False
      colors[n]=1
      if any(cycle(v) for v in graph.get(n,[])):return True
      colors[n]=2;return False
    if cycle(start):out[prefix+'_loop_detected']=True;failed=True
    out[prefix+'_has_outbound']=bool(outbound)
    out[prefix+'_all_outbound_same_destination']=bool(outbound) and len(set(outbound))==1
    exit_count=len(outbound)+out[prefix+'_return_exit_count']+out[prefix+'_indirect_exit_count']
    out[prefix+'_completed']=not failed and bool(seen) and exit_count>0
    return seen,graph,exits

def prove_local(start,seen,graph,exits,r):
    """Finite call-state proof, conditioned on entry and normal helper returns."""
    import capstone as cs
    from capstone.x86 import X86_OP_IMM,X86_INS_CALL
    if not r['case_completed'] or r['case_indirect_exit_count'] or r['case_return_exit_count'] or r['case_fallthrough_exit_count']:return
    if not exits or any(kind!='direct' for _,_,kind in exits):return
    names=ANALYSIS['calls']
    calls={}
    for pc,ins in seen.items():
      if ins.group(cs.CS_GRP_CALL):
        if ins.id!=X86_INS_CALL or ins.size!=5 or bytes(ins.bytes[:1])!=b'\xe8' or len(ins.operands)!=1 or ins.operands[0].type!=X86_OP_IMM or ins.operands[0].size!=4 or ins.operands[0].imm not in names:return
        calls[pc]=names[ins.operands[0].imm]
    if start not in seen or any(n not in seen or any(v not in seen for v in dests) for n,dests in graph.items()):return
    exit_sources={src for src,_,_ in exits}
    if any(pc not in graph and pc not in exit_sources for pc in seen):return
    r['local_control_valid']=True
    pending=[(start,())];visited=set();outcomes=set()
    while pending:
      if r['local_state_count']>=256:
        r['local_state_budget_exhausted']=True;return
      state=pending.pop()
      if state in visited:continue
      visited.add(state);r['local_state_count']+=1
      pc,seq=state
      if pc in calls:seq=seq+(calls[pc],)
      if len(seq)>3 or len(set(seq))!=len(seq):return
      if pc in exit_sources:
        r['local_exit_state_count']+=1
        if len(seq)!=3 or set(seq)!={'read','resolve','wrap'}:return
        outcomes.add(seq)
      for nxt in graph.get(pc,[]):pending.append((nxt,seq))
    if not outcomes:return
    r['local_nonvacuous_exits']=True;r['local_one_each_every_exit']=True
    r['local_destination_before_read']=all(s.index('resolve')<s.index('read') for s in outcomes)
    r['local_read_before_destination']=all(s.index('read')<s.index('resolve') for s in outcomes)
    r['local_read_before_wrap']=all(s.index('read')<s.index('wrap') for s in outcomes)
    r['local_wrap_before_read']=all(s.index('wrap')<s.index('read') for s in outcomes)
    r['local_order_resolved']=(r['local_destination_before_read']!=r['local_read_before_destination']) and (r['local_read_before_wrap']!=r['local_wrap_before_read'])
    r['local_reference_consistent']=r['local_order_resolved'] and r['local_destination_before_read'] and r['local_read_before_wrap']

def strict_owner(l,image,maps,pdb,build,symbol,read_payload=True):
    expected=l._object_path(build/'source.ltcg.obj')
    modules=[x for x in pdb['modules'] if l._object_basename(x['object'])=='source.ltcg.obj']
    assert len(modules)==1 and l._object_path(modules[0]['object'])==expected
    rows=[x for x in maps if x['symbol']==symbol];assert len(rows)==1
    row=rows[0];start=(row['section_number'],row['section_offset'])
    assert sum((x['section_number'],x['section_offset'])==start for x in maps)==1
    cs=[x for x in pdb['contributions'] if (x['section_number'],x['section_offset'])==start];assert len(cs)==1
    c=cs[0];size=c['size'];section=image.section(start[0]);required=l.IMAGE_SCN_CNT_CODE|l.IMAGE_SCN_MEM_EXECUTE
    assert type(size) is int and size>0 and l._object_path(c['object'])==expected
    assert c['module_index']==modules[0]['index'] and c['module']==modules[0]['module']
    assert l._object_basename(row['object'])=='source.ltcg.obj'
    if any(x in str(row['object']) for x in ('/','\\',':')):assert l._object_path(row['object'])==expected
    assert 'f' in row['flags'] and c['characteristics']&required==required and section.characteristics&required==required
    assert 0<=start[1] and start[1]+size<=section.virtual_size
    assert start[1]+size<=section.raw_size and 0<=section.raw_offset and section.raw_offset+start[1]+size<=len(image.data)
    assert not any(x['section_number']==start[0] and start[1]<x['section_offset']<start[1]+size and 'f' in x['flags'] for x in maps)
    assert row['address']==image.image_base+section.rva+start[1]
    assert not any(x is not c and x['section_number']==start[0] and x['size']>0 and x['section_offset']<start[1]+size and start[1]<x['section_offset']+x['size'] for x in pdb['contributions'])
    if not read_payload:return row['address'],size
    data=image.read_section(start[0],start[1],size);assert len(data)==size
    return row['address'],data

def alias_binding(alias_bytes,source_bytes):
    text=alias_bytes.decode('ascii');lines=text.splitlines(keepends=True)
    bindings={}
    for index,line in enumerate(lines):
      m=re.fullmatch(r'/alternatename:([^\s=]+)=(_th10_ltcg_probe_anchor_[0-9]{4})\n',line)
      assert m and m[1] not in bindings and m[2]=='_th10_ltcg_probe_anchor_%04d'%index
      bindings[m[1]]=m[2]
    assert len(bindings)==74 and list(bindings)==sorted(bindings)
    expected='/* Diagnostic-only, non-runnable unresolved-symbol address anchors. */\n'+''.join('unsigned char %s[4] = {0};\n'%a[1:] for a in bindings.values())
    assert source_bytes==expected.encode('ascii')
    assert '?EnemyWrapAngle@@YGMM@Z' in bindings
    return bindings['?EnemyWrapAngle@@YGMM@Z']

def anchor_address(l,image,maps,pdb,build,alias,report=None):
    # Requires the original symbol and numbered anchor as a consistent MAP pair.
    requested='?EnemyWrapAngle@@YGMM@Z'
    if report is not None:
      report['baseline_wrap_symbol_count_capped']=min(3,sum(x['symbol']==requested for x in maps))
      report['baseline_anchor_symbol_count_capped']=min(3,sum(x['symbol']==alias for x in maps))
    rows=[x for x in maps if x['symbol'] in (requested,alias)]
    assert len(rows)==2 and {x['symbol'] for x in rows}=={requested,alias}
    a,b=rows
    assert all(a[k]==b[k] for k in ('address','section_number','section_offset','flags','object'))
    assert 'f' not in a['flags']
    start=(a['section_number'],a['section_offset'])
    assert {x['symbol'] for x in maps if x['address']==a['address'] or (x['section_number'],x['section_offset'])==start}=={requested,alias}
    assert len([x for x in maps if x['address']==a['address']])==2
    expected=l._object_path(build/'link-anchors.obj')
    assert l._object_basename(a['object'])=='link-anchors.obj'
    if any(x in str(a['object']) for x in ('/','\\',':')):assert l._object_path(a['object'])==expected
    modules=[x for x in pdb['modules'] if l._object_basename(x['object'])=='link-anchors.obj']
    assert len(modules)==1 and l._object_path(modules[0]['object'])==expected
    cs=[x for x in pdb['contributions'] if x['section_number']==start[0] and x['section_offset']<=start[1] and start[1]+4<=x['section_offset']+x['size']]
    if report is not None:report['baseline_anchor_contribution_count_capped']=min(3,len(cs))
    assert len(cs)==1;c=cs[0];section=image.section(start[0])
    assert not any(x is not c and x['section_number']==start[0] and x['size']>0 and x['section_offset']<start[1]+4 and start[1]<x['section_offset']+x['size'] for x in pdb['contributions'])
    assert l._object_path(c['object'])==expected and c['module_index']==modules[0]['index'] and c['module']==modules[0]['module']
    assert not c['characteristics']&0x20000020 and not section.characteristics&0x20000020
    assert c['characteristics']&0x40000000 and section.characteristics&0x40000000
    assert 0<=start[1] and start[1]+4<=section.virtual_size
    assert a['address']==image.image_base+section.rva+start[1]
    # Zero-fill data need not have file backing. No attempt to execute/read a helper body.
    assert not any(x['section_number']==start[0] and start[1]<x['section_offset']<start[1]+4 for x in maps)
    return a['address']

def baseline_tables(owner,origin,target_owner):
    assert len(owner)==7020
    selector_offset=7020-88;jump_offset=selector_offset-240
    selectors=owner[selector_offset:]
    assert selectors==target_owner[-88:]
    table=struct.unpack_from('<60I',owner,jump_offset)
    assert len(set(table))==60 and all(origin<=x<origin+jump_offset-2 for x in table)
    assert all(x<60 for x in selectors) and sum(x!=59 for x in selectors)==59
    entry=table[selectors[82]];assert selectors[82]!=59
    end=min([x for x in table if x>entry]+[origin+jump_offset-2])
    return table,entry,end,origin+selector_offset,origin+jump_offset

def inspect(root,target,r):
    if str(root)!=EXPECTED_ROOT or str(target)!=EXPECTED_TARGET or not no_symlinks(root) or not no_symlinks(target):raise RuntimeError()
    if not no_symlinks(root/'.git') or not all(no_symlinks(root/p) for p in PINS):raise RuntimeError()
    r['scope_valid']=True;assert repo_clean(root);r['repository_pre_valid']=True
    payload={p:(root/p).read_bytes() for p in PINS}
    assert all(sha(payload[p])==h for p,h in PINS.items());r['source_pins_valid']=True
    r['config_sha256']=PINS['config/match-units.toml'];r['reporter_sha256']=PINS['scripts/report-ecl-vm-table.py'];r['decoder_lock_sha256']=PINS['config/tools.lock.toml']
    literals={}
    for n in ast.parse(payload['scripts/report-ecl-vm-table.py']).body:
      if isinstance(n,ast.Assign) and len(n.targets)==1 and isinstance(n.targets[0],ast.Name) and n.targets[0].id in CONSTANTS:literals[n.targets[0].id]=ast.literal_eval(n.value)
    assert literals==CONSTANTS;r['constants_valid']=True
    cfg=tomllib.loads(payload['config/match-units.toml'].decode());assert cfg['target_sha256']==TARGET_HASH
    for key,(name,symbol,address,size) in HELPERS.items():
      u=cfg['units'][key];assert (u['functions'],u['symbol'],u['target_address'],u['size'])==([name],symbol,address,size)
    r['helper_identity_valid']=True
    ident=load_module('_op82_target_identity',root/'scripts/target_identity.py',payload['scripts/target_identity.py'])
    li=load_module('_op82_linked_image',root/'scripts/linked_image.py',payload['scripts/linked_image.py'])
    lock=tomllib.loads(payload['config/tools.lock.toml'].decode());li.verify_capstone(lock['capstone']);r['decoder_pin_valid']=True
    observed,problems=ident.verify_target(target);assert not problems and observed['sha256']==TARGET_HASH
    data=target.read_bytes();assert sha(data)==TARGET_HASH;r['target_identity_valid']=True;r['target_sha256']=TARGET_HASH
    target_owner=owner_slice(data);r['target_owner_sha256']=sha(target_owner)
    table_metadata(target_owner,payload['src/EclVm.cpp'].decode());r['table_shape_valid']=True;r['source_case_coverage_valid']=True
    build=root/BASELINE_REL;assert no_symlinks(build)
    run=root/RUN_REL;run_files={n:run/n for n in RUN_PINS}
    assert all(no_symlinks(p) and sha(p.read_bytes())==RUN_PINS[n] for n,p in run_files.items())
    manifest=json.loads(run_files['manifest.json'].read_text())
    assert manifest['base_commit']==HEAD and len(manifest['source_files'])==83 and not manifest['source_changes'] and not manifest['overlays']
    candidate_pins=dict(manifest['source_files']);candidate_pins.update(manifest['tool_pins']);candidate_pins['config/match-units.toml']=manifest['canonical_config_sha256']
    candidate_files={n:run/'candidate'/n for n in candidate_pins}
    assert all(not Path(n).is_absolute() and '..' not in Path(n).parts and no_symlinks(p) and sha(p.read_bytes())==candidate_pins[n] for n,p in candidate_files.items())
    files={n:build/n for n in BASELINE_PINS}
    assert all(no_symlinks(p) and sha(p.read_bytes())==BASELINE_PINS[n] for n,p in files.items())
    r['baseline_artifact_pins_valid']=True
    image=li.PEImage(files['source.exe']);assert image.sha256==BASELINE_PINS['source.exe'] and image.image_base==0x400000
    maps=li.map_publics(files['source.map'])+li.map_statics(files['source.map']);pdb=li.pdb_section_contributions(files['source.pdb'])
    metadata=li.map_metadata(files['source.map']);cv=image.codeview_record()
    assert metadata['timestamp']==image.timestamp and metadata['image_base']==image.image_base and cv['guid']==pdb['guid'] and cv['age']==pdb['pdb_age']
    r['baseline_pdb_identity_valid']=True
    origin,owner=strict_owner(li,image,maps,pdb,build,'?Run@EclVmContext@@QAEHM@Z')
    assert len(owner)==7020 and sha(owner)==BASELINE_OWNER_HASH
    r['whole_owner_valid']=True;r['owner_mapping_valid']=True;r['owner_size']=7020;r['owner_sha256']=sha(owner);r['baseline_owner_attribution_valid']=True
    read,_=strict_owner(li,image,maps,pdb,build,'?ReadFloat@EclVmContext@@QAEMI@Z',False)
    resolve,_=strict_owner(li,image,maps,pdb,build,'?ResolveFloat@EclVmContext@@QAEPAMI@Z',False)
    assert read!=resolve;r['baseline_helper_owners_valid']=True
    alias=alias_binding(files['link-aliases.rsp'].read_bytes(),files['link-anchors.c'].read_bytes())
    wrap=anchor_address(li,image,maps,pdb,build,alias,r);assert wrap not in (read,resolve)
    r['baseline_alias_provenance_valid']=True;r['wrap_is_diagnostic_anchor']=True
    table,entry,end,selector,jump=baseline_tables(owner,origin,target_owner)
    r['baseline_table_shape_valid']=True;r['active_opcode_count']=59;r['default_opcode_count']=29
    ANALYSIS.update(origin=origin,selector=selector,jump=jump,calls={read:'read',resolve:'resolve',wrap:'wrap'})
    import capstone as cs
    decoder=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32);decoder.detail=True
    scan(decoder,owner,origin,min(table),'prefix',r)
    seen,graph,exits=scan(decoder,owner,entry,end,'case',r)
    assert all(no_symlinks(root/p) and sha((root/p).read_bytes())==h for p,h in PINS.items());li.verify_capstone(lock['capstone']);r['post_pins_valid']=True
    assert no_symlinks(target) and sha(target.read_bytes())==TARGET_HASH;r['post_target_valid']=True
    assert all(no_symlinks(p) and sha(p.read_bytes())==BASELINE_PINS[n] for n,p in files.items());r['baseline_artifact_postpins_valid']=True
    assert all(no_symlinks(p) and sha(p.read_bytes())==RUN_PINS[n] for n,p in run_files.items())
    assert all(no_symlinks(p) and sha(p.read_bytes())==candidate_pins[n] for n,p in candidate_files.items())
    assert repo_clean(root);r['repository_post_valid']=True
    for key,name in [('baseline_image_sha256','source.exe'),('baseline_map_sha256','source.map'),('baseline_pdb_sha256','source.pdb'),('baseline_alias_sha256','link-aliases.rsp'),('baseline_anchor_source_sha256','link-anchors.c'),('baseline_anchor_object_sha256','link-anchors.obj')]:r[key]=BASELINE_PINS[name]
    prove_local(entry,seen,graph,exits,r)

def main():
    # Parse failures also emit the fixed schema; never argparse's path-bearing errors.
    r=blank()
    try:
      r['verifier_sha256']=sha(Path(__file__).read_bytes())
      if not __debug__:raise RuntimeError()
      assert len(sys.argv)==3
      root=Path(sys.argv[1]);target=Path(sys.argv[2])
      limit_resources();r['resource_bounds_valid']=True
      with contextlib.redirect_stdout(io.StringIO()),contextlib.redirect_stderr(io.StringIO()):
        inspect(root,target,r)
    except BaseException:
      r['failure_count']=1
      for key in ('local_order_resolved','local_destination_before_read','local_read_before_destination','local_read_before_wrap','local_wrap_before_read','local_reference_consistent'):
        r[key]=False
    signal.alarm(0)
    encoded=json.dumps(r,sort_keys=True,separators=(',',':'))
    if len(encoded.encode())+1>OUTPUT_LIMIT:
      r=blank();r['failure_count']=1;encoded=json.dumps(r,sort_keys=True,separators=(',',':'))
    print(encoded)
    return 0 if not r['failure_count'] else 1

if __name__=='__main__':sys.exit(main())
