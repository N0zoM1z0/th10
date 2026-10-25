#!/usr/bin/env python3
"""Host-only complete tail initializer contract, not native ABI/product proof.

Extracts the complete production initializer and real RNG methods. Uses actual
pointer-free tail/RNG layouts; only owner x86 pointer assertions are removed.
Checks every output byte and guard, RNG saved seed and wraparound counter against
an independent rotate reference. No original target execution is claimed.
"""
from pathlib import Path
import importlib.util, re, subprocess, tempfile, os
ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('save_fixture',ROOT/'scripts/test-game-statistics-save.py')
h=importlib.util.module_from_spec(spec);spec.loader.exec_module(h)
def main():
 source=(ROOT/'src/GameStatisticsLoad.cpp').read_text()
 tail=h.balanced(source,'void InitializeStatisticsTail42ADD0(GameStatisticsTail *tail)\n{')
 header=h.strip_includes((ROOT/'src/GameStatistics.hpp').read_text())
 header=re.sub(r'// GAME_STATISTICS_X86_LAYOUT_BEGIN.*?// GAME_STATISTICS_X86_LAYOUT_END','// host64 owner pointer seam',header,flags=re.S)
 rng=h.strip_includes((ROOT/'src/Rng.hpp').read_text())
 methods=h.strip_includes((ROOT/'src/RandomMath.cpp').read_text())
 program='#include <cstring>\n#include <cstddef>\n#include <climits>\n#include <cstdio>\n'+header+rng+methods+'\nRngView g_RngView;\n'+tail+r"""
int main() {
 for (unsigned seed=0;seed<65536;seed+=17) {
  struct Fixture { unsigned int before; GameStatisticsTail tail; unsigned int after; } f;
  memset(&f,0xa5,sizeof(f));
  unsigned char want[0x448]={0};want[0]=0x53;want[1]=0x54;want[8]=0x48;want[9]=4;
  memset(want+0xc,' ',8);
  unsigned state=seed;
  for(unsigned i=0;i<512;++i) {
   unsigned t=((state ^ 0x9630u)-0x6553u)&65535;
   state=((t<<2)|(t>>14))&65535;
   want[0x46+i*2]=static_cast<unsigned char>(state);
   want[0x47+i*2]=static_cast<unsigned char>(state>>8);
  }
  unsigned count=(seed&1)?0xffffff00u:seed;
  g_RngView.seed=static_cast<unsigned short>(seed);g_RngView.savedSeed=0x1234;g_RngView.generationCount=count;
  InitializeStatisticsTail42ADD0(&f.tail);
  if(memcmp(&f.tail,want,sizeof(want)) || f.before!=0xa5a5a5a5 || f.after!=0xa5a5a5a5 ||
     g_RngView.seed!=state || g_RngView.savedSeed!=0x1234 || g_RngView.generationCount!=count+512u) {
    fprintf(stderr,"tail contract failed seed=%u\n",seed);return 1;
  }
 }
 puts("statistics tail:3856 full-byte/guard/RNG cases passed");return 0;
}
"""
 with tempfile.TemporaryDirectory(prefix='statistics-tail-') as temp:
  path=Path(temp);(path/'test.cpp').write_text(program)
  subprocess.run(['g++','-std=c++11','-O1','-Wall','-Wextra','-Werror','-fno-omit-frame-pointer','-fsanitize=address,undefined','-fno-pie','-no-pie',str(path/'test.cpp'),'-o',str(path/'test')],check=True)
  subprocess.run([str(path/'test')],check=True,timeout=60,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'))
 return 0
if __name__=='__main__':raise SystemExit(main())
