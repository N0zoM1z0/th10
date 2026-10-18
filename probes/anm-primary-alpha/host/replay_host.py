#!/usr/bin/env python3
"""Reconstruct two pinned sources transiently, check reversible bytes, run host contracts."""
import argparse,hashlib,json,pathlib,shutil,subprocess,sys,tempfile
P=pathlib.Path;ROOT=P(__file__).resolve().parent
def need(ok,msg):
 if not ok:raise ValueError(msg)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def reconstruct(source_root,work):
 ids=json.loads((ROOT/'identities.json').read_text())['sources'];patch=ROOT/'hypothesis.patch'
 need(all(not p.is_symlink() for p in [source_root,*source_root.parents]),'symlink source root')
 need(sha(patch)=='254e67b507f4d0da9b6ba102147fde7fbabdc2ca5393d8dac0aad53ab873e43c','patch identity drift')
 for name,identity in ids.items():
  src=source_root/'src'/name;need(not src.is_symlink() and not src.parent.is_symlink(),'symlink source')
  need(sha(src)==identity['baseline_sha256'],'requires exact baseline source: '+name)
  for kind in ('baseline','candidate'):
   dst=work/kind/'src'/name;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(src,dst)
 for reverse,kind in ((False,'candidate'),(True,'reverse')):
  if reverse:shutil.copytree(work/'candidate',work/'reverse')
  with patch.open('rb') as inp:
   result=subprocess.run(['patch','--binary','--batch','--fuzz=0','-p1',*(['-R'] if reverse else [])],cwd=work/kind,stdin=inp,stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=10)
  need(result.returncode==0,'patch apply/reverse failed')
  for name,identity in ids.items():
   p=work/kind/'src'/name;expected=identity['baseline_sha256' if reverse else 'candidate_sha256']
   need(sha(p)==expected,'raw byte reconstruction failed')
   data=p.read_bytes();need(data.count(b'\n')==data.count(b'\r\n') if name.endswith('.cpp') else b'\r' not in data,'newline drift')
 for kind in ('baseline','candidate','reverse'):
  need({str(p.relative_to(work/kind)) for p in (work/kind).rglob('*') if p.is_file()}=={'src/'+n for n in ids},'unexpected reconstructed files')
 return {'forward_byte_exact':True,'reverse_byte_exact':True,'cpp_newlines':'CRLF','hpp_newlines':'LF'}
def main():
 need(__debug__,'optimized Python unsupported');ap=argparse.ArgumentParser();ap.add_argument('--source-root',type=P,required=True);ap.add_argument('--check-only',action='store_true');ap.add_argument('--output',type=P);a=ap.parse_args()
 with tempfile.TemporaryDirectory(prefix='anm-alpha-archive-host-') as tmp:
  work=P(tmp);out=reconstruct(a.source_root.absolute(),work)
  if not a.check_only:
   for name in ('test_contract.py','identities.json'):shutil.copyfile(ROOT/name,work/name)
   done=subprocess.run([sys.executable,str(work/'test_contract.py')],cwd=work,stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=180)
   need(done.returncode==0,'host contract failed')
   out['host_results']=json.loads((work/'host-results.json').read_text());out['host_variants']=len(out['host_results'])
  if a.output:a.output.write_text(json.dumps(out,indent=2)+'\n')
  print(json.dumps({k:v for k,v in out.items() if k!='host_results'},sort_keys=True))
if __name__=='__main__':main()
