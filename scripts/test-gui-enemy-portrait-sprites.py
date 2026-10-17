#!/usr/bin/env python3
"""Local opcode13 caller-contract tests using complete maintained Run host types.
No native/PE/ABI claim. Generated fixture/dependency observers are synthetic;
maintained finder traversal and setter resolution remain intact. Fresh output only.
"""
from pathlib import Path
import argparse, hashlib, importlib.util, json, os, re, shutil, subprocess, sys

START = '        case GUI_MSG_ENEMY_PORTRAIT_SPRITES:'
END = '        case GUI_MSG_TEXT_LINE_0:'
# Audited caller contracts; values are independent of source being tested.
CONTEXTS = [(1,0,0x18,0x1b,0x0f,0x17),(2,0,0x18,0x1b,0x15,0x1e),
 (3,0,0x18,0x1b,0x22,0x2b),(4,0,0x18,0x1b,0x25,0x2e),
 (5,0,0x18,0x1b,0x16,0x1f),(6,0,0x1c,0x1f,0x30,0x39),
 (7,0,0x21,0x24,0x44,0x4d),(7,1,0x18,0x1b,0x2e,0x37)]
ARG = 'GuiMessageInstructionIntArgument(message->currentInstruction)'
RESOURCE = 'static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2])'
PARENT = '&message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT]'

def named_pair(row):
    _,_,a,b,x,y = row
    return f'''{{
const int first = {ARG} + {x:#x};
AnmLoadedView *resource = {RESOURCE};
AnmVmIdView child0 = ResultMenuFindScript({PARENT}, {a:#x});
child0.SetSpriteWithAnm(resource, first);
resource = {RESOURCE};
const int second = {ARG} + {y:#x};
AnmVmIdView child1 = ResultMenuFindScript({PARENT}, {b:#x});
child1.SetSpriteWithAnm(resource, second);
break;
}}'''

def pair(row, named=False):
    text = named_pair(row)
    if named:
        return text
    return re.sub(r'AnmVmIdView child([01]) = (ResultMenuFindScript\([^;]+\));\nchild\1\.SetSpriteWithAnm\(resource, (first|second)\);',
                  lambda m: m[2] + '.SetSpriteWithAnm(resource, ' + m[3] + ');', text)

def named_body(body):
    # Equivalent handle materialization makes post-finder mutant reads sequenced.
    pattern = r'(ResultMenuFindScript\([^;]+\))\.SetSpriteWithAnm\(resource, (first|second)\);'
    def expand(m):
        name = 'child0' if m[2] == 'first' else 'child1'
        return 'AnmVmIdView ' + name + ' = ' + m[1] + ';\n                        ' + name + '.SetSpriteWithAnm(resource, ' + m[2] + ');'
    changed, count = re.subn(pattern, expand, body)
    if count != 32:
        raise RuntimeError('named equivalent inventory changed')
    return changed

def expected(grouped=False, named=False):
    def chars(row):
        p = pair(row, named)
        return 'switch (g_GuiCharacter) {' + ('case 0: case 1:' + p if grouped else 'case 0:' + p + 'case 1:' + p) + '}'
    s = START + '\n{switch (g_GuiStage) {'
    for row in CONTEXTS[:6]:
        s += f'case {row[0]}: {{' + chars(row) + 'break;}'
    return s + 'case 7: {if (message->messageIndex >= 1) {' + chars(CONTEXTS[7]) + '} else {' + chars(CONTEXTS[6]) + '}break;}}break;}\n\n'

def region(text):
    if text.count(START) != 1 or text.count(END) != 1:
        raise RuntimeError('opcode13 source boundary inventory changed')
    a,b = text.index(START),text.index(END)
    return a,b,text[a:b]

def structure(text):
    if re.sub(r'\s+', '', region(text)[2]) != re.sub(r'\s+', '', expected()):
        raise RuntimeError('opcode13 structural contract mismatch')

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def exact(text, old, new, count=1):
    if text.count(old) != count:
        raise RuntimeError('exact adaptation inventory changed: ' + old[:90])
    return text.replace(old,new)

# These observers do not replace production types or lookup/setting algorithms.
# Callbacks at finder return and existing resource-setter outcall are synthetic
# dependency effects, not claims that actual callees mutate these owners.
OBSERVER = r'''
struct O13Find { AnmVmIdView *address; int parent, script, child; };
struct O13Set { int child; AnmLoadedView *resource; int index; };
struct O13State {
 bool active=false; AnmVmIdView *parent=nullptr; int expectedParent=102;
 std::vector<O13Find> finds; std::vector<O13Set> sets;
 std::vector<std::string> order; std::function<void(int)> afterFind;
};
static O13State o13;
static void O13BeforeSet(AnmVmIdView *id, AnmLoadedView *resource, int index) {
 if (!o13.active) return;
 Check(o13.parent->value==o13.expectedParent,"op13.no_publication");
 o13.order.push_back("set"); o13.sets.push_back({id->value,resource,index});
}
'''
FINDER = r'''
AnmVmIdView ResultMenuFindScript(AnmVmIdView *id,int script) {
 if (!o13.active) return O13MaintainedFind(id,script);
 Check(id==o13.parent,"op13.parent_address");
 Check(id->value==o13.expectedParent,"op13.no_publication");
 // Valid-parent prerequisite is checked before entering maintained traversal.
 Check(g_AnmRenderManagerView->FindVm(id->value)!=nullptr,"op13.live_parent");
 o13.order.push_back("find");
 o13.finds.push_back({id,id->value,script,0});
 AnmVmIdView result=O13MaintainedFind(id,script);
 o13.finds.back().child=result.value;
 if(o13.afterFind)o13.afterFind(static_cast<int>(o13.finds.size()));
 return result;
}
'''
WITNESSES = r'''
struct O13Context {int stage,index,firstScript,secondScript,firstOffset,secondOffset;};
static const O13Context o13Contexts[] = {
 {1,0,0x18,0x1b,0xf,0x17},{2,0,0x18,0x1b,0x15,0x1e},
 {3,0,0x18,0x1b,0x22,0x2b},{4,0,0x18,0x1b,0x25,0x2e},
 {5,0,0x18,0x1b,0x16,0x1f},{6,0,0x1c,0x1f,0x30,0x39},
 {7,0,0x21,0x24,0x44,0x4d},{7,1,0x18,0x1b,0x2e,0x37}};
static void O13Begin(Fixture &f,const O13Context &x,int character,int payload=10) {
 o13=O13State();o13.active=true;o13.parent=&f.message.vmIds[1];
 g_GuiStage=x.stage;g_GuiCharacter=character;f.message.messageIndex=x.index;
 f.Children(1,x.firstScript,x.secondScript);f.Program(13,Fixture::Int(payload));
 f.message.unknown58=0x1357;f.message.flags=0x40;f.message.activeSide=1;f.message.alternatingLine=2;
}
static void O13Run(Fixture &f,unsigned char *next) {
 Check(GuiMessageVmView::Run(&f.message)==0,"op13.result");
 Check(f.message.currentInstruction==next,"op13.advance");
 Check(f.message.instructionTimer.current==1&&f.message.instructionTimer.previous==0&&f.message.lifetimeTimer.current==0,"op13.timers");
 Check(f.message.vmIds[1].value==o13.expectedParent,"op13.no_publication");
 for(int i=0;i<6;++i)if(i!=1)Check(f.message.vmIds[i].value==101+i,"op13.other_ids");
 Check(f.message.unknown58==0x1357&&f.message.flags==0x40&&f.message.activeSide==1&&f.message.alternatingLine==2,"op13.sentinels");
 Check(host.creations.empty()&&host.draws.empty(),"op13.no_creation");
}
static void O13Expect(Fixture &f,const O13Context &x,AnmLoadedView *r0,int n0,AnmLoadedView *r1,int n1,int secondChild=108,int secondParent=102) {
 Check(o13.finds.size()==2&&o13.sets.size()==2&&host.sprites.size()==2,"op13.call_count");
 Check(o13.order==std::vector<std::string>{"find","set","find","set"}&&host.order==std::vector<std::string>{"sprite","sprite"},"op13.order");
 Check(o13.finds[0].script==x.firstScript&&o13.finds[1].script==x.secondScript,"op13.scripts");
 Check(o13.finds[0].address==o13.parent&&o13.finds[1].address==o13.parent&&o13.finds[0].parent==102&&o13.finds[1].parent==secondParent,"op13.parent_values");
 Check(o13.finds[0].child==107&&o13.finds[1].child==secondChild&&o13.sets[0].child==107&&o13.sets[1].child==secondChild&&host.sprites[0].vm==&f.vms[6]&&host.sprites[1].vm==&f.vms[secondChild-101],"op13.child_identity");
 Check(o13.sets[0].resource==r0&&host.sprites[0].resource==r0,"op13.first_resource");
 Check(o13.sets[0].index==n0+x.firstOffset&&host.sprites[0].index==n0+x.firstOffset,"op13.first_payload");
 Check(o13.sets[1].resource==r1&&host.sprites[1].resource==r1,"op13.second_resource");
 Check(o13.sets[1].index==n1+x.secondOffset&&host.sprites[1].index==n1+x.secondOffset,"op13.second_payload");
}
static void O13Static() {
 for(const auto &x:o13Contexts)for(int c:{0,1})for(int n:{INT_MIN,-123,-1,0,1,123,INT_MAX-0x4d}) {
  Fixture f;O13Begin(f,x,c,n);auto profile=f.profile;O13Run(f,f.Future());
  O13Expect(f,x,&f.enemyResource,n,&f.enemyResource,n);
  Check(f.message.messageIndex==x.index&&f.profile==profile,"op13.input_sentinels");
 }o13.active=false;
}
static void O13Guards() {
 for(int c:{INT_MIN,-123,-1,0,1,2,123,INT_MAX})
 for(int stage:{INT_MIN,-123,-1,0,1,2,3,4,5,6,7,8,123,INT_MAX})
 for(int index:{INT_MIN,-123,-1,0,1,2,123,INT_MAX}) {
  Fixture f;auto x=o13Contexts[stage>=1&&stage<=7?stage-1:0];if(stage==7&&index>=1)x=o13Contexts[7];
  x.stage=stage;x.index=index;O13Begin(f,x,c);O13Run(f,f.Future());
  if(stage>=1&&stage<=7&&(c==0||c==1))O13Expect(f,x,&f.enemyResource,10,&f.enemyResource,10);
  else Check(o13.finds.empty()&&o13.sets.empty()&&host.sprites.empty()&&host.order.empty(),"op13.guard");
 }o13.active=false;
}
static void O13Callbacks() {
 for(const auto &x:o13Contexts)for(int c:{0,1})for(int mode=0;mode<13;++mode) {
  Fixture f;O13Begin(f,x,c);std::unique_ptr<EnemyManagerView> replacement(new EnemyManagerView);
  replacement->effectResources[2]=&f.otherResource;
  auto other=f.bytes,third=f.bytes;unsigned char *next=f.Future();
  AnmLoadedView *secondResource=&f.enemyResource;int secondPayload=10,secondChild=108,secondParent=102;
  auto payload=[](unsigned char *p,int n){std::memcpy(p+4,&n,4);};
  auto change=[&](int at){
   if((mode==0&&at==0)||(mode==1&&at==1)) {
    f.enemy.effectResources[2]=&f.otherResource;payload(f.Instruction(),20);secondResource=&f.otherResource;secondPayload=20;
   }
   if((mode==2&&at==0)||(mode==3&&at==1)) {
    g_EnemyManager=replacement.get();payload(other.data(),30);f.message.currentInstruction=other.data();next=other.data()+f.future;secondResource=&f.otherResource;secondPayload=30;
   }
   if((mode==4&&at==0)||(mode==5&&at==1)) {
    f.message.vmIds[1].value=f.vms[9].id;o13.expectedParent=f.vms[9].id;
    f.vms[10].scriptIndex=static_cast<short>(x.secondScript);f.vms[9].layerNode.next=&f.vms[10].layerNode;
    secondChild=f.vms[10].id;secondParent=f.vms[9].id;
   }
   if((mode==6&&at==0)||(mode==7&&at==1)) {
    f.vms[7].scriptIndex=-1;f.vms[8].scriptIndex=static_cast<short>(x.secondScript);f.vms[7].layerNode.next=&f.vms[8].layerNode;secondChild=f.vms[8].id;
   }
   if(mode==8&&at==2) {
    g_EnemyManager=replacement.get();payload(other.data(),40);f.message.currentInstruction=other.data();next=other.data()+f.future;
   }
   if(mode==9) {
    if(at==0){f.enemy.effectResources[2]=&f.playerResource;payload(f.Instruction(),20);}
    if(at==1){g_EnemyManager=replacement.get();payload(other.data(),30);f.message.currentInstruction=other.data();next=other.data()+f.future;secondResource=&f.otherResource;secondPayload=30;}
    if(at==2){replacement->effectResources[2]=&f.playerResource;payload(other.data(),40);payload(third.data(),50);f.message.currentInstruction=third.data();next=third.data()+f.future;}
   }
   if((mode==10&&at==0)||(mode==11&&at==1)) {g_GuiStage=8;g_GuiCharacter=-1;f.message.messageIndex=x.index>=1?0:1;}
   if(mode==12&&at==2) {f.message.vmIds[1].value=f.vms[9].id;o13.expectedParent=f.vms[9].id;f.vms[7].scriptIndex=-1;f.vms[8].scriptIndex=static_cast<short>(x.secondScript);f.vms[7].layerNode.next=&f.vms[8].layerNode;}
  };
  o13.afterFind=[&](int n){change(n==1?0:2);};
  host.afterSprite=[&](){if(host.sprites.size()==1)change(1);};
  // next may change during Run, so check after callbacks have finished.
  Check(GuiMessageVmView::Run(&f.message)==0,"op13.result");
  Check(f.message.currentInstruction==next,"op13.advance");
  Check(f.message.vmIds[1].value==o13.expectedParent,"op13.no_publication");
  for(int i=0;i<6;++i)if(i!=1)Check(f.message.vmIds[i].value==101+i,"op13.other_ids");
  O13Expect(f,x,&f.enemyResource,10,secondResource,secondPayload,secondChild,secondParent);
  o13.afterFind={};host.afterSprite={};
 }o13.active=false;
}
'''


def fixture_text(original):
    text = exact(original, '#include "dependencies.inc"', OBSERVER + '\n#include "dependencies.inc"\n' + FINDER)
    text = exact(text, 'int main(int argc,char**argv)', WITNESSES + '\nint main(int argc,char**argv)')
    return exact(text, 'if(s=="replay")ReplayModes();', 'if(s=="op13_static")O13Static();else if(s=="op13_guards")O13Guards();else if(s=="op13_callbacks")O13Callbacks();else if(s=="replay")ReplayModes();')

def dependency_text(original):
    text = exact(original, 'AnmVmIdView ResultMenuFindScript(AnmVmIdView *id, int scriptIndex)', 'AnmVmIdView O13MaintainedFind(AnmVmIdView *id, int scriptIndex)')
    return exact(text, 'void AnmVmIdView::SetSpriteWithAnm(AnmLoadedView *loaded, int spriteIndex)\n{', 'void AnmVmIdView::SetSpriteWithAnm(AnmLoadedView *loaded, int spriteIndex)\n{\n    O13BeforeSet(this, loaded, spriteIndex);')

# Predeclared expected CHECK classification. All replacements are restricted to
# opcode13's generated owner region. No crash or compiler failure is a detection.
MUTATIONS = [
 ('first_script', ', 0x18);', ', 0x7f);', 12, 'op13_static','op13.call_count'),
 ('second_script', ', 0x1b);', ', 0x7f);', 12, 'op13_static','op13.call_count'),
 ('first_offset', ' + 0xf;', ' + 0xe;', 2, 'op13_static','op13.first_payload'),
 ('second_offset', ' + 0x17;', ' + 0x16;', 2, 'op13_static','op13.second_payload'),
 ('wrong_resource', RESOURCE, 'static_cast<AnmLoadedView *>(g_Player->resource)', 32, 'op13_static','op13.first_resource'),
 ('wrong_parent', PARENT, '&message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT]',32,'op13_static','op13.parent_address'),
 ('first_child_receiver','child0.SetSpriteWithAnm(resource, first);','AnmVmIdView(108).SetSpriteWithAnm(resource, first);',16,'op13_static','op13.child_identity'),
 ('second_child_receiver','child1.SetSpriteWithAnm(resource, second);','child0.SetSpriteWithAnm(resource, second);',16,'op13_static','op13.child_identity'),
 ('skip_first_set','child0.SetSpriteWithAnm(resource, first);','/* omitted setter */',16,'op13_static','op13.call_count'),
 ('duplicate_first_set','child0.SetSpriteWithAnm(resource, first);','child0.SetSpriteWithAnm(resource, first); child0.SetSpriteWithAnm(resource, first);',16,'op13_static','op13.call_count'),
 ('publish_first','child0.SetSpriteWithAnm(resource, first);','message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value=child0.value; child0.SetSpriteWithAnm(resource, first);',16,'op13_static','op13.no_publication'),
 ('publish_second','child1.SetSpriteWithAnm(resource, second);','child1.SetSpriteWithAnm(resource, second); message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value=child1.value;',16,'op13_static','op13.no_publication'),
 ('other_id','child0.SetSpriteWithAnm(resource, first);','message->vmIds[0].value=0; child0.SetSpriteWithAnm(resource, first);',16,'op13_static','op13.other_ids'),
 ('character_guard','case 0:', 'case -1: case 0:',8,'op13_guards','op13.guard'),
 ('stage_guard','switch (g_GuiStage)', 'switch (g_GuiStage==0?1:g_GuiStage)',1,'op13_guards','op13.guard'),
 ('signed_threshold','message->messageIndex >= 1','static_cast<unsigned>(message->messageIndex) >= 1',1,'op13_guards','op13.call_count'),
 ('index_boundary','message->messageIndex >= 1','message->messageIndex >= 0',1,'op13_guards','op13.call_count'),
 ('first_recapture_resource','child0.SetSpriteWithAnm(resource, first);',f'child0.SetSpriteWithAnm({RESOURCE}, first);',16,'op13_callbacks','op13.first_resource'),
 ('first_recapture_payload','child0.SetSpriteWithAnm(resource, first);',f'child0.SetSpriteWithAnm(resource, first + {ARG} - 10);',16,'op13_callbacks','op13.first_payload'),
 ('second_cached_resource',f'                        resource = {RESOURCE};', '                        resource = resource;',16,'op13_callbacks','op13.second_resource'),
 ('second_stale_instruction',f'const int second = {ARG}', 'const int second = GuiMessageInstructionIntArgument(instruction)',16,'op13_callbacks','op13.second_payload'),
 ('second_recapture_resource','child1.SetSpriteWithAnm(resource, second);',f'child1.SetSpriteWithAnm({RESOURCE}, second);',16,'op13_callbacks','op13.second_resource'),
 ('second_recapture_payload','child1.SetSpriteWithAnm(resource, second);','SECOND_RECAPTURE',16,'op13_callbacks','op13.second_payload'),
 ('find_hoisted','child0.SetSpriteWithAnm(resource, first);','HOIST_FIND',16,'op13_static','op13.order'),
 ('redispatch_character','child0.SetSpriteWithAnm(resource, first);','child0.SetSpriteWithAnm(resource, first); if(g_GuiCharacter!=0&&g_GuiCharacter!=1) break;',16,'op13_callbacks','op13.call_count'),
]
EXPECTED_MUTATIONS=25



def mutate(body,old,new,count,name):
    # Each mutant starts from actual chained source. Only the selected expression
    # is materialized where explicit sequencing or a reused returned ID is needed.
    if name in ('first_script','second_script'):
        return exact(body,old[:-1]+'.',new[:-1]+'.',count)
    direct = {'first_offset','second_offset','wrong_resource','wrong_parent',
              'character_guard','stage_guard','signed_threshold','index_boundary',
              'second_cached_resource','second_stale_instruction'}
    if name in direct:
        return exact(body,old,new,count)
    call = r'(ResultMenuFindScript\([^;]+\))\.SetSpriteWithAnm\(resource, (first|second)\);'
    if name == 'find_hoisted':
        pattern = r'(ResultMenuFindScript\([^;]+\))\.SetSpriteWithAnm\(resource, first\);([\s\S]*?)(ResultMenuFindScript\([^;]+\))\.SetSpriteWithAnm\(resource, second\);'
        def hoist(m):
            return ('AnmVmIdView mutationFirst = '+m[1]+';\nAnmVmIdView mutationSecond = '+m[3]+
                    ';\nmutationFirst.SetSpriteWithAnm(resource, first);'+m[2]+
                    'mutationSecond.SetSpriteWithAnm(resource, second);')
        changed,n=re.subn(pattern,hoist,body)
    else:
        phase = 'second' if name in {'second_child_receiver','publish_second',
                                     'second_recapture_resource','second_recapture_payload'} else 'first'
        def replace(m):
            find,value=m[1],m[2]
            if value != phase:return m[0]
            setter='mutationChild.SetSpriteWithAnm(resource, '+value+');'
            local='AnmVmIdView mutationChild = '+find+';\n'
            if name in {'first_child_receiver','second_child_receiver'}:
                return '{ '+local+'AnmVmIdView('+('108' if phase=='first' else '107')+').SetSpriteWithAnm(resource, '+value+'); }'
            if name=='skip_first_set':return find+';'
            if name=='duplicate_first_set':return '{ '+local+setter+' '+setter+' }'
            if name=='publish_first':return '{ '+local+'message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value=mutationChild.value; '+setter+' }'
            if name=='publish_second':return '{ '+local+setter+' message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value=mutationChild.value; }'
            if name=='other_id':return 'message->vmIds[0].value=0; '+m[0]
            if name.endswith('recapture_resource'):
                return '{ '+local+'mutationChild.SetSpriteWithAnm('+RESOURCE+', '+value+'); }'
            if name=='first_recapture_payload':
                return '{ '+local+'mutationChild.SetSpriteWithAnm(resource, first + '+ARG+' - 10); }'
            if name=='second_recapture_payload':
                # Preserve this leaf's independent offset while moving only the read.
                prefix=body[:m.start()]
                offsets=re.findall(r'const int second = '+re.escape(ARG)+r' \+ (0x[0-9a-f]+);',prefix)
                if not offsets:raise RuntimeError('missing second offset')
                return '{ '+local+'mutationChild.SetSpriteWithAnm(resource, '+ARG+' + '+offsets[-1]+'); }'
            if name=='redispatch_character':return m[0]+' if(g_GuiCharacter!=0&&g_GuiCharacter!=1) break;'
            raise RuntimeError('undeclared direct mutation: '+name)
        matches=list(re.finditer(call,body));n=sum(m[2]==phase for m in matches)
        changed=re.sub(call,replace,body)
    if n!=count or changed==body:raise RuntimeError('direct mutation anchor inventory changed: '+name)
    return changed


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root',type=Path,default=Path(__file__).resolve().parents[1]);p.add_argument('--out',type=Path,required=True)
    p.add_argument('--fallback-root',type=Path);p.add_argument('--sanitize',action='store_true');p.add_argument('--no-mutations',action='store_true')
    a=p.parse_args();root,out=a.root.resolve(),a.out.resolve()
    if out.exists():p.error('--out must be fresh and nonexistent')
    if len(MUTATIONS)!=EXPECTED_MUTATIONS or len({m[0] for m in MUTATIONS})!=EXPECTED_MUTATIONS:raise RuntimeError('mutation inventory mismatch')
    source=root/'src/Gui.cpp';source_text=source.read_text();structure(source_text)
    start,end,body=region(source_text)
    controls={'collapsed':expected(True), 'named':expected(named=True), 'added_guard':body.replace('ResultMenuFindScript(', 'if (resource != NULL) ResultMenuFindScript(')}
    for name,replacement in controls.items():
        try:structure(source_text[:start]+replacement+source_text[end:])
        except RuntimeError as e:
            if str(e)!='opcode13 structural contract mismatch':raise
        else:raise RuntimeError(name+' escaped structural rejection')
    sys.path.insert(0,str(root/'scripts'))
    driver_path=root/'scripts/test-gui-message-run.py';spec=importlib.util.spec_from_file_location('op13_run_driver',driver_path)
    d=importlib.util.module_from_spec(spec);spec.loader.exec_module(d);d.classifier()
    base=out/'positive';manifest=d.prepare(root,a.fallback_root,base,False)
    original_fixture=root/'scripts/fixtures/gui-message-run.cpp';fixture=out/'opcode13-fixture.cpp'
    fixture.write_text(fixture_text(original_fixture.read_text()))
    dependencies=base/'dependencies.inc';original_dependencies=dependencies.read_text();dependencies.write_text(dependency_text(original_dependencies))
    manifest.update(driver_sha256=sha(Path(__file__)),run_driver_sha256=sha(driver_path),original_fixture_sha256=sha(original_fixture),run_catalog_sha256=sha(root/'scripts/gui_message_run_mutations.py'),compile_timeout_seconds=120,run_timeout_seconds=15,generated_fixture_sha256=sha(fixture),original_dependencies_sha256=hashlib.sha256(original_dependencies.encode()).hexdigest(),generated_dependencies_sha256=sha(dependencies),sanitize=a.sanitize,
        scope='Portable actual-source caller contract only; no native CFG/ABI/lifetime proof.',
        synthetic_seams='Finder return observer/callback and setter entry observer; existing resource setter callback. Maintained traversal and resolution retained. Callback effects are synthetic, not actual callee behavior.',
        structure='pass',structural_negative_controls=list(controls),cases={'static':112,'guards':896,'synthetic_callbacks':208},mutation_catalog=[dict(name=n,expected_check=ch,selection=sel,anchor_count=k) for n,old,new,k,sel,ch in MUTATIONS],asan_options='detect_leaks=0; no leak claim')
    (out/'predeclared.json').write_text(json.dumps(manifest,indent=2)+'\n')
    environment=dict(os.environ,ASAN_OPTIONS='detect_leaks=0')
    def copy(folder):
        folder.mkdir(parents=True)
        for f in base.iterdir():
            if f.suffix in ('.h','.hpp','.inc'):shutil.copy2(f,folder/f.name)
    def build(folder):
        command=d.compile_command(folder,fixture,a.sanitize)
        r=subprocess.run(command,text=True,capture_output=True,timeout=120)
        (folder/'compile.stdout').write_text(r.stdout);(folder/'compile.stderr').write_text(r.stderr)
        if r.returncode:raise RuntimeError('compile failure is not evidence: '+str(folder))
        return command
    def run(folder,selection):
        r=subprocess.run([str(folder/'run-test'),selection],text=True,capture_output=True,timeout=15,env=environment)
        (folder/(selection+'.stdout')).write_text(r.stdout);(folder/(selection+'.stderr')).write_text(r.stderr)
        return dict(selection=selection,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),r
    def positive(folder):
        command=build(folder);results=[]
        for selection in ['op13_static','op13_guards','op13_callbacks']:
            record,r=run(folder,selection);results.append(record)
            if r.returncode or r.stdout or r.stderr:raise RuntimeError('positive mismatch: '+str(folder)+' '+selection)
        return dict(command=command,compile_returncode=0,runs=results)
    manifest['positive']=positive(base)
    raw=(base/'owner.inc').read_text();start,end,body=region(raw)
    collapsed=out/'collapsed-equivalence';copy(collapsed);(collapsed/'owner.inc').write_text(raw[:start]+expected(True)+raw[end:])
    manifest['collapsed_positive']=positive(collapsed)
    named=out/'named-equivalence';copy(named)
    named_source=named_body(body)
    if re.sub(r'\s+', '', named_source) != re.sub(r'\s+', '', expected(named=True)):
        raise RuntimeError('named positive does not match audited prior form')
    (named/'owner.inc').write_text(raw[:start]+named_source+raw[end:])
    manifest['named_positive']=positive(named)
    manifest['mutation_representation']='All 25 controls mutate actual chained source. Selected expressions alone materialize handles when explicit post-finder sequencing or reuse is required; named correct source separately passes. No control claims a behavioral distinction between named and chained receivers.'
    manifest['collapsed_caveat']='All runtime selections pass; structurally rejected only, not a detected behavioral mutant.'
    results=[]
    if not a.no_mutations:
        for name,old,new,count,selection,check in MUTATIONS:
            folder=out/'mutations'/name;copy(folder)
            mutated=mutate(body,old,new,count,name);(folder/'owner.inc').write_text(raw[:start]+mutated+raw[end:]);build(folder)
            record,r=run(folder,selection);record.update(name=name,expected_check=check,mutant_owner_sha256=sha(folder/'owner.inc'),compile_returncode=0,accepted=d.intended_failure(r,check));results.append(record)
            (out/'mutations.json').write_text(json.dumps(results,indent=2)+'\n')
            if not record['accepted']:raise RuntimeError('incorrect mutation outcome: '+name)
    manifest['mutations']=results
    manifest['source_hashes_still_current']=all(sha(Path(f))==h for f,h in manifest['source_inputs_sha256'].items()) and sha(original_fixture)==manifest['original_fixture_sha256'] and sha(driver_path)==manifest['run_driver_sha256'] and sha(Path(__file__))==manifest['driver_sha256'] and sha(fixture)==manifest['generated_fixture_sha256'] and sha(root/'scripts/gui_message_run_mutations.py')==manifest['run_catalog_sha256']
    if not manifest['source_hashes_still_current']:raise RuntimeError('input changed during test')
    (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print(f'Opcode13: 112 static, 896 guard, 208 synthetic callback cases pass; collapsed runtime positive/structural negative; {len(results)} strict mutants detected.')

if __name__=='__main__':main()
