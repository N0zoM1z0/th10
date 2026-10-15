#!/usr/bin/env python3
"""Actual collection arms with synthetic sinks; owner identity, not runtime closure."""
from pathlib import Path
import re,shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[1]
class ItemSoundOwner(unittest.TestCase):
 @unittest.skipUnless(shutil.which('c++'),'host C++ compiler unavailable')
 def test_actual_collection_receiver_and_order(self):
  text=(ROOT/'src/ItemManager.cpp').read_text()
  decl=re.search(r'extern unsigned char[^;]*g_MainSoundOwner[^;]*;',text).group()
  start=text.index('        switch (item->kind)',text.index('int __stdcall ItemManagerUpdateBody'))
  body=text[start:]
  a=body[body.index('        case 1:'):body.index('        case 2:')]
  b=body[body.index('        case 4:'):body.index('        case 7:')]
  tail=body[body.index('        item->active = 0;'):body.index('        continue;')]
  program=r'''
#include <cassert>
#include <cstring>
#include <vector>
struct Vec {float x,y,z;};
struct Item {int kind,active;Vec worldPosition;};
struct Player {} player;Player*g_Player=&player;
int g_PlayerPower=100;static int changed=1;static Item *current;
static std::vector<int> events;
struct Score {int AddPower(int n){events.push_back(100+n);return changed;}void AddRank(int n){events.push_back(200+n);}void ExtendFaithTimer(int n){events.push_back(300+n);}}g_GameScoreState;
struct Display {void UpdatePowerDisplay(int a,int b){assert(a==5&&b==0);events.push_back(2);}}display;Display*g_ItemPowerDisplayOwner=&display;
void RebuildPlayerOptions(Player*p){assert(p==g_Player);events.push_back(3);}
struct Popup {void EmitValuePopup(Vec*p,int n,unsigned color){assert(p==&current->worldPosition);assert(n==(changed?-1:50));assert(color==(changed?0xffffff40u:0xffff4040u));events.push_back(4);}}popup;Popup*g_ItemPopupOwner=&popup;
struct EnemySoundQueueView {void QueueSoundCue(int,float);};
'''+decl+r'''
unsigned char g_MainSoundOwner[0x52d0];
void EnemySoundQueueView::QueueSoundCue(int id,float x){assert(reinterpret_cast<void*>(this)==static_cast<void*>(g_MainSoundOwner));assert(x==12.5f);assert(current->active==(id==20?0:1));events.push_back(id);}
static int collect(Item*item){int convertPowerItems=0;switch(item->kind){
'''+a+b+r'''
default:break;}
'''+tail+r'''
return convertPowerItems;}
int main(){
 std::memset(g_MainSoundOwner,0xa5,sizeof(g_MainSoundOwner));
 for(int kind: {1,10,4,11})for(int c: {0,1}){Item item={kind,1,{12.5f,2,3}};current=&item;changed=c;events.clear();int result=collect(&item);assert(result==c&&item.active==0);
 if(kind==1||kind==10){if(c)assert(events==std::vector<int>({101,2,3,4,29,212,360,20}));else assert(events==std::vector<int>({101,2,4,360,20}));}
 else {if(c)assert(events==std::vector<int>({120,2,3,29,4,224,320,20}));else assert(events==std::vector<int>({120,2,4,320,20}));}}
 Item item={0,1,{12.5f,2,3}};current=&item;events.clear();assert(collect(&item)==0);assert(events==std::vector<int>({20}));
 for(unsigned char c:g_MainSoundOwner)assert(c==0xa5);
}
'''
  (ROOT/'build').mkdir(exist_ok=True)
  with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='item-sound-') as d:
   p=Path(d);(p/'test.cpp').write_text(program)
   subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
   subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
