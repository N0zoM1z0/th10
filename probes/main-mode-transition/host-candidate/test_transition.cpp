#ifdef NDEBUG
#error Assertions are required for this host fixture
#endif
#include "transition.cpp"
#include <cassert>
#include <vector>
#include <limits>
using namespace transition_candidate;
struct Mock : Hooks {
    unsigned mode, request, factoryResult;
    std::vector<unsigned> calls;
    bool first = true;
    std::uint32_t scalarAfterCleanup = 0;
    std::uint32_t initializationModeSnapshot = 0;
    Mock(unsigned m, unsigned r, unsigned f): mode(m),request(r),factoryResult(f) {}
    void enter(Context& s, Globals& g) override {
        initializationModeSnapshot=(request*13u)^0xa5a55a5au;
        g.gameManagerInitializationMode=initializationModeSnapshot;
        calls.push_back(1); s.field390=mode; s.field38c=request;
        s.field6f9=250; s.field394=0; s.field780=0;
    }
    void observe(unsigned event, Context& s, Globals& g) {
        if(first) { assert(s.field394==request && s.field780==0xff000000u && s.field6f9==251); first=false; }
        if(event==0x41fd00) assert(s.field390==1);
        if(event==0x4203f0 && mode==0) assert(s.field390==3 && s.field768==0);
        if(event==0x418150 && mode==11) assert(s.field39c==0);
        if(event==0x4180e0) {
            assert(s.field39c==(mode==11?(request==7?91u:0u):1u));
            if(mode!=7) assert(s.field390==7);
            if(mode==10) { assert(g.data474c7c==scalarAfterCleanup); assert(g.data477848==0x474788u+0x30u*scalarAfterCleanup); }
        }
        if(event==0x42cd50 && mode==15) assert(s.field390==4 && g.data491c00==3);
        calls.push_back(event);
    }
    void mutate(unsigned event, Context& s, Globals& g) {
        s.field390=event+500u; s.field38c=event+700u; s.field39c=91;
        s.field768=88; s.field6f9=77;
        g.gameManagerInitializationMode=event*31u+0x12345678u;
        g.data474c80=event*17u+0xfffffff0u; g.data474c7c=123; g.data477848=456; g.data491c00=99;
        if(event==0x418150) scalarAfterCleanup=g.data474c80;
    }
    void leave(Context& s, Globals& g) override {
        observe(2,s,g); assert(s.field38c==s.field390);
        if(mode==0) assert(s.field768==factoryResult);
        mutate(2,s,g); s.field6f9=0;
    }
    std::uint32_t call41fd00(Context& s,Globals& g) override {observe(0x41fd00,s,g);mutate(0x41fd00,s,g);return factoryResult;}
#define HOOK(name,id) void name(Context& s,Globals& g) override {observe(id,s,g);mutate(id,s,g);}
    HOOK(call4203f0,0x4203f0) HOOK(call421c50,0x421c50)
    HOOK(call42cd50,0x42cd50) HOOK(call418150,0x418150)
    HOOK(call40b9d0,0x40b9d0) HOOK(call42cdb0,0x42cdb0)
    void call4180e0(Context& s,Globals& g,std::uint32_t initializationMode) override {
        const std::uint32_t expectedArgument=mode==11?initializationModeSnapshot:(mode==12?1u:0u);
        assert(initializationMode==expectedArgument);
        observe(0x4180e0,s,g);mutate(0x4180e0,s,g);
    }
    HOOK(call40b940,0x40b940)
#undef HOOK
};
// Independent fixed call-obligation oracle. This is authored host behavior,
// not an exported native execution trace or a complete target-effect oracle.
std::vector<unsigned> expected(unsigned mode,unsigned request,unsigned factory) {
    std::vector<unsigned> v{1};
    if(mode==0) v.push_back(0x41fd00);
    if(mode==3 || (mode==0 && factory==0)) {v.push_back(0x4203f0);v.push_back(0x421c50);return v;}
    if((mode==4 && request==7)||(mode==15 && request==7)||mode==10||mode==13||((mode==11||mode==14)&&request==7))v.push_back(0x418150);
    if((mode==4||mode==15)&&request==14)v.push_back(0x40b9d0);
    if((mode==4&&(request==1||request==2||request==7||request==14))||(mode==15&&(request==2||request==7||request==14)))v.push_back(0x42cd50);
    if((mode==7||mode==12)&&request==4)v.push_back(0x42cdb0);
    if(mode==7||(mode>=10&&mode<=13))v.push_back(0x4180e0);
    if(mode==14)v.push_back(0x40b940);
    v.push_back(2);return v;
}
int main() {
    unsigned count=0;
    std::vector<unsigned> values;
    for(unsigned n=0;n<=16;++n)values.push_back(n);
    values.push_back(0xffffffffu);
    for(auto mode:values)for(auto request:values)for(auto factory:{0u,9u}) {
        Context s{};Globals g{};s.field38c=100;s.field390=101;
        Mock h(mode,request,factory);int r=transition(s,g,h);
        assert(h.calls==expected(mode,request,factory));
        bool failed=mode==3||(mode==0&&factory==0);assert(r==(failed?4:1));
        if(!failed)assert(s.field6f9==255); // Leave changed it to zero before decrement.
        ++count;
    }
    for(auto value:values) {Context s{};Globals g{};s.field38c=s.field390=value;Mock h(0,0,0);assert(transition(s,g,h)==1&&h.calls.empty());++count;}
    return count==666?0:1;
}
