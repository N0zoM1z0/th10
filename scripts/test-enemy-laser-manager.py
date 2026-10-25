#!/usr/bin/env python3
"""Extract actual EnemyLaser manager constructor, Initialize, cleanup and factory.

Genuine Chain and GameErrorContext declarations; explicit test-only nonnative
Main critical-section, ANM preload and nonvirtual base/list storage seams.
The base constructor is an observation role, NOT the native erased-vptr object.
Actual base whole-memset/timer order and factory null-this hazard are checked
structurally, never executed. No sanitizer suppression, ABI/layout/vtable,
Windows locking, allocation-failure, target-runtime or exactness claim.
"""
import argparse,hashlib,json,os,re,runpy,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
body=runpy.run_path(str(ROOT/'scripts/test-frontend-replay-save.py'))['body']
ERROR=bytes.fromhex('93 47 92 65 83 66 81 5b 83 5e 82 aa 8c a9 82 c2 82 a9 82 e8 82 dc 82 b9 82 f1 81 42 83 66 81 5b 83 5e 82 aa 89 f3 82 ea 82 c4 82 a2 82 dc 82 b7 0d 0a 00')
def structural(src,h):
    ctor=body(h,'EnemyLaserManagerStorageView()')
    assert 'memset(this, 0, sizeof(*this));' in ctor
    assert ctor.index('memset(')<ctor.index('g_EnemyLaserManager =')
    assert 'EnemyLaserConstructionBaseView sentinel;' in h
    base=body(h,'EnemyLaserConstructionBaseView()')
    assert base.index('memset(this, 0, sizeof(*this));')<base.index('timer.SetCurrent(0);')
    init=body(src,'int EnemyLaserManagerStorageView::Initialize()')
    literal=re.search(r'Log\("([^"\n]+)"\)',init).group(1)
    actual=bytes(int(x,16) for x in re.findall(r'\\x([0-9a-fA-F]{2})',literal))+b'\0'
    assert actual==ERROR and len(actual)==51
    assert 'PreloadAnm(7, "bullet.anm")' in init
    factory=body(src,'void *GameCreateOwner41C290()')
    assert 'if (manager->Initialize())' in factory
    assert factory.index('new EnemyLaserManagerStorageView')<factory.index('manager->Initialize()')<factory.index('if (manager)')
    assert 'g_EnemyLaserManager =' not in factory # new-null skips ctor publication, still calls Initialize.
    cleanup=body(src,'void __stdcall GameCleanupLinked41C1C0(')
    assert cleanup.index('savedNext = node->next')<cleanup.index('->Unlink()')<cleanup.index('node->previous->next = node->next')<cleanup.index('::operator delete(node)')<cleanup.index('node = savedNext')
    assert 'Release' not in cleanup and 'effectResource' not in cleanup
    return ctor.replace('EnemyLaserManagerStorageView()', 'EnemyLaserManagerStorageView::EnemyLaserManagerStorageView()',1)
def run(a,out):
    out.mkdir(parents=True,exist_ok=True);hashes={}
    def read(n):
        p=ROOT/'src'/n;hashes['src/'+n]=hashlib.sha256(p.read_bytes()).hexdigest();return p.read_text()
    src=read('EnemyLaserManager.cpp');h=read('EnemyLaser.hpp');ctor=structural(src,h)
    for n in ['Chain.hpp','GameErrorContext.hpp']:(out/n).write_text(read(n))
    (out/'expected.inc').write_text('static char expectedError[]="'+''.join('\\x%02x'%x for x in ERROR[:-1])+'";\n')
    owner=ctor+'\n'+re.sub(r'^#include[^\n]*\n','',src,flags=re.M)
    variants=[('positive',owner)]
    if not a.no_mutations:
        for name,old,new in [('wrong-priority','task, 0x13, chain','task, 0x12, chain'),('wrong-mask','task->flags &= ~2u;','task->flags &= ~1u;'),('wrong-chain-capture','th10::Chain::UnregisterElem(task, chain);','th10::Chain::UnregisterElem(task, th10::g_Chain);')]:
            assert old in owner;variants.append((name,owner.replace(old,new)))
        for old,new in [('savedNext = node->next','savedNext = node->previous'),('if (manager->Initialize())','if (manager && manager->Initialize())')]:
            try:structural(src.replace(old,new),h)
            except (AssertionError,ValueError):pass
            else:raise AssertionError('structural mutation survived: '+old)
    for name,text in variants:
        (out/'owner.inc').write_text(text)
        cmd=['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-I',str(out),str(ROOT/'scripts/fixtures/enemy-laser-manager.cpp'),'-o',str(out/'fixture')]
        if a.sanitize:cmd+=['-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']
        c=subprocess.run(cmd,text=True,capture_output=True)
        if c.returncode:raise RuntimeError(c.stdout+c.stderr)
        r=subprocess.run([str(out/'fixture')],text=True,capture_output=True,env={**os.environ,'ASAN_OPTIONS':'detect_leaks=1'})
        if name=='positive':
            if r.returncode:raise RuntimeError(r.stdout+r.stderr)
        else:
            expected={'wrong-priority':'p==0x13','wrong-mask':'t->flags==0xd','wrong-chain-capture':'c==captured'}[name]
            if r.returncode!=-6 or 'Assertion' not in r.stderr or expected not in r.stderr:raise RuntimeError(name+' not rejected by intended assertion '+expected+': '+r.stdout+r.stderr)
        print('PASS '+name,flush=True)
    (out/'manifest.json').write_text(json.dumps({'scope':__doc__,'inputs':hashes,'sanitize':a.sanitize,'variants':[v[0]for v in variants],'structural_mutations':2 if not a.no_mutations else 0},indent=2)+'\n')
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--sanitize',action='store_true');p.add_argument('--no-mutations',action='store_true');p.add_argument('--out',type=Path);a=p.parse_args()
    if a.out:run(a,a.out)
    else:
        parent=ROOT/'build/host-tests';parent.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='enemy-laser-manager-',dir=parent)as d:run(a,Path(d))
if __name__=='__main__':main()
