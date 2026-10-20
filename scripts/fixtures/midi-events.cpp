#include "Midi.hpp"
#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <string>
#include <vector>
struct Failure {const char *label;};
#define CHECK(x,label) do{if(!(x))throw Failure{label};}while(0)
static MidiOutputView seq;
static MidiTrackView a[3],b[3];
static int handles[2];
static std::vector<std::string> events;
static std::vector<DWORD> messages;
static std::vector<HMIDIOUT> shortHandles,longHandles;
static std::vector<unsigned char> longBytes;
static std::function<void(const std::string&)> after;
static std::map<void*,size_t> allocations;
static unsigned int prepareResult,longResult;
static void event(const std::string&s){events.push_back(s);if(after)after(s);}
static void *HostMalloc(size_t size){void *p=std::malloc(size);CHECK(p!=NULL,"allocation-role");allocations[p]=size;event(size==sizeof(MIDIHDR)?"allocate-header":"allocate-data");return p;}
static void HostFree(void *p){CHECK(allocations.count(p)==1,"free-owned");bool header=allocations.at(p)==sizeof(MIDIHDR);if(header)CHECK(static_cast<MIDIHDR*>(p)->lpData==NULL,"clear-payload-before-header-free");event(header?"free-header":"free-data");allocations.erase(p);std::free(p);}
MMRESULT midiOutPrepareHeader(HMIDIOUT h,MIDIHDR *p,unsigned int size){CHECK(h==seq.outputHandle&&p!=NULL&&size==sizeof(*p),"prepare-arguments");event("prepare");return prepareResult;}
MMRESULT midiOutLongMsg(HMIDIOUT h,MIDIHDR *p,unsigned int size){CHECK(h==seq.outputHandle&&size==sizeof(*p),"long-live-handle");longHandles.push_back(h);longBytes.assign(reinterpret_cast<unsigned char*>(p->lpData),reinterpret_cast<unsigned char*>(p->lpData)+p->dwBufferLength);event("long");return longResult;}
MMRESULT midiOutUnprepareHeader(HMIDIOUT h,MIDIHDR *p,unsigned int size){CHECK(h==seq.outputHandle&&size==sizeof(*p),"unprepare-arguments");CHECK(std::find(seq.pendingLongHeaders,seq.pendingLongHeaders+32,p)==seq.pendingLongHeaders+32,"clear-before-unprepare");event("unprepare");return 1;}
MMRESULT midiOutShortMsg(HMIDIOUT h,DWORD value){shortHandles.push_back(h);messages.push_back(value&0xffffffu);event("short");return 1;}
MMRESULT midiOutReset(HMIDIOUT){event("device-reset");return 1;}
MMRESULT midiOutClose(HMIDIOUT){event("device-close");return 1;}
MMRESULT timeKillEvent(unsigned int id){CHECK(id==seq.timerId,"timer-id");event("kill-timer");return 1;}
MMRESULT timeEndPeriod(unsigned int period){CHECK(period==seq.timerPeriod,"timer-period");event("end-period");return 1;}
#include "Midi.cpp"
static void cleanup(){after={};for(const auto&p:allocations)std::free(p.first);allocations.clear();}
static void reset(){cleanup();seq={};std::memset(a,0,sizeof(a));std::memset(b,0,sizeof(b));seq.tracks=a;seq.trackCount=1;seq.ticksPerQuarterNote=480;seq.tempoMicroseconds=500000;seq.fadeVolume=1.0f;seq.outputHandle=&handles[0];prepareResult=longResult=0;events.clear();messages.clear();shortHandles.clear();longHandles.clear();longBytes.clear();}
static void input(std::vector<unsigned char>&v,MidiTrackView&track=a[0]){track.active=1;track.data=track.cursor=v.data();track.dataSize=v.size();}
static void process(){MidiOutputView::ProcessEvent(&seq,&a[0]);}
static bool has(const std::string&s){return std::find(events.begin(),events.end(),s)!=events.end();}
static int at(const std::string&s){auto p=std::find(events.begin(),events.end(),s);CHECK(p!=events.end(),"event-present");return int(p-events.begin());}
static void suite(){
 for(auto test:std::vector<std::pair<std::vector<unsigned char>,unsigned int>>{{{0},0},{{127},127},{{129,0},128},{{255,127},16383},{{129,128,128,128,0},0x10000000u},{{143,255,255,255,127},0xffffffffu},{{255,255,255,255,255,127},0xffffffffu}}){auto *p=test.first.data();CHECK(MidiReadVariableLength(&p)==test.second&&p==test.first.data()+test.first.size(),"variable-length-wrap");}
 reset();std::vector<unsigned char> data={0xc3,42,0x81,0};input(data);process();CHECK(seq.channels[3].program==42&&messages==std::vector<DWORD>({0x002ac3})&&a[0].nextEventTick==128&&a[0].cursor==data.data()+4,"program-change");
 reset();data={64,0,1};input(data);a[0].runningStatus=0xc5;process();CHECK(seq.channels[5].program==64&&messages[0]==0x40c5&&a[0].cursor==data.data()+2,"running-status");
 reset();data={0x91,60,80,3};input(data);seq.transpose=2;process();CHECK(seq.channels[1].heldNotes[7]==0x40&&messages[0]==0x503e91&&a[0].nextEventTick==3,"note-on");
 data={0x91,60,0,2};input(data);process();CHECK(seq.channels[1].heldNotes[7]==0&&messages.back()==0x003e91,"zero-velocity-off");
 data={0x81,60,20,2};input(data);seq.channels[1].heldNotes[7]=0x40;process();CHECK(seq.channels[1].heldNotes[7]==0&&messages.back()==0x143e81,"note-off");
 for(int control:{0,10,91,93}){reset();data={0xbf,static_cast<unsigned char>(control),73,0};input(data);process();auto &ch=seq.channels[15];CHECK((control==0?ch.bank:control==10?ch.pan:control==91?ch.reverb:ch.chorus)==73,"channel-control");}
 for(float fade:{-1.0f,0.5f,2.0f}){reset();seq.fadeVolume=fade;data={0xb2,7,100,0};input(data);process();int value=fade<0?0:fade<1?50:127;CHECK(seq.channels[2].volume==100&&seq.channels[2].scaledVolume==value&&messages[0]==(0x7b2u|(unsigned(value)<<16)),"volume-clamp");}
 reset();data={0xff,0x51,3,0x07,0xa1,0x20,0x81,0};input(data);seq.elapsedTimerTicks=100;seq.eventTickBase=17;a[0].nextEventTick=11;process();CHECK(seq.tempoMicroseconds==503752&&seq.eventTickBase==113&&seq.elapsedTimerTicks==0&&a[0].nextEventTick==139&&messages.empty(),"tempo-target-recurrence");
 reset();data={0xff,0x2f,2,33,44,0};input(data);a[0].runningStatus=0xa1;a[0].nextEventTick=11;process();CHECK(a[0].active==0&&a[0].runningStatus==0xa1&&a[0].nextEventTick==11&&a[0].cursor==data.data()+3&&messages.empty(),"end-track-return");
 reset();data={0xff,1,2,55,66,4};input(data);process();CHECK(a[0].cursor==data.data()+6&&a[0].nextEventTick==4&&a[0].runningStatus==0xff&&messages.empty(),"unknown-meta");
 reset();data={0xf7,2,99};input(data);process();CHECK(a[0].cursor==data.data()+2&&a[0].nextEventTick==2&&messages.empty(),"f7-target-path");
 reset();std::vector<unsigned char> loop={0xb0,2,0,5,0xc0,11,0};data={8,0xc0,22,0};input(loop);input(data,a[1]);seq.trackCount=2;a[0].nextEventTick=10;a[1].nextEventTick=20;seq.elapsedTimerTicks=123;seq.eventTickBase=456;process();CHECK(a[0].loopCursor==loop.data()+3&&a[0].loopEventTick==10&&a[1].loopCursor==data.data()&&a[1].loopEventTick==20&&seq.loopTempoMicroseconds==500000&&seq.loopElapsedTimerTicks==123&&seq.loopEventTickBase==456&&a[0].nextEventTick==15,"loop-start");
 std::vector<unsigned char> end={0xb0,4,0,7};a[0].cursor=end.data();a[0].nextEventTick=99;a[1].cursor=data.data()+1;a[1].nextEventTick=77;seq.tempoMicroseconds=1;seq.elapsedTimerTicks=2;seq.eventTickBase=3;process();CHECK(a[0].cursor==loop.data()+4&&a[0].nextEventTick==15&&a[1].cursor==data.data()&&a[1].nextEventTick==20&&seq.tempoMicroseconds==500000&&seq.elapsedTimerTicks==123&&seq.eventTickBase==456,"loop-end");
 reset();data={0xc0,1,9};input(data);std::vector<unsigned char> changed={0x81,1};after=[&](const std::string&s){if(s=="short"){a[0].cursor=changed.data();a[0].runningStatus=0x99;}};process();CHECK(a[0].nextEventTick==129&&a[0].cursor==changed.data()+2&&a[0].runningStatus==0xc0,"live-delta-after-message");
 for(int mode:{0,1,2,3}){reset();data={0xf0,3,1,2,0xf7,5};input(data);seq.pendingLongHeaderIndex=31;prepareResult=mode==1?1:0;longResult=mode==2?1:0;if(mode==3)seq.outputHandle=NULL;after=[](const std::string&s){if(s=="prepare")seq.outputHandle=&handles[1];};process();CHECK(seq.pendingLongHeaderIndex==0&&a[0].nextEventTick==5&&a[0].runningStatus==0xf0&&a[0].cursor==data.data()+6,"sysex-ring-wrap");if(mode==1)CHECK(!has("long")&&seq.pendingLongHeaders[31]==NULL&&allocations.empty(),"sysex-prepare-failure");else if(mode==2)CHECK(has("long")&&seq.pendingLongHeaders[31]==NULL&&allocations.empty(),"sysex-long-failure");else{CHECK(seq.pendingLongHeaders[31]!=NULL&&allocations.size()==2,"sysex-retained");if(mode==0)CHECK(longBytes==std::vector<unsigned char>({0xf0,1,2,0xf7})&&longHandles[0]==&handles[1],"sysex-payload");else CHECK(!has("prepare")&&!has("long"),"sysex-no-device");}}
 reset();data={0xf0,1,0xf7,2};input(data);auto *old=static_cast<MIDIHDR*>(HostMalloc(sizeof(MIDIHDR)));std::memset(old,0,sizeof(*old));old->lpData=static_cast<char*>(HostMalloc(3));seq.pendingLongHeaders[0]=old;events.clear();process();CHECK(at("unprepare")<at("free-data")&&at("free-header")<at("allocate-header")&&allocations.size()==2,"sysex-replace-order");
 reset();data={0,0xc0,1,0};std::vector<unsigned char> second={0x81,1,0xc0,2,0};input(data);input(second,a[1]);seq.trackCount=2;seq.fadeActive=1;seq.fadeVolume=.2f;seq.resetState2DC=7;seq.eventTickBase=8;seq.elapsedTimerTicks=9;seq.ResetTracks();CHECK(seq.fadeVolume==1&&seq.fadeActive==0&&seq.resetState2DC==0&&seq.eventTickBase==0&&seq.elapsedTimerTicks==0&&a[0].active==1&&a[0].nextEventTick==0&&a[0].cursor==data.data()+1&&a[0].loopCursor==data.data()&&a[1].active==1&&a[1].nextEventTick==129&&a[1].cursor==second.data()+2,"reset-track-state");
 reset();data={0xc0,1,0,0xff,0x2f,0};input(data);a[0].nextEventTick=-1;seq.Tick();CHECK(messages.empty()&&a[0].active==1&&seq.elapsedTimerTicks==1,"negative-event-not-due");
 reset();input(data);a[0].nextEventTick=1;seq.elapsedTimerTicks=0x100000000ULL;seq.tempoMicroseconds=-500;seq.Tick();CHECK(messages.empty()&&seq.elapsedTimerTicks==0x100000001ULL,"signed-tempo-extension");
 reset();seq.ticksPerQuarterNote=-1;seq.tempoMicroseconds=1;seq.elapsedTimerTicks=1;CHECK(MidiElapsedTicks(&seq)==ULLONG_MAX-999,"signed-clock-multiply");
 reset();data={0xc0,1,0,0xff,0x2f,0};input(data);seq.Tick();CHECK(messages.size()==1&&a[0].active==0&&seq.elapsedTimerTicks==1,"tick-drains-due");
 reset();data={0,0xc0,1,0};input(data);a[0].active=0;seq.Tick();CHECK(a[0].active==1&&a[0].cursor==data.data()+1&&seq.elapsedTimerTicks==0,"tick-rewind-after-increment");
 reset();data={0xc0,1,0,0xff,0x2f,0};input(data);std::vector<unsigned char> replacement={0xc0,2,0};input(replacement,b[0]);b[0].active=0;seq.trackCount=1;after=[](const std::string&s){if(s=="short")seq.tracks=b;};seq.Tick();CHECK(seq.tracks==b&&a[0].active==1&&messages.size()==1&&seq.elapsedTimerTicks==1,"tick-live-track-table");
 reset();input(data);seq.fadeActive=1;seq.fadeDuration=4;seq.fadePosition=4;seq.fadeVolume=.5f;seq.Tick();CHECK(seq.fadeVolume==0&&seq.elapsedTimerTicks==0&&seq.fadePosition==4&&messages.empty(),"fade-terminal");
 reset();data={0xc0,1,0};input(data);a[0].nextEventTick=100;seq.fadeActive=1;seq.fadeDuration=4;seq.fadePosition=1;seq.fadeStep=-1;for(auto &ch:seq.channels)ch.volume=100;seq.Tick();CHECK(seq.fadeVolume==.75f&&seq.fadeStep==96&&seq.fadePosition==2&&messages.size()==16&&messages[0]==0x4b07b0&&seq.elapsedTimerTicks==1,"fade-step");
 reset();for(auto &ch:seq.channels)ch.volume=100;seq.fadeVolume=.5f;seq.ApplyChannelVolumes(10);CHECK(messages.size()==16&&messages[0]==0x3c07b0&&messages.back()==0x3c07bf&&seq.channels[0].scaledVolume==0,"volume-request-only");
 reset();seq.suppressVolumeUpdates=1;seq.ApplyChannelVolumes(0);CHECK(events.empty(),"volume-suppression");
 reset();for(auto &ch:seq.channels)ch.volume=127;seq.ApplyChannelVolumes(INT_MAX);CHECK(messages.size()==16&&(messages[0]>>16)==0,"volume-wrap-before-clamp");
 reset();seq.timerId=7;seq.timerPeriod=2;seq.activeFileIndex=3;seq.StopPlayback();CHECK(events==std::vector<std::string>({"kill-timer","end-period","device-reset","device-close"})&&seq.timerId==0&&seq.outputHandle==NULL&&seq.activeFileIndex==-1,"stop-contract");
 reset();seq.tracks=NULL;CHECK(seq.StopPlayback()==-1&&events.empty(),"stop-without-tracks");
 cleanup();
}
int main(){try{suite();}catch(const Failure&f){cleanup();std::fprintf(stderr,"CHECK %s\n",f.label);return 1;}return 0;}
