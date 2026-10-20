#!/usr/bin/env python3
"""Exercise maintained laser/Player bodies and actual timer, bounds, sound/math helpers.

Host adaptations omit i386 layout assertions, prefill constructor flags and the
two native polar scratch Z values, and supply typed live outer objects. Polar
math, unrecovered virtuals, VM creation/execution, rendering and PlayerHandleCollisionHit
are declared roles. No native ABI, Z-initialization, x87 or product-runtime claim.
"""
import argparse,hashlib,json,os,re,runpy,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
body=runpy.run_path(str(ROOT/'scripts/test-frontend-replay-save.py'))['body']


def run(args,out):
    out.mkdir(parents=True,exist_ok=True);hashes={};pending=['EnemyLaserLine.hpp','EnemyPrimaryResources.hpp','EnemySoundQueue.hpp','Gui.hpp'];seen=set()
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
    owners=read('LaserLine.cpp');owners=re.sub(r'^#include[^\n]*\n','',owners,flags=re.M)
    owners=owners.replace('extern unsigned char g_MainSoundOwner[0x52d0];','// HOST fixed typed queue fixture exposed through pointer role.\nextern unsigned char *g_MainSoundOwner;')
    for name in ['head','point']:
        old=f'EnemyLaserVectorView {name};';assert owners.count(old)==1
        owners=owners.replace(old,f'EnemyLaserVectorView {name} = {{0, 0, 17}}; // HOST unknown native Z prefill.')
    anm=read('AnmManager.cpp');laser=read('EnemyLaser.cpp');ecl=read('EnemyEclDispatcher.cpp')
    helpers='\n'.join([body(anm,'void AnmVmTimerView::Add('),body(anm,'float __stdcall AddNormalizeAngle('),body(laser,'int EnemyLaserVectorView::IsOutsidePlayfield('),body(ecl,'void EnemySoundQueueView::QueueSoundCue(')])
    (out/'helpers.inc').write_text(helpers)
    variants=[('positive',owners,None)]
    mutations=[
        ('skip-commands','    RunCommands();','    // omitted','commands-first'),
        ('stale-flags','if ((flags & 0x10u) != 0) Motion0010();','if (true) Motion0010();','live-motion-flags'),
        ('swap-motion-order','if ((flags & 0x100u) != 0) Motion0100();\n        if ((flags & 0x80u) != 0) Motion0080();','if ((flags & 0x80u) != 0) Motion0080();\n        if ((flags & 0x100u) != 0) Motion0100();','motion-slot-order'),
        ('flash-boundary','patterns[5].timer.current <= 0','patterns[5].timer.current < 0','flash-boundary'),
        ('omit-growth-speed','length += g_AnmGameSpeed * growSpeed;','length += growSpeed;','growth-speed'),
        ('omit-cap','length = request.maximumLength;','length = length;','growth-cap'),
        ('terminal-equality','if (length <= 0.0f)','if (length < 0.0f)','terminal-zero'),
        ('bounds-or','position.IsOutsidePlayfield(width, width) &&','position.IsOutsidePlayfield(width, width) ||','both-bounds'),
        ('no-hold-add','holdTimer.Add(-1.0f);','holdTimer.Add(0.0f);','hold-speed'),
        ('collision-gate','length > 16.0f && width > 3.0f','length >= 16.0f && width > 3.0f','collision-length-gate'),
        ('graze-period','age.current % 5 == 0','age.current % 4 == 0','signed-graze-period'),
        ('wrong-sprite-axis','width / sprite->width','width / sprite->height','scale-axes'),
        ('omit-scale-bit','flags |= 8u;','flags |= 0u;','scale-flags'),
        ('tail-inclusive','if (traveled == 0.0f)','if (traveled >= 0.0f)','strict-tail-zero'),
        ('wrong-draw-field','primaryVm.position.x =','primaryVm.positionOffset.x =','draw-position-field'),
        ('wrong-draw-offset','position.y + 16.0f','position.y + 15.0f','draw-offsets'),
        ('motion-duration','motion.timer.current >= motion.duration','motion.timer.current > motion.duration','motion-duration'),
        ('omit-motion-tick','motion.timer.Tick();','// omitted','motion-tick'),
        ('graze-upper-y','!(width * -1.5f > lower.y)','!(width * -1.5f > upper.y)','graze-lower-y'),
        ('ordered-hit','!(lower.x > length)','(lower.x <= length)','unordered-contact'),
        ('highlight-equality','highlightTimer.current > 0','highlightTimer.current >= 0','highlight-gate'),
    ]
    if not args.no_mutations:
        for name,old,new,label in mutations:
            count=owners.count(old);assert count==(2 if name in ['tail-inclusive','ordered-hit','wrong-draw-offset'] else 1),(name,count)
            variants.append((name,owners.replace(old,new),label))
    for name,text,expected in variants:
        (out/'owner.inc').write_text(text)
        cmd=['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Wno-class-memaccess','-I',str(out),str(ROOT/'scripts/fixtures/laser-line.cpp'),'-o',str(out/'fixture')]
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
        with tempfile.TemporaryDirectory(prefix='laser-line-',dir=parent)as d:run(a,Path(d))


if __name__=='__main__':main()
