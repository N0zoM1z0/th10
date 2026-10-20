#!/usr/bin/env python3
"""Execute maintained laser commands with real timers, aim, VM wrapper and sound.

Canonical header assertions/constructor prefills use explicit host adaptations.
Polar math, VM reset/script execution and outer object construction are roles;
pure aim/timer/sound helpers receive no synthetic callbacks. Valid finite command
programs only; no native ABI/layout, x87, invalid-PC or product-runtime claim.
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
    source=read('LaserScript.cpp');owner=body(source,'void EnemyLaserLineView::RunCommands()')
    anm=read('AnmManager.cpp');enemy=read('Enemy.cpp');sound=read('EnemyEclDispatcher.cpp')
    helpers='\n'.join([body(anm,'void AnmVmView::InitializeForLoadedScript('),body(enemy,'float EnemyAngleFromPlayer('),body(sound,'void EnemySoundQueueView::QueueSoundSample('),body(sound,'void EnemySoundQueueView::QueueSoundCue(')])
    (out/'helpers.inc').write_text(helpers)
    variants=[('positive',owner,None)]
    mutations=[
        ('early-limit','commandIndex < 18','commandIndex < 17','eighteen-record-limit'),
        ('zero-advances','if (opcode == 0)\n            return;','if (opcode == 0)\n        { ++commandIndex; return; }','zero-opcode'),
        ('ignore-gate','command.allowWhileActive == 0 && flags != 0','false','active-gate'),
        ('skip-only-once','++commandIndex;\n            continue;','++commandIndex;\n            return;','same-frame-skip'),
        ('clear-old-flags','flags |= 1u;','flags = 1u;','preserve-flags'),
        ('keep-angle-strict','command.arguments[1].scalar <= -990.0f','command.arguments[1].scalar < -990.0f','angle-negative-sentinel'),
        ('aim-angle-strict','command.arguments[1].scalar >= 990.0f','command.arguments[1].scalar > 990.0f','angle-positive-sentinel'),
        ('zero-first-sound','commandIndex != 0 && soundId >= 0','soundId >= 0','first-command-sound'),
        ('wrong-polar-duration','patterns[2].values[5].integer = command.arguments[2].integer;','patterns[2].values[5].integer = command.arguments[3].integer;','polar-fields'),
        ('width-inclusive','command.arguments[1].scalar > -999.0f','command.arguments[1].scalar >= -999.0f','width-sentinel'),
        ('wrong-speed-fallback',': growSpeed;',': width;','width-sentinel'),
        ('numeric-mode','patterns[3].values[1].integer = command.arguments[0].integer;','patterns[3].values[1].integer = int(command.arguments[0].scalar);','raw-mode-word'),
        ('wrong-script-offset','command.arguments[3].integer;\n                primaryVm','0;\n                primaryVm','embedded-script'),
        ('cached-pc','        ++commandIndex;\n    }','        commandIndex = capturedPc + 1;\n    }','live-pc-after-script'),
        ('wrong-wait-record','patterns[6].timer.SetCurrent','patterns[7].timer.SetCurrent','shared-wait-record'),
        ('clamp-wait','patterns[5].timer.SetCurrent(command.arguments[2].integer);','patterns[5].timer.SetCurrent(0);','wait-integer'),
        ('wrong-state','unknown00C = 3;','unknown00C = 2;','state-word'),
        ('wrong-pan','QueueSoundCue(command.arguments[2].integer, position.x);','QueueSoundCue(command.arguments[2].integer, position.y);','panned-sound'),
        ('jump-increments','commandIndex = command.arguments[2].integer;','commandIndex = command.arguments[2].integer + 1;','jump-target'),
        ('consume-zero-count','command.arguments[2].integer > 0','command.arguments[2].integer >= 0','count-zero'),
        ('counter-copy','EnemyLaserCommandView &command','EnemyLaserCommandView command','mutable-command-count'),
        ('unordered-fallback','command.arguments[0].scalar >= 0.0f','!(command.arguments[0].scalar < 0.0f)','unordered-growth'),
        ('wrong-move-record','patterns[8].values[5].integer = command.arguments[2].integer;','patterns[8].values[5].integer = command.arguments[3].integer;','point-motion-fields'),
    ]
    if not args.no_mutations:
        for name,old,new,label in mutations:
            expected=2 if name in ['skip-only-once','zero-first-sound','wrong-wait-record']else 1
            assert owner.count(old)==expected,(name,owner.count(old))
            text=owner.replace(old,new)
            if name=='cached-pc':text=text.replace('        switch (opcode)','        int capturedPc = commandIndex;\n        switch (opcode)')
            variants.append((name,text,label))
    for name,text,expected in variants:
        (out/'owner.inc').write_text(text)
        cmd=['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Wno-class-memaccess','-I',str(out),str(ROOT/'scripts/fixtures/laser-script.cpp'),'-o',str(out/'fixture')]
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
        with tempfile.TemporaryDirectory(prefix='laser-script-',dir=parent)as d:run(a,Path(d))


if __name__=='__main__':main()
