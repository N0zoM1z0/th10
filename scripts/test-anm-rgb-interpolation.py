#!/usr/bin/env python3
"""Actual RGB evaluator/vector source with bounded timer and linear-curve roles.

Host logic only: no original/native-product execution, target ABI/layout,
production TU/profile identity, or original x87 control-state claim.
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
vector = block(header, 'struct AnmInt3View\n') + ';\n'
owner = block(header, 'struct AnmVmColorInterpolationView\n') + ';\n'
enum = block(header, 'enum AnmInterpolationModeView\n') + ';\n'
body = block((ROOT / 'src/AnmManager.cpp').read_text(),
             'AnmInt3View AnmVmColorInterpolationView::Evaluate()')
scale = block((ROOT / 'src/AnmInt3Scale.cpp').read_text(),
              'AnmInt3View AnmInt3View::operator*(float scale) const')
preamble = '''#include <cstdio>
#include <cstdlib>
#define CHECK(name, expr) do { if (!(expr)) { std::fprintf(stderr,"CHECK:%s\\n",name); std::exit(17); } } while (0)
struct AnmVmTimerView {
 int previous, current; float subframe; int ticks, sets;
 void Tick() { ++ticks; previous=current; ++current; subframe+=1.0f; }
 void SetCurrent(int n) { ++sets; previous=n-1; current=n; subframe=float(n); }
};
float CalculateAnmInterpolation(int mode,float n,float d) {
 CHECK("linear-role",mode==0 && d>0); return n/d;
}
'''
main = '''
// Raw-field inputs and literal expected components keep constructor mutants independent.
AnmInt3View vec(int x,int y,int z) { AnmInt3View v; v.x=x; v.y=y; v.z=z; return v; }
bool eq(const AnmInt3View &v,int x,int y,int z) { return v.x==x && v.y==y && v.z==z; }
AnmVmColorInterpolationView make(AnmInt3View initial,AnmInt3View final,int current,int duration,int mode) {
 AnmVmColorInterpolationView v={}; v.initial=initial; v.final=final;
 v.initialTangent=vec(0,0,0); v.finalTangent=vec(0,0,0);
 v.timer.current=current; v.timer.subframe=float(current); v.duration=duration; v.mode=mode; return v;
}
int main() {
 auto half=make(vec(255,17,-17),vec(0,4,-4),4,10,0);
 CHECK("rgb-linear",eq(half.Evaluate(),128,11,-11));
 CHECK("timer-state",half.timer.ticks==1 && half.timer.current==5 && half.duration==10 && half.timer.sets==0);
 auto tenth=make(vec(255,17,-17),vec(0,4,-4),0,10,0);
 CHECK("rgb-tenth",eq(tenth.Evaluate(),230,16,-16));
 auto rise=make(vec(0,-4,4),vec(255,-17,17),4,10,0);
 CHECK("rgb-rising",eq(rise.Evaluate(),127,-10,10));
 auto done=make(vec(255,17,-17),vec(1,2,3),9,10,0);
 CHECK("completion",eq(done.Evaluate(),1,2,3) && done.duration==0 && done.timer.sets==1 && done.timer.current==10);
 auto addDone=make(vec(31,15,-9),vec(8,-3,5),9,10,ANM_INTERPOLATION_ADD);
 CHECK("add-completion",eq(addDone.Evaluate(),31,15,-9) && eq(addDone.initial,31,15,-9) && addDone.duration==0);
 auto accelDone=make(vec(31,15,-9),vec(8,-3,5),9,10,ANM_INTERPOLATION_ACCELERATE); accelDone.finalTangent=vec(3,-2,4);
 CHECK("accel-completion",eq(accelDone.Evaluate(),8,-3,5) && eq(accelDone.finalTangent,3,-2,4) && accelDone.duration==0);
 auto add=make(vec(31,15,-9),vec(8,-3,5),0,0,ANM_INTERPOLATION_ADD);
 CHECK("add",eq(add.Evaluate(),39,12,-4) && eq(add.initial,39,12,-4) && add.timer.ticks==0);
 auto accel=make(vec(31,15,-9),vec(8,5,-1),0,0,ANM_INTERPOLATION_ACCELERATE); accel.finalTangent=vec(3,-2,4);
 CHECK("acceleration",eq(accel.Evaluate(),34,13,-5) && eq(accel.initial,34,13,-5) && eq(accel.finalTangent,11,3,3));
 CHECK("acceleration-next",eq(accel.Evaluate(),45,16,-2) && eq(accel.finalTangent,19,8,2));
 auto h=make(vec(1,3,5),vec(1,3,5),4,10,ANM_INTERPOLATION_HERMITE);
 CHECK("hermite-partials",eq(h.Evaluate(),0,2,4));
 auto ht=make(vec(0,0,0),vec(0,0,0),4,10,ANM_INTERPOLATION_HERMITE); ht.initialTangent=vec(16,-8,32); ht.finalTangent=vec(8,16,-24);
 CHECK("hermite-tangents",eq(ht.Evaluate(),1,-3,7));
 const auto v=vec(7,-11,23);
 CHECK("scale-operator",eq(v*0.5f,3,-5,11) && eq(v,7,-11,23));
 CHECK("scale-method",eq(v.Scale(0.5f),3,-5,11) && eq(v,7,-11,23));
 CHECK("vector-arithmetic",eq(v+vec(1,2,3),8,-9,26) && eq(v-vec(1,2,3),6,-13,20));
 std::puts("13 RGB interpolation scenarios passed");
}
'''

def require(condition, message):
    if not condition:
        raise RuntimeError(message)

# Mutants replace actual extracted source and must fail only the named assertion.
affine = 'AnmInt3View(static_cast<int>((final.x-initial.x)*value+initial.x), static_cast<int>((final.z-initial.z)*value+initial.z), static_cast<int>((final.y-initial.y)*value+initial.y))'
hermite = 'AnmInt3View(static_cast<int>(initial.x*initialWeight+final.x*finalWeight+initialTangent.x*initialTangentWeight+finalTangent.x*finalTangentWeight), static_cast<int>(initial.z*initialWeight+final.z*finalWeight+initialTangent.z*initialTangentWeight+finalTangent.z*finalTangentWeight), static_cast<int>(initial.y*initialWeight+final.y*finalWeight+initialTangent.y*initialTangentWeight+finalTangent.y*finalTangentWeight))'
mutations = {
 'alpha-rounding-in-rgb': ('body','(final - initial).Scale(value) + initial',affine,'rgb-linear'),
 'wrong-constructor-axis': ('vector','this->y = y;','this->y = z;','rgb-linear'),
 'omit-tick': ('body','timer.Tick();','','rgb-linear'),
 'late-completion': ('body','timer.current >= duration','timer.current > duration','completion'),
 'wrong-completion-arm': ('body','mode != ANM_INTERPOLATION_ADD','mode == ANM_INTERPOLATION_ADD','completion'),
 'omit-add': ('body','initial = initialValue + final;','initial = initialValue;','add'),
 'new-velocity-first': ('body','initial = initialValue + finalTangent;\n        finalTangent = finalTangent + final;',
                       'finalTangent = finalTangent + final;\n        initial = initialValue + finalTangent;','acceleration'),
 'truncate-whole-hermite': ('body','initial * initialWeight + final * finalWeight +\n            initialTangent * initialTangentWeight +\n            finalTangent * finalTangentWeight',hermite,'hermite-partials'),
 'wrong-scale-component': ('scale','static_cast<int>(y * scale)','static_cast<int>(x * scale)','hermite-partials'),
}
inputs = {'vector':vector,'body':body,'scale':scale}
build = ROOT / 'build/host-tests'
build.mkdir(parents=True, exist_ok=True)
with tempfile.TemporaryDirectory(prefix='anm-rgb-',dir=build) as tmp:
    directory=Path(tmp)
    def run(parts,flags):
        cpp=directory/'test.cpp';exe=directory/'test'
        cpp.write_text(preamble+enum+parts['vector']+owner+parts['scale']+parts['body']+main)
        r=subprocess.run(['g++','-std=c++11',*flags,str(cpp),'-o',str(exe)],capture_output=True,timeout=60)
        require(r.returncode==0,'host compile failed: '+r.stderr.decode())
        env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
        return subprocess.run([str(exe)],capture_output=True,env=env,timeout=20)
    def positive(r):
        require((r.returncode,r.stdout,r.stderr)==(0,b'13 RGB interpolation scenarios passed\n',b''),repr((r.returncode,r.stdout,r.stderr)))
    positive(run(inputs,['-O2']))
    for name,(part,old,new,check) in mutations.items():
        candidate=dict(inputs);require(candidate[part].count(old)==1,'nonunique mutation: '+name)
        candidate[part]=candidate[part].replace(old,new);r=run(candidate,['-O2'])
        require((r.returncode,r.stdout,r.stderr)==(17,b'',('CHECK:'+check+'\n').encode()),name+': '+repr((r.returncode,r.stdout,r.stderr)))
    positive(run(inputs,['-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie']))
print(json.dumps({'scope':'actual RGB/vector/helper source with bounded roles; not original/native runtime','scenarios':13,'strict_mutants':len(mutations),'ASan_UBSan_leaks':'passed','extracted_sha256_lf':{k:hashlib.sha256(v.encode()).hexdigest()for k,v in inputs.items()}}))
