import sys,json,tomllib,importlib.util,argparse,hashlib
from pathlib import Path
sys.path.insert(0,'scripts');from linked_image import *
from capstone import *
from capstone.x86_const import *
ap=argparse.ArgumentParser();ap.add_argument('directory');a=ap.parse_args();p=Path(a.directory);cand='.analysis/spt-test-update-20261025/member-candidate/'
spec=importlib.util.spec_from_file_location('compare','scripts/compare-linked-function.py');c=importlib.util.module_from_spec(spec);spec.loader.exec_module(c)
m=tomllib.loads(Path('config/match-units.toml').read_text());im=PEImage(p/'source.exe');tar=c.verified_target();pubs=map_publics(p/'source.map');fs=linked_functions(p/'source.exe',p/'source.map',p/'source.pdb')['functions'];raw=json.loads(Path('.analysis/spt-test-update-20261025/target-inventory.json').read_text())
base=dict(m['units']['spt-test-draw'],source=cand+'SptTest.cpp',pdb_source=cand+'SptTest.cpp',support_sources=[cand+n+'.cpp' for n in ['AnmManager','Chain','ScriptViewerWorker','ScriptViewer','FrontEnd','Main','EclVm','Enemy']],output_dir=str(p),entry_symbol='?SptTestUpdateCallback@@YIHPAX@Z',profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/I'+cand.rstrip('/'),'/Isrc','/GL','/GS','/EHs'])
units={}
for key,source in [('spt-test-draw','SptTest'),('spt-test-draw-callback','SptTest'),('spt-test-constructor','SptTest'),('spt-test-register-tasks','SptTest'),('spt-test-clear-script-names','SptTest'),('ascii-manager-add-small-format-text-gs','AnmManager'),('script-viewer-worker-start','ScriptViewerWorker'),('frontend-cursor-move','FrontEnd'),('frontend-input-repeated','FrontEnd'),('main-set-next-game-mode','Main'),('ecl-vm-host-run','EclVm'),('ecl-vm-find-subroutine','EclVm')]:
 old=m['units'][key];units[key]=dict(base,**{k:old[k] for k in ['symbol','functions','target_address','size','linkages']},pdb_source=cand+source+'.cpp')
roles={0x42bad0:'?ClearScriptNames@SptTestView@@QAEXXZ',0x42be10:'?SptTestLoadCallback@@YAHXZ',0x42bfc0:'?Update@SptTestView@@QAEHXZ',0x44fd10:'?Run@EclVmHost@@QAEHM@Z',0x450470:'?FindSubroutine@EclVmScriptDatabase@@QAEPAUEclVmInstruction@@PBD@Z',0x450500:'?CreateHost@EclVmScriptDatabase@@QAEPAUEnemyEclHostBaseView@@PBD@Z'}
for u in m['units'].values():
 for f in u.get('linkages',[]):
  if 'symbol' in f and f.get('target') not in roles and not f.get('addend'):roles[f['target']]=f['symbol']
syms={'update':'?Update@SptTestView@@QAEHXZ','callback':'?SptTestUpdateCallback@@YIHPAX@Z','factory':'?CreateHost@EclVmScriptDatabase@@QAEPAUEnemyEclHostBaseView@@PBD@Z'}
for key in syms:
 row=raw[key];links=[]
 for f in row['fields']:
  t=int(f['target'],16);v=dict(offset=f['offset'],type=f['type'],target=t,symbol=roles.get(t,''))
  if 0x474e30<=t<=0x474e38:v.update(symbol='?g_FrontEndInput@@3UFrontEndInputView@@A',target=0x474e30,addend=t-0x474e30)
  assert v['symbol'];links.append(v)
 units[key]=dict(base,symbol=syms[key],functions=[key],target_address=int(row['address'],16),size=row['size'],linkages=links,pdb_source=cand+('Enemy.cpp' if key=='factory' else 'SptTest.cpp'))
selected={'target_sha256':m['target_sha256'],'units':units};(p/'comparison-manifest.json').write_text(json.dumps(selected,indent=2)+'\n')
d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True
inventory={};results={}
def inventory_one(image,address,size,publics):
 b=image.read_address(address,size);ii=list(d.disasm(b,address));assert sum(i.size for i in ii)==size;starts={i.address for i in ii};local=[]
 for i in ii:
  if (i.group(CS_GRP_JUMP) or i.group(CS_GRP_CALL)) and i.operands[0].type==X86_OP_IMM:
   dest=i.operands[0].imm
   if address<=dest<address+size:assert dest in starts;local.append(dict(offset=i.address-address,width=i.size,destination=dest-address))
 return dict(size=size,sha256=hashlib.sha256(b).hexdigest(),fields=linked_code_fields(image,publics,address,size)['fields'],locals=local,instructions=[dict(offset=i.address-address,bytes=i.bytes.hex(),mnemonic=i.mnemonic,operands=i.op_str) for i in ii])
for key,u in units.items():
 f=[f for f in fs if f['symbol']==u['symbol']];assert len(f)==1
 inventory[key]={'target':inventory_one(tar,u['target_address'],u['size'],[]),'candidate':inventory_one(im,int(f[0]['address'],16),f[0]['size'],pubs)}
 inventory[key]['target']['semantic_fields']=u['linkages']
 try:r=c.compare_unit(key,_unit_manifest=selected)
 except Exception as e:r={'result':'rejected','error':str(e)}
 if r.get('result')=='exact':assert inventory[key]['target']['locals']==inventory[key]['candidate']['locals']
 results[key]=r;print(key,f[0]['size'],'/',u['size'],r.get('result'),r.get('error',''))
(p/'inventory.json').write_text(json.dumps(inventory,indent=2)+'\n');(p/'comparison.json').write_text(json.dumps(results,indent=2)+'\n')
