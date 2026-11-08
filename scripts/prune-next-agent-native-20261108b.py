#!/usr/bin/env python3
"""One-time audited pruning of superseded TH10 native trial binaries.

Dry run by default. --apply removes only .obj/.exe/.pdb below the exact
2026-11-08 build/gpt-web root snapshot. It preserves all analysis drivers,
source snapshots, JSON, MAP, logs, canonical graphs, and three selected
reproducible enemy witness graphs. No historical archive is modified.
"""
from __future__ import annotations
import argparse, hashlib, json, subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD = ROOT / 'build/gpt-web'
REPORT = ROOT / '.analysis/maintenance-next-agent-20261108b'
EXTENSIONS = frozenset(('.obj', '.exe', '.pdb'))
PRESERVE_ROOTS = ('enemy-runtime-full-ltcg-callers', '20261108-enemy-dispatch-rank-tail-final', '20261108-enemy-runtime-aggregate-sum')
PRUNE_ROOTS = ('20261104-anm-child91', '20261104-current-anm91', '20261104-current-chain-typed-callback', '20261104-current-eclstack', '20261104-current-enemy-laser', '20261104-current-popup', '20261104-live-enemy', '20261104-live-itemdraw-x87', '20261104-next-sound-preload', '20261104-next-text-alpha-profile', '20261105-anm-child-requests', '20261105-anm-flags-halfword', '20261105-anm-label-bytes', '20261105-anm-pointer-mask', '20261105-boss-second', '20261105-bullet-argument', '20261105-bullet-final-reg', '20261105-continuation-bullet', '20261105-continuation-ecl-run', '20261105-continuation-sound', '20261105-ecl-call-descriptor', '20261105-ecl-call-stack-slot', '20261105-ecl-copy-barrier', '20261105-ecl-format-scalar', '20261105-ecl-format-stack', '20261105-ecl-jump-thread', '20261105-ecl-mask', '20261105-ecl-pop-scalar', '20261105-ecl-stack-scalar', '20261105-ecl-thread-flag-bit', '20261105-enemy-drop-word', '20261105-enemy-pattern-helper', '20261105-enemy-shared-header-pattern', '20261105-enemy-time-lifecycle', '20261105-followup-anm', '20261105-handoff-current', '20261105-next-addscript', '20261105-next-anm-child91', '20261105-next-anm-jump', '20261105-next-anm-render-mode', '20261105-next-boss', '20261105-next-callback-life', '20261105-next-ecl-square', '20261105-next-enemy-callback-inline', '20261105-next-enemy-spell', '20261105-next-midi-stop', '20261105-next-seek', '20261105-next-sound-fmt', '20261105-score-groups', '20261105-seek-merge-tail', '20261105-seek-new-register', '20261105-seek-permutation', '20261105-seek-shared-current-end', '20261105-sound-index-lifetime', '20261105-sound-riff-views', '20261105-sound-sample-5tu', '20261105-text', '20261105-th10-conditional-jump', '20261105-th10-enemy-spawn-payload', '20261105-transform-payload', '20261105-turn-ecl-addscript', '20261105-turn-ecl-operands', '20261105-turn-seek-tail', '20261105-turn-sound', '20261105-vector-accel', '20261106-anm-child-layout', '20261106-anm-dword-vector', '20261106-anm-generated-vertex-pairs', '20261106-anm-int-set-wrap', '20261106-anm-op91-order', '20261106-anm-scalar-operand', '20261106-anm-set-script-register', '20261106-anm-timed-vector3-target', '20261106-beginspell-bonus-wrap', '20261106-bullet-angle-reference', '20261106-bullet-vector-accel', '20261106-bullet-vertical-wrap', '20261106-capture-barrier', '20261106-capture-store-order', '20261106-chat-th10-anm-word', '20261106-chat-th10-anm91', '20261106-chat-th10-anmidx', '20261106-chat-th10-bullet', '20261106-chat-th10-bullet-abs', '20261106-chat-th10-bullet-wrap', '20261106-chat-th10-capture', '20261106-chat-th10-count-more', '20261106-chat-th10-ecl', '20261106-chat-th10-ecl-math2', '20261106-chat-th10-enemy', '20261106-chat-th10-enemy-rank', '20261106-chat-th10-enemy2', '20261106-chat-th10-finalize', '20261106-chat-th10-finalize-exact', '20261106-chat-th10-finalize-safe', '20261106-chat-th10-finalize-tail', '20261106-chat-th10-item', '20261106-chat-th10-items', '20261106-chat-th10-loop', '20261106-chat-th10-main', '20261106-chat-th10-main-proper', '20261106-chat-th10-popup-factory', '20261106-chat-th10-score', '20261106-chat-th10-seek', '20261106-chat-th10-seek-order', '20261106-chat-th10-seek-word', '20261106-chat-th10-setsub-real', '20261106-chat-th10-sound', '20261106-chat-th10-stack-word', '20261106-chat-th10-table', '20261106-chat-th10-text-exact', '20261106-ecl-addscript-exact', '20261106-ecl-arithmetic-scalars', '20261106-ecl-call-inline', '20261106-ecl-comparison-int-only', '20261106-ecl-comparison-scalars', '20261106-ecl-count-reference', '20261106-ecl-logic-word', '20261106-ecl-negation-increment', '20261106-ecl-readint-floatbit', '20261106-ecl-stop-thread-register', '20261106-ecl-store-word', '20261106-ecl-time-reg-lifetime', '20261106-ending-ring', '20261106-enemy-bullet-spawn-alias', '20261106-enemy-circle-scalar-bits', '20261106-enemy-constructor-sentinel', '20261106-enemy-pattern-default', '20261106-enemy-pattern-zero-word-array', '20261106-enemy-raw-float3', '20261106-enemy-spawn-scalars', '20261106-enemy-vm-position-bits', '20261106-handoff-anm-wrap', '20261106-input-frame', '20261106-laser-boundary', '20261106-laser-factory', '20261106-laser-type1-box', '20261106-laser-type1-circle', '20261106-laser-type1-draw', '20261106-laser-type1-init', '20261106-main-register', '20261106-popup-factory', '20261106-preload-param-record', '20261106-preload-readfile-wrapper', '20261106-replay-initialize', '20261106-replay-save-scope', '20261106-sound-inline-call', '20261106-stage-camera-mode', '20261106-startup-version', '20261106-statistics-record', '20261106-text-alpha-bitfield', '20261106-text-alpha-coff', '20261106-text-alpha-owner', '20261106-text-current-exact', '20261106-text-entry-exact', '20261106-text-render-current', '20261106-text-sign-intshift', '20261106-upstream-review', '20261107-active-anm-graph', '20261107-active-anm91-lifetimes', '20261107-active-bgm-indices', '20261107-active-bullet-absolute-register', '20261107-active-ecl-advance', '20261107-active-ecl-call-counter', '20261107-active-ecl-counter', '20261107-active-ecl-pop-address', '20261107-active-ecl-sort-move', '20261107-active-ecl-stack', '20261107-active-ecl-stop', '20261107-active-enemy-callback', '20261107-active-enemy-fresh11', '20261107-active-enemy-graph', '20261107-active-enemy-header-timers', '20261107-active-load-format-life', '20261107-active-preload-slots', '20261107-active-preload-table', '20261107-active-seek', '20261107-active-seek-base-merge', '20261107-active-seek-merge', '20261107-active-shot-option', '20261107-active-sound-command-shift', '20261107-active-sound-sample-native', '20261107-active-sound-sample-words', '20261107-active-sound-stage', '20261107-active-sound-steps', '20261107-active-stats-typed', '20261107-active-text-alpha-cleanup', '20261107-active-text-bleed', '20261107-active-text-bleed-template', '20261107-active-text-bytewords', '20261107-active-text-format-words', '20261107-active-text-invert', '20261107-active-text-invert-words', '20261107-active-text-pixel-cleanup', '20261107-active-transition-lifetime', '20261107-anm-child91', '20261107-anm-init-receiver', '20261107-anm-op91-fresh', '20261107-anm-op91-v2', '20261107-anm-render9-index-word', '20261107-anm-stream-word', '20261107-anm-timer-jump-word', '20261107-anm91-retries', '20261107-bullet-angle-caller', '20261107-bullet-final-register', '20261107-bullet-register', '20261107-bullet-spawn-native', '20261107-bullet-timer-wrap', '20261107-chain-alloc', '20261107-chain-ctor', '20261107-current-sound-trials', '20261107-ecl-add-script-word', '20261107-ecl-binarysearch', '20261107-ecl-format-native-address', '20261107-ecl-format-native-index', '20261107-ecl-format-scalar-selector', '20261107-ecl-format-selected-words', '20261107-ecl-format-word-operand-base', '20261107-ecl-format-word-select2', '20261107-ecl-frame-native-words', '20261107-ecl-frame-remaining-word', '20261107-ecl-instruction-pointer-word', '20261107-ecl-length-order', '20261107-ecl-length-struct', '20261107-ecl-operand-index-word', '20261107-ecl-point-angle', '20261107-ecl-pop-sib-native', '20261107-ecl-readint-pop-word', '20261107-ecl-run-return-spawn', '20261107-ecl-stack-push-pop-word', '20261107-ecl-start', '20261107-ecl-start-pointer-address', '20261107-ecl-start-word', '20261107-enemy-current-11tu', '20261107-enemy-flag-shift', '20261107-enemy-laser-native-words', '20261107-enemy-native-combat-words', '20261107-enemy-setsub-caller', '20261107-enemy-single-resource-words', '20261107-fade-in', '20261107-filesystem-archive-words', '20261107-filesystem-crypto-words', '20261107-filesystem-encrypt-words', '20261107-format-stackhomes-new', '20261107-seek', '20261107-seek-address', '20261107-seek-branch-placement', '20261107-seek-cursor-word', '20261107-seek-register-shape', '20261107-seek-remaining-word', '20261107-seek-shared-end', '20261107-session-th10-ecl', '20261107-session-th10-ecl-format', '20261107-session-th10-enemy', '20261107-session-th10-enemy-v2', '20261107-shake-envelope', '20261107-shake-envelope-current', '20261107-shake-envelope-scope', '20261107-shake-linear', '20261107-shake-output-register', '20261107-shake-switch', '20261107-sound-format-words', '20261107-sound-getfmt-dword', '20261107-sound-new-reads', '20261107-sound-preload', '20261107-sound-register-choices', '20261107-stat-ctor-abi', '20261107-stat-default', '20261107-stat-real-caller', '20261107-stat-tail-schedule', '20261107-stat-typed', '20261107-text-main-hybrid', '20261107-text-main-rectblock-graphs', '20261107-text-main-stack-sharing', '20261107-turn2-bullet', '20261107-turn2-ecl-call', '20261107-turn2-ecl-format', '20261107-turn2-seek-tail', '20261107-turn2-sound-sample', '20261107-turn2-wave-cursor', '20261107-turn3-anm-timer', '20261107-turn3-anm91', '20261107-turn3-bullet-table', '20261107-turn3-seek', '20261107-turn4-anm-world', '20261107-turn4-ecl-count', '20261107-turn4-ecl-files', '20261107-turn4-ecl-lookup', '20261107-turn4-enemy', '20261107-turn5-ecl-record', '20261107-turn5-enemy', '20261107-turn5-text-invert', '20261108-anm-boss-mixed-graphs', '20261108-anm-float3-addition-operand-order', '20261108-anm-float3-operator-order', '20261108-anm-mixed-contexts', '20261108-anm-script-switch', '20261108-anm-tail-cleaner', '20261108-anm-tail-native', '20261108-anm91-fastcall', '20261108-anm91-new', '20261108-anm91-register-hints', '20261108-bullet-absolute-aggregate', '20261108-bullet-absolute-angle-view', '20261108-bullet-absolute-asm-call', '20261108-bullet-absolute-asm-fullgraph', '20261108-bullet-absolute-lifetime', '20261108-bullet-absolute-live-range', '20261108-bullet-absolute-register', '20261108-bullet-absolute-speed-register', '20261108-bullet-bucket-words', '20261108-bullet-manager-native-words', '20261108-ecl-find-native-return', '20261108-ecl-format-aggregate-layout', '20261108-ecl-format-aggregate-layout-v2', '20261108-ecl-format-direct-aggregate-fresh', '20261108-ecl-format-local-order', '20261108-ecl-format-prefix-trials', '20261108-ecl-format-struct-local-corrected', '20261108-ecl-format-volatile-slot-order', '20261108-ecl-length-address-taken-home', '20261108-ecl-length-aggregate-x87', '20261108-ecl-length-declaration-order', '20261108-ecl-length-declare-y-first-natural', '20261108-ecl-length-dual-volatile-pointers', '20261108-ecl-length-inline-x87', '20261108-ecl-length-inline-x87-call', '20261108-ecl-length-inline-x87-eax', '20261108-ecl-length-natural-operand-evaluation', '20261108-ecl-length-order', '20261108-ecl-length-ordered-volatile-home', '20261108-ecl-length-resolve-before-products', '20261108-ecl-length-sequenced-cpp', '20261108-ecl-length-volatile-alias-home', '20261108-ecl-length-volatile-pointer-operand-swap', '20261108-ecl-length-x87-eax-order', '20261108-ecl-length-x87-fixed-stack', '20261108-ecl-length-x87-read-order', '20261108-ecl-point-angle-lifetime', '20261108-ecl-pop-native', '20261108-ecl-readint-stack-address-order', '20261108-ecl-run-format-spawn-register', '20261108-ecl-run-fresh-support', '20261108-ecl-run-fresh-support-local-home', '20261108-ecl-run-local-layout', '20261108-ecl-run-return-stack-word', '20261108-ecl-run-stack-order', '20261108-ecl-run-stack-register', '20261108-ecl-run-targeted-local-layout', '20261108-ecl-run-thread', '20261108-ecl-spawn-thread-id-lifetimes', '20261108-ecl-stack-array-address', '20261108-ecl-stack-pop-address-order', '20261108-ecl-stack-pop-format-address', '20261108-ecl-x87-order', '20261108-enemy-destructor-words', '20261108-enemy-dispatch-interpolate-float-rank', '20261108-enemy-dispatch-interpolate-float-rank-order2', '20261108-enemy-dispatch-interpolate-float-rank-volatile-direct', '20261108-enemy-dispatch-local-stack-slots', '20261108-enemy-dispatch-opcode-address-lifetime', '20261108-enemy-dispatch-opcode-register-shapes', '20261108-enemy-dispatch-rank-interpolation-order', '20261108-enemy-dispatch-rank-tail-layout', '20261108-enemy-dispatch-rank2-argument-register', '20261108-enemy-dispatch-register-allocation', '20261108-enemy-dispatch-register-lifetime', '20261108-enemy-dispatch-register-lifetime-v2', '20261108-enemy-dispatch-this-pointer-lifetime', '20261108-enemy-dispatch-typed-instruction-pointer', '20261108-enemy-dispatch-typed-instruction-pointer-fix', '20261108-enemy-dispatch-typed-opcode-lifetimes', '20261108-enemy-dispatch-volatile-opcode-copy', '20261108-enemy-dispatch-volatile-opcode-snapshot', '20261108-enemy-dispatcher-fresh-graph', '20261108-enemy-ecl-index-boss', '20261108-enemy-manager-timer', '20261108-enemy-motion-cleanup-pointer-registers', '20261108-enemy-motion-combined', '20261108-enemy-motion-final-current', '20261108-enemy-motion-floor-double-lifetime', '20261108-enemy-motion-floor-lifetime', '20261108-enemy-motion-floor-order', '20261108-enemy-motion-floor-result-scope', '20261108-enemy-motion-floor-stack-lifetimes', '20261108-enemy-motion-floor-volatile', '20261108-enemy-motion-frontier', '20261108-enemy-motion-helper-tugraph', '20261108-enemy-motion-installed', '20261108-enemy-motion-leaf', '20261108-enemy-motion-materialized-result', '20261108-enemy-motion-member-method', '20261108-enemy-motion-natural-lvalue-order', '20261108-enemy-motion-natural-next', '20261108-enemy-motion-nested-double-lifetimes', '20261108-enemy-motion-noltcg', '20261108-enemy-motion-normal', '20261108-enemy-motion-ob-flags', '20261108-enemy-motion-operand', '20261108-enemy-motion-polar-component-stores', '20261108-enemy-motion-polar-local-lifetime', '20261108-enemy-motion-position-first', '20261108-enemy-motion-safe-polar', '20261108-enemy-motion-safe-volatile-z', '20261108-enemy-motion-scope', '20261108-enemy-motion-y-reference', '20261108-enemy-runtime-alignment-and-motion-position-view-v2', '20261108-enemy-runtime-branch-local-manager-read', '20261108-enemy-runtime-global-manager-volatile', '20261108-enemy-runtime-half-size-lifetime', '20261108-enemy-runtime-half-size-volatile-home', '20261108-enemy-runtime-loop-local-declaration-order', '20261108-enemy-runtime-remaining-address-taken', '20261108-enemy-runtime-remaining-lifetime', '20261108-enemy-runtime-remaining-pointer-alias', '20261108-enemy-runtime-remaining-register-hint', '20261108-enemy-runtime-reversed-vector-constructor', '20261108-enemy-runtime-vector-aggregate-temporaries', '20261108-enemy-runtime-vector-compound', '20261108-enemy-runtime-vector-slot-and-motion-cleanup', '20261108-enemy-runtime-vector-sum-order', '20261108-enemy-runtime-vm-count', '20261108-enemy-runtime-vm-loop', '20261108-enemy-runtime-volatile-manager-read', '20261108-enemy-runtime-volatile-position-operator', '20261108-enemy-runtime-volatile-reversed-vector-sum', '20261108-enemy-runtime-volatile-vector-sum-lifetime', '20261108-enemy-runtime-world-vector-order', '20261108-enemy-runtime-yz-selective-volatility', '20261108-enemy-runtime-yz-x87-order', '20261108-enemy-setsub-name', '20261108-enemy-setsub-order', '20261108-enemy-spawn-native-counts', '20261108-enemy-update-world-vector-volatile-snapshot', '20261108-enemy-update-world-vector-yz-home', '20261108-enemy-update-world-velocity', '20261108-enemy-update-world-velocity-mixed', '20261108-preload-slots', '20261108-seek-begin-reg', '20261108-seek-begin-scoped', '20261108-sound-disk-guard-words', '20261108-sound-open-disk-control', '20261108-sound-preload-readfile-local-layout', '20261108-sound-preload-readfile-register-local', '20261108-sound-preload-readfile-volatile-format-ref', '20261108-text-alternate-stack', '20261108-text-alternate-width', '20261108-text-invert-abi')
EXPECTED_ROOTS = tuple(sorted(PRESERVE_ROOTS + PRUNE_ROOTS))
PROTECTED_FILES = (
    'resources/th10.exe',
    'build/match-linked/BulletHorizontalWrap/source.exe',
    'build/match-linked/EclVmHostRunEnemy/source.exe',
    'build/match-linked/SoundDataLoaderThread/source.exe',
    'build/match-linked/GameStatisticsTail/source.exe',
    'build/gpt-web/enemy-runtime-full-ltcg-callers/source.exe',
    'build/gpt-web/20261108-enemy-runtime-aggregate-sum/source.exe',
    'build/gpt-web/20261108-enemy-dispatch-rank-tail-final/source.exe',
)
PROTECTED_DIRS = (
    'build/match-linked', '.analysis/gpt-web', '.analysis/archive',
    '.tools', '_reference', 'ghidra-project',
)
PRESERVE_FILES = tuple(
    str(p.relative_to(ROOT))
    for family in PRESERVE_ROOTS
    for p in sorted((BUILD / family).rglob('*'))
    if p.is_file()
)

def sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()

def usage(path: Path) -> int:
    return int(subprocess.check_output(['du', '-s', '-B1', str(path)], text=True).split()[0])

def discover():
    if ROOT.name != 'th10' or not BUILD.is_dir() or BUILD.is_symlink():
        raise RuntimeError('refusing noncanonical or symlinked TH10 build path')
    if subprocess.check_output(['git','ls-files','--','build/gpt-web'], text=True, cwd=ROOT).strip():
        raise RuntimeError('build/gpt-web contains tracked files')
    roots = tuple(sorted(p.name for p in BUILD.iterdir() if p.is_dir()))
    if roots != EXPECTED_ROOTS:
        raise RuntimeError('build/gpt-web root inventory changed; review a new explicit snapshot')
    if any(p.is_symlink() for p in BUILD.iterdir()):
        raise RuntimeError('symlink at build/gpt-web root')
    files, per_root = [], {}
    for name in PRUNE_ROOTS:
        family = BUILD / name
        if not family.is_dir() or family.is_symlink() or family.resolve().parent != BUILD.resolve():
            raise RuntimeError(f'unsafe family root: {family}')
        members = []
        for p in sorted(family.rglob('*')):
            if p.is_symlink():
                raise RuntimeError(f'symlink inside family: {p}')
            if not p.is_file() or p.suffix.lower() not in EXTENSIONS:
                continue
            rel = p.relative_to(family)
            if not p.resolve().is_relative_to(family.resolve()):
                raise RuntimeError(f'path escapes family root: {p}')
            if {'accepted-exact','canonical-cold','installed-cold'} & set(rel.parts):
                continue
            members.append(p)
        per_root[name] = {'files':len(members), 'payload_bytes':sum(p.stat().st_size for p in members)}
        files.extend(members)
    if len(files) != len(set(files)):
        raise RuntimeError('duplicate native paths')
    return files, per_root

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--apply',action='store_true')
    args=ap.parse_args()
    files, per_root = discover()
    payload=sum(p.stat().st_size for p in files)
    allocated=sum(p.stat().st_blocks*512 for p in files)
    print(json.dumps({'mode':'apply' if args.apply else 'dry-run',
        'reviewed_roots':len(PRUNE_ROOTS),'preserved_roots':list(PRESERVE_ROOTS),
        'native_files':len(files),'payload_bytes':payload,
        'allocated_payload_bytes':allocated,
        'largest_roots':sorted(per_root.items(),key=lambda x:-x[1]['payload_bytes'])[:15]},indent=2))
    if not args.apply: return
    if not files or REPORT.exists():
        raise RuntimeError('one-time cleanup already applied or nothing eligible')
    if subprocess.check_output(['git','status','--porcelain'],text=True,cwd=ROOT).strip():
        raise RuntimeError('commit tracked work and start from a clean tree')
    before=usage(BUILD)
    protected={p:sha256(ROOT/p) for p in PROTECTED_FILES}
    protected_sizes={p:usage(ROOT/p) for p in PROTECTED_DIRS}
    preserve_hashes={p:sha256(ROOT/p) for p in PRESERVE_FILES}
    for p in PROTECTED_FILES + PRESERVE_FILES:
        if not (ROOT/p).is_file(): raise RuntimeError(f'missing protected file: {p}')
    REPORT.mkdir(parents=True,exist_ok=False)
    manifest=REPORT/'removed-native.jsonl'
    with manifest.open('w',encoding='utf-8') as out:
        for p in files:
            st=p.stat()
            out.write(json.dumps({'path':str(p.relative_to(ROOT)),
                'bytes':st.st_size,'allocated_bytes':st.st_blocks*512,
                'sha256':sha256(p)},sort_keys=True)+'\n')
    for p in files: p.unlink()
    after=usage(BUILD)
    if after >= before: raise RuntimeError('allocated build usage did not decrease')
    for p,h in protected.items():
        if sha256(ROOT/p)!=h: raise RuntimeError(f'protected file changed: {p}')
    for p,n in protected_sizes.items():
        if usage(ROOT/p)!=n: raise RuntimeError(f'protected directory changed: {p}')
    for p,h in preserve_hashes.items():
        if sha256(ROOT/p)!=h: raise RuntimeError(f'preserved witness changed: {p}')
    result={'date':'2026-11-08','scope':'all regenerable native experiment outputs in the fixed 2026-11-08 build/gpt-web inventory, excluding three selected witness graphs',
        'reviewed_roots':len(PRUNE_ROOTS),'preserved_roots':list(PRESERVE_ROOTS),
        'families':per_root,'deleted_files':len(files),'payload_bytes':payload,
        'allocated_payload_bytes':allocated,'allocated_before':before,
        'allocated_after':after,'allocated_reclaimed':before-after,
        'manifest':str(manifest.relative_to(ROOT)),'manifest_sha256':sha256(manifest),
        'protected_sha256':protected,'protected_allocated_bytes':protected_sizes,
        'preserved_witness_files':len(preserve_hashes),
        'preserved_witness_sha256':preserve_hashes,
        'analysis_drivers_snapshots_json_maps_logs':'preserved',
        'archive_created':False,
        'warning':'The SHA manifest proves deletion identity; it is not a backup. Rebuild from retained drivers/source snapshots if needed.'}
    (REPORT/'cleanup-result.json').write_text(json.dumps(result,indent=2)+'\n')
    print('CLEANUP_OK',json.dumps({k:result[k] for k in ('deleted_files','payload_bytes','allocated_before','allocated_after','allocated_reclaimed','manifest_sha256')},indent=2))

if __name__=='__main__': main()
