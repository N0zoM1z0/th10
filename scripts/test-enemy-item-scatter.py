#!/usr/bin/env python3
"""Actual scatter numeric/owner projection; no i386 ABI or game-runtime claim."""
from pathlib import Path
import argparse,hashlib,json,re,subprocess,resource
R=Path(__file__).resolve().parents[1]
ap=argparse.ArgumentParser();ap.add_argument('source',type=Path,nargs='?',default=R/'src/EnemyEclDispatcher.cpp');ap.add_argument('--out',type=Path,default=R/'build/host-tests/enemy-item-scatter');ap.add_argument('--mutations',action='store_true');a=ap.parse_args()
text=a.source.read_text()
def extent(start):
 b=text.index('{',start);depth=0
 for i in range(b,len(text)):
  if text[i]=='{':depth+=1
  elif text[i]=='}':
   depth-=1
   if not depth:return text[start:i+1]
 raise ValueError('unbalanced maintained body')
vec=extent(text.index('struct EnemyDropVectorView'))+';'
core=extent(re.search(r'(?m)^__declspec\(noinline\) void(?: __stdcall)? EnemyDropItemCountsCore\(',text).start())
wrap=extent(re.search(r'(?m)^(?:__declspec\(noinline\) )?float __stdcall EnemyWrapAngle\(float angle\)\s*\{',text).start()).replace('EnemyWrapAngle(','HostWrapBody(',1)
# Two float bit reads use memcpy in the host projection; actual i386 source
# continues to use the target's offset-based block view.
for index in [13,14]:
 old='reinterpret_cast<float *>(itemDropBlock)[%d]'%index;assert core.count(old)==1;core=core.replace(old,'read_float_word(itemDropBlock + %d)'%index)
header=r'''
#include <cassert>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <vector>
#include <array>
#define __declspec(x)
#define __stdcall
struct AnmFloat3View {float x,y,z;};
using PlayerFloat3=AnmFloat3View;
struct Spawn {AnmFloat3View p;int kind;unsigned color;float angle,speed;};
static std::vector<Spawn> spawns;
static std::vector<float> wrapInputs;
struct ItemManagerView {int Spawn(const AnmFloat3View*,int,unsigned,float,float);};
static ItemManagerView manager,otherManager;
static ItemManagerView *g_ItemManager=&manager;
int ItemManagerView::Spawn(const AnmFloat3View*p,int k,unsigned c,float a,float s){
 assert(this==g_ItemManager);AnmFloat3View v;std::memcpy(&v,p,sizeof(v));
 spawns.push_back({v,k,c,a,s});return 0;
}
struct Rng {std::vector<unsigned> words;std::size_t index;float signedValue;int signedCalls;
 unsigned GetRandomU32(){assert(index<words.size());return words[index++];}
 float GetRandomF32Signed(){++signedCalls;return signedValue;}
}g_RngView;
static float read_float_word(const int*p){float f;std::memcpy(&f,p,4);return f;}
static void write_float_word(int*p,float f){std::memcpy(p,&f,4);}
'''
tail=r'''
static float reference_wrap(float a){
 int n=0;while(a>3.1415927f){a-=6.2831855f;if(n++>32)break;}
 while(a< -3.1415927f){a+=6.2831855f;if(n++>32)break;}return a;
}
static void close(float x,float y){assert(std::fabs(x-y)<=0.00015f);}
int main(){
 static_assert(sizeof(int)==4&&sizeof(float)==4&&sizeof(PlayerFloat3)==12,"host data model");
 unsigned values[]={0u,0x00010001u,0x7fff7fffu,0x80008000u,0xffffffffu};
 int scenarios=0;
 for(float seed:{0.0f,0.3f,-0.8f})for(unsigned r:values)for(unsigned u:values)for(int shape=0;shape<3;++shape){
  std::array<int,15> b{};b[0]=7;b[1]=2;b[3]=1;b[12]=77;
  const float rx=shape==0?10.0f:shape==1?3.0f:0.0f;
  const float ry=shape==0?10.0f:shape==1?0.0f:7.0f;
  write_float_word(&b[13],rx);write_float_word(&b[14],ry);
  const int xbits=b[13],ybits=b[14];PlayerFloat3 position={12.0f,-5.0f,9.25f};
  g_RngView={std::vector<unsigned>{r,u,0x80008000u,0x7fff7fffu,0xffffffffu,0u},0,seed,0};spawns.clear();wrapInputs.clear();
  EnemyDropItemCountsCore(&position,b.data());
  assert(spawns.size()==3&&wrapInputs.size()==3&&g_RngView.index==6&&g_RngView.signedCalls==1);
  float angle=seed*3.1415927f;
  for(int i=0;i<3;++i){
   const long double radial=static_cast<long double>(g_RngView.words[2*i])/8589934592.0L+0.5L;
   const float distance=static_cast<float>(radial);
   const float x=static_cast<float>(std::cos(angle))*rx*distance+position.x;
   const float y=static_cast<float>(std::sin(angle))*ry*distance+position.y;
   close(spawns[i].p.x,x);close(spawns[i].p.y,y);assert(spawns[i].p.z==position.z);
   assert(spawns[i].kind==(i<2?1:3)&&spawns[i].color==0xffffffffu&&spawns[i].angle== -1.5707964f&&spawns[i].speed==2.2f);
   const long double turn=(static_cast<long double>(g_RngView.words[2*i+1])/2147483648.0L-1.0L)*static_cast<long double>(0.7853982f)+angle+static_cast<long double>(1.5707964f);
   const float argument=static_cast<float>(turn);close(wrapInputs[i],argument);angle=reference_wrap(argument);
  }
  assert(b[0]==7&&b[13]==xbits&&b[14]==ybits);
  for(int i=1;i<=12;++i)assert(b[i]==0);++scenarios;
 }
 for(int n:{0,-1,-100}){std::array<int,15>b{};b[0]=5;b[1]=n;b[12]=99;g_RngView={{},0,0.2f,0};spawns.clear();wrapInputs.clear();EnemyDropItemCountsCore(new PlayerFloat3{0,0,0},b.data());assert(spawns.empty()&&wrapInputs.empty()&&g_RngView.index==0&&g_RngView.signedCalls==1&&b[0]==5);for(int i=1;i<=12;++i)assert(b[i]==0);++scenarios;}
 std::printf("%d actual-body host scenarios passed\n",scenarios);
}
'''
# Avoid a deliberate leak in the no-drop cases while preserving ordinary lifetimes.
tail=tail.replace('EnemyDropItemCountsCore(new PlayerFloat3{0,0,0},b.data());','PlayerFloat3 p{0,0,0};EnemyDropItemCountsCore(&p,b.data());')
program=header+vec+'\n'+wrap+'\nfloat EnemyWrapAngle(float a){wrapInputs.push_back(a);return HostWrapBody(a);}\n'+core+tail
variants={'positive':program}
if a.mutations:
 for bits,var in [('radiusBits','radiusScale'),('angleBits','angleRandom')]:
  needle='float '+var+' = static_cast<float>('+bits+');';assert program.count(needle)==1
  variants['duplicate-'+bits]=program.replace(needle,needle+'\nif (static_cast<int>('+bits+') < 0) '+var+' += 4294967296.0f;')
 clear=next(x for x in ['memset(counts, 0, 12 * sizeof(int))','memset(itemDropBlock + 1, 0, 12 * sizeof(int))']if x in program)
 assert program.count(clear)==1
 variants['clear-eleven']=program.replace(clear,clear.replace('12 * sizeof(int)','11 * sizeof(int)'))
 variants['wrong-manager']=program.replace('g_ItemManager->Spawn(\n','(&otherManager)->Spawn(\n')
a.out.mkdir(parents=True,exist_ok=True);results=[]
for label,s in variants.items():
 f=a.out/(label+'.cpp');binary=a.out/label;f.write_text(s)
 cmd=['c++','-std=c++17','-O1','-ffp-contract=off','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie',str(f),'-o',str(binary)]
 r=subprocess.run(cmd,capture_output=True,text=True);assert r.returncode==0,r.stderr
 r=subprocess.run([str(binary)],capture_output=True,text=True,preexec_fn=lambda:resource.setrlimit(resource.RLIMIT_CORE,(0,0)));(a.out/(label+'.log')).write_text(r.stdout+r.stderr)
 assert (r.returncode==0)==(label=='positive'),(label,r.returncode,r.stdout,r.stderr)
 results.append(dict(control=label,returncode=r.returncode,result='passed'if label=='positive'else'rejected',stdout=r.stdout.strip(),program_sha256=hashlib.sha256(s.encode()).hexdigest()));print(label,results[-1]['result'],r.stdout.strip(),flush=True)
receipt=dict(scope='Maintained complete core/vector/wrap host projection with scripted RNG and synthetic item sink; not original/native-game runtime or i386 ABI/rounding proof',source=str(a.source),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),adaptations=['host PlayerFloat3/AnmFloat3 alias uses ordinary constructed objects','Item sink copies object representation with memcpy','two offset radius reads use memcpy','actual vector uses its maintained non-MSVC sin/cos arm'],sanitizers='address,undefined,leaks',scenarios=228,strict_mutants=len(variants)-1,results=results)
(a.out/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n')
