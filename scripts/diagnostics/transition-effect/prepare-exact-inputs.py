#!/usr/bin/env python3
"""Restore hash-exact LF/CRLF variants only, in an explicit clean linked worktree.
Default is check-only. Never builds, executes a PE, or rewrites a native plan.
"""
import argparse,hashlib,json,os,stat,subprocess,tempfile
from pathlib import Path

def require(ok,msg):
    if not ok:raise RuntimeError(msg)
def sha(b):return hashlib.sha256(b).hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--fresh-worktree',type=Path,required=True)
    p.add_argument('--plan',type=Path,required=True)
    p.add_argument('--apply',action='store_true')
    a=p.parse_args();root=a.fresh_worktree.resolve();plan=json.loads(a.plan.read_bytes())
    historical=json.loads(Path(__file__).with_name('native-inputs.json').read_bytes())
    require(plan['input_sha256']==historical['input_sha256'],'historical native pins changed')
    require(set(plan['candidate_file_sha256'])==set(historical['candidate_file_sha256']),'candidate scope changed')
    require((root/'.git').is_file(),'use an explicit linked git worktree, not the current checkout')
    def git(*args):return subprocess.run(['git','-C',str(root),*args],check=True,capture_output=True,text=True).stdout.strip()
    require(Path(git('rev-parse','--show-toplevel')).resolve()==root,'not worktree root')
    admin=Path(git('rev-parse','--absolute-git-dir')).resolve()
    common=Path(git('rev-parse','--path-format=absolute','--git-common-dir')).resolve()
    require(admin!=common and (admin/'commondir').is_file() and (admin/'gitdir').is_file(),'not a linked worktree admin directory')
    require(Path((admin/'gitdir').read_text().strip()).resolve()==(root/'.git').resolve(),'worktree linkage mismatch')
    require(not git('status','--porcelain','--untracked-files=all'),'worktree must be clean before preparation')
    for name in {historical['output_dir'],plan['output_dir']}:
        require(not os.path.lexists(root/name),'existing observation/output must be preserved: '+name)
    pins=dict(plan['candidate_file_sha256'])
    for n,h in plan['input_sha256'].items():
        require(n not in pins or pins[n]==h,'overlapping pin mismatch: '+n);pins[n]=h
    changes=[];verified=[]
    for name,expected in sorted(pins.items()):
        rel=Path(name);path=root/rel
        require(not rel.is_absolute() and '..' not in rel.parts and path.resolve().is_relative_to(root),'unsafe input path: '+name)
        require(path.is_file() and not any((root/Path(*rel.parts[:i])).is_symlink() for i in range(1,len(rel.parts)+1)),'missing or symlinked input: '+name)
        raw=path.read_bytes();lf=raw.replace(b'\r\n',b'\n')
        require(b'\r' not in lf,'bare CR is not an approved EOL variant: '+name)
        variants=set([raw,lf,lf.replace(b'\n',b'\r\n')]);matches=[b for b in variants if sha(b)==expected]
        require(len(matches)==1,'content mismatch; no exact EOL-only reconstruction: '+name)
        target=matches[0];verified.append({'path':name,'before_sha256':sha(raw),'expected_sha256':expected,'changed':raw!=target})
        if raw!=target:changes.append((path,raw,target,stat.S_IMODE(path.stat().st_mode)))
    # All inputs validate before any byte is changed. Recheck against concurrent edits.
    for path,raw,target,mode in changes:require(path.read_bytes()==raw,'input changed during preparation: '+str(path))
    if a.apply:
        for path,raw,target,mode in changes:
            fd,tmp=tempfile.mkstemp(prefix='.exact-eol-',dir=path.parent)
            try:
                with os.fdopen(fd,'wb') as stream:stream.write(target)
                os.chmod(tmp,mode)
                require(not path.is_symlink() and path.read_bytes()==raw,'input changed before replacement: '+str(path))
                os.replace(tmp,path)
            finally:
                if os.path.exists(tmp):os.unlink(tmp)
        for name,h in pins.items():require(sha((root/name).read_bytes())==h,'post-write hash failure: '+name)
    print(json.dumps({'result':'restored-exact-inputs' if a.apply else 'verified-reconstructible-no-writes','worktree':str(root),'plan_sha256':sha(a.plan.read_bytes()),'native_pin_count':len(plan['input_sha256']),'candidate_pin_count':len(plan['candidate_file_sha256']),'changed_file_count':len(changes),'files':verified},indent=2))
if __name__=='__main__':main()
