"""Reconstruct historical packets offline from immutable A003 inputs only."""
import argparse,hashlib,json,pathlib
P=pathlib.Path

def digest(raw):return hashlib.sha256(raw).hexdigest()
def require(v):
 if not v:raise ValueError('archival reconstruction mismatch')
def splice(raw,edits):
 text=raw.decode('utf-8');parts=[];end=0
 for edit in edits:
  a,b=edit['start'],edit['end'];require(type(a)is int and type(b)is int and end<=a<=b<=len(text) and type(edit['replacement'])is str);parts.extend([text[end:a],edit['replacement']]);end=b
 parts.append(text[end:]);return ''.join(parts).encode('utf-8')
def reconstruct(base_runner,base_manifest,base_overlay,recipes,tag):
 require(tag in {'a004','r001','a005','a006','a007','a008'});require(digest(base_runner)==recipes['base_runner_sha256'] and digest(base_manifest)==recipes['base_manifest_sha256'] and digest(base_overlay)==recipes['base_overlay_sha256']);spec=recipes['recipes'][tag]
 runner=splice(base_runner,spec['runner_splices']);overlay=splice(base_overlay,spec['overlay_splices']);m=json.loads(base_manifest)
 for key in spec['manifest_remove']:del m[key]
 m.update(spec['manifest_replace']);manifest=(json.dumps(m,indent=2,sort_keys=True)+'\n').encode()
 require(digest(runner)==spec['runner_sha256'] and digest(manifest)==spec['manifest_sha256'] and digest(overlay)==spec['overlay_sha256'])
 return {'run.py':runner,'manifest.proposed.json':manifest,'src/BulletTransform.cpp':overlay}
def materialize(output,files):
 require(not output.exists() and not output.is_symlink());require(not any(p.is_symlink() for p in output.parents));output.mkdir(parents=False,exist_ok=False)
 for name,raw in files.items():
  dest=output/name;dest.parent.mkdir(exist_ok=True);dest.write_bytes(raw)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('recipe',choices=['a004','r001','a005','a006','a007','a008']);ap.add_argument('--repo',type=P,default=P(__file__).resolve().parents[4]);ap.add_argument('--output',type=P,required=True);a=ap.parse_args();root=a.repo.resolve();require(not a.output.resolve().is_relative_to(root));base=root/'probes/bullet-transform/native-experiment/a003';spec=json.loads(P(__file__).with_name('recipes.json').read_text());files=reconstruct((base/'run.py').read_bytes(),(base/'manifest.proposed.json').read_bytes(),(base/'src/BulletTransform.cpp').read_bytes(),spec,a.recipe);materialize(a.output,files);print(json.dumps({'recipe':a.recipe,'execution_performed':False,'sha256':{k:digest(v) for k,v in files.items()}},sort_keys=True))
if __name__=='__main__':main()
