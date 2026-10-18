"""Source-only reconstruction and patch check; never writes, builds or reads native files."""
import argparse, difflib, hashlib, json, re
from pathlib import Path
BASE_SHA='9fae2594e13b7e399e7a7b27a3117495f4a54afd08907df9dcc668d5b00a1e30'
CANDIDATE_SHA='9f9c16093cebf99c958536c043a92ddefd7d3890b4179c7ac25ff19bc1cb7ca5'
PATCH_SHA='442d9314b009fe5cfd789b0d6ee898231eb24487d166cef85131d96a0bf045fa'
def need(value):
    if not value: raise ValueError('source-reconstruction-gate')
def sha(raw): return hashlib.sha256(raw).hexdigest()
def reconstruct(b):
    need(sha(b)==BASE_SHA)
    s=b.decode()
    start=s.index('void __stdcall EnemyBeginSpell(int gameState, int spellId, char *name, int value)\r\n{')
    end=s.index('\r\nstruct EnemyDropVectorView',start)
    old=s[start:end]
    need(old.endswith('}\r\n'))
    new=old.replace('    AnmVmView *stageVm;\r\n    int stageScript;\r\n','')
    pattern=r'( +)stageVm = g_AnmRenderManagerView->AllocateVm\(\);\r\n\1stageScript = (0x[0-9a-f]+);'
    new,n=re.subn(pattern,lambda m:m[1]+'CreateSpellStageAnimation(stageResource, '+m[2]+');',new)
    need(n==9)
    begin=new.index('    stageVm->flags35C |= 0x40000000u;')
    tail=new[begin:new.rfind('}\r\n')]
    need(tail.count('#pragma inline_depth(0)')==1 and tail.count('#pragma inline_depth(16)')==1)
    new=new[:begin]+'}\r\n'
    decl='static void CreateSpellStageAnimation(AnmLoadedView *resource, int script);\r\n\r\n'
    helper='\r\nstatic void CreateSpellStageAnimation(AnmLoadedView *resource, int script)\r\n{\r\n    AnmVmView *stageVm = g_AnmRenderManagerView->AllocateVm();\r\n'+tail.replace('stageResource->InitializeVm(stageVm, stageScript);','resource->InitializeVm(stageVm, script);')+'}\r\n'
    c=s[:start]+decl+new+helper+s[end:]
    raw=c.encode();need(sha(raw)==CANDIDATE_SHA);return raw

def patch_bytes(base,candidate):
    return ''.join(difflib.unified_diff(base.decode().splitlines(True),candidate.decode().splitlines(True),fromfile='a/src/EnemyEclDispatcher.cpp',tofile='b/src/EnemyEclDispatcher.cpp')).encode()

def verify(source):
    need(not source.is_symlink() and source.is_file() and source.stat().st_size<=4*1024*1024)
    base=source.read_bytes();candidate=reconstruct(base)
    patch=Path(__file__).with_name('lifecyclecandidate.patch').read_bytes()
    need(sha(patch)==PATCH_SHA and patch_bytes(base,candidate)==patch)
    return {'source_verified':True,'baseline_sha256':BASE_SHA,'candidate_sha256':CANDIDATE_SHA,'patch_sha256':PATCH_SHA,'native_executed':False,'exactness_verified':False,'production_changed':False}

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--source',type=Path,required=True);a=p.parse_args()
    print(json.dumps(verify(a.source),sort_keys=True))
