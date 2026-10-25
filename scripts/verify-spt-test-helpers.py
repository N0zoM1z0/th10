#!/usr/bin/env python3
"""Replay three complete SptTest helpers and nine same-image dependency guards."""
import argparse,importlib.util,json,tomllib
from pathlib import Path
from linked_image import PEImage,linked_functions
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM
ROOT=Path(__file__).resolve().parents[1]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--output-dir',default='build/match-linked/SptTestHelpers-cold');a=ap.parse_args()
 p=(ROOT/a.output_dir).resolve();assert p.is_relative_to(ROOT/'build')
 spec=importlib.util.spec_from_file_location('compare',ROOT/'scripts/compare-linked-function.py');c=importlib.util.module_from_spec(spec);spec.loader.exec_module(c)
 m=tomllib.loads((ROOT/'config/match-units.toml').read_text());keys=['spt-test-constructor','spt-test-register-tasks','spt-test-clear-script-names'];units={k:dict(m['units'][k],output_dir=str(p.relative_to(ROOT))) for k in keys};base=units[keys[0]]
 for k,src in [('spt-test-draw','SptTest'),('spt-test-draw-callback','SptTest'),('ascii-manager-add-small-format-text-gs','AnmManager'),('decomphelp-chain-unregister','Chain'),('script-viewer-worker-destructor','ScriptViewerWorker'),('script-viewer-worker-stop','ScriptViewerWorker'),('script-viewer-worker-start','ScriptViewerWorker'),('anm-loaded-release','AnmManager'),('anm-release-slot','AnmManager')]:
  old=m['units'][k];units[k]=dict(base,**{x:old[x] for x in ['symbol','functions','target_address','size','linkages']},pdb_source='src/'+src+'.cpp')
 selected={'target_sha256':m['target_sha256'],'units':units};results={k:c.compare_unit(k,_unit_manifest=selected) for k in units};assert all(r['result']=='exact' for r in results.values())
 im=PEImage(p/'source.exe');tar=c.verified_target();fs=linked_functions(p/'source.exe',p/'source.map',p/'source.pdb')['functions'];d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True
 def locals(image,address,size):
  ii=list(d.disasm(image.read_address(address,size),address));assert sum(i.size for i in ii)==size;starts={i.address for i in ii};out=[]
  for i in ii:
   if i.group(CS_GRP_JUMP) and i.operands[0].type==X86_OP_IMM:
    dest=i.operands[0].imm
    if address<=dest<address+size:assert dest in starts;out.append([i.address-address,i.size,dest-address])
  return out
 checked={}
 for k,u in units.items():
  f=[x for x in fs if x['symbol']==u['symbol']];assert len(f)==1;expected=locals(tar,u['target_address'],u['size']);assert locals(im,int(f[0]['address'],16),f[0]['size'])==expected;checked[k]=expected
 assert checked['spt-test-clear-script-names']==[[5,2,85],[15,2,54],[25,2,46],[52,2,17],[60,2,78]]
 result={'result':'exact','authored_new_bytes':315,'helpers':3,'same_image_guard_bytes':1225,'units':results,'fixed_local_targets':checked};print(json.dumps(result,indent=2))
if __name__=='__main__':main()
