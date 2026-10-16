#!/usr/bin/env python3
"""Source-extracted host contract for RefreshKeyConfigDisplay (not native/ABI proof).
Default: ../src/FrontEnd.cpp relative to this script; no private fixtures required.
Valid missing labels/resolution failures are covered; null parent is outside the
native owner's precondition and is deliberately never executed.
"""
import argparse,re,subprocess,tempfile
from pathlib import Path

def mask(s):
    return re.sub(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',lambda m: ''.join('\n' if c=='\n' else ' ' for c in m[0]),s)
def extract(s,name,optional=False):
    m=mask(s)
    p=r'^\s*(?:static\s+)?(?:(?:inline|__forceinline)\s+)?(?:void|int|AnmVmIdView|AnmVmView\s*\*)\s*'+re.escape(name)+r'\s*\([^;{}]*\)\s*\{'
    matches=list(re.finditer(p,m,re.M))
    if optional and not matches:return ''
    if len(matches)!=1:raise ValueError(f'{name}: expected one definition, got {len(matches)}')
    a=matches[0].start();b=m.index('{',a,matches[0].end())+1;depth=1
    while depth and b<len(m):depth+=(m[b]=='{')-(m[b]=='}');b+=1
    if depth:raise ValueError('unclosed '+name)
    return s[a:b]
PREFIX=r'''
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <cassert>
using namespace std;
#define __forceinline inline
vector<string> trace;
function<void(string)> hook;
void event(string s) { trace.push_back(s);if(hook)hook(s); }
struct Binding { short value=0; int index=0; operator short() const { event("read:"+to_string(index)+":"+to_string(value));return value; } Binding& operator=(int n) {value=static_cast<short>(n);return *this;} };
struct AnmVmIdView { int value; AnmVmIdView(int n=0):value(n) {} };
struct AnmVmView;
struct AnmLoadedView { int identity;void SetSprite(AnmVmView*,int); };
struct AnmVmLayerNodeView {void* owner=nullptr;AnmVmLayerNodeView* next=nullptr;};
struct AnmVmView {AnmVmLayerNodeView layerNode;short scriptIndex=-1;int id=0;AnmLoadedView* anmFile=nullptr;int sprite=-999;};
void AnmLoadedView::SetSprite(AnmVmView* vm,int n) {vm->sprite=n;event("sprite:"+to_string(identity)+":"+to_string(vm->id)+":"+to_string(n));}
struct FrontEndControllerView {AnmVmIdView vmIds[3];
#ifdef PLAIN_SHORT
short keyConfigBindings[5];
#else
Binding keyConfigBindings[5];
#endif
void RefreshKeyConfigDisplay();} controller;
struct AnmRenderManagerView {int identity;map<int,AnmVmView*> vms;AnmVmView* FindVm(AnmVmIdView id) {auto* v=vms.count(id.value)?vms[id.value]:nullptr;event("find:"+to_string(identity)+":"+to_string(id.value));return v;}} managers[2];
AnmRenderManagerView* g_AnmRenderManagerView;
'''
ORACLE=r'''
const int labels[]={0x43,0x44,0x4d,0x4e,0x45,0x46,0x4f,0x50,0x47,0x48,0x51,0x52,0x49,0x4a,0x53,0x54,0x4b,0x4c,0x55,0x56};
AnmVmView* targetFind(AnmRenderManagerView* manager,short label) {
 auto* root=manager->FindVm(controller.vmIds[2]);assert(root && "null root outside contract");
 int id=0;
 for(auto* node=&root->layerNode;node;node=node->next) {
  if(static_cast<AnmVmView*>(node->owner)->scriptIndex==label) {id=static_cast<AnmVmView*>(node->owner)->id;break;}
 }
 return manager->FindVm(id);
}
void target() {
 auto* manager=g_AnmRenderManagerView;
 for(int i=0;i<20;++i) {auto* vm=targetFind(manager,labels[i]);if(vm) {int binding=controller.keyConfigBindings[i/4];vm->anmFile->SetSprite(vm,(i%2?binding%10:binding/10)+0x33);}}
}
struct Case {string name;int value=19,kind=0,index=0,missing=-1,unresolved=-1;bool all=false,parentMatch=false,duplicate=false,remap=false,fileSwap=false,shared=false,empty=false;};
AnmVmView roots[2][3],nodes[2][3][21],resolved[2][20];AnmLoadedView files[2][20];
int finds,sprites;
void reset(const Case& c) {
 trace.clear();hook=nullptr;finds=sprites=0;
 for(int i=0;i<5;++i) {
#ifndef PLAIN_SHORT
 controller.keyConfigBindings[i].index=i;
#endif
 controller.keyConfigBindings[i]=c.value;
 }
 controller.vmIds[1]=1;controller.vmIds[2]=2;g_AnmRenderManagerView=&managers[0];
 for(int m=0;m<2;++m) {
  managers[m].identity=m;managers[m].vms.clear();
  for(int i=0;i<20;++i) {files[m][i].identity=m*100+i;auto& v=resolved[m][i];v={};v.id=100+i;v.scriptIndex=labels[i];v.anmFile=&files[m][i];managers[m].vms[v.id]=(c.unresolved==i?nullptr:&v);}
  if(c.shared) for(int i=0;i<20;++i)managers[m].vms[100+i]=&resolved[m][7];
  for(int r=0;r<3;++r) {
   auto& root=roots[m][r];root={};root.id=r+1;root.layerNode.owner=&root;root.layerNode.next=&nodes[m][r][0].layerNode;managers[m].vms[r+1]=&root;
   for(int i=0;i<21;++i) {auto& v=nodes[m][r][i];v={};v.id=100+((i+r+19)%20);v.scriptIndex=i<20?labels[i]:-1;v.anmFile=&files[m][i%20];v.layerNode.owner=&v;v.layerNode.next=i<20?&nodes[m][r][i+1].layerNode:nullptr;if(c.missing==i)v.scriptIndex=-1;}
   if(c.parentMatch) {root.scriptIndex=labels[0];root.id=117;root.anmFile=&files[m][17];}
   if(c.empty)root.layerNode.next=nullptr;
   if(c.duplicate) {nodes[m][r][20].scriptIndex=labels[0];nodes[m][r][20].id=118;}
  }
 }
 hook=[c](string s) {
  int kind=s.rfind("find:",0)==0?1:s.rfind("sprite:",0)==0?2:0;
  int count=kind==1?++finds:kind==2?++sprites:0;
  if(kind && ((kind==c.kind && count==c.index)||c.all)) {
   // These callbacks run after FindVm snapshots its return value and after
   // SetSprite writes its result. Changes affect later resolution/file uses.
   if(c.remap) {
    for(int m=0;m<2;++m)for(int i=0;i<20;++i)
     managers[m].vms[100+i]=&resolved[m][(i+7)%20];
    return;
   }
   if(c.fileSwap) {
    for(int m=0;m<2;++m)for(int i=0;i<20;++i)
     resolved[m][i].anmFile=&files[1-m][(i+9)%20];
    return;
   }
   const int values[]={-32768,-101,-10,-1,0,9,10,99,32767};
   for(int i=0;i<5;++i)controller.keyConfigBindings[i]=values[(i+count)%9];
   g_AnmRenderManagerView=&managers[1];controller.vmIds[2]=2+(count%2);
  }
 };
}
vector<string> finish() {
 hook=nullptr;trace.push_back("root:"+to_string(controller.vmIds[2].value));
 for(int m=0;m<2;++m)for(int i=0;i<20;++i)trace.push_back("state:"+to_string(m)+":"+to_string(i)+":"+to_string(resolved[m][i].sprite));
 return trace;
}
int main() {
 vector<Case> cases;
 for(int v:{-32768,-32767,-101,-100,-11,-10,-9,-1,0,1,9,10,11,99,100,32766,32767})cases.push_back({"signed-boundary-"+to_string(v),v});
 for(int k:{1,2})for(int i=1;i<=(k==1?40:20);++i) {Case c;c.name=(k==1?"find-mutation-":"sprite-mutation-")+to_string(i);c.kind=k;c.index=i;cases.push_back(c);}
 Case all;all.name="all-events-mutate";all.all=true;cases.push_back(all);
 for(int i=0;i<20;++i) {Case c;c.name="missing-label-"+to_string(i);c.missing=i;c.all=true;cases.push_back(c);c.name="failed-resolution-"+to_string(i);c.missing=-1;c.unresolved=i;cases.push_back(c);}
 Case p;p.name="parent-node-first";p.parentMatch=true;cases.push_back(p);
 Case d;d.name="duplicate-first-match";d.duplicate=true;cases.push_back(d);
 // Preserve the original 120 cases, then add topology/identity challenges.
 assert(cases.size()==120);
 for(int mode=0;mode<2;++mode)for(int k:{1,2})for(int i=1;i<=(k==1?40:20);++i) {
  Case c;c.name=(mode?"file-swap-":"id-remap-")+string(k==1?"find-":"sprite-")+to_string(i);
  c.kind=k;c.index=i;c.remap=mode==0;c.fileSwap=mode==1;cases.push_back(c);
 }
 Case alias;alias.name="shared-resolved-child";alias.shared=true;cases.push_back(alias);
 alias.name="shared-resolved-child-mutating";alias.all=true;cases.push_back(alias);
 Case empty;empty.name="empty-continuation-no-match";empty.empty=true;cases.push_back(empty);
 empty.name="empty-continuation-parent-match";empty.parentMatch=true;cases.push_back(empty);
 int failed=0,functionalFailed=0;
 for(auto& c:cases) {
  reset(c);target();auto expected=finish();reset(c);controller.RefreshKeyConfigDisplay();auto actual=finish();
  auto withoutReads=[](const vector<string>& xs){vector<string> out;for(auto& s:xs)if(s.rfind("read:",0)!=0)out.push_back(s);return out;};
  if(withoutReads(expected)!=withoutReads(actual))++functionalFailed;
  if(expected!=actual) {++failed;if(failed<=5) {size_t i=0;while(i<expected.size()&&i<actual.size()&&expected[i]==actual[i])++i;cerr<<"FAIL "<<c.name<<" event "<<i<<" expected "<<(i<expected.size()?expected[i]:"<end>")<<" actual "<<(i<actual.size()?actual[i]:"<end>")<<"\n";}}
 }
 cout<<"cases="<<cases.size()<<" passed="<<cases.size()-failed<<" failed="<<failed<<" functional_failures="<<functionalFailed<<"\n";
 return failed?1:0;
}
'''

def run(source,plain=False):
    with tempfile.TemporaryDirectory(prefix='keyconfig-display-') as td:
        cpp=Path(td)/'test.cpp';exe=Path(td)/'test';cpp.write_text(source)
        subprocess.run(['g++','-std=c++17','-O0','-Wall','-Wextra',*(['-DPLAIN_SHORT'] if plain else []),str(cpp),'-o',str(exe)],check=True)
        return subprocess.run([str(exe)],capture_output=True,text=True)

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=Path(__file__).resolve().parents[1]/'src/FrontEnd.cpp')
    ap.add_argument('--expect-failure',action='store_true');ap.add_argument('--mutation-controls',action='store_true');a=ap.parse_args();s=a.source.read_text()
    owner=extract(s,'FrontEndControllerView::RefreshKeyConfigDisplay')
    names=['ResolveVm','FindChildVmId','FindChildVm','SetChildSprite','SetKeyBindingDigitSprites','FindKeyConfigChildId','FindKeyConfigChildVm']
    selected={};todo=owner
    while True:
        added=False
        for name in names:
            if name not in selected and re.search(r'\b'+name+r'\s*\(',mask(todo)):
                selected[name]=extract(s,name);todo+='\n'+selected[name];added=True
        if not added:break
    enums=['FRONT_END_ROOT_VM','FRONT_END_KEY_CONFIG_ROOT_VM']+['FRONT_END_KEY_CONFIG_'+n for n in ['SHOT','BOMB','FOCUS','MENU','SKIP']]
    constants='\n'.join('const int '+n+'='+re.search(r'\b'+n+r'\s*=\s*(\d+)',mask(s))[1]+';' for n in enums)
    actual='\n'.join(selected[n] for n in names if n in selected)+'\n'+owner
    code=PREFIX+constants+'\n'+actual+'\n'+ORACLE
    for plain in [False,True]:
        r=run(code,plain);print(('Plain short: ' if plain else 'Instrumented signed short: ')+r.stdout.strip());print(r.stderr,end='')
        if a.expect_failure:
            if r.returncode!=1 or not re.search(r'functional_failures=[1-9]',r.stdout):raise SystemExit('Expected functional historical failure, not crash/pass')
        elif r.returncode:raise SystemExit(r.returncode)
    if a.mutation_controls:
        if a.expect_failure:raise SystemExit('controls require passing source')
        mutations={
            'wrong-root':actual.replace('vmIds[FRONT_END_KEY_CONFIG_ROOT_VM]','vmIds[FRONT_END_ROOT_VM]'),
            'early-binding-cache':actual.replace('AnmVmView *vm;','AnmVmView *vm; short cached[5]; for(int i=0;i<5;++i)cached[i]=keyConfigBindings[i];').replace('vm, keyConfigBindings[','vm, cached['),
            'reload-manager':actual.replace('return manager->FindVm(FindKeyConfigChildId','return g_AnmRenderManagerView->FindVm(FindKeyConfigChildId'),
            'cache-root-id':actual.replace('manager->FindVm(*parentId)','manager->FindVm(2)'),
            'direct-child-pointer':actual.replace('return manager->FindVm(FindKeyConfigChildId(controller, manager, scriptIndex));','AnmVmView *parent = manager->FindVm(controller->vmIds[2]); for (AnmVmLayerNodeView *n = &parent->layerNode; n; n = n->next) { AnmVmView *v = static_cast<AnmVmView *>(n->owner); if (v->scriptIndex == scriptIndex) return v; } return NULL;'),
            'unsigned-binding':re.sub(r'(keyConfigBindings\[FRONT_END_KEY_CONFIG_\w+\])',r'static_cast<unsigned short>(\1)',actual),
            'clamp-negative':re.sub(r'(keyConfigBindings\[FRONT_END_KEY_CONFIG_\w+\])',r'(\1 < 0 ? 0 : static_cast<int>(\1))',actual),
            'plus-one':re.sub(r'(keyConfigBindings\[FRONT_END_KEY_CONFIG_\w+\])',r'(\1 + 1)',actual),
            'wrong-selected-base':re.sub(r'(vm = FindKeyConfigChildVm\(this, manager, 0x(?:4[d-f]|5[0-6])\);[\s\S]*?10 \+ )0x33',r'\g<1>0x3d',actual),
            'last-match':actual.replace('AnmVmLayerNodeView *node = &parent->layerNode;', 'AnmVmIdView last; AnmVmLayerNodeView *node = &parent->layerNode;').replace('return AnmVmIdView(static_cast<AnmVmView *>(node->owner)->id);','last = AnmVmIdView(static_cast<AnmVmView *>(node->owner)->id);').replace('return AnmVmIdView();','return last;'),
            'skip-parent-node':actual.replace('&parent->layerNode;','parent->layerNode.next;'),
        }
        for name,broken in mutations.items():
            if broken==actual:raise SystemExit('Unapplied control '+name)
            r=run(PREFIX+constants+'\n'+broken+'\n'+ORACLE,True)
            if r.returncode!=1 or not re.search(r'functional_failures=[1-9]',r.stdout):raise SystemExit('Control must fail functionally without crashing: '+name+' '+r.stdout+r.stderr)
            print('DETECTED '+name+': '+r.stdout.strip())
    print('Scope: real source-extracted owner/helpers; synthetic host behavior. No native ABI, assembly topology, resource or runtime proof; invalid null-parent states excluded.')
if __name__=='__main__':main()
