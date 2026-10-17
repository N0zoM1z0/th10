// FRESH HOST ACTUAL-SOURCE TESTS. Actual Run, local helpers, canonical header
// fields, byte accessors and selected dependencies are retained. Explicit seams:
// three timer constructors; owner/message constructors/destructors; SDK-only
// types; rendering/resource outcalls; decode/advance and unresolved effects.
#define __stdcall
#define __fastcall
#define __cdecl
#define __thiscall
#define __declspec(x)
#define __forceinline inline
#define __int64 long long
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <new>
#include <climits>
#include "owner.inc"
#include "Player.hpp"
#include "Enemy.hpp"
static void Check(bool x,const char*n){if(!x){std::fprintf(stderr,"CHECK %s\n",n);std::exit(1);}}
float g_AnmGameSpeed=1;AnmRenderManagerView *g_AnmRenderManagerView;
GuiView *g_GuiView;GuiPlayerView *g_GuiPlayer;GuiEnemyManagerView *g_GuiEnemyManager;GuiGameOwnerView *g_GuiGameOwner;GuiModeOwnerView *g_GuiModeOwner;unsigned char *g_GuiPersistentData;
int g_GuiHighScore,g_GuiCounter4C4C,g_GuiState4C58,g_GuiCharacter,g_GuiShotType,g_GuiPowerLevel,g_GuiDifficulty,g_GuiStage,g_GuiMessageState,g_GuiMessageStateAux,g_GuiValue4C90,g_GuiValue4C94,g_GuiValue4C9C,g_GuiGlobalMode;
short g_GuiPower;unsigned int g_GuiGameFlags,g_GuiInputCurrent,g_GuiInputEdges,g_GuiSupervisorFlags;GuiScoreView g_GuiScore;MainSupervisorView g_MainSupervisorView;
Player *g_Player;EnemyManagerView *g_EnemyManager;GameStageUpdateView *g_GameStageUpdate;
unsigned int g_FrontEndSupervisorFlags;unsigned char *g_FrontEndProfileData;
void FrontEndQueueSoundCommand(const char *,int,int);
alignas(EnemySoundQueueView) unsigned char g_MainSoundOwner[0x52d0];unsigned char g_GameStageState[0x100];
#include "canonical-dependency-types.inc"
GameChainOwnerView *g_GameOwner477830;static ResultMenuView gameOwner;EnemySoundCueMetadataView g_EnemySoundCueMetadata[134];
AnmVmView::AnmVmView(){}AnmVmView::~AnmVmView(){}AnmLoadedView::AnmLoadedView(){}AnmRenderManagerView::AnmRenderManagerView(){}AnmRenderManagerView::~AnmRenderManagerView(){}GuiView::~GuiView(){}Player::Player(){}Player::~Player(){}EnemyManagerView::EnemyManagerView(){}EnemyManagerView::~EnemyManagerView(){}
// Guaranteed static zero-initialization precedes unchanged Player raw-flag member
// constructors. No raw-storage prefill or extra header constructor seam is used.
static Player playerOwner;
GuiMessageVmView::GuiMessageVmView(const unsigned char *){messageIndex=0;lifetimeTimer.SetCurrent(0);instructionTimer.SetCurrent(0);waitTimer.SetCurrent(0);unknown58=0;currentInstruction=nullptr;textPositions[0]=AnmFloat3View(24,3,5);textPositions[1]=AnmFloat3View(144,13,15);inputCooldown=0;flags=0;alternatingLine=0;activeSide=0;textColors[0]=0xf8f08f;textColors[1]=0xb5e1ff;}
GuiMessageVmView::~GuiMessageVmView(){}
#include "dependencies.inc"
struct Creation{AnmLoadedView*resource;int script;unsigned layer;int id;};struct Sprite{AnmLoadedView*resource;AnmVmView*vm;int index;};struct Draw{AnmRenderManagerView*manager;AnmVmView*vm;unsigned color;std::string text;};
struct Host{std::vector<Creation>creations;std::vector<Sprite>sprites;std::vector<Draw>draws;std::vector<std::string>order;std::vector<int>transitions;std::vector<float>effects;std::vector<std::vector<int>>screen;std::vector<std::pair<int,int>>music;std::vector<const void*>advances;int advanceMode=-99,id=116;std::function<void()>afterSprite,afterDecode,afterDraw,afterCreate,afterEffect,afterTransition;};static Host host;
AnmVmIdView AnmLoadedView::CreateVmVariant0(int s,unsigned l){host.creations.push_back({this,s,l,host.id});host.order.push_back("create");if(host.afterCreate)host.afterCreate();return AnmVmIdView(host.id);}
int AnmLoadedView::SetSprite(AnmVmView*v,int n){host.sprites.push_back({this,v,n});host.order.push_back("sprite");if(host.afterSprite)host.afterSprite();return 0;}
void AnmRenderManagerView::DrawTextLeft(AnmVmView*v,unsigned c,const char*t,...){Check(v!=nullptr,"draw.live_vm");host.draws.push_back({this,v,c,t});host.order.push_back("draw");if(host.afterDraw)host.afterDraw();}
namespace th10{namespace leaf{
unsigned char *Fn00417010(unsigned char*t){static unsigned char decoded[64];auto n=std::strlen(reinterpret_cast<char*>(t));Check(n<64,"decode.bounded_text");std::memcpy(decoded,t,n+1);host.order.push_back("decode");if(host.afterDecode)host.afterDecode();return decoded;}
void Fn004175E0(unsigned char*p){host.advances.push_back(p);host.advanceMode=g_MainSupervisorView.nextGameMode;host.order.push_back("advance");}
}}
void GameCompleteStageTransition(ResultMenuView*p){Check(p==reinterpret_cast<ResultMenuView *>(g_GameOwner477830)&&p,"transition.live_receiver");host.transitions.push_back(0);host.order.push_back("transition");if(host.afterTransition)host.afterTransition();}
int __stdcall GameQueueScaledSoundCommand(float v){host.effects.push_back(v);host.order.push_back("effect");return 0;}
void *StageCreateTransitionEffect(int priority,int kind,int frames,unsigned a,unsigned b,unsigned c){host.screen.push_back({priority,kind,frames,static_cast<int>(a),static_cast<int>(b),static_cast<int>(c)});host.order.push_back("screen");if(host.afterEffect)host.afterEffect();return &gameOwner;}
void FrontEndQueueSoundCommand(const char*n,int cmd,int slot){Check(!std::strcmp(n,"dummy"),"music.command_name");host.music.push_back({cmd,slot});host.order.push_back("music");}
struct Fixture{
 std::unique_ptr<AnmRenderManagerView>manager,alternate;std::unique_ptr<GuiView>gui;AnmLoadedView playerResource,enemyResource,otherResource;GuiPlayerView playerShadow;GuiEnemyManagerView enemyShadow;GuiModeOwnerView mode;EnemyManagerView enemy;GameStageUpdateView descriptor;AnmVmView vms[16];GuiMessageVmView message;std::vector<unsigned char>bytes,profile;std::size_t first=0,future=0;
 Fixture():manager(new AnmRenderManagerView),alternate(new AnmRenderManagerView),gui(new GuiView),message(nullptr),profile(0x20000,0xa5){host=Host();g_AnmGameSpeed=1;g_AnmRenderManagerView=manager.get();g_GuiView=gui.get();g_GuiPlayer=&playerShadow;g_GuiEnemyManager=&enemyShadow;g_GuiModeOwner=&mode;g_GuiPersistentData=profile.data();g_Player=&playerOwner;g_EnemyManager=&enemy;playerOwner.resource=&playerResource;enemy.effectResources[2]=&enemyResource;playerShadow.portraitAnm=enemyShadow.portraitAnm=&otherResource;g_GameStageUpdate=&descriptor;descriptor.musicIndex=41;descriptor.value28=23;g_GameOwner477830=reinterpret_cast<GameChainOwnerView *>(&gameOwner);g_FrontEndSupervisorFlags=0;g_FrontEndProfileData=profile.data();new(g_MainSoundOwner)EnemySoundQueueView{};for(int i=0;i<12;++i)reinterpret_cast<EnemySoundQueueView*>(g_MainSoundOwner)->activeSoundIds[i]=-1;g_EnemySoundCueMetadata[0].cueValue=37;g_MainSupervisorView.flags=0;g_MainSupervisorView.nextGameMode=-99;mode.state=0;gui->frontAnm=gui->stageUiAnm=&otherResource;gui->guiFlags=0;gui->endingCounter=47;gui->specialVmId9E14.value=115;
  g_GuiStage=2;g_GuiCharacter=g_GuiShotType=g_GuiDifficulty=0;g_GuiPower=2;g_GuiPowerLevel=1;g_GuiCounter4C4C=3;g_GuiScore.score=0;g_GuiGameFlags=g_GuiInputCurrent=g_GuiInputEdges=0;g_GuiMessageState=3;g_GuiMessageStateAux=5;
  for(int i=0;i<16;++i){vms[i].id=101+i;vms[i].managerNode.owner=&vms[i];vms[i].managerNode.previous=i?&vms[i-1].managerNode:nullptr;vms[i].managerNode.next=i<15?&vms[i+1].managerNode:nullptr;vms[i].layerNode.owner=&vms[i];vms[i].layerNode.next=vms[i].layerNode.previous=nullptr;vms[i].pendingInterrupt=-12;vms[i].positionOffset=AnmFloat3View(-7,-8,-9);vms[i].flags35C=0;vms[i].scriptIndex=0;vms[i].anmFile=&playerResource;if(i<6)message.vmIds[i].value=101+i;}
  for(auto*m:{manager.get(),alternate.get()}){m->primaryVmListHead=&vms[0].managerNode;m->primaryVmListTail=&vms[15].managerNode;m->secondaryVmListHead=m->secondaryVmListTail=nullptr;}}
 unsigned char*Instruction(){return bytes.data()+first;}unsigned char*Future(){return bytes.data()+future;}
 std::size_t Append(unsigned char op,const std::vector<unsigned char>&p={},unsigned short t=0){Check(p.size()<256,"record.payload_bound");auto n=bytes.size();bytes.push_back(static_cast<unsigned char>(t));bytes.push_back(static_cast<unsigned char>(t>>8));bytes.push_back(op);bytes.push_back(static_cast<unsigned char>(p.size()));bytes.insert(bytes.end(),p.begin(),p.end());return n;}
 static std::vector<unsigned char>Int(int n){std::vector<unsigned char>p(4);std::memcpy(p.data(),&n,4);return p;}
 void Program(unsigned char op,const std::vector<unsigned char>&p={},bool hasFuture=true,unsigned short t=0){bytes.clear();first=Append(op,p,t);future=hasFuture?Append(0,{},100):bytes.size();message.currentInstruction=Instruction();}
 void Children(int parent,int a,int b){vms[6].scriptIndex=static_cast<short>(a);vms[7].scriptIndex=static_cast<short>(b);vms[parent].layerNode.next=&vms[6].layerNode;vms[6].layerNode.previous=&vms[parent].layerNode;vms[6].layerNode.next=&vms[7].layerNode;vms[7].layerNode.previous=&vms[6].layerNode;}
 std::size_t Offset()const{return(g_GuiShotType+3*g_GuiCharacter)*0x437c+0x4d0+4*g_GuiDifficulty;}
 void Counter(int n){std::memcpy(profile.data()+Offset(),&n,4);}int Counter()const{int n;std::memcpy(&n,profile.data()+Offset(),4);return n;}
};
static std::vector<int>Sounds(){auto*s=reinterpret_cast<EnemySoundQueueView*>(g_MainSoundOwner);std::vector<int>r;for(int i=0;i<12;++i)if(s->activeSoundIds[i]>=0){Check(s->activeSoundIds[i]==0&&s->cueValues[0]==37,"sound.receiver_sample");for(int n=0;n<s->sampleCounts[i];++n){Check(s->samples[i][n]==0,"sound.receiver_sample");r.push_back(0);}}return r;}
static bool Same(const AnmFloat3View&a,const AnmFloat3View&b){return a.x==b.x&&a.y==b.y&&a.z==b.z;}
static void Normal(Fixture&f){Check(GuiMessageVmView::Run(&f.message)==0,"normal.result");Check(f.message.currentInstruction==f.Future(),"normal.advance");Check(f.message.instructionTimer.current==1&&f.message.instructionTimer.previous==0,"normal.tick");Check(f.message.lifetimeTimer.current==0,"normal.lifetime");}
static void Portraits(){
 for(int c:{0,1,2,-1})for(int s:{1,2,3,4,5,6,7})for(int index:{-1,0,1}){Fixture f;g_GuiCharacter=c;g_GuiStage=s;f.message.messageIndex=index;f.Program(2);Normal(f);if(c==0||c==1){bool special=s==6||(s==7&&index<1);Check(host.creations.size()==1&&host.creations[0].resource==(special?&f.enemyResource:&f.playerResource),"portrait.enemy_resource");Check(host.creations[0].script==(s==6?0x20:s==7&&index<1?0x25:0x1e)&&host.creations[0].layer==15,"portrait.enemy_script_layer");Check(f.message.vmIds[1].value==host.id,"portrait.enemy_id");}else Check(host.creations.empty()&&f.message.vmIds[1].value==102,"portrait.character_guard");}
 for(int c:{0,1,2,-1}){Fixture f;g_GuiCharacter=c;f.Program(1);Normal(f);if(c==0||c==1)Check(host.creations.size()==1&&host.creations[0].resource==&f.playerResource&&host.creations[0].script==0x1d&&host.creations[0].layer==15,"portrait.player_create");else Check(host.creations.empty(),"portrait.player_guard");}
 {Fixture f;f.Program(3);Normal(f);Check(host.creations.size()==1&&host.creations[0].resource==&f.otherResource&&host.creations[0].script==0x5a&&f.message.vmIds[2].value==host.id,"portrait.box");}}
static void Sides(){for(int s:{0,1}){Fixture f;f.Program(7+s);f.message.alternatingLine=7;Normal(f);Check(f.message.activeSide==s&&f.message.alternatingLine==0,"side.state");Check(f.vms[0].pendingInterrupt==(s?3:2)&&f.vms[1].pendingInterrupt==(s?2:3),"side.interrupts");Check(Same(f.vms[3].positionOffset,f.message.textPositions[s])&&Same(f.vms[4].positionOffset,f.message.textPositions[s]),"side.both_selected");}}
static void Hides(){
 {Fixture f;f.Program(4);Normal(f);Check(f.vms[0].pendingInterrupt==1&&f.message.vmIds[0].value==0,"hide.player");}
 {Fixture f;f.Program(5);Normal(f);Check(f.vms[1].pendingInterrupt==1&&f.message.vmIds[1].value==0&&f.vms[5].pendingInterrupt==1&&f.message.vmIds[5].value==106,"hide.enemy_name");}
 {Fixture f;f.Program(6);Normal(f);Check(f.vms[2].pendingInterrupt==1&&f.vms[3].pendingInterrupt==1&&f.vms[4].pendingInterrupt==1,"hide.text");}
 {Fixture f;f.Program(17);Normal(f);Check(f.vms[3].pendingInterrupt==3&&f.vms[4].pendingInterrupt==3,"hide.lines");}}
static void Flags(){for(unsigned flags:{0u,1u,0xfffffffeu,0xffffffffu})for(unsigned char input:{0,1,2,255}){Fixture f;f.Program(9,{input});f.message.flags=flags;Normal(f);Check(f.message.flags==((flags&~1u)|(input&1u)),"flags.preserve_other_bits");}{Fixture f;f.Program(11);f.message.inputCooldown=8;Normal(f);Check(f.message.inputCooldown==1,"cooldown.set_one");}}
static void Wait(){
 for(float scale:{.99f,1.f,1.01f}){Fixture f;f.Program(10,Fixture::Int(5));g_AnmGameSpeed=scale;f.message.alternatingLine=1;Check(GuiMessageVmView::Run(&f.message)==0,"wait.pending_result");Check(f.message.waitTimer.current==static_cast<int>(5.f-scale)&&f.message.currentInstruction==f.Instruction(),"wait.pending_state");Check(f.message.instructionTimer.current==0&&f.message.instructionTimer.previous==-1&&f.message.alternatingLine==1&&Sounds().empty(),"wait.pending_no_tick");}
 for(int n:{-2,-1,0,1}){Fixture f;f.Program(10,Fixture::Int(n));f.message.alternatingLine=1;Normal(f);Check(f.message.waitTimer.current==0&&f.message.alternatingLine==0&&Sounds()==std::vector<int>{0},"wait.expiry");}
 for(unsigned e:{1u,0x1000u,0x1001u}){Fixture f;f.Program(10,Fixture::Int(20));g_GuiInputEdges=e;Normal(f);Check(Sounds()==std::vector<int>{0}&&f.message.waitTimer.current==0,"wait.edge");}
 {Fixture f;f.Program(10,Fixture::Int(20));f.message.flags=1;g_GuiInputCurrent=0x100;f.message.alternatingLine=1;Normal(f);Check(Sounds().empty()&&f.message.waitTimer.current==0&&f.message.alternatingLine==0,"wait.skip");}
 {Fixture f;f.Program(10,Fixture::Int(99));f.message.waitTimer.SetCurrent(3);Check(GuiMessageVmView::Run(&f.message)==0&&f.message.waitTimer.current==2,"wait.retain_existing");}}
static void Sprites(){
 for(int c:{0,1,2,-1}){Fixture f;g_GuiCharacter=c;f.Children(0,0x17,0x1a);f.Program(12,Fixture::Int(10));Normal(f);if(c==0||c==1)Check(host.sprites.size()==2&&host.sprites[0].vm==&f.vms[6]&&host.sprites[1].vm==&f.vms[7]&&host.sprites[0].index==10+(c?0x2d:0x34)&&host.sprites[1].index==10+(c?0x35:0x3c),"sprites.player_values");else Check(host.sprites.empty(),"sprites.player_character_guard");}
 const int offsets[7][2]={{0x0f,0x17},{0x15,0x1e},{0x22,0x2b},{0x25,0x2e},{0x16,0x1f},{0x30,0x39},{0x44,0x4d}},children[7][2]={{0x18,0x1b},{0x18,0x1b},{0x18,0x1b},{0x18,0x1b},{0x18,0x1b},{0x1c,0x1f},{0x21,0x24}};
 for(int c:{0,1,2,-1})for(int s:{0,1,2,3,4,5,6,7,8})for(int index:{-1,0,1}){Fixture f;g_GuiCharacter=c;g_GuiStage=s;f.message.messageIndex=index;f.Program(13,Fixture::Int(10));bool valid=s>=1&&s<=7&&(c==0||c==1),alt=s==7&&index>=1;if(s>=1&&s<=7)f.Children(1,alt?0x18:children[s-1][0],alt?0x1b:children[s-1][1]);Normal(f);if(valid)Check(host.sprites.size()==2&&host.sprites[0].resource==&f.enemyResource&&host.sprites[1].resource==&f.enemyResource&&host.sprites[0].index==10+(alt?0x2e:offsets[s-1][0])&&host.sprites[1].index==10+(alt?0x37:offsets[s-1][1]),"sprites.enemy_table");else Check(host.sprites.empty(),"sprites.enemy_guard");}
 // Synthetic callbacks mutate only live record bytes, resources and children.
 {Fixture f;f.Children(1,0x18,0x1b);f.Program(13,Fixture::Int(10));host.afterSprite=[&](){if(host.sprites.size()==1){int n=20;std::memcpy(f.Instruction()+4,&n,4);f.enemy.effectResources[2]=&f.otherResource;}};Normal(f);Check(host.sprites.size()==2&&host.sprites[1].resource==&f.otherResource&&host.sprites[1].index==20+0x1e,"sprites.enemy_reload");}
 {Fixture f;f.Children(0,0x17,0x1a);f.Program(12,Fixture::Int(10));host.afterSprite=[&](){if(host.sprites.size()==1){int n=20;std::memcpy(f.Instruction()+4,&n,4);f.vms[7].scriptIndex=0x19;f.vms[8].scriptIndex=0x1a;f.vms[7].layerNode.next=&f.vms[8].layerNode;f.vms[8].layerNode.previous=&f.vms[7].layerNode;}};Normal(f);Check(host.sprites.size()==2&&host.sprites[1].vm==&f.vms[8]&&host.sprites[1].index==20+0x3c,"sprites.find_set_reload_order");}}
static void Text(){
 for(int op:{14,15})for(int side:{0,1}){Fixture f;f.Program(op,{'O','K',0});f.message.activeSide=side;Normal(f);Check(host.draws.size()==1&&host.draws[0].manager==f.manager.get()&&host.draws[0].vm==&f.vms[3+op-14]&&host.draws[0].text=="OK"&&host.draws[0].color==f.message.textColors[side],"text.line");Check(f.vms[3+op-14].pendingInterrupt==2,"text.interrupt");}
 for(int side:{0,1})for(int alt:{0,1,2}){Fixture f;f.Program(16,{'O','K',0});f.message.activeSide=side;f.message.alternatingLine=alt;Normal(f);if(!alt)Check(host.draws.size()==3&&host.draws[0].vm==&f.vms[3]&&host.draws[0].text==" "&&host.draws[1].vm==&f.vms[4]&&host.draws[1].text==" "&&host.draws[2].vm==&f.vms[3]&&host.draws[2].text=="OK"&&f.vms[3].pendingInterrupt==2&&f.vms[4].pendingInterrupt==3&&f.message.alternatingLine==1,"text.alternating_zero");else Check(host.draws.size()==1&&host.draws[0].vm==&f.vms[4]&&host.draws[0].text=="OK"&&f.vms[4].pendingInterrupt==2&&f.message.alternatingLine==0,"text.alternating_nonzero");for(auto&d:host.draws)Check(d.color==f.message.textColors[side],"text.colors");}
 for(int op:{14,15,16})for(int alt:{0,1}){Fixture f;f.Program(op,{'A',0});f.message.alternatingLine=alt;int line=op==14?0:op==15?1:alt?1:0;host.afterDecode=[&](){f.message.vmIds[3+line].value=f.vms[9].id;g_AnmRenderManagerView=f.alternate.get();};Normal(f);auto&d=host.draws.back();Check(d.vm==&f.vms[3+line],"text.lookup_before_decode");Check(d.manager==(op==16&&!alt?f.alternate.get():f.manager.get()),"text.cached_receiver");}}
static void Music(){Fixture f;f.Program(18);Normal(f);Check(host.music==std::vector<std::pair<int,int>>{{2,1}}&&f.profile[0x1d892+23]==1&&f.profile[0x1d892+41]==0xa5,"music.receiver_track");Check(host.order==std::vector<std::string>{"music","create"}&&host.creations.size()==1&&host.creations[0].resource==&f.otherResource&&host.creations[0].script==2&&host.creations[0].layer==15,"music.before_ui");for(int i=0;i<6;++i)Check(f.message.vmIds[i].value==101+i,"music.discard_created_id");}
static void Names(){
 const int scripts[7]={0x0d,0x10,0x14,0x15,0x0e,0x23,0x1d};
 for(int s:{INT_MIN,-123,-1,0,1,2,3,4,5,6,7,8,123,INT_MAX}){
  Fixture f;g_GuiStage=s;f.Program(19);auto profile=f.profile;
  f.message.unknown58=0x1357;f.message.flags=0x40;f.message.activeSide=1;f.message.alternatingLine=2;
  Normal(f);
  if(s>=1&&s<=7){
   Check(host.creations.size()==1&&host.creations[0].resource==&f.enemyResource&&host.creations[0].script==scripts[s-1]&&f.message.vmIds[5].value==host.id,"names.table");
   Check(host.creations[0].layer==15&&host.order==std::vector<std::string>{"create"},"names.layer_one_create");
  }else Check(host.creations.empty()&&host.order.empty()&&f.message.vmIds[5].value==106,"names.stage_guard");
  for(int i=0;i<5;++i)Check(f.message.vmIds[i].value==101+i,"names.other_ids");
  Check(f.message.unknown58==0x1357&&f.message.flags==0x40&&f.message.activeSide==1&&f.message.alternatingLine==2&&f.profile==profile&&f.gui->guiFlags==0&&f.gui->endingCounter==47,"names.sentinels");
  for(auto&v:f.vms)Check(v.pendingInterrupt==-12&&v.flags35C==0&&Same(v.positionOffset,AnmFloat3View(-7,-8,-9)),"names.vm_sentinels");
 }
}
static void Complete(){
 const int bonuses[5]={20000000,25000000,35000000,40000000,40000000},power[5]={100000,100000,200000,300000,400000};
 for(int stage:{6,7})for(int difficulty:{0,1,2,3,4})for(int count:{-1,99998,99999,100000})for(int mode:{0,1}){Fixture f;g_GuiStage=stage;g_GuiDifficulty=difficulty;g_GuiCharacter=1;g_GuiShotType=2;f.mode.state=mode;f.Counter(count);auto before=f.profile;f.Program(20);Normal(f);Check((f.gui->guiFlags&0x20)!=0,"complete.clear_flag");Check(g_GuiScore.score==300+(stage==7?40000000:bonuses[difficulty])/10+(stage==7?400000:power[difficulty])*2/10,"complete.score");if(mode)Check(g_MainSupervisorView.nextGameMode==4&&host.transitions.empty()&&host.screen.empty()&&f.gui->endingCounter==47&&(f.gui->guiFlags&0x10)==0&&f.Counter()==count,"complete.mode_skip");else{Check(f.Counter()==(count<99999?count+1:count),"complete.capped_counter");if(stage==6)Check(host.screen==std::vector<std::vector<int>>{{0x31,5,120,0,0,0}}&&f.gui->endingCounter==0&&(f.gui->guiFlags&0x10)&&host.transitions.empty(),"complete.stage6_ending");else Check(host.transitions==std::vector<int>{0}&&host.screen.empty()&&f.gui->endingCounter==47&&(f.gui->guiFlags&0x10)==0,"complete.stage7_no_ending");}auto unlock=5*0x437c+0x4e0+(stage+difficulty*6)*8;if(difficulty!=4){Check(f.profile[unlock]==1&&f.profile[unlock+1]==1,"complete.unlock");before[unlock]=before[unlock+1]=1;}if(!mode){int n=count<99999?count+1:count;std::memcpy(before.data()+f.Offset(),&n,4);}Check(f.profile==before,"complete.profile_sentinels");}
 {Fixture f;g_GuiStage=7;g_GuiPower=-2;f.Counter(2);f.Program(20);Normal(f);Check(g_GuiScore.score==3920300,"complete.signed_power");}
 {Fixture f;g_GuiStage=6;g_GuiGameFlags=0x10;f.Counter(8);f.Program(20);Normal(f);Check(host.transitions==std::vector<int>{0}&&g_MainSupervisorView.nextGameMode==-99&&host.screen.empty()&&g_GuiScore.score==0&&f.Counter()==8&&f.gui->guiFlags==0,"complete.early_transition");Check(f.profile[0x4e0+48]==1&&f.profile[0x4e1+48]==1,"complete.unlock_before_early");}
 for(int stage:{6,7})for(int diff:{-1,5}){Fixture f;g_GuiStage=stage;g_GuiDifficulty=diff;f.Counter(8);f.Program(20);Normal(f);Check(g_GuiScore.score==(stage==6?300:4080300)&&f.Counter()==9,"complete.invalid_difficulty");}
 {Fixture f;g_GuiStage=7;f.gui->guiFlags=0x10;f.Counter(8);f.Program(20);Normal(f);Check(f.gui->guiFlags==0x30&&f.gui->endingCounter==47,"complete.stage7_preserve_ending_flag");}
 {Fixture f;g_GuiStage=6;f.mode.state=1;g_MainSupervisorView.flags=0x1000;f.Counter(8);f.Program(20);Normal(f);Check(g_MainSupervisorView.nextGameMode==2&&f.Counter()==8,"complete.supervisor_override");}
 for(int stage:{6,7}){Fixture f;std::vector<unsigned char>other(0x20000,0xa5);std::unique_ptr<GuiView>otherGui(new GuiView);otherGui->guiFlags=0x40;otherGui->endingCounter=99;g_GuiStage=stage;f.Counter(8);int count=77;std::size_t offset=5*0x437c+0x4d0+12;std::memcpy(other.data()+offset,&count,4);auto swap=[&](){g_GuiPersistentData=other.data();g_GuiCharacter=1;g_GuiShotType=2;g_GuiDifficulty=3;g_GuiView=otherGui.get();};if(stage==6)host.afterEffect=swap;else host.afterTransition=swap;f.Program(20);Normal(f);std::memcpy(&count,other.data()+offset,4);int original;std::memcpy(&original,f.profile.data()+0x4d0,4);Check(count==78&&original==8,"complete.reload_profile_scalars");Check(f.gui->guiFlags==0x20&&f.gui->endingCounter==47&&otherGui->guiFlags==(stage==6?0x50u:0x40u)&&otherGui->endingCounter==(stage==6?0:99),"complete.reload_gui");}
 for(int stage:{1,2,3,4,5}){Fixture f;g_GuiStage=stage;f.Program(20);Normal(f);Check((f.vms[14].flags35C&0x04000000u)&&f.gui->specialVmId9E14.value==host.id,"complete.replace_special");Check(host.creations.size()==1&&host.creations[0].resource==&f.otherResource&&host.creations[0].script==0x4c&&g_MainSupervisorView.nextGameMode==0x0b,"complete.ordinary_ui_mode");Check(host.advances.size()==1&&host.advances[0]==g_GameStageState,"complete.advance_owner");Check(host.order==std::vector<std::string>{"create","advance"}&&host.advanceMode==0x0b,"complete.advance_order");}}
static void Effects(){for(int stage:{1,6,7}){Fixture f;g_GuiStage=stage;f.Program(21);Normal(f);Check(host.effects==std::vector<float>{stage==6?8.f:2.f},"effects.magnitude");}}
static void Exit(){for(int op:{22,23}){Fixture f;f.Program(op);Normal(f);Check(f.vms[op-22].pendingInterrupt==7&&f.message.vmIds[op-22].value==101+op-22,"exit.interrupt_keep_id");}}
static void Control(){
 for(int state:{0,3}){Fixture f;f.Program(0,{},false);g_GuiMessageState=state;f.message.inputCooldown=2;Check(GuiMessageVmView::Run(&f.message)==-1,"end.result");Check(f.message.currentInstruction==f.Instruction()&&f.message.instructionTimer.current==0&&f.message.instructionTimer.previous==-1,"end.no_advance_tick");Check(g_GuiMessageState==0&&g_GuiMessageStateAux==(state?0:5)&&f.message.inputCooldown==1,"end.state");}
 for(unsigned char op:{24,255}){Fixture f;f.Program(op);Normal(f);Check(host.order.empty(),"unknown.no_effects");}
 {Fixture f;f.Program(11,{},true,2);Check(GuiMessageVmView::Run(&f.message)==0,"future.result");Check(f.message.currentInstruction==f.Instruction()&&f.message.instructionTimer.current==1&&f.message.inputCooldown==0,"future.tick_only");}
 {Fixture f;f.Program(11,{},true,17);f.message.flags=1;g_GuiInputCurrent=0x100;Check(GuiMessageVmView::Run(&f.message)==0&&f.message.inputCooldown==1&&f.message.instructionTimer.current==18,"skip.record_time");}
 {Fixture f;f.first=f.Append(9,{1});f.Append(11);f.future=f.Append(0,{},100);f.message.currentInstruction=f.Instruction();Normal(f);Check(f.message.flags==1&&f.message.inputCooldown==1,"multiple.odd_offset");}
 {Fixture f;f.first=f.Append(9,{1});f.Append(10,Fixture::Int(7));f.future=f.Append(0,{},100);f.message.currentInstruction=f.Instruction();g_GuiInputEdges=1;Normal(f);Check(Sounds()==std::vector<int>{0},"multiple.odd_int_payload");}
 {Fixture f;f.first=f.Append(3);f.Append(11);auto other=f.Append(24);f.future=f.Append(0,{},100);f.message.currentInstruction=f.Instruction();host.afterCreate=[&](){f.message.currentInstruction=f.bytes.data()+other;};Normal(f);Check(f.message.inputCooldown==0,"dispatch.tail_reload");}
 {Fixture f;f.Program(11);f.message.instructionTimer.SetCurrent(-1);f.message.inputCooldown=-2;Check(GuiMessageVmView::Run(&f.message)==0&&f.message.currentInstruction==f.Instruction()&&f.message.instructionTimer.current==0&&f.message.inputCooldown==-2,"timer.signed_current");}
 for(float scale:{.99f,1.f,1.01f}){Fixture f;g_AnmGameSpeed=scale;f.Program(24);Check(GuiMessageVmView::Run(&f.message)==0,"timer.result");Check(f.message.instructionTimer.current==static_cast<int>(scale)&&f.message.instructionTimer.subframe==scale,"timer.strict_bounds");}}
int main(int argc,char**argv){Check(CHAR_BIT==8&&sizeof(unsigned short)==2&&sizeof(int)==4,"host.scalar_widths");unsigned n=1;Check(*reinterpret_cast<unsigned char*>(&n)==1,"host.endian");Check(argc==2,"selection.required");std::string s=argv[1];if(s=="control")Control();else if(s=="portraits")Portraits();else if(s=="sides")Sides();else if(s=="hides")Hides();else if(s=="flags")Flags();else if(s=="wait")Wait();else if(s=="sprites")Sprites();else if(s=="text")Text();else if(s=="music")Music();else if(s=="names")Names();else if(s=="complete")Complete();else if(s=="effects")Effects();else if(s=="exit")Exit();else Check(false,"selection.unknown");return 0;}
