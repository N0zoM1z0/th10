#include "winmm_axes.hpp"
#include <cstdio>
#include <cstdlib>
// Host-only independent oracle uses 64-bit unsigned long and explicit modulo.
// The candidate itself always uses unsigned32 and no widening arithmetic.
typedef char oracle_host_width[sizeof(unsigned long)>=8 ? 1:-1];
static unsigned long cases=0;
static void bounds(unsigned int min,unsigned int max,unsigned int& lower,unsigned int& upper) {
    const unsigned long modulus=4294967296UL;
    const unsigned long mid=(static_cast<unsigned long>(min)+max)%modulus/2;
    const unsigned long quarter=(static_cast<unsigned long>(max)+modulus-min)%modulus/4;
    lower=static_cast<unsigned int>((mid+modulus-quarter)%modulus);
    upper=static_cast<unsigned int>((mid+quarter)%modulus);
}
static void check(unsigned short mask,unsigned int x,unsigned int y,unsigned int xmin,unsigned int xmax,unsigned int ymin,unsigned int ymax) {
    unsigned int xl,xu,yl,yu; bounds(xmin,xmax,xl,xu);bounds(ymin,ymax,yl,yu);
    unsigned int add=0;
    if(xl>x)add+=64;
    if(xu<x)add+=128;
    if(yl>y)add+=16;
    if(yu<y)add+=32;
    const unsigned int expected=mask|add;
    const unsigned int actual=input_candidate::WinMMAxes(mask,x,y,xmin,xmax,ymin,ymax);
    if(expected!=actual) { std::printf("SEMANTIC_MISMATCH case=%lu expected=%u actual=%u x=%u xmin=%u xmax=%u\n",cases,expected,actual,x,xmin,xmax);std::exit(10); }
    ++cases;
}
int main() {
    const unsigned int limits[]={0,1,2,3,4,7,8,15,16,65535,65536,0x3fffffffu,0x40000000u,0x7ffffffeu,0x7fffffffu,0x80000000u,0x80000001u,0xfffffffeu,0xffffffffu};
    for(unsigned int a=0;a<sizeof(limits)/sizeof(limits[0]);++a)
    for(unsigned int b=0;b<sizeof(limits)/sizeof(limits[0]);++b) {
        const unsigned int xmin=limits[a],xmax=limits[b],ymin=limits[b],ymax=limits[a];
        unsigned int xl,xu,yl,yu;bounds(xmin,xmax,xl,xu);bounds(ymin,ymax,yl,yu);
        const unsigned int xs[]={0,0xffffffffu,xl-1,xl,xl+1,xu-1,xu,xu+1,0x7fffffffu,0x80000000u};
        const unsigned int ys[]={0,0xffffffffu,yl-1,yl,yl+1,yu-1,yu,yu+1,0x7fffffffu,0x80000000u};
        for(unsigned int i=0;i<10;++i)for(unsigned int j=0;j<10;++j) {
            check(0,xs[i],ys[j],xmin,xmax,ymin,ymax);
            check(0xff5fu,xs[i],ys[j],xmin,xmax,ymin,ymax);
        }
    }
    for(unsigned int v=0;v<65536u;++v) {
        unsigned int lower,upper;bounds(v,~v,lower,upper);
        const unsigned int p[]={lower-1,lower,lower+1,upper-1,upper,upper+1,0,0xffffffffu};
        for(unsigned int i=0;i<8;++i)check(static_cast<unsigned short>(v),p[i],~p[i],v,~v,~v,v);
        check(static_cast<unsigned short>(v),0,0xffffffffu,0x80000000u,0xffffffffu,0xffffffffu,0);
        check(static_cast<unsigned short>(v),0xffffffffu,0,0xffffffffu,0,0x80000000u,0xffffffffu);
    }
    unsigned int random=0x9513beefu;
    for(unsigned int i=0;i<100000u;++i) {
        unsigned int values[6];
        for(unsigned int j=0;j<6;++j){random=random*1664525u+1013904223u;values[j]=random;}
        check(static_cast<unsigned short>(random>>16),values[0],values[1],values[2],values[3],values[4],values[5]);
    }
    std::printf("PASS cases=%lu\n",cases);
}
