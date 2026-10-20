#!/usr/bin/env python3
"""Actual alpha evaluator host regression; bounded timer/linear-curve roles only.

Does not execute TH10, prove the private ABI, validate the real timer/curve, or
establish the original x87 control state. Native exact replay is separate.
"""
from pathlib import Path
import hashlib
import json
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]

def block(text, marker):
    if text.count(marker) != 1:
        raise RuntimeError('nonunique extraction: ' + marker)
    start = text.index(marker)
    left = text.index('{', start)
    depth = 0
    for i in range(left, len(text)):
        depth += (text[i] == '{') - (text[i] == '}')
        if depth == 0:
            return text[start:i+1]
    raise RuntimeError('unbalanced extraction: ' + marker)

header = (ROOT / 'src/AnmManager.hpp').read_text()
source = (ROOT / 'src/AnmManager.cpp').read_text()
body = block(source, 'int AnmVmAlphaInterpolationView::Evaluate()')
enum = block(header, 'enum AnmInterpolationModeView\n') + ';\n'
owner = block(header, 'struct AnmVmAlphaInterpolationView\n') + ';\n'
preamble = '''#include <cstdio>
#include <cstdlib>
#define CHECK(name, expr) do { if (!(expr)) { std::fprintf(stderr,"CHECK:%s\\n",name); std::exit(17); } } while (0)
// Explicit bounded roles: one tick advances one frame; only linear easing.
struct AnmVmTimerView {
 int previous, current; float subframe; int ticks, sets;
 void Tick() { ++ticks; previous=current; ++current; subframe+=1.0f; }
 void SetCurrent(int n) { ++sets; previous=n-1; current=n; subframe=float(n); }
};
float CalculateAnmInterpolation(int mode, float n, float d) {
 CHECK("linear-role", mode==0 && d>0); return n/d;
}
''' + enum + owner
main = '''
AnmVmAlphaInterpolationView make(int initial,int final,int current,int duration,int mode) {
 AnmVmAlphaInterpolationView v={}; v.initial=initial; v.final=final;
 v.timer.current=current; v.timer.subframe=float(current); v.duration=duration; v.mode=mode; return v;
}
int main() {
 struct Fade { int initial,final,current,duration,expected; } fades[]={
  {255,0,4,10,127}, {255,0,0,10,229}, {17,4,4,10,10},
  {0,255,4,10,127}, {4,17,4,10,10}, {-17,-4,4,10,-10}};
 for (const Fade &f:fades) {
  auto v=make(f.initial,f.final,f.current,f.duration,0);
  CHECK("affine-rounding",v.Evaluate()==f.expected);
  CHECK("timer-advance",v.timer.ticks==1 && v.timer.current==f.current+1 && v.duration==f.duration && v.timer.sets==0);
 }
 auto done=make(255,13,9,10,0);
 CHECK("completion",done.Evaluate()==13 && done.duration==0 && done.timer.current==10 && done.timer.sets==1);
 auto addDone=make(31,8,9,10,ANM_INTERPOLATION_ADD);
 CHECK("add-completion",addDone.Evaluate()==31 && addDone.initial==31 && addDone.duration==0);
 auto add=make(31,8,0,0,ANM_INTERPOLATION_ADD);
 CHECK("add-step",add.Evaluate()==39 && add.initial==39 && add.timer.ticks==0);
 auto accel=make(31,8,0,0,ANM_INTERPOLATION_ACCELERATE); accel.finalTangent=3;
 CHECK("acceleration",accel.Evaluate()==34 && accel.initial==34 && accel.finalTangent==11);
 auto h=make(1,1,4,10,ANM_INTERPOLATION_HERMITE);
 CHECK("hermite-sum",h.Evaluate()==1);
 auto ht=make(0,0,4,10,ANM_INTERPOLATION_HERMITE); ht.initialTangent=16; ht.finalTangent=8;
 CHECK("hermite-tangents",ht.Evaluate()==1);
 std::puts("12 alpha interpolation scenarios passed");
}
'''

def require(condition, message):
    if not condition:
        raise RuntimeError(message)

mutations = {
 'old-affine-truncation': ('return static_cast<int>((final - initialValue) * value + initialValue);',
                         'return static_cast<int>((final - initialValue) * value) + initialValue;', 'affine-rounding'),
 'omit-tick': ('timer.Tick();', '', 'affine-rounding'),
 'late-completion': ('timer.current >= duration', 'timer.current > duration', 'completion'),
 'wrong-completion': ('return mode == ANM_INTERPOLATION_ADD ? initial : final;', 'return initial;', 'completion'),
 'add-at-completion': ('return mode == ANM_INTERPOLATION_ADD ? initial : final;', 'return final;', 'add-completion'),
 'acceleration-order': ('initial += finalTangent;\n        finalTangent += final;',
                        'finalTangent += final;\n        initial += finalTangent;', 'acceleration'),
 'truncate-hermite-terms': ('float result = finalWeight * final;\n        result += finalTangentWeight * finalTangent;\n        result += initialTangentWeight * initialTangent;\n        result += initialWeight * initial;',
                            'float result = int(finalWeight * final);\n        result += int(finalTangentWeight * finalTangent);\n        result += int(initialTangentWeight * initialTangent);\n        result += int(initialWeight * initial);', 'hermite-sum'),
 'reverse-final-tangent': ('minusOne * value * value', '-minusOne * value * value', 'hermite-tangents'),
}
build = ROOT / 'build/host-tests'
build.mkdir(parents=True, exist_ok=True)
with tempfile.TemporaryDirectory(prefix='anm-alpha-', dir=build) as tmp:
    directory = Path(tmp)
    def run(candidate, flags):
        cpp=directory/'test.cpp'; exe=directory/'test'
        cpp.write_text(preamble+candidate+main)
        r=subprocess.run(['g++','-std=c++11',*flags,str(cpp),'-o',str(exe)],capture_output=True,timeout=60)
        require(r.returncode==0,'host compile failed: '+r.stderr.decode())
        env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
        return subprocess.run([str(exe)],capture_output=True,timeout=20,env=env)
    def positive(r):
        require((r.returncode,r.stdout,r.stderr)==(0,b'12 alpha interpolation scenarios passed\n',b''),repr((r.returncode,r.stdout,r.stderr)))
    positive(run(body,['-O2']))
    for name,(old,new,check) in mutations.items():
        require(body.count(old)==1,'nonunique mutation: '+name)
        r=run(body.replace(old,new),['-O2'])
        require((r.returncode,r.stdout,r.stderr)==(17,b'',('CHECK:'+check+'\n').encode()),name+': '+repr((r.returncode,r.stdout,r.stderr)))
    positive(run(body,['-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie']))
print(json.dumps({'scope':'host source logic with bounded roles; not TH10/native runtime','scenarios':12,'strict_mutants':len(mutations),'ASan_UBSan_leaks':'passed','body_sha256_lf':hashlib.sha256(body.encode()).hexdigest()}))
