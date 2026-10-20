#!/usr/bin/env python3
"""Exercise actual line Initialize/Motion0040, command parser and shared helpers.

Host layout assertions and constructor prefills are adapted explicitly. Native
construction/vtables, VM reset/bind/execute and polar math are roles; timer,
command, aim, sound and embedded-VM wrapper bodies are real. No native ABI,
union-lifetime, x87 or product/runtime claim.
"""
import argparse,hashlib,json,os,re,runpy,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
body=runpy.run_path(str(ROOT/'scripts/test-frontend-replay-save.py'))['body']


def run(args,out):
    out.mkdir(parents=True,exist_ok=True);hashes={};pending=['EnemyLaserLine.hpp','EnemyPrimaryResources.hpp','EnemySoundQueue.hpp'];seen=set()
    while pending:
        name=pending.pop()
        if name in seen:continue
        seen.add(name);p=ROOT/'src'/name;text=p.read_text();hashes['src/'+name]=hashlib.sha256(p.read_bytes()).hexdigest()
        pending.extend(re.findall(r'^\s*#\s*include\s*"([^"]+)"',text,re.M))
        text=re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;','// HOST native assertion omitted.',text)
        for cls,field in [('AnmVmTimerView','flags'),('PlayerTimerView','flags'),('PlayerOptionRuntime','flags80'),('PlayerConstructorFlagView','flags')]:
            text=text.replace(f'{cls}() {{ {field} &= ~1u; }}',f'{cls}() : {field}(0) {{}}')
        (out/name).write_text(text)
    def read(name):
        p=ROOT/'src'/name;hashes['src/'+name]=hashlib.sha256(p.read_bytes()).hexdigest();return p.read_text()
    owners=read('LaserInitialize.cpp');owners=re.sub(r'^#include[^\n]*\n','',owners,flags=re.M)
    owners=owners.replace('extern unsigned char g_MainSoundOwner[0x52d0];','extern unsigned char *g_MainSoundOwner; // HOST live typed queue role.')
    anm=read('AnmManager.cpp');enemy=read('Enemy.cpp');sound=read('EnemyEclDispatcher.cpp');parser=read('LaserScript.cpp')
    helpers='\n'.join([body(anm,'void AnmVmView::InitializeForLoadedScript('),body(enemy,'float EnemyAngleFromPlayer('),body(sound,'void EnemySoundQueueView::QueueSoundSample('),body(sound,'void EnemySoundQueueView::QueueSoundCue('),body(parser,'void EnemyLaserLineView::RunCommands()')])
    (out/'helpers.inc').write_text(helpers)
    variants=[('positive',owners,None)]
    mutations=[
        ('skip-copy','request = *initial;','(void)initial;','complete-request-copy'),
        ('wrong-state','unknown00C = 2;','unknown00C = 1;','state-before-vm'),
        ('wrong-primary-script','request.color + g_BulletSpriteScriptBase[request.type]','request.type + g_BulletSpriteScriptBase[request.color]','signed-script-selection'),
        ('wrong-interrupt','primaryVm.pendingInterrupt = 2;','primaryVm.pendingInterrupt = 1;','pending-before-execute'),
        ('skip-primary-step','AnmRenderManagerView::ExecuteScript(vm);','(void)vm;','vm-step-order'),
        ('force-primary-flag','if ((request.flags & 1u) != 0)','if (true)','primary-gated-bit'),
        ('captured-primary-flag','if ((request.flags & 1u) != 0)','if ((capturedFlags & 1u) != 0)','flags-after-execution'),
        ('captured-resource','reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->effectResource','capturedResource','live-resource-owned-script'),
        ('wrong-primary-mask','0xfc63ffffu','0xfc7fffffu','primary-flag-mask'),
        ('wrong-secondary-script','request.color + 0x103','request.color + 0x102','signed-script-selection'),
        ('wrong-secondary-mask','0xfc7fffffu','0xfc63ffffu','secondary-flag-mask'),
        ('short-startup','holdTimer.SetCurrent(30);','holdTimer.SetCurrent(29);','startup-timer'),
        ('early-geometry','position = request.position;','position = initial->position;','late-owned-geometry'),
        ('wrong-width','width = request.width;','width = request.terminalDistance;','geometry-slots'),
        ('wrong-sound','soundId = 24;','soundId = 23;','geometry-slots'),
        ('unordered-positive','length > 0.0f ?','!(length <= 0.0f) ?','unordered-length'),
        ('overwrite-z','velocity.FromAngleMagnitude(angle, growSpeed);','velocity.FromAngleMagnitude(angle, growSpeed);\n    velocity.z = 0;','polar-z-preserved'),
        ('wrong-return','    return 0;','    return 1;','initialize-return'),
        ('turn-early','motion.timer.current >= duration','motion.timer.current >= duration - 1','fractional-deceleration'),
        ('wrong-limit','motion.values[7].integer >= motion.values[6].integer','motion.values[7].integer > motion.values[6].integer','repeat-limit'),
        ('skip-final-tail','flags &= ~0x40u;','{ flags &= ~0x40u; return; }','turn-after-clear'),
        ('normalize-turn','angle += motion.values[1].scalar;','angle = std::remainder(angle + motion.values[1].scalar, 6.2831855f);','unwrapped-angle'),
        ('stale-speed','growSpeed = motion.values[0].scalar;','growSpeed = growSpeed;','new-cycle-speed'),
        ('wrong-deceleration','speed * elapsed','speed * int(elapsed)','fractional-deceleration'),
        ('omit-reset','motion.timer.SetCurrent(0);','// omitted','reset-before-tick'),
        ('omit-tick','motion.timer.Tick();','// omitted','post-polar-tick'),
        ('zero-sound-only','if (soundId >= 0)','if (soundId > 0)','zero-sound-id'),
    ]
    if not args.no_mutations:
        for name,old,new,label in mutations:
            assert owners.count(old)==(2 if name in ['skip-primary-step','captured-resource'] else 1),(name,owners.count(old))
            text=owners.replace(old,new)
            if name=='captured-primary-flag':text=text.replace('request = *initial;','request = *initial;\n    unsigned capturedFlags = request.flags;')
            if name=='captured-resource':text=text.replace('request = *initial;','request = *initial;\n    AnmLoadedView *capturedResource = reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->effectResource;')
            variants.append((name,text,label))
    for name,text,expected in variants:
        (out/'owner.inc').write_text(text)
        cmd=['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Wno-class-memaccess','-I',str(out),str(ROOT/'scripts/fixtures/laser-init.cpp'),'-o',str(out/'fixture')]
        if args.sanitize:cmd+=['-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']
        compiled=subprocess.run(cmd,capture_output=True,text=True)
        if compiled.returncode:raise RuntimeError(name+'\n'+compiled.stdout+compiled.stderr)
        result=subprocess.run([str(out/'fixture')],capture_output=True,text=True,env={**os.environ,'ASAN_OPTIONS':'detect_leaks=1'})
        want=(0,'','')if expected is None else(1,'','CHECK '+expected+'\n')
        if (result.returncode,result.stdout,result.stderr)!=want:raise RuntimeError(f'{name}: {result.returncode}\n{result.stdout}{result.stderr}')
        print('PASS '+name,flush=True)
    (out/'manifest.json').write_text(json.dumps({'scope':__doc__,'inputs':hashes,'sanitize':args.sanitize,'variants':[x[0]for x in variants]},indent=2)+'\n')


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--sanitize',action='store_true');p.add_argument('--no-mutations',action='store_true');p.add_argument('--out',type=Path);a=p.parse_args()
    if a.out:run(a,a.out)
    else:
        parent=ROOT/'build/host-tests';parent.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='laser-init-',dir=parent)as d:run(a,Path(d))


if __name__=='__main__':main()
