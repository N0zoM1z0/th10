#!/usr/bin/env python3
"""Local declaration-extraction/layout check; not MSVC ABI or native acceptance."""
import argparse,hashlib,subprocess,tempfile,sys,json
from pathlib import Path
if sys.flags.optimize:raise SystemExit('optimized Python is unsupported')
p=argparse.ArgumentParser();p.add_argument('--baseline-root',type=Path,required=True);p.add_argument('--overlay-tree',type=Path,required=True);a=p.parse_args()
root=Path(__file__).resolve().parent;src=a.baseline_root/'src';out=a.overlay_tree/'src'
def sha(x):return hashlib.sha256(x).hexdigest()
s=(src/'GameManager.cpp').read_bytes();assert sha(s)=='2a09b328a64e45f9f027bfe84b25b5800e21c5e59b7759a97d63925760ed9433'
assert sha((src/'Player.hpp').read_bytes())=='8969b24cfd14cfcfcdbaa1504968b0e9f56ea8b4972748de1e6bf65fada362ce'
alias=b'typedef unsigned int ZunBool;\r\ntypedef unsigned char u8;\r\ntypedef unsigned int u32;\r\n'
begin=s.index(b'struct GameChainElementView;\r\n');end=s.index(b'\r\nZunBool __fastcall GameManager::OnDraw()',begin);decl=s[begin:end]
header=(out/'GameManager.hpp').read_bytes();assert alias in header and decl in header
expected=s.replace(alias,b'',1).replace(decl,b'',1).replace(b'#include "Main.hpp" // Shield ANM imports before the Windows SDK.\r\n',b'#include "Main.hpp" // Shield ANM imports before the Windows SDK.\r\n#include "GameManager.hpp"\r\n',1).replace(b'extern GameManager *g_GameManager;                         // 0x477810\r\n',b'',1)
assert (out/'GameManager.cpp').read_bytes()==expected
for name in ('GameManager.cpp','GameManager.hpp','Main.hpp'):
 data=(out/name).read_bytes();assert data.count(b'\n')==data.count(b'\r\n')
assert b'\r' not in (out/'Main.cpp').read_bytes()
prefix='#define __fastcall\n#define __forceinline inline\n#define __declspec(x)\n'
guards='''
static_assert(sizeof(GameManager)==0x60,"size");
static_assert(alignof(GameManager)==4,"alignment");
static_assert(offsetof(GameManager,stageResourceIndex)==4,"resource");
static_assert(offsetof(GameManager,updateChain)==8,"update");
static_assert(offsetof(GameManager,drawChain)==12,"draw");
static_assert(offsetof(GameManager,stageTimer)==0x10,"timer");
static_assert(offsetof(GameManager,configuration)==0x24,"config");
static_assert(offsetof(GameManager,m_Flags)==0x58,"flags");
static_assert(offsetof(GameManager,initializationMode)==0x5c,"mode");
static_assert(sizeof(((GameManager*)0)->initializationMode)==4,"mode width");
'''
with tempfile.TemporaryDirectory(prefix='helper572-layout-') as td:
 d=Path(td);(d/'string.h').write_text('#include <stddef.h>\nextern "C" void *memset(void *, int, size_t);\n')
 def compile(body):
  (d/'check.cpp').write_text(prefix+body+guards)
  return subprocess.run(['g++','-m32','-std=c++17','-fsyntax-only','-I'+str(d),'-I'+str(out),'-I'+str(src),str(d/'check.cpp')],capture_output=True,timeout=30).returncode
 assert compile('#include "Player.hpp"\n'+alias.decode()+decl.decode())==0
 assert compile('#include "GameManager.hpp"\n')==0
 for old,new in ((b'int initializationMode;',b'short initializationMode;'),(b'u32 configuration[13];',b'u32 configuration[14];')):
  assert header.count(old)==1
  (d/'GameManager.hpp').write_bytes(header.replace(old,new))
  assert compile('#include "GameManager.hpp"\n')!=0
print(json.dumps({'status':'PASS','raw_extraction_identity':True,'layout_compiles':2,'layout_mutants_rejected':2,'msvc_or_native_abi_proof':False}))
