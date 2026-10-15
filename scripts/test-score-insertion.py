#!/usr/bin/env python3
"""Actual score insertion body on constructed synthetic records, without file IO."""
from pathlib import Path
import shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
def block(text,marker):
 a=text.index(marker);i=text.index('{',a);j=i+1;depth=1
 while depth:
  depth+=(text[j]=='{')-(text[j]=='}');j+=1
 return text[a:j]
class ScoreInsertion(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_insert_body(self):
  s=(ROOT/'src/FrontEnd.cpp').read_text();body=block(s,'int FrontEndInsertScore(FrontEndScoreTableView *table)\n{')
  # Native time_t is32 bits. Preserve the production record's24-byte host
  # layout using int32_t, while replacing time() with a deterministic sink.
  record=block(s,'struct FrontEndScoreRecordView\n').replace('time_t timestamp','int32_t timestamp')+';'
  table=block(s,'struct FrontEndScoreTableView\n')+';'
  timing=block((ROOT/'src/ReplayTimingSource.hpp').read_text(),'struct ReplayTimingSource\n')+';'
  program=r'''
#include <cassert>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
'''+record+'\n'+table+'\n#pragma pack(push,4)\n'+timing+r'''
#pragma pack(pop)
static_assert(sizeof(FrontEndScoreRecordView)==24,"record");
static_assert(offsetof(FrontEndScoreRecordView,timestamp)==16,"timestamp");
static_assert(offsetof(ReplayTimingSource,lagNumerator)==0x24&&offsetof(ReplayTimingSource,lagDenominator)==0x2c,"timing");
struct Storage{FrontEndScoreTableView prefix;FrontEndScoreRecordView records[50];};
static_assert(offsetof(Storage,records)==0x10,"prefix");
static Storage storage,before;static int g_ReplayDifficulty,g_FrontEndSelectedStage,g_GameStartupValue474C90;
static struct{int score;}g_GameScoreState;
static ReplayTimingSource first,second;static ReplayTimingSource*g_ReplayTimingSource;
static int calls;static bool mutate;static int32_t*writtenTimestamp;
int32_t ScoreTime(int32_t*p){++calls;writtenTimestamp=p;*p=123456;if(mutate){g_ReplayDifficulty=1;g_ReplayTimingSource=&second;g_GameScoreState.score=9999;g_FrontEndSelectedStage=9;g_GameStartupValue474C90=9;}return *p;}
#define time ScoreTime
'''+body+r'''
void reset(){storage={};for(int d=0;d<5;++d)for(int i=0;i<10;++i){auto&r=storage.records[d*10+i];r.score=1000-i*100;r.stageNameIndex=i;r.scoreSuffix=d;memset(r.name,'a'+i,10);r.timestamp=100+i;r.slowdownRate=float(i)+.25f;}before=storage;g_ReplayDifficulty=0;g_FrontEndSelectedStage=0x180;g_GameStartupValue474C90=0x1ff;g_GameScoreState.score=1200;first={};second={};first.lagNumerator=1;first.lagDenominator=4;second.lagNumerator=1;second.lagDenominator=2;g_ReplayTimingSource=&first;calls=0;mutate=false;writtenTimestamp=NULL;}
void shifted(int difficulty,int row){for(int i=row+1;i<10;++i)assert(memcmp(&storage.records[difficulty*10+i],&before.records[difficulty*10+i-1],24)==0);for(int i=0;i<row;++i)assert(memcmp(&storage.records[difficulty*10+i],&before.records[difficulty*10+i],24)==0);}
int main(){
 reset();assert(FrontEndInsertScore(&storage.prefix)==0);shifted(0,0);auto&r=storage.records[0];assert(r.score==1200&&r.stageNameIndex==-128&&r.scoreSuffix==-1&&r.timestamp==123456&&r.slowdownRate==75);assert(memcmp(r.name,"        ",9)==0&&r.name[9]=='a'&&calls==1);assert(memcmp(&storage.records[10],&before.records[10],40*24)==0);
 reset();g_GameScoreState.score=500;assert(FrontEndInsertScore(&storage.prefix)==5);shifted(0,5);assert(storage.records[5].score==500&&storage.records[6].score==500&&storage.records[5].name[9]=='f');
 reset();g_GameScoreState.score=100;assert(FrontEndInsertScore(&storage.prefix)==9);shifted(0,9);assert(storage.records[9].name[9]=='j');
 reset();g_GameScoreState.score=99;assert(FrontEndInsertScore(&storage.prefix)==-1&&calls==0&&memcmp(&storage,&before,sizeof(storage))==0);
 reset();for(int i=0;i<10;++i)storage.records[i].score=-100-i*100;before=storage;g_GameScoreState.score=-500;assert(FrontEndInsertScore(&storage.prefix)==4);shifted(0,4);assert(storage.records[4].score==-500);
 // Mixed signs distinguish signed ranking from unsigned comparison.
 reset();g_GameScoreState.score=-1;assert(FrontEndInsertScore(&storage.prefix)==-1&&calls==0&&memcmp(&storage,&before,sizeof(storage))==0);
 reset();for(int i=0;i<10;++i)storage.records[i].score=-100-i*100;before=storage;g_GameScoreState.score=0;assert(FrontEndInsertScore(&storage.prefix)==0);shifted(0,0);assert(storage.records[0].score==0);
 reset();g_ReplayDifficulty=2;g_GameScoreState.score=750;assert(FrontEndInsertScore(&storage.prefix)==3);shifted(2,3);assert(memcmp(storage.records,before.records,20*24)==0&&memcmp(&storage.records[30],&before.records[30],20*24)==0);
 // time() sees the old record address. Subsequent difficulty/timing are fresh.
 reset();mutate=true;assert(FrontEndInsertScore(&storage.prefix)==0);assert(writtenTimestamp==&storage.records[0].timestamp&&storage.records[0].timestamp==123456&&storage.records[0].score==1200&&storage.records[0].stageNameIndex==-128&&storage.records[0].scoreSuffix==-1);assert(memcmp(storage.records[0].name,before.records[0].name,10)==0&&storage.records[0].slowdownRate==.25f);assert(memcmp(storage.records[10].name,"        ",9)==0&&storage.records[10].name[9]=='a'&&storage.records[10].slowdownRate==50&&storage.records[10].score==1000);
 reset();first.lagNumerator=2;first.lagDenominator=1;FrontEndInsertScore(&storage.prefix);assert(storage.records[0].slowdownRate==-100);
 // Default host IEEE environment only; no target FPU-mask or x87 equivalence claim.
 reset();first.lagNumerator=1;first.lagDenominator=0;FrontEndInsertScore(&storage.prefix);assert(std::isinf(storage.records[0].slowdownRate)&&storage.records[0].slowdownRate<0);
 reset();first.lagNumerator=0;first.lagDenominator=0;FrontEndInsertScore(&storage.prefix);assert(std::isnan(storage.records[0].slowdownRate));
 // Exact dyadic quotient: an erroneous float quotient becomes1 and gives0.
 reset();first.lagNumerator=67108863.0;first.lagDenominator=67108864.0;FrontEndInsertScore(&storage.prefix);assert(storage.records[0].slowdownRate==static_cast<float>(100.0/67108864.0)&&storage.records[0].slowdownRate>0);float premature=static_cast<float>(first.lagNumerator/first.lagDenominator);assert(100.0f-premature*100.0f==0);
 reset();first.lagNumerator=1;first.lagDenominator=3;FrontEndInsertScore(&storage.prefix);assert(storage.records[0].slowdownRate==static_cast<float>(100.0-(1.0/3.0)*100.0));
}
'''
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='score-insert-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
