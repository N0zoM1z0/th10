#!/usr/bin/env python3
"""Source-only linkage/provenance gate; not a native linker or ABI test."""
import argparse,re,json,hashlib
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('--root',type=Path,required=True);a=p.parse_args();src=a.root/'src'
rows=[('EnemyEclDispatcher.cpp','void EnemyMarkPendingInterrupt(int *id)',0),('Player.cpp','void PlayerSetShotVmInterrupt2(unsigned int *vmId)',0),('FrontEnd.cpp','void ResultMenuPendingThree(AnmVmIdView *id)',0),('FrontEnd.cpp','AnmVmIdView ResultMenuFindScript(AnmVmIdView *id, int scriptIndex)',0),('FrontEnd.cpp','int __stdcall FrontEndPlayMusic(int slot, int trackIndex)',0),('DecomphelpLeafAccessors.cpp','void Fn004175E0(u8 *p0)',2),('DecomphelpLeafAccessors.cpp','u8 * Fn00417010(u8 *p0)',2)]
out=[]
for file,sig,depth in rows:
 raw=(src/file).read_bytes();s=raw.decode();clean=re.sub(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',lambda m:'\n'*m.group().count('\n'),s)
 matches=[m for m in re.finditer(re.escape(sig),clean) if clean[m.end():].lstrip().startswith('{')];assert len(matches)==1,(file,sig,len(matches));m=matches[0];prefix=clean[:m.start()];assert prefix.count('{')-prefix.count('}')==depth,(file,sig,'scope');assert not re.search(r'\bstatic\b[^;{}]*\b'+re.escape(sig.split('(')[0].split()[-1])+r'\s*\(',clean),(file,sig,'static')
 if depth:assert 'namespace th10' in prefix and 'namespace leaf' in prefix
 out.append({'file':file,'signature':sig,'scope_depth':depth,'sha256':hashlib.sha256(raw).hexdigest()})
assert re.search(r'typedef\s+unsigned char\s+u8\s*;', (src/'DecomphelpLeafTypes.hpp').read_text())
g=(src/'Gui.cpp').read_text();assert 'extern unsigned char g_GameStageState[];' in g;assert 'th10::leaf::Fn004175E0(g_GameStageState);' in g
print(json.dumps({'source_only':True,'definitions':out,'stage_state_contract':'existing byte storage, target474C40; host seam does not execute leaf+3C'},indent=2))
