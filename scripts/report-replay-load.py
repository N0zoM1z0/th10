#!/usr/bin/env python3
"""Audit target replay factory/FPS boundaries and their observed call protocols."""
from collections import Counter
import csv,json,struct,tomllib
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import X86_OP_IMM
from linked_image import PEImage,verify_capstone
from linked_allocation_eh import protocol,TARGET_FRAME_HANDLER,TARGET_DELETE
from target_identity import pe_bytes_at,resolve_target,verify_target
ROOT=Path(__file__).resolve().parents[1]


def require(condition,message):
    if not condition:raise ValueError(message)


def main():
    path=resolve_target(None);identity,problems=verify_target(path);require(not problems,'; '.join(problems))
    with (ROOT/'config/tools.lock.toml').open('rb')as stream:verify_capstone(tomllib.load(stream)['capstone'])
    raw=path.read_bytes();image=PEImage(path);decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    def decode(start,size):
        instructions=list(decoder.disasm(pe_bytes_at(raw,start,size),start))
        require(sum(i.size for i in instructions)==size,'incomplete dense decode')
        return instructions
    factory=decode(0x4296f0,181);fps=decode(0x429a70,182);callback=decode(0x42a430,28)
    calls=Counter(i.operands[0].imm for i in factory if i.mnemonic=='call'and i.operands[0].type==X86_OP_IMM)
    require(calls=={0x452493:1,0x45252d:1,0x42a200:1,0x4294a0:1,0x4524a1:1},'factory calls differ')
    returns=[i for i in factory if i.mnemonic=='ret']
    require(len(returns)==2 and all(i.operands[0].imm==4 for i in returns),'factory RET4 protocol differs')
    callers=(0x42ce7f,0x4239fa,0x423f23,0x43169f,0x431702,0x433608,0x4339aa)
    for site in callers:
        edge=decode(site,5)[0];require(edge.mnemonic=='call'and edge.operands[0].imm==0x4296f0,'factory callsite differs')
    require(struct.unpack('<I',pe_bytes_at(raw,0x4296f3,4))[0]==0x46564b,'factory EH entry differs')
    eh=protocol(image,0x46564b,TARGET_FRAME_HANDLER,TARGET_DELETE)
    require([i.operands[0].imm for i in fps if i.mnemonic=='call']==[0x401630],'FPS formatter differs')
    require(fps[-1].mnemonic=='ret'and not fps[-1].operands,'FPS return differs')
    require(callback[-1].mnemonic=='jmp'and callback[-1].operands[0].imm==0x429a70,'FPS callback edge differs')
    require(struct.unpack('<2f',pe_bytes_at(raw,0x470c2c,8))==(50.0,30.0),'FPS thresholds differ')
    require(pe_bytes_at(raw,0x46e658,4)==b'%3d\0','FPS format differs')
    with (ROOT/'config/function-boundaries.csv').open(newline='')as stream:bounds={int(r['address'],0):r for r in csv.DictReader(stream)}
    for start,size in [(0x4296f0,181),(0x429a70,182),(0x42a430,28)]:
        row=bounds[start];require(row['state']=='reviewed'and int(row['span_end'],0)==start+size-1,'target boundary differs')
    for start,size in [(0x4297a5,11),(0x429b26,10),(0x42a44c,4)]:require(pe_bytes_at(raw,start,size)==b'\xcc'*size,'adjacent padding differs')
    print(json.dumps({'authority':'target observations only; no exact or runtime credit','target':identity,
        'factory':{'entry':'0x4296f0','owned_bytes':181,'callers':[hex(x)for x in callers],'stack_cleanup':4,'allocation_eh':eh},
        'fps':{'entry':'0x429a70','owned_bytes':182,'callback':'0x42a430','callback_bytes':28,'thresholds':[30,50],'format':'%3d'},'result':'passed'},indent=2))

if __name__=='__main__':main()
