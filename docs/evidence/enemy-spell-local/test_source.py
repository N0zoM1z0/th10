from pathlib import Path
import hashlib,json,re,unittest
import argparse,sys
import reproduce_source as r
parser=argparse.ArgumentParser();parser.add_argument('--source',type=Path,required=True)
args,rest=parser.parse_known_args();sys.argv=[sys.argv[0]]+rest
M={'baseline_sha256':r.BASE_SHA,'candidate_sha256':r.CANDIDATE_SHA}
B=args.source.read_bytes().decode();C=r.reconstruct(B.encode()).decode()
SIG='void __stdcall EnemyBeginSpell(int gameState, int spellId, char *name, int value)\r\n{'
DECL='static void CreateSpellStageAnimation(AnmLoadedView *resource, int script);\r\n\r\n'
HSTART='static void CreateSpellStageAnimation(AnmLoadedView *resource, int script)\r\n{\r\n'
bs=B.index(SIG); be=B.index('\r\nstruct EnemyDropVectorView',bs)
cs=C.index(SIG); hs=C.index(HSTART,cs); ce=C.index('\r\nstruct EnemyDropVectorView',hs)
body=C[cs:hs]; helper=C[hs:ce]
SCRIPTS=[0x0f,0x0e,0x11,0x15,0x16,0x0f,0x24,0x28,0x1e]
# Explicit model of the pre-existing stage decisions; source invariants independently
# bind its chosen constants and embedded initialization sequences. Not compiled C++.
def model(stage,index,chapter,transformed,mutate):
    embedded={1:[12,11],2:[14,15],3:[18,19],4:[19,20],5:[12,13],6:[33,34],7:([38] if chapter<24 else [27,28])}.get(stage,[])
    if stage not in range(1,8): return []
    script={1:15 if index<2 else 14,2:17,3:21,4:22,5:15,6:36,7:40 if chapter<24 else 30}[stage]
    trace=[]; resource='r0'; manager='m0'
    for i,embedded_script in enumerate(embedded):
        trace.append(('embedded',resource,embedded_script))
        if mutate: resource='r_after_embedded_'+str(i)
    captured=resource
    def lifecycle(resource_arg,script_arg):
        nonlocal resource,manager
        trace.append(('allocate',manager))
        if mutate: resource='r_after_allocate'; manager='m_after_allocate'
        trace.extend([('flags_or',0x40000000),('render_layer',15),('initialize',resource_arg,script_arg)])
        if mutate: manager='m_after_initialize'
        trace.append(('register',manager))
    if transformed: lifecycle(captured,script)
    else:
        # Old shared tail, deliberately represented independently of helper call.
        trace.append(('allocate',manager))
        if mutate: resource='r_after_allocate'; manager='m_after_allocate'
        trace.extend([('flags_or',0x40000000),('render_layer',15),('initialize',captured,script)])
        if mutate: manager='m_after_initialize'
        trace.append(('register',manager))
    return trace
class Review(unittest.TestCase):
    def test_hashes_crlf(self):
        self.assertEqual(hashlib.sha256(B.encode()).hexdigest(),M['baseline_sha256'])
        self.assertEqual(hashlib.sha256(C.encode()).hexdigest(),M['candidate_sha256'])
        self.assertNotIn('\n',C.replace('\r\n',''))
    def test_inverse_overlay(self):
        restored=body.removesuffix('\r\n')
        restored=restored.replace('    AnmLoadedView *stageResource;\r\n','    AnmLoadedView *stageResource;\r\n    AnmVmView *stageVm;\r\n    int stageScript;\r\n')
        restored=re.sub(r'( +)CreateSpellStageAnimation\(stageResource, (0x[0-9a-f]+)\);',lambda m:m[1]+'stageVm = g_AnmRenderManagerView->AllocateVm();\r\n'+m[1]+'stageScript = '+m[2]+';',restored)
        tail=helper[len(HSTART+'    AnmVmView *stageVm = g_AnmRenderManagerView->AllocateVm();\r\n'):-len('}\r\n')]
        tail=tail.replace('resource->InitializeVm(stageVm, script);','stageResource->InitializeVm(stageVm, stageScript);')
        restored=restored[:-len('}\r\n')]+tail+'}\r\n'
        whole=C[:cs].removesuffix(DECL)+restored+C[ce:]
        self.assertEqual(whole,B)
    def test_nine_literal_calls(self):
        self.assertEqual([int(x,16) for x in re.findall(r'CreateSpellStageAnimation\(stageResource, (0x[0-9a-f]+)\)',body)],SCRIPTS)
        self.assertEqual(C.count(HSTART),1); self.assertEqual(C.count(DECL),1)
        self.assertNotIn('stageScript',body); self.assertNotIn('stageVm;',body)
    def test_helper_exact_order(self):
        ops=['AllocateVm();','stageVm->flags35C |= 0x40000000u;','stageVm->renderLayer = 15;','resource->InitializeVm(stageVm, script);','#pragma inline_depth(0)','g_AnmRenderManagerView->AddVmVariant0(stageVm);','#pragma inline_depth(16)']
        positions=[helper.index(x) for x in ops]
        self.assertEqual(positions,sorted(positions))
        self.assertEqual(helper.count('g_AnmRenderManagerView'),2)
        self.assertNotRegex(helper,r'__forceinline|__declspec|__asm|\bvolatile\b')
    def test_policy_single_pair_after_owner(self):
        self.assertEqual(C.count('#pragma inline_depth(0)'),1)
        self.assertEqual(C.count('#pragma inline_depth(16)'),1)
        self.assertGreater(C.index('#pragma inline_depth(0)'),hs)
        self.assertNotIn('#pragma',body)
    def test_all_paths(self):
        for stage in range(-1,10):
            for index in [-1,0,1,2,3]:
                for chapter in [0,23,24,25]:
                    for mutate in [False,True]:
                        with self.subTest(stage=stage,index=index,chapter=chapter,mutate=mutate):
                            a=model(stage,index,chapter,False,mutate); b=model(stage,index,chapter,True,mutate)
                            self.assertEqual(a,b)
                            self.assertEqual(sum(x[0]=='allocate' for x in b),int(1<=stage<=7))
    def test_callback_visibility_model(self):
        t=model(7,0,23,True,True)
        self.assertIn(('initialize','r_after_embedded_0',40),t)
        self.assertEqual(t[-1],('register','m_after_initialize'))
        self.assertEqual(t[-4][0],'flags_or'); self.assertEqual(t[-3][0],'render_layer')
if __name__=='__main__': unittest.main(verbosity=2)
