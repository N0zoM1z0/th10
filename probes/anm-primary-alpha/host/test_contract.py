import sys
if not __debug__:
    raise SystemExit("Optimized Python is unsupported: validation assertions must remain enabled")
from pathlib import Path
import hashlib,json,subprocess,tempfile
root=Path(__file__).resolve().parent

def block(text, marker):
 start=text.index(marker); p=text.index('{',start); depth=1; end=p+1
 while depth:
  depth += (text[end]=='{')-(text[end]=='}');end+=1
 return text[start:end]

def load(kind):
 return [(root/kind/'src'/x).read_text() for x in ('AnmManager.cpp','AnmManager.hpp')]
bcpp,bhpp=load('baseline');ccpp,chpp=load('candidate')
# Identity and exact single-hypothesis checks, independent of generated host code.
ids=json.loads((root/'identities.json').read_text())['sources']
for n,info in ids.items():
 for k in ('baseline','candidate'):
  assert hashlib.sha256((root/k/'src'/n).read_bytes()).hexdigest()==info[k+'_sha256']
base=block(bcpp,'void AnmVmView::StartPrimaryAlphaInterpolation(')
cand=block(ccpp,'void AnmVmView::StartPrimaryAlphaInterpolation(')
expected=base.replace('unsigned char mode','int mode').replace('unsigned char initial, unsigned char final','int initial, int final')
for f in ('mode','initial','final'):expected=expected.replace(f'.{f} = {f};',f'.{f} = static_cast<unsigned char>({f});')
assert cand==expected
assert bcpp.replace(base,cand,1)==ccpp
old='int duration, unsigned char mode,\n        unsigned char initial, unsigned char final);'
start=bhpp.index('    void StartPrimaryAlphaInterpolation(');end=bhpp.index(';',start)+1
bd=bhpp[start:end];cd=bd.replace('unsigned char mode','int mode').replace('unsigned char initial, unsigned char final','int initial, int final')
assert bhpp[:start]+cd+bhpp[end:]==chpp
# Real declarations, including full SetCurrent implementation, extracted unchanged.
timer=block(bhpp,'struct AnmVmTimerView')+';'
alpha=block(bhpp,'struct AnmVmAlphaInterpolationView')+';'
assert timer==block(chpp,'struct AnmVmTimerView')+';'
assert alpha==block(chpp,'struct AnmVmAlphaInterpolationView')+';'
pre='#include <climits>\n#include <cstdio>\nfloat g_AnmGameSpeed=1.0f;\n'+timer+'\n'+alpha+'\n'
for name,decl in [('Baseline',bd),('Candidate',cd)]:
 pre+='struct '+name+' { unsigned guardBefore; AnmVmAlphaInterpolationView primaryAlphaInterpolation; AnmVmAlphaInterpolationView secondaryAlphaInterpolation; unsigned guardAfter; '+decl+' };\n'
pre+=base.replace('AnmVmView::','Baseline::')+'\n'
test=r'''
static Baseline a; static Candidate b; // Static zero initialization makes the real timer constructor's flags read defined.
static float otherSpeed=0.25f;
static void seed(AnmVmAlphaInterpolationView &p, unsigned flags) {
 p.initial=-41;p.final=76543;p.initialTangent=17;p.finalTangent=-18;
 p.duration=-89;p.mode=4321;p.timer.previous=-76;p.timer.current=81;
 p.timer.subframe=12.25f;p.timer.scale=&otherSpeed;p.timer.flags=flags;
}
static bool equal(const AnmVmAlphaInterpolationView &x,const AnmVmAlphaInterpolationView &y) {
 return x.initial==y.initial&&x.final==y.final&&x.initialTangent==y.initialTangent&&x.finalTangent==y.finalTangent&&x.duration==y.duration&&x.mode==y.mode&&x.timer.previous==y.timer.previous&&x.timer.current==y.timer.current&&x.timer.subframe==y.timer.subframe&&x.timer.scale==y.timer.scale&&x.timer.flags==y.timer.flags;
}
int main() {
 static_assert(CHAR_BIT==8 && UCHAR_MAX==255,"requires unsigned 8-bit byte");
 const int v[]={INT_MIN,INT_MIN+1,-65537,-257,-256,-255,-1,0,1,127,128,254,255,256,257,65535,65536,INT_MAX-1,INT_MAX};
 const unsigned flags[]={0u,1u,0xa5a50000u,0xa5a50001u,0xfffffffeu,0xffffffffu};
 unsigned long count=0;
 for(unsigned fl:flags) for(int d:v) for(int m:v) for(int i:v) for(int f:v) {
  seed(a.primaryAlphaInterpolation,fl);seed(b.primaryAlphaInterpolation,fl);
  seed(a.secondaryAlphaInterpolation,0x5a5a0001u);seed(b.secondaryAlphaInterpolation,0x5a5a0001u);
  const auto unchanged=a.secondaryAlphaInterpolation;
  a.guardBefore=b.guardBefore=0x12345678u;a.guardAfter=b.guardAfter=0x87654321u;
  a.StartPrimaryAlphaInterpolation(d,m,i,f);b.StartPrimaryAlphaInterpolation(d,m,i,f);
  const auto &p=b.primaryAlphaInterpolation;
  if(!equal(a.primaryAlphaInterpolation,p)||p.duration!=d||p.mode!=static_cast<unsigned char>(m)||p.initial!=static_cast<unsigned char>(i)||p.final!=static_cast<unsigned char>(f)||p.initialTangent!=0||p.finalTangent!=0||p.timer.current!=0||p.timer.previous!=-1||p.timer.subframe!=0.0f||p.timer.flags!=(fl|1u)||p.timer.scale!=((fl&1u)?&otherSpeed:&g_AnmGameSpeed)||!equal(a.secondaryAlphaInterpolation,unchanged)||!equal(b.secondaryAlphaInterpolation,unchanged)||a.guardBefore!=0x12345678u||b.guardBefore!=0x12345678u||a.guardAfter!=0x87654321u||b.guardAfter!=0x87654321u) {
   std::fprintf(stderr,"contract mismatch flags=%u duration=%d mode=%d initial=%d final=%d\n",fl,d,m,i,f);return 1;
  }
  ++count;
 }
 std::printf("PASS %lu cases\n",count);
}
'''
mutants={
 'drop_mode_narrowing':cand.replace('static_cast<unsigned char>(mode)','mode'),
 'drop_initial_narrowing':cand.replace('static_cast<unsigned char>(initial)','initial'),
 'drop_final_narrowing':cand.replace('static_cast<unsigned char>(final)','final'),
 'narrow_duration':cand.replace('.duration = duration;','.duration = static_cast<unsigned char>(duration);'),
 'omit_initial_tangent_zero':cand.replace('primaryAlphaInterpolation.initialTangent = 0;',''),
 'omit_final_tangent_zero':cand.replace('primaryAlphaInterpolation.finalTangent = 0;',''),
 'omit_real_timer_reset':cand.replace('primaryAlphaInterpolation.timer.SetCurrent(0);',''),
 'clobber_unrelated_field':cand.replace('primaryAlphaInterpolation.timer.SetCurrent(0);','primaryAlphaInterpolation.timer.SetCurrent(0); secondaryAlphaInterpolation.mode = 0;'),
 'clobber_initialized_scale':cand.replace('primaryAlphaInterpolation.timer.SetCurrent(0);','primaryAlphaInterpolation.timer.SetCurrent(0); primaryAlphaInterpolation.timer.scale = &g_AnmGameSpeed;'),
}
results=[]
with tempfile.TemporaryDirectory(prefix='alpha-host-') as tmp:
 for name,body in [('candidate',cand),*mutants.items()]:
  if name != 'candidate' and body == cand:
   raise RuntimeError('Mutation did not change source: '+name)
  source=pre+body.replace('AnmVmView::','Candidate::')+'\n'+test
  p=Path(tmp)/'contract.cpp';p.write_text(source)
  if name=='candidate':(root/'host_contract.cpp').write_text(source)
  build=subprocess.run(['g++','-std=c++11','-O2','-Wall','-Wextra','-Werror','-Wno-error=uninitialized',str(p),'-o',tmp+'/test'],capture_output=True,text=True,timeout=30)
  assert build.returncode==0,(name,build.stderr)
  run=subprocess.run([tmp+'/test'],capture_output=True,text=True,timeout=30)
  assert run.returncode==(0 if name=='candidate' else 1),(name,run.stdout,run.stderr)
  if name != 'candidate' and 'contract mismatch' not in run.stderr:
   raise RuntimeError('Mutant failed without contract diagnostic: '+name)
  results.append({'variant':name,'compiled':True,'compiler_diagnostics':build.stderr.strip(),'exit_code':run.returncode,'output':(run.stdout+run.stderr).strip()})
(root/'host-results.json').write_text(json.dumps(results,indent=2)+'\n')
print(json.dumps(results,indent=2))
