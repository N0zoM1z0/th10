#!/usr/bin/env python3
"""Execute maintained spell-panel bodies and real sprite/ID/position helpers.

Canonical panel/ANM headers use host layout/prefill adaptations. VM execution
and outer Player/GUI/Enemy/stage globals are explicit host roles. Sprite helper
entry observation is pure: it cannot replace globals or invoke fake callbacks.
No target/native ABI, x87 or product/runtime claim.
"""
import argparse,hashlib,json,os,re,runpy,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
body=runpy.run_path(str(ROOT/'scripts/test-frontend-replay-save.py'))['body']


def run(args,out):
    out.mkdir(parents=True,exist_ok=True);hashes={};pending=['EnemyGameState.hpp'];seen=set()
    while pending:
        name=pending.pop()
        if name in seen:continue
        seen.add(name);p=ROOT/'src'/name;text=p.read_text();hashes['src/'+name]=hashlib.sha256(p.read_bytes()).hexdigest()
        pending.extend(re.findall(r'^\s*#\s*include\s*"([^"]+)"',text,re.M))
        text=re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;','// HOST native assertion omitted.',text)
        text=text.replace('AnmVmTimerView() { flags &= ~1u; }','AnmVmTimerView() : flags(0) {}')
        (out/name).write_text(text)
    p=ROOT/'src/SpellPanel.cpp';text=p.read_text();hashes['src/SpellPanel.cpp']=hashlib.sha256(p.read_bytes()).hexdigest()
    owners='\n'.join(body(text,sig) for sig in ['int EnemyGameStateView::UpdateSpellPanel()', 'int __fastcall EnemySpellPanelUpdateCallback('])
    p=ROOT/'src/AnmManager.cpp';anm=p.read_text();hashes['src/AnmManager.cpp']=hashlib.sha256(p.read_bytes()).hexdigest()
    helpers=body(anm,'void AnmMatrixView::SetIdentity()')+'\n'+body(anm,'AnmVmView *AnmRenderManagerView::FindVm(')+'\n'+body(anm,'void __stdcall AnmRenderManagerView::SetVmWorldPosition(')
    sprite=body(anm,'int AnmLoadedView::SetSprite(');opening=sprite.index('{')
    sprite=sprite[:opening+1]+'\n    HostObserveSprite(this, vm, spriteIndex); // HOST pure entry observation.\n'+sprite[opening+1:]
    helpers+='\n'+sprite+'\n'+body(anm,'void AnmLoadedView::SetAndExecuteScriptIndex(')
    p=ROOT/'src/AnmVmId.cpp';ids=p.read_text();hashes['src/AnmVmId.cpp']=hashlib.sha256(p.read_bytes()).hexdigest()
    helpers+='\n'+body(ids,'void AnmVmIdView::SetInterrupt(')
    (out/'helpers.inc').write_text(helpers)
    variants=[('positive',owners,None)]
    mutations=[
        ('early-stage','spellTimer.current >= 60','spellTimer.current >= 59','stage-pre-timer'),
        ('wrong-bonus-slot','initialSpellBonus - initialSpellBonus / 10','currentSpellBonus - currentSpellBonus / 10','bonus-source-slots'),
        ('early-decay','spellTimer.current >= 300','spellTimer.current >= 299','decay-pre-timer'),
        ('ignore-freeze','(flags378C & 8u) == 0','true','bonus-freeze'),
        ('late-panel','spellTimer.current >= 120','spellTimer.current > 120','panel-post-timer'),
        ('inclusive-hide','drawPosition.y < 96.0f','drawPosition.y <= 96.0f','panel-hide-boundary'),
        ('inclusive-show','drawPosition.y > 128.0f','drawPosition.y >= 128.0f','panel-show-boundary'),
        ('short-hide-range','i < 8; ++i)\n                    frontVms[i].pendingInterrupt = 3','i < 7; ++i)\n                    frontVms[i].pendingInterrupt = 3','hide-vm-ranges'),
        ('script-instead-sprite','->SetSprite(','->SetAndExecuteScriptIndex(','sprite-binding'),
        ('stale-bonus-value','value %= divisor;','value = currentSpellBonus % divisor;','bonus-captured-value'),
        ('early-gui-capture','GuiView *gui = g_GuiView;','GuiView *gui = originalGui;','counter-gui-after-bonus'),
        ('wrong-marker','specialEnemySlots[0]','specialEnemySlots[1]','marker-interpolation'),
        ('callback-return','return state->UpdateSpellPanel();','state->UpdateSpellPanel();\n    return 0;','callback-return'),
    ]
    if not args.no_mutations:
        for name,old,new,label in mutations:
            count=owners.count(old);assert count==(6 if name=='script-instead-sprite' else 1),(name,count)
            variant=owners.replace(old,new)
            if name=='early-gui-capture':
                needle='    if ((flags378C & 1u) == 0)';assert variant.count(needle)==1
                variant=variant.replace(needle,'    GuiView *originalGui = g_GuiView;\n'+needle)
            variants.append((name,variant,label))
    for name,text,expected in variants:
        (out/'owner.inc').write_text(text)
        cmd=['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Wno-class-memaccess','-I',str(out),str(ROOT/'scripts/fixtures/spell-panel.cpp'),'-o',str(out/'fixture')]
        if args.sanitize:cmd+=['-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']
        compiled=subprocess.run(cmd,capture_output=True,text=True)
        if compiled.returncode:raise RuntimeError(name+'\n'+compiled.stdout+compiled.stderr)
        result=subprocess.run([str(out/'fixture')],capture_output=True,text=True,env={**os.environ,'ASAN_OPTIONS':'detect_leaks=1'})
        want=(0,'','')if expected is None else(1,'','CHECK '+expected+'\n')
        if (result.returncode,result.stdout,result.stderr)!=want:raise RuntimeError(f'{name}: {result.returncode}\n{result.stdout}{result.stderr}')
        print('PASS '+name,flush=True)
    (out/'manifest.json').write_text(json.dumps({'scope':__doc__,'inputs':hashes,'sanitize':args.sanitize,'variants':[x[0]for x in variants]},indent=2)+'\n')


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--sanitize',action='store_true');parser.add_argument('--no-mutations',action='store_true');parser.add_argument('--out',type=Path);args=parser.parse_args()
    if args.out:run(args,args.out)
    else:
        parent=ROOT/'build/host-tests';parent.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='spell-panel-',dir=parent)as d:run(args,Path(d))


if __name__=='__main__':main()
