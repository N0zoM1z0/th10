"""Reconstruct historical packets offline. Never execute their ready manifests."""
import argparse,hashlib,json,pathlib
P=pathlib.Path

def digest(raw):return hashlib.sha256(raw).hexdigest()
def require(v):
 if not v:raise ValueError('archival reconstruction mismatch')
def reconstruct(base_runner,base_manifest,overlay,recipes,tag):
 require(tag in {'a004','r001'});require(digest(base_runner)==recipes['base_runner_sha256'] and digest(base_manifest)==recipes['base_manifest_sha256']);spec=recipes['recipes'][tag];text=base_runner.decode('utf-8');parts=[];end=0
 for edit in spec['runner_splices']:
  a,b=edit['start'],edit['end'];require(type(a)is int and type(b)is int and end<=a<=b<=len(text));parts.extend([text[end:a],edit['replacement']]);end=b
 parts.append(text[end:]);runner=''.join(parts).encode('utf-8');m=json.loads(base_manifest)
 for key in spec['manifest_remove']:del m[key]
 m.update(spec['manifest_replace']);manifest=(json.dumps(m,indent=2,sort_keys=True)+'\n').encode()
 require(digest(runner)==spec['runner_sha256'] and digest(manifest)==spec['manifest_sha256'] and digest(overlay)==spec['overlay_sha256'])
 return {'run.py':runner,'manifest.proposed.json':manifest,'src/BulletTransform.cpp':overlay}
def materialize(output,files):
 require(not output.exists() and not output.is_symlink());require(not any(p.is_symlink() for p in output.parents));output.mkdir(parents=False,exist_ok=False)
 for name,raw in files.items():
  dest=output/name;dest.parent.mkdir(exist_ok=True);dest.write_bytes(raw)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('recipe',choices=['a004','r001']);ap.add_argument('--repo',type=P,default=P(__file__).resolve().parents[4]);ap.add_argument('--overlay',type=P);ap.add_argument('--output',type=P,required=True);a=ap.parse_args();root=a.repo.resolve();require(not a.output.resolve().is_relative_to(root));base=root/'probes/bullet-transform/native-experiment/a003';overlay=a.overlay or root/'src/BulletTransform.cpp';spec=json.loads(P(__file__).with_name('recipes.json').read_text());files=reconstruct((base/'run.py').read_bytes(),(base/'manifest.proposed.json').read_bytes(),overlay.read_bytes(),spec,a.recipe);materialize(a.output,files);print(json.dumps({'recipe':a.recipe,'execution_performed':False,'sha256':{k:digest(v) for k,v in files.items()}},sort_keys=True))
if __name__=='__main__':main()
