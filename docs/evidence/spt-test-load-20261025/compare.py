import sys,json,tomllib,importlib.util,argparse,hashlib
from pathlib import Path
sys.path.insert(0,'scripts');from linked_image import *
from capstone import *
from capstone.x86_const import X86_OP_IMM
ap=argparse.ArgumentParser();ap.add_argument('directory');ap.add_argument('--extra-support',action='append',default=[]);args=ap.parse_args()
p=Path(args.directory);cand='.analysis/spt-test-load-20261025/candidate/'
spec=importlib.util.spec_from_file_location('compare','scripts/compare-linked-function.py');c=importlib.util.module_from_spec(spec);spec.loader.exec_module(c)
m=tomllib.loads(Path('config/match-units.toml').read_text());im=PEImage(p/'source.exe');tar=c.verified_target();pubs=map_publics(p/'source.map');fs=linked_functions(p/'source.exe',p/'source.map',p/'source.pdb')['functions']
base=dict(m['units']['spt-test-draw'],source=cand+'SptTest.cpp',pdb_source=cand+'SptTest.cpp',support_sources=[cand+n+'.cpp' for n in ['AnmManager','FileSystem',*args.extra_support]],output_dir=str(p),entry_symbol='?SptTestLoadCallback@@YAHXZ',profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/I'+cand.rstrip('/'),'/Isrc','/GL','/GS','/EHs'])
units={}
for key in ['spt-test-draw','ascii-manager-add-small-format-text-gs','enemy-ecl-resource-base-constructor']:
 old=m['units'][key];u=dict(base,**{k:old[k] for k in ['symbol','functions','target_address','size','linkages']});u['pdb_source']=cand+('AnmManager.cpp' if 'ascii' in key else 'SptTest.cpp');units[key]=u
loadlinks=[(7,'DIR32','___security_cookie',0x473660),(39,'DIR32','??_C@_02DKCKIIND@?$CFs?$AA@',0x46e1c8),(45,'REL32','_sprintf',0x452ae8),(95,'REL32','_free',0x452422),(115,'REL32','?OpenFile@FileSystem@@YGPAEPBDPAHH@Z',0x44b360),(131,'REL32','??2@YAPAXI@Z',0x452493),(147,'DIR32','??_7EnemyEclResourceBaseView@@6B@',0x46d0f0),(250,'REL32','@__security_check_cookie@4',0x458ea5)]
units['load']=dict(base,symbol='?LoadScript@SptTestView@@QAEHXZ',functions=['SptTestView::LoadScript'],size=261,target_address=0x42be20,linkages=[dict(offset=o,type=t,symbol=s,target=a) for o,t,s,a in loadlinks])
units['callback']=dict(base,symbol='?SptTestLoadCallback@@YAHXZ',functions=['SptTestLoadCallback'],size=16,target_address=0x42be10,linkages=[dict(offset=3,type='DIR32',symbol='?g_SptTestView@@3PAUSptTestView@@A',target=0x477844),dict(offset=8,type='REL32',symbol='?LoadScript@SptTestView@@QAEHXZ',target=0x42be20)])
if args.extra_support:
 old=m['units']['file-system-open-file']; units['file-system-open-file']=dict(base,**{k:old[k] for k in ['symbol','functions','target_address','size','linkages']},pdb_source=cand+'FileSystem.cpp')
selected={'target_sha256':m['target_sha256'],'units':units};(p/'comparison-manifest.json').write_text(json.dumps(selected,indent=2)+'\n')
d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True
inventory={};results={}
def inventory_one(image,address,size,publics):
 raw=image.read_address(address,size);ins=list(d.disasm(raw,address));assert sum(i.size for i in ins)==size
 starts={i.address for i in ins};local=[]
 for i in ins:
  if i.group(CS_GRP_JUMP) and i.operands[0].type==X86_OP_IMM:
   dest=i.operands[0].imm
   if address<=dest<address+size:
    assert dest in starts;local.append({'offset':i.address-address,'width':i.size,'destination':dest-address})
 return dict(size=size,sha256=hashlib.sha256(raw).hexdigest(),fields=linked_code_fields(image,publics,address,size)['fields'],locals=local,instructions=[dict(offset=i.address-address,bytes=i.bytes.hex(),mnemonic=i.mnemonic,operands=i.op_str) for i in ins])
for key,u in units.items():
 f=[f for f in fs if f['symbol']==u['symbol']];assert len(f)==1
 inventory[key]={'target':inventory_one(tar,u['target_address'],u['size'],[]),'candidate':inventory_one(im,int(f[0]['address'],16),f[0]['size'],pubs)}
 # Target PE lacks base relocations: audit manifest DIR32 against decoded
 # absolute immediate/displacement operands, not PE relocation absence.
 target_fields=inventory[key]['target']['fields']
 for field in u['linkages']:
  if field['type']=='DIR32':
   o=field['offset']; ins=[i for i in d.disasm(tar.read_address(u['target_address'],u['size']),u['target_address']) if i.address-u['target_address']<=o<i.address-u['target_address']+i.size];assert len(ins)==1
   i=ins[0];assert o-i.address+u['target_address'] in (i.imm_offset,i.disp_offset)
   value=int.from_bytes(tar.read_address(u['target_address']+o,4),'little');assert value==field['target']+field.get('addend',0)
   target_fields.append(dict(offset=o,width=4,type='DIR32',target=value,symbol=field['symbol'],source='decoded-absolute-operand-plus-reviewed-binding'))
 target_fields.sort(key=lambda x:x['offset'])
 try:r=c.compare_unit(key,_unit_manifest=selected)
 except Exception as e:r={'result':'rejected','error':str(e)}
 results[key]=r;print(key,f[0]['size'],'/',u['size'],r.get('result'),r.get('error',''))
(p/'inventory.json').write_text(json.dumps(inventory,indent=2)+'\n');(p/'comparison.json').write_text(json.dumps(results,indent=2)+'\n')
assert all(results[k]['result']=='exact' for k in ['spt-test-draw','ascii-manager-add-small-format-text-gs','enemy-ecl-resource-base-constructor'])
