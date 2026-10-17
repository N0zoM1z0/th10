#!/usr/bin/env python3
"""Static target-decode inventory only; no compiled candidate/exactness claim."""
import argparse,re,ast,hashlib,json
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('--decode',type=Path,required=True);p.add_argument('--source',type=Path);a=p.parse_args();raw=a.decode.read_bytes();s=raw.decode();rows=[]
assert hashlib.sha256(raw).hexdigest()=='11e74da4c61244ace73debec6376da65b88687a4045670f277b864bc21bd247b', 'decode differs from reviewed v2 target export'
for line in s.splitlines():
 m=re.match(r'^([0-9A-F]{8}) ([0-9a-f]+)\s',line)
 if m and 0x415e90<=int(m[1],16)<0x416f62:rows.append((int(m[1],16),bytes.fromhex(m[2])))
cur=0x415e90
for addr,data in rows:assert addr==cur,hex(cur);cur+=len(data)
assert cur==0x416f62
sizes=[]
for addr,name,n in [(0x416f64,'opcode',24),(0x416fc4,'portraitstage',7),(0x416fe0,'namestage',7),(0x416ffc,'difficulty',5)]:
 m=re.search(r'^TABLE '+hex(addr)+' '+name+r' (.*)$',s,re.M);assert m;v=ast.literal_eval(m[1]);assert len(v)==n;assert all(0x415e90<=int(x,16)<0x416f62 for x in v);sizes.append(n)
assert sum(sizes)*4==172
if a.source:
 src=a.source.read_text();run=src[src.index('int __stdcall GuiMessageVmView::Run'):src.index('// Target 0x00414900')];assert len(re.findall(r'case GUI_MSG_',run))==24
print(json.dumps({'source_only':True,'owner_bytes':4480,'code_bytes':cur-0x415e90,'alignment_bytes':2,'table_bytes':172,'table_counts':sizes,'decode_sha256':hashlib.sha256(raw).hexdigest(),'target_owner_sha256':'13c80eaec4d7e90718b107f1e37ea8de7bd57d8ac3f86f0af4b6269644614c35'},indent=2))
