#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdarg>
#include <cmath>
#include <limits>
#include <new>
#include <string>
#include <vector>
#include <algorithm>
#define __stdcall
#define __fastcall
#define __declspec(x)
#include "Hint.hpp"
// Canonical global storage supplied by the host harness, as at the game link boundary.
HintOwnerView *g_GameOwner477814;
#include "FileSystem.hpp"
#define CHECK(condition) do { if (!(condition)) { \
    std::fprintf(stderr,"HINT_LOAD_CHECK_FAILED line %d: %s\n",__LINE__,#condition); \
    std::exit(1); \
} } while(0)
using std::string;
using std::vector;
static vector<void *> scratch, freed;
static vector<HintEntryView *> allocated, deleted;
static vector<string> events;
static string payload;
static bool missingFile, exporting;
static string exportText;
static void *inputAllocation;
static unsigned scratchAllocations;
static void reset()
{
    for (size_t i = 0; i<scratch.size(); ++i) std::free(scratch[i]);
    for (size_t i = 0; i<allocated.size(); ++i) {
        allocated[i]->~HintEntryView();
        std::free(allocated[i]);
    }
    scratch.clear();
    freed.clear();
    allocated.clear();
    deleted.clear();
    events.clear();
    payload.clear();
    missingFile = false;
    exporting = false;
    exportText.clear();
    inputAllocation = 0;
    scratchAllocations = 0;
}
static void *MockMalloc(size_t size)
{
    CHECK(size == 4096);
    events.push_back("malloc");
    ++scratchAllocations;
    void *p = std::malloc(size);
    CHECK(p != 0);
    // A defined fixture allocator fill, not a claim about native malloc bytes.
    memset(p, 0x5a, size);
    scratch.push_back(p);
    return p;
}
static void MockFree(void *p)
{
    vector<void*>::iterator it = std::find(scratch.begin(), scratch.end(), p);
    CHECK(it != scratch.end());
    events.push_back(p == inputAllocation?"free-input":"free-scratch");
    scratch.erase(it);
    freed.push_back(p);
    std::free(p);
}
static HintEntryView *MockNewEntry();
static void MockDelete(HintEntryView *entry)
{
    CHECK(std::find(allocated.begin(), allocated.end(), entry) != allocated.end());
    CHECK(std::find(deleted.begin(), deleted.end(), entry) == deleted.end());
    deleted.push_back(entry);
    events.push_back("delete-entry");
    // Deferred host release permits checking target-retained dangling heads.
    // reset() calls the actual trivial destructor and matching std::free.
}
static double MockAtof(const char *text)
{
    if (string(text) == "<NAN>") return std::numeric_limits<double>::quiet_NaN();
    return std::atof(text);
}
static int MockMkdir(const char *path) {
    CHECK(exporting && string(path) == "hint");
    return -1;
}
namespace FileSystem {
    unsigned char *__stdcall OpenFile(const char *path, int *sizeOut, int mode)
    {
        CHECK(string(path) == "synthetic-input");
        CHECK(mode == 1);
        events.push_back("open");
        if (missingFile) return 0;
        *sizeOut = static_cast<int>(payload.size());
        // Deliberately padded only in the fixture: original OpenFile allocates exact size.
        char *data = static_cast<char*>(std::malloc(payload.size()+32));
        CHECK(data != 0);
        memcpy(data, payload.data(), payload.size());
        memset(data+payload.size(), 0, 32);
        inputAllocation = data;
        scratch.push_back(data);
        return reinterpret_cast<unsigned char*>(data);
    }
    int __stdcall OpenWriteFile(const char *) {
        CHECK(exporting);
        return 0;
    }
    void WriteToOpenFile(const void *data, unsigned int size) {
        CHECK(exporting);
        exportText.append(static_cast<const char*>(data), size);
    }
    int CloseWriteFile() {
        CHECK(exporting);
        return 0;
    }
}
#define malloc MockMalloc
#define free MockFree
#define atof MockAtof
#define _mkdir MockMkdir
#include "Hint.cpp"
#undef malloc
#undef free
#undef atof
#undef _mkdir
static HintEntryView *MockNewEntry()
{
    void *storage = std::malloc(sizeof(HintEntryView));
    CHECK(storage != 0);
    HintEntryView *p = new(storage) HintEntryView;
    allocated.push_back(p);
    events.push_back("new-entry");
    return p;
}
static void ResetOwner(HintOwnerView &owner)
{
    owner.~HintOwnerView();
    new (&owner) HintOwnerView;
    CHECK(g_GameOwner477814 == &owner);
    for (int stage = 0; stage < 8; ++stage) {
        CHECK(owner.loadedStageLists[stage].payload == &owner);
        CHECK(owner.exportStageLists[stage].payload == &owner);
    }
}
static void load(HintOwnerView &owner, const string &text, int mode = 1)
{
    unsigned beforeAllocations = scratchAllocations;
    size_t beforeFrees = freed.size();
    payload = text;
    CHECK(LoadHintFile(&owner, "synthetic-input", mode) == 0);
    CHECK(events.front() == "open");
    CHECK(scratchAllocations == beforeAllocations+3);
    CHECK(freed.size() == beforeFrees+4);
    CHECK(freed.back() == inputAllocation);
    CHECK(scratch.empty());
}
static vector<HintEntryView *> entries(HintLinkView &head)
{
    vector<HintEntryView*> result;
    HintLinkView *prev = &head;
    for (HintLinkView *node = head.next; node; node = node->next) {
        CHECK(result.size()<1024);
        CHECK(node->previous == prev);
        HintEntryView *entry = static_cast<HintEntryView *>(node->payload);
        CHECK(entry != 0);
        CHECK(&entry->link == node);
        result.push_back(entry);
        prev = node;
    }
    return result;
}
static HintEntryView *one(HintOwnerView &owner, int stage = 1)
{
    vector<HintEntryView*> es = entries(owner.loadedStageLists[stage]);
    CHECK(es.size() == 1);
    return es[0];
}
static string item(const string &fields = "", const string &ending = "End\r\n")
{return "Tips\r\n"+fields+ending;
}
int main()
{
    // Constructor and source helper contracts, exercised directly.
    HintEntryView initial;
    CHECK(initial.link.payload == &initial && initial.link.next == 0 && initial.link.previous == 0);
    CHECK(initial.remaining == -1 && initial.time == 300 && initial.scale == 1.0f);
    CHECK(initial.blue == 255 && initial.green == 255 && initial.red == 255 && initial.alpha == 255);
    CHECK(initial.text[64] == 0 && initial.text[65] == 0 && initial.text[67] == 0);
    char trim[] = " \t\r\n hello \t\r\n";
    CHECK(HintTrimWhitespace(trim) == trim);
    CHECK(string(trim) == "hello");
    char notTrimmed[] = "\v\f x \v\f";
    HintTrimWhitespace(notTrimmed);
    CHECK(string(notTrimmed) == "\v\f x \v\f");
    char blank[] = " \t\n\r";
    HintTrimWhitespace(blank);
    CHECK(*blank == 0);
    char key[128], value[128];
    strcpy(value, "stale");
    HintSplitLine("  Tips  ", key, value);
    CHECK(string(key) == "Tips" && string(value) == "stale");
    HintSplitLine(" Key : a:b  ", key, value);
    CHECK(string(key) == "Key" && string(value) == "a:b");
    char mixed[] = " A\rB\nC";
    char line[128];
    int remaining = strlen(mixed);
    char *next = HintReadLine(mixed, line, &remaining, sizeof(line));
    CHECK(string(line) == "A\rB");
    CHECK(next == mixed+5 && remaining == 1);
    char trailing[] = " tail # untouched ";
    remaining = strlen(trailing);
    next = HintReadLine(trailing, line, &remaining, sizeof(line));
    CHECK(next == trailing && remaining == 0 && string(line) == " tail # untouched ");
    char skip[] = " #comment\r\n \t\n value #suffix\r\n";
    remaining = strlen(skip);
    next = HintReadLine(skip, line, &remaining, sizeof(line));
    CHECK(string(line) == "value");
    CHECK(remaining == 0);
    char longline[] = "abcdefghijklmnop\r\nZ";
    char small[8];
    remaining = strlen(longline);
    next = HintReadLine(longline, small, &remaining, sizeof(small));
    CHECK(string(small) == "abcdefg");
    CHECK(*next == 'Z' && remaining == 1);
    // Uncapped final-tail contract on oversized backing storage, safely observable.
    char finalLong[] = "abcdefghij";
    char backing[32] = {};
    remaining = 10;
    next = HintReadLine(finalLong, backing, &remaining, 8);
    CHECK(next == finalLong && remaining == 0 && memcmp(backing, "abcdefghij", 10) == 0);

    HintOwnerView owner = {};
    reset();
    missingFile = true;
    CHECK(LoadHintFile(&owner, "synthetic-input", 0) == -1);
    CHECK(events.size() == 1 && scratchAllocations == 0 && allocated.empty());
    reset();
    load(owner, "");
    CHECK(allocated.empty());
    reset();
    ResetOwner(owner);
    load(owner, "Version : 0.0\r\nStage : 1\r\n"+item(
    "Pos : -12, 34\r\nText : \"ab\"cd\"\r\nCount : -27\r\nTime : 41\r\nBase : boss_card_12\r\nAlign : right\r\nRemain : 5\r\nScale : 2\r\nColor : -1, 256, 258\r\nAlpha : 511\r\n"), 0);
    HintEntryView *e = one(owner);
    CHECK(e->position.x == -12 && e->position.y == 34);
    CHECK(string(e->text) == "ab\"cd" && e->count == -27 && e->time == 41 && e->base == 53 && e->alignment == 2);
    CHECK(e->remaining == 5 && e->scale == 2 && e->red == 255 && e->green == 0 && e->blue == 2 && e->alpha == 255);
    CHECK(e->textWidth == 75);
    CHECK(allocated.size() == 2);
    vector<HintEntryView*> copies = entries(owner.exportStageLists[1]);
    CHECK(copies.size() == 1 && copies[0] != e);
    CHECK(copies[0]->count == e->count && copies[0]->textWidth == e->textWidth && string(copies[0]->text) == string(e->text));
    e->remaining = 0;
    CHECK(copies[0]->remaining == 5);
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\n"+item(), 1);
    CHECK(entries(owner.exportStageLists[1]).empty());
    e = one(owner);
    CHECK(e->remaining == -1 && e->time == 300 && e->scale == 1 && e->textWidth == 0);

    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\n"+item("Count : 1\n"), 0);
    load(owner, "Stage : 1\n"+item("Count : 2\n"), 1);
    CHECK(entries(owner.loadedStageLists[1]).size() == 2 && entries(owner.exportStageLists[1]).size() == 1);
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\n"+item(), 2);
    CHECK(allocated.size() == 1 && entries(owner.exportStageLists[1]).empty());
    // All stages including rejected boundaries.
    for (int stage = -1; stage <= 8; ++stage) {
        reset();
        ResetOwner(owner);
        char number[20];
        std::sprintf(number, "%d", stage);
        load(owner, string("Stage : ")+number+"\n"+item());
        CHECK(allocated.size() == static_cast<size_t>(stage >= 1 && stage <= 7?1:0));
        if (stage >= 1 && stage <= 7) CHECK(one(owner, stage)->count == 0);
    }
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\n"+item()+"StageEnd\n"+item());
    CHECK(allocated.size() == 1);
    // New Stage resets the 255-entry counter; missing stage values reuse prior value.
    for (int count = 254; count <= 256; ++count) {
        reset();
        ResetOwner(owner);
        string text = "Stage : 1\n";
        for (int i = 0; i<count; ++i) text += item();
        text += "Stage : 2\n"+item();
        load(owner, text);
        CHECK(entries(owner.loadedStageLists[1]).size() == static_cast<size_t>(std::min(count, 255)));
        CHECK(entries(owner.loadedStageLists[2]).size() == 1);
    }
    reset();
    ResetOwner(owner);
    string sameStage = "Stage : 1\n";
    for (int i = 0; i < 255; ++i) sameStage += item();
    sameStage += "Stage : 1\n"+item();
    load(owner, sameStage);
    CHECK(entries(owner.loadedStageLists[1]).size() == 256);
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 2\nStageEnd\nStage\n"+item());
    CHECK(one(owner, 2) != 0);

    // The equals-sign line emitted by exporter is not a Version directive.
    reset();
    ResetOwner(owner);
    load(owner, "Version = WRONG\nStage : 1\n"+item());
    CHECK(one(owner) != 0 && deleted.empty());
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\n"+item()+"Stage : 2\n"+item()+"Version : wrong\nStage : 3\n"+item(), 0);
    CHECK(allocated.size() == 4 && deleted.size() == 4);
    CHECK(deleted[0] == allocated[0] && deleted[1] == allocated[1] && deleted[2] == allocated[2] && deleted[3] == allocated[3]);
    CHECK(owner.loadedStageLists[1].next == &allocated[0]->link && owner.exportStageLists[1].next == &allocated[1]->link);
    CHECK(events.back() == "free-input");

    // Quoted spans cover the 65th copy byte and constructor-zeroed trailing storage.
    for (int count = 63; count <= 65; ++count) {
        reset();
        ResetOwner(owner);
        string text(count, 'q');
        load(owner, "Stage : 1\n"+item("Text : \""+text+"\"\n"));
        e = one(owner);
        CHECK(strlen(e->text) == static_cast<size_t>(count));
        CHECK(e->text[count] == 0);
        CHECK(e->textWidth == count*7.5f);
        if (count == 65) {
            owner.exportStageLists[1].next = &e->link;
            exporting = true;
            CHECK(ExportAutomaticHints(&owner, "synthetic-output") == 0);
            CHECK(exportText.find("\tText\t: \""+text+"\"\r\n") != string::npos);
            exporting = false;
        }

    }
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\nTips\nText : \"unfinished\nCount : 99\nEnd\n");
    e = one(owner);
    CHECK(e->count == 0 && e->text[0] == 0);
    // A partial entry is appended.
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\nTips\nCount : 8");
    CHECK(one(owner)->count == 8);
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\n"+item("Color : 17,22\n"));
    e = one(owner);
    CHECK(e->red == 17 && e->green == 255 && e->blue == 255);
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\n"+item("Color : 17\n"));
    CHECK(one(owner)->red == 255);
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\n"+item("Pos : 9\nText : no quotes\nUnknown : x\n"));
    e = one(owner);
    CHECK(e->position.x == 0 && e->text[0] == 0);

    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\n"+item("Pos : 12,34\nCount\nColor : 7,8,9\nTime\nStage : 2\n"));
    e = one(owner);
    CHECK(e->count == 12 && e->time == 7 && entries(owner.loadedStageLists[2]).empty());
    vector<string> names;
    names.push_back("start");
    const char *prefix[] = {"converse_", "mboss_", "mboss_card_", "boss_", "boss_card_"};
    int maximum[] = {5, 9, 9, 19, 19};
    for (int k = 0; k < 5; ++k)
    for (int n = 1; n <= maximum[k]; ++n) {
        if (k == 4 && n == 11) continue;
        char b[20];
        std::sprintf(b, "%d", n);
        names.push_back(string(prefix[k])+b);
    }
    const char *align[] = {"center", "left", "right"};
    CHECK(names.size() == 61);
    for (int base = 0; base < 61; ++base)
    for (int a = 0; a < 3; ++a) {
        reset();
        ResetOwner(owner);
        load(owner, "Stage : 1\n"+item("Base : "+names[base]+"\nAlign : "+align[a]+"\n"));
        e = one(owner);
        CHECK(e->base == base && e->alignment == a);
    }
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\n"+item("Base : unknown\nAlign : unknown\n"));
    e = one(owner);
    CHECK(e->base == 0 && e->alignment == 0);
    const char *scaleText[] = {"-5", "-4", "-3.999", "0", "3.999", "4", "5", "<NAN>"};
    const float expectedScale[] = {-4, -4, -3.999f, 0, 3.999f, 4, 4, 0};
    for (int i = 0; i < 8; ++i) {
        reset();
        ResetOwner(owner);
        load(owner, "Stage : 1\n"+item(string("Scale : ")+scaleText[i]+"\nText : \"xx\"\n"));
        e = one(owner);
        if (i == 7) CHECK(std::isnan(e->scale) && std::isnan(e->textWidth));
        else {
            // Compare two stored float values under x87 excess precision.
            volatile float expectedWidth = expectedScale[i]*15;
            CHECK(e->scale == expectedScale[i] && e->textWidth == expectedWidth);
        }
    }
    reset();
    ResetOwner(owner);
    load(owner, "Stage : 1\n"+item("Text : \"abc\"\nScale : 1.829711675643921\n"));
    CHECK(one(owner)->textWidth == 41.168514251708984f);
    // Actual append/cleanup walks across every stage and both groups.
    reset();
    ResetOwner(owner);
    vector<HintEntryView*> expectedDeletes;
    for (int stage = 0; stage < 8; ++stage) {
        for (int group = 0; group < 2; ++group)
        for (int n = 0; n < 2; ++n) {
            HintEntryView *p = MockNewEntry();
            expectedDeletes.push_back(p);
            HintAppendLink(group?&owner.exportStageLists[stage]:&owner.loadedStageLists[stage], &p->link);
        }
    }
    // Append does not reset an incoming next link in the ordinary tail path.
    HintLinkView independentHead = {};
    HintEntryView a, b;
    a.link.next = &b.link;
    HintAppendLink(&independentHead, &a.link);
    CHECK(a.link.next == &b.link);
    ClearHintLists(&owner);
    CHECK(deleted == expectedDeletes);
    for (int stage = 0; stage < 8; ++stage) CHECK(owner.loadedStageLists[stage].next != 0 && owner.exportStageLists[stage].next != 0);
    reset();
    puts("PASS: actual loader/parser/lifetime source, 183 token pairs, boundaries and failure contracts");
}
