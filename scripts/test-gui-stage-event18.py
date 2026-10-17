#!/usr/bin/env python3
"""Opcode18 valid-object actual-Run checks. Synthetic music/create callbacks;
null skip removal is checked structurally, never by null C++ member calls.
No native ABI/storage/exactness or actual callback mutation claim.
"""
from pathlib import Path
import argparse,hashlib,importlib.util,json,os,re,shutil,subprocess,sys
EXPECTED='''case GUI_MSG_EVENT_18:
            FrontEndPlayMusic(1, g_GameStageUpdate->value28);
            {
                AnmLoadedView *loaded = g_GuiView->stageUiAnm;
                loaded->CreateVmVariant0(2, GUI_VM_RENDER_LAYER).value;
            }
            break;'''
OLD='''case GUI_MSG_EVENT_18:
            FrontEndPlayMusic(1, g_GameStageUpdate->value28);
            if (g_GuiView != NULL)
                {
                    AnmLoadedView *loaded = g_GuiView->stageUiAnm;
                    if (loaded != NULL)
                        loaded->CreateVmVariant0(2, GUI_VM_RENDER_LAYER).value;
                }
            break;'''
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def once(s,a,b):
 if s.count(a)!=1:raise RuntimeError('anchor count: '+a)
 return s.replace(a,b)
def region(s):
 a='        case GUI_MSG_EVENT_18:';b='        case GUI_MSG_ENEMY_NAME:'
 if s.count(a)!=1 or s.count(b)!=1:raise RuntimeError('opcode boundary count')
 start=s.index(a);end=s.index(b,start);return start,end,s[start:end]
def replace_region(s,new):
 a,b,_=region(s);return s[:a]+'        '+new+'\n\n'+s[b:]
def shape(s):
 if re.sub(r'\s+','',region(s)[2])!=re.sub(r'\s+','',EXPECTED):raise RuntimeError('opcode18 structure mismatch')
def intended(r,label):return r.returncode==1 and r.stdout=='' and r.stderr=='CHECK '+label+'\n'
def classifier():
 assert intended(subprocess.CompletedProcess([],1,'','CHECK x\n'),'x')
 for v in [(-11,'','CHECK x\n'),(139,'','CHECK x\n'),(0,'','CHECK x\n'),(2,'','CHECK x\n'),(1,'x','CHECK x\n'),(1,'','CHECK other\n'),(1,'','compiler error'),(1,'','CHECK x\nextra'),(1,'',''),(1,'','CHECK x'),(1,'','CHECK x\nAddressSanitizer: error\n')]:
  if intended(subprocess.CompletedProcess([],*v),'x'):raise RuntimeError('failure classifier weakened')
 return 11
CASES=r'''
static void Event18(){
 for(int mode:{0,1,2})for(int id:{0,-1,INT_MIN,INT_MAX,137}){
  Fixture f;std::unique_ptr<GuiView> replacement(new GuiView);GuiMessageVmView replacementMessage(nullptr);
  replacement->stageUiAnm=&f.enemyResource;replacement->frontAnm=&f.playerResource;replacement->message=&replacementMessage;
  for(int i=0;i<6;++i)replacementMessage.vmIds[i].value=901+i;
  f.gui->message=&f.message;f.gui->frontAnm=&f.playerResource;
  f.message.flags=0x40;f.message.unknown58=0x1357;f.message.activeSide=1;f.message.alternatingLine=2;
  f.Program(18);host.id=id;int callbacks=0;AnmLoadedView*selected=&f.enemyResource;
  host.afterMusic=[&](){++callbacks;Check(host.creations.empty(),"event18.music_first");
   if(mode==0)f.gui->stageUiAnm=selected;
   else {g_GuiView=replacement.get();if(mode==2){replacement->stageUiAnm=&f.playerResource;selected=&f.playerResource;}}
   f.descriptor.value28=7;
  };
  host.afterCreate=[&](){Check(callbacks==1,"event18.music_callback");for(int i=0;i<6;++i)Check(f.message.vmIds[i].value==101+i,"event18.no_early_publication");g_GuiView=f.gui.get();f.gui->message=&replacementMessage;};
  Normal(f);
  Check(callbacks==1&&host.order==std::vector<std::string>{"music","create"}&&host.creations.size()==1,"event18.once_order");
  Check(host.music==std::vector<std::pair<int,int>>{{2,1}}&&f.profile[0x1d892+23]==1&&f.profile[0x1d892+7]==0xa5,"event18.music_arguments");
  Check(host.creations[0].resource==selected,"event18.postmusic_resource");
  Check(host.creations[0].script==2&&host.creations[0].layer==15,"event18.arguments");
  for(int i=0;i<6;++i){Check(f.message.vmIds[i].value==101+i,"event18.discard_result");Check(replacementMessage.vmIds[i].value==901+i,"event18.replacement_message_unchanged");}
  Check(f.message.flags==0x40&&f.message.unknown58==0x1357&&f.message.activeSide==1&&f.message.alternatingLine==2,"event18.sentinels");
  host.afterMusic={};host.afterCreate={};g_GuiView=f.gui.get();
 }
}
static void Event18Continuity(){
 Fixture f;f.first=f.Append(18);f.Append(11);auto other=f.Append(24);f.future=f.Append(0,{},100);f.message.currentInstruction=f.Instruction();
 host.afterCreate=[&](){f.message.currentInstruction=f.bytes.data()+other;};Normal(f);
 Check(f.message.inputCooldown==0&&host.order==std::vector<std::string>{"music","create"},"event18.retained_dispatcher_reload");
}
'''
CALL='loaded->CreateVmVariant0(2, GUI_VM_RENDER_LAYER).value;'
MUSIC='FrontEndPlayMusic(1, g_GameStageUpdate->value28);'
LOAD='AnmLoadedView *loaded = g_GuiView->stageUiAnm;'
MUTATIONS=[
 ('script',EXPECTED.replace('(2, GUI_VM_RENDER_LAYER)','(3, GUI_VM_RENDER_LAYER)'),'event18.arguments'),
 ('layer',EXPECTED.replace('(2, GUI_VM_RENDER_LAYER)','(2, 14)'),'event18.arguments'),
 ('wrong-resource',EXPECTED.replace('g_GuiView->stageUiAnm','g_GuiView->frontAnm'),'event18.postmusic_resource'),
 ('music-slot',EXPECTED.replace('FrontEndPlayMusic(1,','FrontEndPlayMusic(0,'),'event18.music_arguments'),
 ('music-track',EXPECTED.replace('g_GameStageUpdate->value28','g_GameStageUpdate->musicIndex'),'event18.music_arguments'),
 ('omit-music',EXPECTED.replace(MUSIC,''),'event18.music_callback'),
 ('late-music',EXPECTED.replace(MUSIC,'').replace(CALL,CALL+'\n                '+MUSIC),'event18.music_callback'),
 ('preload-resource',EXPECTED.replace('case GUI_MSG_EVENT_18:','case GUI_MSG_EVENT_18: {').replace('break;','break; }').replace(MUSIC,LOAD+'\n            '+MUSIC).replace('                '+LOAD,''),'event18.postmusic_resource'),
 ('preload-gui',EXPECTED.replace('case GUI_MSG_EVENT_18:','case GUI_MSG_EVENT_18: {').replace('break;','break; }').replace(MUSIC,'GuiView *captured = g_GuiView;\n            '+MUSIC).replace('g_GuiView->stageUiAnm','captured->stageUiAnm'),'event18.postmusic_resource'),
 ('publish-result',EXPECTED.replace(CALL,'message->vmIds[0].value = '+CALL),'event18.discard_result'),
 ('duplicate-create',EXPECTED.replace(CALL,CALL+'\n                '+CALL),'event18.once_order'),
 ('omit-create',EXPECTED.replace(CALL,''),'event18.once_order'),
 ('early-return',EXPECTED.replace('break;','return 0;'),'normal.advance'),
]
def main():
 p=argparse.ArgumentParser();p.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[1]);p.add_argument('--out',type=Path,required=True);p.add_argument('--sanitize',action='store_true');p.add_argument('--no-mutations',action='store_true');a=p.parse_args()
 if os.path.lexists(a.out):p.error('output must be fresh, including dangling symlinks')
 root=a.root.resolve();out=a.out.resolve();source=root/'src/Gui.cpp';text=source.read_text();shape(text);assert len(MUTATIONS)==13 and len({n for n,_,_ in MUTATIONS})==13
 negatives=[]
 for n,body in [('owner-skip',EXPECTED.replace('{','if (g_GuiView != NULL)\n            {',1)),('resource-skip',EXPECTED.replace(CALL,'if (loaded != NULL) '+CALL)),('accepted-old-two-skips',OLD)]:
  try:shape(replace_region(text,body))
  except RuntimeError as e:
   if str(e)!='opcode18 structure mismatch':raise
  else:raise RuntimeError('structural control escaped')
  negatives.append(n)
 classifier_count=classifier();sys.dont_write_bytecode=True;sys.path.insert(0,str(root/'scripts'));driverpath=root/'scripts/test-gui-message-run.py';fixture=root/'scripts/fixtures/gui-message-run.cpp'
 inputs={str(x):sha(x) for x in [source,driverpath,fixture,root/'scripts/gui_message_run_mutations.py',Path(__file__).resolve()]}
 spec=importlib.util.spec_from_file_location('event18_driver',driverpath);driver=importlib.util.module_from_spec(spec);spec.loader.exec_module(driver)
 out.mkdir(parents=True,exist_ok=False);base=out/'positive';manifest=driver.prepare(root,None,base,False);inputs.update(manifest['source_inputs_sha256'])
 ft=fixture.read_text();ft=once(ft,'afterSprite,afterDecode,afterDraw,afterCreate,afterEffect,afterTransition','afterSprite,afterDecode,afterDraw,afterCreate,afterEffect,afterTransition,afterMusic');ft=once(ft,'host.order.push_back("music");}','host.order.push_back("music");if(host.afterMusic)host.afterMusic();}');ft=once(ft,'int main(int argc,char**argv){',CASES+'\nint main(int argc,char**argv){');ft=once(ft,'if(s=="replay")','if(s=="event18")Event18();else if(s=="event18-continuity")Event18Continuity();else if(s=="replay")');generated=out/'fixture.cpp';generated.write_text(ft)
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
  for selection in ['event18','event18-continuity','music']:
   r=run(folder,selection)
   if r.returncode or r.stdout or r.stderr:raise RuntimeError('positive failure '+selection+': '+r.stderr)
 positive(base);old=out/'accepted-old-guards';copy(old);(old/'owner.inc').write_text(replace_region((base/'owner.inc').read_text(),OLD));positive(old)
 results=[]
 if not a.no_mutations:
  for name,body,label in MUTATIONS:
   folder=out/'mutations'/name;copy(folder);(folder/'owner.inc').write_text(replace_region((base/'owner.inc').read_text(),body));compile_case(folder);r=run(folder,'event18');good=intended(r,label);results.append({'name':name,'expected_check':label,'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr,'accepted':good});(out/'mutations.json').write_text(json.dumps(results,indent=2)+'\n')
   if not good:raise RuntimeError('wrong mutation outcome '+name+': '+r.stderr)
 if any(sha(Path(n))!=h for n,h in inputs.items()):raise RuntimeError('input changed during test')
 manifest.update(status='PASS',source_inputs_sha256=inputs,structural_controls=negatives,classifier_rejections=classifier_count,valid_callback_cases_per_form=15,old_guarded_valid_positives='PASS; structural-only difference, no null runtime counterexample',continuity_and_original_music_cases='PASS both forms',mutations=results,sanitize=a.sanitize,limits=['Synthetic callbacks, live objects only','Host timer/lifecycle/SDK/outcall seams retained','No null-member execution, native ABI, runtime-storage closure or exactness claim'])
 (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');print('PASS opcode18 valid callbacks, original music, dispatcher continuity, old guarded form; strict mutants '+str(len(results)))
if __name__=='__main__':main()
