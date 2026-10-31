#!/usr/bin/env python3
"""Strict complete-owner TH10 RequestCaptureRectangle nonexact diagnostic."""
from pathlib import Path
import argparse,copy,json,runpy,sys,tomllib,hashlib
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'scripts'))
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--artifact-dir',type=Path,required=True);args=p.parse_args();directory=args.artifact_dir.resolve();directory.relative_to(ROOT/'build')
 target=ROOT/'resources/th10.exe'
 assert hashlib.sha256(target.read_bytes()).hexdigest()=='2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040'
 with (ROOT/'config/match-units.toml').open('rb') as f:m=tomllib.load(f)
 unit=dict(artifact_kind='linked-pe',source='src/FrontEnd.cpp',support_sources=['src/AnmManager.cpp'],output_dir=str(directory.relative_to(ROOT)),profile=['/TP','/MT','/O2','/Gy','/GF','/Oi','/DNDEBUG','/Isrc','/GL'],entry_symbol='?ResultMenuOpenPause@@YGXPAUResultMenuView@@@Z',harness='unresolved-symbol-data-anchors-v1',functions=['AnmRenderManagerView::RequestCaptureRectangle'],symbol='?RequestCaptureRectangle@AnmRenderManagerView@@QAEHUAnmVmIdView@@HHHH@Z',target_address=0x00424480,size=162,linkages=[
 dict(offset=0x07,type='DIR32',symbol='?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A',target=0x00491C10),
 dict(offset=0x10,type='REL32',symbol='?FindVm@AnmRenderManagerView@@QAEPAUAnmVmView@@UAnmVmIdView@@@Z',target=0x004491C0),
 *[dict(offset=offset,type='REL32',symbol='__ftol2',target=0x00463B2C) for offset in (0x20,0x2A,0x36,0x42)]])
 m['units']['diagnostic-request-capture']=unit
 compare=runpy.run_path(str(ROOT/'scripts/compare-linked-function.py'))['compare_unit']
 r=compare('diagnostic-request-capture',_unit_manifest=m)
 assert r['result']=='mismatch' and r['size']==162 and r['matched_bytes']==148,r
 assert len(r['linkages'])==6 and len(r['first_differences'])==14
 expected=[0x64,0x65,0x67,0x6B,0x6C,0x6E,0x73,0x7A,0x7B,0x7D,0x81,0x84,0x8F,0x92]
 assert [int(x['offset'],16) for x in r['first_differences']]==expected,r
 print(json.dumps(dict(result='complete-nonexact',matched_bytes=148,target_size=162,native_size=162,field_bindings=6,ordinary_difference_offsets=[hex(x) for x in expected],exact_credit=0),indent=2))
if __name__=='__main__':main()
