#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#include "Main.hpp"
#include "Ending.hpp"
#include "InputState.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "EnemySoundQueue.hpp"
#include "TransitionEffect.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <new>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <atomic>
using u8=unsigned char; using u32=unsigned int;
static bool replaceTextScriptOnAllocation;static unsigned char *textAllocationReplacement;
static void *freed[20000];static unsigned freedCount;
static void RecordFree(void *p){if(p){if(freedCount==20000)std::abort();freed[freedCount++]=p;}}
void *operator new(std::size_t n){void *p=std::malloc(n?n:1);if(!p)throw std::bad_alloc();return p;}
void operator delete(void *p) noexcept {RecordFree(p);std::free(p);}
void operator delete(void *p,std::size_t) noexcept {::operator delete(p);}
// HOST explicit target allocation event; constructor cleanup is in the host copy.
static void *HostAllocateEndingText(EndingView *owner){void *p=::operator new(sizeof(EndingTextVmView));if(replaceTextScriptOnAllocation){replaceTextScriptOnAllocation=false;owner->scriptData=textAllocationReplacement;}return p;}
static void HostFree(void *p){RecordFree(p);std::free(p);}
static bool WasFreed(void *p,unsigned since){for(unsigned i=since;i<freedCount;++i)if(freed[i]==p)return true;return false;}
struct Failure{const char *label;};
#define CHECK(x,label) do{if(!(x))throw Failure{label};}while(0)
static unsigned char decoded[1024];
#define LEAF_GLOB(address) (::decoded)
static void _ReadWriteBarrier(){std::atomic_signal_fence(std::memory_order_seq_cst);}
struct EnemySoundCueMetadataView {int unknown000;unsigned short unknown004;short cueValue;};
EnemySoundCueMetadataView g_EnemySoundCueMetadata[134];
EndingView *g_EndingView;
AnmRenderManagerView::AnmRenderManagerView(){}
AnmRenderManagerView::~AnmRenderManagerView(){}
AnmVmView::AnmVmView(){std::memset(this,0,sizeof(*this));}
AnmVmView::~AnmVmView(){}
AnmLoadedView::AnmLoadedView(){std::memset(this,0,sizeof(*this));}
AsciiManagerView::AsciiManagerView(){}
AsciiManagerView::~AsciiManagerView(){}
size_t AsciiManagerView::GetSize(){return sizeof(*this);}
static AnmRenderManagerView renderer;
static AsciiManagerView ascii,otherAscii;
static AnmLoadedView fontBank,asciiBank;
AnmRenderManagerView *g_AnmRenderManagerView=&renderer;
AsciiManagerView *g_AsciiManagerView=&ascii;
MainSupervisorView g_MainSupervisorView;
FrontEndInputView g_FrontEndInput;
float g_AnmGameSpeed=1;
namespace th10 {Chain chain;Chain *g_Chain=&chain;GameErrorContext g_GameErrorContext;}
alignas(EnemySoundQueueView) unsigned char g_MainSoundOwner[0x52d0];
char g_StageFilePath[260];char g_EndingDataErrorMessage[]="ending asset error";
const char *g_EndingScriptPaths[12]={"e00.msg","e01.msg","e02.msg","e03.msg","e04.msg","e05.msg","e06.msg","e07.msg","e08.msg","e09.msg","e10.msg","e11.msg"};
static unsigned char persistent[0x1d890];unsigned char *g_GuiPersistentData=persistent;
int g_GuiCharacter,g_GuiShotType,g_GuiDifficulty,g_GameStartupValue474C90,g_GameStartupCounter;
static AnmVmView hostVmPool[64];static AnmVmLayerNodeView nodes[64];static int vmCount;
struct Allocation{void *pointer;unsigned fence;};static std::vector<Allocation> buffers,banks;
static std::vector<std::string> drawn;static std::vector<int> effects,music;
static std::vector<th10::ChainElem*> tasks;static int registrationCount,closed,logs,ensureCount;
static void *threadContext;static unsigned(*threadEntry)(void*);
static std::function<unsigned char*(const char*)> openAction;
static std::function<void()> drawAction,createAction,preloadAction,selectionAction;
static AnmLoadedView *preloadResult;static int preloadSlot;
static unsigned char *NewBuffer(unsigned n){auto *p=(unsigned char*)std::malloc(n);std::memset(p,0,n);buffers.push_back({p,freedCount});return p;}
static AnmLoadedView *NewBank(){auto *p=new AnmLoadedView;banks.push_back({p,freedCount});return p;}
static void Clean(){
 for(auto a:buffers){if(!WasFreed(a.pointer,a.fence))HostFree(a.pointer);}
 buffers.clear();
 for(auto a:banks){if(!WasFreed(a.pointer,a.fence))delete static_cast<AnmLoadedView*>(a.pointer);}
 banks.clear();
}
static void Reset(){
 Clean();renderer.primaryVmListHead=renderer.secondaryVmListHead=nullptr;
 std::memset(renderer.loadedAnms,0,sizeof(renderer.loadedAnms));vmCount=0;
 std::memset(&g_FrontEndInput,0,sizeof(g_FrontEndInput));std::memset(persistent,0,sizeof(persistent));
 std::memset(g_MainSoundOwner,0,sizeof(g_MainSoundOwner));new(g_MainSoundOwner)EnemySoundQueueView;
 g_AsciiManagerView=&ascii;ascii.asciiAnm=&asciiBank;ascii.textAnm=&fontBank;ascii.auxiliaryVm89A4=0;otherAscii.asciiAnm=&asciiBank;otherAscii.textAnm=&fontBank;otherAscii.auxiliaryVm89A4=777;replaceTextScriptOnAllocation=false;textAllocationReplacement=nullptr;
 g_GuiCharacter=g_GuiShotType=g_GuiDifficulty=g_GameStartupValue474C90=0;g_GameStartupCounter=9;g_AnmGameSpeed=1;
 drawn.clear();effects.clear();music.clear();tasks.clear();registrationCount=closed=logs=ensureCount=0;
 drawAction=createAction=preloadAction=selectionAction=nullptr;openAction=nullptr;preloadResult=nullptr;threadContext=nullptr;threadEntry=nullptr;
}
static unsigned char *Record(unsigned op,int arg=0,unsigned frame=0){
 unsigned n=(op==0||op==1||op==2||op==4)?4:(op==7?24:16);auto *p=NewBuffer(n+4);
 unsigned short time=(unsigned short)frame;std::memcpy(p,&time,2);p[2]=(unsigned char)op;p[3]=(unsigned char)(n-4);
 if(n>4){std::memcpy(p+4,&arg,4);}
 unsigned short future=1000;std::memcpy(p+n,&future,2);return p;
}
static void Arg(unsigned char *p,int index,int value){std::memcpy(p+4+index*4,&value,4);}
static void HostClearEndingText(EndingTextVmView *vm){
 void *vptr;std::memcpy(&vptr,&vm->worker,sizeof(vptr));
 std::memset(static_cast<void*>(vm),0,sizeof(*vm));std::memcpy(&vm->worker,&vptr,sizeof(vptr));
}
extern "C" void EnterCriticalSection(CRITICAL_SECTION*){}
extern "C" void LeaveCriticalSection(CRITICAL_SECTION*){}
extern "C" void Sleep(unsigned int){}
extern "C" unsigned int WaitForSingleObject(void*,unsigned int){return 0;}
extern "C" int CloseHandle(void*){++closed;return 1;}
static std::uintptr_t _beginthreadex(void*,unsigned,unsigned(*entry)(void*),void *context,unsigned,unsigned *id){threadEntry=entry;threadContext=context;*id=37;return 0x1234;}
namespace FileSystem {unsigned char *OpenFile(const char *path,int *size,int mode){CHECK(!size&&mode==0,"file-contract");return openAction?openAction(path):nullptr;}}
char *th10::GameErrorContext::Log(char *fmt,...){CHECK(fmt==g_EndingDataErrorMessage,"error-role");++logs;return fmt;}
AnmVmView *AnmRenderManagerView::AllocateVm(){CHECK(vmCount<64,"vm-capacity");AnmVmView *vm=&hostVmPool[vmCount];std::memset(vm,0,sizeof(*vm));vm->id=++vmCount;return vm;}
AnmVmIdView AnmRenderManagerView::AddVmVariant0(AnmVmView *vm){auto &n=nodes[vm->id-1];n.owner=vm;n.next=primaryVmListHead;n.previous=nullptr;primaryVmListHead=&n;return AnmVmIdView(vm->id);}
void AnmLoadedView::InitializeVm(AnmVmView *vm,int script){if(createAction)createAction();vm->scriptIndex=(short)script;vm->anmFile=this;}
AnmVmIdView AnmLoadedView::CreateVmAtScreenVariant0(int script,const AnmFloat3View *p){CHECK(script==6&&p->x==480&&p->y==392&&p->z==0,"selection-position");if(selectionAction)selectionAction();auto *vm=renderer.AllocateVm();return renderer.AddVmVariant0(vm);}
AnmVmIdView AnmRenderManagerView::AddVmVariant1(AnmVmView *vm){auto &n=nodes[vm->id-1];n.owner=vm;n.next=secondaryVmListHead;n.previous=nullptr;secondaryVmListHead=&n;return AnmVmIdView(vm->id);}
void AnmLoadedView::Release(){}
void AnmRenderManagerView::ReleaseSurface(int index){CHECK(index==0,"surface-index");}
AnmLoadedView *AnmRenderManagerView::PreloadAnm(int index,const char *path){CHECK(std::string(path)=="bank.anm","worker-path");preloadSlot=index;if(preloadAction)preloadAction();loadedAnms[index]=preloadResult;return preloadResult;}
void AnmRenderManagerView::DrawTextLeft(AnmVmView *vm,unsigned int color,const char *fmt,...){CHECK(vm!=nullptr&&color==0xffffff,"text-role");drawn.emplace_back(fmt);if(drawAction)drawAction();}
int AsciiManagerView::EnsureSelectionVm(float x,float y){CHECK(x==480&&y==392,"ensure-position");++ensureCount;return 0;}
int FrontEndLoadMusic(int channel,const char*){CHECK(channel==0,"music-channel");music.push_back(-1);return 0;}
int FrontEndPlayMusic(int channel,int track){CHECK(channel==0,"music-channel");music.push_back(track);return 0;}
int GameQueueScaledSoundCommand(float scale){CHECK(scale==3,"sound-scale");return 0;}
void *StageCreateTransitionEffect(int priority,int kind,int frames,unsigned a,unsigned b,unsigned c){CHECK(priority==49&&!a&&!b&&!c,"effect-contract");effects.push_back(kind);effects.push_back(frames);return nullptr;}
#include "dependencies.inc"
#define free HostFree
#include "owner.inc"
#undef free
int th10::Chain::AddToCalcChain(ChainElem *e,int priority,Chain *c){CHECK(priority==28&&c==g_Chain&&e->flags==0xa5a5a5a7u&&e->arg==g_EndingView,"calc-registration");e->prio=priority;tasks.push_back(e);++registrationCount;return 0;}
int th10::Chain::AddToDrawChain(ChainElem *e,int priority,Chain *c){CHECK(priority==6&&c==g_Chain&&e->callback==reinterpret_cast<ChainCallback>(th10::leaf::Fn0040BA80),"draw-registration");e->prio=priority;tasks.push_back(e);++registrationCount;return 0;}
void th10::Chain::UnregisterElem(ChainElem *e,Chain *c){CHECK(c==g_Chain,"unregister-chain");delete static_cast<EndingTaskView*>(e);}
static void RunTests(){
 Reset();{EndingView owner;auto *r=Record(0);auto *vm=new EndingTextVmView(r);owner.text=vm;CHECK(vmCount==5,"text-constructor-count");for(int i=0;i<5;++i){CHECK(hostVmPool[i].glyphWidth==16&&hostVmPool[i].glyphHeight==16&&(hostVmPool[i].textFlags360&2),"text-constructor-style");}CHECK(vm->Run()==-1,"terminator");vm->flags|=4;vm->instruction=nullptr;CHECK(vm->Run()==0,"pending-gate");}
 Reset();{EndingView owner;auto *r=Record(1);auto *vm=new EndingTextVmView(r);owner.text=vm;vm->Run();CHECK(vm->instruction==r+4&&vm->instructionTimer.current==1,"header-only-record");}
 Reset();{EndingView owner;auto *r=Record(3);auto *live=Record(3);r[4]=0x36;r[5]=0x7e;live[4]=0x35;live[5]=0x7e;owner.text=new EndingTextVmView(r);bool once=true;drawAction=[&]{if(once){once=false;owner.text->instruction=live;}};owner.text->Run();CHECK(drawn.size()==6&&drawn.back()=="B"&&owner.text->textLine==1,"text-live-cursor");CHECK(hostVmPool[0].pendingInterrupt==2&&hostVmPool[1].pendingInterrupt==3,"text-interrupts");}
 Reset();{EndingView owner;owner.flags=1;auto *r=Record(5,20);owner.text=new EndingTextVmView(r);g_FrontEndInput.pressed=1;owner.text->Run();CHECK(owner.text->instruction==r+16&&owner.text->waitTimer.current==0,"wait-pressed-edge");CHECK(reinterpret_cast<EnemySoundQueueView*>(g_MainSoundOwner)->sampleCounts[0]==1,"wait-sound");}
 Reset();{EndingView owner;owner.flags=1;auto *r=Record(5,-1);owner.text=new EndingTextVmView(r);owner.text->Run();CHECK(owner.text->instruction==r&&owner.text->waitTimer.current==999,"negative-wait");}
 Reset();{EndingView owner;auto *r=Record(6,7);owner.text=new EndingTextVmView(r);owner.text->textLine=3;g_FrontEndInput.current=0x100;owner.text->Run();CHECK(owner.text->instruction==r+16&&owner.text->textLine==0&&g_GameStartupCounter==0,"six-frame-skip");}
 for(int op: {8,15,16,17}){Reset();EndingView owner;g_GuiDifficulty=(op==8)?1:op-14;auto *r=Record(op,1);Arg(r,1,0);Arg(r,2,42);auto *live=Record(9,2);owner.text=new EndingTextVmView(r);owner.text->resources[0]=&fontBank;createAction=[&]{owner.text->instruction=live;};owner.text->Run();CHECK(owner.text->animationIds[1].value==0&&owner.text->animationIds[2].value>0,"animation-live-destination");CHECK(renderer.secondaryVmListHead==nullptr,"animation-primary-chain");auto *created=renderer.FindVm(owner.text->animationIds[2]);CHECK(created&&created->scriptIndex==42&&created->renderLayer==15&&(created->flags35C&0x40000000u),"animation-arguments");}
 Reset();{EndingView owner;auto *r=Record(12);owner.text=new EndingTextVmView(r);auto *loaded=NewBuffer(28);int offset=8;std::memcpy(loaded+4,&offset,4);auto *first=Record(9,0xabcdef);std::memcpy(loaded+8,first,20);openAction=[&](const char*){return loaded;};owner.text->Run();CHECK(owner.text->color==0xabcdef&&owner.text->instruction==loaded+24,"reload-first-command");CHECK(owner.text->flags==2&&owner.text->frameTimer.current==0,"reload-timers");}
 Reset();{EndingView owner;auto *r=Record(7,1);std::memcpy(r+8,"bank.anm",9);owner.text=new EndingTextVmView(r);owner.text->Run();CHECK(ensureCount==1&&owner.text->flags&4&&threadContext==owner.text&&threadEntry!=nullptr&&owner.text->instruction==r+24,"worker-launch");auto *live=Record(9,2);owner.text->instruction=Record(9,1);preloadResult=NewBank();preloadAction=[&]{owner.text->instruction=live;};EndingLoadAnimationCallback();CHECK(preloadSlot==30&&owner.text->resources[2]==preloadResult&&(owner.text->flags&4)==0,"worker-late-cursor");}
 Reset();{EndingView owner;auto *old=NewBuffer(8);owner.scriptData=old;auto *replacement=NewBuffer(8);auto *loaded=NewBuffer(8);openAction=[&](const char*){owner.scriptData=replacement;return loaded;};unsigned fence=freedCount;CHECK(owner.LoadScript("x.msg")==loaded&&WasFreed(replacement,fence)&&!WasFreed(old,fence),"load-live-buffer");}
 Reset();{EndingView owner;owner.scriptData=NewBuffer(8);auto *old=owner.scriptData;unsigned fence=freedCount;CHECK(owner.LoadScript("missing.msg")==nullptr&&owner.scriptData==nullptr&&WasFreed(old,fence),"load-failure-releases-current");}
 Reset();{EndingView owner;owner.text=new EndingTextVmView(Record(1));owner.flags=0;owner.text->flags=2;g_FrontEndInput.current=0x100;for(int i=1;i<=12;++i)CHECK(owner.Update()==(i%12?6:1),"fast-update-cycle");owner.text->instruction=Record(0);g_MainSupervisorView.flags|=0x1000;CHECK(owner.Update()==1&&g_MainSupervisorView.nextGameMode==2,"next-mode");}
 Reset();{auto *file=NewBuffer(12);int offset=8;std::memcpy(file+4,&offset,4);unsigned short future=1000;std::memcpy(file+8,&future,2);openAction=[&](const char *p){CHECK(std::string(p)=="e08.msg","ending-path");return file;};g_GuiShotType=2;g_GuiDifficulty=1;EndingView *owner=EndingCreate();CHECK(owner&&owner->endingIndex==8&&registrationCount==2&&owner->flags==3&&persistent[0x1d88a]&&persistent[0x1d88e],"ending-initialization");EndingDelete(owner);CHECK(!g_EndingView,"ending-delete");}
 Reset();{EndingView *owner=EndingCreate();CHECK(!owner&&logs==1&&!g_EndingView,"failed-file-cleanup");}
 Reset();{auto *file=NewBuffer(12);int offset=8;std::memcpy(file+4,&offset,4);unsigned short future=1000;std::memcpy(file+8,&future,2);EndingView owner;openAction=[&](const char*){return file;};selectionAction=[&]{g_AsciiManagerView=&otherAscii;};CHECK(owner.Initialize()==0,"selection-initialize");CHECK(ascii.auxiliaryVm89A4!=0&&otherAscii.auxiliaryVm89A4==777,"selection-original-owner");}
 Reset();{auto *old=NewBuffer(12);auto *replacement=NewBuffer(24);int oldOffset=8,newOffset=16;std::memcpy(old+4,&oldOffset,4);std::memcpy(replacement+4,&newOffset,4);unsigned short future=1000;std::memcpy(old+8,&future,2);std::memcpy(replacement+16,&future,2);EndingView owner;openAction=[&](const char*){textAllocationReplacement=replacement;replaceTextScriptOnAllocation=true;return old;};CHECK(owner.Initialize()==0,"allocation-initialize");CHECK(!replaceTextScriptOnAllocation&&owner.text->instruction==replacement+16,"allocation-live-header");}
 Clean();
}
int main(){try{RunTests();}catch(Failure failure){Clean();std::fprintf(stderr,"CHECK %s\n",failure.label);return 1;}Clean();return 0;}
