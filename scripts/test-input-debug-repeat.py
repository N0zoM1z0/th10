#!/usr/bin/env python3
"""Check actual 0x40AC20 source against target-derived edge/threshold fixtures.

This host test establishes neither Windows runtime nor historical codegen.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class InputDebugRepeat(unittest.TestCase):
    @unittest.skipUnless(shutil.which('c++'), 'host C++ compiler unavailable')
    def test_actual_source(self):
        program = r'''
#define __declspec(x)
#include "InputState.hpp"
#include <cassert>
#include <cstring>
'''
        program += (ROOT / 'src/InputState.cpp').read_text()
        program += r'''
int main(){
 const unsigned short masks[]={0,1,2,0x8000,0xffff,0xa5a5};
 const unsigned short before[]={0,24,25,26,65534,65535};
 for(unsigned short held:masks)for(unsigned short previous:masks)for(unsigned seed=0;seed<6;++seed){
  FrontEndInputView state{};
  state.current=0x12345678;state.repeated=0xffff;state.pressed=0x8217;state.mask008=0x81ab0123;
  std::memset(state.unknown00C,0xa5,sizeof state.unknown00C);state.unknown036=0xbeef;
  state.previousRaw=held;state.previousHeld=previous;state.repeatFlags=0x400;
  unsigned short expected[16];bool anyRepeat=false;
  for(unsigned bit=0;bit<16;++bit){
   unsigned old=before[(seed+bit)%6];state.repeatCounters[bit]=static_cast<unsigned short>(old);
   unsigned next=(held&(1u<<bit))?((old+1)&0xffffu):0;
   if(next>=26){next-=8;anyRepeat=true;}
   expected[bit]=static_cast<unsigned short>(next);
  }
  state.UpdateDebugRepeat();
  assert(state.current==0x12345678&&state.repeated==0&&state.pressed==0x8217&&state.mask008==0x81ab0123);
  assert(state.previousRaw==held&&state.previousHeld==previous&&state.unknown036==0xbeef);
  for(unsigned char c:state.unknown00C)assert(c==0xa5);
  for(unsigned bit=0;bit<16;++bit)assert(state.repeatCounters[bit]==expected[bit]);
  assert(state.repeatFlags==(anyRepeat?0x401:0x400));
  assert(state.frameMask032==static_cast<unsigned short>(held&~previous));
  assert(state.frameMask034==static_cast<unsigned short>(previous&~held));
 }
 // A high-numbered key still ORs bit zero, and never clears old repeat flags.
 FrontEndInputView state{};state.previousRaw=0x8000;state.repeatCounters[15]=25;state.repeatFlags=0x80;
 state.UpdateDebugRepeat();assert(state.repeatCounters[15]==18&&state.repeatFlags==0x81);
 state.previousRaw=0;state.UpdateDebugRepeat();assert(state.repeatFlags==0x81&&state.repeatCounters[15]==0);
}
'''
        with tempfile.TemporaryDirectory(prefix='th10-debug-repeat-') as tmp:
            source, binary = Path(tmp) / 'fixture.cpp', Path(tmp) / 'fixture'
            source.write_text(program)
            subprocess.run(['c++', '-std=c++17', '-O2', '-I', str(ROOT / 'src'),
                            str(source), '-o', str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == '__main__':
    unittest.main()
