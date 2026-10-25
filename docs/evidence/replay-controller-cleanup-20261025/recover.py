from pathlib import Path
import sys,json,hashlib
sys.path.insert(0,'scripts')
from linked_image import PEImage,linked_functions
root=Path.cwd()
e=root/'docs/evidence/replay-controller-cleanup-20261025'
frozen=json.loads((e/'retained-artifacts.json').read_text())
for path,digest in frozen.items():
    assert hashlib.sha256((root/path).read_bytes()).hexdigest()==digest,path
p=root/'build/replay-controller-cleanup-replay-eh-20261025'
functions=linked_functions(p/'source.exe',p/'source.map',p/'source.pdb')['functions']
selected=[{k:v for k,v in f.items() if k in ['symbol','address','size','sha256','extent_source','map_object']} for f in functions if 'GameCleanupController422220' in f['symbol'] or f['symbol']=='??1ReplayManager@@QAE@XZ']
result={'recovery':'retained PE/MAP/PDB only; no native rerun','original_exit':1,'original_failure':'post-link PosixPath support-profile key JSON serialization','original_artifacts_unchanged':len(frozen),'image_sha256':PEImage(p/'source.exe').sha256,'functions':selected}
out=root/'.analysis/gpt-dots/replay-controller-cleanup-20261025/recovery.json'
out.write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
