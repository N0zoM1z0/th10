# TH10 reconstruction handoff — 2026-11-02

This is the **current resume brief**, not a running log of every experiment.
Use [PROGRESS.md](PROGRESS.md) for exact counts, the function and origin
ledgers for individual claims, [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md)
for durable target facts, and [RE_ARTIFACTS.md](RE_ARTIFACTS.md) for the
current private working set and artifact cleanup. Prior dated narratives
and historical numbers are retained in Git and the
[2026-10-30 history](RE_HANDOFF_HISTORY_20261030.md). They are
not current acceptance receipts.

## Target, rules and validation

- Target: **original Japanese TH10 v1.00a** at ignored
  resources/th10.exe, 487,936 bytes, SHA-256
  2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
- Branch: main, upstream origin/main. The latest source checkpoint in
  this handoff is e0ad99a; use git status/log to see subsequent docs
  or local commits. **Never push automatically.**
- Local commit format: **gpt-web: xxx**, author
  N0zoM1z0 <1705004398@qq.com>, with the existing gensokyo-dev
  co-author trailer. Take multiple useful checkpoints per working session.
- **Do not discard dirty work**; inspect and finish or document it. Work on
  both large core functions and useful leaves. Do not trust old claims
  without independent source/target comparison.
- Cold replay **only new/affected --unit selectors**; no full historical
  cold replay. A matched size, masked bytes, successful compile or local
  commit does not equal codegen exactness.
- Run the local repository Python wrapper. Original source/TU,
  game product, runtime semantics and portability remain unproven.
  Ghidra/IDA names are provisional until confirmed.

    git status --short --branch
    git log -6 --oneline
    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --check
    scripts/repo-python scripts/ci.py

Default CI has seven lightweight checks. Only a complete target-bound,
PDB/COFF-owned and cold-reproducible zero-difference result with *every*
external address and required local destination bound can enter
config/matches.csv. Generate ledger statistics with:

    scripts/repo-python scripts/progress.py
    scripts/repo-python scripts/report-reconstruction-status.py

## Current accepted snapshot

| Measure | 2026-11-02 accepted state |
| --- | ---: |
| Tracked target extents / reviewed boundaries | 1,823 / 1,823 |
| Source-present target mappings | 1,250 |
| Canonical exact owners / bytes | **1,445 / 185,573** |
| Reviewed authored exact owners / bytes | **553 / 116,830** |
| Reviewed authored byte denominator | 276,024 (provisional) |
| Authored source-present nonexact backlog | 167 |
| Classified excluded / origin-indeterminate owners | 520 / 555 |

These values are generated from current ledgers. The denominator is not
an established total for all original game-authored code.

### Recent substantive complete-owner proofs

| Area | Accepted full-owner result | Reproduce / evidence |
| --- | --- | --- |
| **DirectInput axis EnumObjects callback @0x0043BB30** | **98/98 full-owner authored exact**, one strictly bound `g_InputPadDevices` global; two independent VC7.1 colds and existing input-controller guard **264/264 exact**. Original 439-byte input initializer remains separate. | [Complete callback proof](evidence/input-axis-range-callback-exact-20261102/README.md) |
| **Laser pattern cancellation @0x0041C800 and Bomb auxiliary @0x00405AC0** | **76/76 RAW EQUAL authored exact** Laser manager list traversal with zero relocation fields; two independent six-source colds, Bomb callback10 exact guard. Real 124-byte Bomb auxiliary source restored (**128/124 NONEXACT**) linking Bullet cancel260 and Laser cancel76; parent Bomb Update220/223 remains nonexact | [Laser / Bomb cancellation proof](evidence/bomb-laser-cancel-20261102/README.md) |
| **Bomb Update @0x00405750 / callback @0x00405840** | Full **223-byte** real Update source now maintained, current native **220/223 NONEXACT**; independent **10/10 codegen exact** callback with target-bound CALL; two separate colds, old Bomb lifecycle **306/306** guards | [Bomb update/callback proof](evidence/script-viewer-bomb-update-callback-exact-20261102/README.md) |
| **ScriptViewer Bomb register/create @0x004055C0 / 0x004056B0** | **84/84 + 85/85 authored exact** in two independent colds, all 13 external fields strictly bound. Bomb destructor137 selected-cold guard; 223-byte Update and 10-byte callback remain separate. | [Bomb creator proof](evidence/script-viewer-bomb-creator-exact-20261102/README.md) |
| **Popup insertion @0x0042B9C0 / ItemManager** | Rejoined **eight original calls** and popup constructor/teardown to the single shared global **0x477840**; full 167-byte insertion implementation moved to shared Popup owner (native **171/167 NONEXACT**). 470/470 previously exact guarded bytes selected cold. | [Shared owner and field evidence](evidence/popup-shared-owner-20261102/README.md) |
| **Popup Draw @0x0042B780 / draw callback @0x0042B9B0** | Complete 540-byte Draw **source now maintained**, current PDB **537/540 nonexact** (15 native field slots); distinct 10-byte draw callback maps but differs in 3 register bytes. Three neighboring exact owners retain 275/275 in selected cold. **Zero exact credit.** | [Complete Draw source / nonexact proof](evidence/script-viewer-popup-draw-source-20261102/README.md) |
| **ScriptViewer popup Update @0x0042B6F0 and callback @0x0042B9A0** | **134/134 authored exact + 7/7 indeterminate-origin adapter exact**, seven strict fields and original active-723-record timer loop; two independent four-TU colds; constructor134 same-image guard | [Full update and callback proof](evidence/script-viewer-popup-update-exact-20261102/README.md) |
| **ScriptViewer popup constructor @0x0042B430** | **134/134 full-owner codegen exact**, one global field; two independent four-TU colds, bomb destructor137 guard; initializer145 and factory71 remain nonexact | [Popup constructor proof](evidence/script-viewer-popup-constructor-exact-20261102/README.md) |
| **ResultMenu cleanup @0x00422220** | **311/311 authored exact**, 18 strict fields and all 16 local branches; two independent selected colds; neighboring three lifecycle owners preserve 310/310 | [Complete cleanup proof](evidence/result-menu-cleanup-exact-20261102/README.md)  with [integrated 621/621 five-source double-cold](evidence/result-menu-integrated-lifecycle-20261102/README.md) |
| ResultMenu lifecycle @0x004220E0 / 0x00422150 / 0x00422360 | **101+142+67 = 310/310** exact in two-source real graph, 15 strictly bound fields; constructor origin indeterminate; two independent scoped cold links. Existing update206 and draw callback7 guarded | [Lifecycle proof](evidence/result-menu-lifecycle-exact-20261102/README.md) |
| SptTestCreate @0x0042BF30 | **67/67 authored exact**, five strictly bound calls; real constructor145 and registration84 selected-cold guard | [Full factory proof](evidence/spt-test-create-exact-20261102/README.md) |
| Keyboard high-bit reset @0x0044B080 | **75/75 authored exact**, four bound IAT/cookie fields; two independent selected cold links | [Keyboard reset proof](evidence/game-keyboard-clear-high-bits-exact-20261102/README.md) |
| Game owner creator @0x0040AF90 | **86/86 authored exact**, 5 strict fields, including both constructor/null merges and failure teardown; selected cold graph also preserves initializer134 and cleanup137 | [Creator](evidence/game-owner-creator-exact-20261102/README.md), [initializer](evidence/game-owner-40ae70-initialize-exact-20261102/README.md), [cleanup](evidence/game-owner-40af00-exact-20261102/README.md) |
| Game owner initialization @0x0040AE70 | **134/134 authored exact**, 14 strict fields, selected cold with creator86; cleanup @0x0040AF00 stays 137/137 | [Initializer](evidence/game-owner-40ae70-initialize-exact-20261102/README.md), [cleanup](evidence/game-owner-40af00-exact-20261102/README.md) |
| Game callback cleanup @0x0040AF00 | **137/137 authored exact**, 15 fields; correct final global is 0x004776FC, not the similar bomb object's 0x004776EC | [Complete game owner proof](evidence/game-owner-40af00-exact-20261102/README.md) |
| Hint automatic-entry @0x0041A120 | **220/220 authored exact**, seven fields | [Hint allocator proof](evidence/hint-automatic-entry-exact-20261102/README.md) |
| ResultMenuResume @0x00422C30 | **74/74 authored exact**, seven fields, target VM-deletion/speed scheduling | [Result menu proof](evidence/result-menu-resume-exact-20261102/README.md) |
| Stage cross product @0x00421B90 | **77/77 codegen exact** and camera330 guarded; original target entry/authoring indeterminate | [Stage proof](evidence/stage-cross-product-exact-20261102/README.md) |
| Duplicate RNG target @0x0044BA30 | **78/78 codegen exact** using existing relocation-free RNG symbol; do not invent a second C++ implementation or authored origin | [RNG proof](evidence/rng-duplicate-u32-exact-20261102/README.md) |
| BulletRuntimeView destructor @0x00405DE0 | **34/34 codegen exact**, one strict free call, origin indeterminate | [Bullet lifecycle proof](evidence/bullet-runtime-generated-destructor-exact-20261102/README.md) |
| ScriptViewer deleting destructor @0x0040A410 | **27/27 codegen exact**, two strictly bound calls, origin indeterminate | [ScriptViewer proof](evidence/script-viewer-scalar-deleting-dtor-exact-20261102/README.md) |
| ScriptViewer enemy release @0x0040D730 | **19/19 codegen exact**, two strict callee fields, origin indeterminate | [Release proof](evidence/script-viewer-release-enemies-exact-20261102/README.md) |
| ZWave GetBuffer and Get3DBuffer | **19/19** and **52/52 codegen exact**, source origin indeterminate | [GetBuffer](evidence/zwave-getbuffer-exact-20261102/README.md), [Get3D](evidence/zwave-get3d-buffer-exact-20261102/README.md) |

The Sound/ZWave duplicate GetBuffer definition was also fixed at the
source level; true Sound+ZWave compilation now links and its selected
exact Sound owners remain 1,641/1,641. This is **integration work,
not additional codegen credit**:
[producer repair](evidence/sound-zwave-single-owner-20261102/README.md).
Other earlier exact functions remain accepted according to the ledgers.

## Open complete functions: next agent's priorities

Every row below is **NONEXACT**, even when a candidate has the right length.
The numerical comparison is a particular source-epoch diagnostic; recompile
the current source before claiming an improvement.

| Owner, target extent | Last useful diagnostic and next concrete lead |
| --- | --- |
| **ANM ExecuteScript @0x0043EE30, 9,964 bytes** | Last complete native diagnostic **9,962/9,964**, 350 external fields, 95 local obligations. Only opcode91 child-create ECX-versus-EDX two-byte scheduling remains. Fresh direct call and split script/layer forms regress to 9954/9964 and 9952/9964; a volatile layer load remains 9962/9964; all strict 350 fields preserved and source restored. Sequential assignments, const record, pre-call barrier were byte-neutral in fresh whole-owner tests; temporary ID regressed and split-ID shifted fields. The CreateVmVariant2 member/stdcall ABI is verified; do not fake the registers. [Boss frontier](evidence/anm-executor-two-byte-frontier-20261031/README.md), [member ABI](evidence/anm-child91-member-stdcall-20261101/README.md) |
| **EclVmContext::Run @0x0044E1A0, 7,020 bytes** | Maintained volatile-x LENGTH_SQUARED scope gives **6,997/7,020** with all 189 fields; 23 ordinary bytes remain in x87/format trace locals. Fresh LengthPair aggregate changes the PDB owner extent and is rejected; the retained volatile-x 6997/7020 source is restored. POINT_ANGLE/format local stack homes remain open. [Large ECL proof](evidence/ecl-length-squared-stack-20261102/README.md) |
| **GameManagerCleanup @0x00417C80, 1,105 bytes** | Historical 34-input candidate **1,098/1,105**; source now includes additional exact helper definitions, so that number is **not asserted current**. Rebuild before testing flags, branch blocks and VM calls. [Previous cleanup context](evidence/gui-message-teardown-20261031/README.md) |
| **MainConfigurationDialogProc @0x0043A3A0, 233 bytes** | Current maintained barrier near WM_COMMAND gave **216/233**, 14 fields; ESI saves and return epilogue order still differ. [Command scheduling](evidence/main-dialog-command-scheduling-20261102/README.md) |
| **Sound LoadSoundSample @0x0043D390, 684 bytes** | Last source graph **678/684**, 25 address-field positions (not all original roles proven); six bytes at +0x122..+0x127 are MOV/LEA argument-order scheduling. [Sound proof](evidence/sound-sample-lifetime-20261102/README.md) |
| **Sound PreloadBgm @0x0043D7D0, 369 bytes** | Same-size candidate, **not exact**; last useful proof was **365/369** with ten address fields, remaining EAX/EDX ReadFile setup bytes. Do not use the older six-difference note as current. [Bounded proof](evidence/sound-preload-barrier-20261031/README.md) |
| **Hint ConsumeEntries @0x00419120, 1,317 bytes** | Historical **1,308/1,317**, still nonexact; ID result and script-push register scheduling. The Hint Update571 and AddAutomaticEntry220 are already exact. [Hint consumption](evidence/hint-consume-context-20261031/README.md) |
| **GUI DrawStageElements @0x00415800, 724 bytes** | Full-size owner and 33 native fields, **not exact**: local rectangle LEA/input field order still differs. [GUI draw proof](evidence/gui-draw-read-order-20261031/README.md) |
| **ANM LoadTexture / LoadTextureRegion** | Native owners **242/243** and **319/320** respectively, wrong embedded-header ADD/LEA length. Three selected existing texture guards still replay 437/437. [Texture upload](evidence/anm-texture-load-scope-20261102/README.md), [region](evidence/anm-texture-region-scope-20261102/README.md) |
| **ResultMenuMarkTasks422200 @0x00422200, 28 bytes** | Newly discovered and tracked dense 28-byte original function with RET and CC separation, origin indeterminate; real source updates both Chain flags but native PDB contribution is 26/28 and uses wrong receiver. **NONEXACT, zero credit**; existing ResultMenu lifecycle310 exact selected cold. [Boundary and source](evidence/result-menu-mark-tasks-20261102/README.md) |
| **Chain::AllocElem @0x00449ED0, 74 bytes** | Full-size 74-byte native owner recovered from original 69-byte constructor model, but strict complete comparator still rejects **22 ordinary bytes** after binding the operator-new field (52/74 bytes); ECX/EDX scheduling and callback field order unresolved. Nonexact and zero credit; GameCreator86/Initialize134 narrow cold guards pass. [Current source/native proof](evidence/chain-alloc-74-20261102/README.md) |
| **Bullet UpdateAbsoluteDirectionChange @0x004078E0, 335 bytes** | Fresh BulletHorizontalWrap context is **335 bytes with two ordinary register differences** at +0xD7/+0xDD after masking eight candidate address fields; source variants did not close EAX-versus-EDX lifetime. This is **NONEXACT** and not a canonical field proof. [Current native controls](evidence/native-frontier-controls-20261102/README.md) |
| **GUI message Run / Enemy float operands / Ending Run** | Earlier complete contexts remain nonexact, but their source pins are stale. Consult the original evidence and rebuild a chosen scope; do not repeat old scores as current facts. |

Do not waste runs on a previously rejected cosmetic permutation unless a
new source/ABI observation yields a falsifiable instruction-level prediction.
Continue substantial large-owner work alongside small exact candidates.

## Artifacts and storage after handoff cleanup

The **2026-11-02 storage cleanup** removed about **200.41 MiB** of
regenerable experimental PE/OBJ/PDB and Python bytecode. It preserved
six particularly useful nonexact/near-exact witnesses under
build/gpt-web, the entire **build/match-linked** canonical producer
cache, prior build/gpt-6.1-sol contexts, private analysis archives,
target, toolchain, Ghidra project and D3D adapter evidence.

[Cleanup receipt](evidence/handoff-storage-20261102/cleanup-summary.json)
and [artifact recovery guide](RE_ARTIFACTS.md) give exact paths and
storage policy. Deleted experimental paths must be **regenerated**; the
ignored pruned-file inventory records names and hashes, not payloads.
Historical build manifests may have mismatched source/header pins.
Never silently reuse an older compiled PE/PDB as a current-source witness.

There is no global whole-product exactness or native runnable binary.
Make new commits after substantial verified source improvements, and
leave a concise current handoff rather than appending duplicate
sections to this page. Older handoff history remains in Git.
