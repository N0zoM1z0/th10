import sys,json,tomllib,importlib.util,argparse,hashlib
from pathlib import Path
sys.path.insert(0,'scripts');from linked_image import *
from capstone import *
from capstone.x86_const import *
ap=argparse.ArgumentParser();ap.add_argument('directory');ap.add_argument('--clean',action='store_true');a=ap.parse_args();p=Path(a.directory);cand='.analysis/spt-test-cleanup-20261025/'+('clean-candidate/' if a.clean else 'candidate/')
spec=importlib.util.spec_from_file_location('compare','scripts/compare-linked-function.py');c=importlib.util.module_from_spec(spec);spec.loader.exec_module(c)
m=tomllib.loads(Path('config/match-units.toml').read_text());im=PEImage(p/'source.exe');tar=c.verified_target();pubs=map_publics(p/'source.map');fs=linked_functions(p/'source.exe',p/'source.map',p/'source.pdb')['functions'];raw=json.loads(Path('.analysis/spt-test-cleanup-20261025/target-inventory.json').read_text())
base=dict(m['units']['spt-test-draw'],source=cand+'SptTest.cpp',pdb_source=cand+'SptTest.cpp',support_sources=[cand+n+'.cpp' for n in (['AnmManager','Chain','ScriptViewerWorker','ScriptViewer'] if a.clean else ['AnmManager','Chain','ScriptViewerWorker'])],output_dir=str(p),entry_symbol='?SptTestCreate@@YAPAUSptTestView@@XZ',profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/I'+cand.rstrip('/'),'/Isrc','/GL','/GS','/EHs'])
units={}
for key,source in [('spt-test-draw','SptTest'),('spt-test-draw-callback','SptTest'),('ascii-manager-add-small-format-text-gs','AnmManager'),('decomphelp-chain-unregister','Chain'),('script-viewer-worker-destructor','ScriptViewerWorker'),('script-viewer-worker-stop','ScriptViewerWorker'),('script-viewer-worker-start','ScriptViewerWorker'),('anm-loaded-release','AnmManager'),('anm-release-slot','AnmManager')]:
 old=m['units'][key];units[key]=dict(base,**{k:old[k] for k in ['symbol','functions','target_address','size','linkages']},pdb_source=cand+source+'.cpp')
roles={0x452422:'_free',0x4703e4:'??_7ScriptViewerWorkerView@@6B@',0x477844:'?g_SptTestView@@3PAUSptTestView@@A',0x42c580:'?SptTestUpdateCallback@@YIHPAX@Z',0x42c590:'?SptTestDrawCallback@@YIHPAX@Z',0x449ed0:'?AllocElem@Chain@th10@@SGPAUChainElem@2@P6IHPAX@Z@Z',0x491be4:'?g_Chain@th10@@3PAUChain@1@A',0x449ae0:'?AddToCalcChain@Chain@th10@@SGHPAUChainElem@2@HPAU12@@Z',0x449b70:'?AddToDrawChain@Chain@th10@@SGHPAUChainElem@2@HPAU12@@Z',0x42bc30:'??1SptTestView@@QAE@XZ',0x4524a1:'??3@YAXPAX@Z',0x452493:'??2@YAPAXI@Z',0x42bb30:'??0SptTestView@@QAE@XZ',0x42bbd0:'?RegisterTasks@SptTestView@@QAEHXZ',0x42bad0:'?ClearScriptNames@SptTestView@@QAEXXZ',0x449f60:'?UnregisterElem@Chain@th10@@SIXPAUChainElem@2@PAU12@@Z',0x4660b0:'__imp__EnterCriticalSection@4',0x4660b4:'__imp__LeaveCriticalSection@4',0x46d0f0:'??_7EnemyEclResourceBaseView@@6B@',0x491c10:'?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A',0x447810:'?Release@AnmLoadedView@@QAEXXZ',0x44c150:'?Stop@ScriptViewerWorkerView@@QAEXXZ'}
syms={'ctor':'??0SptTestView@@QAE@XZ','register':'?RegisterTasks@SptTestView@@QAEHXZ','names':'?ClearScriptNames@SptTestView@@QAEXXZ','deleting':'??_GSptTestView@@QAEPAXI@Z','factory':'?SptTestCreate@@YAPAUSptTestView@@XZ','destructor':'??1SptTestView@@QAE@XZ'}
for key in ['ctor','register','names','deleting','factory']:
 row=raw[key];links=[dict(offset=f['offset'],type=f['type'],target=int(f['target'],16),symbol=roles[int(f['target'],16)]) for f in row['fields']]
 units[key]=dict(base,symbol=syms[key],functions=[key],target_address=int(row['address'],16),size=row['size'],linkages=links)
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
 for field in u['linkages']:
  if field['type']=='DIR32':
   o=field['offset'];ii=[i for i in d.disasm(tar.read_address(u['target_address'],u['size']),u['target_address']) if i.address-u['target_address']<=o<i.address-u['target_address']+i.size];assert len(ii)==1;i=ii[0];assert o-i.address+u['target_address'] in(i.imm_offset,i.disp_offset)
   value=int.from_bytes(tar.read_address(u['target_address']+o,4),'little');assert value==field['target']+field.get('addend',0)
   inventory[key]['target']['fields'].append(dict(offset=o,width=4,type='DIR32',target=value,symbol=field['symbol']))
 inventory[key]['target']['fields'].sort(key=lambda x:x['offset'])
 try:r=c.compare_unit(key,_unit_manifest=selected)
 except Exception as e:r={'result':'rejected','error':str(e)}
 if r.get('result')=='exact':assert inventory[key]['target']['locals']==inventory[key]['candidate']['locals']
 results[key]=r;print(key,f[0]['size'],'/',u['size'],r.get('result'),r.get('error',''))
if not a.clean:
 f=next(f for f in fs if f['symbol']==syms['destructor']);inventory['destructor']={'target':raw['destructor'],'candidate':inventory_one(im,int(f['address'],16),f['size'],pubs)}
 results['destructor']={'result':'rejected','target_size':405,'candidate_size':f['size'],'reason':'complete extent differs; all33 fields (including three FS zero fields) and locals retained; generated EH binding separate unresolved'}
(p/'inventory.json').write_text(json.dumps(inventory,indent=2)+'\n');(p/'comparison.json').write_text(json.dumps(results,indent=2)+'\n')
assert all(results[k]['result']=='exact' for k in ['spt-test-draw','spt-test-draw-callback','ascii-manager-add-small-format-text-gs','decomphelp-chain-unregister','script-viewer-worker-destructor','script-viewer-worker-stop','script-viewer-worker-start'])
