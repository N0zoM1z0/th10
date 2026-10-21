#!/usr/bin/env python3
"""Complete maintained finalizer body with ordinary logical host containers."""
from pathlib import Path
import argparse,hashlib,json,re,resource,subprocess
R=Path(__file__).resolve().parents[1];ap=argparse.ArgumentParser();ap.add_argument('source',type=Path,nargs='?',default=R/'src/Enemy.cpp');ap.add_argument('--out',type=Path,default=R/'build/host-tests/enemy-finalize-death');ap.add_argument('--mutations',action='store_true');a=ap.parse_args();text=a.source.read_text();m=re.search(r'(?m)^(?:__declspec\(noinline\) )?int __stdcall EnemyFinalizeDeath\(',text);assert m
b=text.index('{',m.start());depth=0;end=None
for i in range(b,len(text)):
 if text[i]=='{':depth+=1
 elif text[i]=='}':
  depth-=1
  if not depth:end=i+1;break
assert end;body=text[m.start():end]
header=r'''
#include <cassert>
#include <vector>
#include <cstring>
#include <cstdio>
#include <new>
#define __stdcall
#define __declspec(x)
struct AnmFloat3View {float x,y,z;};using PlayerFloat3=AnmFloat3View;
struct Motion {PlayerFloat3 position;};
struct EnemyRuntimeView {int deathSoundId,deathEffectScript,deathEffectResourceIndex;Motion worldMotion;int itemDropType;};
struct EnemyFullObjectView {EnemyRuntimeView runtime;};
struct AnmVmIdView {unsigned value;};
struct AnmLoadedView {int identity;AnmVmIdView CreateVmAtWorldVariant0(int,const AnmFloat3View*);};
struct ItemManagerView {int identity;int Spawn(const AnmFloat3View*,int,unsigned,float,float);};
struct EnemyManagerView {void*effectResources[4];};
struct EnemySoundQueueView {void QueueSoundCue(int,float);};
struct GameScoreStateView {void ExtendFaithTimer(int);};
alignas(EnemySoundQueueView) unsigned char g_MainSoundOwner[0x52d0],g_OtherSoundOwner[0x52d0];
static GameScoreStateView g_GameScoreState;
static ItemManagerView itemA{1},itemB{2};static ItemManagerView*g_ItemManager;
static AnmLoadedView effects[8]={{0},{1},{2},{3},{4},{5},{6},{7}};
static EnemyManagerView enemyA,enemyB;static EnemyManagerView*g_EnemyManager;
static EnemyFullObjectView*current;static std::vector<int>events;
static bool changeSound,changeEffect,spawned;static int leadingSeen;
static void checkPosition(const AnmFloat3View*p){assert(p==&current->runtime.worldMotion.position);}
void EnemySoundQueueView::QueueSoundCue(int id,float x){
 assert(reinterpret_cast<void*>(this)==static_cast<void*>(g_MainSoundOwner));assert(events.empty());
 assert(id==current->runtime.deathSoundId&&x==current->runtime.worldMotion.position.x);events.push_back(1);
 if(changeSound){current->runtime.deathEffectScript=17;current->runtime.deathEffectResourceIndex=2;current->runtime.worldMotion.position={71,72,73};g_EnemyManager=&enemyB;}
}
AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant0(int script,const AnmFloat3View*p){
 assert(this==static_cast<AnmLoadedView*>(g_EnemyManager->effectResources[current->runtime.deathEffectResourceIndex]));
 assert(script==current->runtime.deathEffectScript);checkPosition(p);events.push_back(2);
 if(changeEffect){current->runtime.itemDropType=15;current->runtime.worldMotion.position={81,82,83};g_ItemManager=&itemB;}
 return {0x12345678};
}
int ItemManagerView::Spawn(const AnmFloat3View*p,int kind,unsigned color,float angle,float speed){
 assert(this==g_ItemManager);checkPosition(p);assert(kind==current->runtime.itemDropType&&kind>0);
 assert(color==0xffffffffu&&angle== -1.5707964f&&speed==2.2f);events.push_back(3);spawned=true;
 current->runtime.itemDropType=99;current->runtime.worldMotion.position={91,92,93};return 0;
}
void EnemyDropItemCountsCore(const PlayerFloat3*p,int*block){
 assert(block==&current->runtime.itemDropType);checkPosition(p);assert(!spawned||*block==99);
 leadingSeen=*block;*block=111;events.push_back(4);
}
void GameScoreStateView::ExtendFaithTimer(int n){
 assert(this==&g_GameScoreState&&n==10&&current->runtime.itemDropType==0);
 assert(!events.empty()&&events.back()==4);events.push_back(5);
}
'''
tail=r'''
int main(){
 auto*q=::new(static_cast<void*>(g_MainSoundOwner))EnemySoundQueueView;
 auto*other=::new(static_cast<void*>(g_OtherSoundOwner))EnemySoundQueueView;
 for(int i=0;i<4;++i){enemyA.effectResources[i]=&effects[i];enemyB.effectResources[i]=&effects[i+4];}
 int scenarios=0;
 for(int sound:{-1,0,9})for(int effect:{-1,0,7})for(int drop:{-2,0,3})for(int index=0;index<4;++index)for(int sm=0;sm<2;++sm)for(int em=0;em<2;++em){
  EnemyFullObjectView enemy{{sound,effect,index,{{12.5f,2,3}},drop}};current=&enemy;
  g_ItemManager=&itemA;g_EnemyManager=&enemyA;events.clear();changeSound=sm;changeEffect=em;spawned=false;leadingSeen=-999;
  const bool hasSound=sound>=0;const bool hasEffect=(hasSound&&sm)||effect>=0;
  const bool hasDrop=(hasEffect&&em)||drop>0;
  assert(EnemyFinalizeDeath(&enemy)==1);std::vector<int>expected;
  if(hasSound)expected.push_back(1);if(hasEffect)expected.push_back(2);if(hasDrop)expected.push_back(3);
  expected.push_back(4);expected.push_back(5);assert(events==expected&&enemy.runtime.itemDropType==0);
  assert(leadingSeen==(hasDrop?99:drop));
  assert(g_ItemManager==((hasEffect&&em)?&itemB:&itemA));assert(g_EnemyManager==((hasSound&&sm)?&enemyB:&enemyA));++scenarios;
 }
 q->~EnemySoundQueueView();other->~EnemySoundQueueView();std::printf("%d complete finalizer host scenarios passed\n",scenarios);
}
'''
variants={'positive':body}
if a.mutations:
 clear=next(x for x in ['runtime->itemDropType = 0;','*itemDropBlock = 0;']if x in body)
 changes={
  'wrong-sound-owner':('reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)','reinterpret_cast<EnemySoundQueueView *>(g_OtherSoundOwner)'),
  'wrong-effect-slot':('effectResources[runtime->deathEffectResourceIndex]','effectResources[runtime->deathEffectResourceIndex ^ 1]'),
  'conditional-core':('    EnemyDropItemCountsCore(','    if (runtime->itemDropType > 0) EnemyDropItemCountsCore('),
  'skip-leading-clear':('    '+clear,''),
  'wrong-timer-amount':('ExtendFaithTimer(10)','ExtendFaithTimer(11)'),
  'timer-before-clear':('    '+clear+'\n    g_GameScoreState.ExtendFaithTimer(10);','    g_GameScoreState.ExtendFaithTimer(10);\n    '+clear),
 }
 for label,(old,new)in changes.items():assert body.count(old)==1;variants[label]=body.replace(old,new)
 old='reinterpret_cast<const AnmFloat3View *>(\n                        &runtime->worldMotion.position)';assert body.count(old)==1
 s=body.replace('    EnemyRuntimeView *runtime = &enemy->runtime;','    EnemyRuntimeView *runtime = &enemy->runtime;\n    const PlayerFloat3 oldPosition = runtime->worldMotion.position;')
 variants['snapshot-effect-position']=s.replace(old,'&oldPosition')
a.out.mkdir(parents=True,exist_ok=True);results=[]
for label,code in variants.items():
 program=header+code+tail;src=a.out/(label+'.cpp');binary=a.out/label;src.write_text(program)
 r=subprocess.run(['c++','-std=c++17','-O1','-g','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie',str(src),'-o',str(binary)],capture_output=True,text=True);assert r.returncode==0,r.stderr
 r=subprocess.run([str(binary)],capture_output=True,text=True,preexec_fn=lambda:resource.setrlimit(resource.RLIMIT_CORE,(0,0)));log=r.stdout+r.stderr;(a.out/(label+'.log')).write_text(log)
 assert (r.returncode==0)==(label=='positive'),(label,r.returncode,log)
 if label!='positive':assert 'Assertion `'in log and 'failed.'in log and 'ERROR: AddressSanitizer'not in log and 'runtime error:'not in log,(label,log)
 results.append(dict(control=label,result='passed'if label=='positive'else'rejected',returncode=r.returncode,program_sha256=hashlib.sha256(program.encode()).hexdigest(),log_sha256=hashlib.sha256(log.encode()).hexdigest()));print(label,results[-1]['result'],r.stdout.strip(),flush=True)
receipt=dict(scope='Complete maintained finalizer host projection with synthetic logical containers and mutable real-object sinks; no target layout/i386 ABI/original/native-game runtime claim',source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),scenarios=432,strict_mutants=len(variants)-1,sanitizers='address,undefined,leaks',adaptations=['ordinary logical Enemy containers; no native offsets or virtual host construction','PlayerFloat3 aliases AnmFloat3 for live-object parameter types','sound queue objects placement-constructed in aligned owner buffers','ANM,Item,Score and scatter sinks are synthetic mutable boundaries'],results=results);(a.out/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n')
