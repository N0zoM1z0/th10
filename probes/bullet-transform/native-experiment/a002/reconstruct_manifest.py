"""Print the exact historical A002 manifest; no native work or writes."""
import hashlib,json,pathlib,sys
root=pathlib.Path(__file__).resolve().parent
raw=(root.parent/'a003/manifest.proposed.json').read_bytes()
d=json.loads((root/'manifest-delta.json').read_text())
if hashlib.sha256(raw).hexdigest()!=d['base_manifest_sha256']:raise ValueError('base manifest pin')
m=json.loads(raw)
for k in d['remove']:del m[k]
m.update(d['replace'])
out=(json.dumps(m,indent=2,sort_keys=True)+'\n').encode()
if hashlib.sha256(out).hexdigest()!=d['result_manifest_sha256']:raise ValueError('result manifest pin')
sys.stdout.buffer.write(out)
