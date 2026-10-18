#!/usr/bin/env python3
"""Pinned normal-COFF call-form metadata only. No bytes, offsets or disassembly export."""
import hashlib,importlib.util,json,os,pathlib,resource,signal,struct,subprocess,sys
sys.dont_write_bytecode=True
P=pathlib.Path
HEAD='e94559cc95d583b702660b72bbc605e5dfc5743a'
RUN='.analysis/bullet1454-native-baseline-20261018-a002'
OBJECT='support-01-BulletTransform.normal.obj'
OBJECT_SHA='6f8476b0641451c88f8f9738d0e3a5b1b1edd315a3116592be8f654ed31e5c73'
PARSER_PINS={'scripts/compare-coff-function.py':'0264700809093a503971ed599f7d3a9cde83a2f6128e63e3c988e651e28fa8cb','scripts/msvc_symbols.py':'753efb56b8e254d692aa7d4a3cee90528da4f0ebda998ef1dc176e1055f4053a'}
SYMBOLS={'transform':'?AdvanceTransformProgram@BulletRuntimeView@@QAEXXZ','helper':'?BulletResolveTransformAngle@@YAMPAUBulletRuntimeView@@M@Z'}
SIZES={'transform':1476,'helper':68}
ARTIFACT_PINS={'source.exe':'20cc22b102634f6b7f657aa6145a7ab0c4301e0289db6208364d9dd2a59d1120','source.map':'fb126866aa714c4557f0f703a7f8cd519527537675bcb94b02a4b96cf544dc0e','source.pdb':'c09b8ce0a21fade20c1f5f63a9f4c7a84e20d7e1927ffe380e8f5b839651490f'}
def need(v,message='metadata precondition'):
 if not v:raise ValueError(message)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def regular(p,limit=64*1024*1024):
 for x in (p,*p.parents):need(not x.is_symlink())
 need(p.is_file() and p.stat().st_size<=limit);return p

def analyze(data,sections,symbols,function_rows):
 selected={};out={}
 for kind,name in SYMBOLS.items():
  rows=[(i,s) for i,s in symbols.items() if s['name']==name and s['section']>0];need(len(rows)==1,'selected symbol count')
  index,sym=rows[0];need(sym['type']==0x20 and 0<sym['section']<=len(sections),'selected code symbol')
  sec=sections[sym['section']-1];need(sec['flags']&0x20001020==0x20001020,'executable code COMDAT')
  same=[s for s in symbols.values() if s['section']==sym['section'] and s['type']==0x20];need(len(same)==1 and sym['value']==0,'one whole COMDAT function')
  need(sym['storage']==(2 if kind=='transform' else 3),'function storage class')
  metadata=[x for x in function_rows if x['symbol']==name];need(len(metadata)==1,'function metadata count');row=metadata[0]
  need(row['section_number']==sym['section'] and row['section_offset']==0 and row['size']==sec['size']==SIZES[kind],'whole extent disagreement')
  need(row['extent_source'] in {'function-definition-aux','single-function-comdat-section'},'extent provenance')
  start=sec['raw_offset'];end=start+sec['size'];need(start>=20+40*len(sections) and end<=len(data),'raw extent bounds')
  need(sec['reloc_count']<=10000 and 0<=sec['reloc_offset'] and sec['reloc_offset']+10*sec['reloc_count']<=len(data),'relocation table bounds')
  selected[kind]=(index,sym,sec)
  out[kind]={'complete':True,'size':sec['size'],'sha256':hashlib.sha256(data[start:end]).hexdigest(),'unique_function_comdat':True,'expected_storage_class':True}
 need(selected['transform'][1]['section']!=selected['helper'][1]['section'],'distinct owner sections')
 first=selected['transform'][2];second=selected['helper'][2]
 need(first['raw_offset']+first['size']<=second['raw_offset'] or second['raw_offset']+second['size']<=first['raw_offset'],'overlapping raw owner extents')
 _,owner,section=selected['transform'];helper_index,helper,_=selected['helper'];relocs=[];occupied=set()
 for n in range(section['reloc_count']):
  local,target_index,type_id=struct.unpack_from('<IIH',data,section['reloc_offset']+n*10)
  need(type_id in {0x0006,0x0014},'unexpected relocation kind');need(0<=local and local+4<=section['size'],'relocation leaves owner')
  extent=set(range(local,local+4));need(not occupied&extent,'overlapping relocations');occupied|=extent
  need(target_index in symbols,'auxiliary/invalid relocation target');relocs.append((local,target_index,type_id))
 named=[];section_resolved=[];named_non_call=0;section_non_call=0
 for local,target_index,type_id in relocs:
  target=symbols[target_index];exact_name=target['name']==SYMBOLS['helper']
  addend=struct.unpack_from('<i',data,section['raw_offset']+local)[0]
  resolves_start=type_id==0x0014 and target['section']==helper['section'] and target['value']+addend==helper['value']
  e8_form=type_id==0x0014 and local>=1 and data[section['raw_offset']+local-1]==0xE8 and not any(other!=local and other<local+4 and local-1<other+4 for other,_,_ in relocs)
  if exact_name:
   need(target_index==helper_index,'duplicate helper identity')
   if resolves_start and e8_form:named.append(local)
   else:named_non_call+=1
  elif target['section']==helper['section']:
   if resolves_start and e8_form:section_resolved.append(local)
   else:section_non_call+=1
 return {'whole_coff_owners':out,'owner_relocation_count':len(relocs),'helper_named_reference_count':len(named)+named_non_call,'helper_named_rel32_e8_callform_count':len(named),'helper_named_other_reference_count':named_non_call,'helper_section_alias_rel32_e8_callform_count':len(section_resolved),'helper_section_other_reference_count':section_non_call,'helper_rel32_e8_form_present':bool(named or section_resolved),'instruction_boundary_proved':False,'control_flow_proved':False,'callform_definition':'REL32 relocation resolves exactly to helper entry; preceding E8 byte and complete nonoverlapping operand are within the single-function code COMDAT. This does not prove instruction boundaries or control flow; data or other immediates can contain the same form.','native_payload_exported':False,'exactness_credit':'none'}

def main():
 os.nice(10);resource.setrlimit(resource.RLIMIT_CPU,(20,20));resource.setrlimit(resource.RLIMIT_AS,(1024**3,1024**3))
 def timeout(*args):raise TimeoutError('collector wall bound')
 signal.signal(signal.SIGALRM,timeout);signal.alarm(45)
 repo=P.cwd();run=repo/RUN;build=run/'candidate/build/match-linked/BulletRuntimeUpdate'
 def clean():
  need(subprocess.check_output(['git','rev-parse','HEAD'],timeout=10).decode().strip()==HEAD)
  need(not subprocess.check_output(['git','status','--porcelain=v1','--untracked-files=all'],timeout=10))
 clean()
 need(sha(regular(run/'runner.py'))=='c7d2b976c4d6794b24ecf8cedbf8dd82dfa1d4eb387b5a0bd11c40313aea05bf')
 need(sha(regular(run/'manifest.json'))=='7b3371074d056d98438a7a9641dce1748c81c7c3aef29a804a27bc5a78d41a75')
 for name,digest in ARTIFACT_PINS.items():need(sha(regular(build/name))==digest)
 obj=regular(build/OBJECT);need(sha(obj)==OBJECT_SHA)
 for rel,digest in PARSER_PINS.items():need(sha(regular(repo/rel))==digest)
 sys.path.insert(0,str(repo/'scripts'));spec=importlib.util.spec_from_file_location('pinned_coff',repo/'scripts/compare-coff-function.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
 need(P(sys.modules['msvc_symbols'].__file__).resolve()==(repo/'scripts/msvc_symbols.py').resolve(),'symbol module origin')
 data,sections,symbols=module.coff_object(obj);rows=module.object_functions(obj);result=analyze(data,sections,symbols,rows)
 need(sha(obj)==OBJECT_SHA)
 for name,digest in ARTIFACT_PINS.items():need(sha(regular(build/name))==digest)
 clean();result.update(collector_sha256=sha(regular(P(__file__).absolute())),object_sha256=OBJECT_SHA,artifact_sha256=ARTIFACT_PINS,production_clean=True)
 print(json.dumps(result,sort_keys=True))
if __name__=='__main__':
 try:main()
 except BaseException as exc:print(json.dumps({'diagnostic':'failed','error_class':type(exc).__name__,'native_payload_exported':False}));sys.exit(1)
