#!/usr/bin/env python3
"""Source-extracted host contract, NOT ABI/native x87 precision/runtime proof.

By default extracts src/FrontEnd.cpp; --source FILE accepts a snippet or full source. A failing
historical baseline is intentional (--expect-failure). Invalid null root/visibility
VM states, timers, binary layout, actual resources and callbacks are excluded.
Attenuation is executed identically on the host only; no native equivalence claim.
"""
import argparse
import re
import subprocess
import tempfile
from pathlib import Path


def mask(s):
    return re.sub(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
                  lambda m: ''.join('\n' if c == '\n' else ' ' for c in m[0]), s)


def extract(s, name, optional=False):
    m = mask(s)
    pat = r'^\s*(?:static\s+)?(?:inline\s+)?(?:void|int|AnmVmIdView|AnmVmView\s*\*)\s*' + re.escape(name) + r'\s*\([^;{}]*\)\s*\{'
    matches = list(re.finditer(pat, m, re.M))
    if optional and not matches:
        return ''
    if len(matches) != 1:
        raise ValueError(f'{name}: expected one definition, found {len(matches)}')
    a = matches[0].start(); b = m.index('{', a, matches[0].end()); depth = 1; b += 1
    while depth and b < len(m):
        depth += (m[b] == '{') - (m[b] == '}'); b += 1
    if depth:
        raise ValueError('Unclosed definition: ' + name)
    return s[a:b]


PREFIX = r'''
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <cassert>
#include <cstdint>
using namespace std;
vector<string> trace;
function<void(string)> hook;
void event(string s) { trace.push_back(s); if(hook) hook(s); }
struct Byte { signed char value=0; string name; operator signed char() const { event("read:"+name+":"+to_string(int(value))); return value; } Byte& operator=(int n) { value=(signed char)n; return *this; } };
#ifdef PLAIN_SIGNED_BYTES
struct Supervisor { signed char bgmVolume,sfxVolume; } g_MainSupervisorView;
#else
struct Supervisor { Byte bgmVolume,sfxVolume; } g_MainSupervisorView;
#endif
int g_FrontEndSoundBgmVolume, g_FrontEndSoundSfxVolume, g_FrontEndSoundBgmAttenuation;
struct AnmVmView;
struct AnmLoadedView { int identity=0; void SetSprite(AnmVmView*,int); };
struct AnmVmLayerNodeView { void* owner=nullptr; AnmVmLayerNodeView* next=nullptr; };
struct AnmVmView {
 AnmVmLayerNodeView layerNode; short scriptIndex=0; int id=0; AnmLoadedView* anmFile=nullptr;
 union { unsigned flags; struct { unsigned otherLow:1; unsigned drawEnabled:1; unsigned otherFlags:30; }; };
 unsigned alpha=0; int sprite=-999;
};
void AnmLoadedView::SetSprite(AnmVmView* vm,int n) { vm->sprite=n; event("sprite:"+to_string(identity)+":"+to_string(vm->id)+":"+to_string(n)); }
struct AnmVmIdView { int value; AnmVmIdView():value(0) {} AnmVmIdView(int initialValue):value(initialValue) {} };
const int FRONT_END_ROOT_VM=0;
struct FrontEndControllerView { AnmVmIdView vmIds[1]; void RefreshOptionsDisplay(); } controller;
struct AnmRenderManagerView { int identity=0; map<int,AnmVmView*> vms; AnmVmView* FindVm(AnmVmIdView vmId) { int id=vmId.value; auto* result=vms.count(id)?vms[id]:nullptr; event("find:"+to_string(identity)+":"+to_string(id)); return result; } } managers[2];
AnmRenderManagerView* g_AnmRenderManagerView;
void FrontEndQueueSoundCommand(const char* s,int a,int b) { event("queue:"+string(s)+":"+to_string(a)+":"+to_string(b)); }
'''

ORACLE = r'''
AnmVmView* targetFind(AnmRenderManagerView* manager, short label) {
 AnmVmView* root=manager->FindVm(controller.vmIds[0].value);
 if(!root) { controller.vmIds[0].value=0; assert(false && "invalid null root excluded"); }
 int id=0;
 for(auto* node=&root->layerNode;node;node=node->next) {
  auto* vm=static_cast<AnmVmView*>(node->owner);
  if(vm->scriptIndex==label) { id=vm->id; break; }
 }
 return manager->FindVm(id);
}
void target() {
 g_FrontEndSoundBgmVolume=g_MainSupervisorView.bgmVolume;
 FrontEndQueueSoundCommand("SetVol",8,0);
 g_FrontEndSoundSfxVolume=g_MainSupervisorView.sfxVolume;
 if(g_FrontEndSoundSfxVolume==0) g_FrontEndSoundBgmAttenuation=-10000;
 else { float volume=static_cast<float>(g_FrontEndSoundBgmVolume)/100.0f; volume=1.0f-volume;volume*=volume;volume*=volume;volume=1.0f-volume; g_FrontEndSoundBgmAttenuation=-5000-static_cast<int>(-5000.0f*volume); }
 auto* manager=g_AnmRenderManagerView;
 const int labels[]={0x25,0x26,0x27,0x29,0x2a,0x2b,0x2d,0x2e,0x2f,0x31,0x32,0x33};
 for(int i=0;i<12;++i) {
  auto* vm=targetFind(manager,labels[i]);
  if(!vm) continue;
  int value=i<6?int(g_MainSupervisorView.bgmVolume):int(g_MainSupervisorView.sfxVolume);
  int digit=i%3==0?value/100:i%3==1?value/10%10:value%10;
  vm->anmFile->SetSprite(vm,digit+((i%6)<3?0x33:0x3d));
 }
 for(int group=0;group<2;++group) {
  int value=group?int(g_MainSupervisorView.sfxVolume):int(g_MainSupervisorView.bgmVolume);
  int h=value<10?0:value<100?0:1, t=value<10?0:1;
  int labels2[]={0x25,0x26,0x29,0x2a};
  for(int i=0;i<4;++i) targetFind(manager,labels2[i]+group*8)->drawEnabled=(i%2?t:h);
 }
}
struct Case { string name; int bgm=100,sfx=99, eventKind=0,eventIndex=0,missing=-1; bool queueMutation=false, all=false; };
AnmVmView roots[2][2], nodes[2][2][12], resolved[2][12]; AnmLoadedView files[2][12];
int findCount,spriteCount;
void reset(const Case& c) {
 trace.clear(); hook=nullptr; findCount=spriteCount=0;
#ifndef PLAIN_SIGNED_BYTES
 g_MainSupervisorView.bgmVolume.name="bgm";g_MainSupervisorView.sfxVolume.name="sfx";
#endif
 g_MainSupervisorView.bgmVolume=c.bgm;g_MainSupervisorView.sfxVolume=c.sfx;
 g_FrontEndSoundBgmVolume=g_FrontEndSoundSfxVolume=g_FrontEndSoundBgmAttenuation=867;
 controller.vmIds[0].value=1; g_AnmRenderManagerView=&managers[0];
 int labels[]={0x25,0x26,0x27,0x29,0x2a,0x2b,0x2d,0x2e,0x2f,0x31,0x32,0x33};
 for(int m=0;m<2;++m) {
  managers[m].identity=m; managers[m].vms.clear();
  for(int i=0;i<12;++i) {
   files[m][i].identity=1000+m*100+i;
   auto& v=resolved[m][i]; v={};v.id=100+i;v.scriptIndex=labels[i];v.anmFile=&files[m][i];v.flags=0xaaaaaaa8u|((i&1)<<1);v.alpha=37+i;v.sprite=-999;
   managers[m].vms[100+i]=(c.missing==i?nullptr:&v);
  }
  for(int r=0;r<2;++r) {
   auto& root=roots[m][r];root={};root.id=r+1;root.scriptIndex=-1;root.layerNode.owner=&root;root.layerNode.next=&nodes[m][r][0].layerNode;
   managers[m].vms[r+1]=&root;
   for(int i=0;i<12;++i) { auto& v=nodes[m][r][i];v={};v.id=100+((i+r)%12);v.scriptIndex=labels[i];v.layerNode.owner=&v;v.layerNode.next=i<11?&nodes[m][r][i+1].layerNode:nullptr; }
  }
 }
 hook=[c](string s) {
  if(s.rfind("queue:",0)==0 && c.queueMutation) { g_MainSupervisorView.bgmVolume=-128;g_MainSupervisorView.sfxVolume=10;g_AnmRenderManagerView=&managers[1]; }
  int kind=s.rfind("find:",0)==0?1:s.rfind("sprite:",0)==0?2:0;
  int count=kind==1?++findCount:kind==2?++spriteCount:0;
  if(kind && ((kind==c.eventKind && count==c.eventIndex)||c.all)) {
   static const int values[]={-128,-1,0,9,10,99,100,127};
   g_MainSupervisorView.bgmVolume=values[count%8];g_MainSupervisorView.sfxVolume=values[(count+3)%8];
   g_AnmRenderManagerView=&managers[1];
   if(c.missing<0) controller.vmIds[0].value=1+(count%2);
  }
 };
}
vector<string> finish() {
 hook=nullptr;
 trace.push_back("sound:"+to_string(g_FrontEndSoundBgmVolume)+":"+to_string(g_FrontEndSoundSfxVolume)+":"+to_string(g_FrontEndSoundBgmAttenuation));
 trace.push_back("root:"+to_string(controller.vmIds[0].value));
 for(int m=0;m<2;++m)for(int i=0;i<12;++i) { auto& v=resolved[m][i]; trace.push_back("state:"+to_string(m)+":"+to_string(i)+":"+to_string(v.sprite)+":"+to_string(v.flags)+":"+to_string(v.alpha)); }
 // Observable Byte is instrumentation, unlike the real nonvolatile byte.
 // Collapse adjacent identical reads: no injected callback can occur between
 // the two source-level threshold comparisons. Never collapse across events.
 vector<string> normalized;
 for(auto& e:trace) if(normalized.empty() || e.rfind("read:",0)!=0 || normalized.back()!=e) normalized.push_back(e);
 return normalized;
}
int main() {
 vector<Case> cases;
 for(int b:{-128,-1,0,9,10,99,100,127})for(int s:{-128,-1,0,9,10,99,100,127})cases.push_back({"signed-boundary-"+to_string(b)+"-"+to_string(s),b,s});
 Case q; q.name="queue-publish-order-and-manager";q.queueMutation=true;cases.push_back(q);
 for(int kind:{1,2}) for(int i=1;i<=(kind==1?40:12);++i) { Case c;c.name=(kind==1?"lookup-":"sprite-")+to_string(i);c.eventKind=kind;c.eventIndex=i;cases.push_back(c); }
 Case a;a.name="all-mutations";a.all=true;a.queueMutation=true;cases.push_back(a);
 // Only ones digits are absent: every visibility lookup must remain valid.
 for(int missing:{2,5,8,11}) { Case c;c.name="missing-digit-"+to_string(missing);c.missing=missing;c.all=true;cases.push_back(c); }
 int failed=0, functionalFailed=0;
 for(auto& c:cases) {
  reset(c); target();auto expected=finish();
  reset(c);controller.RefreshOptionsDisplay();auto actual=finish();
  auto withoutReads=[](const vector<string>& xs) { vector<string> out;for(auto& x:xs)if(x.rfind("read:",0)!=0)out.push_back(x);return out; };
  if(withoutReads(expected)!=withoutReads(actual)) { ++functionalFailed; if(functionalFailed==1) cerr<<"FUNCTIONAL mismatch (read logging excluded): "<<c.name<<"\n"; }
  if(expected!=actual) { ++failed; if(failed<=12) { size_t i=0;while(i<expected.size()&&i<actual.size()&&expected[i]==actual[i])++i;cerr<<"FAIL "<<c.name<<" event "<<i<<" expected "<<(i<expected.size()?expected[i]:"<end>")<<" actual "<<(i<actual.size()?actual[i]:"<end>")<<"\n"; } }
 }
 cout<<"cases="<<cases.size()<<" passed="<<cases.size()-failed<<" failed="<<failed<<" functional_failures="<<functionalFailed<<"\n";
 cout<<"Scope: source-extracted host synthetic state/read/order tests; excludes ABI, native x87 attenuation precision, invalid null root/visibility states, timer and real runtime.\n";
 return failed?1:0;
}
'''


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=Path(__file__).resolve().parents[1]/'src'/'FrontEnd.cpp')
    ap.add_argument('--expect-failure',action='store_true')
    ap.add_argument('--mutation-controls',action='store_true',help='Require deliberately broken source variants to fail')
    args=ap.parse_args(); s=args.source.read_text()
    probe='''// static AnmVmView *Probe() { bogus }
static AnmVmView *Probe(int x);
/* static AnmVmView *Probe() { bogus } */
static AnmVmView *Probe(int x) { const char *s = "}"; return 0; }
'''
    assert 'const char' in extract(probe,'Probe')
    assert extract(probe,'Missing',optional=True)==''
    owner=extract(s,'FrontEndControllerView::RefreshOptionsDisplay')
    names=['ResolveVm','FindChildVmId','FindChildVm','SetChildSprite','SetChildDrawEnabled','SetVolumeDigitSprites','SetVolumeLeadingDigitVisibility','FindOptionsChildId','FindOptionsChildVm']
    # Include only transitive helper dependencies used by the extracted owner.
    selected={}; todo=owner
    while True:
        added=False
        for name in names:
            if name not in selected and re.search(r'\b'+name+r'\s*\(',mask(todo)):
                selected[name]=extract(s,name);todo+='\n'+selected[name];added=True
        if not added:break
    code=PREFIX+'\n'+'\n'.join(selected[n] for n in names if n in selected)+'\n'+owner+'\n'+ORACLE
    with tempfile.TemporaryDirectory(prefix='options-refresh-') as td:
        cpp=Path(td)/'test.cpp';exe=Path(td)/'test';cpp.write_text(code)
        subprocess.run(['g++','-std=c++17','-O0','-Wall','-Wextra',str(cpp),'-o',str(exe)],check=True)
        r=subprocess.run([str(exe)])
        if not r.returncode:
            subprocess.run(['g++','-std=c++17','-O0','-DPLAIN_SIGNED_BYTES',str(cpp),'-o',str(exe)],check=True)
            print('Plain signed-char fixture:',flush=True)
            plain=subprocess.run([str(exe)])
            if plain.returncode:raise SystemExit(plain.returncode)
    if args.mutation_controls:
        if r.returncode:
            raise SystemExit('Control suite requires an initially passing source')
        controls = {
            'cache-digit-volume': code.replace('AnmVmView *vm;','AnmVmView *vm; int cachedBgm = g_MainSupervisorView.bgmVolume;').replace('g_MainSupervisorView.bgmVolume /','cachedBgm /').replace('g_MainSupervisorView.bgmVolume %','cachedBgm %'),
            'early-sfx-visibility': code.replace('    if (g_MainSupervisorView.bgmVolume < 10)', '    int earlySfx = g_MainSupervisorView.sfxVolume;\n    if (g_MainSupervisorView.bgmVolume < 10)').replace('if (g_MainSupervisorView.sfxVolume <', 'if (earlySfx <'),
            'reload-manager': code.replace('return manager->FindVm(childId);', 'return g_AnmRenderManagerView->FindVm(childId);').replace('return manager->FindVm(FindOptionsChildId(', 'return g_AnmRenderManagerView->FindVm(FindOptionsChildId('),
            'cache-root-id': code.replace('manager->FindVm(parentId->value)', 'manager->FindVm(1)').replace('manager->FindVm(*parentId)', 'manager->FindVm(1)'),
            'unsigned-volume': code.replace('g_MainSupervisorView.bgmVolume /', 'static_cast<unsigned char>(g_MainSupervisorView.bgmVolume) /').replace('g_MainSupervisorView.bgmVolume %', 'static_cast<unsigned char>(g_MainSupervisorView.bgmVolume) %'),
            'clobber-alpha': re.sub(r'(vm = FindOptionsChildVm\(this, manager, 0x[0-9a-f]+\);)', r'\1 if (vm) vm->alpha = 0;', code),
            'resample-visibility': re.sub(r'(FindOptionsChildVm\(this, manager, 0x(25|26|29|2a|2d|2e|31|32)\)->drawEnabled = )[01];', lambda m: m[1]+'g_MainSupervisorView.'+('bgmVolume' if int(m[2],16)<0x2d else 'sfxVolume')+' >= '+('100' if m[2] in ('25','29','2d','31') else '10')+';', code),
        }
        for name, broken in controls.items():
            if broken == code:
                raise SystemExit('Control transformation did not apply: '+name)
            with tempfile.TemporaryDirectory(prefix='options-control-') as td:
                cpp=Path(td)/'test.cpp';exe=Path(td)/'test';cpp.write_text(broken)
                subprocess.run(['g++','-std=c++17','-O0',str(cpp),'-o',str(exe)],check=True)
                cr=subprocess.run([str(exe)],capture_output=True,text=True)
            if cr.returncode!=1:
                raise SystemExit('Control must fail semantically, not pass/crash: '+name+' '+str(cr.returncode))
            functional=re.search(r'functional_failures=(\d+)',cr.stdout)
            if not functional or int(functional[1]) == 0:
                raise SystemExit('Control only changed proxy reads, not functional events/state: '+name)
            print('DETECTED control '+name+' (functional failures='+functional[1]+'): '+cr.stderr.splitlines()[0])
    if args.expect_failure:
        if r.returncode!=1:raise SystemExit('Expected a semantic baseline failure, not success or crash')
        print('Expected historical semantic failures confirmed.')
    else:raise SystemExit(r.returncode)

if __name__=='__main__':main()
