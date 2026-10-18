"""Run only synthetic/mocked tests in temporary historical packet layout."""
import hashlib,json,pathlib,shutil,subprocess,sys,tempfile
root=pathlib.Path(__file__).resolve().parent
raw=(root/'a003/manifest.proposed.json').read_bytes()
d=json.loads((root/'a001-manifest-delta.json').read_text())
if hashlib.sha256(raw).hexdigest()!=d['base_manifest_sha256']:raise ValueError('base manifest pin')
m=json.loads(raw)
for k in d['remove']:del m[k]
m.update(d['replace']);out=(json.dumps(m,indent=2,sort_keys=True)+'\n').encode()
if hashlib.sha256(out).hexdigest()!=d['result_manifest_sha256']:raise ValueError('A001 manifest pin')
with tempfile.TemporaryDirectory() as tmp:
 t=pathlib.Path(tmp);shutil.copytree(root/'a003',t/'a003',ignore=shutil.ignore_patterns('__pycache__'))
 (t/'bullet1454-native-plan').mkdir();(t/'bullet1454-native-plan/manifest.proposed.json').write_bytes(out)
 subprocess.run([sys.executable,'-m','unittest','discover','-s',str(t/'a003'),'-p','test_*.py'],check=True,timeout=30)
 subprocess.run([sys.executable,'-m','unittest','discover','-s',str(root/'helper-relocation-form'),'-p','test_*.py'],check=True,timeout=30)
 subprocess.run([sys.executable,str(root/'a002/reconstruct_manifest.py')],stdout=subprocess.DEVNULL,check=True,timeout=10)
