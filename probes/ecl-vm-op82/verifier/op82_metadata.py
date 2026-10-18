#!/usr/bin/env python3
"""Reviewed-input-only metadata probe. No ordering or exactness authority."""
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
    origin=CONSTANTS['FUNCTION_ADDRESS']
    limits=BUDGETS[prefix]
    pending=[start]; seen={}; spans=[]; graph={}; outbound=[]
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
        if op.type==X86_OP_MEM and op.mem.disp==CONSTANTS['SELECTOR_TABLE_ADDRESS'] and op.size==1 and op.mem.scale==1 and (op.mem.index or op.mem.base):
          inc('selector_read_shape_count')
      if ins.mnemonic in ('ud0','ud1','ud2','hlt','int','int1','int3','into','iret','iretd','iretq','syscall','sysenter','sysexit','sysret','sysretq','rsm','xbegin','xabort','xend') or any(ins.group(g) for g in (cs.CS_GRP_INT,cs.CS_GRP_IRET,cs.CS_GRP_PRIVILEGE)):
        out[prefix+'_decode_invalid']=True;failed=True;break
      if ins.group(cs.CS_GRP_CALL):
        target=ins.operands[0].imm if len(ins.operands)==1 and ins.operands[0].type==X86_OP_IMM else None
        names={0x44FE40:'read',0x450070:'resolve',0x44BC70:'wrap'}
        if target not in names:
          inc('unknown_call_count');failed=True;break
        inc(names[target]+'_call_count');edge(pc,pc+ins.size,'fallthrough');continue
      if ins.group(cs.CS_GRP_RET):inc('return_exit_count');continue
      if ins.group(cs.CS_GRP_JUMP):
        op=ins.operands[0] if len(ins.operands)==1 else None
        if op is None or op.type!=X86_OP_IMM:
          inc('indirect_exit_count')
          if op and op.type==X86_OP_MEM and op.mem.disp==CONSTANTS['JUMP_TABLE_ADDRESS'] and op.size==4 and op.mem.scale==4 and op.mem.index and not op.mem.base:
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
    exits=len(outbound)+out[prefix+'_return_exit_count']+out[prefix+'_indirect_exit_count']
    out[prefix+'_completed']=not failed and bool(seen) and exits>0

def inspect(root,target,r):
    if EXPECTED_ROOT is None or EXPECTED_TARGET is None:raise RuntimeError()
    if str(root)!=EXPECTED_ROOT or str(target)!=EXPECTED_TARGET or not no_symlinks(root) or not no_symlinks(target):raise RuntimeError()
    if not no_symlinks(root/'.git'):raise RuntimeError()
    if not all(no_symlinks(root/p) for p in PINS):raise RuntimeError()
    r['scope_valid']=True
    if not repo_clean(root):raise RuntimeError()
    r['repository_pre_valid']=True
    payload={p:(root/p).read_bytes() for p in PINS}
    assert all(sha(payload[p])==h for p,h in PINS.items())
    r['source_pins_valid']=True
    r['config_sha256']=PINS['config/match-units.toml']
    r['reporter_sha256']=PINS['scripts/report-ecl-vm-table.py']
    r['decoder_lock_sha256']=PINS['config/tools.lock.toml']
    literals={}
    for n in ast.parse(payload['scripts/report-ecl-vm-table.py']).body:
      if isinstance(n,ast.Assign) and len(n.targets)==1 and isinstance(n.targets[0],ast.Name) and n.targets[0].id in CONSTANTS:
        literals[n.targets[0].id]=ast.literal_eval(n.value)
    assert literals==CONSTANTS;r['constants_valid']=True
    cfg=tomllib.loads(payload['config/match-units.toml'].decode())
    assert cfg['target_sha256']==TARGET_HASH
    for key,(name,symbol,address,size) in HELPERS.items():
      u=cfg['units'][key]
      assert (u['functions'],u['symbol'],u['target_address'],u['size'])==([name],symbol,address,size)
    rows=list(csv.DictReader(io.StringIO(payload['config/functions.csv'].decode())))
    rows=[x for x in rows if x['proposed_name']=='EclVmContext::Run']
    assert len(rows)==1 and int(rows[0]['address'],16)==CONSTANTS['FUNCTION_ADDRESS'] and int(rows[0]['size'])==7020 and int(rows[0]['span_end'],16)==CONSTANTS['OWNER_END_ADDRESS']
    r['helper_identity_valid']=True
    ident=load_module('_op82_target_identity',root/'scripts/target_identity.py',payload['scripts/target_identity.py'])
    linked=load_module('_op82_linked_image',root/'scripts/linked_image.py',payload['scripts/linked_image.py'])
    lock=tomllib.loads(payload['config/tools.lock.toml'].decode())
    linked.verify_capstone(lock['capstone']);r['decoder_pin_valid']=True
    observed,problems=ident.verify_target(target)
    assert not problems and observed['sha256']==TARGET_HASH
    data=target.read_bytes();assert sha(data)==TARGET_HASH
    r['target_identity_valid']=True;r['target_sha256']=TARGET_HASH
    owner=owner_slice(data);r['owner_mapping_valid']=True
    assert len(owner)==7020
    r['whole_owner_valid']=True;r['owner_size']=7020;r['owner_sha256']=sha(owner)
    table,entry,end=table_metadata(owner,payload['src/EclVm.cpp'].decode())
    r['table_shape_valid']=r['source_case_coverage_valid']=True
    r['active_opcode_count']=59;r['default_opcode_count']=29
    import capstone as cs
    decoder=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32);decoder.detail=True
    scan(decoder,owner,CONSTANTS['FUNCTION_ADDRESS'],min(table),'prefix',r)
    scan(decoder,owner,entry,end,'case',r)
    if not all(no_symlinks(root/p) and sha((root/p).read_bytes())==h for p,h in PINS.items()):raise RuntimeError()
    linked.verify_capstone(lock['capstone'])
    r['post_pins_valid']=True
    if not no_symlinks(target) or sha(target.read_bytes())!=TARGET_HASH:raise RuntimeError()
    r['post_target_valid']=True
    if not repo_clean(root):raise RuntimeError()
    r['repository_post_valid']=True

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
    signal.alarm(0)
    encoded=json.dumps(r,sort_keys=True,separators=(',',':'))
    if len(encoded.encode())+1>OUTPUT_LIMIT:
      r=blank();r['failure_count']=1;encoded=json.dumps(r,sort_keys=True,separators=(',',':'))
    print(encoded)
    return 0 if not r['failure_count'] else 1

if __name__=='__main__':sys.exit(main())
