import sys,json,tomllib,importlib.util,hashlib
from pathlib import Path
sys.path.insert(0,'scripts')
from linked_image import PEImage,linked_functions,linked_code_fields,map_publics
from capstone import *
from capstone.x86_const import X86_OP_IMM
root=Path.cwd();base=root/'.analysis/gpt-dots/popup-digit-draw-20261025'
manifest=tomllib.loads((root/'config/match-units.toml').read_text());spec=importlib.util.spec_from_file_location('compare',root/'scripts/compare-linked-function.py');compare=importlib.util.module_from_spec(spec);spec.loader.exec_module(compare)
target=compare.verified_target();decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
result={'target_sha256':target.sha256,'acceptance':'none','trials':{}}
def inventory(im,pubs,address,size):
    ins=list(decoder.disasm(im.read_address(address,size),address)); assert sum(i.size for i in ins)==size
    starts={i.address for i in ins};locals=[];calls=[]
    for i in ins:
        if i.group(CS_GRP_JUMP) and i.operands[0].type==X86_OP_IMM:
            assert i.operands[0].imm in starts
            locals.append([i.address-address,i.size,i.operands[0].imm-address])
        if i.group(CS_GRP_CALL):calls.append([i.address-address,i.op_str])
    return {'size':size,'fields':linked_code_fields(im,pubs,address,size),'locals':locals,'calls':calls}
result['target']=inventory(target,[],0x42b780,540)
for trial in ('first','model','identity'):
    out=root/('build/gpt-dots/popup-digit-draw-'+trial);image=PEImage(out/'source.exe');fs=linked_functions(out/'source.exe',out/'source.map',out/'source.pdb')['functions'];f=next(f for f in fs if f['symbol']=='?Draw@PopupNumberView@@QAEHXZ');row=inventory(image,map_publics(out/'source.map'),int(f['address'],16),f['size']);row['image_sha256']=image.sha256;units={}
    for name in ('anm-render-flush-vertex-buffer','anm-render-draw-no-rotation'):
        u=dict(manifest['units'][name]);u.update(source=str((base/'PopupNumber.cpp').relative_to(root)),support_sources=['src/AnmManager.cpp'],pdb_source='src/AnmManager.cpp',output_dir=str(out.relative_to(root)),entry_symbol='?PopupNumberDrawCallback@@YIHPAX@Z',profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/Isrc','/GL','/GS','/EHs']);units[name]=u
    selected={'target_sha256':manifest['target_sha256'],'units':units}
    row['guards']={name:compare.compare_unit(name,_unit_manifest=selected) for name in units};assert all(x['result']=='exact' for x in row['guards'].values())
    result['trials'][trial]=row
(base/'review.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:{'size':v['size'],'fields':len(v['fields']['fields']),'locals':len(v['locals']),'guards':{n:r['result'] for n,r in v['guards'].items()}} for k,v in result['trials'].items()},indent=2))
