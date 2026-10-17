#!/usr/bin/env python3
"""Opcode20 ordinary-stage valid-object tests using complete Run. Creation
callbacks are synthetic stress seams; no null member calls or native claims.
"""
from pathlib import Path
import argparse,hashlib,importlib.util,json,os,re,shutil,subprocess,sys
EXPECTED='            else\n            {\n                GuiView *gui = g_GuiView;\n                g_AnmRenderManagerView->MarkVmForDeletion(gui->specialVmId9E14.value);\n                gui->specialVmId9E14.value = 0;\n                {\n                    AnmLoadedView *loaded = gui->frontAnm;\n                    gui->specialVmId9E14.value = loaded->CreateVmVariant0(0x4c, GUI_VM_RENDER_LAYER).value;\n                }\n                g_MainSupervisorView.SetNextGameMode(0x0b);\n                th10::leaf::Fn004175E0(g_GameStageState);\n            }'
OLD='            else\n            {\n                GuiView *gui = g_GuiView;\n                g_AnmRenderManagerView->MarkVmForDeletion(gui->specialVmId9E14.value);\n                gui->specialVmId9E14.value = 0;\n                {\n                    AnmLoadedView *loaded = gui->frontAnm;\n                    gui->specialVmId9E14.value = loaded == NULL ? 0 : loaded->CreateVmVariant0(0x4c, GUI_VM_RENDER_LAYER).value;\n                }\n                g_MainSupervisorView.SetNextGameMode(0x0b);\n                th10::leaf::Fn004175E0(g_GameStageState);\n            }'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def once(s,a,b):
 if s.count(a)!=1:raise RuntimeError('anchor count: '+a)
 return s.replace(a,b)
def region(s):
 a='            else\n            {\n                GuiView *gui = g_GuiView;';b='            break;'
 if s.count(a)!=1:raise RuntimeError('opcode20 ordinary boundary count')
 start=s.index(a);end=s.index(b,start);return start,end,s[start:end]
def replace_region(s,new):
 a,b,_=region(s);return s[:a]+new+'\n'+s[b:]
def shape(s):
 if re.sub(r'\s+','',region(s)[2])!=re.sub(r'\s+','',EXPECTED):raise RuntimeError('opcode20 structure mismatch')
def intended(r,label):return r.returncode==1 and r.stdout=='' and r.stderr=='CHECK '+label+'\n'
def classifier():
 assert intended(subprocess.CompletedProcess([],1,'','CHECK x\n'),'x')
 for v in [(-11,'','CHECK x\n'),(139,'','CHECK x\n'),(0,'','CHECK x\n'),(2,'','CHECK x\n'),(1,'x','CHECK x\n'),(1,'','CHECK other\n'),(1,'','compiler error'),(1,'','CHECK x\nextra'),(1,'',''),(1,'','CHECK x'),(1,'','CHECK x\nAddressSanitizer: error\n')]:
  if intended(subprocess.CompletedProcess([],*v),'x'):raise RuntimeError('failure classifier weakened')
 return 11
CASES='\nstatic void Event20(){\n for(int stage:{1,2,3,4,5})for(int mode:{0,1,2})for(int id:{0,-1,INT_MIN,INT_MAX,137}){\n  Fixture f;std::unique_ptr<GuiView> replacement(new GuiView);GuiMessageVmView replacementMessage(nullptr);\n  replacement->frontAnm=&f.enemyResource;replacement->stageUiAnm=&f.playerResource;replacement->message=&replacementMessage;\n  replacement->specialVmId9E14.value=901;f.gui->frontAnm=&f.otherResource;f.gui->stageUiAnm=&f.playerResource;\n  f.gui->message=&f.message;f.message.flags=0x40;f.message.unknown58=0x1357;\n  g_GuiStage=stage;f.Program(20);host.id=id;int callbacks=0,advances=0;\n  host.afterCreate=[&](){\n   Check(++callbacks==1,"event20.create_once");\n   Check((f.vms[14].flags35C&0x04000000u)!=0,"event20.delete_before_create");\n   Check(f.gui->specialVmId9E14.value==0,"event20.zero_before_create");\n   Check(host.advances.empty()&&g_MainSupervisorView.nextGameMode==-99,"event20.create_before_mode_advance");\n   Check(host.creations.back().resource==&f.otherResource,"event20.captured_front_resource");\n   Check(host.creations.back().script==0x4c&&host.creations.back().layer==15,"event20.script_layer");\n   if(mode){g_GuiView=replacement.get();f.gui->frontAnm=&f.enemyResource;}\n   if(mode==2){replacement->frontAnm=&f.playerResource;f.gui->specialVmId9E14.value=777;}\n  };\n  host.afterAdvance=[&](){++advances;\n   Check(callbacks==1,"event20.create_before_advance");\n   Check(f.gui->specialVmId9E14.value==id&&replacement->specialVmId9E14.value==901,"event20.publish_captured_slot");\n   Check(g_MainSupervisorView.nextGameMode==0x0b,"event20.mode_before_advance");\n  };\n  Normal(f);\n  Check(callbacks==1&&advances==1&&host.creations.size()==1&&host.order==std::vector<std::string>{"create","advance"},"event20.once_order");\n  Check(host.advances==std::vector<const void*>{g_GameStageState},"event20.advance_owner");\n  Check(f.gui->specialVmId9E14.value==id&&replacement->specialVmId9E14.value==901,"event20.publish_captured_slot");\n  Check(f.message.flags==0x40&&f.message.unknown58==0x1357,"event20.sentinels");\n  host.afterCreate={};host.afterAdvance={};g_GuiView=f.gui.get();\n }\n}\nstatic void Event20Continuity(){\n Fixture f;g_GuiStage=1;f.first=f.Append(20);f.Append(11);auto other=f.Append(24);f.future=f.Append(0,{},100);f.message.currentInstruction=f.Instruction();\n host.afterCreate=[&](){f.message.currentInstruction=f.bytes.data()+other;};Normal(f);\n Check(f.message.inputCooldown==0&&host.order==std::vector<std::string>{"create","advance"},"event20.retained_dispatcher_reload");\n}\n'

CALL='gui->specialVmId9E14.value = loaded->CreateVmVariant0(0x4c, GUI_VM_RENDER_LAYER).value;'
ZERO='gui->specialVmId9E14.value = 0;'
DELETE='g_AnmRenderManagerView->MarkVmForDeletion(gui->specialVmId9E14.value);'
LOAD='AnmLoadedView *loaded = gui->frontAnm;'
ADVANCE='th10::leaf::Fn004175E0(g_GameStageState);'
MODE='g_MainSupervisorView.SetNextGameMode(0x0b);'
MUTATIONS=[
 ('script',EXPECTED.replace('(0x4c, GUI_VM_RENDER_LAYER)','(0x4d, GUI_VM_RENDER_LAYER)'),'event20.script_layer'),
 ('layer',EXPECTED.replace('(0x4c, GUI_VM_RENDER_LAYER)','(0x4c, 14)'),'event20.script_layer'),
 ('wrong-resource',EXPECTED.replace('gui->frontAnm','gui->stageUiAnm'),'event20.captured_front_resource'),
 ('omit-zero',EXPECTED.replace(ZERO,''),'event20.zero_before_create'),
 ('late-zero',EXPECTED.replace(ZERO,'').replace(CALL,CALL+' '+ZERO),'event20.zero_before_create'),
 ('omit-delete',EXPECTED.replace(DELETE,''),'event20.delete_before_create'),
 ('publish-global',EXPECTED.replace(CALL,'g_GuiView->specialVmId9E14.value = loaded->CreateVmVariant0(0x4c, GUI_VM_RENDER_LAYER).value;'),'event20.publish_captured_slot'),
 ('omit-publication',EXPECTED.replace(CALL,'loaded->CreateVmVariant0(0x4c, GUI_VM_RENDER_LAYER).value;'),'event20.publish_captured_slot'),
 ('omit-create',EXPECTED.replace(CALL,''),'event20.create_before_advance'),
 ('duplicate-create',EXPECTED.replace(CALL,CALL+' '+CALL),'event20.create_once'),
 ('wrong-mode',EXPECTED.replace(MODE,'g_MainSupervisorView.SetNextGameMode(0x0c);'),'event20.mode_before_advance'),
 ('early-advance',EXPECTED.replace(ADVANCE,'').replace(DELETE,ADVANCE+' '+DELETE),'event20.create_before_advance'),
 ('omit-advance',EXPECTED.replace(ADVANCE,''),'event20.once_order'),
 ('wrong-advance-owner',EXPECTED.replace(ADVANCE,'th10::leaf::Fn004175E0(g_GameStageState + 4);'),'event20.advance_owner'),
]
def main():
 p=argparse.ArgumentParser();p.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[1]);p.add_argument('--out',type=Path,required=True);p.add_argument('--sanitize',action='store_true');p.add_argument('--no-mutations',action='store_true');a=p.parse_args()
 if os.path.lexists(a.out):p.error('output must be fresh, including dangling symlinks')
 root=a.root.resolve();out=a.out.resolve();source=root/'src/Gui.cpp';text=source.read_text();shape(text);assert len(MUTATIONS)==14 and len({n for n,_,_ in MUTATIONS})==14
 negatives=[]
 for n,body in [('resource-skip',OLD),('conditional-resource-skip',EXPECTED.replace(CALL,'if (loaded != NULL) '+CALL)),('predelete-resource-load',EXPECTED.replace(DELETE,LOAD+' '+DELETE).replace('                    '+LOAD,''))]:
  try:shape(replace_region(text,body))
  except RuntimeError as e:
   if str(e)!='opcode20 structure mismatch':raise
  else:raise RuntimeError('structural control escaped')
  negatives.append(n)
 classifier_count=classifier();sys.dont_write_bytecode=True;sys.path.insert(0,str(root/'scripts'));driverpath=root/'scripts/test-gui-message-run.py';fixture=root/'scripts/fixtures/gui-message-run.cpp'
 inputs={str(x):sha(x) for x in [source,driverpath,fixture,root/'scripts/gui_message_run_mutations.py',Path(__file__).resolve()]}
 spec=importlib.util.spec_from_file_location('event20_driver',driverpath);driver=importlib.util.module_from_spec(spec);spec.loader.exec_module(driver)
 out.mkdir(parents=True,exist_ok=False);base=out/'positive';manifest=driver.prepare(root,None,base,False);inputs.update(manifest['source_inputs_sha256'])
 ft=fixture.read_text();ft=once(ft,'afterSprite,afterDecode,afterDraw,afterCreate,afterEffect,afterTransition','afterSprite,afterDecode,afterDraw,afterCreate,afterEffect,afterTransition,afterAdvance');ft=once(ft,'host.order.push_back("advance");}','host.order.push_back("advance");if(host.afterAdvance)host.afterAdvance();}');ft=once(ft,'int main(int argc,char**argv){',CASES+'\nint main(int argc,char**argv){');ft=once(ft,'if(s=="replay")','if(s=="event20")Event20();else if(s=="event20-continuity")Event20Continuity();else if(s=="replay")');generated=out/'fixture.cpp';generated.write_text(ft)
 env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0')
 def compile_case(folder):
  cmd=driver.compile_command(folder,generated,a.sanitize);r=subprocess.run(cmd,capture_output=True,text=True,timeout=120);(folder/'compile.stdout').write_text(r.stdout);(folder/'compile.stderr').write_text(r.stderr)
  if r.returncode:raise RuntimeError('compile failure is not mutation evidence: '+str(folder))
 def run(folder,selection):return subprocess.run([str(folder/'run-test'),selection],capture_output=True,text=True,timeout=20,env=env)
 def copy(folder):
  folder.mkdir(parents=True,exist_ok=False)
  for x in base.iterdir():
   if x.suffix in ('.h','.hpp','.inc'):shutil.copy2(x,folder/x.name)
 def positive(folder):
  compile_case(folder)
  for selection in ['event20','event20-continuity','complete']:
   r=run(folder,selection)
   if r.returncode or r.stdout or r.stderr:raise RuntimeError('positive failure '+selection+': '+r.stderr)
 positive(base);old=out/'accepted-old-guards';copy(old);(old/'owner.inc').write_text(replace_region((base/'owner.inc').read_text(),OLD));positive(old)
 results=[]
 if not a.no_mutations:
  for name,body,label in MUTATIONS:
   folder=out/'mutations'/name;copy(folder);(folder/'owner.inc').write_text(replace_region((base/'owner.inc').read_text(),body));compile_case(folder);r=run(folder,'event20');good=intended(r,label);results.append({'name':name,'expected_check':label,'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr,'accepted':good});(out/'mutations.json').write_text(json.dumps(results,indent=2)+'\n')
   if not good:raise RuntimeError('wrong mutation outcome '+name+': '+r.stderr)
 if any(sha(Path(n))!=h for n,h in inputs.items()):raise RuntimeError('input changed during test')
 manifest.update(status='PASS',source_inputs_sha256=inputs,structural_controls=negatives,classifier_rejections=classifier_count,valid_callback_cases_per_form=75,old_guarded_valid_positives='PASS; structural-only difference, no null runtime counterexample',continuity_and_original_complete_cases='PASS both forms',mutations=results,sanitize=a.sanitize,limits=['Synthetic creation/advance callbacks, live objects only; no deletion callback seam','Host timer/lifecycle/SDK/outcall seams retained','No null-member execution, native ABI, runtime-storage closure or exactness claim'])
 (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');print('PASS opcode20 valid callbacks, original complete-stage, dispatcher continuity, old guarded form; strict mutants '+str(len(results)))
if __name__=='__main__':main()
