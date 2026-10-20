#!/usr/bin/env python3
"""Execute all three actual TH10 input polling bodies with scripted OS/COM roles.

Host ABI macros and pointer-layout assertion omission are explicit adaptations;
the6A input layout assertions remain active. Failed raw GetDeviceState fills its
buffer in this fixture; unwritten failure data and invalid indexes/WinMM shifts
are outside its defined domain. No native runtime/lifetime/ABI claim follows.
"""
import argparse,hashlib,json,os,re,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]


def run(args,out):
    out.mkdir(parents=True,exist_ok=True);hashes={}
    for name in ['InputPolling.hpp','InputState.hpp']:
        p=ROOT/'src'/name;text=p.read_text();hashes['src/'+name]=hashlib.sha256(p.read_bytes()).hexdigest()
        if name=='InputPolling.hpp':text=re.sub(r'typedef char InputPollingObservedLayouts\[[\s\S]*?\];','// HOST pointer-size ABI assertions omitted.',text)
        (out/name).write_text(text)
    p=ROOT/'src/InputPolling.cpp';hashes['src/InputPolling.cpp']=hashlib.sha256(p.read_bytes()).hexdigest();owner=re.sub(r'^#include[^\n]*\n','',p.read_text(),flags=re.M)
    variants=[('positive',owner,None)]
    query='''        g_InputPadDevices[padIndex]->vtable->GetDeviceState(
            g_InputPadDevices[padIndex], sizeof(state), &state);
        memcpy'''
    mutations=[
        ('record-stride','(&g_FrontEndInput) + bindingIndex','(&g_FrontEndInput) + 0','record-stride',2),
        ('keyboard-enabled','if (g_InputEnabled != 0)','if (true)','keyboard-enabled',1),
        ('keyboard-bool','GetKeyboardState(state);','if (!GetKeyboardState(state)) memset(state, 0, sizeof(state));','keyboard-ignored-bool',1),
        ('keyboard-low-bit','state[0x5a] & 0x80','state[0x5a] & 1','keyboard-bits',1),
        ('keyboard-z','state[0x5a] & 0x80','state[0x59] & 0x80','keyboard-bits',1),
        ('keyboard-di-z','state[0x2c] & 0x80','state[0x2b] & 0x80','keyboard-bits',1),
        ('keyboard-diagonal','(state[0x63] & 0x80) ? 0xa0','(state[0x63] & 0x80) ? 0x80','keyboard-bits',1),
        ('keyboard-positive-result','else if (result != 0)','else if (result < 0)','keyboard-acquire',1),
        ('keyboard-acquire','g_InputKeyboardDevice->vtable->Acquire(g_InputKeyboardDevice);','0;','keyboard-acquire',2),
        ('disabled-joystick','MergeJoystickInput(bindingIndex, 0, mask);','g_InputEnabled ? MergeJoystickInput(bindingIndex, 0, mask) : 0;','joystick-when-disabled',1),
        ('winmm-index','joyGetPosEx(padIndex != 0, &info)','joyGetPosEx(padIndex, &info)','winmm-index',1),
        ('query-index','joyGetPosEx(0, &info)','joyGetPosEx(padIndex, &info)','button-query-index',1),
        ('winmm-zero','memset(&info, 0, sizeof(info));','memset(&info, 0x55, sizeof(info));','winmm-zero',2),
        ('winmm-shift','1u << button','1u << (button & 15)','winmm-shift',1),
        ('winmm-negative','if (button < 0)\n        return 0;','if (button < -1)\n        return 0;','unbound-buttons',2),
        ('winmm-x-equality','info.x > ((','info.x >= ((','winmm-axes',1),
        ('winmm-caps-index','g_InputJoystickCaps[padIndex]','g_InputJoystickCaps[0]','caps-original-index',12),
        ('query-high-bits','while (bit < 32)','while (bit < 31)','button-query-bits',1),
        ('merge-positive-result','g_InputPadDevices[padIndex], sizeof(state), &state) < 0)','g_InputPadDevices[padIndex], sizeof(state), &state) != 0)','merge-positive-result',1),
        ('merge-zero','memset(&state, 0, sizeof(state));','memset(&state, 0x55, sizeof(state));','merge-state-zero',1),
        ('merge-negative-result','g_InputPadDevices[padIndex], sizeof(state), &state) < 0)','g_InputPadDevices[padIndex], sizeof(state), &state) < -1)','merge-failed-result',1),
        ('query-result',query,query.replace('        g_InputPadDevices','        if (g_InputPadDevices',1).replace('&state);','&state) < 0) return g_InputJoystickButtons;'),'query-unchecked-result',1),
        ('direct-x-equality','state.x < -g_InputJoystickXThreshold','state.x <= -g_InputJoystickXThreshold','direct-axes',1),
        ('acquire-limit','++retries >= 400','++retries >= 399','acquire-limit',2),
        ('late-publication','    binding->previous = binding->current;','    // moved early','late-publication',1),
        ('repeat-threshold','++*hold >= 26','++*hold >= 25','repeat-mask',1),
        ('repeat-period','*hold -= 8;','*hold -= 7;','repeat-counters',1),
        ('repeat-mask','binding->repeated |= repeat;','binding->repeated |= 1;','repeat-mask',1),
        ('repeat-clear','binding->repeated = 0;','// preserved old repeat','repeat-mask',1),
        ('repeat-release','*hold = 0;','*hold += 0;','repeat-counters',1),
        ('frame-release','changed & ~binding->current','changed & binding->current','frame-edges',1),
        ('helper-return','return (buttons & mask) ? action : 0;','return 0;','helper-return',1),
    ]
    if not args.no_mutations:
        for name,old,new,label,count in mutations:
            assert owner.count(old)==count,(name,owner.count(old));text=owner.replace(old,new,1 if name in ['keyboard-acquire','winmm-negative'] else count)
            if name=='winmm-negative':text=text.replace('1u << button','1u << (button < 0 ? 0 : button)')
            if name=='late-publication':text=text.replace('    unsigned short bits = MergeJoystickInput','    binding->previous = binding->current;\n    unsigned short bits = MergeJoystickInput')
            variants.append((name,text,label))
    if args.variant:variants=[x for x in variants if x[0]==args.variant];assert variants,'unknown variant'
    for name,text,label in variants:
        (out/'owner.inc').write_text(text)
        cmd=['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-I',str(out),str(ROOT/'scripts/fixtures/input-polling.cpp'),'-o',str(out/'fixture')]
        if args.sanitize:cmd+=['-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']
        c=subprocess.run(cmd,capture_output=True,text=True)
        if c.returncode:raise RuntimeError(name+'\n'+c.stdout+c.stderr)
        r=subprocess.run([str(out/'fixture')],capture_output=True,text=True,env={**os.environ,'ASAN_OPTIONS':'detect_leaks=1'})
        want=(0,'','')if label is None else(1,'','CHECK '+label+'\n')
        if (r.returncode,r.stdout,r.stderr)!=want:raise RuntimeError(f'{name}: {r.returncode}\n{r.stdout}{r.stderr}')
        print('PASS '+name,flush=True)
    for relative in ['scripts/test-input-polling.py','scripts/fixtures/input-polling.cpp']:
        hashes[relative]=hashlib.sha256((ROOT/relative).read_bytes()).hexdigest()
    (out/'manifest.json').write_text(json.dumps({'scope':__doc__,'inputs':hashes,'sanitize':args.sanitize,'complete_mutation_suite':not args.no_mutations and not args.variant,'variants':[x[0]for x in variants]},indent=2)+'\n')


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--sanitize',action='store_true');p.add_argument('--no-mutations',action='store_true');p.add_argument('--variant');p.add_argument('--out',type=Path);a=p.parse_args()
    if a.out:run(a,a.out)
    else:
        parent=ROOT/'build/host-tests';parent.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='input-polling-',dir=parent)as d:run(a,Path(d))


if __name__=='__main__':main()
