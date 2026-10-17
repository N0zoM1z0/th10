#!/usr/bin/env python3
"""Lexically extract maintained owner/declarations; bounded pointer-neutral host contracts.
The mocks deliberately do not prove target layout, ABI, rendering, or x87 rounding.
Run with python3 [-O] this_file.py --source-root PATH [--mutations].
"""
from pathlib import Path
import argparse
import json
import re
import subprocess
import tempfile
import hashlib
import struct
import os

def require(condition, text):
    if not condition:
        raise RuntimeError(text)

def mask_cpp(text):
    # Preserve offsets/newlines while excluding braces and identifiers in literals/comments.
    pattern = r'//[^\n]*|/\*[\s\S]*?\*/|R"([^ ()\\\t\r\n]{0,16})\([\s\S]*?\)\1"|"(?:\\[\s\S]|[^"\\])*"|\'(?:\\[\s\S]|[^\'\\])*\''
    return re.sub(pattern, lambda m: ''.join('\n' if c=='\n' else ' ' for c in m.group()), text)

def extract(text, pattern, declaration=False):
    masked=mask_cpp(text)
    matches=list(re.finditer(pattern,masked))
    require(len(matches)==1, 'Expected one source match: '+pattern)
    start=matches[0].start()
    begin=masked.find('{',matches[0].end())
    require(begin>=0,'Missing body')
    depth=1; end=begin+1
    while depth and end<len(masked):
        depth += (masked[end]=='{')-(masked[end]=='}'); end+=1
    require(depth==0,'Unbalanced source body')
    if declaration:
        while end<len(masked) and masked[end].isspace(): end+=1
        require(masked[end:end+1]==';','Missing declaration terminator');end+=1
    return text[start:end]

def replace_once(body, old, new):
    require(body.count(old)==1, 'Mutation anchor is not unique: '+old)
    return body.replace(old,new,1)

CASES=['unconditional','horizontal','horizontal_positive','gauge_gate','gauge','gauge_reload','segment_zero','marker','final_pair','pair_reload','enemy_reload','post_marker_gates','segment_dynamic_gate']

def load(root):
    gui=(root/'src/Gui.hpp').read_text()
    source=(root/'src/GuiDraw.cpp').read_text()
    owner=extract(source,r'int\s+__stdcall\s+GuiView::DrawStageElements\s*\(\s*GuiView\s*\*\s*gui\s*\)')
    callback=extract(source,r'int\s+__fastcall\s+GuiDrawCallback\s*\(\s*void\s*\*\s*owner\s*\)')
    coefficient=re.findall(r'0\.29230770468711853f', mask_cpp(owner))
    require(len(coefficient)==1 and struct.pack('<f',float(coefficient[0][:-1])).hex()=='5ba9953e','Horizontal coefficient must retain target binary32 bits 0x3e95a95b')
    gauge=extract(gui,r'struct\s+GuiBossGaugeValueView\b',True)
    actual_gui=extract(gui,r'struct\s+GuiView\b',True)
    require(re.search(r'unsigned\s+(?:int\s+)?color\s*;',gauge),'Gauge color must be actual unsigned field')
    # Discover actual rectangle declaration through its use, then locate its complete definition.
    match=re.search(r'\b(\w+)\s+rectangle\s*;',mask_cpp(owner))
    require(match is not None,'Expected named rectangle local')
    recttype=match.group(1)
    definitions=[]
    for p in sorted((root/'src').glob('*.hpp')):
        content=p.read_text()
        if re.search(r'struct\s+'+re.escape(recttype)+r'\s*\{',mask_cpp(content)):
            definitions.append(extract(content,r'struct\s+'+re.escape(recttype)+r'\b',True))
    require(len(definitions)==1,'Exactly one real rectangle declaration required')
    template=(Path(__file__).parent/'fixtures/gui-draw.cpp.in').read_text()
    for key,value in [('GAUGE',gauge),('GUI',actual_gui),('RECTANGLE',definitions[0]),('RECTTYPE',recttype),('CALLBACK',callback)]:
        template=template.replace('@'+key+'@',value)
    return template,owner

def checks(generated):
    result={}
    for n,line in enumerate(generated.splitlines(),1):
        m=re.search(r'CHECK\("([^\"]+)",\s*(.*)\);',line)
        if m:
            name=m.group(1)
            # Use preprocessor stringification, not a guessed normalized Python spelling.
            require(name not in result,'Duplicate CHECK id '+name)
            result[name]=n
    return result

def compile_run(generated, directory, name, flags, cases, expected=None):
    cpp=directory/(name+'.cpp'); exe=directory/name;cpp.write_text(generated)
    p=subprocess.run(['g++','-std=c++17','-Wall','-Wextra','-Werror',*flags,str(cpp),'-o',str(exe)],capture_output=True,text=True,timeout=60)
    require(p.returncode==0,'Compile failure is never a mutation kill: '+name+'\n'+p.stderr)
    for case in cases:
        p=subprocess.run([str(exe),case],capture_output=True,text=True,timeout=60)
        if expected:
            label,line,expression=expected
            want=f'CHECK|{label}|{line}|{expression}\n'
            require(p.returncode==41 and p.stderr==want and p.stdout=='', f'Wrong kill {name}/{case}: rc={p.returncode}, stderr={p.stderr!r}; wanted {want!r}')
        else:
            require(p.returncode==0 and p.stdout=='PASS\n' and not p.stderr, f'Positive failed {name}/{case}: {p.returncode} {p.stdout} {p.stderr}')

# Mutants use a specific case and intended unique CHECK. Compile failures, wrong assertions,
# sanitizer exits/signals and merely nonzero status are explicitly rejected.
def mutations(owner):
    result=[]
    def add(name,old,new,case,label): result.append((name,old,new,case,label))
    add('swap_interleave','Draw(&gui->displayVms0[i]);','Draw(&gui->displayVms1[i]);','unconditional','base_vm_order')
    add('omit_array2','i < 9','i < 8','unconditional','base_count')
    add('omit_array3','i < 4; ++i)\n        g_AnmRenderManagerView->Draw(&gui->displayVms3[i]);','i < 3; ++i)\n        g_AnmRenderManagerView->Draw(&gui->displayVms3[i]);','unconditional','base_count')
    add('omit_array5','i < 7','i < 6','unconditional','base_count')
    changed=owner.replace('int i;', 'int i; AnmRenderManagerView *cached = g_AnmRenderManagerView;').replace('g_AnmRenderManagerView->Draw','cached->Draw')
    add('cache_anm',owner,changed,'unconditional','base_manager_reload')
    add('horizontal_unsigned_gate','g_GuiState4C58 != 0','g_GuiState4C58 > 0','horizontal','horizontal_signed_count')
    add('horizontal_zero_gate','g_GuiState4C58 != 0','g_GuiState4C58 == 0','horizontal','horizontal_signed_count')
    add('horizontal_coefficient','0.29230770468711853f','0.25f','horizontal','horizontal_first')
    add('horizontal_alpha_shift','gui->displayVms5[0].primaryColor.alpha) << 24;','gui->displayVms5[0].primaryColor.alpha) << 16;','horizontal','horizontal_first')
    add('horizontal_alpha_cache','(static_cast<unsigned int>(\n            gui->displayVms5[0].primaryColor.alpha) << 24) | 0x00ffffffu','color | 0x00ffffffu','horizontal','horizontal_reload')
    add('horizontal_right_recompute','rectangle.right = rectangle.right - 1.0f;\n        rectangle.bottom = 462.0f;','rectangle.right = static_cast<float>(g_GuiState4C58) * 0.29230770468711853f + 50.0f;\n        rectangle.bottom = 462.0f;','horizontal','horizontal_reload')
    add('gauge_unordered_gate','gui->bossGaugeDisplayed > 0.0f','!(gui->bossGaugeDisplayed <= 0.0f)','gauge_gate','gauge_ordered_gate')
    add('gauge_nonnegative_gate','gui->bossGaugeDisplayed > 0.0f','gui->bossGaugeDisplayed >= 0.0f','gauge_gate','gauge_ordered_gate')
    add('gauge_width','gui->bossGaugeDisplayed * 342.0f + 41.0f','gui->bossGaugeDisplayed * 341.0f + 41.0f','gauge','gauge_background')
    add('gauge_right_recompute','rectangle.right = rectangle.right - 1.0f;\n        rectangle.bottom = 24.0f;','rectangle.right = gui->bossGaugeDisplayed * 342.0f + 40.0f;\n        rectangle.bottom = 24.0f;','gauge_reload','gauge_right_reload')
    add('segment_positive_only','gui->bossGaugeValues[i].value != 0.0f','gui->bossGaugeValues[i].value > 0.0f','gauge','gauge_count')
    add('segment_skip_nan','gui->bossGaugeValues[i].value != 0.0f','!std::isnan(gui->bossGaugeValues[i].value) && gui->bossGaugeValues[i].value != 0.0f','gauge','gauge_count')
    add('segment_include_zero','gui->bossGaugeValues[i].value != 0.0f','gui->bossGaugeValues[i].value >= 0.0f','segment_zero','segment_zero_count')
    add('segment_max','gui->bossGaugeValues[i].value <','gui->bossGaugeValues[i].value >','gauge','segment_less')
    add('segment_no_clamp','? gui->bossGaugeValues[i].value : gui->bossGaugeDisplayed','? gui->bossGaugeValues[i].value : gui->bossGaugeValues[i].value','gauge','segment_clamp')
    add('segment_color','color = gui->bossGaugeValues[i].color;','color = 0xffffffffu;','gauge','segment_less')
    add('marker_flag10','(boss->runtime.flags & 0x10u) == 0','(boss->runtime.flags & 0x10u) != 0','marker','marker_flags')
    add('marker_flag1','(boss->runtime.flags & 1u) == 0','(boss->runtime.flags & 1u) != 0','marker','marker_flags')
    add('marker_overbroad_flags','(boss->runtime.flags & 0x10u) == 0','boss->runtime.flags == 0','marker','marker_flags')
    add('marker_vm','Draw(&gui->bossPositionVm);','Draw(&gui->displayVms4[0]);','marker','marker_vm')
    add('enemy_cache','manager = g_EnemyManager;\n        }','/* stale manager */\n        }','enemy_reload','enemy_reload_count')
    add('pair_timer','gui->spellSeconds >= 0','gui->spellSeconds > 0','final_pair','pair_timer')
    add('pair_boss_gate','manager->specialEnemySlots[0] != NULL && gui->message == NULL','gui->message == NULL','final_pair','pair_null_boss')
    add('pair_message_gate','gui->message == NULL','gui->message != NULL','final_pair','pair_timer')
    add('pair_omitted','i < 2','i < 0','pair_reload','pair_once_count')
    add('pair_once','i < 2','i < 2 && gui->spellSeconds >= 0','pair_reload','pair_once_count')
    add('pair_second_identity','Draw(&gui->displayVms4[i]);','Draw(&gui->displayVms4[0]);','pair_reload','pair_second_identity')
    changed=owner.replace('int i;', 'int i; const float cachedGauge = gui->bossGaugeDisplayed;').replace('gui->bossGaugeDisplayed','cachedGauge').replace('cachedGauge = cachedGauge','cachedGauge = gui->bossGaugeDisplayed')
    add('cache_gauge',owner,changed,'segment_dynamic_gate','dynamic_gauge_negative')
    add('segment_regate','if (gui->bossGaugeValues[i].value != 0.0f)','if (gui->bossGaugeDisplayed > 0.0f && gui->bossGaugeValues[i].value != 0.0f)','segment_dynamic_gate','dynamic_gauge_count')
    add('pair_cache_manager','for (i = 0; i < 2; ++i)\n                g_AnmRenderManagerView->Draw(&gui->displayVms4[i]);','AnmRenderManagerView *cachedAnm = g_AnmRenderManagerView;\n            for (i = 0; i < 2; ++i)\n                cachedAnm->Draw(&gui->displayVms4[i]);','pair_reload','pair_second_identity')
    add('return_value','return 1;','return 0;','unconditional','return')
    return result

def main():
    parser=argparse.ArgumentParser();parser.add_argument('--source-root',type=Path,default=Path(__file__).resolve().parents[1]);parser.add_argument('--mutations',action='store_true');parser.add_argument('--report',type=Path);parser.add_argument('--asan-no-leaks',action='store_true',help='Also run ASan with detect_leaks=0 for ptrace environments; does not test leaks')
    args=parser.parse_args();template,owner=load(args.source_root)
    generated=template.replace('@OWNER@',owner)
    require('@' not in generated,'Unexpanded template')
    report={'qualification':'Actual-source host contract; pointer-neutral mocks; no native ABI/GPU/x87 equivalence claim','cases':CASES,'coefficient_bits':'0x3e95a95b','owner_sha256':hashlib.sha256(owner.encode()).hexdigest(),'positive':[], 'mutants':[]}
    with tempfile.TemporaryDirectory(prefix='gui-draw-contract-') as temporary:
        directory=Path(temporary)
        for name,flags in [('O0',['-O0']),('O2',['-O2']),('ubsan',['-O1','-fsanitize=undefined','-fno-sanitize-recover=all'])]:
            compile_run(generated,directory,name,flags,CASES);report['positive'].append(name)
        if args.asan_no_leaks:
            previous=os.environ.get('ASAN_OPTIONS')
            os.environ['ASAN_OPTIONS']='detect_leaks=0'
            try:
                compile_run(generated,directory,'asan_no_leaks',['-O1','-fsanitize=address','-fno-omit-frame-pointer','-fno-pie','-no-pie'],CASES)
                report['positive'].append('asan_no_leaks')
            finally:
                if previous is None: os.environ.pop('ASAN_OPTIONS',None)
                else: os.environ['ASAN_OPTIONS']=previous
        if args.mutations:
            plans=mutations(owner);require(plans,'Mutation suite is empty')
            for name,old,new,case,label in plans:
                changed=replace_once(owner,old,new); mutant=template.replace('@OWNER@',changed)
                locations=checks(mutant); require(label in locations,'Unknown expected CHECK '+label)
                # Ask compiler preprocessing for exact #expression spelling, preserving lexical contract.
                cpp=directory/'expression.cpp'; cpp.write_text(mutant)
                pp=subprocess.run(['g++','-std=c++17','-E','-P',str(cpp)],capture_output=True,text=True,timeout=60)
                require(pp.returncode==0,'Preprocess failed')
                match=re.findall(r'"CHECK\|%s\|%d\|%s\\n",\s*"'+re.escape(label)+r'",\s*\d+,\s*("(?:\\.|[^"\\])*")',pp.stdout)
                require(len(match)==1,'Unique preprocessed CHECK required: '+label)
                expression=json.loads(match[0])
                compile_run(mutant,directory,'mutant_'+name,['-O2'],[case],(label,locations[label],expression))
                report['mutants'].append({'name':name,'case':case,'check':label,'expression':expression})
    if args.report: args.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))
if __name__=='__main__': main()
