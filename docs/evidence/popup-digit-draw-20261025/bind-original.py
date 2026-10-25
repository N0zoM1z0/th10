import sys,json,struct,hashlib
from pathlib import Path
sys.path.insert(0,'scripts')
from linked_image import PEImage
b=Path('.analysis/gpt-dots/popup-digit-draw-20261025');r=json.loads((b/'review.json').read_text())
bindings=[(1,'?g_AnmHardwareFlags@@3EA',0x491d78),(22,'?g_StageFogEnabled@@3HA',0x492378),(32,'?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A',0x491c10),(37,'?FlushVertexBuffer@AnmRenderManagerView@@QAEXXZ',0x442f50),(42,'?g_Direct3DDevice@@3PAUD3d9DeviceView@@A',0x491c30),(52,'?g_StageFogEnabled@@3HA',0x492378),(71,'?g_Player@@3PAUPlayer@@A',0x477834),(102,'__real@41000000',0x470bd0),(141,'__real@3f000000',0x470b0c),(150,'__real@43600000',0x470b4c),(165,'__real@41800000',0x470b48),(213,'__ftol2',0x463b2c),(427,'?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A',0x491c10),(460,'?DrawNoRotation@AnmRenderManagerView@@QAEHPAUAnmVmView@@@Z',0x443080),(506,'?g_Player@@3PAUPlayer@@A',0x477834)]
target=PEImage(Path('resources/th10.exe'));code=target.read_address(0x42b780,540)
for off,sym,addr in bindings:
 value=struct.unpack_from('<I',code,off)[0]
 if off in (37,213,460):value=(value+0x42b780+off+4)&0xffffffff
 assert value==addr,(off,value,addr)
normalized={};rows={}
for trial in ('first','model','identity'):
 fields=r['trials'][trial]['fields']['fields'];assert len(fields)==len(bindings)
 im=PEImage(Path('build/gpt-dots/popup-digit-draw-'+trial+'/source.exe'));data=bytearray(im.read_address(0x401000,r['trials'][trial]['size']));rr=[]
 for field, (off,sym,addr) in zip(fields,bindings):
  expected=sym.replace('g_AnmHardwareFlags@@3EA','g_PopupRenderFlags@@3EA') if trial!='identity' else sym
  assert expected in field['candidate_symbols'],(trial,field,expected)
  val=addr if field['type']=='DIR32' else (addr-0x42b780-field['offset']-4)&0xffffffff
  struct.pack_into('<I',data,field['offset'],val)
  if sym.startswith('__real@'): assert im.read_address(field['candidate_target'],4)==target.read_address(addr,4)
  rr.append({'candidate_offset':field['offset'],'target_offset':off,'symbol':sym,'target':hex(addr),'type':field['type']})
 normalized[trial]=data;rows[trial]=rr
assert normalized['model']==normalized['identity']
result={'result':'structural-rejection','target_size':540,'candidate_sizes':{k:len(v) for k,v in normalized.items()},'all_fields':rows,'target_local_branches':r['target']['locals'],'model_identity_normalized_equal':True,'exactness_credit':0,'limitation':'Different extents/local layout prohibit ordinary mismatch-byte score; field role order binding is diagnostic only.'}
(b/'bindings.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k not in ('all_fields','target_local_branches')},indent=2))
