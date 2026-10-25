#!/usr/bin/env python3
"""Exercise actual type1 lifecycle plus real timer, VM-wrapper, contact and sound bodies.

Host layout/constructor prefills and polar scratch Z are explicit adaptations.
VM reset is pure observation. Script binding/execution, world creation, rendering,
collision hit and unused virtuals are roles. No native ABI/FP/lifetime/runtime claim.
"""
import argparse,hashlib,json,os,re,runpy,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
body=runpy.run_path(str(ROOT/'scripts/test-frontend-replay-save.py'))['body']


def run(args,out):
    out.mkdir(parents=True,exist_ok=True);hashes={};pending=['EnemyLaserType1.hpp','Enemy.hpp','EnemyPrimaryResources.hpp','EnemySoundQueue.hpp','Gui.hpp'];seen=set()
    while pending:
        name=pending.pop()
        if name in seen:continue
        seen.add(name);p=ROOT/'src'/name;text=p.read_text();hashes['src/'+name]=hashlib.sha256(p.read_bytes()).hexdigest()
        pending.extend(re.findall(r'^\s*#\s*include\s*"([^"]+)"',text,re.M))
        text=re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;','// HOST native assertion omitted.',text)
        for cls,field in [('AnmVmTimerView','flags'),('PlayerTimerView','flags'),('PlayerOptionRuntime','flags80'),('PlayerConstructorFlagView','flags'),('EnemyInterpolationTimerView','flags')]:
            text=text.replace(f'{cls}() {{ {field} &= ~1u; }}',f'{cls}() : {field}(0) {{}}')
        if name == 'EnemyLaser.hpp':
            # HOST ONLY: storage sentinel avoids native erased-vptr base construction.
            # This fixture does not validate manager/base ctor, layout or vtable.
            assert text.count('EnemyLaserConstructionBaseView sentinel;') == 1
            text=text.replace('EnemyLaserConstructionBaseView sentinel;', 'EnemyLaserCommonStorageView sentinel;')
        (out/name).write_text(text)
    def read(name):
        p=ROOT/'src'/name;hashes['src/'+name]=hashlib.sha256(p.read_bytes()).hexdigest();return p.read_text()
    source=read('LaserType1.cpp');owner=re.sub(r'^#include[^\n]*\n','',source,flags=re.M)
    owner=owner.replace('extern unsigned char g_MainSoundOwner[0x52d0];','extern unsigned char *g_MainSoundOwner; // HOST live typed queue role.')
    assert owner.count('EnemyLaserVectorView point;')==1;owner=owner.replace('EnemyLaserVectorView point;','EnemyLaserVectorView point = {0, 0, 17}; // HOST unknown native Z prefill.')
    anm=read('AnmManager.cpp');contact=read('LaserLine.cpp');sound=read('EnemyEclDispatcher.cpp')
    helpers='\n'.join([body(anm,'void AnmVmView::InitializeForLoadedScript('),body(anm,'float __stdcall AddNormalizeAngle('),body(contact,'int Player::CheckLaserContact('),body(sound,'void EnemySoundQueueView::QueueSoundCue(')])
    (out/'helpers.inc').write_text(helpers)
    variants=[('positive',owner,None)]
    mutations=[
        ('partial-copy','request = *initial;','request.position = initial->position;','complete-copy'),
        ('wrong-state','state = 3;','state = 2;','state-before-reset'),
        ('wrong-script','request.color + g_BulletSpriteScriptBase[request.type]','request.type + g_BulletSpriteScriptBase[request.color]','script-selection'),
        ('wrong-interrupt','primaryVm.pendingInterrupt = 2;','primaryVm.pendingInterrupt = 1;','pending-before-execute'),
        ('wrong-primary-gate','request.flags44 & 2u','request.flags44 & 1u','primary-gated-bit2'),
        ('wrong-primary-mask','0xfc63ffffu','0xfc7fffffu','primary-mask'),
        ('wrong-secondary-mask','0xfc7fffffu','0xfc63ffffu','secondary-mask'),
        ('cached-resource','reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->effectResource','savedResource','live-secondary-resource'),
        ('early-geometry','position = request.position;','position = initial->position;','late-owned-geometry'),
        ('wrong-width','width = 2.0f;','width = request.width;','initial-geometry'),
        ('wrong-growth-field','growSpeed = request.growSpeed;','growSpeed = request.width;','initial-geometry'),
        ('wrong-return','return 0;','return 1;','initialize-return'),
        ('omit-growth-speed','length += g_AnmGameSpeed * growSpeed;','length += growSpeed;','length-speed'),
        ('omit-cap','length = request.maximumLength;','length = length;','length-cap'),
        ('omit-angular-speed','g_AnmGameSpeed * request.angularVelocity','request.angularVelocity','angular-speed'),
        ('wrong-follow-gate','request.flags44 & 1u','request.flags44 & 2u','attach-before-motion'),
        ('omit-z-motion','delta.z = g_AnmGameSpeed * request.velocity.z;','delta.z = 0;','request-motion'),
        ('wait-boundary','age.current >= request.waitFrames','age.current > request.waitFrames','phase-wait'),
        ('skip-reset','age.SetCurrent(0);','// omitted','phase-timer-reset'),
        ('integer-growth','request.width * age.subframe / request.growFrames','request.width * age.current / request.growFrames','fractional-growth'),
        ('stop-growth-fallthrough','width = request.width;','width = request.width; break;','phase-fallthrough'),
        ('wrong-shrink','fullWidth - fullWidth * age.subframe','fullWidth + fullWidth * age.subframe','fractional-shrink'),
        ('retirement-boundary','age.current >= request.shrinkFrames','age.current > request.shrinkFrames','retirement'),
        ('half-taper','0.3333333432674408f','0.5f','collision-taper'),
        ('omit-growth-contact','state == 4 || state == 2','state == 2','growth-contact'),
        ('inclusive-length','length > 16.0f','length >= 16.0f','contact-length'),
        ('wrong-hit-box','hitSize.x = 32.0f;','hitSize.x = 31.0f;','hit-box-contract'),
        ('graze-period','age.current % 5 == 0','age.current % 4 == 0','signed-graze-period'),
        ('wrong-sprite-axis','width / sprite->width','width / sprite->height','scale-axes'),
        ('omit-scale-flag','flags |= 8u;','flags |= 0u;','scale-flags'),
        ('inclusive-tail','traveled == 0.0f','traveled >= 0.0f','script-tail-live'),
        ('wrong-draw-field','primaryVm.position.x =','primaryVm.positionOffset.x =','draw-field'),
        ('wrong-draw-offset','position.y + 16.0f','position.y + 15.0f','draw-offsets'),
        ('cached-draw-owner','g_AnmRenderManagerView->Draw(&secondaryVm);','savedRenderer->Draw(&secondaryVm);','fresh-draw-owner'),
    ]
    if not args.no_mutations:
        for name,old,new,label in mutations:
            count=owner.count(old);assert count==({'cached-resource':2,'skip-reset':3,'inclusive-tail':2,'wrong-return':3,'wrong-draw-offset':2}.get(name,1)),(name,count)
            text=owner.replace(old,new)
            if name=='cached-resource':text=text.replace('request = *initial;','AnmLoadedView *savedResource = reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->effectResource;\n    request = *initial;')
            if name=='cached-draw-owner':text=text.replace('int EnemyLaserType1View::Draw()\n{','int EnemyLaserType1View::Draw()\n{\n    AnmRenderManagerView *savedRenderer = g_AnmRenderManagerView;')
            variants.append((name,text,label))
    if args.variant:variants=[v for v in variants if v[0]==args.variant];assert variants,'unknown variant'
    for name,text,expected in variants:
        (out/'owner.inc').write_text(text)
        cmd=['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Wno-class-memaccess','-Wno-implicit-fallthrough','-fno-strict-aliasing','-I',str(out),str(ROOT/'scripts/fixtures/laser-type1.cpp'),'-o',str(out/'fixture')]
        if args.sanitize:cmd+=['-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']
        compiled=subprocess.run(cmd,capture_output=True,text=True)
        if compiled.returncode:raise RuntimeError(name+'\n'+compiled.stdout+compiled.stderr)
        result=subprocess.run([str(out/'fixture')],capture_output=True,text=True,env={**os.environ,'ASAN_OPTIONS':'detect_leaks=1'})
        want=(0,'','')if expected is None else(1,'','CHECK '+expected+'\n')
        if (result.returncode,result.stdout,result.stderr)!=want:raise RuntimeError(f'{name}: {result.returncode}\n{result.stdout}{result.stderr}')
        print('PASS '+name,flush=True)
    hashes['scripts/fixtures/laser-type1.cpp']=hashlib.sha256((ROOT/'scripts/fixtures/laser-type1.cpp').read_bytes()).hexdigest()
    (out/'manifest.json').write_text(json.dumps({'scope':__doc__,'inputs':hashes,'sanitize':args.sanitize,'complete_suite':args.variant is None,'variants':[v[0]for v in variants]},indent=2)+'\n')


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--sanitize',action='store_true');p.add_argument('--no-mutations',action='store_true');p.add_argument('--variant');p.add_argument('--out',type=Path);a=p.parse_args()
    if a.out:run(a,a.out)
    else:
        parent=ROOT/'build/host-tests';parent.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='laser-type1-',dir=parent)as d:run(a,Path(d))


if __name__=='__main__':main()
