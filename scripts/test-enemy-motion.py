#!/usr/bin/env python3
"""Bounded actual-source interpolation/motion tests, not x87 or ABI proof."""
from pathlib import Path
import shutil, subprocess, tempfile, unittest
ROOT = Path(__file__).resolve().parents[1]
def definition(text, marker, semicolon=False):
    start=text.index(marker); brace=text.index('{',start); depth=1; end=brace+1
    while depth:
        if text[end]=='{': depth+=1
        elif text[end]=='}': depth-=1
        end+=1
    return text[start:end]+(';' if semicolon else '')+'\n'
class EnemyMotion(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_actual_motion_and_interpolation(self):
        h=(ROOT/'src/Enemy.hpp').read_text(); cpp=(ROOT/'src/Enemy.cpp').read_text()
        ah=(ROOT/'src/AnmManager.hpp').read_text(); ac=(ROOT/'src/AnmManager.cpp').read_text()
        pieces=['#include <cassert>\n#include <cmath>\n#define __stdcall\nusing std::floor;\nstruct PlayerFloat3 {float x,y,z;};\nfloat g_AnmGameSpeed=1;\n']
        pieces += [definition(h,'struct '+name+'\n',True) for name in ['EnemyFloat2','EnemyMotionView','EnemyInterpolationTimerView','EnemyScalarInterpolationView']]
        pieces += [definition(ah,'enum AnmInterpolationModeView',True),definition(ac,'float __stdcall CalculateAnmInterpolation('),definition(cpp,'struct EnemyRuntimeVectorView\n',True),definition(cpp,'void EnemyRuntimeVectorView::FromAngleMagnitude('),definition(cpp,'EnemyFloat2 *EnemyScalarInterpolationView::Evaluate('),definition(cpp,'void EnemyAdvanceMotion(')]
        pieces.append(r'''
static bool close(float a,float b){return std::fabs(a-b)<0.00002f;}
// Static zero initialization precedes the maintained timer constructor.
static EnemyScalarInterpolationView zeroSeed;
static EnemyScalarInterpolationView make(){
 EnemyScalarInterpolationView i=zeroSeed;i.initial={2,-4};i.final={10,8};i.initialTangent={3,-5};i.finalTangent={7,9};
 i.timer.previous=12;i.timer.current=0;i.timer.subframe=0;i.timer.scale=&g_AnmGameSpeed;i.timer.flags=1;i.duration=10;i.mode=0;return i;
}
int main(){
 EnemyFloat2 out;
 for(int mode=0;mode<=17;++mode){auto i=make();i.mode=mode;i.duration=1;
 assert(i.Evaluate(&out)==&out);assert(i.duration==0&&i.timer.current==1&&i.timer.previous==0&&i.timer.subframe==1);
 assert(out.x==(mode==7?2:10)&&out.y==(mode==7?-4:8));}
 auto i=make();float scale=0.5f;i.timer.scale=&scale;i.Evaluate(&out);
 assert(i.timer.current==0&&i.timer.previous==0&&i.timer.subframe==0.5f&&i.timer.scale==&scale);assert(close(out.x,2.4f)&&close(out.y,-3.4f));
 i=make();i.timer.flags=0;i.timer.scale=&scale;i.timer.current=9;i.timer.subframe=9.75f;i.Evaluate(&out);
 assert(i.duration==0&&i.timer.current==10&&i.timer.previous==9&&i.timer.subframe==10&&i.timer.flags==1&&i.timer.scale==&g_AnmGameSpeed);
 i=make();i.duration=0;i.mode=7;assert(i.Evaluate(&out)==&out);assert(out.x==12&&out.y==4&&i.initial.x==12&&i.initial.y==4&&i.timer.current==0);
 assert(i.Evaluate(&out)==&out);assert(out.x==22&&out.y==12);
 i=make();i.duration=-1;i.mode=17;assert(i.Evaluate(&out)==&out);assert(out.x==9&&out.y==5&&i.initial.x==9&&i.initial.y==5&&i.finalTangent.x==17&&i.finalTangent.y==17);
 assert(i.Evaluate(&out)==&out);assert(out.x==26&&out.y==22&&i.finalTangent.x==27&&i.finalTangent.y==25);
 i=make();i.mode=8;i.timer.subframe=2.7f;i.timer.current=2;i.Evaluate(&out);
 float t=i.timer.subframe/10.0f,m=t-1,iw=(1+(t+t))*m*m,fw=(3-(t+t))*t*t,it=(1-t)*(1-t)*t,ft=m*t*t;
 assert(close(out.x,iw*2+fw*10+it*3+ft*7)&&close(out.y,iw*-4+fw*8+it*-5+ft*9));
 // Ordinary finite fixtures distinguish floor from truncation, not native x87 boundaries.
 EnemyMotionView motion={};motion.position={0.125f,-0.125f,3};motion.velocity={0,0,2};EnemyAdvanceMotion(&motion);
 assert(close(motion.position.x,0.12f)&&close(motion.position.y,-0.13f)&&motion.position.z==5);
 motion={};motion.position={99,88,77};motion.velocity={0.125f,-0.125f,-3};motion.flags=5;motion.value1C=0;motion.value20=2;EnemyAdvanceMotion(&motion);
 assert(close(motion.position.x,2.12f)&&close(motion.position.y,-0.13f)&&motion.position.z==-3);
 motion={};motion.flags=3;motion.value1C=0.5f;motion.value20=2;motion.velocity={0.125f,-0.125f,4};
 float px=static_cast<float>(std::cos(0.5f))*2+0.125f,py=static_cast<float>(std::sin(0.5f))*2-0.125f;
 EnemyAdvanceMotion(&motion);assert(close(motion.position.x,static_cast<float>(floor(px*100.0f)*0.01f))&&close(motion.position.y,static_cast<float>(floor(py*100.0f)*0.01f))&&motion.position.z==4);
 motion={};motion.position={0,0,1};EnemyAdvanceMotion(&motion);assert(motion.position.x==0&&motion.position.y==0&&motion.position.z==1);
}
''')
        (ROOT/'build').mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=ROOT/'build',prefix='enemy-motion-') as d:
            p=Path(d);(p/'test.cpp').write_text(''.join(pieces))
            subprocess.run(['c++','-std=c++17','-O0',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
            subprocess.run([str(p/'test')],check=True)
if __name__=='__main__':unittest.main()
