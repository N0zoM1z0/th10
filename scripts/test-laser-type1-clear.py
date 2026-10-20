#!/usr/bin/env python3
"""Exercise actual type1 laser Clear, ANM creation/allocation/list and Item Spawn bodies.

Host layout/constructor prefills, polar math and script execution are explicit
roles. Script execution alone may mutate live state. Kind8 uses actual Item
Spawn code and has no injected callback. No native ABI/FP/lifetime/runtime claim.
"""
import argparse,hashlib,json,os,re,runpy,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
body=runpy.run_path(str(ROOT/'scripts/test-frontend-replay-save.py'))['body']


def run(args,out):
    out.mkdir(parents=True,exist_ok=True);hashes={};pending=['EnemyLaserType1.hpp','ItemManager.hpp','EnemyPrimaryResources.hpp'];seen=set()
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
    owner=read('LaserType1Clear.cpp');owner=re.sub(r'^#include[^\n]*\n','',owner,flags=re.M)
    anm=read('AnmManager.cpp');item=read('ItemManager.cpp')
    # World creator also closes the unexercised non-kind8 branches of full Spawn.
    helpers='\n'.join([body(anm,'AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant0('),body(anm,'AnmVmView *AnmRenderManagerView::AllocateVm()'),body(anm,'AnmVmIdView AnmRenderManagerView::AddVmVariant0('),body(anm,'void AnmVmLayerNodeView::InsertAfter('),body(item,'int ItemManagerView::Spawn(')])
    (out/'helpers.inc').write_text(helpers)
    variants=[('positive',owner,None)]
    mutations=[
        ('wrong-start','step.FromAngleMagnitude(angle, 6.0f);','step.FromAngleMagnitude(angle, 5.0f);','first-sample'),
        ('wrong-z','step.y + position.y, position.z','step.y + position.y, 0.0f','sample-z'),
        ('wrong-stride','step.x += step.x;','step.x += 0.0f;','sample-stride'),
        ('wrong-y-stride','step.y += step.y;','step.y += 0.0f;','angled-stride'),
        ('inclusive-initial','if (12.0f < length)','if (12.0f <= length)','initial-boundary'),
        ('unordered-initial','if (12.0f < length)','if (!(12.0f >= length))','initial-unordered'),
        ('inclusive-tail','distance + 6.0f < length','distance + 6.0f <= length','tail-boundary'),
        ('unordered-tail','distance + 6.0f < length','!(distance + 6.0f >= length)','live-unordered-length'),
        ('omit-z-addition','point.z += step.z;','// omitted','zero-z-addition'),
        ('wrong-progress','distance += 12.0f;','distance += 6.0f;','sample-count'),
        ('signed-color','request.color * 2 + 0x11','static_cast<unsigned short>(request.color) * 2 + 0x11','signed-live-script'),
        ('wrong-resource','g_EnemyLaserManager)->effectResource','g_EnemyLaserManager)->primaryEnemyResource','live-resource'),
        ('screen-creator','point.x + 224.0f','point.x','first-sample'),
        ('ignore-mode','spawnItems != 0 &&','true &&','mode-gate'),
        ('outer-too-small','point.x + 16.0f','point.x + 15.0f','outer-margin16'),
        ('inner-too-small','point.x + 32.0f','point.x + 0.0f','inner-margin32'),
        ('outer-x-unbounded','!(point.x - 16.0f >= 192.0f)','true','outer-reject'),
        ('outer-unordered','!(point.x + 16.0f <= -192.0f)','(point.x + 16.0f > -192.0f)','unordered-bounds'),
        ('count-only-emitted','++count;','// omitted','count-offscreen'),
        ('wrong-kind','8, 0xffffffffu','7, 0xffffffffu','item-kind'),
        ('wrong-angle','-1.5707964f, 0.6f','0.0f, 0.6f','item-velocity'),
        ('wrong-speed','-1.5707964f, 0.6f','-1.5707964f, 0.5f','item-velocity'),
        ('stale-item','g_ItemManager->Spawn(','capturedItemManager->Spawn(','live-item-manager'),
        ('stale-length','distance + 6.0f < length','distance + 6.0f < capturedLength','live-unordered-length'),
        ('stale-script','request.color * 2 + 0x11','capturedColor * 2 + 0x11','signed-live-script'),
        ('unordered-bounds','!(point.x + 32.0f <= -192.0f)','(point.x + 32.0f > -192.0f)','unordered-bounds'),
        ('wrong-retirement','state = 1;','state = 2;','retired-at-end'),
        ('early-retirement','    state = 1;','    // omitted','retire-after-scripts'),
        ('wrong-return','return count;','return count + 1;','sample-count'),
    ]
    if not args.no_mutations:
        for name,old,new,label in mutations:
            assert owner.count(old)==1,(name,owner.count(old));text=owner.replace(old,new)
            if name=='count-only-emitted':text=text.replace('                    int scriptIndex =','                    ++count;\n                    int scriptIndex =')
            if name=='wrong-resource':text=text.replace('reinterpret_cast<EnemyLaserManagerStorageView *>(\n                            g_EnemyLaserManager)->primaryEnemyResource','g_EnemyPrimaryResourceOwner->primaryEnemyResource')
            if name=='early-retirement':text=text.replace('distance = 6.0f;','state = 1;\n    distance = 6.0f;')
            if name=='stale-item':text=text.replace('distance = 6.0f;','ItemManagerView *capturedItemManager = g_ItemManager;\n    distance = 6.0f;')
            if name in ['stale-length','stale-script']:
                decl='float capturedLength = length;'if name=='stale-length'else'int capturedColor = request.color;'
                text=text.replace('distance = 6.0f;',decl+'\n    distance = 6.0f;')
            variants.append((name,text,label))
    if args.variant:variants=[v for v in variants if v[0]==args.variant];assert variants,'unknown variant'
    for name,text,expected in variants:
        (out/'owner.inc').write_text(text)
        cmd=['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Wno-class-memaccess','-I',str(out),str(ROOT/'scripts/fixtures/laser-type1-clear.cpp'),'-o',str(out/'fixture')]
        if args.sanitize:cmd+=['-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']
        compiled=subprocess.run(cmd,capture_output=True,text=True)
        if compiled.returncode:raise RuntimeError(name+'\n'+compiled.stdout+compiled.stderr)
        result=subprocess.run([str(out/'fixture')],capture_output=True,text=True,env={**os.environ,'ASAN_OPTIONS':'detect_leaks=1'})
        want=(0,'','')if expected is None else(1,'','CHECK '+expected+'\n')
        if (result.returncode,result.stdout,result.stderr)!=want:raise RuntimeError(f'{name}: {result.returncode}\n{result.stdout}{result.stderr}')
        print('PASS '+name,flush=True)
    for relative in ['scripts/test-laser-type1-clear.py','scripts/fixtures/laser-type1-clear.cpp','scripts/test-frontend-replay-save.py']:
        hashes[relative]=hashlib.sha256((ROOT/relative).read_bytes()).hexdigest()
    (out/'manifest.json').write_text(json.dumps({'scope':__doc__,'inputs':hashes,'sanitize':args.sanitize,'complete_mutation_suite':not args.no_mutations and not args.variant,'variants':[x[0]for x in variants]},indent=2)+'\n')


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--sanitize',action='store_true');p.add_argument('--no-mutations',action='store_true');p.add_argument('--variant');p.add_argument('--out',type=Path);a=p.parse_args()
    if a.out:run(a,a.out)
    else:
        parent=ROOT/'build/host-tests';parent.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='laser-type1-clear-',dir=parent)as d:run(a,Path(d))


if __name__=='__main__':main()
