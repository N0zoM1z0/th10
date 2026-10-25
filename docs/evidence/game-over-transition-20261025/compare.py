from pathlib import Path
import sys,json,hashlib,tomllib,importlib.util,copy
sys.path.insert(0,'scripts')
from linked_image import *
from capstone import *
from capstone.x86_const import X86_OP_IMM
import argparse
ap=argparse.ArgumentParser();ap.add_argument('--first-dir',default='build/gpt-dots/game-over-first');ap.add_argument('--ending-dir',default='build/gpt-dots/game-over-ending');ap.add_argument('--output-dir',required=True);args=ap.parse_args()
assert hashlib.sha256(Path('resources/th10.exe').read_bytes()).hexdigest()=='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
base=Path(args.output_dir).resolve();assert base.is_relative_to(Path('.analysis').resolve()) and not base.exists();base.mkdir(parents=True)
paths={'first':Path(args.first_dir),'ending':Path(args.ending_dir)}
canonical=tomllib.loads(Path('config/match-units.toml').read_text());d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True
spec=importlib.util.spec_from_file_location('cmp','scripts/compare-linked-function.py');cmp=importlib.util.module_from_spec(spec);spec.loader.exec_module(cmp)
def inv(image,address,size,publics):
 raw=image.read_address(address,size);ins=list(d.disasm(raw,address));assert sum(i.size for i in ins)==size
 starts={i.address for i in ins};local=[]
 for i in ins:
  if i.group(CS_GRP_JUMP) and i.operands[0].type==X86_OP_IMM and address<=i.operands[0].imm<address+size:
   assert i.operands[0].imm in starts;local.append(dict(offset=i.address-address,width=i.size,destination=i.operands[0].imm-address))
 return dict(address=hex(address),size=size,sha256=hashlib.sha256(raw).hexdigest(),fields=linked_code_fields(image,publics,address,size)['fields'],locals=local,instructions=[dict(offset=i.address-address,bytes=i.bytes.hex(),mnemonic=i.mnemonic,operands=i.op_str) for i in ins])
allinv={};norm={}
for trial in ['first','ending']:
 out=paths[trial];meta=json.loads((out/'link.json').read_text());src=Path(meta['support_sources'][0]).parent;assert all(Path(s).parent==src for s in meta['support_sources']);im=PEImage(out/'source.exe');pubs=map_publics(out/'source.map');fs=linked_functions(out/'source.exe',out/'source.map',out/'source.pdb')['functions'];f=next(f for f in fs if 'GameOverTransition@@' in f['symbol']);a=int(f['address'],16);inventory=inv(im,a,f['size'],pubs);allinv[trial]=inventory
 (base/(trial+'.asm')).write_text('\n'.join('%03X %s %s'%(i['offset'],i['mnemonic'],i['operands']) for i in inventory['instructions'])+'\n')
 raw=bytearray(im.read_address(a,f['size']))
 for field in inventory['fields']:raw[field['offset']:field['offset']+field['width']]=bytes(field['width'])
 norm[trial]=raw.hex()
 guards={};m=copy.deepcopy(canonical)
 for name in ['anm-manager-add-vm-secondary-tail','anm-manager-find-vm','anm-manager-add-vm-primary-tail','anm-manager-add-vm-primary-head','anm-manager-add-vm-secondary-head']:
  u=m['units'][name];u['output_dir']=str(out);u['pdb_source']=str(src/Path(u['source']).name);u['source']=str(src/'Player.cpp');u['support_sources']=[str(src/n) for n in ['FrontEnd.cpp','AnmManager.cpp','AnmVmCreate.cpp','AnmVmId.cpp','Sound.cpp']+(['Ending.cpp'] if trial=='ending' else [])];u['profile']=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/I'+str((src).resolve()),'/GL'];u['entry_symbol']='?PlayerUpdateCallback@@YIHPAUPlayer@@@Z'
  try: guards[name]=cmp.compare_unit(name,_unit_manifest=m)
  except Exception as e: guards[name]=dict(result='rejected',error=str(e))
 (base/(trial+'-guards.json')).write_text(json.dumps(guards,indent=2));print(trial,f['size'],{n:r.get('result') for n,r in guards.items()})
 print([(x['symbol'],x['size']) for x in fs if 'FrontEndLoadMusic' in x['symbol']])
allinv['target']=inv(PEImage('resources/th10.exe'),0x4231d0,411,[])
(base/'inventory.json').write_text(json.dumps(allinv,indent=2));neutral=dict(masked_whole_body_equal=norm['first']==norm['ending'],diagnostic_only=True);(base/'neutrality.json').write_text(json.dumps(neutral,indent=2));print(neutral)

v=allinv
# Full target instruction operand role/base inventory, independently read from target.
rows=[(2,0x477810),(13,0x491ff4),(33,0x491fb8),(75,0x476f78),(84,0x4776e0),(112,0x491c10),(126,0x449950),(163,0x449870),(169,0x491c10),(180,0x448ac0),(210,0x424480),(216,0x47770c),(233,0x449950),(273,0x449870),(279,0x491c10),(288,0x448ac0),(297,0x46e0d8),(310,0x420a90),(316,0x491d78),(331,0x46df98),(336,0x492590),(341,0x43e460),(350,0x46df98),(355,0x492590),(360,0x43e460),(366,0x47783c),(389,0x476f78),(401,0x476f78)]
# Each source identity has a target-backed role; no nearest-anchor alias.
identity={
'?g_ResultGameManager@@3PAUResultGameManagerView@@A':0x477810,
'?g_GameConfigurationFlags@@3IA':0x491ff4,'?g_GameTransitionState@@3HA':0x491fb8,
'?g_AnmGameSpeed@@3MA':0x476f78,'?g_AsciiManagerView@@3PAUAsciiManagerView@@A':0x4776e0,
'?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A':0x491c10,
'?AllocateVm@AnmRenderManagerView@@QAEPAUAnmVmView@@XZ':0x449950,
'?InitializeVm@AnmLoadedView@@QAEXPAUAnmVmView@@H@Z':0x449870,
'?AddVmVariant1@AnmRenderManagerView@@QAE?AUAnmVmIdView@@PAUAnmVmView@@@Z':0x448ac0,
'?RequestCaptureRectangle@AnmRenderManagerView@@QAEHHHHHH@Z':0x424480,
'?g_GuiView@@3PAUGuiView@@A':0x47770c,
'??_C@_0BA@JMOMNNJJ@bgm?1th10_17?4wav?$AA@':0x46e0d8,
'?FrontEndLoadMusic@@YGHHPBD@Z':0x420a90,'?g_FrontEndSupervisorFlags@@3IA':0x491d78,
'??_C@_05JLNEMJLN@dummy?$AA@':0x46df98,'?g_MainSoundOwner@@3PAEA':0x492590,
'?QueueCommand@SoundPlayerView@@QAEXPBDHH@Z':0x43e460,
'?g_GameStatisticsRows@@3PAUGameStatisticsOwner@@A':0x47783c}
rel={0x449950,0x449870,0x448ac0,0x424480,0x420a90,0x43e460};im=PEImage('resources/th10.exe');links=[]
for offset,target in rows:
 syms=[s for s,a in identity.items() if a==target];assert len(syms)==1
 kind='REL32' if target in rel else 'DIR32';raw=im.read_address(0x4231d0+offset,4);value=int.from_bytes(raw,'little') if kind=='DIR32' else (0x4231d0+offset+4+int.from_bytes(raw,'little',signed=True))&0xffffffff;assert value==target,(offset,hex(value),hex(target))
 links.append(dict(offset=offset,type=kind,symbol=syms[0],target=target,addend=0))
v['target']['explicit_field_bindings']=links
for trial in ['first','ending']:
 pubs=map_publics(paths[trial]/'source.map');bindings=[]
 for f in v[trial]['fields']:
  matches=[(s,a) for s,a in identity.items() if any(x['symbol']==s and x['address']==f['candidate_target'] for x in pubs)]
  assert len(matches)==1,(trial,f,matches)
  s,a=matches[0];bindings.append(dict(offset=f['offset'],type=f['type'],symbol=s,target=a,addend=0,candidate_base=f['candidate_target'],candidate_value=f['candidate_target']))
 assert len(bindings)==28;v[trial]['explicit_field_bindings']=bindings
assert len(v['target']['locals'])==3
(base/'inventory.json').write_text(json.dumps(v,indent=2)+'\n');(base/'target-fields.json').write_text(json.dumps(links,indent=2)+'\n');print('complete target28 and each candidate28; fixed local targets3 each',[(n,len(x['locals'])) for n,x in v.items()])
