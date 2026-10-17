#!/usr/bin/env python3
"""Strict LOCAL-ONLY mutation tests of actual proposal bodies.

Each edit requires exactly one function and one anchor inside that function.
Compile failure, wrong assertion, arbitrary crash, timeout and survival are NOT
kills. Only successful compilation followed by exit86 + exact expected ASSERT
is counted. Outputs retain full compilation/run evidence and hashes.
"""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import argparse, hashlib, json, shutil, subprocess, sys
from transition_effect_host_test import ROOT, DEFAULT_CANON, build, block, once, sha, read

# name, unique function marker, unique scoped anchor, replacement, scenario,
# exact required failure ID. This is a negative-control catalog, not reference
# implementations and not proposed production fixes.
MUTATIONS=[
 ('selector_kind5_update','void TransitionEffectView::Initialize(',
  'case 5:\n        updateNode = Chain::RegisterCalc(TransitionFadeIn, 14, this);',
  'case 5:\n        updateNode = Chain::RegisterCalc(TransitionFadeOut, 14, this);','selector','SELECTOR_UPDATE_9'),
 ('selector_kind7_draw','void TransitionEffectView::Initialize(',
  'drawNode = Chain::RegisterDraw(TransitionDrawDimPlayfield, drawPriority, this);',
  'drawNode = Chain::RegisterDraw(TransitionDrawPlayfield, drawPriority, this);','selector','SELECTOR_DRAW_9'),
 ('selector_kind3_alpha','void TransitionEffectView::Initialize(',
  'alpha = 255;','alpha = 0;','selector','SELECTOR_KIND3_ALPHA'),
 ('init_flags','void *StageCreateTransitionEffect(',
  'effect->flags |= 2u;','effect->flags |= 4u;','selector','FACTORY_ZERO_FLAGS_UNKNOWN'),
 ('init_payload_early','void TransitionEffectView::Initialize(',
  'using th10::Chain;','using th10::Chain; frames = duration;','selector','INIT_PAYLOAD_AFTER_REGISTRATION'),
 ('init_timer_early','void TransitionEffectView::Initialize(',
  'using th10::Chain;','using th10::Chain; timer.SetCurrent(0);','selector','INIT_TIMER_AFTER_REGISTRATION'),
 ('init_delete_early','void TransitionEffectView::Initialize(',
  'updateNode = Chain::RegisterCalc(TransitionFadeOut, 14, this);\n        drawNode = Chain::RegisterDraw(TransitionDrawScreen, drawPriority, this);',
  'updateNode = Chain::RegisterCalc(TransitionFadeOut, 14, this);\n        updateNode->deletedCallback = TransitionDelete;\n        drawNode = Chain::RegisterDraw(TransitionDrawScreen, drawPriority, this);','selector','INIT_DELETE_AFTER_BOTH_REGISTRATIONS'),
 ('init_final_field_order','void TransitionEffectView::Initialize(',
  'frames = duration;\n    kind = effectKind;',
  'kind = effectKind;\n    frames = duration;','source_order','INIT_FINAL_FRAMES_BEFORE_KIND'),
 ('fade_out_boundary','static int __fastcall TransitionFadeOut(',
  'effect->timer.current >= effect->frames','effect->timer.current > effect->frames','fades','FADE_OUT_FRAME_BOUNDARY'),
 ('fade_out_alpha','static int __fastcall TransitionFadeOut(',
  'effect->timer.subframe * 255.0f / effect->frames','effect->timer.subframe * 128.0f / effect->frames','fades','FADE_OUT_FRACTIONAL_TRUNC'),
 ('fade_out_abort','static int __fastcall TransitionFadeOut(',
  'if (g_GameStartupCounter != 0)','if (false)','fades','FADE_OUT_ABORT_BEFORE_WRITES'),
 ('fade_in_extra_frames','static int __fastcall TransitionFadeIn(',
  'static_cast<unsigned int>(effect->frames) + 2u','static_cast<unsigned int>(effect->frames) + 0u','fades','FADE_IN_TWO_EXTRA_FIRST'),
 ('fade_in_pause','static int __fastcall TransitionFadeIn(',
  '(game->flags58 & 5u) == 0','(game->flags58 & 1u) == 0','fades','FADE_IN_PAUSE_MASK5'),
 ('fade_in_null_game','static int __fastcall TransitionFadeIn(',
  'game == NULL || (game->flags58 & 5u) == 0','game != NULL && (game->flags58 & 5u) == 0','fades','FADE_IN_NULL_GAME_TICKS'),
 ('dim_release_boundary','static int __fastcall TransitionDim(',
  'effect->timer.current > 8','effect->timer.current >= 8','dim','DIM_RELEASE_FRAME8_ACTIVE'),
 ('dim_unreleased_boundary','static int __fastcall TransitionDim(',
  'effect->timer.current <= effect->frames','effect->timer.current < effect->frames','dim','DIM_INCLUSIVE_END'),
 ('release_reset','void TransitionEffectView::Release(',
  'timer.SetCurrent(0);','timer.SetCurrent(1);','dim','DIM_RELEASE_TIMER_RESET'),
 ('pulse_return7','static int __fastcall TransitionPulse(',
  'if (static_cast<int>(effect->value0) <= 0)\n            return 0;',
  'if (static_cast<int>(effect->value0) <= 0)\n            return 7;','pulse','PULSE_FINISH_RETURNS0'),
 ('pulse_unsigned_count','static int __fastcall TransitionPulse(',
  'static_cast<int>(effect->value0) <= 0','effect->value0 == 0u','pulse','PULSE_COUNT_UNDERFLOW_SIGNED'),
 ('pulse_abort_return7','static int __fastcall TransitionPulse(',
  'if (abort != 0)\n        return 0;','if (abort != 0)\n        return 7;','pulse','PULSE_ABORT_RETURNS0_UNCHANGED'),
 ('pulse_rgb_mask','static int __fastcall TransitionDrawPulse(',
  '(effect->value1 & 0x00ffffffu)','effect->value1','draw','DRAW_COLOR_OR_VERSUS_MASK'),
 ('screen_viewport_width','static int __fastcall TransitionDrawScreen(',
  'g_TransitionViewport.width = 640;','g_TransitionViewport.width = 639;','draw','DRAW_SCREEN_VIEWPORT_RESET'),
 ('screen_device_cached_early','static int __fastcall TransitionDrawScreen(',
  'manager->FlushVertexBuffer();','D3d9DeviceView *savedDevice = g_Direct3DDevice; manager->FlushVertexBuffer();',
  'draw','DRAW_SCREEN_GLOBAL_RELOAD'),
 ('screen_missing_flush','static int __fastcall TransitionDrawScreen(',
  'manager->FlushVertexBuffer();','(void)manager;','draw','DRAW_SCREEN_EVENT_ORDER'),
 ('dim_screen_extra_flush','static int __fastcall TransitionDrawDimScreen(',
  'AnmFloatRectView rectangle =','g_AnmRenderManagerView->FlushVertexBuffer();\n    AnmFloatRectView rectangle =','draw','DRAW_NONSCREEN_NO_RESET_OR_FLUSH'),
 ('linear_standard_rng_x','static int __fastcall TransitionShakeLinear(',
  'g_RngView.GetRandomU16();\n    unsigned int half = g_RngView.GetRandomU16();\n    unsigned int choice = ((half << 16) | half) % 3u;',
  'unsigned int upper = g_RngView.GetRandomU16();\n    unsigned int half = g_RngView.GetRandomU16();\n    unsigned int choice = ((upper << 16) | half) % 3u;','linear','LINEAR_DUPLICATED_SECOND_HALVES'),
 ('linear_standard_rng_y','static int __fastcall TransitionShakeLinear(',
  'g_RngView.GetRandomU16();\n    half = g_RngView.GetRandomU16();\n    choice = ((half << 16) | half) % 3u;',
  'unsigned int upperY = g_RngView.GetRandomU16();\n    half = g_RngView.GetRandomU16();\n    choice = ((upperY << 16) | half) % 3u;','linear','LINEAR_SECOND_AXIS_SEED11'),
 ('linear_tick_late','static int __fastcall TransitionShakeLinear(',
  'effect->timer.Tick();','/* suppressed tick */','linear','LINEAR_TICK_BEFORE_ENVELOPE'),
 ('linear_clear_terminal','static int __fastcall TransitionShakeLinear(',
  'if (effect->timer.current >= effect->frames)\n        return 7;',
  'if (effect->timer.current >= effect->frames) { g_TransitionShakeX = g_TransitionShakeY = 0; return 7; }','linear','LINEAR_TERMINAL_NO_RNG_NO_ZERO'),
 ('envelope_pause_mask','static int __fastcall TransitionShakeEnvelope(',
  '(game->flags58 & 0x77u) != 0','(game->flags58 & 5u) != 0','envelope','ENVELOPE_PAUSE_MASK77'),
 ('envelope_null_game_tick','static int __fastcall TransitionShakeEnvelope(',
  'game == NULL || (game->flags58 & 0x77u) != 0','game != NULL && (game->flags58 & 0x77u) != 0','envelope','ENVELOPE_NULL_GAME_PAUSES'),
 ('envelope_signed_attack','static int __fastcall TransitionShakeEnvelope(',
  'effect->timer.current < static_cast<int>(effect->value0)','static_cast<unsigned int>(effect->timer.current) < effect->value0','envelope','ENVELOPE_SIGNED_ATTACK_COMPARE'),
 ('envelope_signed_divisor','static int __fastcall TransitionShakeEnvelope(',
  '/ static_cast<float>(effect->value2);','/ static_cast<float>(static_cast<int>(effect->value2));','envelope','ENVELOPE_UNSIGNED_DECAY_DIVISOR'),
 ('envelope_signed_total','static int __fastcall TransitionShakeEnvelope(',
  'static_cast<float>(effect->value0 + effect->value1 + effect->value2)',
  'static_cast<float>(static_cast<int>(effect->value0 + effect->value1 + effect->value2))','envelope','ENVELOPE_UNSIGNED_DECAY_TOTAL'),
 ('envelope_attack_rounding_fix','static int __fastcall TransitionShakeEnvelope(',
  'float envelope;','long double envelope;','precision_gap','KNOWN_X87_PRECISION_GAP_RETAINED'),
 ('cleanup_no_chain_reload','void TransitionEffectView::Cleanup(',
  'node = drawNode;\n    chain = th10::g_Chain;','node = drawNode;','cleanup','CLEANUP_RELOADS_GLOBAL_CHAIN'),
 ('cleanup_no_draw_reload','void TransitionEffectView::Cleanup(',
  'th10::ChainElem *node = updateNode;','th10::ChainElem *savedDraw = drawNode; th10::ChainElem *node = updateNode;',
  'cleanup','CLEANUP_RELOADS_DRAW_POINTER'),
 ('cleanup_chain_loaded_in_lock','void TransitionEffectView::Cleanup(',
  '++th10::g_ChainNestCounter;\n        th10::Chain::UnregisterElem(node, chain);',
  '++th10::g_ChainNestCounter;\n        chain = th10::g_Chain;\n        th10::Chain::UnregisterElem(node, chain);',
  'cleanup','CLEANUP_CAPTURES_CHAIN_BEFORE_ENTER'),
 ('cleanup_clears_pointers','void TransitionEffectView::Cleanup(',
  'node = drawNode;','updateNode = NULL; node = drawNode;','cleanup','CLEANUP_DOES_NOT_CLEAR_POINTERS'),
 ('delete_skips_cleanup','void DestroyTransitionEffect(',
  'effect->Cleanup();','/* cleanup omitted */','lifetime7','RET7_TWO_RECURSIVE_CLEANUP_LOCKS'),
]

# Secondary edit for deliberately cached locals; still one exact full-function
# replacement and each individual anchor is required to occur exactly once.
SECONDARY={
 'screen_device_cached_early':('D3d9DeviceView *device = g_Direct3DDevice;','D3d9DeviceView *device = savedDevice;'),
 'cleanup_no_draw_reload':('node = drawNode;','node = savedDraw;'),
 'envelope_attack_rounding_fix':('effect->timer.subframe / static_cast<int>(effect->value0)','static_cast<long double>(effect->timer.subframe) / static_cast<int>(effect->value0)'),
 # The repeated lock sequence is narrowed with its preceding first if block.
}

def classify(compile_ok,rc,stdout,stderr,expected):
    if not compile_ok:return 'COMPILE_ERROR_NOT_A_KILL'
    if rc==86 and stdout=='' and stderr=='ASSERT:'+expected+'\n':return 'EXPECTED_ASSERTION'
    if rc==0:return 'SURVIVED'
    return 'WRONG_FAILURE_NOT_A_KILL'

def self_checks():
    assert classify(True,86,'','ASSERT:X\n','X')=='EXPECTED_ASSERTION'
    assert classify(False,1,'','ASSERT:X\n','X')=='COMPILE_ERROR_NOT_A_KILL'
    for rc,stdout,stderr in [
        (86,'','ASSERT:Y\n'), (86,'polluted stdout\n','ASSERT:X\n'),
        (86,'',' ASSERT:X\n'), (86,'','ASSERT:X\nnoise'),
        (86,'','ASSERT:X\n\n'), (86,'','ASSERT:X\nASSERT:X\n'),
        (86,'','ASSERT:X'), (-11,'',''), (-9,'',''), (None,'','')]:
        assert classify(True,rc,stdout,stderr,'X')=='WRONG_FAILURE_NOT_A_KILL'
    assert classify(True,0,'','','X')=='SURVIVED'
    for text,old in [('abc','z'),('aaa','a')]:
        try:once(text,old,'x')
        except RuntimeError:pass
        else:raise AssertionError('non-exact edit accepted')
    return ['exact stdout/stderr/exit accepted','compile failures rejected',
        'wrong assertions rejected','polluted stdout rejected',
        'leading/trailing stderr noise rejected','extra blank/multiple lines rejected',
        'missing newline rejected','signals/timeouts rejected','survivors rejected',
        'zero/multiple anchors rejected']

def one_mutation(m,source,out,canonical):
    name,marker,old,new,scenario,expected=m
    original=read(source);original_block=block(original,marker)
    # This one deliberate repeated sequence is addressed by its complete
    # prefix so the selected first cleanup cycle has an exact-one anchor.
    if name=='cleanup_chain_loaded_in_lock':
        old='if (node != NULL)\n    {\n        EnterCriticalSection(&th10::g_CriticalSections[0]);\n        ++th10::g_ChainNestCounter;\n        th10::Chain::UnregisterElem(node, chain);\n        LeaveCriticalSection(&th10::g_CriticalSections[0]);\n        --th10::g_ChainNestCounter;\n    }\n    node = drawNode;'
        new=old.replace('th10::Chain::UnregisterElem(node, chain);','chain = th10::g_Chain;\n        th10::Chain::UnregisterElem(node, chain);',1)
    changed=once(original_block,old,new)
    if name in SECONDARY:changed=once(changed,*SECONDARY[name])
    mutated=once(original,original_block,changed)
    if mutated==original:raise RuntimeError('unchanged mutation: '+name)
    folder=out/name;folder.mkdir(parents=True,exist_ok=True)
    mutant=folder/'TransitionEffect.cpp';mutant.write_text(mutated);shutil.copy2(source.with_suffix('.hpp'),mutant.with_suffix('.hpp'))
    category='source_order_control' if name=='init_final_field_order' else ('known_gap_control' if name=='envelope_attack_rounding_fix' else 'runtime_mutation')
    result={'category':category,'name':name,'scenario':scenario,'expected_assertion':expected,'function_occurrences':original.count(marker),'anchor_occurrences':original_block.count(old),'secondary_anchor_occurrences':(original_block.count(SECONDARY[name][0]) if name in SECONDARY else None),'function_before_sha256':hashlib.sha256(original_block.encode()).hexdigest(),'source_sha256':sha(mutant)}
    try:
        exe=build(mutant,folder/'build',canonical)
    except RuntimeError as exc:
        result.update(status='COMPILE_ERROR_NOT_A_KILL',error=str(exc));return result
    r=subprocess.run([str(exe),scenario],text=True,capture_output=True,timeout=15)
    (folder/'run.stdout').write_text(r.stdout);(folder/'run.stderr').write_text(r.stderr)
    result.update(status=classify(True,r.returncode,r.stdout,r.stderr,expected),compile_returncode=0,run_returncode=r.returncode,stdout=r.stdout,stderr=r.stderr)
    return result

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'src/TransitionEffect.cpp');ap.add_argument('--canonical',type=Path,default=DEFAULT_CANON);ap.add_argument('--out',type=Path,default=ROOT/'.analysis/transition-effect-host/mutations');ap.add_argument('--jobs',type=int,default=1);a=ap.parse_args();a.out.mkdir(parents=True,exist_ok=True)
    guards=self_checks();exe=build(a.source,a.out/'baseline',a.canonical);base=subprocess.run([str(exe),'all'],text=True,capture_output=True,timeout=15)
    (a.out/'baseline/run.stdout').write_text(base.stdout);(a.out/'baseline/run.stderr').write_text(base.stderr)
    if base.returncode!=0:raise RuntimeError('BASELINE MUST PASS BEFORE MUTATIONS: '+base.stderr)
    with ThreadPoolExecutor(max_workers=a.jobs) as pool:results=list(pool.map(lambda m:one_mutation(m,a.source,a.out,a.canonical),MUTATIONS))
    report={'source_sha256':sha(a.source),'header_sha256':sha(a.source.with_suffix('.hpp')),'fixture_sha256':sha(ROOT/'scripts/fixtures/transition-effect.cpp'),'runner_sha256':sha(Path(__file__)),'strict_guard_self_checks':guards,'baseline_returncode':base.returncode,'total_runs':len(results),'strict_expected_assertions':sum(r['status']=='EXPECTED_ASSERTION' for r in results),'groups':{category:{'total':sum(r['category']==category for r in results),'expected_assertions':sum(r['category']==category and r['status']=='EXPECTED_ASSERTION' for r in results)} for category in ['runtime_mutation','source_order_control','known_gap_control']},'results':results}
    (a.out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    for r in results:print(r['name']+': '+r['status']+' '+r.get('stderr',r.get('error','')).strip())
    for category,counts in report['groups'].items():print(f"{category.upper()} {counts['expected_assertions']}/{counts['total']}")
    return 0 if report['strict_expected_assertions']==report['total_runs'] else 1
if __name__=='__main__':sys.exit(main())
