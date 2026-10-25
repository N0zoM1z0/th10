#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <vector>
#include <map>
#define __stdcall
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"GUI_LIFECYCLE_CHECK_FAILED line %d: %s\n",__LINE__,#x); std::fflush(stderr); std::_Exit(1); } } while(0)
struct AnmLoadedView { int tag; void Release(); };
struct AnmVmLayerNodeView {void *owner;AnmVmLayerNodeView *next,*previous;};
struct AnmVmIdView {int value;};
struct AnmVmView {int id;void *generatedVertices;unsigned flags35C;AnmVmLayerNodeView layerNode;AnmLoadedView *anmFile;~AnmVmView();};
struct AnmVmTimerView {int previous,current;float subframe;};
struct AnmFloat3View {float x,y,z;};
struct GuiMessageInstructionView {};
@GUI_DECLARATIONS@
// Host-only GUI setup seam; destructor/member tests intentionally exclude creation.
// Complete creation body is covered by test_th10_gui_creation_contracts.py.
GuiView::GuiView() {}

// HOST CONSTRUCTION SEAM (one definition): parameter matches the copied real
// declaration. This creates a live fixture object and initializes test state;
// it does not execute or validate the production message constructor/effects.
// The complete source-extracted message destructor below remains unchanged.
static unsigned char hostMessageRecord;
static int hostMessageConstructorCalls;
GuiMessageVmView::GuiMessageVmView(const unsigned char *record)
    : messageIndex(0), lifetimeTimer(), instructionTimer(), waitTimer(),
      vmIds(), unknown58(0), currentInstruction(NULL), textPositions(),
      inputCooldown(0), flags(0), alternatingLine(0), activeSide(0), textColors()
{
    CHECK(record==&hostMessageRecord);
    ++hostMessageConstructorCalls;
}
namespace th10 {
struct ChainElem {int prio;unsigned flags;};
struct Chain {int tag;static void UnregisterElem(ChainElem *,Chain *);};
Chain *g_Chain;
}
struct AnmRenderManagerView {
 AnmLoadedView *loadedAnms[33]; AnmVmLayerNodeView *primaryVmListHead,*secondaryVmListHead;
 AnmVmView *FindVm(AnmVmIdView);void MarkVmForDeletion(AnmVmIdView);void MarkActual(AnmVmIdView);
 void MarkLoadedVmsForDeletion(AnmLoadedView *);void MarkLoadedActual(AnmLoadedView *);
};
struct MainView {int criticalSections[7];unsigned char criticalSectionLockCounts[7];} g_MainSupervisorView;
AnmRenderManagerView *g_AnmRenderManagerView;
GuiView *g_GuiView;
unsigned g_GuiGameFlags;
void *g_GuiRetainedMessageFile491BE8;
static GuiView *active;
static AnmRenderManagerView managers[3];
static th10::Chain chains[3];
static th10::Chain *firstFixtureChain,*expectedChain;
static th10::ChainElem updateElem,drawElem;
static AnmLoadedView oldLoaded,newLoaded,stageLoaded,frontLoaded;
static AnmVmView valueVms[8];
static AnmVmLayerNodeView nodes[8],children[3];
static std::map<void *,int> allocations;
static std::vector<int> freedMembers,events,memberDestructors,expectedMemberFrees;
static std::map<AnmVmView *,int> memberTags;
static void *originalDraw;
static unsigned initialFixtureFlags;
static unsigned char depth;
static int scenarioEffect,enters,messageMarks,marks,fileFrees,loadedFrees,messageFrees;
static bool held,expectMemberTeardown,expectValueInjection;
static GuiMessageVmView *messageAllocation;
static AnmLoadedView **releasedSlot;
static AnmRenderManagerView *messageManager,*specialManager,*destructorManager,*resourceManager;
static unsigned expectedFlags;
static bool finalStateChecked;
static void HostFree(void *);
static void ObserveGuiEnd(GuiView *);
static void ObserveMemberDestructor(AnmVmView *p) {if(active!=NULL && expectMemberTeardown)memberDestructors.push_back(memberTags.count(p)?memberTags[p]:-1);}
static void HostDeleteAllocation(AnmLoadedView *);
static void HostDeleteMessage(GuiMessageVmView *);
static void ObserveMessageEnd(GuiMessageVmView *p) {bool messageIdsClear=true;for(int i=0;i<6;++i)messageIdsClear &= p->vmIds[i].value==0;CHECK(messageIdsClear);}
static void EnterCriticalSection(int *);
static void LeaveCriticalSection(int *);
#define free HostFree
@DEPENDENCIES@
@LIFECYCLE@
#undef free

static void Remember(void *p,int tag) { CHECK(p != NULL);CHECK(allocations.count(p)==0);allocations[p]=tag; }
static void *Alloc(int tag) {void *p=std::malloc(8);Remember(p,tag);return p;}
static void ConfigureValues() {
 for(int i=0;i<8;++i) {valueVms[i].id=101+i;valueVms[i].flags35C=0x81;valueVms[i].layerNode={NULL,NULL,NULL};valueVms[i].anmFile=NULL;nodes[i]={&valueVms[i],NULL,NULL};}
 // primary101 root, primary103 wins over secondary duplicate103; secondary102 child.
 nodes[0].next=&nodes[2];nodes[2].next=&nodes[7];nodes[1].next=&nodes[3];valueVms[3].id=103;valueVms[7].id=0;
 children[0]={&valueVms[4],&children[1],NULL};children[1]={&valueVms[5],NULL,NULL};children[2]={&valueVms[6],NULL,NULL};
 valueVms[0].layerNode.next=&children[0];valueVms[1].layerNode.previous=&nodes[0];valueVms[1].layerNode.next=&children[2];
 managers[0].primaryVmListHead=managers[1].primaryVmListHead=managers[2].primaryVmListHead=&nodes[0];
 managers[0].secondaryVmListHead=managers[1].secondaryVmListHead=managers[2].secondaryVmListHead=&nodes[1];
 const int ids[8]={101,102,103,0,999,103,102,101};
 for(int i=0;i<8;++i)active->valueVmIds[i].value=ids[i];
}
static void CheckValues() {
 bool valueIdsClear=true;for(int i=0;i<8;++i)valueIdsClear &= active->valueVmIds[i].value==0;CHECK(valueIdsClear);
 bool valueEffectsCorrect=true;for(int i=0;i<8;++i)valueEffectsCorrect &= valueVms[i].flags35C==((i==3 || i==6 || i==7)?0x81u:0x04000081u);CHECK(valueEffectsCorrect);
 bool segmentsClear=true;for(int i=0;i<10;++i)segmentsClear &= active->bossGaugeSegmentVmIds[i].value==0;CHECK(segmentsClear);
 bool segmentCountClear=active->bossGaugeSegmentCount==0;CHECK(segmentCountClear);
}
void AnmLoadedView::Release() {
 CHECK(this==&oldLoaded);CHECK(!held);events.push_back(500);
 releasedSlot=&managers[0].loadedAnms[28];
 if(scenarioEffect==1){*releasedSlot=&newLoaded;g_AnmRenderManagerView=&managers[1];}
}
static void HostDeleteAllocation(AnmLoadedView *p) {
 bool releasedCorrectSlot=releasedSlot && p==*releasedSlot;CHECK(releasedCorrectSlot);
 CHECK(p==&oldLoaded || p==&newLoaded);++loadedFrees;events.push_back(501);
 // Loaded resources are static valid seam objects. This boundary records delete;
 // actual AnmLoadedView::Release/heap ownership is not executed in this fixture.
}
void AnmRenderManagerView::MarkVmForDeletion(AnmVmIdView id) {
 events.push_back(id.value);
 if(id.value>=600 && id.value<606) {
  if(messageMarks==0)messageManager=this;
  bool messageManagerStable=this==messageManager;CHECK(messageManagerStable);
  ++messageMarks;
  if(scenarioEffect==2){g_AnmRenderManagerView=&managers[2];g_GuiGameFlags=9;}
 }
 if(id.value==201){specialManager=this;if(scenarioEffect==3)g_AnmRenderManagerView=&managers[1];}
 if(id.value==202){resourceManager=this;CHECK(this==g_AnmRenderManagerView);if(scenarioEffect==5)g_AnmRenderManagerView=&managers[2];}
 if(id.value==203)CHECK(this==resourceManager);
 MarkActual(id);
 if(id.value==204) {
  destructorManager=this;CheckValues();
  if(expectValueInjection){ConfigureValues();managers[2].primaryVmListHead=NULL;managers[2].secondaryVmListHead=NULL;g_AnmRenderManagerView=&managers[2];}
 }
 ++marks;
}
void AnmRenderManagerView::MarkLoadedVmsForDeletion(AnmLoadedView *p) {
 CHECK(!held);events.push_back(p==&frontLoaded?701:700);
 if(p==&frontLoaded){bool destructorManagerStable=this==destructorManager;CHECK(destructorManagerStable);CheckValues();CHECK(active->specialVmId9E50.value==0);}
 MarkLoadedActual(p);
}
static void HostDeleteMessage(GuiMessageVmView *p) {
 CHECK(p==messageAllocation);CHECK(active->stageUiAnm==NULL);p->~GuiMessageVmView();
 CHECK(allocations.erase(p)==1);::operator delete(p);messageAllocation=NULL;++messageFrees;events.push_back(610);
}
static void EnterCriticalSection(int *p) {
 CHECK(p==&g_MainSupervisorView.criticalSections[0]);CHECK(!held);held=true;
 expectedChain=th10::g_Chain;th10::g_Chain=&chains[(enters+1)%3];++enters;events.push_back(800);
}
void th10::Chain::UnregisterElem(th10::ChainElem *p,th10::Chain *chain) {
 CHECK(held);CHECK(chain == expectedChain);
 bool countCorrect=g_MainSupervisorView.criticalSectionLockCounts[0]==static_cast<unsigned char>(depth+1);CHECK(countCorrect);
 CHECK(p==&updateElem || p==&drawElem);if(p==&updateElem)CHECK((p->flags&2)==0);events.push_back(p==&updateElem?801:802);
}
static void LeaveCriticalSection(int *p) {
 CHECK(p==&g_MainSupervisorView.criticalSections[0]);CHECK(held);
 bool countCorrect=g_MainSupervisorView.criticalSectionLockCounts[0]==static_cast<unsigned char>(depth+1);CHECK(countCorrect);
 held=false;events.push_back(803);
}
static void ObserveGuiEnd(GuiView *p) {
 CHECK(p==active);bool globalCleared=g_GuiView==NULL;CHECK(globalCleared);
 bool updateCleared=active->updateCallback==NULL;CHECK(updateCleared);
 CHECK(!held);CHECK(g_MainSupervisorView.criticalSectionLockCounts[0]==depth);
 CHECK(active->drawCallback==originalDraw);CHECK(active->stageUiAnm==NULL);CHECK(active->message==NULL);
 CHECK(active->specialVmId9E14.value==0 && active->specialVmId9E18.value==0 && active->bossGaugeVmId.value==0);
 CheckValues();finalStateChecked=true;
}
static void HostFree(void *p) {
 CHECK(allocations.count(p)==1);int tag=allocations[p];
 if(tag<43){
  CHECK(expectMemberTeardown);CHECK(finalStateChecked);CHECK(g_GuiView==NULL);
  freedMembers.push_back(tag);
 }else{CHECK(tag==900);++fileFrees;events.push_back(900);}
 allocations.erase(p);std::free(p);
}
static void Initialize(unsigned flags,int callbackMask,bool haveMessage,bool haveLoaded,bool haveFile,int effect) {
 CHECK(allocations.empty());for(int i=0;i<3;++i)managers[i]={};
 active=new GuiView();g_GuiView=active;g_GuiGameFlags=initialFixtureFlags=flags;
 g_AnmRenderManagerView=&managers[0];th10::g_Chain=firstFixtureChain=&chains[0];
 updateElem={0,0xb};drawElem={0,0xd};active->updateCallback=(callbackMask&1)?&updateElem:NULL;active->drawCallback=(callbackMask&2)?&drawElem:NULL;originalDraw=active->drawCallback;
 managers[0].loadedAnms[28]=haveLoaded?&oldLoaded:NULL;active->stageUiAnm=&stageLoaded;active->frontAnm=&frontLoaded;
 active->specialVmId9E14.value=201;active->specialVmId9E18.value=202;active->bossGaugeVmId.value=203;active->specialVmId9E50.value=204;
 for(int i=0;i<10;++i) {active->bossGaugeSegmentVmIds[i].value=300+i;}active->bossGaugeSegmentCount=10;
 active->messageFile=haveFile?Alloc(900):NULL;g_GuiRetainedMessageFile491BE8=&oldLoaded;
 hostMessageConstructorCalls=0;
 messageAllocation=haveMessage?new GuiMessageVmView(&hostMessageRecord):NULL;active->message=messageAllocation;
 CHECK(hostMessageConstructorCalls==(haveMessage?1:0));
 if(messageAllocation){Remember(messageAllocation,950);for(int i=0;i<6;++i)messageAllocation->vmIds[i].value=600+i;}
 memberTags.clear();memberDestructors.clear();
 int tag=0;for(auto &v:active->displayVms0) {memberTags[&v]=tag;v.generatedVertices=Alloc(tag++);}for(auto &v:active->displayVms1) {memberTags[&v]=tag;v.generatedVertices=Alloc(tag++);}
 for(auto &v:active->displayVms2) {memberTags[&v]=tag;v.generatedVertices=Alloc(tag++);}for(auto &v:active->displayVms3) {memberTags[&v]=tag;v.generatedVertices=Alloc(tag++);}
 for(auto &v:active->displayVms4) {memberTags[&v]=tag;v.generatedVertices=Alloc(tag++);}for(auto &v:active->displayVms5) {memberTags[&v]=tag;v.generatedVertices=Alloc(tag++);}memberTags[&active->bossPositionVm]=tag;active->bossPositionVm.generatedVertices=Alloc(tag++);CHECK(tag==43);
 scenarioEffect=effect;enters=messageMarks=marks=fileFrees=loadedFrees=messageFrees=0;held=false;expectMemberTeardown=true;expectValueInjection=(effect==4);releasedSlot=NULL;messageManager=specialManager=NULL;
 events.clear();freedMembers.clear();depth=(effect%2)?255:7;g_MainSupervisorView.criticalSectionLockCounts[0]=depth;finalStateChecked=false;
 ConfigureValues();
 expectedMemberFrees.clear();
 for(auto item:memberTags){if(effect==7 || (effect==6 && item.second%2==0)){CHECK(allocations.erase(item.first->generatedVertices)==1);std::free(item.first->generatedVertices);item.first->generatedVertices=NULL;}}
 for(int t=42;t>=0;--t)for(auto item:memberTags)if(item.second==t && item.first->generatedVertices)expectedMemberFrees.push_back(t);
}
static void Exercise(unsigned flags,int mask,bool message,bool loaded,bool file,int effect) {
 Initialize(flags,mask,message,loaded,file,effect);void *savedFile=active->messageFile;
 // Real destructor invocation is separate from matching owner allocation release.
 active->~GuiView();::operator delete(active);active=NULL;
 CHECK(finalStateChecked || expectedMemberFrees.empty());bool memberDestructionCountCorrect=memberDestructors.size()==43;CHECK(memberDestructionCountCorrect);
 for(int i=0;i<43;++i) {CHECK(memberDestructors[i]==42-i);}memberTags.clear();CHECK(freedMembers==expectedMemberFrees);
 CHECK(enters==((mask&1)?1:0)+((mask&2)?1:0));CHECK(messageFrees==(message?1:0));CHECK(messageMarks==(message?6:0));
 CHECK(loadedFrees==(((flags&9)==0 && loaded)?1:0));
 const bool retain=(g_GuiGameFlags&9)!=0;
 bool fileOwnershipCorrect=fileFrees==((!retain&&file)?1:0) && g_GuiRetainedMessageFile491BE8==(retain?savedFile:NULL);CHECK(fileOwnershipCorrect);
 std::vector<int> expectedEvents;
 if((flags&9)==0){if(loaded){expectedEvents.push_back(500);expectedEvents.push_back(501);}}else expectedEvents.push_back(700);
 if(message){for(int i=0;i<6;++i)expectedEvents.push_back(600+i);expectedEvents.push_back(610);}
 if(!retain&&file)expectedEvents.push_back(900);
 expectedEvents.push_back(201);expectedEvents.push_back(202);expectedEvents.push_back(203);
 if(mask&1){expectedEvents.push_back(800);expectedEvents.push_back(801);expectedEvents.push_back(803);}
 if(mask&2){expectedEvents.push_back(800);expectedEvents.push_back(802);expectedEvents.push_back(803);}
 expectedEvents.push_back(204);expectedEvents.push_back(701);CHECK(events == expectedEvents);
 if((flags&9)==0)CHECK(managers[0].loadedAnms[28]==NULL);
 if(retain&&file){CHECK(allocations.erase(savedFile)==1);std::free(savedFile);}
 CHECK(allocations.empty());CHECK(g_GuiView==NULL);
}
int main() {
 for(unsigned f: {0u,1u,8u,9u})for(int mask=0;mask<4;++mask)for(int opts=0;opts<8;++opts)Exercise(f,mask,opts&1,opts&2,opts&4,0);
 Exercise(0,3,true,true,true,1);Exercise(0,3,true,true,true,2);Exercise(0,3,true,true,true,3);Exercise(0,3,true,true,true,4);Exercise(0,3,true,true,true,5);Exercise(0,3,true,true,true,6);Exercise(0,3,true,true,true,7);
 std::puts("PASS 135 full lifecycle scenarios, 43 natural member destructors each, null/mixed/full vertex allocations, ownership/flags/lock reloads");
}
