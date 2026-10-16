#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdarg>
#include <string>
#include <vector>
#include <algorithm>
#define __stdcall
#include "Hint.hpp"
#include "FileSystem.hpp"
// Explicit failure survives NDEBUG and produces one recognized diagnostic.
#define CHECK(condition) do { if (!(condition)) { \
    std::fprintf(stderr, "HINT_CHECK_FAILED line %d: %s\n", __LINE__, #condition); \
    std::exit(1); \
} } while (0)
using std::string;
using std::vector;
static vector<string> events, writes;
static vector<int> closes;
static int gReplayFileHandle, gReplayFileCriticalSection;
static unsigned char gReplayFileOpenCount;
static const int INVALID_HANDLE_VALUE = -1;
typedef unsigned int DWORD;
static bool failOpen, shortWrite, apiResult, invalidateAfterFirst, nullAllocation;
static int swapHandle, nullFormats;
static HintLinkView *cutLink;
static tm stamp;
static void reset() {
 events.clear();writes.clear();closes.clear();gReplayFileHandle=-1;
 gReplayFileOpenCount=0;failOpen=false;shortWrite=false;apiResult=true;
 invalidateAfterFirst=false;nullAllocation=false;swapHandle=0;nullFormats=0;cutLink=0;
 memset(&stamp,0,sizeof stamp);stamp.tm_year=126;stamp.tm_mon=9;
 stamp.tm_mday=16;stamp.tm_hour=3;stamp.tm_min=4;
}
static void *MockMalloc(size_t n) {CHECK(n==4096);events.push_back("alloc");return nullAllocation?0:std::malloc(n);}
static void MockFree(void *p) {events.push_back("free");std::free(p);}
static int MockMkdir(const char *p) {CHECK(string(p)=="hint");events.push_back("mkdir");return -1;}
static time_t MockTime(time_t *p) {events.push_back("time");*p=123;return 123;}
static tm *MockLocaltime(const time_t *p) {CHECK(*p==123);events.push_back("localtime");return &stamp;}
static int MockSprintf(char *out,const char *fmt,...) {
 va_list args;va_start(args,fmt);
 if (strcmp(fmt,"\tBase\t: %s\r\n")==0 || strcmp(fmt,"\tAlign\t: %s\r\n")==0) {
  const char *p=va_arg(args,const char*);
  if (!p) {++nullFormats;strcpy(out,"<null-format-observed>\r\n");va_end(args);return strlen(out);}
  int n=std::sprintf(out,fmt,p);va_end(args);return n;
 }
 int n=std::vsprintf(out,fmt,args);va_end(args);return n;
}
static bool WriteFile(int handle,const void *p,unsigned n,DWORD *written,void *) {
 CHECK(handle!=-1);events.push_back("write");writes.push_back(string(static_cast<const char*>(p),n));
 if (cutLink && writes.back()=="Tips\r\n") {cutLink->next=0;cutLink=0;}
 if (swapHandle) gReplayFileHandle=swapHandle;
 if (invalidateAfterFirst) gReplayFileHandle=-1;
 *written=shortWrite?n-1:n;return apiResult;
}
static int CloseHandle(int h) {events.push_back("close");closes.push_back(h);return 0;}
static void LeaveCriticalSection(int *p) {CHECK(p==&gReplayFileCriticalSection);events.push_back("leave");}
namespace FileSystem {
 int OpenWriteFile(const char *path) {
  CHECK(string(path)=="synthetic-output");events.push_back("open");
  gReplayFileHandle=failOpen?-1:7;gReplayFileOpenCount=failOpen?0:1;
  return failOpen?-1:0;
 }
}
#include "FileSystemWrite.inc"
#define sprintf MockSprintf
#define malloc MockMalloc
#define free MockFree
#define _mkdir MockMkdir
#define time(x) MockTime(x)
#define localtime(x) MockLocaltime(x)
#include "Hint.cpp"
#undef sprintf
#undef malloc
#undef free
#undef _mkdir
#undef time
#undef localtime
static string stream() {string s;for(size_t i=0;i<writes.size();++i)s+=writes[i];return s;}
static size_t occurrences(const string &s,const string &needle) {
 size_t n=0,p=0;while((p=s.find(needle,p))!=string::npos){++n;p+=needle.size();}return n;
}
static HintEntryView entry(int remain=3) {
 HintEntryView e={};e.remaining=remain;e.scale=1.5f;e.time=300;e.count=-5;
 e.position[0]=-3.75f;e.position[1]=9.9f;
 e.blue=255;e.green=0;e.red=128;e.alpha=254;strcpy(e.text,"quoted \"text\"");return e;
}
static void attach(HintOwnerView &o,vector<HintEntryView> &es,int stage) {
 o.exportStageLists[stage].next=es.empty()?0:&es[0].link;
 for(size_t i=0;i<es.size();++i){es[i].link.entry=&es[i];es[i].link.next=i+1<es.size()?&es[i+1].link:0;}
}
static void run(HintOwnerView &o) {CHECK(ExportAutomaticHints(&o,"synthetic-output")==0);CHECK(events[0]=="alloc"&&events[1]=="mkdir"&&events[2]=="open");CHECK(events.back()=="free");}
static const char expectedHeader[] = "\x23\x20\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x20\x0d\x0a\x23\x20\x93\x8c\x95\xfb\x95\x97\x90\x5f\x98\x5e\x81\x40\x8d\x55\x97\xaa\x83\x71\x83\x93\x83\x67\x83\x74\x83\x40\x83\x43\x83\x8b\x20\x0d\x0a\x23\x0d\x0a\x23\x20\x82\xb1\x82\xcc\x83\x74\x83\x40\x83\x43\x83\x8b\x82\xcd\x81\x41\x8e\xa9\x93\xae\x93\x49\x82\xc9\x90\xb6\x90\xac\x81\x41\x8f\xe3\x8f\x91\x82\xab\x82\xb3\x82\xea\x82\xdc\x82\xb7\x20\x0d\x0a\x23\x20\x8e\xa9\x95\xaa\x82\xc5\x8d\x55\x97\xaa\x83\x71\x83\x93\x83\x67\x82\xf0\x8d\x5c\x90\xac\x82\xb5\x82\xbd\x82\xa2\x8f\xea\x8d\x87\x82\xcd\x81\x41\x95\xca\x82\xcc\x83\x74\x83\x40\x83\x43\x83\x8b\x82\xf0\x8e\x67\x97\x70\x82\xb5\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2\x20\x0d\x0a\x23\x0d\x0a\x23\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x54\x69\x6d\x65\x2d\x73\x74\x61\x6d\x70\x3a\x20\x3c\x32\x30\x32\x36\x2f\x31\x30\x2f\x31\x36\x20\x30\x33\x3a\x30\x34\x3e\x0d\x0a\x0d\x0a\x0d\x0a\x56\x65\x72\x73\x69\x6f\x6e\x20\x3d\x20\x30\x2e\x30\x0d\x0a\x0d\x0a";

int main() {
 HintOwnerView o={};reset();run(o);CHECK(writes.size()==9);CHECK(stream()==expectedHeader);
 CHECK(closes==vector<int>(1,7));CHECK(gReplayFileOpenCount==0);
 reset();failOpen=true;run(o);CHECK(writes.empty()&&closes.empty());CHECK(events.size()==4);
 reset();failOpen=true;nullAllocation=true;run(o);CHECK(writes.empty()&&closes.empty()&&events.size()==4);
 vector<HintEntryView> es(1,entry());attach(o,es,0);reset();run(o);
 string expected=expectedHeader;
 expected+="# ================================== \r\nStage : 0\r\n\r\nTips\r\n\tRemain\t: 3\r\n\tText\t: \"quoted \"text\"\"\r\n\tPos\t\t: -3, 9\r\n\tCount\t: -5\r\n\tBase\t: start\r\n\tAlign\t: center\r\n\tTime\t: 300\r\n\tAlpha\t: 254\r\n\tColor\t: 128, 0, 255\r\n\tScale\t: 1.5\r\nEnd\r\n\r\nStageEnd\r\n";
 CHECK(stream()==expected);CHECK(writes.size()==24);
 es[0].remaining=0;reset();run(o);CHECK(stream()==expectedHeader);
 es[0].remaining=-1;reset();run(o);CHECK(stream().find("Tips\r\nTips\r\n")!=string::npos);CHECK(stream().find("Remain")==string::npos);
 for(int stage=0;stage<8;++stage) {
  o=HintOwnerView();attach(o,es,stage);reset();run(o);
  char label[40];std::sprintf(label,"Stage : %d\r\n",stage);CHECK(stream().find(label)!=string::npos);
  CHECK(occurrences(stream(),"Stage : ")==1);
 }
 // Explicit expected values independent of the candidate tables.
 vector<string> names;names.push_back("start");
 const char *prefixes[]={"converse_","mboss_","mboss_card_","boss_","boss_card_"};
 int limits[]={5,9,9,19,19};
 for(int k=0;k<5;++k)for(int n=1;n<=limits[k];++n){if(k==4&&n==11)continue;char number[20];std::sprintf(number,"%d",n);names.push_back(string(prefixes[k])+number);}
 CHECK(names.size()==61);
 const char *align[]={"center","left","right"};
 for(int base=0;base<61;++base)for(int a=0;a<3;++a){
  es[0].base=base;es[0].alignment=a;reset();run(o);
  CHECK(stream().find("\tBase\t: "+names[base]+"\r\n")!=string::npos);
  CHECK(stream().find(string("\tAlign\t: ")+align[a]+"\r\n")!=string::npos);
 }
 es[0].base=-1;es[0].alignment=3;reset();run(o);CHECK(nullFormats==2);
 // Bound emitted entries, not examined links: alternate zero/surviving entries.
 for(int count=254;count<=256;++count) {
  o=HintOwnerView();vector<HintEntryView> many(2*count,entry(0));
  for(int i=0;i<count;++i)many[2*i+1].remaining=1;
  attach(o,many,3);reset();run(o);
  CHECK(occurrences(stream(),"\tRemain\t: ")==static_cast<size_t>(std::min(count,255)));
  CHECK(occurrences(stream(),"StageEnd")==1);
 }
 // Snapshot next before first formatting/write callback modifies the current link.
 o=HintOwnerView();es=vector<HintEntryView>(2,entry());attach(o,es,2);
 reset();cutLink=&es[0].link;run(o);CHECK(occurrences(stream(),"\tRemain\t: ")==2);
 // Invalidated global skips every subsequent API write and final close.
 attach(o,es,2);reset();invalidateAfterFirst=true;run(o);CHECK(writes.size()==1&&closes.empty());
 // BOOL failure with complete byte count remains success.
 reset();apiResult=false;run(o);CHECK(closes.size()==1&&gReplayFileOpenCount==0);
 // Short writes continue. Each closes the reloaded current handle, with one final close.
 reset();shortWrite=true;swapHandle=77;run(o);
 CHECK(writes.size()==36);CHECK(closes.size()==writes.size()+1);
 CHECK(std::count(closes.begin(),closes.end(),77)==static_cast<int>(closes.size()));
 CHECK(std::count(events.begin(),events.end(),"leave")==static_cast<int>(closes.size()));
 CHECK(gReplayFileOpenCount==static_cast<unsigned char>(1-closes.size()));
 CHECK(gReplayFileHandle==77);
 reset();FileSystem::WriteToOpenFile("x",1);CHECK(writes.empty());
 gReplayFileHandle=7;gReplayFileOpenCount=1;shortWrite=true;FileSystem::WriteToOpenFile("x",1);
 CHECK(writes.size()==1&&closes.size()==1&&gReplayFileOpenCount==0);
 shortWrite=false;FileSystem::WriteToOpenFile("x",1);
 CHECK(writes.size()==2&&closes.size()==1);
 puts("PASS: exact stream, 183 table pairs, stage/limit/lifetime/error-path fixtures");
}
