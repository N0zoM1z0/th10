#!/usr/bin/env python3
"""Execute complete extracted GUI lifecycle bodies with real C++ member teardown.
Host contracts only: copied complete GuiView/GuiMessage declarations, pointer-width
neutral ANM/Win32 seams, real extracted Find/Mark and AnmVm destructor bodies.
No target ABI/layout, SEH equivalence, native execution or byte-exactness claim.
"""
from pathlib import Path
import argparse, subprocess, tempfile, re, os
ROOT=Path(__file__).resolve().parents[1]
def need(x,m):
    if not x: raise RuntimeError(m)
def block(s,a,semi=False):
    need(s.count(a)==1,'anchor '+a)
    i=s.index(a);j=s.index('{',i);d=1;k=j+1
    while d:d+=(s[k]=='{')-(s[k]=='}');k+=1
    return s[i:k+(1 if semi else 0)]
def load(root,p):
    f=root/p
    if not f.exists():f=root/'baseline'/p
    return f.read_text()
def material(root):
    s=load(root,'src/Gui.cpp');h=load(root,'src/Gui.hpp');a=load(root,'src/AnmManager.cpp');ah=load(root,'src/AnmManager.hpp')
    begin='// GUI lifecycle begin:';end='// GUI lifecycle end.'
    need(s.count(begin)==s.count(end)==1,'lifecycle markers')
    bodies=s[s.index(begin):s.index(end)]
    declarations='\n'.join(block(h,k,True) for k in ['enum GuiMessageVmIdSlot\n','struct GuiMessageVmView\n','struct GuiBossGaugeValueView\n','struct GuiView\n'])
    for f in ['AnmVmView displayVms0[10];','AnmVmView displayVms1[10];','AnmVmView displayVms2[9];','AnmVmView displayVms3[4];','AnmVmView displayVms4[2];','AnmVmView displayVms5[7];','AnmVmView bossPositionVm;','~GuiView();','~GuiMessageVmView();']:
        need(f in declarations,'declaration drift '+f)
    for f in ['void *generatedVertices;','AnmLoadedView *loadedAnms[33];','AnmVmLayerNodeView *primaryVmListHead;','AnmVmLayerNodeView *secondaryVmListHead;']:
        need(f in ah,'ANM seam field drift '+f)
    deps='\n'.join(block(a,k) for k in ['AnmVmView *AnmRenderManagerView::FindVm(AnmVmIdView id)','void AnmRenderManagerView::MarkVmForDeletion(AnmVmIdView id)','void AnmRenderManagerView::MarkLoadedVmsForDeletion(AnmLoadedView *loaded)','AnmVmView::~AnmVmView()'])
    deps=deps.replace('::MarkVmForDeletion(', '::MarkActual(').replace('::MarkLoadedVmsForDeletion(', '::MarkLoadedActual(')
    deps=deps.replace('AnmVmView::~AnmVmView()\n{','AnmVmView::~AnmVmView()\n{\n    ObserveMemberDestructor(this);')
    return bodies,declarations,deps

def program(root,bodies,declarations,deps):
    # Instrument only allocation boundaries after mutation. Matching new/delete
    # lifetimes remain real; no cleanup algorithm or member destructor is replaced.
    message_body=block(bodies,'GuiMessageVmView::~GuiMessageVmView()')
    bodies=bodies.replace(message_body,message_body[:-1]+'    ObserveMessageEnd(this);\n}')
    gui_body=block(bodies,'GuiView::~GuiView()')
    bodies=bodies.replace(gui_body,gui_body[:-1]+'    ObserveGuiEnd(this);\n}')
    bodies=bodies.replace('::operator delete(*slot);','HostDeleteAllocation(*slot);').replace('delete message;','HostDeleteMessage(message);')
    return (root/'scripts/fixtures/gui-lifecycle.cpp').read_text().replace('@GUI_DECLARATIONS@',declarations).replace('@DEPENDENCIES@',deps).replace('@LIFECYCLE@',bodies)
MUTANTS=[
('message-sixth-id','manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_ENEMY_NAME]);\n    vmIds[GUI_MESSAGE_ENEMY_NAME].value = 0;',';','messageIdsClear'),
('message-manager-reload',
    ('manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_PLAYER_PORTRAIT]);',
     'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_ENEMY_PORTRAIT]);',
     'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_TEXT_BOX]);',
     'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_TEXT_LINE_0]);',
     'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_TEXT_LINE_1]);',
     'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_ENEMY_NAME]);'),
    tuple('g_AnmRenderManagerView->'+anchor.split('->',1)[1] for anchor in (
     'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_PLAYER_PORTRAIT]);',
     'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_ENEMY_PORTRAIT]);',
     'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_TEXT_BOX]);',
     'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_TEXT_LINE_0]);',
     'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_TEXT_LINE_1]);',
     'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_ENEMY_NAME]);')),
    'messageManagerStable'),
('resource-slot-cached','(*slot)->Release();\n            ::operator delete(*slot);','AnmLoadedView *saved = *slot;\n            (*slot)->Release();\n            ::operator delete(saved);','releasedCorrectSlot'),
('message-file-mode-not-reloaded','if ((g_GuiGameFlags & 9u) == 0)','if ((initialFixtureFlags & 9u) == 0)','fileOwnershipCorrect',2),
('first-chain-not-captured','UnregisterElem(element, chain);','UnregisterElem(element, ((void)chain, th10::g_Chain));','chain == expectedChain',2),
('draw-chain-not-reloaded','chain = th10::g_Chain;\n    if (element != NULL)','chain = firstFixtureChain;\n    if (element != NULL)','chain == expectedChain',2),
('count-increment-omitted','++g_MainSupervisorView.criticalSectionLockCounts[0];',';','countCorrect',2),
('count-decrement-before-leave','LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);\n        --g_MainSupervisorView.criticalSectionLockCounts[0];','--g_MainSupervisorView.criticalSectionLockCounts[0];\n        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);','countCorrect',2),
('draw-cleared-without-target-store','    updateCallback = NULL;','    updateCallback = NULL;\n    drawCallback = NULL;','active->drawCallback==originalDraw'),
('cleanup-after-chains','    GuiCleanupResources(this);','    ;','(p->flags&2)==0'),
('update-clear-omitted','    updateCallback = NULL;','    ;','updateCleared'),
('global-clear-omitted','    g_GuiView = NULL;','    ;','globalCleared'),
('value-eighth-omitted','i < 8','i < 7','valueIdsClear',2),
('destructor-manager-reloaded','manager->MarkLoadedVmsForDeletion(frontAnm);','g_AnmRenderManagerView->MarkLoadedVmsForDeletion(frontAnm);','destructorManagerStable'),
('resource-manager-reloaded','manager->MarkVmForDeletion(gui->bossGaugeVmId);','g_AnmRenderManagerView->MarkVmForDeletion(gui->bossGaugeVmId);','this==resourceManager'),
('value-zero-looked-up','if (id != 0)','if (true)','valueEffectsCorrect',2),
('value-nonroot-traversed','vm->layerNode.previous == NULL','true','valueEffectsCorrect',2),
('value-root-test-reversed','vm->layerNode.previous == NULL','vm->layerNode.previous != NULL','valueEffectsCorrect',2),
('value-child-mark-omitted','static_cast<AnmVmView *>(child->owner)->flags35C |= 0x04000000u;',';','valueEffectsCorrect',2),
('value-deletion-bit','vm->flags35C |= 0x04000000u;','vm->flags35C |= 0x02000000u;','valueEffectsCorrect',2),
('segments-not-cleared','i < 10','i < 9','segmentsClear'),
('segment-count-not-cleared','gui->bossGaugeSegmentCount = 0;',';','segmentCountClear'),
]
def check_line(candidate,expression,scope=None):
    start=candidate.index(scope) if scope else 0
    region=candidate[start:]
    if scope: region=block(candidate,scope)
    matches=list(re.finditer(r'CHECK\('+re.escape(expression)+r'\)',region))
    need(len(matches)==1,'CHECK location must be unique: '+expression)
    position=start+matches[0].start()
    return candidate.count('\n',0,position)+1

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--root',type=Path,default=ROOT);ap.add_argument('--baseline-only',action='store_true');ap.add_argument('--optimization',default='-O1');ap.add_argument('--sanitizers',default='undefined');args=ap.parse_args()
    bodies,decl,deps=material(args.root)
    cxx=os.environ.get('CXX','g++');flags=['-std=c++17',args.optimization,'-g','-Wall','-Wextra','-Werror','-Wno-unused-variable','-Wno-unused-parameter','-fno-omit-frame-pointer']
    if args.sanitizers: flags+=['-fsanitize='+args.sanitizers,'-fno-sanitize-recover=all']
    # Host-only ASan executables use the same non-PIE convention as other fixtures.
    # PIE controls intermittently hit repeated DEADLYSIGNAL before an intended
    # CHECK on this host; this changes no native target compiler profile.
    if 'address' in args.sanitizers.split(','): flags+=['-fno-pie','-no-pie']
    with tempfile.TemporaryDirectory(prefix='gui-lifecycle-') as td:
        td=Path(td)
        def run(name,b,ds=decl):
            src=td/(name+'.cpp');exe=td/name;src.write_text(program(args.root,b,ds,deps))
            c=subprocess.run([cxx,*flags,str(src),'-o',str(exe)],capture_output=True,text=True,timeout=60)
            need(c.returncode==0,'compile failure '+name+'\n'+c.stdout+c.stderr)
            return src.read_text(),subprocess.run([str(exe)],capture_output=True,text=True,timeout=20,env=dict(os.environ,ASAN_OPTIONS=os.environ.get('ASAN_OPTIONS','detect_leaks=0')))
        _,p=run('baseline',bodies);need(p.returncode==0 and not p.stderr,'baseline failure\n'+p.stdout+p.stderr);print(p.stdout,end='',flush=True)
        if not args.baseline_only:
            for m in MUTANTS:
                name,old,new,expected,*n=m
                if isinstance(old,tuple): need(len(old)==len(new),'mutant anchor/replacement length '+name)
                pairs=zip(old,new) if isinstance(old,tuple) else [(old,new)]
                b=bodies
                for anchor,replacement in pairs:
                    need(bodies.count(anchor)==(n[0] if n else 1),'mutant anchor '+name+': '+anchor)
                    b=b.replace(anchor,replacement)
                if name=='cleanup-after-chains':
                    b=b.replace('    AnmRenderManagerView *manager = g_AnmRenderManagerView;\n    updateCallback = NULL;','    GuiCleanupResources(this);\n    AnmRenderManagerView *manager = g_AnmRenderManagerView;\n    updateCallback = NULL;')
                # The saved-pointer mutant is still observed safely at the deletion
                # boundary; wrong ownership fails CHECK before any invalid free.
                b=b.replace('::operator delete(saved);','HostDeleteAllocation(saved);')
                candidate,p=run(name,b)
                scope=None
                if expected=='countCorrect': scope='static void LeaveCriticalSection(int *p)' if name=='count-decrement-before-leave' else 'void th10::Chain::UnregisterElem('
                expected_line=check_line(candidate,expected,scope)
                need(p.returncode==1 and p.stdout=='' and p.stderr==f'GUI_LIFECYCLE_CHECK_FAILED line {expected_line}: {expected}\n','invalid kill '+name+'\n'+p.stdout+p.stderr)
                print('PASS intended CHECK mutant '+name,flush=True)
            for name,ds,expected in [
                ('member-array-order',decl.replace('    AnmVmView displayVms0[10];\n    AnmVmView displayVms1[10];','    AnmVmView displayVms1[10];\n    AnmVmView displayVms0[10];'),'memberDestructors[i]==42-i'),
                ('extra-member-destruction',decl.replace('    AnmVmView bossPositionVm;','    AnmVmView bossPositionVm;\n    AnmVmView extraTestMember;'),'memberDestructionCountCorrect')]:
                need(ds!=decl,'declaration mutant anchor '+name)
                candidate,p=run(name,bodies,ds)
                expected_line=check_line(candidate,expected)
                need(p.returncode==1 and not p.stdout and p.stderr==f'GUI_LIFECYCLE_CHECK_FAILED line {expected_line}: {expected}\n','invalid declaration kill '+name+'\n'+p.stdout+p.stderr)
                print('PASS intended CHECK header mutant '+name,flush=True)
    print('PASS complete extracted GUI lifecycle host contracts; no native/ABI/exactness credit')
if __name__=='__main__': main()
