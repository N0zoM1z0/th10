#include "di_axes.hpp"
#include <cstdio>
#include <cstdlib>
static unsigned long cases=0;
static void check(unsigned short incoming,int x,int y,short tx,short ty) {
    // Oracle comparisons use reversed inequalities and original short promotions;
    // no candidate helper is used to derive expected contributions.
    unsigned int contribution=0;
    if (-static_cast<int>(tx)>x) contribution+=64;
    if (static_cast<int>(tx)<x) contribution+=128;
    if (-static_cast<int>(ty)>y) contribution+=16;
    if (static_cast<int>(ty)<y) contribution+=32;
    const unsigned int expected=static_cast<unsigned int>(incoming)|contribution;
    const unsigned short actual=input_candidate::DirectInputAxes(incoming,x,y,tx,ty);
    if(actual!=expected) {
        std::printf("SEMANTIC_MISMATCH input=%u x=%d y=%d tx=%d ty=%d expected=%u actual=%u\n",static_cast<unsigned int>(incoming),x,y,tx,ty,expected,static_cast<unsigned int>(actual));
        std::exit(10);
    }
    ++cases;
}
int main() {
    for(int t=-32768;t<=32767;++t) {
        const int points[9]={INT_MIN,INT_MAX,-t-1,-t,-t+1,t-1,t,t+1,0};
        const short tx=static_cast<short>(t),ty=static_cast<short>(-1-t);
        const int other=-1-t;
        const int ypoints[9]={INT_MIN,INT_MAX,-other-1,-other,-other+1,other-1,other,other+1,0};
        for(unsigned int i=0;i<9;++i) for(unsigned int j=0;j<9;++j) {
            check(0,points[i],ypoints[j],tx,ty);
            check(0xffafu,points[i],ypoints[j],tx,ty);
        }
    }
    for(unsigned int mask=0;mask<65536u;++mask) {
        check(static_cast<unsigned short>(mask),0,0,-1,-32768);
        check(static_cast<unsigned short>(mask),INT_MIN,INT_MAX,32767,32767);
        check(static_cast<unsigned short>(mask),INT_MAX,INT_MIN,-32768,-1);
        check(static_cast<unsigned short>(mask),0,0,0,0);
    }
    std::printf("PASS cases=%lu\n",cases);
}
