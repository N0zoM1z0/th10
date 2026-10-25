#!/usr/bin/env python3
"""Replay complete SptTest owners, seven local branches and the real ASCII callee."""
import argparse
import importlib.util
import json
from pathlib import Path
import tomllib
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP
from capstone.x86_const import X86_OP_IMM
from linked_image import PEImage, linked_functions
ROOT = Path(__file__).resolve().parents[1]

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output-dir', default='build/match-linked/SptTestDraw-cold')
    args=parser.parse_args()
    directory=(ROOT/args.output_dir).resolve()
    assert directory.is_relative_to((ROOT/'build').resolve())
    spec=importlib.util.spec_from_file_location('compare',ROOT/'scripts/compare-linked-function.py')
    comparator=importlib.util.module_from_spec(spec);spec.loader.exec_module(comparator)
    manifest=tomllib.loads((ROOT/'config/match-units.toml').read_text())
    units={key:dict(manifest['units'][key],output_dir=str(directory.relative_to(ROOT)))
           for key in ('spt-test-draw','spt-test-draw-callback')}
    draw=units['spt-test-draw']
    # Preserve the original complete callee contract; only select this image/TU.
    old=manifest['units']['ascii-manager-add-small-format-text-gs']
    units['spt-test-ascii-guard']={**draw,**{key:old[key] for key in
        ('symbol','functions','size','target_address','linkages')},'pdb_source':'src/AnmManager.cpp'}
    selected={'target_sha256':manifest['target_sha256'],'units':units}
    results={key:comparator.compare_unit(key,_unit_manifest=selected) for key in units}
    assert all(row['result']=='exact' for row in results.values())
    image=PEImage(directory/'source.exe');target=comparator.verified_target()
    functions=linked_functions(directory/'source.exe',directory/'source.map',directory/'source.pdb')['functions']
    function=[f for f in functions if f['symbol']==draw['symbol']]
    assert len(function)==1 and function[0]['size']==531
    address=int(function[0]['address'],16)
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    def locals(image,start):
        result=[];instructions=list(decoder.disasm(image.read_address(start,531),start))
        starts={i.address for i in instructions}
        assert sum(i.size for i in instructions)==531
        for i in instructions:
            if i.group(CS_GRP_JUMP) and i.operands[0].type==X86_OP_IMM:
                destination=i.operands[0].imm
                assert destination in starts
                result.append([i.address-start,i.size,destination-start])
        return result
    expected=[[56,6,276],[63,2,179],[66,6,520],[315,2,345],[343,2,358],[384,2,417],[415,2,430]]
    assert locals(image,address)==locals(target,0x42c360)==expected
    print(json.dumps({'result':'exact','units':results,'fixed_local_branches':expected},indent=2))

if __name__=='__main__':main()
