#!/usr/bin/env python3
"""Reconstruct the conditional native source overlays in an absent separate tree."""
import argparse,hashlib,json,subprocess,tempfile,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parent
def require(ok,message):
 if not ok:raise ValueError(message)
def sha(data):return hashlib.sha256(data).hexdigest()
def reconstruct(base,a003,output):
 base=Path(base).resolve();a003=Path(a003).resolve();output=Path(output).absolute()
 require(not output.exists() and not output.is_symlink(),'output must not exist');output=output.resolve()
 for source in (base,a003,ROOT):require(source!=output and source not in output.parents and output not in source.parents,'output must be separate from all inputs')
 m=json.loads((ROOT/'manifest.json').read_text())
 for rel,digest in m['files'].items():require(sha((ROOT/rel).read_bytes())==digest,'packet hash mismatch: '+rel)
 values={}
 for group,tree in (('a003_inputs',a003),('production_inputs',base)):
  for rel,digest in m[group].items():
   data=(tree/rel).read_bytes();require(sha(data)==digest,'input raw hash mismatch: '+rel);values[rel]=data
 result={}
 with tempfile.TemporaryDirectory(prefix='mode-transition-source-') as td:
  work=Path(td)
  for rel,digest in m['overlays'].items():
   name=Path(rel).name;patch=ROOT/'patches'/(name+'.patch')
   if patch.exists():
    data=values[rel].replace(b'\r\n',b'\n');require(b'\r' not in data,'mixed newline');dest=work/name;dest.write_bytes(data)
    proc=subprocess.run(['patch','--batch','--forward','--fuzz=0',str(dest),str(patch)],capture_output=True,text=True,timeout=15)
    require(proc.returncode==0,'zero-fuzz patch failed: '+name);data=dest.read_bytes();require(b'\r' not in data,'patch newline')
    if m['output_newlines'][rel]=='CRLF':data=data.replace(b'\n',b'\r\n')
   else:data=values[rel] if rel in values else (ROOT/rel).read_bytes()
   require(sha(data)==digest,'output raw hash mismatch: '+rel);result[rel]=data
  output.mkdir(parents=False,exist_ok=False);(output/'src').mkdir()
  for rel,data in result.items():(output/rel).write_bytes(data)
  for rel,digest in m['overlays'].items():require(sha((output/rel).read_bytes())==digest,'written raw hash mismatch: '+rel)
 return m['overlays']
def main():
 require(not sys.flags.optimize,'optimized Python is unsupported')
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--base-tree',required=True,type=Path);p.add_argument('--a003-tree',required=True,type=Path);p.add_argument('--output',required=True,type=Path);a=p.parse_args()
 print(json.dumps(reconstruct(a.base_tree,a.a003_tree,a.output),indent=2))
if __name__=='__main__':main()
